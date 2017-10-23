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
		byte[] buffimg = new byte[1024 * 1024 * 64];
		byte[] bufflbl = new byte[1024 * 1024];


		public Trainer trainer;
		public Net net;

		public MNIST()
		{
			FileStream fsimg1 = new FileStream(@"E:\MNIST\t10k-images.idx3-ubyte", FileMode.Open, FileAccess.Read);
			FileStream fslbl1 = new FileStream(@"E:\MNIST\t10k-labels.idx1-ubyte", FileMode.Open, FileAccess.Read);
			FileStream fsimg2 = new FileStream(@"E:\MNIST\train-images.idx3-ubyte", FileMode.Open, FileAccess.Read);
			FileStream fslbl2 = new FileStream(@"E:\MNIST\train-labels.idx1-ubyte", FileMode.Open, FileAccess.Read);

			fsimg1.Read(buffimg, 0, 16);
			int len = fsimg1.Read(buffimg, 0, 1024 * 1024 * 64);
			fsimg2.Read(buffimg, len, 16);
			fsimg2.Read(buffimg, len, 1024 * 1024 * 64 - len);

			fslbl1.Read(bufflbl, 0, 8);
			len = fslbl1.Read(bufflbl, 0, 1024 * 1024);
			fslbl2.Read(bufflbl, len, 8);
			fslbl2.Read(bufflbl, len, 1024 * 1024 - len);

			fsimg1.Close();
			fslbl1.Close();
			fsimg2.Close();
			fslbl2.Close();
		}

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
			/*
	layer_defs = [];
	layer_defs.push({type:'input', out_sx:28, out_sy:28, out_depth:1});
	layer_defs.push({type:'fc', num_neurons:50, activation:'tanh'});
	layer_defs.push({type:'fc', num_neurons:50, activation:'tanh'});
	layer_defs.push({type:'fc', num_neurons:2});
	layer_defs.push({type:'fc', num_neurons:50, activation:'tanh'});
	layer_defs.push({type:'fc', num_neurons:50, activation:'tanh'});
	layer_defs.push({type:'regression', num_neurons:28*28});

	net = new convnetjs.Net();
	net.makeLayers(layer_defs);

	trainer = new convnetjs.SGDTrainer(net, {learning_rate:1, method:'adadelta', batch_size:50, l2_decay:0.001, l1_decay:0.001});
			 */
			bool selftrain = true;

			if (selftrain)
			{
				net = new Net();
				net.Add(new InputLayer(out_sx: 28, out_sy: 28, out_depth: 1));
				net.Add(new ConvLayer(sx: 4, sy: 4, filters: 4, stride: 2, pad: 2, bias_pref: 0.1f));
				net.Add(new ReluLayer());

				net.Add(new ConvLayer(sx: 4, sy: 4, filters: 8, stride: 2, pad: 2, bias_pref: 0.1f));
				net.Add(new ReluLayer());

				fc2 = new FullyConnLayer(num_neurons: 2, bias_pref: 0.0f);
				net.Add(fc2);//link and buffer
				net.Add(new TanhLayer());

				net.Add(new FullyConnLayer(num_neurons: 7 * 7 * 8, bias_pref: 0.0f));//link and buffer
				net.Add(new TanhLayer());
				net.Add(new ReshapeLayer(out_sx: 7, out_sy: 7, out_depth: 8));

				net.Add(new ConvLayer(sx: 4, sy: 4, filters: 4, unstride: 2, pad: 2, bias_pref: 0.1f));
				net.Add(new ReluLayer());

				net.Add(new ConvLayer(sx: 4, sy: 4, filters: 1, unstride: 2, pad: 2, bias_pref: 0.1f));
				net.Add(new ReluLayer());

				net.Add(new RegressionLayer());
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

		public void train(int index)
		{
			var v = getImg(index);
			DataSet ds = new DataSet();
			ds.predict = getLbl(index);
			trainer.train(v, ds);
		}


		public void selftrain(int index)
		{
			var v = getImg(index);
			DataSet ds = new DataSet();
			ds.data = v;
			trainer.train(v, ds);
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
		public Bitmap display()
		{
			//"red", "blue", "green", "black", "magenta", "cyan", "purple", "aqua", "olive", "lime", "navy"
			Random rnd = new Random();
			Bitmap b = new Bitmap(100,100);
			
			for (int i = 0; i < 50000; i++)
			{
				int index = (int)(rnd.NextDouble() * 70000);
				var v = getImg(index);
				int lbl = getLbl(index);
				net.forward(v);
				int x = (int)(fc2.out_act.w[0] * 25 + 50);
				int y = (int)(fc2.out_act.w[1] * 25 + 50);
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
