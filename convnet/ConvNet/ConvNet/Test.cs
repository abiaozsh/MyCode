using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace ConvNet
{
	public class Test
	{
		public static long test(int time, string type, Action act)
		{
			long sum = 0;
			for (int i = 0; i < time; i++)
			{
				long t2;
				Stopwatch sw = new Stopwatch();
				sw.Start();
				act();
				sw.Stop();
				t2 = sw.ElapsedTicks;
				Console.WriteLine(type + " time:" + t2);
				if (i > 2)
				{
					sum += t2;
				}
			}
			Console.WriteLine(type + " sum time:" + sum);
			return sum;
		}
		public static void compare(string thing, MyFloat a, MyFloat b)
		{
			if (a == b)
			{
				throw new Exception("same point");
			}
			bool hadok = false;
			int errcount = 0;
			for (int i = 0; i < a.size; i++)
			{
				float val0 = a[i];
				float val1 = b[i];

				if (val0 != val1)
				{
					if (val0 - val1 > 0.000001 || val0 - val1 < -0.000001)
					{
						if (errcount < 10)
						{
							Console.WriteLine(thing + "--err--:" + i + "[" + (val0 - val1) + "]" + "," + val0 + "---" + val1);
							errcount++;
						}
					}
					else
					{
						if (!hadok)
						{
							Console.WriteLine(thing + "--ok-- :" + i + "[" + (val0 - val1) + "]" + "," + val0 + "---" + val1);
							hadok = true;
						}
					}
				}
			}
		}

	}
}
