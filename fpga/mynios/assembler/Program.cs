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
			string test = "c";
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

			int basePos;

			List<Line> lines = new List<Line>();

			lines.Add(Line.match(".section .text"));
			bool standalone = true;
			if (standalone)
			{
				//0200C000
				lines.Add(Line.match("movhi sp, %hiadj(33603584)"));
				lines.Add(Line.match("addi sp, sp, %lo(33603584)"));
				lines.Add(Line.match("jmpi main"));
				basePos = 0x02000000;
			}
			else
			{
				basePos = 0x00000000;
				lines.Add(Line.match("call main"));
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
				//bgt rA, rB, label blt rB, rA, label
				//bgtu rA, rB, label bltu rB, rA, label
				//ble rA, rB, label bge rB, rA, label
				//bleu rA, rB, label bgeu rB, rA, label
				//cmpgt rC, rA, rB cmplt rC, rB, rA
				//cmpgti rB, rA, IMMED cmpgei rB, rA, (IMMED+1)
				//cmpgtu rC, rA, rB cmpltu rC, rB, rA
				//cmpgtui rB, rA, IMMED cmpgeui rB, rA, (IMMED+1)
				//cmple rC, rA, rB cmpge rC, rB, rA
				//cmplei rB, rA, IMMED cmplti rB, rA, (IMMED+1)
				//cmpleu rC, rA, rB cmpgeu rC, rB, rA
				//cmpleui rB, rA, IMMED cmpltui rB, rA, (IMMED+1)

				//movia rB, label orhi rB, r0, %hiadj(label)
				//addi, rB, r0, %lo(label)
				//movui rB, IMMED ori rB, r0, IMMED
				//subi rB, rA, IMMED addi rB, rA, (-IMMED)

				if ((new Config("nop")).match(line))//nop add r0, r0, r0
				{
					linespass2.Add(Line.match("add r0, r0, r0"));
				}
				else if ((new Config("movi reg, ins")).match(line))//movi rB, IMMED addi, rB, r0, IMMED
				{
					linespass2.Add(Line.match("addi " + line.op1.text + ", r0, " + line.op2.text));
				}
				else if ((new Config("mov reg, reg")).match(line))//mov rC, rA add rC, rA, r0
				{
					linespass2.Add(Line.match("add " + line.op1.text + ", " + line.op2.text + ", r0"));
				}
				else if ((new Config("movhi reg, ins")).match(line))//movhi rB, IMMED orhi rB, r0, IMMED
				{
					linespass2.Add(Line.match("orhi " + line.op1.text + ", r0, " + line.op2.text));
				}
				else if ((new Config("stw reg, gprel")).match(line))//stw r2, %gprel(Sd2Card_status_)(gp)
				{
					//%gprel(immed32) Replace the immed32 address with an offset from the global pointer immed32 –_gp
					//movhi	r1, %hiadj(c)
					linespass2.Add(Line.match("orhi r1, r0, %hiadj(" + line.op2.sym + ")"));
					//addi	r1, r1, %lo(c)
					linespass2.Add(Line.match("addi r1, r1, %lo(" + line.op2.sym + ")"));
					//ldw	r4, 0(r1)
					linespass2.Add(Line.match("stw " + line.op1.text + ", 0(r1)"));
				}
				else if ((new Config("ldw reg, gprel")).match(line))//ldw r4, %gprel(c)(gp)
				{
					//%gprel(immed32) Replace the immed32 address with an offset from the global pointer immed32 –_gp
					//movhi	r1, %hiadj(c)
					linespass2.Add(Line.match("orhi r1, r0, %hiadj(" + line.op2.sym + ")"));
					//addi	r1, r1, %lo(c)
					linespass2.Add(Line.match("addi r1, r1, %lo(" + line.op2.sym + ")"));
					//ldw	r4, 0(r1)
					linespass2.Add(Line.match("ldw " + line.op1.text + ", 0(r1)"));
				}
				else
				{
					linespass2.Add(line);
				}
			}
			lines = linespass2;


			int pos;
			bool iscode;


			List<Ins> insList = new List<Ins>();
			pos = basePos;
			iscode = false;
			List<Sym> syms = new List<Sym>();
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
						Sym codeSym = new Sym();
						codeSym.name = line.sym;
						codeSym.pos = pos;
						syms.Add(codeSym);
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
									case 0://xxx
										break;
									case 1://ret
										ins.bitregA = 31;
										break;
									case 5://hlt ins
										ins.IMM16 = line.op1.ins.Value;
										break;
									case 10://andhi rB, rA, IMM16
										ins.bitregB = line.op1.reg.Value;
										ins.bitregA = line.op2.reg.Value;
										ins.op = line.op3;
										break;
									case 11://stw rB, byte_offset(rA)
										ins.bitregB = line.op1.reg.Value;
										ins.bitregA = line.op2.reg.Value;
										ins.IMM16 = line.op2.ins.Value;
										break;
									case 15://bne rA, rB, label
										ins.bitregA = line.op1.reg.Value;
										ins.bitregB = line.op2.reg.Value;
										ins.op = line.op3;
										break;
									case 20://jmpi sym
										ins.op = line.op1;
										break;
									case 21://call sym
										ins.op = line.op1;
										break;
									case 22://br sym
										ins.op = line.op1;
										break;
									case 30://add rC, rA, rB
										ins.bitregC = line.op1.reg.Value;
										ins.bitregA = line.op2.reg.Value;
										ins.bitregB = line.op3.reg.Value;
										break;

									case 40://slli rC, rA, IMM5
										ins.bitregC = line.op1.reg.Value;
										ins.bitregA = line.op2.reg.Value;
										ins.IMM5 = line.op3.ins.Value;
										break;


									default:
										throw new Exception("unknown text format:" + matchCfg.textformat);
								}
							}
							catch (Exception ex)
							{
								Console.WriteLine(ex.ToString());
								throw new Exception("error:" + line.text);
							}
							ins.line = line;
							ins.txtformat = matchCfg.textformat;
							ins.format = matchCfg.insformat;
							ins.bitcmd = matchCfg.cmd;
							ins.pos = pos;
							insList.Add(ins);
							pos += 4;
						}
					}
				}
			}

			List<Data> dataList = new List<Data>();
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
						string[] arr = txt.Split(',');
						Sym dataSym = new Sym();
						dataSym.name = arr[0];
						dataSym.pos = pos;
						syms.Add(dataSym);

						int len = int.Parse(arr[1]);
						Data data = new Data(line);
						data.data = new byte[len];
						data.len = len;
						dataList.Add(data);
						pos += len;
					}
					else
					{
						if (line.type == Line.LineType.sym)
						{
							Sym dataSym = new Sym();
							dataSym.name = line.sym;
							dataSym.pos = pos;
							syms.Add(dataSym);
						}

						if (line.text.StartsWith(".string \""))
						{
							Data data = new Data(line);
							string txt = line.text.Substring(".string \"".Length, line.text.Length - 1 - ".string \"".Length) + "\0";
							txt = System.Text.RegularExpressions.Regex.Unescape(txt);
							data.data = System.Text.Encoding.Default.GetBytes(txt);
							dataList.Add(data);
							data.len = Util.align4(data.data.Length);
							pos += data.len;
						}
						if (line.text.StartsWith(".long"))
						{
							Data data = new Data(line);
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
						if (line.text == ".zero 4")
						{
							Data data = new Data(line);
							byte[] src = new byte[4];
							data.data = src;
							data.len = 4;
							pos += data.len;
							dataList.Add(data);
						}

						if (line.text.StartsWith(".byte"))
						{
							Data data = new Data(line);
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
						if (line.text.StartsWith(".short"))
						{
							Data data = new Data(line);
							string txt = line.text.Substring(".short ".Length);
							int val;
							if (int.TryParse(txt, out val))
							{
								byte[] src = new byte[4];
								src[3] = 0;
								src[2] = 0;
								src[1] = (byte)((val >> 8) & 0xFF);//高8位
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
				if (ins.line.cmd == "br")
				{
					int a = 1;
				}
				if (ins.op != null)
				{
					if (ins.op.sym != null)
					{
						Sym found = null;
						foreach (var sym in syms)
						{
							if (ins.op.sym == sym.name)
							{
								found = sym; break;
							}
						}
						if (found == null)
						{
							throw new Exception("not found:" + ins.op.sym);
						}
						if (ins.format == 0)
						{
							if (ins.txtformat == 15 || ins.txtformat == 20)
							{
								ins.IMM16 = found.pos - ins.pos - 4;
								if (ins.IMM16 > 30000 || ins.IMM16 < -30000)
								{
									throw new Exception();
								}
							}
							else if (ins.txtformat == 10)
							{
								ins.IMM16 = procAdj(ins.op, found.pos);
								if (ins.IMM16 > 30000 || ins.IMM16 < -30000)
								{
									throw new Exception();
								}
							}
							else
							{
								throw new Exception();
							}
						}
						else if (ins.format == 2)
						{
							if (found.pos > 35000000)
							{
								throw new Exception();
							}
							ins.IMM26 = found.pos >> 2;
						}
					}
					else
					{
						ins.IMM16 = procAdj(ins.op, ins.op.ins.Value);
						if (ins.IMM16 > 70000 || ins.IMM16 < -70000)
						{
							throw new Exception();
						}
					}
				}

			}

			foreach (var data in dataList)
			{
				if (data.sym != null)
				{
					Sym found = null;
					int val = 0;
					foreach (var sym in syms)
					{
						if (data.sym == sym.name)
						{
							found = sym;
							val = found.pos;
							break;
						}
					}
					if (found == null)
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
				foreach (var sym in syms)
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
				FileStream fs3 = new FileStream(filetemp, FileMode.Create, FileAccess.Write);

				//BinaryWriter bw = new BinaryWriter(fs);
				StreamWriter sw = new StreamWriter(fs2);
				StreamWriter sw3 = new StreamWriter(fs3);
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
					int insbin = 0;
					if (ins.format == 0)//andi reg, reg, ins
					{
						insbin = (ins.bitregA << (32 - 5)) | (ins.bitregB << (32 - 10)) | ((ins.IMM16 & 0x0000FFFF) << (6)) | (ins.bitcmd);
					}
					else if (ins.format == 1)
					{
						insbin = (ins.bitregA << (32 - 5)) | (ins.bitregB << (32 - 10)) | (ins.bitregC << (32 - 15)) | (ins.bitcmd << (6 + 5)) | (ins.IMM5 << (6)) | (0x3A);
					}
					else if (ins.format == 2)
					{
						insbin = (ins.IMM26 << 6) | (ins.bitcmd);
					}
					//bw.Write(insbin);
					writehex(insbin, posx++, sw);

					sw3.WriteLine(((posx - 1) * 4) + " [" + getHex8(insbin) + "] " + ins.line.text);

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
						sw3.WriteLine(((posx - 1) * 4) + " [" + getHex8(val) + "] ");
					}
				}

				sw.WriteLine(":00000001FF");

				sw.Flush();
				fs2.Close();

				sw3.Flush();
				fs3.Close();

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
