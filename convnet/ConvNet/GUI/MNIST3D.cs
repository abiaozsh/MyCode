using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using ConvNet;

namespace GUI
{
	public class MNIST3D
	{

		//1维 2分类
		//2维 4分类
		//3维 8分类
		//4维 16分类
		//5维 32分类
		//6维 64分类
		//7维 128分类
		//8维 256分类
		//9维 512分类
		//10维 1024分类

		public class CoTrainNet : Net//net4
		{
			//输入图片，输出预测
			public ConvLayer cv1;
			public ConvLayer cv2;
			public ConvLayer cv3;
			public FullyConnLayer fc144;
			public FullyConnLayer fc2;
			public FullyConnLayer fc20A;
			public FullyConnLayer fc20B;
			public FullyConnLayer fc10;
			public void save(TextWriter s)
			{
				cv1.save(s);
				cv2.save(s);
				cv3.save(s);
				fc144.save(s);
				fc2.save(s);
				fc20A.save(s);
				fc20B.save(s);
				fc10.save(s);
			}
			public void load(TextReader s)
			{
				cv1.load(s);
				cv2.load(s);
				cv3.load(s);
				fc144.load(s);
				fc2.load(s);
				fc20A.load(s);
				fc20B.load(s);
				fc10.load(s);
			}

			public Trainer trainer;

			//联合训练网
			public void init()
			{
				cv1 = new ConvLayer(sx: 4, sy: 4, filters: 4, stride: 2, pad: 2, bias_pref: 0.1f);//ReluLayer
				cv2 = new ConvLayer(sx: 4, sy: 4, filters: 8, stride: 2, pad: 2, bias_pref: 0.1f);//ReluLayer
				cv3 = new ConvLayer(sx: 4, sy: 4, filters: 16, stride: 2, pad: 2, bias_pref: 0.1f);//ReluLayer
				fc144 = new FullyConnLayer(num_neurons: 144, bias_pref: 0.0f);
				fc2 = new FullyConnLayer(num_neurons: 3, bias_pref: 0.1f);
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

				Add(fc144);
				Add(new TanhLayer());

				Add(fc2);
				Add(new ReluLayer());

				Add(fc20A);
				Add(new ReluLayer());

				Add(fc20B);
				Add(new ReluLayer());

				Add(fc10);
				Add(new ReluLayer());
				Add(new SoftmaxLayer());

				trainer = new Trainer(this, new Trainer.Option() { learning_rate = 10.0f, method = "adadelta", batch_size = 20, l2_decay = 0.0f });//0.001f
			}

			static Random r = new Random();
			int trainIndex = (int)(r.NextDouble() * 69999);

			public void train()
			{
				for (int i = 0; i < trainer.batch_size; i++)
				{
					DataSet ds = new DataSet();
					trainIndex++;
					if (trainIndex >= 70000)
					{
						trainIndex = 0;
					}
					ds.predict = MNIST.getLbl(trainIndex);

					var v = MNIST.getImg(trainIndex);
					trainer.train(v, ds);
				}
			}
		}
		public class CoTrainTestNet : Net
		{
			//输入 x,y 输出预测 0~9
			public void init(FullyConnLayer fc20A, FullyConnLayer fc20B, FullyConnLayer fc10)
			{
				Add(new InputLayer(out_sx: 1, out_sy: 1, out_depth: 3));
				Add(new TanhLayer());

				Add(fc20A);
				Add(new ReluLayer());

				Add(fc20B);
				Add(new ReluLayer());

				Add(fc10);
				Add(new ReluLayer());
				Add(new SoftmaxLayer());
			}

			Vol regen = new Vol(1, 1, 3, 0);
		//	public string proc(int X, int Y, int Z)
		//	{
		//		float x = (X - 50f) / MNIST.visScale;
		//		float y = (Y - 50f) / MNIST.visScale;
		//		float z = (Z - 50f) / MNIST.visScale;
		//		regen.w[0] = x;
		//		regen.w[1] = y;
		//		regen.w[1] = z;
		//
		//		var _out = forward(regen);
		//		String s = MNIST.report(_out);
		//
		//		return s;
		//	}

		}

		public class XYZ2ImgNet : Net//net2
		{
			// 输入二维向量 全连接 再多层反卷积 输出图像
			// 和主网络共享层
			public void init(FullyConnLayer fc1, ConvLayer ucv0, ConvLayer ucv1, ConvLayer ucv2)
			{
				Add(new InputLayer(out_sx: 1, out_sy: 1, out_depth: 3));
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

			Vol regen = new Vol(1, 1, 3, 0);
			public Bitmap proc(int X, int Y, int Z)
			{
				float x = (X - 50f) / MNIST.visScale;
				float y = (Y - 50f) / MNIST.visScale;
				float z = (Z - 50f) / MNIST.visScale;
				regen.w[0] = x;
				regen.w[1] = y;
				regen.w[1] = z;

				var _out = forward(regen);

				return _out.vis(0);
			}
		}
		public class Img2XYZNet : Net//net3
		{
			//输入图像 输坐标
			public void init(ConvLayer cv1, ConvLayer cv2, ConvLayer cv3, FullyConnLayer fc144, FullyConnLayer fc2)
			{
				Add(new InputLayer(out_sx: 28, out_sy: 28, out_depth: 1));
				Add(cv1);
				Add(new ReluLayer());

				Add(cv2);
				Add(new ReluLayer());

				Add(cv3);
				Add(new ReluLayer());

				Add(fc144);
				Add(new TanhLayer());

				Add(fc2);
			}
		}

		public CoTrainNet coTrainNet;
		public CoTrainTestNet coTrainTestNet;
		public XYZ2ImgNet xyz2ImgNet;
		public Img2XYZNet img2XYZNet;

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

			coTrainNet = new CoTrainNet();
			coTrainNet.init();

			coTrainTestNet = new CoTrainTestNet();
			coTrainTestNet.init(coTrainNet.fc20A, coTrainNet.fc20B, coTrainNet.fc10);

			img2XYZNet = new Img2XYZNet();
			img2XYZNet.init(coTrainNet.cv1, coTrainNet.cv2, coTrainNet.cv3, coTrainNet.fc144, coTrainNet.fc2);

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
		public string display()
		{
			//"red", "blue", "green", "black", "magenta", "cyan", "purple", "aqua", "olive", "lime", "navy"
			Random rnd = new Random();

			StringBuilder sb = new StringBuilder();

			for (int i = 0; i < 1000; i++)
			{
				int index = (int)(rnd.NextDouble() * 70000);
				var v = MNIST.getImg(index);
				int lbl = MNIST.getLbl(index);
				var fco2 = img2XYZNet.forward(v);
				float x = fco2.w[0];
				float y = fco2.w[1];
				float z = fco2.w[2];


				Color color = cDict[lbl];

				sb.AppendLine(lbl + "," + x + "," + y + "," + z);
			}

			return sb.ToString();
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
