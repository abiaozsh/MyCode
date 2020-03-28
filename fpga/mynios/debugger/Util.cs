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
            return convt[((val >> 12) & 0x0F)] + convt[((val >> 8) & 0x0F)] + convt[((val >> 4) & 0x0F)] + convt[((val >> 0) & 0x0F)];
		}
		public static string getHex2(int val)//"ll"+"hh"
		{
            return convt[((val >> 4) & 0x0F)] + convt[((val >> 0) & 0x0F)];
		}
		public static string getHex8(uint val)//"ll"+"hh"
		{
			return convt[((val >> 28) & 0x0F)] + convt[((val >> 24) & 0x0F)] + convt[((val >> 20) & 0x0F)] + convt[((val >> 16) & 0x0F)] + convt[((val >> 12) & 0x0F)] + convt[((val >> 8) & 0x0F)] + convt[((val >> 4) & 0x0F)] + convt[((val >> 0) & 0x0F)];
		}



	}
}
