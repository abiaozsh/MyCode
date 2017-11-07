using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Drawing;
using System.Drawing.Imaging;

namespace ConvNet
{
	public class Util
	{
		public delegate void DoSave(StreamWriter sw);
		public static void save(string filename, DoSave sv)
		{
			FileStream fs = new FileStream(filename, FileMode.Create, FileAccess.Write);
			StreamWriter sw = new StreamWriter(fs);
			sv(sw);
			sw.Flush();
			fs.Flush();
			fs.Close();
		}

		public delegate void DoLoad(StreamReader sr);
		public static void load(string filename, DoLoad ld)
		{
			FileStream fs = new FileStream(filename, FileMode.Open, FileAccess.Read);
			StreamReader sr = new StreamReader(fs);
			ld(sr);
			fs.Close();
		}
		public static void log(string txt)
		{
			FileStream fs = new FileStream("log.txt", FileMode.Append, FileAccess.Write);
			StreamWriter sw = new StreamWriter(fs);
			sw.WriteLine(txt);
			sw.Flush();
			fs.Flush();
			fs.Close();
		}

		static Random rnd = new Random();
		public static float randf(float a, float b)
		{
			return (float)rnd.NextDouble() * (b - a) + a;

		}
		public static int randi(float a, float b)
		{
			return (int)Math.Floor(rnd.NextDouble() * (b - a) + a);
		}

	}
	public class Range
	{
		public float max;
		public float min;
	}
	public interface Persistence
	{
		void save(TextWriter s);
		void load(TextReader s);
	}

	public class Vol : Persistence
	{
		public int sx;
		public int sy;
		public int depth;
		public MyFloat w;
		public MyFloat dw;

		public void vis(string path)
		{
			for (int d = 0; d < depth; d++)
			{
				Bitmap b = vis(d);
				b.Save(path + d + ".png", ImageFormat.Png);
			}
		}
		public Bitmap vis(int d)
		{
			Bitmap b = new Bitmap(sx, sy);
			for (int i = 0; i < sx; i++)
			{
				for (int j = 0; j < sy; j++)
				{
					float v = get(i, j, d);
					Color c;
					if (v > 0)
					{
						if (v > 1) v = 1;
						c = Color.FromArgb((int)(v * 255), 0, 0);
					}
					else
					{
						if (v < -1) v = -1;
						c = Color.FromArgb(0, (int)(-v * 255), 0);
					}
					b.SetPixel(i, j, c);
				}
			}
			return b;
		}
		public Bitmap visRGB()
		{
			if (depth != 3) return null;
			Bitmap bmp = new Bitmap(sx, sy);
			for (int i = 0; i < sx; i++)
			{
				for (int j = 0; j < sy; j++)
				{
					float r = get(j, i, 0);
					float g = get(j, i, 1);
					float b = get(j, i, 2);
					if (r < 0) r = 0;
					if (r > 1) r = 1;

					if (g < 0) g = 0;
					if (g > 1) g = 1;

					if (b < 0) b = 0;
					if (b > 1) b = 1;
					Color c;
					c = Color.FromArgb((int)(r * 255), (int)(g * 255), (int)(b * 255));
					bmp.SetPixel(i, j, c);
				}
			}
			return bmp;
		}

		public void save(TextWriter s)
		{
			w.save(s);
		}
		public void load(TextReader s)
		{
			w.load(s);
		}

		static bool return_v = false;
		static float v_val = 0.0f;
		static Random _rnd = new Random();
		static object objLock = new object();
		public static float gaussRandom()
		{
			lock (objLock)
			{
				if (return_v)
				{
					return_v = false;
					return v_val;
				}
				float u = 2.0f * (float)_rnd.NextDouble() - 1.0f;
				float v = 2.0f * (float)_rnd.NextDouble() - 1.0f;
				float r = u * u + v * v;
				if (r == 0 || r > 1)
				{
					return gaussRandom();
				}
				float c = (float)Math.Sqrt(-2.0f * (float)Math.Log(r) / r);
				v_val = v * c; // cache this
				return_v = true;
				return u * c;
			}
		}

