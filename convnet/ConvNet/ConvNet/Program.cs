using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace ConvNet
{
	class Program
	{

		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
		static extern IntPtr testsss();
		public static string test()
		{
			IntPtr ip = testsss();
			StringBuilder sb = new StringBuilder();
			unsafe
			{
				char* p;
				p = (char*)ip.ToPointer();

				for (int i = 0; i < 4096; i++)
				{
					if (p[i] == 0) break;
					sb.Append((char)p[i]);
				}
			}
			return sb.ToString();
		}

		//		void openclTest() {
		//			string fda = test();
		//
		//
		//			MyFloat f;
		//			f = MyFloat.getArray(4096);
		//			f[0] = 1.1f;
		//			f[1] = 2.2f;
		//
		//			var kernel = MyFloat.getKernel("ConvNet.cl", "test1");
		//			int[] param = new int[1];
		//			param[0] = 3;
		//			for (int i = 0; i < 10000; i++)
		//			{
		//				MyFloat.runKernel(kernel, 5, 1, param);
		//			}
		//
		//			Console.WriteLine(f[0]);
		//			Console.WriteLine(f[1]);
		//			Console.WriteLine(f[2]);
		//			Console.WriteLine(f[3]);
		//			Console.WriteLine(f[4]);
		//			Console.WriteLine(f[5]);
		//			Console.WriteLine(f[6]);
		//			Console.WriteLine(f[7]);
		//			Console.WriteLine(f[8]);
		//			Console.WriteLine(f[9]);
		//			Console.WriteLine(f[10]);
		//			Console.WriteLine(f[11]);
		//			Console.WriteLine(f[12]);
		//		}

		//static double tanh(double val)
		//{
		//	if (val > 0)
		//	{
		//		double n = Math.Exp(-val - val);
		//		return (1 - n) / 1 + n;
		//	}
		//	else
		//	{
		//		double n = Math.Exp(val + val);
		//		return -(1 - n) / 1 + n;
		//	}
		//}

		static void Main(string[] args)
		{
			//Stopwatch s = new Stopwatch();
			//s.Start();
			//double t = 0;
			//for (double i = 0; i < 100000000; i++)
			//{
			//	//t += Math.Exp(-i);
			//	//t += tanh(i);
			//	t += Math.Tanh(i);
			//}
			//s.Stop();
			//Console.WriteLine(t);
			//Console.WriteLine(s.Elapsed);
			//Console.ReadLine();

			//string p = MyFloat.getPlatforms();
			//string d0 = MyFloat.getDevices(0);
			//string d1 = MyFloat.getDevices(1);
			//string d2 = MyFloat.getDevices(2);
			//Console.Write(p);
			//Console.Write("p0:[" + d0 + "]\r\n\r\n");
			//Console.Write("p1:[" + d1 + "]\r\n\r\n");
			//Console.Write("p2:[" + d2 + "]\r\n\r\n");
			MyFloat.Init();



			//Img_reg.Proc();

			MyRnn r = new MyRnn();

			r.reinit();
			for (int i = 0; ; i++)
			{
				r.tick();
			}

			Console.ReadLine();
		}
	}
}
