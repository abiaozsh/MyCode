using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace ConvNet
{
	public class CVBWD_Test : Test
	{

		static int in_cnt_x = 55;
		static int in_cnt_y = 55;
		static int in_cnt_d = 8;

		static int out_cnt = 16;

		static Layer.Instance initInData(ConvLayer cv)
		{
			Layer.Instance ins = cv.getInstance();
			ins.in_act = new Vol(in_cnt_x, in_cnt_y, in_cnt_d, null);
			Vol.init(ins.out_act.w, ins.out_act.w.size, 9.9f);
			return ins;
		}

		public static void Test()
		{

			var inp = new InputLayer(in_cnt_x, in_cnt_y, in_cnt_d);
			var cv = new ConvLayer(sx: 4, sy: 4, filters: out_cnt, stride: 2, pad: 2, adj: 0, bias_pref: 0.1f);//, act: new ReluLayer()
			cv.in_layer = inp;
			cv.init();

			Layer.Instance[] insList = new Layer.Instance[2];

			insList[0] = initInData(cv);
			insList[1] = initInData(cv);

			var in_act = new Vol(in_cnt_x, in_cnt_y, in_cnt_d, null);
			var out_act = new Vol(cv.out_sx, cv.out_sy, out_cnt, null);
			Vol.init(out_act.dw, cv.out_sx * cv.out_sy * out_cnt, null);

			insList[0].in_act.w = in_act.w;
			insList[1].in_act.w = in_act.w;

			insList[0].out_act.dw = out_act.dw;
			insList[1].out_act.dw = out_act.dw;

			Console.WriteLine("start");


			Util.useSSE = false;
			test(10, "cpu", () =>
			{
				cv.backward(insList[0]);
			});

			Util.useSSE = true;
			test(10, "sse", () =>
			{
				cv.backward(insList[1]);
			});


			compare("in_act.dw 1 ", insList[0].in_act.dw, insList[1].in_act.dw);

			compare("filters_dw 1 ", ((TrainableLayer.TrainableInstance)insList[0]).filters_dw, ((TrainableLayer.TrainableInstance)insList[1]).filters_dw);

			compare("bias_dw 1 ", ((TrainableLayer.TrainableInstance)insList[0]).bias_dw, ((TrainableLayer.TrainableInstance)insList[1]).bias_dw);

			Console.WriteLine("ok");

		}

	}
}
