using System;
using System.Collections.Generic;
using System.Text;
using System.IO.Ports;
using System.IO;
using System.Globalization;
using System.Threading;
using System.Text.RegularExpressions;

namespace Assembler
{
	class Program : Base
	{
		public static List<Config> loadConfig(string path)
		{
			List<Config> cfgs = new List<Config>();
			{
				FileStream fs = new FileStream(path, FileMode.Open, FileAccess.Read);
				StreamReader sr = new StreamReader(fs);
				var temp = sr.ReadToEnd().Split('\n');
				foreach (var txt in temp)
				{
					var item = txt.Trim();
					if (item.Length == 0 || item.StartsWith("#"))
					{
						continue;
					}
					var arr = item.Split('@');
					Config cfg = new Config(arr[0].Trim());
					cfg.textformat = int.Parse(arr[1]);
					cfg.insformat = int.Parse(arr[2]);
					cfg.cmd = int.Parse(arr[3]);
					cfgs.Add(cfg);
				}

				fs.Close();
			}
			return cfgs;
		}

		static void Main(string[] args)
		{

			string filein;
			string fileout;
			string filetemp;
			if (args.Length > 0 && !String.IsNullOrEmpty(args[0]))
			{
				filein = args[0];
			}
			else
			{
				filein = "a.s";
			}
			if (args.Length > 1 && !String.IsNullOrEmpty(args[1]))
			{
				fileout = args[1];
			}
			else
			{
				fileout = "a.hex";
			}
			if (args.Length > 2 && !String.IsNullOrEmpty(args[2]))
			{
				filetemp = args[2];
			}
			else
			{
				filetemp = "a.temp.s";
			}
			Console.WriteLine(filein);
			Console.WriteLine(fileout);
			Console.WriteLine(filetemp);

			try
			{
				compile(filein, fileout, filetemp);
			}
			catch (Exception ex)
			{
				Console.WriteLine(ex);
			}

			Console.ReadLine();
		}
		public static void compile(string filein, string fileout, string filetemp)
		{
			List<Config> cfgs = loadConfig(@"assembler\config.txt");
			string[] linesraw = null;
			{
				FileStream fs = new FileStream(filein, FileMode.Open, FileAccess.Read);
				StreamReader sr = new StreamReader(fs);
				var temp = sr.ReadToEnd().Replace('\t', ' ');
				while (true)
				{
					if (temp.Contains("  "))
					{
						temp = temp.Replace("  ", " ");
					}
					else
					{
						break;
					}
				}
				linesraw = temp.Split('\n');
				fs.Close();
			}

			List<Line> lines = new List<Line>();

			bool standalone = true;
			if (standalone)
			{
				lines.Add(Line.match("jmp _main"));
			}
			else
			{
				lines.Add(Line.match("call _main"));
				lines.Add(Line.match("ret"));
			}

			foreach (var raw in linesraw)
			{
				if (raw.Trim().Length > 0)
				{
					Line line = Line.match(raw.Trim());
					if (line == null)
					{
						throw new Exception("unknow line:" + raw);
					}
					lines.Add(line);
				}
			}

			List<Line> linespass2 = new List<Line>();
			foreach (var line in lines)
			{

				if ((new Config("leave")).match(line))
				{
					//leave :   Set ESP to EBP, then pop EBP
					linespass2.Add(Line.match("mov ebp, esp"));
					linespass2.Add(Line.match("pop ebp"));
				}
				else if ((new Config("add DWPTRregoff, ins")).match(line))
				{
					linespass2.Add(Line.match("mov rb, " + line.op1.text));
					linespass2.Add(Line.match("mov ra, " + line.op2.text));
					linespass2.Add(Line.match("add rb, ra"));
					linespass2.Add(Line.match("mov " + line.op1.text + ", rb"));
				}
				else if ((new Config("cmp reg, DWPTRregoff")).match(line))
				{
					linespass2.Add(Line.match("mov ra, " + line.op2.text + ""));
					linespass2.Add(Line.match("cmp " + line.op1.text + ", ra"));
				}
				else if ((new Config("mov DWPTRregoff, ins")).match(line))
				{
					linespass2.Add(Line.match("mov ra, " + line.op2.text));
					linespass2.Add(Line.match("mov " + line.op1.text + ", ra"));
				}
				else if ((new Config("add reg, ins")).match(line))
				{
					linespass2.Add(Line.match("mov ra, " + line.op2.text));
					linespass2.Add(Line.match("add " + line.op1.text + ", ra"));
				}
				else if ((new Config("and reg, ins")).match(line))
				{
					linespass2.Add(Line.match("mov ra, " + line.op2.text));
					linespass2.Add(Line.match("and " + line.op1.text + ", ra"));
				}
				else if ((new Config("sub reg, ins")).match(line))
				{
					linespass2.Add(Line.match("mov ra, " + line.op2.text));
					linespass2.Add(Line.match("sub " + line.op1.text + ", ra"));
				}

				else
				{
					linespass2.Add(line);
				}
			}
			lines = linespass2;

			Line last = null;
			foreach (var line in lines)
			{

				if (line.type == Line.LineType.cmd && !line.cmd.StartsWith("jmp") && line.cmd.StartsWith("j"))
				{
					if (last.cmd != ("cmp"))
					{
						throw new Exception();
					}
				}
				last = line;
			}

			{
				FileStream fs2 = new FileStream(filetemp, FileMode.Create, FileAccess.Write);
				StreamWriter sw = new StreamWriter(fs2);
				foreach (var item in lines)
				{
					sw.WriteLine(item.text);
				}
				sw.Flush();
				fs2.Close();
			}

			List<DataSym> datasyms = new List<DataSym>();
			int pos = 0;


			List<Ins> insList = new List<Ins>();
			pos = 0;
			List<CodeSym> codeSyms = new List<CodeSym>();
			{
				foreach (var item in lines)
				{
					var line = item;

					if (line.type == Line.LineType.sym)
					{
						CodeSym codeSym = new CodeSym();
						codeSym.name = line.sym;
						codeSym.pos = pos;
						codeSyms.Add(codeSym);
					}
					else if (line.type == Line.LineType.cmd)
					{
						Ins ins = new Ins();
						Config matchCfg = null;
						foreach (var cfg in cfgs)
						{
							if (cfg.match(line))
							{
								matchCfg = cfg;
								break;
							}
						}
						if (matchCfg == null)
						{
							throw new Exception("unknow cmd:" + line.text);
						}
						else
						{
							try
							{
								switch (matchCfg.textformat)
								{
									case 0://nop
										break;
									case 1://mov reg, ins
										ins.bitreg1 = line.op1.reg.Value;
										ins.bitins2 = line.op2.ins.Value;
										break;
									case 2://mov eax, DWORD PTR [esp+28]
										ins.bitreg1 = line.op1.reg.Value;
										ins.bitreg2 = line.op2.reg.Value;
										ins.bitins1 = line.op2.ins.Value;
										break;
									case 3://mov DWORD PTR [esp+28], eax
										ins.bitreg1 = line.op1.reg.Value;
										ins.bitreg2 = line.op2.reg.Value;
										ins.bitins1 = line.op1.ins.Value;
										break;
									case 4://push	ebp
										ins.bitreg1 = line.op1.reg.Value;
										break;
									case 5://mov	ebp, esp
										ins.bitreg1 = line.op1.reg.Value;
										ins.bitreg2 = line.op2.reg.Value;
										break;
									case 6:
										break;
									case 7://call ___main
										ins.sym = check(line.op1.sym);
										break;
								}
							}
							catch (Exception ex)
							{
								Console.WriteLine(ex.ToString());
								throw new Exception("error:" + line.text);
							}
							ins.line = line;
							ins.format = matchCfg.insformat;
							ins.bitcmd = matchCfg.cmd;
							ins.pos = pos;
							insList.Add(ins);
							if (matchCfg.insformat == 2)
							{
								pos += 8;
							}
							else
							{
								pos += 4;
							}
						}
					}
				}
			}

			foreach (var ins in insList)
			{
				if (ins.sym != null)
				{
					CodeSym found = null;
					foreach (var sym in codeSyms)
					{
						if (ins.sym == sym.name)
						{
							found = sym; break;
						}
					}
					if (found == null)
					{
						throw new Exception("not found:" + ins.sym);
					}
					ins.bitins1 = found.pos - ins.pos;
				}
			}

			{
				int entry = 0;
				int len = pos;
				//FileStream fs = new FileStream(@"..\..\..\a.bin", FileMode.Create, FileAccess.Write);
				FileStream fs2 = new FileStream(fileout, FileMode.Create, FileAccess.Write);

				//BinaryWriter bw = new BinaryWriter(fs);
				StreamWriter sw = new StreamWriter(fs2);
				//bw.Write(len);
				//bw.Write(entry);

				int posx = 0;
				foreach (var ins in insList)
				{
					//[31]1bit:4 byte/8 byte
					//[30:24]7bit:cmd
					//[23:20]4bit:reg1
					//[19:16]4bit:reg2
					//[15:0]16bit:ins1
					//[31:0]32bit:ins2

					if (ins.bitins1 > 30000 || ins.bitins1 < -30000)
					{
						throw new Exception();
					}

					if (ins.format == 0)
					{
						int insbin = (ins.bitcmd << 24) | (ins.bitreg1 << 20) | (ins.bitreg2 << 16) | (ins.bitins1 & 0x0000FFFF);
						//bw.Write(insbin);
						writehex(insbin, posx++, sw);
					}
					else if (ins.format == 1)
					{
						int insbin = (ins.bitcmd << 24) | (ins.bitreg1 << 20) | (ins.bitreg2 << 16) | (ins.bitins1 & 0x00FFFFFF);
						//bw.Write(insbin);
						writehex(insbin, posx++, sw);
					}
					else if (ins.format == 2)
					{
						int insbin = (ins.bitcmd << 24) | (ins.bitreg1 << 20) | (ins.bitreg2 << 16) | (ins.bitins1 & 0x0000FFFF);

						//bw.Write(insbin);
						writehex(insbin, posx++, sw);

						if ((insbin & 0x80000000) != 0)
						{
							//bw.Write(ins.bitins2);
							writehex(ins.bitins2, posx++, sw);
						}
					}
				}

				sw.WriteLine(":00000001FF");

				sw.Flush();
				fs2.Close();

				//bw.Flush();
				//fs.Close();

			}
			//Console.ReadLine();
		}


		public static void writehex(int data, int posx, StreamWriter sw)
		{
			//:04 0000 00 0302 0100 51
			int sum = 4;
			sw.Write(":04" + getHex4(posx, ref sum) + "00" + getHex8(data, ref sum));
			sw.Write(getHex2(0x100 - (sum & 0xFF)));
			sw.WriteLine();
		}
	}
}
