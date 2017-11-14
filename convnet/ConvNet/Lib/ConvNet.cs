using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;

namespace ConvNet
{
	public class Util
	{
		public static bool useGPU = false;
		public static bool useSSE = false;

		[DllImport("cuda2Lib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern int init(int device);

		public static int initGPU(int device)
		{
			return init(device);
		}

		public delegate void DoSave(StreamWriter sw);
		public static void save(string filename, DoSave sv)
		{
			try
			{
				FileStream fs = new FileStream(filename, FileMode.Create, FileAccess.Write);
				StreamWriter sw = new StreamWriter(fs);
				sv(sw);
				sw.Flush();
				fs.Flush();
				fs.Close();
			}
			catch
			{
			}
		}

		public delegate void DoLoad(StreamReader sr);
		public static void load(string filename, DoLoad ld)
		{
			try
			{
				FileStream fs = new FileStream(filename, FileMode.Open, FileAccess.Read);
				StreamReader sr = new StreamReader(fs);
				ld(sr);
				fs.Close();
			}
			catch
			{
			}
		}
		public static void log(string txt)
		{
			try
			{
				FileStream fs = new FileStream("log.txt", FileMode.Append, FileAccess.Write);
				StreamWriter sw = new StreamWriter(fs);
				sw.WriteLine(txt);
				sw.Flush();
				fs.Flush();
				fs.Close();
			}
			catch
			{
			}
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
				Bitmap b = vis(d, 255);
				b.Save(path + d + ".png", ImageFormat.Png);
			}
		}
		public Bitmap vis(int d, float scale)
		{
			Bitmap b = new Bitmap(sx, sy);
			for (int i = 0; i < sx; i++)
			{
				for (int j = 0; j < sy; j++)
				{
					float v = get(i, j, d) * scale;
					Color c;
					if (v > 0)
					{
						if (v > 255) v = 255;
						c = Color.FromArgb((int)(v), 0, 0);
					}
					else
					{
						if (v < -255) v = -255;
						c = Color.FromArgb(0, (int)(-v), 0);
					}
					b.SetPixel(i, j, c);
				}
			}
			return b;
		}
		public Bitmap visRGB(float scale)
		{
			if (depth != 3) return null;
			Bitmap bmp = new Bitmap(sx, sy);
			for (int i = 0; i < sx; i++)
			{
				for (int j = 0; j < sy; j++)
				{
					float r = get(j, i, 0) * scale;
					float g = get(j, i, 1) * scale;
					float b = get(j, i, 2) * scale;
					if (r < 0) r = 0;
					if (r > 255) r = 255;

					if (g < 0) g = 0;
					if (g > 255) g = 255;

					if (b < 0) b = 0;
					if (b > 255) b = 255;
					Color c;
					c = Color.FromArgb((int)(r), (int)(g), (int)(b));
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
		public static void init(MyFloat f, int length, float? c)
		{
			// we were given dimensions of the vol
			int n = length;
			if (c == null)
			{
				// weight normalization is done to equalize the output
				// variance of every neuron, otherwise neurons with a lot
				// of incoming connections have outputs of larger variance
				float scale = (float)Math.Sqrt(1.0f / (length));
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
		public Vol forward(Instance ins, Vol V)
		{
			//if(typeof(is_training)==='undefined') is_training = false;
			Vol act = V;
			for (int i = 0; i < this.layers.Count; i++)
			{
				act = this.layers[i].forward(ins.list[i], act);
			}
			return act;
		}

		// backprop: compute gradients wrt all parameters
		public void backward(Instance ins)
		{
			int N = this.layers.Count;
			for (int i = N - 1; i >= 0; i--)
			{
				this.layers[i].backward(ins.list[i]);
			}
		}

		public class Instance
		{
			public Vol inact;
			public Layer.Instance[] list;
		}

		public Instance getInstance()
		{
			Instance instance = new Instance();
			instance.list = new Layer.Instance[layers.Count];

			for (int i = 0; i < this.layers.Count; i++)
			{
				instance.list[i] = this.layers[i].getInstance();
			}
			return instance;
		}

		public float train(Instance instance, Vol x, DataSet y)//Report
		{
			//Stopwatch sw = new Stopwatch();
			//sw.Start();
			forward(instance, x); // also set the flag that lets the net know we're just training
			//long t1 = sw.ElapsedTicks;
			//var lastLayer = ((LastLayer)this.net.layers[this.net.layers.Count - 1]);
			var lastLayer = (LastLayer)out_layer;
			var lastIns = ((LastLayer.LastInstance)instance.list[instance.list.Length - 1]);
			lastIns.y = y;
			//lastLayer.setData(y);
			backward(instance);
			float cost_loss = lastIns.loss;
			//long t2 = sw.ElapsedTicks;
			//float l2_decay_loss = 0.0f;
			//float l1_decay_loss = 0.0f;


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


		public void endofBatch(Instance[] instance, int batchSize)
		{
			for (int i = 0; i < this.layers.Count; i++)
			{
				if (layers[i] is TrainableLayer)
				{
					TrainableLayer.TrainableInstance instance0 = (TrainableLayer.TrainableInstance)instance[0].list[i];
					MyFloat bias_dw_0 = instance0.bias_dw;
					MyFloat filters_dw_0 = instance0.filters_dw;
					for (int j = 1; j < instance.Length; j++)
					{

						MyFloat bias_dw_j = ((TrainableLayer.TrainableInstance)instance[j].list[i]).bias_dw;
						for (int k = 0; k < bias_dw_0.size; k++)
						{
							bias_dw_0[k] += bias_dw_j[k];
							bias_dw_j[k] = 0;
						}

						MyFloat filters_dw_j = ((TrainableLayer.TrainableInstance)instance[j].list[i]).filters_dw;
						for (int k = 0; k < filters_dw_0.size; k++)
						{
							filters_dw_0[k] += filters_dw_j[k];
							filters_dw_j[k] = 0;
						}
					}

					((TrainableLayer)layers[i]).train(instance0, trainer, 1.0f / batchSize);
				}
			}

		}
	}




}
