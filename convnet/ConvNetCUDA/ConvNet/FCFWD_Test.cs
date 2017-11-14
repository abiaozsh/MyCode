using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace ConvNet
{
	public class FCFWD_Test : Test
	{

		static int in_cnt = 256;
		static int out_cnt = 256;

		static Layer.Instance initInData(FullyConnLayer fc)
		{
			Layer.Instance ins = fc.getInstance();
			Vol.init(ins.out_act.w, 1 * 1 * out_cnt, 9.9f);
			return ins;
		}

		public static void Test()
		{

			var inp = new InputLayer(1, 1, in_cnt);
			var fc = new FullyConnLayer(num_neurons: out_cnt, bias_pref: 0.1f);
			fc.in_layer = inp;
			fc.init();

			Layer.Instance[] insList = new Layer.Instance[3];

			insList[0] = initInData(fc);
			insList[1] = initInData(fc);
			insList[2] = initInData(fc);

			var in_act = new Vol(1, 1, in_cnt, null);

			Console.WriteLine("start");


			Util.useGPU = false;
			Util.useSSE = false;
			test(10, "cpu", () =>
			{
				fc.forward(insList[0], in_act);
			});

			Util.useGPU = false;
			Util.useSSE = true;
			test(10, "sse", () =>
			{
				fc.forward(insList[1], in_act);
			});

			Util.useGPU = true;
			Util.useSSE = false;
			test(10, "gpu", () =>
			{
				fc.forward(insList[2], in_act);
			});


			compare("out_act.w 1 ", insList[0].out_act.w, insList[1].out_act.w);
			compare("out_act.w 2 ", insList[0].out_act.w, insList[2].out_act.w);

			Console.WriteLine("ok");

		}

	}
}
