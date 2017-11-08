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

		static void Main(string[] args)
		{
			Stopwatch s = new Stopwatch();
			s.Start();
			double t = 0;
			MyFloat mf = new MyFloat(100000000);
			for (int i = 0; i < 100000000; i++)
			{
				mf[i] = 1E30f;
			}

			for (int i = 0; i < 100000000; i++)
			{
				t += Math.Sqrt(mf[i]);
			}
			s.Stop();
			Console.WriteLine(t);
			Console.WriteLine(s.Elapsed);
			Console.ReadLine();

			return;
			//string p = MyFloat.getPlatforms();
			//string d0 = MyFloat.getDevices(0);
			//string d1 = MyFloat.getDevices(1);
			//string d2 = MyFloat.getDevices(2);
			//Console.Write(p);
			//Console.Write("p0:[" + d0 + "]\r\n\r\n");
			//Console.Write("p1:[" + d1 + "]\r\n\r\n");
			//Console.Write("p2:[" + d2 + "]\r\n\r\n");
			//MyFloat.Init();



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
