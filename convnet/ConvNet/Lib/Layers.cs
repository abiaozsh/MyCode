using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using System.Drawing;
using Lib;
using System.Diagnostics;

namespace ConvNet
{
	//in_depth % 8 == 0)  有问题

	//w 和 dw 分离
	//instance 化
	//并行训练
	//合并后调参

	//输入参数归一化



	public abstract class Layer
	{
		public Layer in_layer;
		public Layer out_layer;

		protected bool _inited = false;
		public int out_sx;
		public int out_sy;
		public int out_depth;
		public Vol in_act;
		public Vol out_act;
		public float l1_decay_mul;
		public float l2_decay_mul;
		public abstract bool inited();
		public abstract void init();
		public abstract Vol forward(Vol V);
		public abstract void backward();
		//public float backwardData(DataSet y){return 0;}
		//public abstract void getParamsAndGrads(List<ParamsAndGrads> pg);
	}

	public abstract class LastLayer : Layer
	{
		public abstract void setData(DataSet y);

		public abstract float getLoss();
	}
	public abstract class ActivationLayer : Layer
	{
	}
	public abstract class TrainableLayer : Layer, Persistence
	{
		public abstract void save(TextWriter s);
		public abstract void load(TextReader s);

		public abstract void train(Trainer trainer);
	}


	public class FullyConnLayer : TrainableLayer
	{
		public string name;
		public override void save(TextWriter s)
		{
			filters_w.save(s);
			bias_w.save(s);
		}
		public override void load(TextReader s)
		{
			filters_w.load(s);
			bias_w.load(s);
		}
		//Vol[] filters;
		MyFloat filters_w;

		MyFloat filters_dw;

		MyFloat[] filters_gsum; //[]?
		MyFloat[] filters_xsum; //[]?

		MyFloat bias_w;

		MyFloat bias_dw;

		MyFloat bias_gsum; //[]?
		MyFloat bias_xsum; //[]?

		public int num_inputs;

		float bias_pref;

		public ActivationLayer act;

		public FullyConnLayer(int num_neurons = 0, float bias_pref = 0.0f, ActivationLayer act = null)
		{

			// required
			// ok fine we will allow 'filters' as the word as well
			this.out_depth = num_neurons;

			// optional 
			//this.l1_decay_mul = opt.l1_decay_mul;
			//this.l2_decay_mul = opt.l2_decay_mul;

			this.bias_pref = bias_pref;

			this.act = act;
		}

		public override bool inited()
		{
			return _inited;
		}
		public override void init()
		{
			this.num_inputs = in_layer.out_sx * in_layer.out_sy * in_layer.out_depth;
			this.out_sx = 1;
			this.out_sy = 1;

			// initializations
			filters_gsum = new MyFloat[out_depth];
			filters_xsum = new MyFloat[out_depth];
			for (int i = 0; i < this.out_depth; i++)
			{
				filters_gsum[i] = new MyFloat(num_inputs);
				filters_xsum[i] = new MyFloat(num_inputs);
				for (int j = 0; j < num_inputs; j++)
				{
					filters_gsum[i][j] = 0.0f;
					filters_xsum[i][j] = 0.0f;
				}
			}
			//filters = new Vol[out_depth];
			//filters[i] = new Vol(1, 1, this.num_inputs, null);
			filters_w = new MyFloat(num_inputs * out_depth);
			Vol.init(filters_w, 1, 1, num_inputs * out_depth, null);

			filters_dw = new MyFloat(num_inputs * out_depth);


			bias_w = new MyFloat(out_depth);
			Vol.init(bias_w, 1, 1, out_depth, null);
			bias_dw = new MyFloat(out_depth);

			for (int j = 0; j < out_depth; j++)
			{
				bias_w[j] = bias_pref;
				bias_dw[j] = 0.0f;
			}

			this.bias_gsum = new MyFloat(out_depth); // last iteration gradients (used for momentum calculations)
			this.bias_xsum = new MyFloat(out_depth); // used in adadelta

			for (int j = 0; j < out_depth; j++)
			{
				bias_gsum[j] = 0.0f;
				bias_xsum[j] = 0.0f;
			}

			this.out_act = new Vol(1, 1, this.out_depth, 0.0f);
			_inited = true;
			if (act != null)
			{
				act.in_layer = this;
				act.init();
			}
		}

		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern void FCFWD(
			int out_depth,
			int num_inputs,
			IntPtr p_in_act_w,
			IntPtr p_filters_w,
			IntPtr p_bias_w,
			IntPtr p_out_act_w
		);
		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern IntPtr GK_FCFWD(IntPtr oclobjects);
		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern int RK_FCFWD(
			IntPtr oclobjects,
			IntPtr kernel,

			int out_depth,
			int num_inputs,
			IntPtr p_in_act_w,
			IntPtr p_filters_w,
			IntPtr p_bias_w,
			IntPtr p_out_act_w
		);
		static IntPtr kernel_FCFWD;

		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern IntPtr GK_FCBWD(IntPtr oclobjects);
		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern int RK_FCBWD(
			IntPtr oclobjects,
			IntPtr kernel,

			int out_depth,
			int num_inputs,
			IntPtr p_in_act_w,
			IntPtr p_filters_w,
			IntPtr p_in_act_dw,
			IntPtr p_out_act_dw,
			IntPtr p_filters_dw,
			IntPtr p_bias_dw
		);
		static IntPtr kernel_FCBWD;
		static FullyConnLayer()
		{
			if (OpenCL.oclobjects != IntPtr.Zero)
			{
				kernel_FCFWD = GK_FCFWD(OpenCL.oclobjects);
				kernel_FCBWD = GK_FCBWD(OpenCL.oclobjects);
			}
		}

