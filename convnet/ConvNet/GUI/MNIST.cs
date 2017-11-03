using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using ConvNet;

namespace GUI
{
	public class MNISTData
	{
		public static byte[] boundary;
		public static byte[] buffimg = new byte[1024 * 1024 * 64];
		public static byte[] bufflbl = new byte[1024 * 1024];

		public static void proc()
		{
			//for (int i = 0; i < 70000; i++)
			//{
			//	//Bitmap b = _______getBmp(i);
			//	//int l = _______getLbl(i);
			//	//b.Save(@"E:\MNIST\imgs\" + l + "\\" + i + ".bmp", System.Drawing.Imaging.ImageFormat.Bmp);
			//}


			//Util.save("boundary.txt", (s) =>
			//{
			//	for (int i = 0; i < 70000; i++)
			//	{
			//		//Bitmap b = _______getBmp(i);
			//		//int l = _______getLbl(i);
			//		//b.Save(@"E:\MNIST\imgs\" + l + "\\" + i + ".bmp", System.Drawing.Imaging.ImageFormat.Bmp);
			//
			//		int left = 100;
			//		int top = 100;
			//		int right = 0;
			//		int bottom = 0;
			//
			//		for (int y = 0; y < 28; y++)
			//		{
			//			for (int x = 0; x < 28; x++)
			//			{
			//				int pixel = buffimg[i * 28 * 28 + y * 28 + x];
			//				if (pixel > 4)
			//				{
			//					if (y < top) top = y;
			//					if (y > bottom) bottom = y;
			//
			//					if (x < left) left = x;
			//					if (x > right) right = x;
			//				}
			//			}
			//		}
			//
			//		s.WriteLine(left);
			//		s.WriteLine(top);
			//		s.WriteLine(right);
			//		s.WriteLine(bottom);
			//	}
			//});
		}

		public static void loadBoundary()
		{
			boundary = new byte[70000 * 4];
			Util.load(@"..\boundary.txt", (s) =>
			{
				for (int i = 0; i < 70000; i++)
				{
					boundary[i * 4] = (byte)int.Parse(s.ReadLine());//left
					boundary[i * 4 + 1] = (byte)int.Parse(s.ReadLine());//top
					boundary[i * 4 + 2] = (byte)int.Parse(s.ReadLine());//right
					boundary[i * 4 + 3] = (byte)int.Parse(s.ReadLine());//bottom
				}
			});
		}
		public static void getBoundary(int idx, out int left, out int top, out int right, out int bottom)
		{
			left = boundary[idx * 4];
			top = boundary[idx * 4 + 1];
			right = boundary[idx * 4 + 2];
			bottom = boundary[idx * 4 + 3];
		}

