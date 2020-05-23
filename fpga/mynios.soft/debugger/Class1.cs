using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace debugger
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
					cfg.cmd = Convert.ToInt32(arr[3].Trim(), 2);// int.Parse(arr[3], );
					cfgs.Add(cfg);
				}

				fs.Close();
			}
			return cfgs;
		}

		private static string Reg(uint val)
		{
			if (val == 26)
			{
				return "gp";
			}
			else if (val == 27)
			{
				return "sp";
			}
			else if (val == 28)
			{
				return "fp";
			}
			else
			{
				return "r" + val;
			}
		}

		public static string dasm(List<CodeSym> syms, List<Config> cfgs, uint ins, int pos, int baseaddr)
		{
			StringBuilder sb = new StringBuilder();

			string hex = Util.getHex8(ins);
			string sIns = "";

			uint cmd = (ins & 0x3F);

			sIns = "";

			if (cmd == 0x3F)
			{
				uint cmd3 = (ins >> (6 + 8)) & 7;
				Config cfg = null;
				foreach (var item in cfgs)
				{
					if (cmd3 + 50 == item.textformat)
					{
						cfg = item;
					}
				}
				if (cfg != null)
				{
					sIns = cfg.ToString() + " ";
				}
				else
				{
					sIns = "unknow";
				}

				string A = Reg(ins >> (32 - 5));
				string B = Reg((ins >> (32 - 10)) & 0x1F);
				string C = Reg((ins >> (32 - 15)) & 0x1F);
				uint imm8 = (ins >> (6)) & 255;
				sIns += "C:" + C + " <- (A:" + A + " ... B:" + B + ") cmd3:" + cmd3 + " imm8:" + imm8;

			}
			else
			{
				Config cfg = null;
				foreach (var item in cfgs)
				{
					if (cmd == item.cmd)
					{
						cfg = item;
					}
				}
				if (cfg != null)
				{
					sIns = cfg.scmd + " ";
				}
				else
				{
					sIns = "unknow";
				}
				if (cmd == 7) {
					long target = (((ins >> 6)) - (baseaddr / 4)) * 4;
					string sym2 = getSym((int)target, syms);
					sIns += ((ins >> 6)) + "(" + (((ins >> 6)) - (baseaddr / 4)) + ")" + sym2;
				}
				else if ((cmd & 4) == 4)
				{
					string A = Reg(ins >> (32 - 5));
					string B = Reg((ins >> (32 - 10)) & 0x1F);
					string C = Reg((ins >> (32 - 15)) & 0x1F);
					sIns += "C:" + C + " <- (A:" + A + " ... B:" + B + ")";
				}
				else
				{
					string A = Reg(ins >> (32 - 5));
					string B = Reg((ins >> (32 - 10)) & 0x1F);
					uint IMM = (ins >> (32 - 26)) & 0xFFFF;
					sIns += "B:" + B + " A:" + A + " IMM:" + Util.getHex4((int)IMM);

				}

			}
			sIns += " cmd:" + cmd;
			/*
			Rtype rtype = Rtype.none;
			if (itype == Instype.Rtype)
			{
				rtype = (Rtype)((ins >> (6 + 5)) & 0x3F);

				if (rtype == Rtype.add)
				{
					sIns += C + " <- " + A + "+" + B;
				}
				else if (rtype == Rtype.cmpeq)
				{
					sIns += C + " <- (" + A + " == " + B + " ? 1 : 0)";
				}
				else if (rtype == Rtype.cmplt)
				{
					sIns += C + " <- ((signed)" + A + " < (signed)" + B + " ? 1 : 0)";
				}
				else if (rtype == Rtype.cmpge)
				{
					sIns += C + " <- ((signed)" + A + " >= (signed)" + B + " ? 1 : 0)";
				}
				else if (rtype == Rtype.sll)
				{
					sIns += C + " <- " + A + " << " + B + "[4:0]";
				}
				else if (rtype == Rtype.callr)
				{
					sIns += "PC <- " + A;
				}
				else if (rtype == Rtype.jmp)
				{
					sIns += "PC <- " + A;
				}
				else if (rtype == Rtype.ret)
				{
					sIns += "PC <- " + A;
				}
				else
				{
					sIns += "C:" + C + " A:" + A + " B:" + B + "--------------------------------------";
				}
			}
			else
			{
				string A = Reg(ins >> (32 - 5));
				string B = Reg((ins >> (32 - 10)) & 0x1F);
				uint IMM = (ins >> (32 - 26)) & 0xFFFF;

				sIns = itype.ToString() + " ";

				if (itype == Instype.ori)
				{
					sIns += B + " <- " + A + " | " + Util.getHex4((int)IMM);
				}
				else if (itype == Instype.hlt)
				{
					sIns += IMM;
				}
				else if (itype == Instype.orhi)
				{
					sIns += B + " <- " + A + " | " + Util.getHex4((int)IMM) + "0000";
				}
				else if (itype == Instype.addi)
				{
					sIns += B + " <- " + A + (((short)IMM) > 0 ? "+" : "") + ((short)IMM);
				}
				else if (itype == Instype.andi)
				{
					sIns += B + " <- " + A + "&" + Util.getHex4((int)IMM);
				}
				else if (itype == Instype.cmplti)
				{
					sIns += B + " <- ((signed)" + A + "<" + ((short)IMM) + " ? 1 : 0 )";
				}
				else if (itype == Instype.stw || itype == Instype.stwio)
				{
					sIns += "[" + A + "+" + Util.getHex4((int)IMM) + "] <- " + B;
				}
				else if (itype == Instype.ldw || itype == Instype.ldwio)
				{
					sIns += B + " <- [" + A + "+" + Util.getHex4((int)IMM) + "]";
				}
				else if (itype == Instype.br)
				{
					sIns += ((short)IMM / 4) + "(" + (pos + 1 + ((short)IMM / 4)) + ")";
				}
				else if (itype == Instype.beq || itype == Instype.bgeu)
				{
					sIns += A + "==" + B + "," + ((short)IMM / 4) + "(" + (pos + 1 + ((short)IMM / 4)) + ")";
				}
				else if (itype == Instype.bne)
				{
					sIns += A + "!=" + B + "," + ((short)IMM / 4) + "(" + (pos + 1 + ((short)IMM / 4)) + ")";
				}
				else if (itype == Instype.bltu || itype == Instype.blt)
				{
					sIns += A + "<" + B + "," + ((short)IMM / 4) + "(" + (pos + 1 + ((short)IMM / 4)) + ")";
				}
				else if (itype == Instype.call || itype == Instype.jmpi)
				{
					long target = (((ins >> 6)) - (baseaddr / 4))*4;
					string sym2 = getSym((int)target, syms);
					sIns += ((ins >> 6)) + "(" + (((ins >> 6)) - (baseaddr / 4)) + ")"+ sym2;
				}
				else
				{
					sIns += A + "," + B + "," + Util.getHex4((int)IMM) + "--------------------------------------";
				}
			}

			*/
			string sym = getSym(pos, syms);

			sb.Append("(" + sym + ") [" + hex + "]" + " : " + sIns);

			return sb.ToString();
		}

		public static string getSym(int pos, List<CodeSym> syms)
		{
			string foundsym = "";
			if (syms != null)
			{
				foreach (var sym in syms)
				{
					if (sym.pos == pos)
					{
						foundsym += sym.name + ",";
					}
				}
			}
			return foundsym;
		}

		public static string getReg(int? val)
		{
			switch (val)
			{
				case 0: return "eax";
				case 1: return "ebx";
				case 2: return "ecx";
				case 3: return "edx";
				case 4: return "ebp";
				case 5: return "esp";
				case 6: return "esi";
				case 7: return "edi";
				case 8: return "ra";
				case 9: return "rb";
			}
			return "reg(" + val + ")";
		}
		public static string getReg8(int? val)
		{
			switch (val)
			{
				case 0: return "al";
				case 1: return "bl";
				case 2: return "cl";
				case 3: return "dl";
				case 8: return "ralowbyte";
				case 9: return "rblowbyte";
			}
			return "reg8(" + val + ")";
		}

		public string ToString()
		{
			return text;
		}
	}

}
