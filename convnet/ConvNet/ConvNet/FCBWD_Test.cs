using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace ConvNet
{
	public class FCBWD_Test : Test
	{

		static int in_cnt = 256;
		static int out_cnt = 256;
		static Layer.Instance initInData(FullyConnLayer fc)
		{
			Layer.Instance ins = fc.getInstance();
			ins.in_act = new Vol(1, 1, in_cnt, null);
			Vol.init(ins.out_act.w, 1 * 1 * out_cnt, 9.9f);
			return ins;
		}

		public static void Test()
		{

			var inp = new InputLayer(1, 1, in_cnt);
			var fc = new FullyConnLayer(num_neurons: out_cnt, bias_pref: 0.1f);
			fc.in_layer = inp;
			fc.init();

			Layer.Instance[] insList = new Layer.Instance[2];

			insList[0] = initInData(fc);
			insList[1] = initInData(fc);

			var in_act = new Vol(1, 1, in_cnt, null);
			var out_act = new Vol(1, 1, out_cnt, null);
			Vol.init(out_act.dw, 1 * 1 * out_cnt, null);

			insList[0].in_act.w = in_act.w;
			insList[1].in_act.w = in_act.w;

			insList[0].out_act.dw = out_act.dw;
			insList[1].out_act.dw = out_act.dw;

			//ins.in_act.w.getHostMemPointReadOnly(), input
			//filters_w.getHostMemPointReadOnly(),    input
			//ins.out_act.dw.getHostMemPointReadOnly(), input

			//ins.in_act.dw.getHostMemPointWriteOnly(), output
			//trainableIns.filters_dw.getHostMemPointWriteOnly(), input / output
			//trainableIns.bias_dw.getHostMemPointWriteOnly()); input / output

			Console.WriteLine("start");

			Util.useSSE = false;
			test(10, "cpu", () =>
			{
				fc.backward(insList[0]);
			});

			Util.useSSE = true;
			test(10, "sse", () =>
			{
				fc.backward(insList[1]);
			});

			compare("in_act.dw 1 ", insList[0].in_act.dw, insList[1].in_act.dw);

			compare("filters_dw 1 ", ((TrainableLayer.TrainableInstance)insList[0]).filters_dw, ((TrainableLayer.TrainableInstance)insList[1]).filters_dw);

			compare("bias_dw 1 ", ((TrainableLayer.TrainableInstance)insList[0]).bias_dw, ((TrainableLayer.TrainableInstance)insList[1]).bias_dw);

			Console.WriteLine("ok");


		}

	}
}
