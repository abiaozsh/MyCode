using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace ConvNet
{
	public class FCFWD_Test
	{

		static int cnt = 256;

		public static void Test()
		{

			var inp = new InputLayer(1, 1, cnt);
			var fc = new FullyConnLayer(num_neurons: cnt, bias_pref: 0.1f);
			fc.in_layer = inp;
			fc.init();

			Layer.Instance[] insList = new Layer.Instance[3];
			insList[0] = fc.getInstance();
			insList[1] = fc.getInstance();
			insList[2] = fc.getInstance();

			insList[0].out_act = new Vol(1, 1, cnt, 9.9f);
			insList[1].out_act = new Vol(1, 1, cnt, 9.9f);

			var in_act = new Vol(1, 1, cnt, null);

			Console.WriteLine("start");

			for (int i = 0; i < 30; i++)
			{
				long t2;
				Stopwatch sw = new Stopwatch();
				sw.Start();

				Vol o;
				string type = "";

				if (i >= 0 && i < 10)
				{
					type = "cpu";
					Util.useGPU = false;
					Util.useSSE = false;
					o = fc.forward(insList[2], in_act);
				}
				else if (i >= 10 && i < 20)
				{
					type = "sse";
					Util.useGPU = false;
					Util.useSSE = true;
					o = fc.forward(insList[1], in_act);
				}
				else
				{
					type = "gpu";
					Util.useGPU = true;
					Util.useSSE = false;
					o = fc.forward(insList[0], in_act);
				}

				sw.Stop();
				t2 = sw.ElapsedTicks;

				Console.WriteLine(type + " time:" + t2);
			}

			for (int i = 0; i < insList[0].out_act.w.size; i++)
			{
				float val0 = insList[0].out_act.w[i];
				float val1 = insList[1].out_act.w[i];
				float val2 = insList[2].out_act.w[i];

				testVal(val0, val1, i);
				testVal(val0, val2, i);
			}

			Console.WriteLine("ok");

			Console.ReadLine();
			return;

		}

		static void testVal(float val0, float val1, int i)
		{
			if (val0 != val1)
			{
				if (val0 - val1 > 0.000001)
				{
					Console.WriteLine("--err--:" + i + "[" + (val0 - val1) + "]" + "," + val0 + "---" + val1);
				}
				else if (val0 - val1 < -0.000001)
				{
					Console.WriteLine("--err-- :" + i + "[" + (val0 - val1) + "]" + "," + val0 + "---" + val1);
				}
				else
				{
					//Console.WriteLine("--ok-- :" + i + "[" + (val0 - val1) + "]" + "," + val0 + "---" + val1);
				}
			}
		}
	}
}
