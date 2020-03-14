using System;

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
	}
	public class Config
	{
		public string regex;
		public int format;
        public int insformat;
        public int cmd;
		public string ToString()
		{
			return regex;
		}
	}
	public class Ins
	{
		public string line;
		public string name;
		public string sym;
        public int format;
		public int code;
		public int data;

		public int bitcmd;//[30:24]8bit:cmd
		public int bitreg1;//[23:20]4bit:reg1
		public int bitreg2;//[19:15]4bit:reg2
		public int bitins1;//[15:0]16bit:ins1
        public int bitins2;
		public int pos;
	}
	public static string regCmd = @"[a-zA-Z]+";
	public static string regSym = @"[_a-zA-Z0-9]+";
	public static string regIns = @"[\+\-0-9]+";
	public static string regReg = @"(eax|ebx|ecx|edx|ebp|esp|esi|edi|ra|rb)";

	public static int getReg(string reg)
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
		
		throw new Exception("reg not found");
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
		throw new Exception("wrong reg");
	}


	public static int check(int? val)
	{
		if (val == null)
		{
			throw new Exception();
		}
		return val.Value;
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
