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

		static void Main(string[] args)
		{

			//FCFWD_Test.Test();
			//FCBWD_Test.Test();
			//CVFWD_Test.Test();
			//CVBWD_Test.Test();


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

			for (int j = 0; j < 1000; j++)
			{
				for (int i = 0; i < 1000; i++)
				{
					net.train(ins, vola);
					net.train(ins, volb);
					net.train(ins, volc);
					net.endofBatch(new Net.Instance[] { ins }, 3);
				}

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


			Console.ReadLine();

		}
	}
}