		public static Random rnd = new Random();
		public static void rotate(int idx, int[] outdata)
		{
			int Width = 28;
			int Heigh = 28;
			int Selectx1;
			int Selectx2;
			int Selecty1;
			int Selecty2;
			getBoundary(idx, out Selectx1, out Selecty1, out Selectx2, out Selecty2);

			double Wid = Math.Abs(Selectx1 - Selectx2);
			double Hei = Math.Abs(Selecty1 - Selecty2);
			int xC = (Selectx1 + Selectx2) / 2;
			int yC = (Selecty1 + Selecty2) / 2;
			//r = (Math.Min(Wid, Hei) + 1) ^ 2;
			double r1 = Math.Max(Wid, Hei);


			double Angle = (rnd.NextDouble() - 0.5) / 4;
			int shiftX = (int)((rnd.NextDouble()) * (28 - Wid) * 0.92 + Wid / 2) + 1;
			int shiftY = (int)((rnd.NextDouble()) * (28 - Hei) * 0.92 + Hei / 2) + 1;
			//double Angle = 0;
			//int shiftX = (int)(Wid / 2);
			//int shiftY = (int)(Hei / 2);

			double s = Angle;// / HUDU
			double cs = Math.Cos(s);
			double ss = Math.Sin(s);
			int pageidx = idx * 28 * 28;
			for (double j = -r1 + 1; j < r1 - 1; j += 1)
			{
				for (double i = -r1 + 1; i <= r1 - 1; i += 1)
				{
					double x = i * cs + j * ss;
					double y = j * cs - i * ss;
					int x_big = (int)(x) + 1;
					int y_big = (int)(y) + 1;
					int x_sml = (int)(x);
					int y_sml = (int)(y);
					int a_color = 0;
					int b_color = 0;
					int c_color = 0;
					int d_color = 0;
					if (x_big + xC >= 0 && x_big + xC < Width && y_big + yC >= 0 && y_big + yC < Heigh)
					{
						//int temp_color = indata[(x_big + xC) * Width + y_big + yC];
						a_color = buffimg[pageidx + (y_big + yC) * 28 + x_big + xC];
						//a_color_b = (temp_color And &HFF)
						//a_color_g = ((temp_color And 65280) \ &H100)
						//a_color_r = ((temp_color And &HFF0000) \ &H10000)
					}
					if (x_big + xC >= 0 && x_big + xC < Width && y_sml + yC >= 0 && y_sml + yC < Heigh)
					{
						//int temp_color = indata[(x_big + xC) * Width + y_sml + yC];
						b_color = buffimg[pageidx + (y_sml + yC) * 28 + x_big + xC];
						//b_color_b = (temp_color And &HFF)
						//b_color_g = ((temp_color And 65280) \ &H100)
						//b_color_r = ((temp_color And &HFF0000) \ &H10000)
					}
					if (x_sml + xC >= 0 && x_sml + xC < Width && y_big + yC >= 0 && y_big + yC < Heigh)
					{
						//int temp_color = indata[(x_sml + xC) * Width + y_big + yC];
						c_color = buffimg[pageidx + (y_big + yC) * 28 + x_sml + xC];
						//c_color_b = (temp_color And &HFF)
						//c_color_g = ((temp_color And 65280) \ &H100)
						//c_color_r = ((temp_color And &HFF0000) \ &H10000)
					}
					if (x_sml + xC >= 0 && x_sml + xC < Width && y_sml + yC >= 0 && y_sml + yC < Heigh)
					{
						//int temp_color = indata[(x_sml + xC) * Width + y_sml + yC];
						d_color = buffimg[pageidx + (y_sml + yC) * 28 + x_sml + xC];
						//d_color_b = (temp_color And &HFF)
						//d_color_g = ((temp_color And 65280) \ &H100)
						//d_color_r = ((temp_color And &HFF0000) \ &H10000)
					}

					double t_color = (a_color * (y - y_sml) + b_color * (y_big - y)) * (x - x_sml) + (c_color * (y - y_sml) + d_color * (y_big - y)) * (x_big - x);
					//t_color_g = (a_color_g * (y - y_sml) + b_color_g * (y_big - y)) * (x - x_sml) + (c_color_g * (y - y_sml) + d_color_g * (y_big - y)) * (x_big - x)
					//t_color_b = (a_color_b * (y - y_sml) + b_color_b * (y_big - y)) * (x - x_sml) + (c_color_b * (y - y_sml) + d_color_b * (y_big - y)) * (x_big - x)
					//t_color = t_color_r * &H10000 + t_color_g * &H100 + t_color_b
					int targetX = (int)(i + shiftX);
					int targetY = (int)(j + shiftY);
					//if (i + xC >= 0 && i + xC <= Width && j + yC >= 0 && j + yC <= Heigh)
					//{
					//	outdata[((int)i + xC) * Width + (int)j + yC] = (int)t_color;
					//}
					if (targetX >= 0 && targetX < Width && targetY >= 0 && targetY < Heigh)
					{
						outdata[targetY * Width + targetX] = (int)t_color;
					}
				}
			}

		}

		static MNISTData()
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

			try
			{
				loadBoundary();
			}
			catch { }

		}

		static Vol v = new Vol(28, 28, 1, 0.0f);
		public static Vol getImg(int idx)
		{
			//idx = MNIST.getGoodData(idx);
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
		public static Vol getImg(int[] data)
		{
			int[] img = data;
			for (var i = 0; i < 28 * 28; i++)
			{
				v.w[i] = (float)img[i] / 255.0f;
			}
			return v;
		}
		public static void getImgV(int[] data, Vol v2)
		{
			int[] img = data;
			for (var i = 0; i < 28 * 28; i++)
			{
				v2.w[i] = (float)img[i] / 255.0f;
			}
		}
		static public Bitmap getBmp(int idx)
		{
			//idx = MNIST.getGoodData(idx);
			Bitmap b = new Bitmap(28, 28);

			for (int j = 0; j < 28; j++)
			{
				for (int i = 0; i < 28; i++)
				{
					int val = buffimg[idx * 28 * 28 + j * 28 + i];
					b.SetPixel(i, j, Color.FromArgb(val, val, val));
				}
			}
			return b;
		}
		static public Bitmap getBmp(int[] data)
		{
			//idx = MNIST.getGoodData(idx);
			Bitmap b = new Bitmap(28, 28);

			for (int j = 0; j < 28; j++)
			{
				for (int i = 0; i < 28; i++)
				{
					int val = data[j * 28 + i];
					if (val > 255) val = 255;
					if (val < 0) val = 0;
					b.SetPixel(i, j, Color.FromArgb(val, val, val));
				}
			}
			return b;
		}
		static public int getLbl(int idx)
		{
			//idx = MNIST.getGoodData(idx);
			return bufflbl[idx];
		}

	}


