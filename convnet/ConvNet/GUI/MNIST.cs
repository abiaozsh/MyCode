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

		public class MainNet : Net//net
		{
			//主网络，多层卷积 +全连接 收窄到2维神经元 全连接 再多层反卷积 输出图像

			public ConvLayer cv1;
			public ConvLayer cv2;
			public ConvLayer cv3;
			public FullyConnLayer fc2;
			public FullyConnLayer fc1;
			public ConvLayer ucv0;
			public ConvLayer ucv1;
			public ConvLayer ucv2;

			public Trainer trainer;

			public void init()
			{
				cv1 = new ConvLayer(sx: 4, sy: 4, filters: 4, stride: 2, pad: 2, bias_pref: 0.1f);
				cv2 = new ConvLayer(sx: 4, sy: 4, filters: 8, stride: 2, pad: 2, bias_pref: 0.1f);
				cv3 = new ConvLayer(sx: 4, sy: 4, filters: 16, stride: 2, pad: 2, bias_pref: 0.1f);
				fc2 = new FullyConnLayer(num_neurons: 2, bias_pref: 0.0f);
				fc1 = new FullyConnLayer(num_neurons: 4 * 4 * 16, bias_pref: 0.0f);
				ucv0 = new ConvLayer(sx: 4, sy: 4, filters: 8, unstride: 2, pad: 2, bias_pref: 0.1f);
				ucv1 = new ConvLayer(sx: 4, sy: 4, filters: 4, unstride: 2, pad: 2, bias_pref: 0.1f);
				ucv2 = new ConvLayer(sx: 4, sy: 4, filters: 1, unstride: 2, pad: 1, bias_pref: 0.1f);

				Add(new InputLayer(out_sx: 28, out_sy: 28, out_depth: 1));
				Add(cv1);
				Add(new ReluLayer());

				Add(cv2);
				Add(new ReluLayer());

				Add(cv3);
				Add(new ReluLayer());

				Add(fc2);//link and buffer
				Add(new TanhLayer());

				Add(fc1);//link and buffer
				Add(new TanhLayer());
				Add(new ReshapeLayer(out_sx: 4, out_sy: 4, out_depth: 16));

				Add(ucv0);
				Add(new ReluLayer());

				Add(ucv1);
				Add(new ReluLayer());

				Add(ucv2);
				Add(new ReluLayer());
				Add(new RegressionLayer());

				trainer = new Trainer(this, new Trainer.Option() { learning_rate = 1.1f, method = "adadelta", batch_size = 50, l2_decay = 0.001f });//0.001f
			}
			public void save(TextWriter s)
			{
				cv1.save(s);
				cv2.save(s);
				cv3.save(s);
				fc2.save(s);
				fc1.save(s);
				ucv0.save(s);
				ucv1.save(s);
				ucv2.save(s);
			}
			public void load(TextReader s)
			{
				cv1.load(s);
				cv2.load(s);
				cv3.load(s);
				fc2.load(s);
				fc1.load(s);
				ucv0.load(s);
				ucv1.load(s);
				ucv2.load(s);

				//rom
				//cv1.load(s); cv1.noUpdate = true;
				//cv2.load(s); cv2.noUpdate = true;
				//cv3.load(s); cv3.noUpdate = true;
				//fc2.load(s); fc2.noUpdate = true;
			}

			static Random r = new Random();
			int trainIndex = (int)(r.NextDouble() * 69999);
			public void train()
			{
				for (int i = 0; i < trainer.batch_size; i++)
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
			}
		}
		public class CoTrainNet : Net//net4
		{
			//输入图片，输出预测
			public FullyConnLayer fc20A;
			public FullyConnLayer fc20B;
			public FullyConnLayer fc10;
			public void save(TextWriter s)
			{
				fc20A.save(s);
				fc20B.save(s);
				fc10.save(s);
			}
			public void load(TextReader s)
			{
				fc20A.load(s);
				fc20B.load(s);
				fc10.load(s);
			}

			public Trainer trainer;

			//联合训练网
			public void init(ConvLayer cv1, ConvLayer cv2, ConvLayer cv3, FullyConnLayer fc2)
			{
				fc20A = new FullyConnLayer(num_neurons: 20, bias_pref: 0.1f);
				fc20B = new FullyConnLayer(num_neurons: 20, bias_pref: 0.1f);
				fc10 = new FullyConnLayer(num_neurons: 10, bias_pref: 0.1f);

				Add(new InputLayer(out_sx: 28, out_sy: 28, out_depth: 1));
				Add(cv1);
				Add(new ReluLayer());

				Add(cv2);
				Add(new ReluLayer());

				Add(cv3);
				Add(new ReluLayer());

				Add(fc2);
				Add(new TanhLayer());

				Add(fc20A);
				Add(new ReluLayer());

				Add(fc20B);
				Add(new ReluLayer());

				Add(fc10);
				Add(new ReluLayer());
				Add(new SoftmaxLayer());

				trainer = new Trainer(this, new Trainer.Option() { method = "adadelta", batch_size = 20, l2_decay = 0.0f });//0.001f
			}

			static Random r = new Random();
			int trainIndex = (int)(r.NextDouble() * 69999);

			public void train()
			{
				for (int i = 0; i < trainer.batch_size; i++)
				{
					while (true)
					{
						DataSet ds = new DataSet();
						trainIndex++;
						if (trainIndex >= 70000)
						{
							trainIndex = 0;
						}
						ds.predict = MNIST.getLbl(trainIndex);
						if (ds.predict != 8)
						{
							continue;
						}
						var v = MNIST.getImg(trainIndex);
						trainer.train(v, ds);

						break;
					}
				}
			}
		}
		public class CoTrainTestNet : Net
		{
			//输入 x,y 输出预测 0~9
			public void init(FullyConnLayer fc20A, FullyConnLayer fc20B, FullyConnLayer fc10)
			{
				Add(new InputLayer(out_sx: 1, out_sy: 1, out_depth: 2));
				Add(new TanhLayer());

				Add(fc20A);
				Add(new ReluLayer());

				Add(fc20B);
				Add(new ReluLayer());

				Add(fc10);
				Add(new ReluLayer());
				Add(new SoftmaxLayer());
			}

			Vol regen = new Vol(1, 1, 2, 0);
			public string proc(int X, int Y)
			{
				float x = (X - 50f) / MNIST.visScale;
				float y = (Y - 50f) / MNIST.visScale;
				regen.w[0] = x;
				regen.w[1] = y;

				var _out = forward(regen);
				String s = MNIST.report(_out);

				return s;
			}

		}

		public class XY2ImgNet : Net//net2
		{
			// 输入二维向量 全连接 再多层反卷积 输出图像
			// 和主网络共享层
			public void init(FullyConnLayer fc1, ConvLayer ucv0, ConvLayer ucv1, ConvLayer ucv2)
			{
				Add(new InputLayer(out_sx: 1, out_sy: 1, out_depth: 2));
				Add(new TanhLayer());

				Add(fc1);//link and buffer
				Add(new TanhLayer());
				Add(new ReshapeLayer(out_sx: 4, out_sy: 4, out_depth: 16));

				Add(ucv0);
				Add(new ReluLayer());

				Add(ucv1);
				Add(new ReluLayer());

				Add(ucv2);
				Add(new ReluLayer());
				Add(new RegressionLayer());
			}

			Vol regen = new Vol(1, 1, 2, 0);
			public Bitmap proc(int X, int Y)
			{
				float x = (X - 50f) / MNIST.visScale;
				float y = (Y - 50f) / MNIST.visScale;
				regen.w[0] = x;
				regen.w[1] = y;

				var _out = forward(regen);

				return _out.vis(0);
			}
		}
		public class Img2XYNet : Net//net3
		{
			//输入图像 输坐标
			public void init(ConvLayer cv1, ConvLayer cv2, ConvLayer cv3, FullyConnLayer fc2)
			{
				Add(new InputLayer(out_sx: 28, out_sy: 28, out_depth: 1));
				Add(cv1);
				Add(new ReluLayer());

				Add(cv2);
				Add(new ReluLayer());

				Add(cv3);
				Add(new ReluLayer());

				Add(fc2);
			}
		}

		public MainNet mainNet;
		public CoTrainNet coTrainNet;
		public CoTrainTestNet coTrainTestNet;
		public XY2ImgNet xy2ImgNet;
		public Img2XYNet img2XYNet;

		//public void proc()
		//{
		//	for (int i = 0; i < 70000; i++)
		//	{
		//		Bitmap b = getBmp(i);
		//		int l = getLbl(i);
		//		b.Save(@"E:\MNIST\imgs\" + l + "\\" + i + ".bmp", System.Drawing.Imaging.ImageFormat.Bmp);
		//	}
		//}

		public void Init()
		{
			mainNet = new MainNet();
			mainNet.init();

			coTrainNet = new CoTrainNet();
			coTrainNet.init(mainNet.cv1, mainNet.cv2, mainNet.cv3, mainNet.fc2);

			coTrainTestNet = new CoTrainTestNet();
			coTrainTestNet.init(coTrainNet.fc20A, coTrainNet.fc20B, coTrainNet.fc10);

			xy2ImgNet = new XY2ImgNet();
			xy2ImgNet.init(mainNet.fc1, mainNet.ucv0, mainNet.ucv1, mainNet.ucv2);

			img2XYNet = new Img2XYNet();
			img2XYNet.init(mainNet.cv1, mainNet.cv2, mainNet.cv3, mainNet.fc2);

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

		public static float visScale = 10.0f;
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
				var fco2 = img2XYNet.forward(v);
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


		static Vol v = new Vol(28, 28, 1, 0.0f);
		public static Vol getImg(int idx)
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
		static public Bitmap getBmp(int idx)
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
		static public int getLbl(int idx)
		{
			return bufflbl[idx];
		}

		public class Pack
		{
			public float predict;
			public int val;
		}
		public static string report(Vol v)
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
