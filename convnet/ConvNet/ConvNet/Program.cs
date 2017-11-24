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

				cv1 = new ConvLayer(filterSize: 3, out_depth: 4, stride: 2, padding: true);
				fc1 = new FullyConnLayer(num_neurons: 8);
				fc2 = new FullyConnLayer(num_neurons: 3 * 3 * 4);
				ucv1 = new DeConvLayer(filterSize: 3, out_sx: 6, out_sy: 6, out_depth: 2, stride: 2, pad: 0);

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
			public void test()
			{

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

				var ins = getInstance();

				Vol vres2 = forward(ins, inp);

				Console.WriteLine(print(ins.list[1].in_act));
				Console.WriteLine(print(ins.list[1].out_act));
				Console.WriteLine(print(ins.list[3].out_act));
				Console.WriteLine(print(ins.list[4].out_act));
				Console.WriteLine(print(ins.list[6].out_act));


				Console.ReadLine();
			}

		}

		class ConvTestNet2 : Net
		{
			public ConvLayer cv1;
			public FullyConnLayer fc1;
			public FullyConnLayer fc2;
			public DeConvLayer ucv1;

			public void init()
			{

				cv1 = new ConvLayer(filterSize: 3, out_depth: 4, stride: 1, padding: true);
				fc1 = new FullyConnLayer(num_neurons: 8);
				fc2 = new FullyConnLayer(num_neurons: 3 * 3 * 4);
				ucv1 = new DeConvLayer(filterSize: 3, out_sx: 6, out_sy: 6, out_depth: 2, stride: 2, pad: 0);

				Add(new InputLayer(6, 6, 2));
				Add(cv1);
				Add(new PoolLayer(stride: 2));
				Add(new ReshapeLayer(1, 1, 3 * 3 * 4));
				Add(fc1);
				Add(fc2);
				Add(new ReshapeLayer(3, 3, 4));
				Add(ucv1);

				Util.load(@"..\conv.txt", (s) =>
				{
					cv1.load(s);
					fc1.load(s);
					fc2.load(s);
					ucv1.load(s);
				});

			}
			public void test()
			{

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

				var ins = getInstance();

				Vol vres2 = forward(ins, inp);

				Console.WriteLine(print(ins.list[1].in_act));
				Console.WriteLine(print(ins.list[1].out_act));
				Console.WriteLine(print(ins.list[4].out_act));
				Console.WriteLine(print(ins.list[5].out_act));
				Console.WriteLine(print(ins.list[7].out_act));


				Console.ReadLine();
			}

		}

		class ConvTestNet3 : Net
		{
			public ConvLayer cv1;
			public FullyConnLayer fc1;
			public FullyConnLayer fc2;
			public DeConvLayer ucv1;

			public void init()
			{

				cv1 = new ConvLayer(filterSize: 3, out_depth: 4, stride: 1, padding: true, act: new ReluLayer());
				fc1 = new FullyConnLayer(num_neurons: 8, act: new ReluLayer());
				fc2 = new FullyConnLayer(num_neurons: 3 * 3 * 4, act: new ReluLayer());
				ucv1 = new DeConvLayer(filterSize: 3, out_sx: 6, out_sy: 6, out_depth: 2, stride: 2, pad: 0, act: new ReluLayer());

				Add(new InputLayer(6, 6, 2));
				Add(cv1);
				Add(new PoolLayer(stride: 2));
				Add(new ReshapeLayer(1, 1, 3 * 3 * 4));
				Add(fc1);
				Add(fc2);
				Add(new ReshapeLayer(3, 3, 4));
				Add(ucv1);

				Util.load(@"..\conv.txt", (s) =>
				{
					cv1.load(s);
					fc1.load(s);
					fc2.load(s);
					ucv1.load(s);
				});

			}
			public void test()
			{

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

				var ins = getInstance();

				Vol vres2 = forward(ins, inp);

				Console.WriteLine(print(ins.list[1].in_act));
				Console.WriteLine(print(ins.list[2].out_act));
				Console.WriteLine(print(ins.list[5].in_act));
				Console.WriteLine(print(ins.list[6].in_act));
				Console.WriteLine(print(vres2));


				Console.ReadLine();
			}

		}

		class ConvTestNet4 : Net
		{
			public InputLayer inputL;
			public ConvLayer cv1;
			public PoolLayer p1;
			public ConvLayer cv2;
			public PoolLayer p2;
			public FullyConnLayer fc1;
			public FullyConnLayer fc2;
			public DeConvLayer ucv1;
			public DeConvLayer ucv2;

			public void init()
			{
				inputL = new InputLayer(8, 8, 2);
				cv1 = new ConvLayer(filterSize: 5, out_depth: 4, stride: 1, padding: true, act: new ReluLayer());
				p1 = new PoolLayer(stride: 2);
				cv2 = new ConvLayer(filterSize: 5, out_depth: 8, stride: 1, padding: true, act: new ReluLayer());
				p2 = new PoolLayer(stride: 2);
				fc1 = new FullyConnLayer(num_neurons: 8, act: new ReluLayer());
				fc2 = new FullyConnLayer(num_neurons: 2 * 2 * 8, act: new ReluLayer());
				ucv1 = new DeConvLayer(filterSize: 5, out_sx: 4, out_sy: 4, out_depth: 4, stride: 2, pad: 1, act: new ReluLayer());//pad关联 filterSize  0:3  1:5 (?2:7)
				ucv2 = new DeConvLayer(filterSize: 5, out_sx: 8, out_sy: 8, out_depth: 2, stride: 2, pad: 1, act: new ReluLayer());


				Add(inputL);
				Add(cv1);
				Add(p1);
				Add(cv2);
				Add(p2);
				Add(new ReshapeLayer(1, 1, 2 * 2 * 8));
				Add(fc1);
				Add(fc2);
				Add(new ReshapeLayer(2, 2, 8));
				Add(ucv1);
				Add(ucv2);

				Util.load(@"..\conv.txt", (s) =>
				{
					cv1.load(s);
					cv2.load(s);
					fc1.load(s);
					fc2.load(s);
					ucv1.load(s);
					ucv2.load(s);
				});

			}
			public void test()
			{

				Vol inp = new Vol(8, 8, 2, 0);

				for (int i = 0; i < 8; i++)
				{
					for (int j = 0; j < 8; j++)
					{
						//inp.set(j, i, 0, (i+1) * 6 + j+1);
						//inp.set(j, i, 1, (i+1) * 6 + j+1 + 0.5f);
						inp.w[i * 8 * 2 + j * 2 + 0] = (i + 1) * 8 + j + 1;
						inp.w[i * 8 * 2 + j * 2 + 1] = (i + 1) * 8 + j + 1 + 0.5f;
					}
				}

				var ins = getInstance();

				Console.WriteLine(print(forward(ins, inp,null, inputL)));
				Console.WriteLine(print(forward(ins, inp,null, p1)));
				Console.WriteLine(print(forward(ins, inp,null, p2)));
				Console.WriteLine(print(forward(ins, inp,null, fc1)));
				Console.WriteLine(print(forward(ins, inp,null, fc2)));
				Console.WriteLine(print(forward(ins, inp,null, ucv1)));
				Console.WriteLine(print(forward(ins, inp,null, ucv2)));


				Console.ReadLine();
			}

		}


		public static string print(Vol v)
		{
			string ret = "";

			ret += "[";
			for (int y = 0; y < v.sy; y++)
			{
				ret += "[";
				for (int x = 0; x < v.sx; x++)
				{
					ret += "[";
					for (int i = 0; i < v.depth; i++)
					{
						ret += v.get(x, y, i) + ",";
					}
					ret += "]\r\n";
				}
				ret += "]\r\n";
			}
			ret += "]\r\n";

			return ret;
		}

		static void Main(string[] args)
		{

			ConvTestNet4 net = new ConvTestNet4();
			net.init();
			net.test();

			//ConvTestNet net = new ConvTestNet();
			//net.init();
			//net.test();
		}
	}
}