	public class MNIST
	{


		public class MainNet : Net
		{
			public ConvLayer cv1;
			public ConvLayer cv2;
			public ConvLayer cv3;
			public FullyConnLayer fc144;
			public FullyConnLayer fc10;

			public void save(TextWriter s)
			{
				cv1.save(s);
				cv2.save(s);
				cv3.save(s);
				fc144.save(s);
				fc10.save(s);
			}
			public void load(TextReader s)
			{
				cv1.load(s);
				cv2.load(s);
				cv3.load(s);
				fc144.load(s);
				fc10.load(s);
			}
			string cv1data = @"

			0,0,1,1,
			0,0,1,1,
			0,0,1,1,
			0,0,1,1,
				1,1,0,0,
				1,1,0,0,
				1,1,0,0,
				1,1,0,0,
			1,0,0,1,
			1,0,0,1,
			1,0,0,1,
			1,0,0,1,
				0,1,1,0,
				0,1,1,0,
				0,1,1,0,
				0,1,1,0,

			1,1,1,1,
			1,1,1,1,
			0,0,0,0,
			0,0,0,0,
				0,0,0,0,
				0,0,0,0,
				1,1,1,1,
				1,1,1,1,
			1,1,1,1,
			0,0,0,0,
			0,0,0,0,
			1,1,1,1,
				0,0,0,0,
				1,1,1,1,
				1,1,1,1,
				0,0,0,0,

			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1,
				0,0,0,1,
				0,0,1,0,
				0,1,0,0,
				1,0,0,0,
			1,0,0,1,
			0,1,1,0,
			0,1,1,0,
			1,0,0,1,
				0,1,1,0,
				1,1,1,1,
				1,1,1,1,
				0,1,1,0,

			0,1,0,0,
			0,1,0,0,
			0,1,1,1,
			0,0,0,0,
				0,0,0,0,
				1,1,1,0,
				0,0,1,0,
				0,0,1,0,
			0,0,1,0,
			0,0,1,0,
			1,1,1,0,
			0,0,0,0,
				0,0,0,0,
				0,1,1,1,
				0,1,0,0,
				0,1,0,0,
			";

			public void init()
			{
				//this, new Trainer.Option() 
				trainer = new AdaDeltaTrainer(66) { l2_decay = 0.001f };//0.001f

				cv1 = new ConvLayer(sx: 4, sy: 4, filters: 16, stride: 2, pad: 2, bias_pref: 0.1f, act: new ReluLayer());
				cv2 = new ConvLayer(sx: 4, sy: 4, filters: 16, stride: 2, pad: 2, bias_pref: 0.1f, act: new ReluLayer());
				cv3 = new ConvLayer(sx: 4, sy: 4, filters: 32, stride: 2, pad: 2, bias_pref: 0.1f, act: new ReluLayer());
				fc144 = new FullyConnLayer(num_neurons: 288, bias_pref: 0.1f, act: new ReluLayer());
				fc10 = new FullyConnLayer(num_neurons: 11, bias_pref: 0.1f);

				Add(new InputLayer(out_sx: 28, out_sy: 28, out_depth: 1));
				Add(cv1);
				Add(cv2);
				Add(cv3);
				Add(fc144);
				Add(fc10);
				Add(new SoftmaxLayer());

				string[] ss = cv1data.Split(',');
				for (int i = 0; i < 256; i++)
				{
					cv1.filters.w[i] = float.Parse(ss[i]);
				}
				//cv1.noUpdate = true;

			}


			Vol v = new Vol(28, 28, 1, 0.0f);
			int[] data = new int[28 * 28];
			public void train()
			{
				DataSet ds = new DataSet();
				//train
				for (int i = 0; i < trainer.batchSize; i++)
				{
					int trainIndex = (int)(MNISTData.rnd.NextDouble() * 70000);
					if ((i + 1) % 10 == 0)
					{
						for (int n = 0; n < 28 * 28; n++)
						{
							data[n] = 0;
						}
						ds.predict = 10;
					}
					else if ((i + 2) % 10 == 0)
					{
						for (int n = 0; n < 28 * 28; n++)
						{
							data[n] = (int)(MNISTData.rnd.NextDouble() * 255);
						}
						ds.predict = 10;
					}
					else
					{
						MNISTData.rotate(trainIndex, data);
						ds.predict = MNISTData.getLbl(trainIndex);
					}
					MNISTData.getImgV(data, v);

					train(v, ds);
				}

			}

