﻿using System;
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
			//int idx = 0;

			//idx = load(@"E:\MNIST\cifar-10-batches-bin\data_batch_1.bin", idx);
			//idx = load(@"E:\MNIST\cifar-10-batches-bin\data_batch_2.bin", idx);
			//idx = load(@"E:\MNIST\cifar-10-batches-bin\data_batch_3.bin", idx);
			//idx = load(@"E:\MNIST\cifar-10-batches-bin\data_batch_4.bin", idx);
			//idx = load(@"E:\MNIST\cifar-10-batches-bin\data_batch_5.bin", idx);
			//idx = load(@"E:\MNIST\cifar-10-batches-bin\test_batch.bin", idx);
			load(@"E:\MNIST\cifar-10-batches-bin\32x32.bin");

		}

		//public static Bitmap get4x4(int n, int x, int y)
		//{
		//	byte[] img = imgData[n];
		//	Bitmap bmp = new Bitmap(4, 4);
		//	for (int j = 0; j < 4; j++)
		//	{
		//		int linepos = (j + y) * 32;
		//		for (int i = 0; i < 4; i++)
		//		{
		//			int r = img[linepos + i + x];
		//			int g = img[linepos + i + x + 1024];
		//			int b = img[linepos + i + x + 2048];
		//			bmp.SetPixel(i, j, Color.FromArgb(r, g, b));
		//		}
		//	}
		//	return bmp;
		//}
		static Vol v4 = new Vol(4, 4, 3, 0.0f);
		public static Vol get4x4(int n, int x, int y)
		{
			byte[] img = imgData[n];
			for (int j = 0; j < 4; j++)
			{
				int linepos = (j + y) * 32;
				for (int i = 0; i < 4; i++)
				{
					int r = img[linepos + i + x];
					int g = img[linepos + i + x + 1024];
					int b = img[linepos + i + x + 2048];
					v4.set(j, i, 0, r / 255.0f);
					v4.set(j, i, 1, g / 255.0f);
					v4.set(j, i, 2, b / 255.0f);
				}
			}
			return v4;
		}
		static Vol v8 = new Vol(8, 8, 3, 0.0f);
		public static Vol get8x8(int n, int x, int y)
		{
			byte[] img = imgData[n];
			for (int j = 0; j < 8; j++)
			{
				int linepos = (j + y) * 32;
				for (int i = 0; i < 8; i++)
				{
					int r = img[linepos + i + x];
					int g = img[linepos + i + x + 1024];
					int b = img[linepos + i + x + 2048];
					v8.set(j, i, 0, r / 255.0f);
					v8.set(j, i, 1, g / 255.0f);
					v8.set(j, i, 2, b / 255.0f);
				}
			}
			return v8;
		}

		//static void save() {
		//
		//	FileStream fsimg1 = new FileStream(@"E:\MNIST\cifar-10-batches-bin\32x32.bin", FileMode.Create, FileAccess.Write);
		//
		//	for (int i = 0; i < 60000; i++)
		//	{
		//		fsimg1.Write(imgData[i], 0, 3072);
		//	}
		//
		//	fsimg1.Flush();
		//	fsimg1.Close();
		//}
		//static int load(string filename, int idx)
		//{
		//	FileStream fsimg1 = new FileStream(filename, FileMode.Open, FileAccess.Read);
		//
		//	for(int i=0;i<10000;i++){
		//		fsimg1.Read(lblData, idx, 1);
		//		byte[] img = new byte[3072];
		//		fsimg1.Read(img, 0, 3072);
		//		imgData[idx] = img;
		//		idx++;
		//	}
		//	fsimg1.Close();
		//	return idx;
		//}
		static void load(string filename)
		{
			FileStream fsimg1 = new FileStream(filename, FileMode.Open, FileAccess.Read);

			for (int i = 0; i < 60000; i++)
			{
				byte[] img = new byte[3072];
				fsimg1.Read(img, 0, 3072);
				imgData[i] = img;
			}

			fsimg1.Close();
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

		public static Bitmap getBmp(int idx)
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



		public class MainNet : Net//net
		{
			//主网络，多层卷积 +全连接 收窄到2维神经元 全连接 再多层反卷积 输出图像

			public ConvLayer cv1;
			public ConvLayer cv2;
			public ConvLayer cv3;
			public FullyConnLayer fc4096;
			public FullyConnLayer fc10;

			public static int l1Filter = 16;
			public static int l2Filter = 32;
			public static int l3Filter = 64;
			public void init()
			{
				trainer = new AdaDeltaTrainer(50) { l2_decay = 0.001f };//0.001f method = "adadelta",learning_rate = 0.01f, 
				//trainer = new SGDTrainer(50) {learning_rate = 0.01f,  l2_decay = 0.001f };//0.001f method = "adadelta",

				cv1 = new ConvLayer(sx: 4, sy: 4, filters: l1Filter, stride: 2, pad: 2, bias_pref: 0.1f, act: new ReluLayer());
				cv2 = new ConvLayer(sx: 4, sy: 4, filters: l2Filter, stride: 2, pad: 2, bias_pref: 0.1f, act: new ReluLayer());
				cv3 = new ConvLayer(sx: 4, sy: 4, filters: l3Filter, stride: 2, pad: 2, bias_pref: 0.1f, act: new ReluLayer());
				fc4096 = new FullyConnLayer(num_neurons: 4096, bias_pref: 0.1f, act: new ReluLayer());
				fc10 = new FullyConnLayer(num_neurons: 10, bias_pref: 0.0f);

				Add(new InputLayer(out_sx: 32, out_sy: 32, out_depth: 3));
				Add(cv1);
				Add(cv2);
				Add(cv3);
				Add(fc4096);
				Add(fc10);
				Add(new SoftmaxLayer());

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
				for (int i = 0; i < trainer.batchSize; i++)
				{
					var v = getImg(trainIndex);
					DataSet ds = new DataSet();
					ds.data = v;
					train(v, ds);

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

		public class RegNet : Net//net
		{
			//主网络，多层卷积 +全连接 收窄到2维神经元 全连接 再多层反卷积 输出图像

			public ConvLayer cv1;
			public ConvLayer cv2;
			public ConvLayer cv3;
			public FullyConnLayer fc4096A;
			//public FullyConnLayer fc4096B;
			public ConvLayer ucv0;
			public ConvLayer ucv1;
			public ConvLayer ucv2;

			public static int l1Filter = 16;
			public static int l2Filter = 32;
			public static int l3Filter = 64;
			public void init()
			{
				trainer = new AdaDeltaTrainer(10) { ro = 0.5f, l2_decay = 0.001f };//0.001f learning_rate = 0.1f, 
				//trainer = new SGDTrainer(5) { learning_rate = 0.0002f, l2_decay = 0.001f };//0.001f

				cv1 = new ConvLayer(sx: 4, sy: 4, filters: l1Filter, stride: 2, pad: 0, bias_pref: 0.1f, act: new ReluLayer());
				cv2 = new ConvLayer(sx: 4, sy: 4, filters: l2Filter, stride: 2, pad: 0, bias_pref: 0.1f, act: new ReluLayer());
				cv3 = new ConvLayer(sx: 4, sy: 4, filters: l3Filter, stride: 2, pad: 0, bias_pref: 0.1f, act: new ReluLayer());
				fc4096A = new FullyConnLayer(num_neurons: 512, bias_pref: 0.1f, act: new ReluLayer());
				//ucv0 = new ConvLayer(sx: 4, sy: 4, filters: l2Filter, unstride: 2, pad: 3,adj:-1, bias_pref: 0.1f, act: new ReluLayer());
				//ucv1 = new ConvLayer(sx: 4, sy: 4, filters: l1Filter, unstride: 2, pad: 3, adj: -1, bias_pref: 0.1f, act: new ReluLayer());
				//ucv2 = new ConvLayer(sx: 4, sy: 4, filters: 3, unstride: 2, pad: 3, bias_pref: 0.1f);

				Add(new InputLayer(out_sx: 32, out_sy: 32, out_depth: 3));
				Add(cv1);
				Add(cv2);
				Add(cv3);
				Add(fc4096A);
				Add(new ReshapeLayer(out_sx: 3, out_sy: 3, out_depth: l3Filter));
				Add(ucv0);
				Add(ucv1);
				Add(ucv2);
				Add(new RegressionLayer());

				Util.load(@"..\lv1_4_4v0_Filter16.txt",(s)=>{
					//cv1.load(s);
				});
				cv1.noUpdate=true;
			}
			public void save(TextWriter s)
			{
			}
			public void load(TextReader s)
			{
			}

			public Vol train(int n)
			{
				DataSet ds = new DataSet();
				//train
				//for (int i = 0; i < trainer.batchSize; i++)
				{
					//int trainIndex = n;//(int)(MNISTData.rnd.NextDouble() * 70000);


					var v = getImg(n);
					ds.data = v;

					train(v, ds);
				}
				return ucv2.out_act;
			}
		}

		public class Reg1LNet : Net//net
		{
			//主网络，多层卷积 +全连接 收窄到2维神经元 全连接 再多层反卷积 输出图像

			public ConvLayer cv1;
			public FullyConnLayer fc;
			public ConvLayer ucv2;

			public static int l1Filter = 16;
			public void init()
			{
				trainer = new AdaDeltaTrainer(17) { ro = 0.5f, l2_decay = 0.001f };//0.001f learning_rate = 0.1f, 
				//trainer = new SGDTrainer(5) { learning_rate = 0.0002f, l2_decay = 0.001f };//0.001f

				cv1 = new ConvLayer(sx: 4, sy: 4, filters: l1Filter, stride: 1, pad: 0, bias_pref: 0.1f, act: new ReluLayer());
				fc = new FullyConnLayer(num_neurons: l1Filter, bias_pref: 0.1f, act: new ReluLayer());
				//ucv2 = new ConvLayer(sx: 4, sy: 4, filters: 3, unstride: 1, pad: 3, adj: 2, bias_pref: 0.1f);

				Add(new InputLayer(out_sx: 4, out_sy: 4, out_depth: 3));
				Add(cv1);
				Add(fc);
				//Add(new ReshapeLayer(out_sx: 1, out_sy: 1, out_depth: l1Filter));
				Add(ucv2);
				Add(new RegressionLayer());

			}
			public void save(TextWriter s)
			{
			}
			public void load(TextReader s)
			{
			}

			public void train(int n, int x, int y)
			{
				DataSet ds = new DataSet();
				//train
				var v = get4x4(n, x, y);
				ds.data = v;

				train(v, ds);
			}
		}

		public class Reg1LBNet : Net//net
		{
			//主网络，多层卷积 +全连接 收窄到2维神经元 全连接 再多层反卷积 输出图像

			public ConvLayer cv1;
			public FullyConnLayer fc;
			public FullyConnLayer fc2;
			public ConvLayer ucv2;

			public static int l1Filter = 16;
			public void init()
			{
				trainer = new AdaDeltaTrainer(10) { ro = 0.5f, l2_decay = 0.001f };//0.001f learning_rate = 0.1f, 
				//trainer = new SGDTrainer(5) { learning_rate = 0.0002f, l2_decay = 0.001f };//0.001f

				fc = new FullyConnLayer(num_neurons: 64, bias_pref: 0.1f, act: new ReluLayer());
				fc2 = new FullyConnLayer(num_neurons: 32*32*3, bias_pref: 0.1f);

				Add(new InputLayer(out_sx: 32, out_sy: 32, out_depth: 3));
				Add(fc);
				Add(fc2);
				Add(new ReshapeLayer(out_sx: 32, out_sy: 32, out_depth: 3));
				Add(new RegressionLayer());

				//Util.load(@"..\lv1_4_4v2_Filter16.txt", (s) =>
				//{
				//	cv1.load(s);
				//});
				//cv1.noUpdate = true;
			}
			public void save(TextWriter s)
			{
			}
			public void load(TextReader s)
			{
			}

			public void train(int n, int x, int y)
			{
				DataSet ds = new DataSet();
				//train
				//for (int i = 0; i < trainer.batchSize; i++)
				{
					var v = getImg(n);// get8x8(n, x, y);
					ds.data = v;

					train(v, ds);
				}
			}
		}

		public void init()
		{
		}


	}
}
