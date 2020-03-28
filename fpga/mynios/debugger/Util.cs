using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace debugger
{
	class Util
	{
		static string[] convt = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f" };
		public static string getHex4(int val)//"ll"+"hh"
		{
			return convt[((val & 0xF0) >> 4)] + convt[((val & 0x0F))] + convt[((val & 0xF000) >> 12)] + convt[((val & 0x0F00) >> 8)];
		}
		public static string getHex2(int val)//"ll"+"hh"
		{
			return convt[((val & 0xF0) >> 4)] + convt[((val & 0x0F))];
		}
		public static string getHex8(uint val)//"ll"+"hh"
		{
			return convt[((val >> 28) & 0x0F)] + convt[((val >> 24) & 0x0F)] + convt[((val >> 20) & 0x0F)] + convt[((val >> 16) & 0x0F)] + convt[((val >> 12) & 0x0F)] + convt[((val >> 8) & 0x0F)] + convt[((val >> 4) & 0x0F)] + convt[((val >> 0) & 0x0F)];
		}
		public string getHex(byte[] buff, int idx, int len)//"ll"+"hh"
		{
			StringBuilder sb = new StringBuilder();
			for (int i = idx; i < idx + len; i++)
			{
				byte val = buff[i];
				sb.Append(convt[((val & 0xF0) >> 4)] + convt[((val & 0x0F))]);
			}
			return sb.ToString();
		}




	}
}
