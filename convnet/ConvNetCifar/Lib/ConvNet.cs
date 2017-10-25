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
			this.w = MyFloat.getArray(n);//Util.zeros(n);
			this.dw = MyFloat.getArray(n);//Util.zeros(n);
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
	public class ParamsAndGrads
	{
		public int params_size;
		public MyFloat params_;
		public MyFloat grads_;
		public int params_idx=0;
		public int grads_idx=0;
		public MyFloat gsum; //[]?
		public MyFloat xsum; //[]?
		public float l1_decay_mul = 1.0f;
		public float l2_decay_mul = 1.0f;
	}
	public abstract class Net
	{
		public List<Layer> layers;

		//public void save(TextWriter s)
		//{
		//	for (int i = 0; i < layers.Count; i++)
		//	{
		//		layers[i].save(s);
		//	}
		//}
		//public void load(TextReader s)
		//{
		//	for (int i = 0; i < layers.Count; i++)
		//	{
		//		layers[i].load(s);
		//	}
		//}

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


		public List<ParamsAndGrads> getParamsAndGrads()
		{
			// accumulate parameters and gradients for the entire network
			List<ParamsAndGrads> response = new List<ParamsAndGrads>();
			for (int i = 0; i < this.layers.Count; i++)
			{
				this.layers[i].getParamsAndGrads(response);
			}
			return response;
		}

	}




}