		private bool check(MyFloat f)
		{
			for (int i = 0; i < f.size; i++)
			{
				if (float.IsNaN(f[0]))
				{
					throw new Exception();
				}
			}
			return false;
		}
		public override Vol forward(Vol V)
		{
			this.in_act = V;

			//16*16: 1700,2170,2399 /  5
			if (false)//OpenCL.oclobjects != IntPtr.Zero)
			{
				//check(in_act.w);
				//check(filters.w);
				//check(bias.w);
				//check(out_act.w);
				long t1, t2, t3;
				Stopwatch sw = new Stopwatch();
				sw.Start();
				in_act.w.copyToCLMEM();
				filters_w.copyToCLMEM();
				bias_w.copyToCLMEM();
				out_act.w.copyToCLMEM();
				sw.Stop();
				t1 = sw.ElapsedTicks;
				sw.Start();
				int err = RK_FCFWD(OpenCL.oclobjects, kernel_FCFWD, out_depth, num_inputs, in_act.w.p_cl_mem, filters_w.p_cl_mem, bias_w.p_cl_mem, out_act.w.p_cl_mem);
				sw.Stop();
				t2 = sw.ElapsedTicks;
				sw.Start();
				in_act.w.copyFromCLMEM();
				filters_w.copyFromCLMEM();
				bias_w.copyFromCLMEM();
				out_act.w.copyFromCLMEM();
				sw.Stop();
				t3 = sw.ElapsedTicks;
				//sw.Start();

				//Console.WriteLine(t1 + "," + t2 + "," + t3);
				//check(in_act.w);
				//check(filters.w);
				//check(bias.w);
				//check(out_act.w);
				//Console.WriteLine(err);
			}
			else
			{
				//long aa = Stopwatch.Frequency;
				//long t1 = 0, t2 = 0, t3 = 0;
				//Stopwatch sw = new Stopwatch();
				//sw.Start();
				FCFWD(out_depth, num_inputs, in_act.w.ori_p, filters_w.ori_p, bias_w.ori_p, out_act.w.ori_p);
				//sw.Stop();
				//t3 = sw.ElapsedTicks;
				//if (t3 < -10000)
				//{
				//	throw new Exception("slow");
				//}
			}

			if (act != null)
			{
				return act.forward(this.out_act);
			}
			else
			{
				return this.out_act;
			}
		}

		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern void FCBWD(
			int out_depth,
			int num_inputs,
			IntPtr p_in_act_w,
			IntPtr p_filters_w,
			IntPtr p_in_act_dw,
			IntPtr p_out_act_dw,
			IntPtr p_filters_dw,
			IntPtr p_bias_dw
		);

