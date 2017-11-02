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
		static byte[][] imgData = new byte[60000][];
		static byte[] lblData = new byte[60000];

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
		static int load(string filename, int idx)
		{
			FileStream fsimg1 = new FileStream(filename, FileMode.Open, FileAccess.Read);

			for(int i=0;i<10000;i++){
				fsimg1.Read(lblData, idx, 1);
				byte[] img = new byte[3072];
				fsimg1.Read(img, 0, 3072);
				imgData[idx] = img;
				idx++;
			}
			fsimg1.Close();
			return idx;
		}



		public class MainNet : Net//net
		{
			//主网络，多层卷积 +全连接 收窄到2维神经元 全连接 再多层反卷积 输出图像

			public ConvLayer cv1;
			public ConvLayer cv2;
			public ConvLayer cv3;
			public FullyConnLayer fc4096;
			public FullyConnLayer fc10;

			public Trainer trainer;

			public static int l1Filter = 16;
			public static int l2Filter = 32;
			public static int l3Filter = 64;
			public void init()
			{

				cv1 = new ConvLayer(sx: 4, sy: 4, filters: l1Filter, stride: 2, pad: 2, bias_pref: 0.1f);
				cv2 = new ConvLayer(sx: 4, sy: 4, filters: l2Filter, stride: 2, pad: 2, bias_pref: 0.1f);
				cv3 = new ConvLayer(sx: 4, sy: 4, filters: l3Filter, stride: 2, pad: 2, bias_pref: 0.1f);
				fc4096 = new FullyConnLayer(num_neurons: 4096, bias_pref: 0.1f);
				fc10 = new FullyConnLayer(num_neurons: 10, bias_pref: 0.0f);

				Add(new InputLayer(out_sx: 32, out_sy: 32, out_depth: 3));
				Add(cv1);
				Add(new ReluLayer());

				Add(cv2);
				Add(new ReluLayer());

				Add(cv3);
				Add(new ReluLayer());

				Add(fc4096);
				Add(new ReluLayer());

				Add(fc10);
				Add(new SoftmaxLayer());

				trainer = new Trainer(this, new Trainer.Option() { learning_rate = 0.01f, method = "adadelta", batch_size = 50, l2_decay = 0.001f });//0.001f
			}
			public void save(TextWriter s)
			{
				//cv1.save(s);
				//cv2.save(s);
				//cv3.save(s);
			}
			public void load(TextReader s)
			{
				//cv1.load(s);
				//cv2.load(s);
				//cv3.load(s);
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
			public void test()
			{
				//for (int i = 0; i < trainer.batch_size; i++)
				//{
				//	var v = getImg(trainIndex);
				//	DataSet ds = new DataSet();
				//	ds.data = v;
				//	trainer.train(v, ds);
				//
				//	trainIndex++;
				//	if (trainIndex >= imgCount)
				//	{
				//		trainIndex = 0;
				//	}
				//}
			}
		}

		public MainNet mainNet;


		public void init()
		{
			mainNet = new MainNet();
			mainNet.init();


		}

		static Vol v = new Vol(32, 32, 3, 0.0f);
		public static Vol getImg(int idx)
		{
			for (int i = 0; i < 32; i++)
			{
				int linepos = i * 32;
				for (int j = 0; j < 32; j++)
				{
					int r = imgData[idx][linepos + j];
					int g = imgData[idx][linepos + j + 1024];
					int b = imgData[idx][linepos + j + 2048];
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
			for (int i = 0; i < 32; i++)
			{
				int linepos = i * 32;
				for (int j = 0; j < 32; j++)
				{
					int r = imgData[idx][linepos + j];
					int g = imgData[idx][linepos + j + 1024];
					int b = imgData[idx][linepos + j + 2048];
					bmp.SetPixel(j, i, Color.FromArgb(r, g, b));
				}
			}
			return bmp;
		}

		public int getLbl(int idx)
		{

			return lblData[idx];
		}

	}
}
