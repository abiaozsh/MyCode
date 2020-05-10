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
	public enum Instype
	{
        call = 0x1A,
        jmpi = 0x0A,
		hlt = 0x02,
		ldbu = 0x03,
		addi = 0x04,
		stb = 0x05,
		br = 0x06,
		ldb = 0x07,
		cmpgei = 0x08,
		ldhu = 0x0B,
		andi = 0x0C,
		sth = 0x0D,
		bge = 0x0E,
		ldh = 0x0F,
		cmplti = 0x10,
		initda = 0x13,
		ori = 0x14,
		stw = 0x15,
		blt = 0x16,
		ldw = 0x17,
		cmpnei = 0x18,
		flushda = 0x1B,
		xori = 0x1C,
		bne = 0x1E,
		cmpeqi = 0x20,
		ldbuio = 0x23,
		muli = 0x24,
		stbio = 0x25,
		beq = 0x26,
		ldbio = 0x27,
		cmpgeui = 0x28,
		ldhuio = 0x2B,
		andhi = 0x2C,
		sthio = 0x2D,
		bgeu = 0x2E,
		ldhio = 0x2F,
		cmpltui = 0x30,
		custom = 0x32,
		initd = 0x33,
		orhi = 0x34,
		stwio = 0x35,
		bltu = 0x36,
		ldwio = 0x37,
		rdprs = 0x38,
		Rtype = 0x3A,
		flushd = 0x3B,
		xorhi = 0x3C,
	}
	public enum Rtype
	{
		none = 0x00,
		cmplt = 0x10,
		cmpeq = 0x20,
		cmpltu = 0x30,
		eret = 0x01,
		add = 0x31,
		roli = 0x02,
		slli = 0x12,
		rol = 0x03,
		sll = 0x13,
		flushp = 0x04,
		wrprs = 0x14,
		divu = 0x24,
		break_ = 0x34,
		ret = 0x05,
		div = 0x25,
		nor = 0x06,
		or = 0x16,
		rdctl = 0x26,
		sync = 0x36,
		mulxuu = 0x07,
		mulxsu = 0x17,
		mul = 0x27,
		cmpge = 0x08,
		cmpne = 0x18,
		cmpgeu = 0x28,
		bret = 0x09,
		initi = 0x29,
		sub = 0x39,
		srli = 0x1A,
		srai = 0x3A,
		ror = 0x0B,
		srl = 0x1B,
		sra = 0x3B,
		flushi = 0x0C,
		nextpc = 0x1C,
		jmp = 0x0D,
		callr = 0x1D,
		trap = 0x2D,
		and = 0x0E,
		xor = 0x1E,
		wrctl = 0x2E,
		mulxss = 0x1F,
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

		public static string dasm(List<CodeSym> syms, uint ins, int pos, int baseaddr)
		{
			StringBuilder sb = new StringBuilder();
			/*
	Register 	Name 	Function 	Register 	Name 	Function
	r0 	zero 	0x00000000 	r16 	  	Callee-saved register
	r1 	at 	Assembler temporary 	r17 	  	Callee-saved register
	r2 	  	Return value 	r18 	  	Callee-saved register
	r3 	  	Return value 	r19 	  	Callee-saved register
	r4 	  	Register arguments 	r20 	  	Callee-saved register
	r5 	  	Register arguments 	r21 	  	Callee-saved register
	r6 	  	Register arguments 	r22 	  	Callee-saved register
	r7 	  	Register arguments 	r23 	  	Callee-saved register
	r8 	  	Caller-saved register 	r24 	et 	Exception temporary
	r9 	  	Caller-saved register 	r25 	bt 	Breakpoint temporary
	r10 	  	Caller-saved register 	r26 	gp 	Global pointer
	r11 	  	Caller-saved register 	r27 	sp 	Stack pointer
	r12 	  	Caller-saved register 	r28 	fp 	Frame pointer
	r13 	  	Caller-saved register 	r29 	ea 	Exception return address
	r14 	  	Caller-saved register 	r30 	sstatus 	Status register
	r15 	  	Caller-saved register 	r31 	ra 	Return address
	Note: r25 is used exclusively by the JTAG debug module. It is used as the breakpoint temporary (bt) register in the normal register set. In shadow register sets, r25 is reserved.
	Note: r30 is used as the breakpoint return address (ba) in the normal register set, and as the shadow register set status (sstatus) in each shadow register set. For details about sstatus, refer to The Status Register section.
	For more information, refer to the Application Binary Interface chapter of the Nios® II Processor Reference Handbook.
			 */
			string hex = Util.getHex8(ins);
			string sIns = "";

			Instype itype = (Instype)(ins & 0x3F);
			Rtype rtype = Rtype.none;
			if (itype == Instype.Rtype)
			{
				rtype = (Rtype)((ins >> (6 + 5)) & 0x3F);
				string A = Reg(ins >> (32 - 5));
				string B = Reg((ins >> (32 - 10)) & 0x1F);
				string C = Reg((ins >> (32 - 15)) & 0x1F);

				sIns = rtype.ToString() + " ";
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