		public override void backward()
		{
			if (noUpdate) return;

			if (act != null)
			{
				act.backward();
			}

			if (false)//OpenCL.oclobjects != IntPtr.Zero)
			{
				for (int i = 0; i < num_inputs; i++)
				{
					//in_act.dw[d] = 0;
					in_act.dw[i] = 0;
				}
				//check(in_act.w);
				//check(filters.w);
				//check(bias.w);
				//check(out_act.w);
				long t1, t2, t3;
				Stopwatch sw = new Stopwatch();
				sw.Start();

				in_act.w.copyToCLMEM();
				filters_w.copyToCLMEM();
				in_act.dw.copyToCLMEM();
				out_act.dw.copyToCLMEM();
				filters_dw.copyToCLMEM();
				bias_dw.copyToCLMEM();

				sw.Stop();
				t1 = sw.ElapsedTicks;
				sw.Start();
				int err = RK_FCBWD(OpenCL.oclobjects, kernel_FCBWD, out_depth, num_inputs, in_act.w.p_cl_mem, filters_w.p_cl_mem, in_act.dw.p_cl_mem, out_act.dw.p_cl_mem, filters_dw.p_cl_mem, bias_dw.p_cl_mem);
				sw.Stop();
				t2 = sw.ElapsedTicks;
				sw.Start();

				in_act.w.copyFromCLMEM();
				filters_w.copyToCLMEM();
				in_act.dw.copyToCLMEM();
				out_act.dw.copyToCLMEM();
				filters_dw.copyToCLMEM();
				bias_dw.copyToCLMEM();

				sw.Stop();
				t3 = sw.ElapsedTicks;
				//sw.Start();

				//Console.WriteLine(t1 + "," + t2 + "," + t3);
				//check(in_act.w);
				//check(filters.w);
				//check(bias.w);
				//check(out_act.w);
				//Console.WriteLine(err);
			}
			else
			{
				//long aa = Stopwatch.Frequency;
				//long t1 = 0, t2 = 0, t3 = 0;
				//Stopwatch sw = new Stopwatch();
				//sw.Start();
				FCBWD(out_depth, num_inputs, in_act.w.ori_p, filters_w.ori_p, in_act.dw.ori_p, out_act.dw.ori_p, filters_dw.ori_p, bias_dw.ori_p);
				//sw.Stop();
				//t3 = sw.ElapsedTicks;
				//if (t3 < -10000)
				//{
				//	throw new Exception("slow");
				//}
			}
		}
		public bool noUpdate = false;
		public override void train(Trainer trainer)
		{
			if (noUpdate) return;
			for (int i = 0; i < this.out_depth; i++)
			{
				//int iw = i * this.num_inputs;
				//response.Add(new ParamsAndGrads() { Params = this.filters[i].w, grads = this.filters[i].dw, l1_decay_mul = this.l1_decay_mul, l2_decay_mul = this.l2_decay_mul });
				trainer.train(
					num_inputs,//params_size = 
					this.filters_w,//params_ = 
					this.filters_dw,//grads_ = 
					i * num_inputs,//params_idx = 
					i * num_inputs,//grads_idx = 
					filters_gsum[i],//gsum = 
					filters_xsum[i],//xsum = 
					this.l1_decay_mul,//l1_decay_mul = 
					this.l2_decay_mul//l2_decay_mul = 
				);
			}
			//response.Add(new ParamsAndGrads() { Params = this.biases.w, grads = this.biases.dw, l1_decay_mul = 0.0f, l2_decay_mul = 0.0f });
			trainer.train(
				out_depth,//params_size = 
				this.bias_w,//params_ = 
				this.bias_dw,//grads_ = 
				0,//params_idx = 
				0,//grads_idx = 
				bias_gsum,//gsum = 
				bias_xsum,//xsum = 
				0.0f,//l1_decay_mul = 
				0.0f//l2_decay_mul = 
			);
		}
	}
	public class ConvLayer : TrainableLayer
	{
		public string name;
		public override void save(TextWriter s)
		{
			bias_w.save(s);
			filters_w.save(s);
		}
		public override void load(TextReader s)
		{
			bias_w.load(s);
			filters_w.load(s);
		}
		// This file contains all layers that do dot products with input,
		// but usually in a different connectivity pattern and weight sharing
		// schemes: 
		// - FullyConn is fully connected dot products 
		// - ConvLayer does convolutions (so weight sharing spatially)
		// putting them together in one file because they are very similar
		int sx;
		int sy;
		int in_depth;
		public int filterSize;
		int in_sx;
		int in_sy;
		int stride;
		int pad;
		//最外层是filter，然后是行单元，然后是行元素 然后是输入层深度，行相邻元素在一起
		public MyFloat filters_w;
		public MyFloat filters_dw;


		MyFloat[] filters_gsum; //[]?
		MyFloat[] filters_xsum; //[]?


		MyFloat bias_w;
		MyFloat bias_dw;


