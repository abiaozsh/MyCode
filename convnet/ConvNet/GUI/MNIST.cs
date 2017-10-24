using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using ConvNet;

namespace GUI
{
	public class MNIST
	{
		static byte[] buffimg = new byte[1024 * 1024 * 64];
		static byte[] bufflbl = new byte[1024 * 1024];

		static MNIST()
		{
			FileStream fsimg1 = new FileStream(@"E:\MNIST\t10k-images.idx3-ubyte", FileMode.Open, FileAccess.Read);
			FileStream fslbl1 = new FileStream(@"E:\MNIST\t10k-labels.idx1-ubyte", FileMode.Open, FileAccess.Read);
			FileStream fsimg2 = new FileStream(@"E:\MNIST\train-images.idx3-ubyte", FileMode.Open, FileAccess.Read);
			FileStream fslbl2 = new FileStream(@"E:\MNIST\train-labels.idx1-ubyte", FileMode.Open, FileAccess.Read);

			fsimg1.Read(buffimg, 0, 16);//head
			int len = fsimg1.Read(buffimg, 0, 1024 * 1024 * 64);
			fsimg2.Read(buffimg, len, 16);
			fsimg2.Read(buffimg, len, 1024 * 1024 * 64 - len);

			fslbl1.Read(bufflbl, 0, 8);//head
			len = fslbl1.Read(bufflbl, 0, 1024 * 1024);
			fslbl2.Read(bufflbl, len, 8);
			fslbl2.Read(bufflbl, len, 1024 * 1024 - len);

			fsimg1.Close();
			fslbl1.Close();
			fsimg2.Close();
			fslbl2.Close();
		}


		public Trainer trainer;
		public Trainer trainer2;
		public Net net;
		public Net net2;
		public Net net3;
		public Net net4;

