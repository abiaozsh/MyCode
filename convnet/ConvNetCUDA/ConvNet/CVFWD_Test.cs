using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace ConvNet
{
	public class CVFWD_Test : Test
	{

		static int in_cnt_x = 64;
		static int in_cnt_y = 64;
		static int in_cnt_d = 32;

		static int out_cnt = 32;

		static Layer.Instance initInData(ConvLayer cv)
		{
			Layer.Instance ins = cv.getInstance();
			Vol.init(ins.out_act.w, ins.out_act.w.size, 9.9f);
			return ins;
		}

		public static void Test()
		{

			var inp = new InputLayer(in_cnt_x, in_cnt_y, in_cnt_d);
			var cv1 = new ConvLayer(sx: 4, sy: 4, filters: out_cnt, stride: 1, pad: 2, adj: -1, bias_pref: 0.1f, act: new ReluLayer());
			cv1.in_layer = inp;
			cv1.init();

			Layer.Instance[] insList = new Layer.Instance[3];

			insList[0] = initInData(cv1);
			insList[1] = initInData(cv1);
			insList[2] = initInData(cv1);

			var in_act = new Vol(in_cnt_x, in_cnt_y, in_cnt_d, null);

			Console.WriteLine("start");


			Util.useGPU = false;
			Util.useSSE = false;
			test(10, "cpu", () =>
			{
				cv1.forward(insList[0], in_act);
			});

			Util.useGPU = false;
			Util.useSSE = true;
			test(10, "sse", () =>
			{
				cv1.forward(insList[1], in_act);
			});

			Util.useGPU = true;
			Util.useSSE = false;
			test(10, "gpu", () =>
			{
				cv1.forward(insList[2], in_act);
			});


			compare("out_act.w 1 ", insList[0].out_act.w, insList[1].out_act.w);
			compare("out_act.w 2 ", insList[0].out_act.w, insList[2].out_act.w);

			Console.WriteLine("ok");

		}

	}
}