		MyFloat bias_gsum; //[]?
		MyFloat bias_xsum; //[]?
		float bias_pref;
		int adj = 0;
		ActivationLayer act;
		public ConvLayer(int sx = 0, int sy = 0, int filters = 0, int stride = 1, int adj = 0, int pad = 0, float bias_pref = 0.0f, ActivationLayer act = null)//if pad = sx(sy) / 2  the size is same   bigger pad bigger out
		{
			// required
			this.out_depth = filters;
			this.sx = sx; // filter size. Should be odd if possible, it's cleaner.
			this.sy = sy;

			this.bias_pref = bias_pref;

			// optional
			this.stride = stride; // stride at which we apply filters to input volume
			this.pad = pad; // amount of 0 padding to add around borders of input volume
			this.adj = adj;
			//this.l1_decay_mul = opt.l1_decay_mul;
			//this.l2_decay_mul = opt.l2_decay_mul;
			this.act = act;
		}
		public override bool inited()
		{
			return _inited;
		}
		public override void init()
		{
			this.in_depth = in_layer.out_depth;
			this.in_sx = in_layer.out_sx;
			this.in_sy = in_layer.out_sy;

			filterSize = sx * sy * in_depth;
			{
				if ((in_sx + pad * 2 - sx + adj) / stride * stride != (in_sx + pad * 2 - sx + adj))
				{
					throw new Exception("err");
				}
				if ((in_sy + pad * 2 - sy + adj) / stride * stride != (in_sy + pad * 2 - sy + adj))
				{
					throw new Exception("err");
				}
				this.out_sx = (in_sx + pad * 2 - sx + adj) / stride + 1;
				this.out_sy = (in_sy + pad * 2 - sy + adj) / stride + 1;
			}

			filters_gsum = new MyFloat[out_depth];
			filters_xsum = new MyFloat[out_depth];
			for (int i = 0; i < this.out_depth; i++)
			{
				filters_gsum[i] = new MyFloat(sx * sy * in_depth);
				filters_xsum[i] = new MyFloat(sx * sy * in_depth);
				for (int j = 0; j < sx * sy * in_depth; j++)
				{
					filters_gsum[i][j] = 0.0f;
					filters_xsum[i][j] = 0.0f;
				}
			}
			//this.filters = new Vol[out_depth];
			//this.filters[i] = new Vol(this.sx, this.sy, this.in_depth, null);
			//this.filters = new Vol(this.sx, this.sy, this.in_depth * out_depth, null);
			this.filters_w = new MyFloat(this.sx * this.sy * this.in_depth * out_depth);
			Vol.init(this.filters_w, this.sx, this.sy, this.in_depth * out_depth, null);
			this.filters_dw = new MyFloat(this.sx * this.sy * this.in_depth * out_depth);
			//this.bias = new Vol(1, 1, this.out_depth, bias_pref);
			this.bias_w = new MyFloat(out_depth);
			Vol.init(this.bias_w, 1, 1, out_depth, null);
			this.bias_dw = new MyFloat(out_depth);


			this.bias_gsum = new MyFloat(out_depth); // last iteration gradients (used for momentum calculations)
			this.bias_xsum = new MyFloat(out_depth); // used in adadelta
			for (int j = 0; j < out_depth; j++)
			{
				bias_gsum[j] = 0.0f;
				bias_xsum[j] = 0.0f;
			}
			this.out_act = new Vol(this.out_sx, this.out_sy, this.out_depth, 0.0f);
			_inited = true;

			if (act != null)
			{
				act.in_layer = this;
				act.init();
			}
		}

		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern void CVFWD(
			int stride,
			int pad,
			int sx,
			int sy,
			int in_depth,
			int out_sx,
			int out_sy,
			int out_depth,
			int filterSize,
			int in_act_sx,
			int in_act_sy,
			IntPtr p_filters_w,
			IntPtr p_in_act_w,
			IntPtr p_bias_w,
			IntPtr p_out_act_w
		);

		public override Vol forward(Vol V)
		{
			// optimized code by @mdda that achieves 2x speedup over previous version

			this.in_act = V;
			int filterSize = sx * sy * in_depth;

			long aa = Stopwatch.Frequency;
			long t1 = 0, t2 = 0, t3 = 0;
			Stopwatch sw = new Stopwatch();
			sw.Start();
			CVFWD(
				stride,
				pad,
				sx,
				sy,
				in_depth,
				out_sx,
				out_sy,
				out_depth,
				filterSize,
				in_act.sx,
				in_act.sy,
				filters_w.ori_p,
				in_act.w.ori_p,
				bias_w.ori_p,
				out_act.w.ori_p
			);
			sw.Stop();
			t3 = sw.ElapsedTicks;
			int count = out_sy * out_sx * out_depth * sy * sx * in_depth;
			double t4 = t3 / (double)(3117998.0 / 1000000);
			if (t4 < -1 && count < -1)
			{
				throw new Exception("slow");
			}

			if (act != null)
			{
				return act.forward(this.out_act);
			}
			else
			{
				return this.out_act;
			}
		}


		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern void CVBWD(
			int stride,
			int pad,
			int sx,
			int sy,
			int in_size,
			int in_depth,
			int out_sx,
			int out_sy,
			int out_depth,
			int filterSize,
			int in_act_sx,
			int in_act_sy,
			IntPtr p_filters_w,
			IntPtr p_in_act_w,
			IntPtr p_in_act_dw,
			IntPtr p_out_act_dw,
			IntPtr p_bias_dw,
			IntPtr p_filters_dw
		);

