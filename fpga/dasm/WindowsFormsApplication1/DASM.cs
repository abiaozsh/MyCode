using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WindowsFormsApplication1
{
	public class DASM
	{

		public enum Instype
		{
			call = 0x00,
			jmpi = 0x01,
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

		static string[] convt = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f" };
		private static string getHex4(int val)//"ll"+"hh"
		{
			return "0x" + convt[((val & 0xF000) >> 12)] + convt[((val & 0x0F00) >> 8)] + convt[((val & 0xF0) >> 4)] + convt[((val & 0x0F))];
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

		private static string CReg(uint val)
		{/*
Register 	Name 	Register Contents
0 	status 	Refer to The status Register
1 	estatus 	Refer to The estatus Register
2 	bstatus 	Refer to The bstatus Register
3 	ienable 	Internal interrupt-enable bits

The ienable Register

Available only when the external interrupt controller interface is not present. Otherwise reserved.
4 	ipending 	Pending internal interrupt bits

The ipending Register

Available only when the external interrupt controller interface is not present. Otherwise reserved.
5 	cpuid 	Unique processor identifier
6 	Reserved 	Reserved
7 	exception 	Refer to The exception Register
8 	pteaddr 	Refer to The pteaddr Register

Available only when the MMU is present. Otherwise reserved.
9 	tlbacc 	Refer to The tlbacc Register

Available only when the MMU is present. Otherwise reserved.
10 	tlbmisc 	Refer to The tlbmisc Register

Available only when the MMU is present. Otherwise reserved.
11 	eccinj 	Refer to The eccinj Register

Available only when ECC is present.
12 	badaddr 	Refer to The badaddr Register
13 	config 	Refer to The config Register

Available only when the MPU or ECC is present. Otherwise reserved.
14 	mpubase 	Refer to The mpubase Register

Available only when the MPU is present. Otherwise reserved.
15 	mpuacc 	Refer to The mpuacc Register for MASK variations table.

Available only when the MPU is present. Otherwise reserved.*/
			if (val == 0)
			{
				return "ctl_0_status";
			}
			else if (val == 3)
			{
				return "ctl_3_ienable";
			}
			else
			{
				return "ctl_" + val;
			}
		}

		public static string decode(WindowsFormsApplication1.ELF.ProgHead phead)
		{
			byte[] data = phead.data;
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
			StringBuilder sb = new StringBuilder();
			int pos = 0;
			for (int i = 0; i < data.Length; i += 4)
			{
				uint ins = (uint)(data[i + 0]) | (uint)(data[i + 1] << 8) | (uint)(data[i + 2] << 16) | (uint)(data[i + 3] << 24);

				string sIns = "";

				Instype itype = (Instype)(ins & 0x3F);
				Rtype rtype = Rtype.none;
				if (itype == Instype.Rtype)
				{
					rtype = (Rtype)((ins >> (6 + 5)) & 0x3F);
					string A = Reg(ins >> (32 - 5));
					string B = Reg((ins >> (32 - 10)) & 0x1F);
					string C = Reg((ins >> (32 - 15)) & 0x1F);
					string CR = CReg((ins >> (6)) & 0x1F);

					sIns = rtype.ToString() + " ";
					if (rtype == Rtype.add)
					{
						sIns += C + " <- " + A + "+" + B;
					}
					else if (rtype == Rtype.jmp)
					{
						sIns += "PC <- " + A;
					}
					else if (rtype == Rtype.wrctl)
					{
						sIns += CR + " <- " + A;
					}
					else
					{
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
						sIns += B + " <- " + A + " | " + getHex4((int)IMM);
					}
					else if (itype == Instype.orhi)
					{
						sIns += B + " <- " + A + " | " + getHex4((int)IMM) + "0000";
					}
					else if (itype == Instype.addi)
					{
						sIns += B + " <- " + A + (((short)IMM) > 0 ? "+" : "") + ((short)IMM);
					}
					else if (itype == Instype.stw)
					{
						sIns += "[" + A + "+" + getHex4((int)IMM) + "] <- " + B;
					}
					else if (itype == Instype.ldw)
					{
						sIns += B + " <- [" + A + "+" + getHex4((int)IMM) + "]";
					}
					else if (itype == Instype.br)
					{
						sIns += ((short)IMM / 4) + "(" + (pos + 1 + ((short)IMM / 4)) + ")";
					}
					else if (itype == Instype.beq)
					{
						sIns += A + "==" + B + "," + ((short)IMM / 4) + "(" + (pos + 1 + ((short)IMM / 4)) + ")";
					}
					else if (itype == Instype.bltu || itype == Instype.blt)
					{
						sIns += A + "<" + B + "," + ((short)IMM / 4) + "(" + (pos + 1 + ((short)IMM / 4)) + ")";
					}
					else if (itype == Instype.call || itype == Instype.jmpi)
					{
						sIns += ((ins >> 6)) + "(" + (((ins >> 6)) - (phead.p_paddr / 4)) + ")";
					}
					else
					{
						sIns += A + "," + B + "," + getHex4((int)IMM);
					}
				}
				sb.Append(pos + " : " + sIns + "\r\n");
				pos++;
			}
			return sb.ToString();
		}
	}
}
