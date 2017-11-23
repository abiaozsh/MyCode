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
		int padx;
		int pady;
		//最外层是filter，然后是行单元，然后是行元素 然后是输入层深度，行相邻元素在一起
		public MyFloat filters_w;

		MyFloat[] filters_gsum; //[]?
		MyFloat[] filters_xsum; //[]?

		public MyFloat bias_w;
		bool padding;
		MyFloat bias_gsum; //[]?
		MyFloat bias_xsum; //[]?
		float bias_pref;
		ActivationLayer act;
		public ConvLayer(int filterSize = 0, int out_depth = 0, int stride = 1, bool padding = true, float bias_pref = 0.0f, ActivationLayer act = null)//if pad = sx(sy) / 2  the size is same   bigger pad bigger out
		{
			// required
			this.out_depth = out_depth;
			this.sx = filterSize; // filter size. Should be odd if possible, it's cleaner.
			this.sy = filterSize;

			this.bias_pref = bias_pref;

			this.padding = padding;
			// optional
			this.stride = stride; // stride at which we apply filters to input volume
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
				//1、如果padding = ‘VALID’
				if (!padding)
				{
					//new_height = new_width = (W – F + 1) / S （结果向上取整）
					out_sx = (in_sx - sx + 1) / stride;
					out_sy = (in_sy - sy + 1) / stride;
					padx = 0;
					pady = 0;
				}
				//也就是说，conv2d的VALID方式不会在原有输入的基础上添加新的像素（假定我们的输入是图片数据，因为只有图片才有像素），输出矩阵的大小直接按照公式计算即可。
				//2、如果padding = ‘SAME’
				else
				{
					//new_height = new_width = W / S （结果向上取整）
					out_sx = in_sx / stride;
					out_sy = in_sy / stride;

					int pad_needed_x = (out_sx - 1) * stride + sx - in_sx;
					int pad_needed_y = (out_sy - 1) * stride + sy - in_sy;

					padx = pad_needed_x / 2;
					pady = pad_needed_y / 2;

				}

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
		static extern void SSE_CVFWD(
			int stride,
			int pad,
			int sx,
			int sy,
			int in_sx,
			int in_sy,
			int in_depth,
			int out_sx,
			int out_sy,
			int out_depth,
			IntPtr p_filters_w,
			IntPtr p_in_act_w,
			IntPtr p_bias_w,
			IntPtr p_out_act_w
		);
		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern void CVFWD(
			int stride,
			int pad,
			int sx,
			int sy,
			int in_sx,
			int in_sy,
			int in_depth,
			int out_sx,
			int out_sy,
			int out_depth,
			IntPtr p_filters_w,
			IntPtr p_in_act_w,
			IntPtr p_bias_w,
			IntPtr p_out_act_w
		);

		public override Vol forward(Instance ins, Vol V)
		{
			// optimized code by @mdda that achieves 2x speedup over previous version

			ins.in_act = V;

			//if (Util.useSSE && in_depth % 8 == 0)
			//{
			//	SSE_CVFWD(
			//		stride,
			//		pad,
			//		sx,
			//		sy,
			//		in_sx,
			//		in_sy,
			//		in_depth,
			//		out_sx,
			//		out_sy,
			//		out_depth,
			//		filters_w.p,
			//		ins.in_act.w.p,
			//		bias_w.p,
			//		ins.out_act.w.p);
			//}
			//else
			//{
			//	CVFWD(
			//		stride,
			//		pad,
			//		sx,
			//		sy,
			//		in_sx,
			//		in_sy,
			//		in_depth,
			//		out_sx,
			//		out_sy,
			//		out_depth,
			//		filters_w.p,
			//		ins.in_act.w.p,
			//		bias_w.p,
			//		ins.out_act.w.p);
			//}
			int filterSize = sx * sy * in_depth;
			for (int out_y = 0; out_y < out_sy; out_y++)
			{
				int frame_y = out_y * stride - pady;
				int out_act_sx_out_y = out_sx * out_y;
				for (int out_x = 0; out_x < out_sx; out_x++)
				{
					int frame_x = out_x * stride - padx;
					// convolve centered at this particular location
					for (int d = 0; d < out_depth; d++)
					{
						//Vol f = this.filters[d];
						int filterIdx = d * filterSize;
						float a = 0.0f;
						for (int fy = 0; fy < sy; fy++)
						{
							int oy = frame_y + fy; // coordinates in the original input array coordinates
							int oy2 = (in_sx * oy);
							int fy2 = (fy * sx);
							for (int fx = 0; fx < sx; fx++)
							{
								int ox = frame_x + fx;
								if (oy >= 0 && oy < in_sy && ox >= 0 && ox < in_sx)
								{
									int fidx = (fy2 + fx) * in_depth + filterIdx;
									int Vidx = (oy2 + ox) * in_depth;
									for (int fd = 0; fd < in_depth; fd++)
									{
										// avoid function call overhead (x2) for efficiency, compromise modularity :(
										a += filters_w[fidx + fd] * ins.in_act.w[Vidx + fd];
									}
								}
							}
						}
						a += bias_w[d];
						ins.out_act.w[(out_act_sx_out_y + out_x) * out_depth + d] = a;
					}
				}
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