		public override void backward()
		{
			if (noUpdate) return;

			//????????TODO 未初始化可能影响后续????????????????V.dw = Util.zeros(V.w.Length); // zero out gradient wrt bottom data, we're about to fill it
			int in_size = in_sx * in_sy * in_depth;
			int filterSize = sx * sy * in_depth;

			if (act != null)
			{
				act.backward();
			}

			CVBWD(
				stride,
				pad,
				sx,
				sy,
				in_size,
				in_depth,
				out_sx,
				out_sy,
				out_depth,
				filterSize,
				in_act.sx,
				in_act.sy,
				filters_w.ori_p,
				in_act.w.ori_p,
				in_act.dw.ori_p,
				out_act.dw.ori_p,
				bias_dw.ori_p,
				filters_dw.ori_p
			);


		}
		public bool noUpdate = false;
		public override void train(Trainer trainer)
		{
			if (noUpdate) return;
			for (int i = 0; i < this.out_depth; i++)
			{
				int filterSize = sx * sy * in_depth;
				trainer.train(
					filterSize,//params_size = 
					this.filters_w,//params_ = 
					this.filters_dw,//grads_ = 
					i * filterSize,//params_idx = 
					i * filterSize,//grads_idx = 
					filters_gsum[i],//gsum = 
					filters_xsum[i],//xsum = 
					this.l2_decay_mul,//l2_decay_mul = 
					this.l1_decay_mul//l1_decay_mul = 
				);
			}
			trainer.train(
				this.out_depth,//params_size = 
				this.bias_w,//params_ = 
				this.bias_dw,//grads_ = 
				0,//params_idx = 
				0,//grads_idx = 
				bias_gsum,//gsum = 
				bias_xsum,//xsum = 
				0.0f,//l1_decay_mul = 
				l2_decay_mul = 0.0f
			);
		}

		public Bitmap vis(int idx, int d, float scale)
		{
			Bitmap b = new Bitmap(sx, sy);
			for (int i = 0; i < sx; i++)
			{
				for (int j = 0; j < sy; j++)
				{
					float v = filters_w[idx * filterSize + ((this.sx * j) + i) + d];
					Color c;
					if (v > 0)
					{
						if (v > 1) v = 1;
						c = Color.FromArgb((int)(v * scale), 0, 0);
					}
					else
					{
						if (v < -1) v = -1;
						c = Color.FromArgb(0, (int)(-v * scale), 0);
					}
					b.SetPixel(i, j, c);
				}
			}
			return b;
		}
		public Bitmap visRGB(int idx, float scale)
		{
			//最外层是filter，然后是行单元，然后是行元素 然后是输入层深度，行相邻元素在一起
			Bitmap bmp = new Bitmap(sx, sy);
			for (int i = 0; i < sx; i++)
			{
				for (int j = 0; j < sy; j++)
				{
					int r = (int)(filters_w[idx * filterSize + (((this.sx * j) + i)) * 3 + 0] * scale) + 127;
					int g = (int)(filters_w[idx * filterSize + (((this.sx * j) + i)) * 3 + 1] * scale) + 127;
					int b = (int)(filters_w[idx * filterSize + (((this.sx * j) + i)) * 3 + 2] * scale) + 127;
					Color c;
					if (r < 0) r = 0;
					if (g < 0) g = 0;
					if (b < 0) b = 0;
					if (r > 255) r = 255;
					if (g > 255) g = 255;
					if (b > 255) b = 255;
					c = Color.FromArgb(r, g, b);
					bmp.SetPixel(i, j, c);
				}
			}
			return bmp;
		}

	}

	public class ReluLayer : ActivationLayer
	{
		// Implements ReLU nonlinearity elementwise
		// x -> max(0, x)
		// the output is in [0, inf)
		public ReluLayer()
		{
		}
		public override bool inited()
		{
			return _inited;
		}
		public override void init()
		{
			// computed
			this.out_sx = in_layer.out_sx;
			this.out_sy = in_layer.out_sy;
			this.out_depth = in_layer.out_depth;
			//this.layer_type = "relu";

			this.out_act = new Vol(out_sx, out_sy, out_depth, null);
			_inited = true;
		}

		public override Vol forward(Vol V)
		{
			this.in_act = V;

			//Vol V2 = V.clone();

			int N = V.w.size;
			//float[] V2w = V2.w;
			for (int i = 0; i < N; i++)
			{
				if (V.w[i] < 0)
				{
					this.out_act.w[i] = 0; // threshold at 0
				}
				else
				{
					this.out_act.w[i] = V.w[i];
				}
			}
			//this.out_act = V2;
			return this.out_act;
		}
		public override void backward()
		{
			//Vol V = this.in_act; // we need to set dw of this
			//Vol V2 = this.out_act;
			int N = in_act.w.size;
			//V.dw = Util.zeros(N); // zero out gradient wrt data
			for (int i = 0; i < N; i++)
			{
				//in_act.dw[i] = 0;
				if (out_act.w[i] <= 0)
				{
					in_act.dw[i] = 0; // threshold
				}
				else
				{
					in_act.dw[i] = out_act.dw[i];
				}
			}
		}
	}
	public class TanhLayer : ActivationLayer
	{
		public TanhLayer()
		{
		}