		public void proc()
		{
			for (int i = 0; i < 70000; i++)
			{
				Bitmap b = getBmp(i);
				int l = getLbl(i);
				b.Save(@"E:\MNIST\imgs\" + l + "\\" + i + ".bmp", System.Drawing.Imaging.ImageFormat.Bmp);
			}
		}

		FullyConnLayer fc2;

		public void Init()
		{
			bool selftrain = true;

			if (selftrain)
			{
				net = new Net();
				net2 = new Net();
				net3 = new Net();
				net4 = new Net();

				var cv1 = new ConvLayer(sx: 4, sy: 4, filters: 4, stride: 2, pad: 2, bias_pref: 0.1f);
				{
					net.Add(new InputLayer(out_sx: 28, out_sy: 28, out_depth: 1));
					net.Add(cv1);
					net.Add(new ReluLayer());

					net3.Add(new InputLayer(out_sx: 28, out_sy: 28, out_depth: 1));
					net3.Add(cv1);
					net3.Add(new ReluLayer());

					net4.Add(new InputLayer(out_sx: 28, out_sy: 28, out_depth: 1));
					net4.Add(cv1);
					net4.Add(new ReluLayer());
				}

				var cv2 = new ConvLayer(sx: 4, sy: 4, filters: 8, stride: 2, pad: 2, bias_pref: 0.1f);
				{
					net.Add(cv2);
					net.Add(new ReluLayer());
					net3.Add(cv2);
					net3.Add(new ReluLayer());
					net4.Add(cv2);
					net4.Add(new ReluLayer());
				}

				var cv3 = new ConvLayer(sx: 4, sy: 4, filters: 16, stride: 2, pad: 2, bias_pref: 0.1f);
				{
					net.Add(cv3);
					net.Add(new ReluLayer());
					net3.Add(cv3);
					net3.Add(new ReluLayer());
					net4.Add(cv3);
					net4.Add(new ReluLayer());
				}
				fc2 = new FullyConnLayer(num_neurons: 2, bias_pref: 0.0f);
				{
					net.Add(fc2);//link and buffer
					net.Add(new TanhLayer());

					net3.Add(fc2);

					net4.Add(fc2);
					net4.Add(new TanhLayer());

					net4.Add(new FullyConnLayer(num_neurons: 20, bias_pref: 0.1f));
					net4.Add(new ReluLayer());

					net4.Add(new FullyConnLayer(num_neurons: 20, bias_pref: 0.1f));
					net4.Add(new ReluLayer());

					net4.Add(new FullyConnLayer(num_neurons: 10, bias_pref: 0.1f));
					net4.Add(new ReluLayer());
					net4.Add(new SoftmaxLayer());

				}
				net2.Add(new InputLayer(out_sx: 1, out_sy: 1, out_depth: 2));
				net2.Add(new TanhLayer());

				var fc1 = new FullyConnLayer(num_neurons: 4 * 4 * 16, bias_pref: 0.0f);
				net.Add(fc1);//link and buffer
				net.Add(new TanhLayer());
				net.Add(new ReshapeLayer(out_sx: 4, out_sy: 4, out_depth: 16));

				net2.Add(fc1);//link and buffer
				net2.Add(new TanhLayer());
				net2.Add(new ReshapeLayer(out_sx: 4, out_sy: 4, out_depth: 16));

				var ucv0 = new ConvLayer(sx: 4, sy: 4, filters: 8, unstride: 2, pad: 2, bias_pref: 0.1f);
				net.Add(ucv0);
				net.Add(new ReluLayer());
				net2.Add(ucv0);
				net2.Add(new ReluLayer());

				var ucv1 = new ConvLayer(sx: 4, sy: 4, filters: 4, unstride: 2, pad: 2, bias_pref: 0.1f);
				net.Add(ucv1);
				net.Add(new ReluLayer());
				net2.Add(ucv1);
				net2.Add(new ReluLayer());

				var ucv2 = new ConvLayer(sx: 4, sy: 4, filters: 1, unstride: 2, pad: 1, bias_pref: 0.1f);
				net.Add(ucv2);
				net.Add(new ReluLayer());
				net.Add(new RegressionLayer());

				net2.Add(ucv2);
				net2.Add(new ReluLayer());
				net2.Add(new RegressionLayer());

				trainer2 = new Trainer(net4, new Trainer.Option() { method = "adadelta", batch_size = 20, l2_decay = 0.0f });//0.001f

				trainer = new Trainer(net, new Trainer.Option() { learning_rate = 1, method = "adadelta", batch_size = 50, l2_decay = 0.001f });//0.001f
			}
			else
			{
				net = new Net();
				net.Add(new InputLayer(out_sx: 28, out_sy: 28, out_depth: 1));
				net.Add(new ConvLayer(sx: 5, sy: 5, filters: 4, stride: 2, pad: 2, bias_pref: 0.1f));
				net.Add(new ReluLayer());
				//net.Add(new PoolLayer(stride: 2));

				net.Add(new ConvLayer(sx: 5, sy: 5, filters: 8, stride: 3, pad: 2, bias_pref: 0.1f));
				net.Add(new ReluLayer());
				//net.Add(new PoolLayer(stride: 3));


				//net.Add(new FullyConnLayer(num_neurons: 10, bias_pref: 0.1f));
				//net.Add(new ReluLayer());
				net.Add(new FullyConnLayer(num_neurons: 10, bias_pref: 0.0f));//link and buffer
				net.Add(new SoftmaxLayer());
				trainer = new Trainer(net, new Trainer.Option() { method = "adadelta", batch_size = 20, l2_decay = 0.0f });//0.001f
			}

		}

		int trainIndex = 0;

		public void train2()
		{
			var v = getImg(trainIndex);
			DataSet ds = new DataSet();
			ds.predict = getLbl(trainIndex);
			trainer2.train(v, ds);

			trainIndex++;
			if (trainIndex >= 70000)
			{
				trainIndex = 0;
			}
		}


		public void selftrain()
		{
			var v = getImg(trainIndex);
			DataSet ds = new DataSet();
			ds.data = v;
			trainer.train(v, ds);

			trainIndex++;
			if (trainIndex >= 70000)
			{
				trainIndex = 0;
			}
		}

		public Color[] cDict = new Color[]{
				Color.Red, //0
				Color.Blue,//1
				Color.Green,//2
				Color.Black,//3
				Color.Magenta,//4
				Color.Cyan,//5
				Color.Purple,//6
				Color.Orange,//7
				Color.Olive,//8
				Color.Lime,//9
				Color.Navy//10
			};

		public float visScale = 10.0f;
		public Bitmap display()
		{
			//"red", "blue", "green", "black", "magenta", "cyan", "purple", "aqua", "olive", "lime", "navy"
			Random rnd = new Random();
			Bitmap b = new Bitmap(100, 100);

			for (int i = 0; i < 10000; i++)
			{
				int index = (int)(rnd.NextDouble() * 70000);
				var v = getImg(index);
				int lbl = getLbl(index);
				var fco2 = net3.forward(v);
				int x = (int)(fco2.w[0] * visScale + 50);
				int y = (int)(fco2.w[1] * visScale + 50);
				if (x >= 100) x = 99;
				if (x < 0) x = 0;
				if (y >= 100) y = 99;
				if (y < 0) y = 0;
				Color color = cDict[lbl];
				b.SetPixel(x, y, color);
			}

			return b;
		}


		Vol v = new Vol(28, 28, 1, 0.0f);
		public Vol getImg(int idx)
		{
			int[] data = new int[28 * 28];

			for (int i = 0; i < 28 * 28; i++)
			{
				int val = buffimg[idx * 28 * 28 + i];
				data[i] = val;
			}

			int[] img = data;
			for (var i = 0; i < 28 * 28; i++)
			{
				v.w[i] = (float)img[i] / 255.0f;
			}
			return v;
		}
		public Bitmap getBmp(int idx)
		{
			Bitmap b = new Bitmap(28, 28);

			for (int i = 0; i < 28; i++)
			{
				for (int j = 0; j < 28; j++)
				{
					int val = buffimg[idx * 28 * 28 + i * 28 + j];
					b.SetPixel(j, i, Color.FromArgb(val, val, val));
				}
			}
			return b;
		}
		public int getLbl(int idx)
		{
			return bufflbl[idx];
		}

		public class Pack
		{
			public float predict;
			public int val;
		}
		public string report(Vol v)
		{

			List<Pack> list = new List<Pack>();
			for (int i = 0; i < 10; i++)
			{
				list.Add(new Pack() { predict = v.w[i], val = i });
			}

			list.Sort((a, b) => ((a.predict > b.predict) ? -1 : 1));

			string ret = "";
			for (int i = 0; i < 10; i++)
			{
				ret += list[i].val + ":" + list[i].predict + "\r\n";
			}
			return ret;
		}

	}
}
