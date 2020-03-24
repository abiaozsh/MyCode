using System;
using System.Collections.Generic;
using System.IO;

public class Base
{
	public class CodeSym
	{
		public string name;
		public int pos;
	}
	public class DataSym
	{
		public string name;
		public int pos;
		public string sym;
	}

	public class Line
	{
		public enum LineType
		{
			cmd = 1,// mov 指令
			sym = 2,// _xxx: 符号
			dot = 3,// .xxx 
			app = 4,// /APP
			noapp = 5,// /NO_APP
			sharp = 6,// # 7 "d.c" 1
		}
		public bool isCodeSeg()
		{
			return ".text" == text.Trim();
		}
		public bool isDataSeg()
		{
			return text.Trim().StartsWith(".data") || text.Trim().StartsWith(".section");
		}
		public LineType type;
		public string text;
		public string ToString()
		{
			return text;
		}
		public string sym;
		public string cmd;
		public Op op1;
		public Op op2;
		public static Line match(string line)
		{
			Line l = new Line();
			l.text = line;
			if (line.StartsWith("."))
			{
				l.type = LineType.dot;
			}
			else if (line.StartsWith("/APP"))
			{
				l.type = LineType.app;
			}
			else if (line.StartsWith("/NO_APP"))
			{
				l.type = LineType.noapp;
			}
			else if (line.StartsWith("#"))
			{
				l.type = LineType.noapp;
			}
			else if (line.EndsWith(":"))
			{
				l.type = LineType.sym;
				l.sym = line.TrimEnd(':');
			}
			else
			{
				string[] temparr;
				try
				{
					l.type = LineType.cmd;
					int pos1 = line.IndexOf(" ");
					if (pos1 == -1)
					{
						l.cmd = line;
						temparr = new string[0];
					}
					else
					{
						l.cmd = line.Substring(0, pos1);
						string temp = line.Substring(pos1);
						temparr = temp.Split(',');
					}
				}
				catch (Exception ex)
				{
					throw new Exception("err" + line);
				}
				if (temparr.Length == 2)
				{
					l.op1 = Op.match(temparr[0].Trim());
					l.op2 = Op.match(temparr[1].Trim());
				}
				else if (temparr.Length == 1)
				{
					l.op1 = Op.match(temparr[0].Trim());
				}
				else if (temparr.Length == 0)
				{
				}
				else
				{
					throw new Exception("err temparr.Length" + line);
				}
			}
			return l;
		}
	}

	public class Op
	{
		public enum OpType
		{
			reg = 1,// rax 寄存器
			reg8 = 2,// rax 寄存器
			ins = 3,// 123 立即数
			sym = 4,//_xxx 符号引用
			dsym = 5,//OFFSET FLAT:_gbuff
			DWPTRregoff = 6,//DWORD PTR [esp+12] ,DWORD PTR [esp]
			DWPTRdsym = 7,//DWORD PTR _aaa
			BYTEPTRregoff = 8,//BYTE PTR [eax+12] ,BYTE PTR [eax]
			BYTEPTRdsym = 9,//BYTE PTR _a2
			addr = 10,//[eax+12]
			//OFFSET FLAT
		}
		public OpType type;
		public int? reg;
		public int? off;
		public int? ins;
		public string sym;
		public string dsym;

		public string text;

		public static string matchReg(string s)
		{
			string word = "";
			for (int i = 0; i < s.Length; i++)
			{
				if (s[i] >= 'a' && s[i] <= 'z' || s[i] >= 'A' && s[i] <= 'Z')
				{
					word += s[i];
				}
				else
				{
					break;
				}
			}
			return word;
		}
		public static string matchSym(string s, out string s2)
		{
			string word = "";
			for (int i = 0; i < s.Length; i++)
			{
				bool match = false;
				if (i == 0)
				{
					if (s[i] == '_' || s[i] >= 'a' && s[i] <= 'z' || s[i] >= 'A' && s[i] <= 'Z')
					{
						match = true;
					}
				}
				else
				{
					if (s[i] == '_' || s[i] >= 'a' && s[i] <= 'z' || s[i] >= 'A' && s[i] <= 'Z' || s[i] >= '0' && s[i] <= '9')
					{
						match = true;
					}
				}
				if (match)
				{
					word += s[i];
				}
				else
				{
					break;
				}
			}
			s2 = s.Substring(word.Length);
			return word;
		}