		public override bool inited()
		{
			return _inited;
		}
		public override void init()
		{
			this.out_sx = in_layer.out_sx;
			this.out_sy = in_layer.out_sy;
			this.out_depth = in_layer.out_depth;
			//this.layer_type = "relu";

			this.out_act = new Vol(out_sx, out_sy, out_depth, null);
			//float y = (float)Math.Exp(2 * x);
			//return (y - 1) / (y + 1);
			_inited = true;
		}
		public override Vol forward(Vol V)
		{
			this.in_act = V;
			int N = V.w.size;
			for (int i = 0; i < N; i++)
			{
				out_act.w[i] = (float)Math.Tanh(V.w[i]);
			}
			return this.out_act;
		}
		public override void backward()
		{
			Vol V = this.in_act; // we need to set dw of this
			int N = V.w.size;
			//V.dw = Util.zeros(N); // zero out gradient wrt data
			for (int i = 0; i < N; i++)
			{
				float v2wi = out_act.w[i];
				V.dw[i] = (1.0f - v2wi * v2wi) * out_act.dw[i];
			}
		}
	}

	public class PoolLayer : Layer
	{
		//public int sx;
		//public int sy;
		public int in_depth;
		public int in_sx;
		public int in_sy;
		public int stride;
		//public int pad;
		public int[] switchx;
		public int[] switchy;

		public PoolLayer(int stride = 2)
		{
			// optional
			//this.sy = opt.sy != 0 ? opt.sy : this.sx;
			this.stride = stride;//opt.stride != 0 ? opt.stride : 2;
			//this.pad = opt.pad != 0 ? opt.pad : 0; // amount of 0 padding to add around borders of input volume

		}
		public override bool inited()
		{
			return _inited;
		}
		public override void init()
		{
			// required
			//this.sx = opt.sx; // filter size
			this.in_depth = in_layer.out_depth;
			this.in_sx = in_layer.out_sx;
			this.in_sy = in_layer.out_sy;

			// computed
			this.out_depth = this.in_depth;
			this.out_sx = in_sx / this.stride;
			this.out_sy = in_sy / this.stride;

			// store switches for x,y coordinates for where the max comes from, for each output neuron
			this.switchx = new int[this.out_sx * this.out_sy * this.out_depth];//Util.zeros(this.out_sx * this.out_sy * this.out_depth);
			this.switchy = new int[this.out_sx * this.out_sy * this.out_depth];//Util.zeros(this.out_sx * this.out_sy * this.out_depth);

			this.out_act = new Vol(this.out_sx, this.out_sy, this.out_depth, 0.0f);
			_inited = true;
		}

		public override Vol forward(Vol V)
		{
			this.in_act = V;
			int n = 0; // a counter for switches
			for (int d = 0; d < this.out_depth; d++)
			{
				int x = 0;
				int y = 0;
				for (int ax = 0; ax < this.out_sx; x += stride, ax++)
				{
					y = 0;
					for (int ay = 0; ay < this.out_sy; y += stride, ay++)
					{
						// convolve centered at this particular location
						float a = V.get(x, y, d); // hopefully small enough ;\
						int winx = 0;
						int winy = 0;
						for (int fx = 0; fx < stride; fx++)
						{
							for (int fy = 0; fy < stride; fy++)
							{
								int oy = y + fy;
								int ox = x + fx;
								if (oy >= 0 && oy < V.sy && ox >= 0 && ox < V.sx)
								{
									float v = V.get(ox, oy, d);
									// perform max pooling and store pointers to where
									// the max came from. This will speed up backprop 
									// and can help make nice visualizations in future
									if (v > a)
									{
										a = v;
										winx = ox;
										winy = oy;
									}
								}
							}
						}
						this.switchx[n] = winx;
						this.switchy[n] = winy;
						n++;
						out_act.set(ax, ay, d, a);
					}
				}
			};
			return this.out_act;
		}
		public override void backward()
		{
			// pooling layers have no parameters, so simply compute 
			// gradient wrt data here
			for (int i = 0; i < in_act.dw.size; i++)
			{
				in_act.dw[i] = 0;
			}

			int n = 0;
			for (int d = 0; d < this.out_depth; d++)
			{
				int x = 0;
				int y = 0;
				for (int ax = 0; ax < this.out_sx; x += this.stride, ax++)
				{
					y = 0;
					for (int ay = 0; ay < this.out_sy; y += this.stride, ay++)
					{
						float chain_grad = this.out_act.get_grad(ax, ay, d);
						in_act.add_grad(this.switchx[n], this.switchy[n], d, chain_grad);
						n++;
					}
				}
			};
		}
	}
	public class SoftmaxLayer : LastLayer
	{
		DataSet y;
		float loss;
		public override void setData(DataSet y)
		{
			this.y = y;
		}

