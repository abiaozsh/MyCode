using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ConvNet;

namespace GUI
{
	class Cifar
	{
		static byte[] buffData = new byte[1024 * 1024 * 200];

		static int imgCount = 60000;

		static Cifar()
		{
			int idx = 0;

			idx = load(@"E:\MNIST\cifar-10-batches-bin\data_batch_1.bin", idx);
			idx = load(@"E:\MNIST\cifar-10-batches-bin\data_batch_2.bin", idx);
			idx = load(@"E:\MNIST\cifar-10-batches-bin\data_batch_3.bin", idx);
			idx = load(@"E:\MNIST\cifar-10-batches-bin\data_batch_4.bin", idx);
			idx = load(@"E:\MNIST\cifar-10-batches-bin\data_batch_5.bin", idx);
			idx = load(@"E:\MNIST\cifar-10-batches-bin\test_batch.bin", idx);
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

			public static int l1Filter = 16;
			public static int l2Filter = 32;
			public static int l3Filter = 64;
			public void init()
			{

				cv1 = new ConvLayer(sx: 4, sy: 4, filters: l1Filter, stride: 2, pad: 2, bias_pref: 0.1f);
				cv2 = new ConvLayer(sx: 4, sy: 4, filters: l2Filter, stride: 2, pad: 2, bias_pref: 0.1f);
				cv3 = new ConvLayer(sx: 4, sy: 4, filters: l3Filter, stride: 2, pad: 2, bias_pref: 0.1f);
				fc2 = new FullyConnLayer(num_neurons: 2, bias_pref: 0.0f);
				fc1 = new FullyConnLayer(num_neurons: 4 * 4 * l3Filter, bias_pref: 0.0f);
				ucv0 = new ConvLayer(sx: 4, sy: 4, filters: l2Filter, unstride: 2, pad: 2, bias_pref: 0.1f);
				ucv1 = new ConvLayer(sx: 4, sy: 4, filters: l1Filter, unstride: 2, pad: 2, bias_pref: 0.1f);
				ucv2 = new ConvLayer(sx: 4, sy: 4, filters: 3, unstride: 2, pad: 2, bias_pref: 0.1f);

				Add(new InputLayer(out_sx: 32, out_sy: 32, out_depth: 3));
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
				Add(new ReshapeLayer(out_sx: 4, out_sy: 4, out_depth: l3Filter));

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
			}

			static Random r = new Random();
			int trainIndex = (int)(r.NextDouble() * (imgCount - 1));
			public void train()
			{
				for (int i = 0; i < trainer.batch_size; i++)
				{
					var v = getImg(trainIndex);
					DataSet ds = new DataSet();
					ds.data = v;
					trainer.train(v, ds);

					trainIndex++;
					if (trainIndex >= imgCount)
					{
						trainIndex = 0;
					}
				}
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
				Add(new ReshapeLayer(out_sx: 4, out_sy: 4, out_depth: MainNet.l3Filter));

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
				Add(new InputLayer(out_sx: 32, out_sy: 32, out_depth: 3));
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
		public XY2ImgNet xy2ImgNet;
		public Img2XYNet img2XYNet;


		public void init()
		{
			mainNet = new MainNet();
			mainNet.init();

			xy2ImgNet = new XY2ImgNet();
			xy2ImgNet.init(mainNet.fc1, mainNet.ucv0, mainNet.ucv1, mainNet.ucv2);

			img2XYNet = new Img2XYNet();
			img2XYNet.init(mainNet.cv1, mainNet.cv2, mainNet.cv3, mainNet.fc2);

		}
		public Bitmap display(Form3 form,float scale)
		{
			//"red", "blue", "green", "black", "magenta", "cyan", "purple", "aqua", "olive", "lime", "navy"
			Random rnd = new Random();
			int width = 10000;
			int height = 10000;
			Bitmap b = new Bitmap(width, height);
			Graphics g = Graphics.FromImage(b);
			for (int i = 0; i < 1000; i++)
			{
				int index = (int)(rnd.NextDouble() * imgCount);
				var v = getImg(index);
				Bitmap bmp = getBmp(index);
				var fco2 = img2XYNet.forward(v);
				int x = (int)(fco2.w[0] * scale + width / 2);
				int y = (int)(fco2.w[1] * scale + height / 2);
				if (x >= width) x = width - 1;
				if (x < 0) x = 0;
				if (y >= height) y = height - 1;
				if (y < 0) y = 0;

				g.DrawImage(bmp, x, y);

				form.Text = i.ToString();
				g.Flush();
			}
			g.Dispose();

			return b;
		}

		static Vol v = new Vol(32, 32, 3, 0.0f);
		public static Vol getImg(int idx)
		{
			int imgsize = 32 * 32 * 3 + 1;
			int pos = idx * imgsize;
			for (int i = 0; i < 32; i++)
			{
				int linepos = i * 32;
				for (int j = 0; j < 32; j++)
				{
					int r = buffData[pos + linepos + j + 1];
					int g = buffData[pos + linepos + j + 1 + 1024];
					int b = buffData[pos + linepos + j + 1 + 2048];
					v.set(i, j, 0, r / 255.0f);
					v.set(i, j, 1, g / 255.0f);
					v.set(i, j, 2, b / 255.0f);
				}
			}
			return v;
		}

		public Bitmap getBmp(int idx)
		{
			Bitmap bmp = new Bitmap(32, 32);
			int imgsize = 32 * 32 * 3 + 1;
			int pos = idx * imgsize;
			for (int i = 0; i < 32; i++)
			{
				int linepos = i * 32;
				for (int j = 0; j < 32; j++)
				{
					int r = buffData[pos + linepos + j + 1];
					int g = buffData[pos + linepos + j + 1 + 1024];
					int b = buffData[pos + linepos + j + 1 + 2048];
					bmp.SetPixel(j, i, Color.FromArgb(r, g, b));
				}
			}
			return bmp;
		}

		public int getLbl(int idx)
		{
			int imgsize = 32 * 32 * 3 + 1;

			return buffData[idx * imgsize];
		}

		static int load(string filename, int idx)
		{
			FileStream fsimg1 = new FileStream(filename, FileMode.Open, FileAccess.Read);

			int len = fsimg1.Read(buffData, idx, 1024 * 1024 * 40);

			fsimg1.Close();
			return len + idx;
		}
	}
}
