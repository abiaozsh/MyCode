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
		public static void log(string txt)
		{
			FileStream fs = new FileStream("log.txt", FileMode.Append, FileAccess.Write);
			StreamWriter sw = new StreamWriter(fs);
			sw.WriteLine(txt);
			sw.Flush();
			fs.Flush();
			fs.Close();
		}

		public static byte[] buffimg = new byte[1024 * 1024 * 64];
		public static byte[] bufflbl = new byte[1024 * 1024];

		public static int[] blackList = new int[70000];
		public static int goodIndex = 70000;
		public static void loadBlackList()
		{
			Util.load(@"..\blacklist.txt", (sr) =>
			{
				goodIndex = int.Parse(sr.ReadLine());
				for (int i = 0; i < 70000; i++)
				{
					blackList[i] = int.Parse(sr.ReadLine());
				}
			});
		}
		public static void saveBlackList()
		{
			Util.save(@"..\blacklist.txt", (sw) =>
			{
				sw.WriteLine(goodIndex);
				for (int i = 0; i < 70000; i++)
				{
					sw.WriteLine(blackList[i]);
				}
			});
		}

		public static int getGoodData(int idx)
		{
			if (idx > goodIndex)
			{
				throw new Exception();
			}
			return blackList[idx];
		}

		public static void removeData(int idx)
		{
			goodIndex--;
			int tmp = blackList[idx];
			blackList[idx] = blackList[goodIndex];
			blackList[goodIndex] = tmp;
		}

		public static void proc()
		{
			for (int i = 0; i < 70000; i++)
			{
				Bitmap b = _______getBmp(i);
				int l = _______getLbl(i);
				b.Save(@"E:\MNIST\imgs\" + l + "\\" + i + ".bmp", System.Drawing.Imaging.ImageFormat.Bmp);
			}
		}
		static public Bitmap _______getBmp(int idx)
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
		static public int _______getLbl(int idx)
		{
			return bufflbl[idx];
		}


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

			for (int i = 0; i < 70000; i++)
			{
				blackList[i] = i;
			}
		}

		public class First3CVNet : Net
		{
			public ConvLayer cv1;
			public ConvLayer cv2;
			public ConvLayer cv3;
			public FullyConnLayer fc144;
			public FullyConnLayer fc10;

			public Trainer trainer;

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

			public void init()
			{
				cv1 = new ConvLayer(sx: 4, sy: 4, filters: 8, stride: 2, pad: 2, bias_pref: 0.1f, act: new ReluLayer());
				cv2 = new ConvLayer(sx: 4, sy: 4, filters: 16, stride: 2, pad: 2, bias_pref: 0.1f, act: new ReluLayer());
				cv3 = new ConvLayer(sx: 4, sy: 4, filters: 32, stride: 2, pad: 2, bias_pref: 0.1f, act: new ReluLayer());
				fc144 = new FullyConnLayer(num_neurons: 288, bias_pref: 0.1f, act: new ReluLayer());
				fc10 = new FullyConnLayer(num_neurons: 10, bias_pref: 0.1f);

				Add(new InputLayer(out_sx: 28, out_sy: 28, out_depth: 1));
				Add(cv1);
				Add(cv2);
				Add(cv3);
				Add(fc144);
				Add(fc10);
				Add(new SoftmaxLayer());

				trainer = new Trainer(this, new Trainer.Option() { method = "adadelta", batch_size = 60, l2_decay = 0.001f });//0.001f
			}


			int trainIndex = 0;
			public void train()
			{
				//train
				for (int i = 0; i < trainer.batch_size; i++)
				{
					DataSet ds = new DataSet();
					trainIndex++;
					if (trainIndex >= 50000) trainIndex = 0;
					ds.predict = MNIST.getLbl(trainIndex);
					var v = MNIST.getImg(trainIndex);
					trainer.train(v, ds);
				}

			}

			int testIndex = 50000;
			public float test()
			{
				float accu = 0;

				for (int j = 0; j < 1000; j++)//test
				{
					DataSet ds = new DataSet();
					ds.predict = MNIST.getLbl(testIndex);
					var v = MNIST.getImg(testIndex);
					testIndex++;
					if (testIndex >= goodIndex) testIndex = 50000;
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

		public class Classifier2DNetTrainer : Net
		{
			public ConvLayer cv1;
			public ConvLayer cv2;
			public ConvLayer cv3;
			public FullyConnLayer fc144;
			public FullyConnLayer fc100;
			public FullyConnLayer fc2;
			public FullyConnLayer fc20A;
			public FullyConnLayer fc20B;
			public FullyConnLayer fc10;

			public Trainer trainer;

			public void save(TextWriter s)
			{
				fc100.save(s);
				fc2.save(s);
			}

			public void init()
			{
				cv1 = new ConvLayer(sx: 4, sy: 4, filters: 8, stride: 2, pad: 2, bias_pref: 0.1f, act: new ReluLayer());
				cv2 = new ConvLayer(sx: 4, sy: 4, filters: 16, stride: 2, pad: 2, bias_pref: 0.1f, act: new ReluLayer());
				cv3 = new ConvLayer(sx: 4, sy: 4, filters: 32, stride: 2, pad: 2, bias_pref: 0.1f, act: new ReluLayer());
				fc144 = new FullyConnLayer(num_neurons: 288, bias_pref: 0.1f, act: new ReluLayer());
				fc100 = new FullyConnLayer(num_neurons: 100, bias_pref: 0.1f, act: new ReluLayer());
				fc2 = new FullyConnLayer(num_neurons: 2, bias_pref: 0.1f, act: new TanhLayer());
				fc20A = new FullyConnLayer(num_neurons: 20, bias_pref: 0.1f, act: new ReluLayer());
				fc20B = new FullyConnLayer(num_neurons: 20, bias_pref: 0.1f, act: new ReluLayer());
				fc10 = new FullyConnLayer(num_neurons: 10, bias_pref: 0.1f);

				Add(new InputLayer(out_sx: 28, out_sy: 28, out_depth: 1));
				Add(cv1);
				Add(cv2);
				Add(cv3);
				Add(fc100);
				Add(fc144);
				Add(fc2);
				Add(fc20A);
				Add(fc20B);
				Add(fc10);
				Add(new SoftmaxLayer());

				Util.load(@"..\first3L1FC.txt", (sr) =>
				{
					cv1.load(sr); cv1.noUpdate = true;
					cv2.load(sr); cv2.noUpdate = true;
					cv3.load(sr); cv3.noUpdate = true;
					fc144.load(sr); fc144.noUpdate = true;
				});


				trainer = new Trainer(this, new Trainer.Option() { method = "adadelta", batch_size = 60, l2_decay = 0.001f });//0.001f
			}


			int trainIndex = 0;
			int testIndex = 60000;

			public void train()
			{
				//train
				for (int i = 0; i < trainer.batch_size; i++)
				{
					DataSet ds = new DataSet();
					trainIndex++;
					if (trainIndex >= 60000) trainIndex = 0;
					ds.predict = MNIST.getLbl(trainIndex);
					var v = MNIST.getImg(trainIndex);
					trainer.train(v, ds);
				}

			}
			public float test()
			{
				float accu = 0;

				for (int j = 0; j < 1000; j++)//test
				{
					DataSet ds = new DataSet();
					ds.predict = MNIST.getLbl(testIndex);
					var v = MNIST.getImg(testIndex);
					testIndex++;
					if (testIndex >= 70000) testIndex = 60000;
					var o = forward(v);
					int predict = MNIST.GetPredicted(o);
					if (ds.predict == predict)
					{
						accu += 1.0f;
					}
				}
				return accu / 1000.0f;
			}
			public Bitmap display()
			{

				Random rnd = new Random();
				Bitmap b = new Bitmap(100, 100);
				for (int i = 0; i < 1000; i++)
				{
					int index = (int)(rnd.NextDouble() * 70000);
					var v = MNIST.getImg(index);
					int lbl = MNIST.getLbl(index);
					//var fco = mainNet.forward(v);
					//var fco2 = img2XYNet.forward(v);
					forward(v);
					var fco2 = fc2.act.out_act;
					int x = (int)(fco2.w[0] * MNIST.visScale + 50);
					int y = (int)(fco2.w[1] * MNIST.visScale + 50);
					if (x >= 100) x = 99;
					if (x < 0) x = 0;
					if (y >= 100) y = 99;
					if (y < 0) y = 0;
					Color color = MNIST.cDict[lbl];
					b.SetPixel(x, y, color);
				}
				return b;
			}
		}

		public class _2D_10NetTrainer : Net
		{
			public ConvLayer cv1;
			public ConvLayer cv2;
			public ConvLayer cv3;
			public FullyConnLayer fc144;
			public FullyConnLayer fc100;
			public FullyConnLayer fc2;
			public FullyConnLayer fc20A;
			public FullyConnLayer fc20B;
			public FullyConnLayer fc10;

			public Trainer trainer;

			public void save(TextWriter s)
			{
				fc20A.save(s);
				fc20B.save(s);
				fc10.save(s);
			}

			public void init()
			{
				cv1 = new ConvLayer(sx: 4, sy: 4, filters: 8, stride: 2, pad: 2, bias_pref: 0.1f, act: new ReluLayer());
				cv2 = new ConvLayer(sx: 4, sy: 4, filters: 16, stride: 2, pad: 2, bias_pref: 0.1f, act: new ReluLayer());
				cv3 = new ConvLayer(sx: 4, sy: 4, filters: 32, stride: 2, pad: 2, bias_pref: 0.1f, act: new ReluLayer());
				fc144 = new FullyConnLayer(num_neurons: 288, bias_pref: 0.1f, act: new ReluLayer());
				fc100 = new FullyConnLayer(num_neurons: 100, bias_pref: 0.1f, act: new ReluLayer());
				fc2 = new FullyConnLayer(num_neurons: 2, bias_pref: 0.1f, act: new TanhLayer());
				fc20A = new FullyConnLayer(num_neurons: 20, bias_pref: 0.1f, act: new ReluLayer());
				fc20B = new FullyConnLayer(num_neurons: 20, bias_pref: 0.1f, act: new ReluLayer());
				fc10 = new FullyConnLayer(num_neurons: 10, bias_pref: 0.1f);

				Add(new InputLayer(out_sx: 28, out_sy: 28, out_depth: 1));
				Add(cv1);
				Add(cv2);
				Add(cv3);
				Add(fc100);
				Add(fc144);
				Add(fc2);
				Add(fc20A);
				Add(fc20B);
				Add(fc10);
				Add(new SoftmaxLayer());

				Util.load(@"..\first3L1FC.txt", (sr) =>
				{
					cv1.load(sr); cv1.noUpdate = true;
					cv2.load(sr); cv2.noUpdate = true;
					cv3.load(sr); cv3.noUpdate = true;
					fc144.load(sr); fc144.noUpdate = true;
				});

				Util.load(@"..\net2Dclassifier.txt", (sr) =>
				{
					fc100.load(sr); fc100.noUpdate = true;
					fc2.load(sr); fc2.noUpdate = true;
				});

				trainer = new Trainer(this, new Trainer.Option() { method = "adadelta", batch_size = 60, l2_decay = 0.001f });//0.001f
			}


			int trainIndex = 0;
			int testIndex = 60000;

			public void train()
			{
				//train
				for (int i = 0; i < trainer.batch_size; i++)
				{
					DataSet ds = new DataSet();
					trainIndex++;
					if (trainIndex >= 60000) trainIndex = 0;
					ds.predict = MNIST.getLbl(trainIndex);
					var v = MNIST.getImg(trainIndex);
					trainer.train(v, ds);
				}

			}
			public float test()
			{
				float accu = 0;

				for (int j = 0; j < 1000; j++)//test
				{
					DataSet ds = new DataSet();
					ds.predict = MNIST.getLbl(testIndex);
					var v = MNIST.getImg(testIndex);
					testIndex++;
					if (testIndex >= 70000) testIndex = 60000;
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

		public class _2D_ImgNetTrainer : Net
		{
			public ConvLayer cv1;
			public ConvLayer cv2;
			public ConvLayer cv3;
			public FullyConnLayer fc144;
			public FullyConnLayer fc100;
			public FullyConnLayer fc2;

			public FullyConnLayer fc1;
			public ConvLayer ucv0;
			public ConvLayer ucv1;
			public ConvLayer ucv2;

			public Trainer trainer;

			public void save(TextWriter s)
			{
				fc1.save(s);
				ucv0.save(s);
				ucv1.save(s);
				ucv2.save(s);
			}

			public void init()
			{
				cv1 = new ConvLayer(sx: 4, sy: 4, filters: 8, stride: 2, pad: 2, bias_pref: 0.1f, act: new ReluLayer());
				cv2 = new ConvLayer(sx: 4, sy: 4, filters: 16, stride: 2, pad: 2, bias_pref: 0.1f, act: new ReluLayer());
				cv3 = new ConvLayer(sx: 4, sy: 4, filters: 32, stride: 2, pad: 2, bias_pref: 0.1f, act: new ReluLayer());
				fc144 = new FullyConnLayer(num_neurons: 288, bias_pref: 0.1f, act: new ReluLayer());
				fc100 = new FullyConnLayer(num_neurons: 100, bias_pref: 0.1f, act: new ReluLayer());
				fc2 = new FullyConnLayer(num_neurons: 2, bias_pref: 0.1f, act: new TanhLayer());

				fc1 = new FullyConnLayer(num_neurons: 4 * 4 * 32, bias_pref: 0.1f, act: new TanhLayer());
				ucv0 = new ConvLayer(sx: 4, sy: 4, filters: 16, unstride: 2, pad: 2, bias_pref: 0.1f, act: new ReluLayer());
				ucv1 = new ConvLayer(sx: 4, sy: 4, filters: 8, unstride: 2, pad: 2, bias_pref: 0.1f, act: new ReluLayer());
				ucv2 = new ConvLayer(sx: 4, sy: 4, filters: 1, unstride: 2, pad: 1, bias_pref: 0.1f);

				Add(new InputLayer(out_sx: 28, out_sy: 28, out_depth: 1));
				Add(cv1);
				Add(cv2);
				Add(cv3);
				Add(fc100);
				Add(fc144);
				Add(fc2);

				Add(fc1);
				Add(new ReshapeLayer(out_sx: 4, out_sy: 4, out_depth: 32));
				Add(ucv0);
				Add(ucv1);
				Add(ucv2);
				Add(new RegressionLayer());

				Util.load(@"..\first3L1FC.txt", (sr) =>
				{
					cv1.load(sr); cv1.noUpdate = true;
					cv2.load(sr); cv2.noUpdate = true;
					cv3.load(sr); cv3.noUpdate = true;
					fc144.load(sr); fc144.noUpdate = true;
				});

				Util.load(@"..\net2Dclassifier.txt", (sr) =>
				{
					fc100.load(sr); fc100.noUpdate = true;
					fc2.load(sr); fc2.noUpdate = true;
				});

				trainer = new Trainer(this, new Trainer.Option() { method = "adadelta", batch_size = 60, l2_decay = 0.001f });//0.001f
			}

			int trainIndex = 0;
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


		public class _2D_10Net : Net
		{
			public void init(FullyConnLayer fc20A, FullyConnLayer fc20B, FullyConnLayer fc10)
			{
				Add(new InputLayer(out_sx: 1, out_sy: 1, out_depth: 2));
				Add(fc20A);
				Add(fc20B);
				Add(fc10);
				Add(new SoftmaxLayer());
			}

			public void init()
			{
				FullyConnLayer fc20A = new FullyConnLayer(num_neurons: 20, bias_pref: 0.1f, act: new ReluLayer());
				FullyConnLayer fc20B = new FullyConnLayer(num_neurons: 20, bias_pref: 0.1f, act: new ReluLayer());
				FullyConnLayer fc10 = new FullyConnLayer(num_neurons: 10, bias_pref: 0.1f);

				Add(new InputLayer(out_sx: 1, out_sy: 1, out_depth: 2));
				Add(fc20A);
				Add(fc20B);
				Add(fc10);
				Add(new SoftmaxLayer());

				Util.load(@"..\2D210.txt", (sr) =>
				{
					fc20A.load(sr); fc20A.noUpdate = true;
					fc20B.load(sr); fc20B.noUpdate = true;
					fc10.load(sr); fc10.noUpdate = true;
				});

			}
			Vol regen = new Vol(1, 1, 2, 0);
			public string proc(int X, int Y)
			{
				float x = (X - 50f) / MNIST.visScale;
				float y = (Y - 50f) / MNIST.visScale;
				regen.w[0] = x;
				regen.w[1] = y;

				var _out = forward(regen);
				String s = MNIST.report(_out,-1,-1);

				return s;
			}

		}

		public class _2D_ImgNet : Net
		{
			public void init()
			{
				FullyConnLayer fc1 = new FullyConnLayer(num_neurons: 4 * 4 * 32, bias_pref: 0.1f, act: new TanhLayer());
				ConvLayer ucv0 = new ConvLayer(sx: 4, sy: 4, filters: 16, unstride: 2, pad: 2, bias_pref: 0.1f, act: new ReluLayer());
				ConvLayer ucv1 = new ConvLayer(sx: 4, sy: 4, filters: 8, unstride: 2, pad: 2, bias_pref: 0.1f, act: new ReluLayer());
				ConvLayer ucv2 = new ConvLayer(sx: 4, sy: 4, filters: 1, unstride: 2, pad: 1, bias_pref: 0.1f);

				Add(new InputLayer(out_sx: 1, out_sy: 1, out_depth: 2));
				Add(fc1);
				Add(new ReshapeLayer(out_sx: 4, out_sy: 4, out_depth: 32));
				Add(ucv0);
				Add(ucv1);
				Add(ucv2);
				Add(new RegressionLayer());

				Util.load(@"..\2D2Img.txt", (sr) =>
				{
					fc1.load(sr); fc1.noUpdate = true;
					ucv0.load(sr); ucv0.noUpdate = true;
					ucv1.load(sr); ucv1.noUpdate = true;
					ucv2.load(sr); ucv2.noUpdate = true;
				});
			}

			public void init(FullyConnLayer fc1, ConvLayer ucv0, ConvLayer ucv1, ConvLayer ucv2)
			{
				Add(new InputLayer(out_sx: 1, out_sy: 1, out_depth: 2));
				Add(fc1);
				Add(new ReshapeLayer(out_sx: 4, out_sy: 4, out_depth: 32));
				Add(ucv0);
				Add(ucv1);
				Add(ucv2);
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

		public class Img_2DNet : Net
		{
			public ConvLayer cv1;
			public ConvLayer cv2;
			public ConvLayer cv3;
			public FullyConnLayer fc144;
			public FullyConnLayer fc100;
			public FullyConnLayer fc2;

			public void init()
			{
				cv1 = new ConvLayer(sx: 4, sy: 4, filters: 8, stride: 2, pad: 2, bias_pref: 0.1f, act: new ReluLayer());
				cv2 = new ConvLayer(sx: 4, sy: 4, filters: 16, stride: 2, pad: 2, bias_pref: 0.1f, act: new ReluLayer());
				cv3 = new ConvLayer(sx: 4, sy: 4, filters: 32, stride: 2, pad: 2, bias_pref: 0.1f, act: new ReluLayer());
				fc144 = new FullyConnLayer(num_neurons: 288, bias_pref: 0.1f, act: new ReluLayer());
				fc100 = new FullyConnLayer(num_neurons: 100, bias_pref: 0.1f, act: new ReluLayer());
				fc2 = new FullyConnLayer(num_neurons: 2, bias_pref: 0.1f, act: new TanhLayer());

				Add(new InputLayer(out_sx: 28, out_sy: 28, out_depth: 1));
				Add(cv1);
				Add(cv2);
				Add(cv3);
				Add(fc100);
				Add(fc144);
				Add(fc2);

				Util.load(@"..\first3L1FC.txt", (sr) =>
				{
					cv1.load(sr); cv1.noUpdate = true;
					cv2.load(sr); cv2.noUpdate = true;
					cv3.load(sr); cv3.noUpdate = true;
					fc144.load(sr); fc144.noUpdate = true;
				});

				Util.load(@"..\net2Dclassifier.txt", (sr) =>
				{
					fc100.load(sr); fc100.noUpdate = true;
					fc2.load(sr); fc2.noUpdate = true;
				});

			}

			public Bitmap display(int count)
			{
				Random rnd = new Random();
				Bitmap b = new Bitmap(100, 100);
				for (int i = 0; i < count; i++)
				{
					int index = (int)(rnd.NextDouble() * 70000);
					var v = MNIST.getImg(index);
					int lbl = MNIST.getLbl(index);
					//var fco = mainNet.forward(v);
					//var fco2 = img2XYNet.forward(v);
					forward(v);
					var fco2 = fc2.act.out_act;
					int x = (int)(fco2.w[0] * MNIST.visScale + 50);
					int y = (int)(fco2.w[1] * MNIST.visScale + 50);
					if (x >= 100) x = 99;
					if (x < 0) x = 0;
					if (y >= 100) y = 99;
					if (y < 0) y = 0;
					Color color = MNIST.cDict[lbl];
					b.SetPixel(x, y, color);
				}
				return b;
			}
		}





		public void Init()
		{
		}


		public static Color[] cDict = new Color[]{
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

		public static float visScale = 50.0f;
		public Bitmap display()
		{
			//"red", "blue", "green", "black", "magenta", "cyan", "purple", "aqua", "olive", "lime", "navy"
			Random rnd = new Random();
			Bitmap b = new Bitmap(100, 100);

			//			for (int i = 0; i < 1000; i++)
			//			{
			//				int index = (int)(rnd.NextDouble() * 70000);
			//				var v = getImg(index);
			//				int lbl = getLbl(index);
			//				//var fco = mainNet.forward(v);
			//				//var fco2 = img2XYNet.forward(v);
			//				classifierNet.forward(v);
			//				var fco2 = classifierNet.fc2.act.out_act;
			//				int x = (int)(fco2.w[0] * visScale + 50);
			//				int y = (int)(fco2.w[1] * visScale + 50);
			//				if (x >= 100) x = 99;
			//				if (x < 0) x = 0;
			//				if (y >= 100) y = 99;
			//				if (y < 0) y = 0;
			//				Color color = cDict[lbl];
			//				b.SetPixel(x, y, color);
			//			}

			return b;
		}


		static Vol v = new Vol(28, 28, 1, 0.0f);
		public static Vol getImg(int idx)
		{
			idx = MNIST.getGoodData(idx);
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
			idx = MNIST.getGoodData(idx);
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
			idx = MNIST.getGoodData(idx);
			return bufflbl[idx];
		}

		public class Pack
		{
			public float predict;
			public int val;
		}
		public static string report(Vol v, int index, int correctVal)
		{
			List<Pack> list = new List<Pack>();
			for (int i = 0; i < 10; i++)
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