		public override float getLoss()
		{
			return loss;
		}
		// Layers that implement a loss. Currently these are the layers that 
		// can initiate a backward() pass. In future we probably want a more 
		// flexible system that can accomodate multiple losses to do multi-task
		// learning, and stuff like that. But for now, one of the layers in this
		// file must be the final layer in a Net.

		// This is a classifier, with N discrete classes from 0 to N-1
		// it gets a stream of N incoming numbers and computes the softmax
		// function (exponentiate and normalize to sum to 1 as probabilities should)
		public int num_inputs;
		//public MyFloat es;
		public override bool inited()
		{
			return _inited;
		}
		public override void init()
		{
			this.num_inputs = in_layer.out_sx * in_layer.out_sy * in_layer.out_depth;
			this.out_depth = this.num_inputs;
			this.out_sx = 1;
			this.out_sy = 1;

			//es = MyFloat.getArray(this.out_depth);
			this.out_act = new Vol(1, 1, this.out_depth, 0.0f);
			_inited = true;
		}

		public override Vol forward(Vol V)
		{
			this.in_act = V;

			// compute max activation
			float amax = V.w[0];
			for (int i = 1; i < this.out_depth; i++)
			{
				if (V.w[i] > amax) amax = V.w[i];
			}

			// compute exponentials (carefully to not blow up)

			float esum = 0.0f;
			for (int i = 0; i < this.out_depth; i++)
			{
				float e = (float)Math.Exp(V.w[i] - amax);
				esum += e;
				out_act.w[i] = e;
			}

			// normalize and output to sum to one
			for (int i = 0; i < this.out_depth; i++)
			{
				out_act.w[i] /= esum;
				//out_act.w[i] = es[i];
			}

			//this.es = es; // save these for backprop
			//this.out_act = A;
			return this.out_act;
		}
		public override void backward()
		{
			// compute and accumulate gradient wrt weights and bias of this layer
			Vol x = this.in_act;

			for (int i = 0; i < x.dw.size; i++)
			{
				x.dw[i] = 0;
			}

			for (int i = 0; i < this.out_depth; i++)
			{
				float indicator = (i == y.predict) ? 1.0f : 0.0f;
				float mul = -(indicator - out_act.w[i]);
				x.dw[i] = mul;
			}

			// loss is the class negative log likelihood
			this.loss = -(float)Math.Log(out_act.w[y.predict]);
		}
	}
	public class RegressionLayer : LastLayer
	{
		DataSet y;
		float loss;
		public override void setData(DataSet y)
		{
			this.y = y;
		}

		public override float getLoss()
		{
			return loss;
		}
		// implements an L2 regression cost layer,
		// so penalizes \sum_i(||x_i - y_i||^2), where x is its input
		// and y is the user-provided array of "correct" values.
		int num_inputs;
		//		public RegressionLayer(Def def)
		//		{
		//			Def opt = def;
		//
		//			// computed
		//			this.num_inputs = opt.in_sx * opt.in_sy * opt.in_depth;
		//			this.out_depth = this.num_inputs;
		//			this.out_sx = 1;
		//			this.out_sy = 1;
		//			//this.layer_type = "regression";
		//		}
		public override bool inited()
		{
			return _inited;
		}
		public override void init()
		{
			this.num_inputs = in_layer.out_sx * in_layer.out_sy * in_layer.out_depth;
			this.out_depth = this.num_inputs;
			this.out_sx = 1;
			this.out_sy = 1;
			_inited = true;
		}

		public override Vol forward(Vol V)
		{
			this.in_act = V;
			return V; // identity function
		}
		// y is a list here of size num_inputs
		public override void backward()
		{
			// compute and accumulate gradient wrt weights and bias of this layer
			float loss = 0.0f;
			for (int i = 0; i < this.out_depth; i++)
			{
				float dy = in_act.w[i] - y.data.w[i];
				in_act.dw[i] = dy;
				loss += 2 * dy * dy;
			}
			this.loss = loss;
		}
	}
	public class InputLayer : Layer
	{
		//public InputLayer(Def def)
		//{
		//	Def opt = def;
		//
		//	// this is a bit silly but lets allow people to specify either ins or outs
		//	this.out_sx = opt.out_sx != 0 ? opt.out_sx : opt.in_sx;
		//	this.out_sy = opt.out_sy != 0 ? opt.out_sy : opt.in_sy;
		//	this.out_depth = opt.out_depth != 0 ? opt.out_depth : opt.in_depth;
		//	//this.layer_type = "input";
		//}
		public InputLayer(int out_sx = 0, int out_sy = 0, int out_depth = 0)
		{
			this.out_sx = out_sx;
			this.out_sy = out_sy;
			this.out_depth = out_depth;
		}

