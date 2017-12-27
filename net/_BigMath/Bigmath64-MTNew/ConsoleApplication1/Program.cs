using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using BigMathLib;
using System.Runtime.InteropServices;

namespace ConsoleApplication1
{
	class Program
	{
		[DllImport("kernel32.dll")]
		public static extern uint GetTickCount();

		static void Main(string[] args)
		{
			testPi();

			//Console.Read();
		}
		public static void testPi()
		{
			//,31,0,0,0, 0,15,16,109, 672,4718,33625,239093
			//,15,0,0,0, 0, 0,47,313,2282,19125
			//,15,0,0,0, 0,15,32,140, 953,6781
			// ,0,0,0,0,15,16,15,141, 546,3760

			//,0,0,0,0, 15,16,94,608,4102,28923
			BigMath b = new BigMath(2, "3.141592653589793238462");
			Console.WriteLine();
			for (int i = 0; i < 10; i++)
			{
				BigMath t = new BigMath(b.Precision * 2, 0);
				b.CopyTo(t);
				b = t.clone();

				uint start = GetTickCount();
				b.Sin();
				//b.Sin_slow();
				Console.Write("," + (GetTickCount() - start));

				t.Add(b);
				b = t;
				System.IO.StreamWriter sw = new System.IO.StreamWriter("d:\\pi\\a" + i + ".txt");
				sw.WriteLine(b.toString(-1));
				sw.Close();
			}
		}


		public static void testPiMT()
		{
			//,31,0,0,0, 0,15,16,109, 672,4718,33625,239093
			//,15,0,0,0, 0, 0,47,313,2282,19125
			//,15,0,0,0, 0,15,32,140, 953,6781
			// ,0,0,0,0,15,16,15,141, 546,3760
			//,0,0,0,0, 15,16,94,608,4102,28923
			BigMath b = new BigMath(2, "3.141592653589793238462");
			Console.WriteLine();
			for (int i = 0; i < 10; i++)
			{
				BigMath t = new BigMath(b.Precision * 2, 0);
				b.CopyTo(t);
				b = t.clone();

				uint start = GetTickCount();
				if (i < 9)
				{
					b.Sin();
					//b.SinMT(4);
				}
				else
				{
					b.SinMT(4);
				}
				//b.Sin_slow();
				Console.Write("," + (GetTickCount() - start));
				t.Add(b);
				b = t;
				System.IO.StreamWriter sw = new System.IO.StreamWriter("d:\\pi\\b" + i + ".txt");
				sw.WriteLine(b.toString(-1));
				sw.Close();
			}
		}


	}
}
