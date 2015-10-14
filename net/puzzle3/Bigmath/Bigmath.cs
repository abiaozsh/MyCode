using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace BigMathLib
{
	public class BigMath
	{
		[DllImport("BigMathDll.dll")]
		private static extern void N_Add(Int64[] Data, Int64[] Length, Int64[] Idx, Int64[] result);
		public static void Add(Int64[] Data, Int64[] Length, Int64[] Idx, Int64[] result)
		{

			N_Add( Data,  Length,  Idx,  result);
		}
	}
}