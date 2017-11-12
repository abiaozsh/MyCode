using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace ConvNet
{

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

		MyFloat[] filters_gsum; //[]?
		MyFloat[] filters_xsum; //[]?

		MyFloat bias_w;

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

			this.filters_w = new MyFloat(this.sx * this.sy * this.in_depth * out_depth);
			Vol.init(filters_w, sx * sy * in_depth * out_depth, null);

			this.bias_w = new MyFloat(out_depth);
			Vol.init(bias_w, out_depth, null);


			this.bias_gsum = new MyFloat(out_depth); // last iteration gradients (used for momentum calculations)
			this.bias_xsum = new MyFloat(out_depth); // used in adadelta
			for (int j = 0; j < out_depth; j++)
			{
				bias_gsum[j] = 0.0f;
				bias_xsum[j] = 0.0f;
			}
			_inited = true;

			if (act != null)
			{
				act.in_layer = this;
				act.init();
			}
		}
		public override Instance getInstance(int Count)
		{
		//public MyFloat filters_dw;
		//MyFloat bias_dw;

			TrainableInstance ins = new TrainableInstance();
            ins.out_act = new MultiVol(Count, out_sx, out_sy, this.out_depth, 0.0f);

			ins.filters_dw = new MyFloat(sx * sy * in_depth * out_depth);
			Vol.init(ins.filters_dw, sx * sy * in_depth * out_depth, 0.0f);

			ins.bias_dw = new MyFloat(out_depth);
			Vol.init(ins.bias_dw, out_depth, 0.0f);

			if (act != null)
			{
                ins.actIns = act.getInstance(Count);
			}

			return ins;

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

		public override MultiVol forward(Instance instance, MultiVol V)
		{
			// optimized code by @mdda that achieves 2x speedup over previous version

			instance.in_act = V;
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
				in_sx,
				in_sy,
				filters_w.getHostMemPointReadOnly(),
				instance.in_act.w.getHostMemPointReadOnly(),
				bias_w.getHostMemPointReadOnly(),
				instance.out_act.w.getHostMemPointReadWrite()
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
				return act.forward(((TrainableInstance)instance).actIns, instance.out_act);
			}
			else
			{
				return instance.out_act;
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

		public override void backward(Instance instance)
		{
			if (noUpdate) return;

			TrainableInstance trainableInstance = ((TrainableInstance)instance);

			//zero out gradient wrt bottom data, we're about to fill it
			int in_size = in_sx * in_sy * in_depth;
			int filterSize = sx * sy * in_depth;

			if (act != null)
			{
				act.backward(trainableInstance.actIns);
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
				in_sx,
				in_sy,
				filters_w.getHostMemPointReadWrite(),
				instance.in_act.w.getHostMemPointReadWrite(),
				instance.in_act.dw.getHostMemPointReadWrite(),
				instance.out_act.dw.getHostMemPointReadWrite(),
				trainableInstance.bias_dw.getHostMemPointReadWrite(),
				trainableInstance.filters_dw.getHostMemPointReadWrite()
			);
		}

		//public void backward2(Instance instance)
		//{
		//	TrainableInstance trainableInstance = ((TrainableInstance)instance);
		//
		//	//????????TODO 未初始化可能影响后续????????????????V.dw = Util.zeros(V.w.Length); // zero out gradient wrt bottom data, we're about to fill it
		//	int in_size = in_sx * in_sy * in_depth;
		//	int filterSize = sx * sy * in_depth;
		//
		//	if (act != null)
		//	{
		//		act.backward(trainableInstance.actIns);
		//	}
		//
		//	CVBWD(
		//		stride,
		//		pad,
		//		sx,
		//		sy,
		//		in_size,
		//		in_depth,
		//		out_sx,
		//		out_sy,
		//		out_depth,
		//		filterSize,
		//		in_sx,
		//		in_sy,
		//		filters_w.ori_p,
		//		instance.in_act.w.ori_p,
		//		instance.in_act.dw.ori_p,
		//		instance.out_act.dw.ori_p,
		//		trainableInstance.bias_dw.ori_p,
		//		trainableInstance.filters_dw.ori_p
		//	);
		//}


		public bool noUpdate = false;
		public override void train(TrainableInstance instance, Trainer trainer, float oneBatchSize)
		{
			if (noUpdate) return;
			for (int i = 0; i < this.out_depth; i++)
			{
				int filterSize = sx * sy * in_depth;
				trainer.train(
					filterSize,//params_size = 
					this.filters_w,//params_ = 
					instance.filters_dw,//grads_ = 
					i * filterSize,//params_idx = 
					i * filterSize,//grads_idx = 
					filters_gsum[i],//gsum = 
					filters_xsum[i],//xsum = 
					this.l2_decay_mul,//l2_decay_mul = 
					this.l1_decay_mul,//l1_decay_mul = 
					oneBatchSize
				);
			}
			trainer.train(
				this.out_depth,//params_size = 
				this.bias_w,//params_ = 
				instance.bias_dw,//grads_ = 
				0,//params_idx = 
				0,//grads_idx = 
				bias_gsum,//gsum = 
				bias_xsum,//xsum = 
				0.0f,//l1_decay_mul = 
				l2_decay_mul = 0.0f,
				oneBatchSize
			);
		}

		public Bitmap vis(int idx, float scale)
		{
			Bitmap b = new Bitmap(sx, sy);
			for (int i = 0; i < sx; i++)
			{
				for (int j = 0; j < sy; j++)
				{
					float v = filters_w[idx * filterSize + ((this.sx * j) + i) + 0] * scale;
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
}
