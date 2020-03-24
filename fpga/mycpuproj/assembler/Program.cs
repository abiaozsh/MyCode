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

		static void Main(string[] args)
		{
			string filein;
			string fileout;
			string filetemp;
			string test = "b";
			if (args.Length > 0 && !String.IsNullOrEmpty(args[0]))
			{
				filein = args[0];
			}
			else
			{
				filein = test + ".s";
			}
			if (args.Length > 1 && !String.IsNullOrEmpty(args[1]))
			{
				fileout = args[1];
			}
			else
			{
				fileout = test + ".hex";
			}
			if (args.Length > 2 && !String.IsNullOrEmpty(args[2]))
			{
				filetemp = args[2];
			}
			else
			{
				filetemp = test + ".temp.s";
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
			List<Config> cfgs = Config.loadConfig(@"assembler\config.txt");
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

			lines.Add(Line.match(".text"));
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

				if ((new Config("nop")).match(line))
				{
					linespass2.Add(Line.match("hlt 0"));
				}
				else if ((new Config("leave")).match(line))
				{
					//leave :   Set ESP to EBP, then pop EBP
					linespass2.Add(Line.match("mov esp, ebp"));
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
				else if ((new Config("add reg, DWPTRregoff")).match(line))//add eax, DWORD PTR [ebp+8]
				{
					linespass2.Add(Line.match("mov ra, " + line.op2.text + ""));
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
				else if ((new Config("movsx reg, BYTEPTRregoff")).match(line))
				{
					linespass2.Add(Line.match("movzx ra, " + line.op2.text));
					linespass2.Add(Line.match("movsx " + line.op1.text + ", ralowbyte"));
				}
				else if ((new Config("cmp BYTEPTRregoff, ins")).match(line))
				{
					linespass2.Add(Line.match("movzx ra, " + line.op1.text));
					linespass2.Add(Line.match("mov rb, " + line.op2.text));
					linespass2.Add(Line.match("cmp ralowbyte, rblowbyte"));
				}
				else if ((new Config("mov BYTEPTRregoff, ins")).match(line))
				{
					linespass2.Add(Line.match("mov ra, " + line.op2.text));
					linespass2.Add(Line.match("mov " + line.op1.text + ", ralowbyte"));
				}
				else if ((new Config("mov DWPTRregoff, dsym")).match(line))//mov DWORD PTR [esp], OFFSET FLAT:_gbuff
				{
					linespass2.Add(Line.match("mov ra, " + line.op2.text));
					linespass2.Add(Line.match("mov " + line.op1.text + ", ra"));
				}
				else if ((new Config("mov DWPTRdsym, dsym")).match(line))//mov DWORD PTR _tempstr, OFFSET FLAT:LC0
				{
					linespass2.Add(Line.match("mov ra, " + line.op1.text));//addr
					linespass2.Add(Line.match("mov rb, " + line.op2.text));//val
					linespass2.Add(Line.match("mov DWORD PTR [ra], rb"));
				}
				else if ((new Config("movzx reg, BYTEPTRdsym")).match(line))//movzx eax, BYTE PTR _a2
				{
					linespass2.Add(Line.match("mov ra, " + line.op2.text));//addr
					linespass2.Add(Line.match("mov rb, DWORD PTR [ra]"));
					linespass2.Add(Line.match("movzx " + line.op1.text + ", rblowbyte"));//val
				}
				else if ((new Config("mov reg, DWPTRdsym")).match(line))//mov	eax, DWORD PTR _bb
				{
					linespass2.Add(Line.match("mov ra, " + line.op2.text));//addr
					linespass2.Add(Line.match("mov " + line.op1.text + ", DWORD PTR [ra]"));
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
					if (last.cmd != "cmp" && last.cmd != "test")
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

			int pos;
			bool iscode;


			List<Ins> insList = new List<Ins>();
			pos = 0;
			iscode = false;
			List<CodeSym> codeSyms = new List<CodeSym>();
			{
				foreach (var item in lines)
				{
					var line = item;
					if (line.type == Line.LineType.dot)
					{
						if (line.isCodeSeg())
						{
							iscode = true;
						}
						if (line.isDataSeg())
						{
							iscode = false;
						}
					}
					if (!iscode)
					{
						continue;
					}
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
									case 0://ret
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
										ins.bitreg1 = line.op2.reg.Value;
										ins.bitreg2 = line.op1.reg.Value;
										ins.bitins1 = line.op1.ins.Value;
										break;
									case 8://mov BYTE PTR [eax], dl
										ins.bitreg1 = line.op2.reg.Value;
										ins.bitreg2 = line.op1.reg.Value;
										ins.bitins1 = line.op1.ins.Value;
										break;
									case 4://push	ebp
										ins.bitreg1 = line.op1.reg.Value;
										break;
									case 5://mov	ebp, esp
										ins.bitreg1 = line.op1.reg.Value;
										ins.bitreg2 = line.op2.reg.Value;
										break;
									case 6://movzx	ebp, al
										ins.bitreg1 = line.op1.reg.Value;
										ins.bitreg2 = line.op2.reg.Value;
										break;
									case 7://call ___main
										ins.sym = check(line.op1.sym);
										break;
									case 9://lea eax, [ebp+12]
										ins.bitreg1 = line.op1.reg.Value;
										ins.bitreg2 = line.op2.reg.Value;
										ins.bitins1 = line.op2.ins.Value;
										break;
									case 10://cmp al, bl
										ins.bitreg1 = line.op1.reg.Value;
										ins.bitreg2 = line.op2.reg.Value;
										break;
									case 11://movzx eax, BYTE PTR [esp+28]
										ins.bitreg1 = line.op1.reg.Value;
										ins.bitreg2 = line.op2.reg.Value;
										ins.bitins1 = line.op2.ins.Value;
										break;
									case 12://mov reg, dsym                @          12 @          2 @ 128 @ mov eax,123
										ins.bitreg1 = line.op1.reg.Value;
										ins.dsym = line.op2.dsym;
										break;
									case 13://mov reg, DWPTRdsym           @          13 @          2 @ 128 @ mov eax,123
										ins.bitreg1 = line.op1.reg.Value;
										ins.dsym = line.op2.dsym;
										break;
									case 14://hlt ins
										ins.bitins1 = line.op1.ins.Value;
										break;
									case 15://mov reg, BYTEPTRdsym         @          15 @          2 @ 128 @ mov eax,123
										ins.bitreg1 = line.op1.reg.Value;
										ins.dsym = line.op2.dsym;
										break;


									default:
										throw new Exception("unknown text format");
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

			List<Data> dataList = new List<Data>();
			List<DataSym> dataSyms = new List<DataSym>();
			iscode = true;
			{
				foreach (var item in lines)
				{
					var line = item;
					if (line.type == Line.LineType.dot)
					{
						if (line.isCodeSeg())
						{
							iscode = true;
						}
						if (line.isDataSeg())
						{
							iscode = false;
						}
					}
					if (iscode && !line.text.Trim().StartsWith(".comm"))
					{
						continue;
					}

					//.comm	_gbuff, 64	 # 51
					//.comm	_tempstr, 16	 # 4
					if (line.text.Trim().StartsWith(".comm "))
					{
						string txt = line.text.Trim().Substring(".comm ".Length);
						int idx = txt.IndexOf(",");
						DataSym dataSym = new DataSym();
						dataSym.name = txt.Substring(0, idx);
						dataSym.pos = pos;
						dataSyms.Add(dataSym);

						idx = txt.IndexOf('#');
						int len = align4(int.Parse(txt.Substring(idx + 1).Trim()));
						Data data = new Data();
						data.data = new byte[len];
						data.len = len;
						dataList.Add(data);
						pos += len;
					}
					else
					{
						if (line.type == Line.LineType.sym)
						{
							DataSym dataSym = new DataSym();
							dataSym.name = line.sym;
							dataSym.pos = pos;
							dataSyms.Add(dataSym);
						}

						if (line.text.StartsWith(".ascii \""))
						{
							Data data = new Data();
							string txt = line.text.Substring(".ascii \"".Length, line.text.Length - 1 - ".ascii \"".Length);
							txt = System.Text.RegularExpressions.Regex.Unescape(txt);
							data.data = System.Text.Encoding.Default.GetBytes(txt);
							dataList.Add(data);
							data.len = align4(data.data.Length);
							pos += data.len;
						}
						if (line.text.StartsWith(".long"))
						{
							Data data = new Data();
							string txt = line.text.Substring(".long ".Length);
							int val;
							if (int.TryParse(txt, out val))
							{
								byte[] src = new byte[4];
								src[3] = (byte)((val >> 24) & 0xFF);
								src[2] = (byte)((val >> 16) & 0xFF);
								src[1] = (byte)((val >> 8) & 0xFF);//高8位
								src[0] = (byte)(val & 0xFF);//低位
								data.data = src;
							}
							else
							{
								data.sym = txt;
							}
							data.len = 4;
							pos += data.len;
							dataList.Add(data);

						}
						if (line.text.StartsWith(".byte"))
						{
							Data data = new Data();
							string txt = line.text.Substring(".byte ".Length);
							int val;
							if (int.TryParse(txt, out val))
							{
								byte[] src = new byte[4];
								src[3] = 0;
								src[2] = 0;
								src[1] = 0;//高8位
								src[0] = (byte)(val & 0xFF);//低位
								data.data = src;
							}
							else
							{
								throw new Exception("byte:" + txt);
							}
							data.len = 4;
							pos += data.len;
							dataList.Add(data);
						}
						//word??
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
				if (ins.dsym != null)
				{
					DataSym found = null;
					foreach (var dsym in dataSyms)
					{
						if (ins.dsym == dsym.name)
						{
							found = dsym; break;
						}
					}
					if (found == null)
					{
						throw new Exception("not found:" + ins.dsym);
					}
					ins.bitins2 = found.pos;
				}
			}

			foreach (var data in dataList)
			{
				if (data.sym != null)
				{
					CodeSym found = null;
					int val = 0;
					foreach (var sym in codeSyms)
					{
						if (data.sym == sym.name)
						{
							found = sym;
							val = found.pos;
							break;
						}
					}
					DataSym found2 = null;
					foreach (var dsym in dataSyms)
					{
						if (data.sym == dsym.name)
						{
							found2 = dsym;
							val = found2.pos;
							break;
						}
					}
					if (found == null && found2 == null)
					{
						throw new Exception("not found:" + data.sym);
					}
					byte[] src = new byte[4];
					src[3] = (byte)((val >> 24) & 0xFF);
					src[2] = (byte)((val >> 16) & 0xFF);
					src[1] = (byte)((val >> 8) & 0xFF);//高8位
					src[0] = (byte)(val & 0xFF);//低位
					data.data = src;
				}
			}

			{
				FileStream fs2 = new FileStream("temp.sym", FileMode.Create, FileAccess.Write);
				StreamWriter sw = new StreamWriter(fs2);
				foreach (var sym in codeSyms)
				{
					sw.WriteLine(sym.pos + "," + sym.name);
				}
				foreach (var sym in dataSyms)
				{
					sw.WriteLine(sym.pos + "," + sym.name);
				}
				sw.Flush();
				fs2.Close();
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
				foreach (var data in dataList)
				{
					for (int i = 0; i < data.len; i += 4)
					{
						int val = 0;
						if (i + 0 < data.data.Length)
						{
							val |= (data.data[i + 0]);
						}
						if (i + 1 < data.data.Length)
						{
							val |= (data.data[i + 1] << 8);
						}
						if (i + 2 < data.data.Length)
						{
							val |= (data.data[i + 2] << 16);
						}
						if (i + 3 < data.data.Length)
						{
							val |= (data.data[i + 3] << 24);
						}
						//bw.Write(ins.bitins2);
						writehex(val, posx++, sw);
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
