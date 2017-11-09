using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using Lib;

namespace ConvNet
{
	class Program
	{

		static void Main(string[] args)
		{
			Console.WriteLine(OpenCL.getPlatforms());
            
            int Platform = 1;
            int device = 1;
            //int Platform = 0;
            //int device = 0;
            Console.WriteLine(OpenCL.getDevices(Platform));


            IntPtr openclp = OpenCL.init(Platform, device);
			OpenCL.oclobjects = openclp;
		
			//Stopwatch s = new Stopwatch();
			//s.Start();
			//double t = 0;
			//MyFloat mf = new MyFloat(100000000);
			//for (int i = 0; i < 100000000; i++)
			//{
			//	mf[i] = 1E30f;
			//}
			//
			//for (int i = 0; i < 100000000; i++)
			//{
			//	t += Math.Sqrt(mf[i]);
			//}
			//s.Stop();
			//Console.WriteLine(t);
			//Console.WriteLine(s.Elapsed);
			//Console.ReadLine();
			int cnt = 8192;

			var inp = new InputLayer(1, 1, cnt);
			var fc = new FullyConnLayer(num_neurons: cnt, bias_pref: 0.1f);
			fc.in_layer = inp;
			fc.init();

			var ins2 = fc.getInstance();

			var in_act = new Vol(1, 1, cnt, null);

			Console.WriteLine("start");


			StringBuilder sb = new StringBuilder();

			int[] idx = new int[4];
			Random r = new  Random();

			idx[0] = (int)(r.NextDouble() * cnt);
			idx[1] = (int)(r.NextDouble() * cnt);
			idx[2] = (int)(r.NextDouble() * cnt);
			idx[3] = (int)(r.NextDouble() * cnt);

            OpenCL.oclobjects = openclp;
			for (int i = 0; i < 10; i++)
			{
				Stopwatch sw = new Stopwatch();

				sw.Start();

				if (i == 5) OpenCL.oclobjects = IntPtr.Zero;

				var o = fc.forward(ins2, in_act);

				sw.Stop();

				sb.AppendLine(o.w[0] + "," + o.w[1] + "," + o.w[2] + "," + o.w[3]);
			}


			Console.WriteLine("ok");
			Console.WriteLine(sb.ToString());

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

			//MyRnn r = new MyRnn();
			//
			//r.reinit();
			//for (int i = 0; ; i++)
			//{
			//	r.tick();
			//}
			//
			//Console.ReadLine();
		}
	}
}