		public override bool inited()
		{
			return true;
		}
		public override void init()
		{
		}

		public override Vol forward(Vol V)
		{
			this.out_act = V;
			return this.out_act; // dummy identity function for now
		}

		public override void backward()
		{
		}
	}
	public class ReshapeLayer : Layer
	{
		public ReshapeLayer(int out_sx = 0, int out_sy = 0, int out_depth = 0)
		{
			this.out_sx = out_sx;
			this.out_sy = out_sy;
			this.out_depth = out_depth;
		}

		public override bool inited()
		{
			return _inited;
		}
		public override void init()
		{
			this.out_act = new Vol(out_sx, out_sy, out_depth, null);
			_inited = true;
		}
		public override Vol forward(Vol V)
		{
			this.in_act = V;
			int N = V.w.size;
			for (int i = 0; i < N; i++)
			{
				this.out_act.w[i] = this.in_act.w[i];
			}
			return this.out_act;
		}
		public override void backward()
		{
			Vol V = this.in_act; // we need to set dw of this
			int N = V.w.size;
			for (int i = 0; i < N; i++)
			{
				in_act.dw[i] = out_act.dw[i];
			}
		}
	}

	//LocalResponseNormalizationLayer
	public class LRNLayer : Layer
	{
		// a bit experimental layer for now. I think it works but I'm not 100%
		// the gradient check is a bit funky. I'll look into this a bit later.
		// Local Response Normalization in window, along depths of volumes
		public float k;
		public int n;
		public float alpha;
		public float beta;

		public Vol S_cache_;

		public LRNLayer(float k, int n, float alpha, float beta)
		{

			// required
			this.k = k;
			this.n = n;
			this.alpha = alpha;
			this.beta = beta;

			// computed
			//this.layer_type = "lrn";

			// checks
			//if (this.n % 2 == 0)
			//{
			//	//console.log('WARNING n should be odd for LRN layer');
			//	throw new Exception();
			//}
		}
		public override bool inited()
		{
			return _inited;
		}
		public override void init()
		{
			this.out_sx = in_layer.out_sx;
			this.out_sy = in_layer.out_sy;
			this.out_depth = in_layer.out_depth;
			this.out_act = new Vol(out_sx, out_sy, out_depth, 0);
			this.S_cache_ = new Vol(out_sx, out_sy, out_depth, 0);
			_inited = true;
		}

		public override Vol forward(Vol V)
		{
			this.in_act = V;

			int n2 = this.n / 2;
			for (int x = 0; x < in_act.sx; x++)
			{
				for (int y = 0; y < in_act.sy; y++)
				{
					for (int i = 0; i < in_act.depth; i++)
					{

						float ai = in_act.get(x, y, i);

						// normalize in a window of size n
						float den = 0.0f;
						for (int j = Math.Max(0, i - n2); j <= Math.Min(i + n2, in_act.depth - 1); j++)
						{
							float aa = in_act.get(x, y, j);
							den += aa * aa;
						}
						den *= this.alpha / this.n;
						den += this.k;
						this.S_cache_.set(x, y, i, den); // will be useful for backprop
						den = (float)Math.Pow(den, this.beta);
						out_act.set(x, y, i, ai / den);
					}
				}
			}

			//this.out_act = A;
			return this.out_act; // dummy identity function for now
		}
		public override void backward()
		{
			// evaluate gradient wrt data
			for (int i = 0; i < in_act.dw.size; i++)
			{
				in_act.dw[i] = 0;
			}

			int n2 = this.n / 2;
			for (int x = 0; x < in_act.sx; x++)
			{
				for (int y = 0; y < in_act.sy; y++)
				{
					for (int i = 0; i < in_act.depth; i++)
					{

						float chain_grad = this.out_act.get_grad(x, y, i);
						float S = this.S_cache_.get(x, y, i);
						float SB = (float)Math.Pow(S, this.beta);
						float SB2 = SB * SB;

						// normalize in a window of size n
						for (int j = Math.Max((int)0, i - n2); j <= Math.Min(i + n2, in_act.depth - 1); j++)
						{
							float aj = in_act.get(x, y, j);
							float g = -aj * this.beta * (float)Math.Pow(S, this.beta - 1) * this.alpha / this.n * 2 * aj;
							if (j == i) g += SB;
							g /= SB2;
							g *= chain_grad;
							in_act.add_grad(x, y, j, g);
						}

					}
				}
			}
		}
	}




}