			public float test()
			{
				float accu = 0;
				DataSet ds = new DataSet();

				for (int j = 0; j < 1000; j++)//test
				{
					int trainIndex = (int)(MNISTData.rnd.NextDouble() * 70000);

					MNISTData.rotate(trainIndex, data);
					ds.predict = MNISTData.getLbl(trainIndex);

					MNISTData.getImgV(data, v);

					var o = forward(v);
					int predict = MNIST.GetPredicted(o);
					if (ds.predict == predict)
					{
						accu += 1.0f;
					}
				}
				return accu / 1000.0f;
			}
		}


		public class RegNet : Net
		{
			public ConvLayer cv1;
			public ConvLayer cv2;
			public ConvLayer cv3;
			public FullyConnLayer fc144;
			public ConvLayer ucv0;
			public ConvLayer ucv1;
			public ConvLayer ucv2;

			public void save(TextWriter s)
			{
			}
			public void load(TextReader s)
			{
			}

			public void init()
			{
				trainer = new AdaDeltaTrainer(5) { ro = 0.5f, l2_decay = 0.001f };//0.001f learning_rate = 0.1f, 
				//trainer = new SGDTrainer(5) { learning_rate = 0.0002f, l2_decay = 0.001f };//0.001f

				cv1 = new ConvLayer(sx: 4, sy: 4, filters: 16, stride: 2, pad: 2, bias_pref: 0.1f, act: new ReluLayer());
				cv2 = new ConvLayer(sx: 4, sy: 4, filters: 16, stride: 2, pad: 2, bias_pref: 0.1f, act: new ReluLayer());
				cv3 = new ConvLayer(sx: 4, sy: 4, filters: 32, stride: 2, pad: 2, bias_pref: 0.1f, act: new ReluLayer());
				fc144 = new FullyConnLayer(num_neurons: 288, bias_pref: 0.1f, act: new ReluLayer());
				ucv0 = new ConvLayer(sx: 4, sy: 4, filters: 16, unstride: 2, pad: 2, bias_pref: 0.1f, act: new ReluLayer());
				ucv1 = new ConvLayer(sx: 4, sy: 4, filters: 8, unstride: 2, pad: 2, bias_pref: 0.1f, act: new ReluLayer());
				ucv2 = new ConvLayer(sx: 4, sy: 4, filters: 1, unstride: 2, pad: 1, bias_pref: 0.1f);

				Add(new InputLayer(out_sx: 28, out_sy: 28, out_depth: 1));
				Add(cv1);
				Add(cv2);
				Add(cv3);
				Add(fc144);
				Add(new ReshapeLayer(out_sx: 4, out_sy: 4, out_depth: 32));
				Add(ucv0);
				Add(ucv1);
				Add(ucv2);
				Add(new RegressionLayer());

			}


			public Vol train()
			{
				DataSet ds = new DataSet();
				//train
				for (int i = 0; i < trainer.batchSize; i++)
				{
					int trainIndex = i%3;//(int)(MNISTData.rnd.NextDouble() * 70000);


					var v = MNISTData.getImg(trainIndex);
					ds.data = v;

					train(v, ds);
				}
				return ucv2.out_act;
			}

		}

		public void Init()
		{
		}



		public class Pack
		{
			public float predict;
			public int val;
		}
		public static string report(Vol v, int index, int correctVal)
		{
			List<Pack> list = new List<Pack>();
			for (int i = 0; i < 11; i++)
			{
				list.Add(new Pack() { predict = v.w[i], val = i });
			}

			list.Sort((a, b) => ((a.predict > b.predict) ? -1 : 1));

			string ret = index + "\t";
			ret += correctVal + "\t";
			ret += list[0].val + "\t";
			if (correctVal != list[0].val)
			{
				ret += "X\t";
				ret += "L\t";
			}
			else
			{
				ret += "O\t";
				if (list[0].predict > 0.9f)
				{
					ret += "H\t";
				}
				else
				{
					ret += "L\t";
				}
			}

			for (int i = 0; i < 3; i++)
			{
				ret += list[i].val + ":" + list[i].predict + "\t";
			}
			return ret;
		}
		public static int GetPredicted(Vol v)
		{

			List<Pack> list = new List<Pack>();
			for (int i = 0; i < 10; i++)
			{
				list.Add(new Pack() { predict = v.w[i], val = i });
			}

			list.Sort((a, b) => ((a.predict > b.predict) ? -1 : 1));

			return list[0].val;
		}

	}
}