		public Vol(int sx, int sy, int depth, float? c)
		{
			// we were given dimensions of the vol
			this.sx = sx;
			this.sy = sy;
			this.depth = depth;
			int n = sx * sy * depth;
			this.w = new MyFloat(n);//Util.zeros(n);
			this.dw = new MyFloat(n);//Util.zeros(n);
			if (c == null)
			{
				// weight normalization is done to equalize the output
				// variance of every neuron, otherwise neurons with a lot
				// of incoming connections have outputs of larger variance
				float scale = (float)Math.Sqrt(1.0f / (sx * sy * depth));
				for (int i = 0; i < n; i++)
				{
					this.w[i] = gaussRandom() * scale;
				}
			}
			else
			{
				for (int i = 0; i < n; i++)
				{
					this.w[i] = c.Value;
				}
			}
		}
		public static void init(MyFloat f,int sx, int sy, int depth, float? c)
		{
			// we were given dimensions of the vol
			int n = sx * sy * depth;
			if (c == null)
			{
				// weight normalization is done to equalize the output
				// variance of every neuron, otherwise neurons with a lot
				// of incoming connections have outputs of larger variance
				float scale = (float)Math.Sqrt(1.0f / (sx * sy * depth));
				for (int i = 0; i < n; i++)
				{
					f[i] = gaussRandom() * scale;
				}
			}
			else
			{
				for (int i = 0; i < n; i++)
				{
					f[i] = c.Value;
				}
			}
		}

		public float get(int x, int y, int d)
		{
			int ix = ((this.sx * y) + x) * this.depth + d;
			return this.w[ix];
		}
		public void set(int x, int y, int d, float val)
		{
			int ix = ((this.sx * y) + x) * this.depth + d;
			this.w[ix] = val;
		}
		public void add_grad(int x, int y, int d, float val)
		{
			int ix = ((this.sx * y) + x) * this.depth + d;
			this.dw[ix] += val;
		}
		public float get_grad(int x, int y, int d)
		{
			int ix = ((this.sx * y) + x) * this.depth + d;
			return this.dw[ix];
		}
		public void set_grad(int x, int y, int d, float val)
		{
			int ix = ((this.sx * y) + x) * this.depth + d;
			this.dw[ix] = val;
		}

	}
	public class DataSet
	{
		//Classification (SVM/Softmax) and Regression (L2) cost functions
		public Vol data;

		//Classification (SVM/Softmax)
		public int predict;
	}
	public abstract class Net
	{
		public List<Layer> layers;

		public Trainer trainer;

		public Net()
		{
			layers = new List<Layer>();
		}

		public Layer in_layer;
		public Layer out_layer;

		public void Add(Layer layer)
		{
			if (in_layer == null)
			{
				in_layer = layer;
			}
			layers.Add(layer);
			layer.in_layer = out_layer;
			if (!layer.inited())
			{
				layer.init();
			}
			if (out_layer != null)
			{
				out_layer.out_layer = layer;
			}
			out_layer = layer;
		}

		// forward prop the network. A trainer will pass in is_training = true
		public Vol forward(Vol V)
		{
			//if(typeof(is_training)==='undefined') is_training = false;
			Vol act = V;
			for (int i = 0; i < this.layers.Count; i++)
			{
				act = this.layers[i].forward(act);
			}
			return act;
		}

		// backprop: compute gradients wrt all parameters
		public void backward()
		{
			int N = this.layers.Count;
			for (int i = N - 1; i >= 0; i--)
			{
				this.layers[i].backward();
			}
		}


		public float train(Vol x, DataSet y)//Report
		{
			//Stopwatch sw = new Stopwatch();
			//sw.Start();
			forward(x); // also set the flag that lets the net know we're just training
			//long t1 = sw.ElapsedTicks;
			//var lastLayer = ((LastLayer)this.net.layers[this.net.layers.Count - 1]);
			var lastLayer = (LastLayer)out_layer;
			lastLayer.setData(y);
			backward();
			float cost_loss = lastLayer.getLoss();
			//long t2 = sw.ElapsedTicks;
			//float l2_decay_loss = 0.0f;
			//float l1_decay_loss = 0.0f;

			trainer.count++;
			if (trainer.count == trainer.batchSize )
			{
				for (int i = 0; i < this.layers.Count; i++)
				{
					if (layers[i] is TrainableLayer)
					{
						((TrainableLayer)layers[i]).train(trainer);
					}
				}
				trainer.count = 0;
			}

			// appending softmax_loss for backwards compatibility, but from now on we will always use cost_loss
			// in future, TODO: have to completely redo the way loss is done around the network as currently 
			// loss is a bit of a hack. Ideally, user should specify arbitrary number of loss functions on any layer
			// and it should all be computed correctly and automatically. 

			//return new Report()
			//{
			//	l2_decay_loss = l2_decay_loss,
			//	l1_decay_loss = l1_decay_loss,
			//	cost_loss = cost_loss,
			//	softmax_loss = cost_loss,
			//	loss = cost_loss + l1_decay_loss + l2_decay_loss
			//};
			return cost_loss;
		}

	}




}
