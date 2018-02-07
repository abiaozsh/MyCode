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

	public class DeConvLayer : TrainableLayer
	{
		public string name;
		//public override void save(TextWriter s)
		//{
		//	bias_w.save(s);
		//	filters_w.save(s);
		//}
		public override void load(TextReader s)
		{
			bias_w.load(s);
			filters_w.load(s);
		}
		public override void load(BinaryReader s)
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
		public MyFloat bias_w;

		ActivationLayer act;
		public DeConvLayer(int filterSize = 0, int out_sx = 0, int out_sy = 0, int out_depth = 0, int stride = 1, ActivationLayer act = null)//if pad = sx(sy) / 2  the size is same   bigger pad bigger out
		{
			// required
			this.out_depth = out_depth;
			this.sx = filterSize; // filter size. Should be odd if possible, it's cleaner.
			this.sy = filterSize;

			this.out_sx = out_sx;
			this.out_sy = out_sy;

			// optional
			this.stride = stride;
			//pad = 0 if filterSize = 3
			//pad关联 filterSize  0:3  1:5 (?2:7)
			if (filterSize == 3)
			{
				this.pad = 0;
			}
			else if (filterSize == 5)
			{
				this.pad = 1;
			}
			else if (filterSize == 7)
			{
				this.pad = 2;
			}
			else
			{
				throw new Exception("filterSize wrong");
			}
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
				//this.out_sx = (in_sx + pad * 2 - sx + adj) / stride + 1;
				int _out_sx = (this.in_sx - 1) * stride + sx - pad * 2;
				//this.out_sy = (in_sy + pad * 2 - sy + adj) / stride + 1;
				int _out_sy = (this.in_sy - 1) * stride + sy - pad * 2;
				//_out_sx = this.out_sx;
				//_out_sy = this.out_sy;
			}


			this.filters_w = new MyFloat(this.sx * this.sy * this.in_depth * out_depth);
			Vol.init(filters_w, sx * sy * in_depth * out_depth, null);

			this.bias_w = new MyFloat(out_depth);
			Vol.init(bias_w, out_depth, null);


			_inited = true;

			if (act != null)
			{
				act.in_layer = this;
				act.init();
			}
		}
		public override Instance getInstance()
		{
			//public MyFloat filters_dw;
			//MyFloat bias_dw;

			TrainableInstance ins = new TrainableInstance();
			ins.out_act = new Vol(out_sx, out_sy, this.out_depth, 0.0f);


			if (act != null)
			{
				ins.actIns = act.getInstance();
			}

			return ins;

		}

		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern void SSE_UCVFWD(
			int out_sx,
			int out_sy,
			int out_depth,
			int sx,
			int sy,
			int pad,
			int stride,
			int in_sx,
			int in_sy,
			int in_depth,
			IntPtr p_in_act_dw,
			IntPtr p_out_act_dw,
			IntPtr p_filters_w,
			IntPtr p_bias_w
		);
		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern void UCVFWD(
			int out_sx,
			int out_sy,
			int out_depth,
			int sx,
			int sy,
			int pad,
			int stride,
			int in_sx,
			int in_sy,
			int in_depth,
			IntPtr p_in_act_dw,
			IntPtr p_out_act_dw,
			IntPtr p_filters_w,
			IntPtr p_bias_w
		);

		public override Vol forward(Instance ins, Vol V)
		{
			// optimized code by @mdda that achieves 2x speedup over previous version

			ins.in_act = V;

			if (Util.useSSE && in_depth % 8 == 0)
			{
				SSE_UCVFWD(
					out_sx,//int in_sx,
					out_sy,//int in_sy,
					out_depth,//int in_depth,
					sx,//int sx,
					sy,//int sy,
					pad,//int pad,
					stride,//int stride,
					in_sx,//int out_sx,
					in_sy,//int out_sy,
					in_depth,//int out_depth,
					ins.in_act.w.p,//MyFloat p_in_act_dw,
					ins.out_act.w.p,//MyFloat p_out_act_dw,
					this.filters_w.p,//MyFloat p_filters_w
					this.bias_w.p//MyFloat p_bias_w,
				);
			}
			else
			{
				UCVFWD(
					out_sx,//int in_sx,
					out_sy,//int in_sy,
					out_depth,//int in_depth,
					sx,//int sx,
					sy,//int sy,
					pad,//int pad,
					stride,//int stride,
					in_sx,//int out_sx,
					in_sy,//int out_sy,
					in_depth,//int out_depth,
					ins.in_act.w.p,//MyFloat p_in_act_dw,
					ins.out_act.w.p,//MyFloat p_out_act_dw,
					this.filters_w.p,//MyFloat p_filters_w
					this.bias_w.p//MyFloat p_bias_w,
				);
			}

			if (act != null)
			{
				return act.forward(((TrainableInstance)ins).actIns, ins.out_act);
			}
			else
			{
				return ins.out_act;
			}
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