		public static Op match(string sop)
		{
			Op op = new Op();
			op.text = sop;
			int tempreg = getReg(sop, true);
			int itemp;
			if (tempreg >= 0)
			{
				op.type = OpType.reg;
				op.reg = tempreg;
				return op;
			}
			int tempreg8 = getReg8(sop, true);
			if (tempreg8 >= 0)
			{
				op.type = OpType.reg8;
				op.reg = tempreg8;
				return op;
			}

			if (int.TryParse(sop, out itemp))
			{
				op.type = OpType.ins;
				op.ins = itemp;
				return op;
			}

			if (sop.StartsWith("DWORD PTR "))
			{
				sop = sop.Substring("DWORD PTR ".Length);
				if (sop.StartsWith("["))
				{
					op.type = OpType.DWPTRregoff;
					procaddr(ref sop, ref op);
					return op;
				}
				else
				{
					string temp = matchSym(sop, out sop);
					if (sop.Length == 0)
					{
						op.type = OpType.DWPTRdsym;
						op.dsym = temp;
						return op;
					}
					else
					{
						throw new Exception("");
					}
				}
			}
			if (sop.StartsWith("BYTE PTR "))
			{
				sop = sop.Substring("BYTE PTR ".Length);
				if (sop.StartsWith("["))
				{
					op.type = OpType.BYTEPTRregoff;
					procaddr(ref sop, ref op);
					return op;
				}
				else
				{
					string temp = matchSym(sop, out sop);
					if (sop.Length == 0)
					{
						op.type = OpType.BYTEPTRdsym;
						op.dsym = temp;
						return op;
					}
					else
					{
						throw new Exception("");
					}
				}
			}
			if (sop.StartsWith("["))
			{
				op.type = OpType.addr;
				procaddr(ref sop, ref op);
				return op;
			}
			if (sop.StartsWith("OFFSET FLAT:"))
			{
				sop = sop.Substring("OFFSET FLAT:".Length);
				string temp = matchSym(sop, out sop);
				if (sop.Length == 0)
				{
					op.type = OpType.dsym;
					op.dsym = temp;
					return op;
				}
				else
				{
					throw new Exception("");
				}
			}

			string temp2 = matchSym(sop, out sop);
			if (sop.Length == 0)
			{
				op.type = OpType.sym;
				op.sym = temp2;
				return op;
			}

			throw new Exception("unknown op");
		}

		private static void procaddr(ref string sop, ref Op op)
		{
			sop = sop.Substring(1);
			string word = matchReg(sop);
			op.reg = getReg(word, true);
			sop = sop.Substring(word.Length);
			if (sop.EndsWith("]"))
			{
				sop = sop.TrimEnd(']');
				if (sop.Length == 0)
				{
					op.ins = 0;
				}
				else
				{
					op.ins = int.Parse(sop);
				}
			}
			else
			{
				throw new Exception("unknow3 op:" + sop);
			}

		}

	}



	public class Config
	{
		public string text;
		public string scmd;
		public string op1;
		public string op2;
		public int textformat;
		public int insformat;
		public int cmd;
		public Config(string text)
		{
			this.text = text;
			string[] temparr;
			int pos1 = text.IndexOf(" ");
			if (pos1 == -1)
			{
				scmd = text;
				temparr = new string[0];
			}
			else
			{
				scmd = text.Substring(0, pos1);
				string temp = text.Substring(pos1);
				temparr = temp.Split(',');
			}
			if (temparr.Length == 2)
			{
				op1 = temparr[0].Trim();
				op2 = temparr[1].Trim();
			}
			else if (temparr.Length == 1)
			{
				op1 = temparr[0].Trim();
			}
		}
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

