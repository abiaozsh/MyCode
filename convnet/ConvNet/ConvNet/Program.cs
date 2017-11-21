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
		class ConvTestNet : Net
		{
			public ConvLayer cv1;
			public FullyConnLayer fc1;
			public FullyConnLayer fc2;
			public DeConvLayer ucv1;

			public void init()
			{
				//this, new Trainer.Option() 
				trainer = new AdaDeltaTrainer() { l2_decay = 0.001f };//0.001f

				cv1 = new ConvLayer(3, 3, 4, stride: 2, pad: 1);
				fc1 = new FullyConnLayer(num_neurons: 8);
				fc2 = new FullyConnLayer(num_neurons: 3 * 3 * 4);
				ucv1 = new DeConvLayer();

				Add(new InputLayer(6, 6, 2));
				Add(cv1);
				Add(new ReshapeLayer(1, 1, cv1.out_sx * cv1.out_sy * cv1.out_depth));
				Add(fc1);
				Add(fc2);
				Add(new ReshapeLayer(cv1.out_sx, cv1.out_sy, cv1.out_depth));
				Add(ucv1);

				Util.load("conv.txt", (s) =>
				{
					cv1.load(s);
					fc1.load(s);
					fc2.load(s);
					ucv1.load(s);
				});

			}

		}


		static void CONV_FC_Test() {

			Vol inp = new Vol(6, 6, 2, 0);

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

			ConvTestNet net = new ConvTestNet();
			net.init();
			var ins = net.getInstance();

			

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

		static void Main(string[] args)
		{

			//FCFWD_Test.Test();
			//FCBWD_Test.Test();
			//CVFWD_Test.Test();
			//CVBWD_Test.Test();

		}
	}
}
