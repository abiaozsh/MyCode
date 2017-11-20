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
		public class MainNet : Net
		{
			public FullyConnLayer fca;
			public FullyConnLayer fcb;

			public void init()
			{
				//this, new Trainer.Option() 
				AdaDeltaTrainer trainer = new AdaDeltaTrainer() { l2_decay = 0.1f };//0.001f
				trainer.ro = 0.99f;
				trainer.eps = 1e-8f;

				this.trainer = trainer;

				fca = new FullyConnLayer(num_neurons: 5, bias_pref: 0.1f, act: new ReluLayer());
				fcb = new FullyConnLayer(num_neurons: 4, bias_pref: 0.1f);

				Add(new InputLayer(out_sx: 1, out_sy: 1, out_depth: 4));
				Add(fca);
				Add(fcb);
				Add(new RegressionLayer());

				Util.load("test.txt", (s) =>
				{
					fca.load(s);
					fcb.load(s);
				});

			}


			public void train(Net.Instance ins, Vol data)
			{
				DataSet ds = new DataSet();
				//train
				ds.data = data;

				train(ins, data, ds);

			}

			public Vol test(Net.Instance ins, Vol data)
			{
				return this.forward(ins, data);
			}
		}
		/*

			MainNet net = new MainNet();
			net.init();

			var ins = net.getInstance();

			Vol vola = new Vol(1, 1, 4, 0);
			vola.w[0] = 1.0f;
			vola.w[1] = 3.0f;
			vola.w[2] = 1.0f;
			vola.w[3] = 3.0f;


			Vol volb = new Vol(1, 1, 4, 0);
			volb.w[0] = 2.0f;
			volb.w[1] = 1.0f;
			volb.w[2] = 2.0f;
			volb.w[3] = 1.0f;


			Vol volc = new Vol(1, 1, 4, 0);
			volc.w[0] = 0.0f;
			volc.w[1] = 0.5f;
			volc.w[2] = 1.0f;
			volc.w[3] = 1.5f;

			for (int j = 0; j < 1; j++)
			{
				//for (int i = 0; i < 1000; i++)
				//{
				//	net.train(ins, vola);
				//	net.train(ins, volb);
				//	net.train(ins, volc);
				//	net.endofBatch(new Net.Instance[] { ins }, 3);
				//}

				Vol v;
				v = net.forward(ins, vola);
				Console.Write(v.w[0] + ",");
				Console.Write(v.w[1] + ",");
				Console.Write(v.w[2] + ",");
				Console.Write(v.w[3] + ",");
				Console.WriteLine();

				v = net.forward(ins, volb);
				Console.Write("    ");
				Console.Write(v.w[0] + ",");
				Console.Write(v.w[1] + ",");
				Console.Write(v.w[2] + ",");
				Console.Write(v.w[3] + ",");
				Console.WriteLine();

				v = net.forward(ins, volc);
				Console.Write("        ");
				Console.Write(v.w[0] + ",");
				Console.Write(v.w[1] + ",");
				Console.Write(v.w[2] + ",");
				Console.Write(v.w[3] + ",");
				Console.WriteLine();
			}

			//Util.save("test2.txt", (s) =>
			//{
			//	net.fca.save(s);
			//	net.fcb.save(s);
			//});
		 
		 */
		static void Main(string[] args)
		{

			//FCFWD_Test.Test();
			//FCBWD_Test.Test();
			//CVFWD_Test.Test();
			//CVBWD_Test.Test();

			Vol inp = new Vol(6, 6, 2, 0);

			InputLayer inlayer = new InputLayer(6, 6, 2);

			for (int i = 0; i < 6; i++)
			{
				for (int j = 0; j < 6; j++)
				{
					//inp.set(j, i, 0, (i+1) * 6 + j+1);
					//inp.set(j, i, 1, (i+1) * 6 + j+1 + 0.5f);
					inp.w[i * 6 * 2 + j * 2 + 0] = (i + 1) * 6 + j + 1;
					inp.w[i * 6 * 2 + j * 2 + 1] = (i + 1) * 6 + j + 1 + 0.5f;
				}
			}

			ReshapeLayer reshape2 = new ReshapeLayer(1, 1, inlayer.out_sx * inlayer.out_sy * inlayer.out_depth);
			reshape2.in_layer = inlayer;
			reshape2.init();
			var resIns2 = reshape2.getInstance();

			ConvLayer cv = new ConvLayer(3, 3, 4, 1, pad: 1, bias_pref: 0.1f);//, act: new ReluLayer()
			cv.in_layer = inlayer;
			cv.init();
			var ins = cv.getInstance();

			ReshapeLayer reshape = new ReshapeLayer(1, 1, cv.out_sx * cv.out_sy * cv.out_depth);
			reshape.in_layer = cv;
			reshape.init();
			var resIns = reshape.getInstance();

			FullyConnLayer fc = new FullyConnLayer(8);
			fc.in_layer = reshape;
			fc.init();
			var fcins = fc.getInstance();

			Util.load("conv.txt", (s) =>
			{
				cv.load(s);
				fc.load(s);
			});

			Vol vres2 = reshape2.forward(resIns2, inp);

			for (int i = 0; i < vres2.depth; i++)
			{
				Console.WriteLine(vres2.get(0, 0, i) + "\t");
			}
			Console.WriteLine();

			Vol vcv = cv.forward(ins, inp);
			Vol vres = reshape.forward(resIns, vcv);
			Vol _out = fc.forward(fcins, vres);

			for (int i = 0; i < vres.depth; i++)
			{
				Console.WriteLine(vres.get(0, 0, i) + "\t");
			}
			Console.WriteLine();
			for (int i = 0; i < 8; i++)
			{
				Console.WriteLine(_out.get(0, 0, i) + "\t");
			}

			Console.ReadLine();
		}
	}
}