		public bool match(Line line)
		{
			if (this.scmd != line.cmd)
			{
				return false;
			}
			if (this.op1 != null)
			{
				if (line.op1 == null)
				{
					return false;
				}
				if (this.op1 != line.op1.type.ToString())
				{
					return false;
				}
			}
			else
			{
				if (line.op1 != null)
				{
					return false;
				}
			}

			if (this.op2 != null)
			{
				if (line.op2 == null)
				{
					return false;
				}
				if (this.op2 != line.op2.type.ToString())
				{
					return false;
				}
			}
			else
			{
				if (line.op2 != null)
				{
					return false;
				}
			}

			return true;
		}
		public string ToString()
		{
			return text;
		}
	}
	public class Ins
	{
		public Line line;
		public string name;
		public string sym;
		public string dsym;
		public int format;

		public int bitcmd;//[30:24]8bit:cmd
		public int bitreg1;//[23:20]4bit:reg1
		public int bitreg2;//[19:15]4bit:reg2
		public int bitins1;//[15:0]16bit:ins1
		public int bitins2;
		public int pos;
	}

	public class Data
	{
		public byte[] data;
		public string sym;
		public int len;
	}

	public static int align4(int val)
	{
		if ((val & 0x03) != 0)
		{
			return (int)((val & 0xFFFFFFFC) + 4);
		}
		return val;
	}

	public static int getReg(string reg, bool noerr)
	{
		if (reg == "eax")//assign eax = regfile[0];
		{
			return 0;
		}
		else if (reg == "ebx")//assign ebx = regfile[1];
		{
			return 1;
		}
		else if (reg == "ecx")//assign ecx = regfile[2];
		{
			return 2;
		}
		else if (reg == "edx")//assign edx = regfile[3];
		{
			return 3;
		}
		else if (reg == "ebp")//assign ebp = regfile[4];
		{
			return 4;
		}
		else if (reg == "esp")//assign esp = regfile[5];
		{
			return 5;
		}
		else if (reg == "esi")//assign esi = regfile[6];
		{
			return 6;
		}
		else if (reg == "edi")//assign edi = regfile[7];
		{
			return 7;
		}
		else if (reg == "ra")//assign ra = regfile[8];
		{
			return 8;
		}
		else if (reg == "rb")//assign rb = regfile[9];
		{
			return 9;
		}
		if (noerr)
		{
			return -1;
		}
		else
		{
			throw new Exception("reg not found");
		}
	}

	public static int getReg8(string reg, bool noerr)
	{
		if (reg == "al")
		{
			return 0;
		}
		else if (reg == "bl")
		{
			return 1;
		}
		else if (reg == "cl")
		{
			return 2;
		}
		else if (reg == "dl")
		{
			return 3;
		}
		else if (reg == "ralowbyte")//assign ra = regfile[8];
		{
			return 8;
		}
		else if (reg == "rblowbyte")//assign rb = regfile[9];
		{
			return 9;
		}

		if (noerr)
		{
			return -1;
		}
		else
		{
			throw new Exception("reg not found");
		}
	}


	public static string check(string val)
	{
		if (val == null)
		{
			throw new Exception();
		}
		return val;
	}



	static string[] convt = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f" };
	public static string getHex4(int val, ref int sum)//"ll"+"hh"
	{
		sum += ((val >> 8) & 0x0FF) + ((val >> 0) & 0x0FF);
		return convt[((val >> 12) & 0x0F)] + convt[((val >> 8) & 0x0F)] + convt[((val >> 4) & 0x0F)] + convt[((val >> 0) & 0x0F)];
	}
	public static string getHex8(int val, ref int sum)//"ll"+"hh"
	{
		sum += ((val >> 24) & 0x0FF) + ((val >> 16) & 0x0FF) + ((val >> 8) & 0x0FF) + ((val >> 0) & 0x0FF);
		return convt[((val >> 28) & 0x0F)] + convt[((val >> 24) & 0x0F)] + convt[((val >> 20) & 0x0F)] + convt[((val >> 16) & 0x0F)] + convt[((val >> 12) & 0x0F)] + convt[((val >> 8) & 0x0F)] + convt[((val >> 4) & 0x0F)] + convt[((val >> 0) & 0x0F)];
	}

	public static string getHex2(int val)
	{
		return convt[((val & 0xF0) >> 4)] + convt[((val & 0x0F))];
	}

}
