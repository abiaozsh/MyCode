﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using System.Drawing;

namespace ConvNet
{

	public abstract class Layer : Persistence
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
		public abstract void save(TextWriter s);
		public abstract void load(TextReader s);
	}

	public abstract class LastLayer : Layer
	{
		public abstract void setData(DataSet y);

		public abstract float getLoss();
	}
	public abstract class ActivationLayer : Layer
	{
	}
	public abstract class TrainableLayer : Layer
	{
		public abstract void train(Trainer trainer);
	}


	public class FullyConnLayer : TrainableLayer
	{
		public override void save(TextWriter s)
		{
			bias.save(s);
			filters.save(s);
		}
		public override void load(TextReader s)
		{
			bias.load(s);
			filters.load(s);
		}
		//Vol[] filters;
		Vol filters;
		MyFloat[] filters_gsum; //[]?
		MyFloat[] filters_xsum; //[]?

		Vol bias;
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
			filters = new Vol(1, 1, this.num_inputs * out_depth, null);


			this.bias = new Vol(1, 1, this.out_depth, bias_pref);
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

		public override Vol forward(Vol V)
		{
			this.in_act = V;

			FCFWD(out_depth, num_inputs, in_act.w.ori_p, filters.w.ori_p, bias.w.ori_p, out_act.w.ori_p);

			//for (int i = 0; i < this.out_depth; i++)
			//{
			//	float a = 0.0f;
			//	//int iw = i * this.num_inputs;
			//	for (int d = 0; d < this.num_inputs; d++)
			//	{
			//		a += in_act.w[d] * this.filters.w[i * num_inputs + d]; // for efficiency use Vols directly for now
			//	}
			//	//a += this.biases.w[i];
			//	a += this.bias.w[i];
			//	this.out_act.w[i] = a;
			//}
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
			if (act != null)
			{
				act.backward();
			}
			FCBWD(out_depth, num_inputs, in_act.w.ori_p, filters.w.ori_p, in_act.dw.ori_p, out_act.dw.ori_p, filters.dw.ori_p, bias.dw.ori_p);

			//for (int d = 0; d < num_inputs; d++)
			//{
			//	in_act.dw[d] = 0;
			//}
			//// compute gradient wrt weights and data
			//for (int i = 0; i < this.out_depth; i++)
			//{
			//	//Vol tfi = this.filters[i];
			//	//int iw = i * this.num_inputs;
			//	float chain_grad = this.out_act.dw[i];
			//	for (int d = 0; d < this.num_inputs; d++)
			//	{
			//		in_act.dw[d] += this.filters.w[i * num_inputs + d] * chain_grad; // grad wrt input data
			//		this.filters.dw[i * num_inputs + d] += in_act.w[d] * chain_grad; // grad wrt params
			//	}
			//	this.bias.dw[i] += chain_grad;
			//}
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
					this.filters.w,//params_ = 
					this.filters.dw,//grads_ = 
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
				this.bias.w,//params_ = 
				this.bias.dw,//grads_ = 
				0,//params_idx = 
				0,//grads_idx = 
				bias_gsum,//gsum = 
				bias_xsum,//xsum = 
				0.0f,//l1_decay_mul = 
				0.0f//l2_decay_mul = 
			);
		}
	}
	public class ReluLayer : ActivationLayer
	{
		public override void save(TextWriter s)
		{
		}
		public override void load(TextReader s)
		{
		}
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
	public class PoolLayer : Layer
	{
		public override void save(TextWriter s) { }
		public override void load(TextReader s) { }
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
			Parallel.For(0, this.out_depth, (d) =>//for (int d = 0; d < this.out_depth; d++)
			{
				int x = 0;
				int y = 0;
				for (int ax = 0; ax < this.out_sx; x += stride, ax++)
				{
					y = 0;
					for (int ay = 0; ay < this.out_sy; y += stride, ay++)
					{
						// convolve centered at this particular location
						float a = -99999; // hopefully small enough ;\
						int winx = -1;
						int winy = -1;
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
			});
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
			Parallel.For(0, this.out_depth, (d) =>//for (int d = 0; d < this.out_depth; d++)
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
			});
		}
	}
	class UnPoolLayer : Layer
	{
		/*
		1
		2 (3) -> a
		3  <-(a)

		??不实现了

		  c<-    a
		1 (1)->  b
				 c
		 * */

		public override void save(TextWriter s) { }
		public override void load(TextReader s) { }
		public int sx;
		public int sy;
		public int in_depth;
		public int in_sx;
		public int in_sy;
		public int stride;
		public int pad;

		//		public UnPoolLayer(Def def)
		//		{
		//			Def opt = def;
		//
		//			// required
		//			this.sx = opt.sx; // filter size
		//			this.in_depth = opt.in_depth;
		//			this.in_sx = opt.in_sx;
		//			this.in_sy = opt.in_sy;
		//
		//			// optional
		//			this.sy = opt.sy != 0 ? opt.sy : this.sx;
		//			this.stride = opt.stride != 0 ? opt.stride : 2;
		//			this.pad = opt.pad != 0 ? opt.pad : 0; // amount of 0 padding to add around borders of input volume
		//
		//			// computed
		//			this.out_depth = this.in_depth;
		//			this.out_sx = (int)Math.Floor((float)((this.in_sx - this.pad * 2 - this.sx) * this.stride + 1));
		//			this.out_sy = (int)Math.Floor((float)((this.in_sy - this.pad * 2 - this.sy) * this.stride + 1));
		//			//this.layer_type = "pool";
		//			// store switches for x,y coordinates for where the max comes from, for each output neuron
		//
		//			this.out_act = new Vol(this.out_sx, this.out_sy, this.out_depth, 0.0f);
		//		}
		public override bool inited()
		{
			return _inited;
		}
		public override void init()
		{
		}

		public override Vol forward(Vol V)
		{
			//			this.in_act = V;
			//			int n = 0; // a counter for switches
			//			Parallel.For(0, this.out_depth, (d) =>//for (int d = 0; d < this.out_depth; d++)
			//			{
			//				int x = -this.pad;
			//				int y = -this.pad;
			//				for (int ax = 0; ax < this.out_sx; x += this.stride, ax++)
			//				{
			//					y = -this.pad;
			//					for (int ay = 0; ay < this.out_sy; y += this.stride, ay++)
			//					{
			//
			//						// convolve centered at this particular location
			//						float a = -99999; // hopefully small enough ;\
			//						int winx = -1;
			//						int winy = -1;
			//						for (int fx = 0; fx < this.sx; fx++)
			//						{
			//							for (int fy = 0; fy < this.sy; fy++)
			//							{
			//								int oy = y + fy;
			//								int ox = x + fx;
			//								if (oy >= 0 && oy < V.sy && ox >= 0 && ox < V.sx)
			//								{
			//									float v = V.get(ox, oy, d);
			//									// perform max pooling and store pointers to where
			//									// the max came from. This will speed up backprop 
			//									// and can help make nice visualizations in future
			//									if (v > a)
			//									{
			//										a = v;
			//										winx = ox;
			//										winy = oy;
			//									}
			//								}
			//							}
			//						}
			//						this.switchx[n] = winx;
			//						this.switchy[n] = winy;
			//						n++;
			//						out_act.set(ax, ay, d, a);
			//					}
			//				}
			//			});
			return this.out_act;
		}
		public override void backward()
		{
			//			// pooling layers have no parameters, so simply compute 
			//			// gradient wrt data here
			//			for (int i = 0; i < in_act.dw.size; i++)
			//			{
			//				in_act.dw[i] = 0;
			//			}
			//
			//			int n = 0;
			//			Parallel.For(0, this.out_depth, (d) =>//for (int d = 0; d < this.out_depth; d++)
			//			{
			//				int x = -this.pad;
			//				int y = -this.pad;
			//				for (int ax = 0; ax < this.out_sx; x += this.stride, ax++)
			//				{
			//					y = -this.pad;
			//					for (int ay = 0; ay < this.out_sy; y += this.stride, ay++)
			//					{
			//
			//						float chain_grad = this.out_act.get_grad(ax, ay, d);
			//						in_act.add_grad(this.switchx[n], this.switchy[n], d, chain_grad);
			//						n++;
			//
			//					}
			//				}
			//			});
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
		public override void save(TextWriter s)
		{
		}
		public override void load(TextReader s)
		{
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

		public override void save(TextWriter s)
		{
		}
		public override void load(TextReader s)
		{
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
	public class ConvLayer : TrainableLayer
	{
		public override void save(TextWriter s)
		{
			bias.save(s);
			filters.save(s);
		}
		public override void load(TextReader s)
		{
			bias.load(s);
			filters.load(s);
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
		int unstride;
		int pad;
		//最外层是filter，然后是行单元，然后是行元素 然后是输入层深度，行相邻元素在一起
		public Vol filters;
		MyFloat[] filters_gsum; //[]?
		MyFloat[] filters_xsum; //[]?
		Vol bias;
		MyFloat bias_gsum; //[]?
		MyFloat bias_xsum; //[]?
		float bias_pref;

		ActivationLayer act;
		public ConvLayer(int sx = 0, int sy = 0, int filters = 0, int stride = 1, int unstride = 0, int pad = 0, float bias_pref = 0.0f, ActivationLayer act = null)//if pad = sx(sy) / 2  the size is same   bigger pad bigger out
		{
			// required
			this.out_depth = filters;
			this.sx = sx; // filter size. Should be odd if possible, it's cleaner.
			this.sy = sy;

			this.bias_pref = bias_pref;

			// optional
			this.stride = stride; // stride at which we apply filters to input volume
			this.unstride = unstride;
			this.pad = pad; // amount of 0 padding to add around borders of input volume
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
			if (unstride > 0)
			{
				this.out_sx = (this.in_sx + this.pad * 2 - this.sx) * this.unstride;
				this.out_sy = (this.in_sy + this.pad * 2 - this.sy) * this.unstride;
			}
			else
			{
				this.out_sx = (this.in_sx + this.pad * 2 - this.sx) / this.stride;
				this.out_sy = (this.in_sy + this.pad * 2 - this.sy) / this.stride;
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
			this.filters = new Vol(this.sx, this.sy, this.in_depth * out_depth, null);

			this.bias = new Vol(1, 1, this.out_depth, bias_pref);
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
			int unstride,
			int pad,
			int sx,
			int sy,
			int in_depth,
			int out_sx,
			int out_sy,
			int out_depth,
			int filterSize,
			int out_act_sx,
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

			CVFWD(
				stride,
				unstride,
				pad,
				sx,
				sy,
				in_depth,
				out_sx,
				out_sy,
				out_depth,
				filterSize,
				out_act.sx,
				in_act.sx,
				in_act.sy,
				filters.w.ori_p,
				in_act.w.ori_p,
				bias.w.ori_p,
				out_act.w.ori_p
			);

			//if (unstride > 0)
			//{
			//	for (int out_x = 0; out_x < out_sx; out_x++)
			//	{
			//		int frame_x = out_x / unstride - pad;
			//		for (int out_y = 0; out_y < out_sy; out_y++)
			//		{
			//			int frame_y = out_y / unstride - pad;
			//			int out_act_sx_out_y = out_act.sx * out_y;
			//			// convolve centered at this particular location
			//			for (int d = 0; d < out_depth; d++)
			//			{
			//				//Vol f = this.filters[d];
			//				int filterIdx = d * filterSize;
			//				float a = 0.0f;
			//				for (int fy = 0; fy < sy; fy++)
			//				{
			//					int oy = frame_y + fy; // coordinates in the original input array coordinates
			//					int oy2 = (in_act.sx * oy);
			//					int fy2 = (fy * sx);
			//					for (int fx = 0; fx < sx; fx++)
			//					{
			//						int ox = frame_x + fx;
			//						if (oy >= 0 && oy < in_act.sy && ox >= 0 && ox < in_act.sx)
			//						{
			//							int fidx = (fy2 + fx) * in_depth + filterIdx;
			//							int Vidx = (oy2 + ox) * in_depth;
			//							for (int fd = 0; fd < in_depth; fd++)
			//							{
			//								// avoid function call overhead (x2) for efficiency, compromise modularity :(
			//								a += filters.w[fidx + fd] * in_act.w[Vidx + fd];
			//							}
			//						}
			//					}
			//				}
			//				a += bias.w[d];
			//				//out_act.set(out_x, out_y, d, a);
			//				out_act.w[(out_act_sx_out_y + out_x) * out_depth + d] = a;
			//			}
			//		}
			//	}
			//}
			//else
			//{
			//	for (int out_x = 0; out_x < out_sx; out_x++)
			//	{
			//		int frame_x = out_x * stride - pad;
			//		for (int out_y = 0; out_y < out_sy; out_y++)
			//		{
			//			int frame_y = out_y * stride - pad;
			//			int out_act_sx_out_y = out_act.sx * out_y;
			//			// convolve centered at this particular location
			//			for (int d = 0; d < out_depth; d++)
			//			{
			//				//Vol f = this.filters[d];
			//				int filterIdx = d * filterSize;
			//				float a = 0.0f;
			//				for (int fy = 0; fy < sy; fy++)
			//				{
			//					int oy = frame_y + fy; // coordinates in the original input array coordinates
			//					int oy2 = (in_act.sx * oy);
			//					int fy2 = (fy * sx);
			//					for (int fx = 0; fx < sx; fx++)
			//					{
			//						int ox = frame_x + fx;
			//						if (oy >= 0 && oy < in_act.sy && ox >= 0 && ox < in_act.sx)
			//						{
			//							int fidx = (fy2 + fx) * in_depth + filterIdx;
			//							int Vidx = (oy2 + ox) * in_depth;
			//							for (int fd = 0; fd < in_depth; fd++)
			//							{
			//								// avoid function call overhead (x2) for efficiency, compromise modularity :(
			//								a += filters.w[fidx + fd] * in_act.w[Vidx + fd];
			//							}
			//						}
			//					}
			//				}
			//				a += bias.w[d];
			//				//out_act.set(out_x, out_y, d, a);
			//				out_act.w[(out_act_sx_out_y + out_x) * out_depth + d] = a;
			//			}
			//		}
			//	}
			//}
			//this.out_act = A;
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
			int unstride,
			int pad,
			int sx,
			int sy,
			int in_size,
			int in_depth,
			int out_sx,
			int out_sy,
			int out_depth,
			int filterSize,
			int out_act_sx,
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
			//????????TODO 未初始化可能影响后续????????????????V.dw = Util.zeros(V.w.Length); // zero out gradient wrt bottom data, we're about to fill it
			int in_size = in_sx * in_sy * in_depth;
			int filterSize = sx * sy * in_depth;

			if (act != null)
			{
				act.backward();
			}

			CVBWD(
				stride,
				unstride,
				pad,
				sx,
				sy,
				in_size,
				in_depth,
				out_sx,
				out_sy,
				out_depth,
				filterSize,
				out_act.sx,
				in_act.sx,
				in_act.sy,
				filters.w.ori_p,
				in_act.w.ori_p,
				in_act.dw.ori_p,
				out_act.dw.ori_p,
				bias.dw.ori_p,
				filters.dw.ori_p
			);


			//for (int i = 0; i < in_size; i++)
			//{
			//	in_act.dw[i] = 0;
			//}
			//
			//if (unstride > 0)
			//{
			//	for (int out_x = 0; out_x < out_sx; out_x++)
			//	{
			//		int x = out_x / unstride - pad;
			//		for (int out_y = 0; out_y < out_sy; out_y++)
			//		{
			//			int y = out_y / unstride - pad;
			//			for (int d = 0; d < out_depth; d++)
			//			{
			//				//Vol f = this.filters[d];
			//				int filterIdx = d * filterSize;
			//				// convolve centered at this particular location
			//				//float chain_grad = out_act.get_grad(out_x, out_y, d); // gradient from above, from chain rule
			//				float chain_grad = out_act.dw[((out_act.sx * out_y) + out_x) * out_depth + d];
			//
			//				for (int fy = 0; fy < sy; fy++)
			//				{
			//					int oy = y + fy; // coordinates in the original input array coordinates
			//					int oy2 = (in_act.sx * oy);
			//					int fy2 = (sx * fy);
			//					for (int fx = 0; fx < sx; fx++)
			//					{
			//						int ox = x + fx;
			//						if (oy >= 0 && oy < in_act.sy && ox >= 0 && ox < in_act.sx)
			//						{
			//							int fidx = (fy2 + fx) * in_depth + filterIdx;
			//							int Vidx = (oy2 + ox) * in_depth;
			//							for (int fd = 0; fd < in_depth; fd++)
			//							{
			//								// avoid function call overhead (x2) for efficiency, compromise modularity :(
			//								filters.dw[fidx + fd] += in_act.w[Vidx + fd] * chain_grad;
			//								in_act.dw[Vidx + fd] += filters.w[fidx + fd] * chain_grad;
			//							}
			//						}
			//					}
			//				}
			//				bias.dw[d] += chain_grad;
			//			}
			//		}
			//	}
			//}
			//else
			//{
			//	for (int out_x = 0; out_x < out_sx; out_x++)
			//	{
			//		int x = out_x * stride - pad;
			//		for (int out_y = 0; out_y < out_sy; out_y++)
			//		{
			//			int y = out_y * stride - pad;
			//			for (int d = 0; d < out_depth; d++)
			//			{
			//				//Vol f = this.filters[d];
			//				int filterIdx = d * filterSize;
			//				// convolve centered at this particular location
			//				//float chain_grad = this.out_act.get_grad(out_x, out_y, d); // gradient from above, from chain rule
			//				float chain_grad = out_act.dw[((out_act.sx * out_y) + out_x) * out_depth + d];
			//				for (int fy = 0; fy < sy; fy++)
			//				{
			//					int oy = y + fy; // coordinates in the original input array coordinates
			//					int oy2 = (in_act.sx * oy);
			//					int fy2 = (sx * fy);
			//					for (int fx = 0; fx < sx; fx++)
			//					{
			//						int ox = x + fx;
			//						if (oy >= 0 && oy < in_act.sy && ox >= 0 && ox < in_act.sx)
			//						{
			//							int fidx = (fy2 + fx) * in_depth + filterIdx;
			//							int Vidx = (oy2 + ox) * in_depth;
			//							for (int fd = 0; fd < in_depth; fd++)
			//							{
			//								// avoid function call overhead (x2) for efficiency, compromise modularity :(
			//								filters.dw[fidx + fd] += in_act.w[Vidx + fd] * chain_grad;
			//								in_act.dw[Vidx + fd] += filters.w[fidx + fd] * chain_grad;
			//							}
			//						}
			//					}
			//				}
			//				bias.dw[d] += chain_grad;
			//			}
			//		}
			//	}
			//}
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
					this.filters.w,//params_ = 
					this.filters.dw,//grads_ = 
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
				this.bias.w,//params_ = 
				this.bias.dw,//grads_ = 
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
					float v = filters.w[idx * filterSize + ((this.sx * j) + i) + d];
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

	}
	public class InputLayer : Layer
	{
		public override void save(TextWriter s)
		{
		}
		public override void load(TextReader s)
		{
		}
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
	public class TanhLayer : ActivationLayer
	{
		public override void save(TextWriter s)
		{
		}
		public override void load(TextReader s)
		{
		}
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
	public class ReshapeLayer : Layer
	{
		public override void save(TextWriter s)
		{
		}
		public override void load(TextReader s)
		{
		}
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







	//DropoutLayer
	/*
	class DropoutLayer : Layer
	{
		public override void getRange(Range r)
		{
		}
		public override void save(BinaryWriter s)
		{
		}
		public override void save(StreamWriter s)
		{
		}
		public override void load(BinaryReader s)
		{
		}
		public override void load(StreamReader s)
		{
		}
		// An inefficient dropout layer
		// Note this is not most efficient implementation since the layer before
		// computed all these activations and now we're just going to drop them :(
		// same goes for backward pass. Also, if we wanted to be efficient at test time
		// we could equivalently be clever and upscale during train and copy pointers during test
		// todo: make more efficient.
		public float drop_prob;
		public bool[] dropped;
		public DropoutLayer(Def def)
			: base(def)
		{

			Def opt = def;

			// computed
			this.out_sx = opt.in_sx;
			this.out_sy = opt.in_sy;
			this.out_depth = opt.in_depth;
			//this.layer_type = "dropout";
			this.drop_prob = opt.drop_prob.Value;
			this.dropped = new bool[this.out_sx * this.out_sy * this.out_depth];//Util.zeros(this.out_sx*this.out_sy*this.out_depth);
		}

		static Random r = new Random();

		public override Vol forward(Vol V, bool is_training)
		{
			this.in_act = V;
			//if(typeof(is_training)==='undefined') { is_training = false; } // default is prediction mode
			Vol V2 = V.clone();
			int N = V.w.Length;
			if (is_training)
			{
				// do dropout
				for (int i = 0; i < N; i++)
				{
					if (r.NextDouble() < this.drop_prob) { V2.w[i] = 0; this.dropped[i] = true; } // drop!
					else { this.dropped[i] = false; }
				}
			}
			else
			{
				// scale the activations during prediction
				for (int i = 0; i < N; i++) { V2.w[i] *= this.drop_prob; }
			}
			this.out_act = V2;
			return this.out_act; // dummy identity function for now
		}
		public override float backward(DataSet y)
		{
			Vol V = this.in_act; // we need to set dw of this
			Vol chain_grad = this.out_act;
			int N = V.w.Length;
			V.dw = Util.zeros(N); // zero out gradient wrt data
			for (int i = 0; i < N; i++)
			{
				if (!(this.dropped[i]))
				{
					V.dw[i] = chain_grad.dw[i]; // copy over the gradient
				}
			}
			return 0;
		}
		public override List<ParamsAndGrads> getParamsAndGrads()
		{
			return new List<ParamsAndGrads>();
		}
		//toJSON: function() {
		//  json.out_depth = this.out_depth;
		//  json.out_sx = this.out_sx;
		//  json.out_sy = this.out_sy;
		//  json.layer_type = this.layer_type;
		//  json.drop_prob = this.drop_prob;
		//  return json;
		//},
		//fromJSON: function(json) {
		//  this.out_depth = json.out_depth;
		//  this.out_sx = json.out_sx;
		//  this.out_sy = json.out_sy;
		//  this.layer_type = json.layer_type; 
		//  this.drop_prob = json.drop_prob;
		//}
	}*/
	//LocalResponseNormalizationLayer
	/*
	class LocalResponseNormalizationLayer : Layer
	{
		public override void getRange(Range r)
		{
		}
		public override void save(BinaryWriter s)
		{
		}
		public override void save(StreamWriter s)
		{
		}
		public override void load(BinaryReader s)
		{
		}
		public override void load(StreamReader s)
		{
		}
		// a bit experimental layer for now. I think it works but I'm not 100%
		// the gradient check is a bit funky. I'll look into this a bit later.
		// Local Response Normalization in window, along depths of volumes
		public float k;
		public float n;
		public float alpha;
		public float beta;

		public Vol S_cache_;

		public LocalResponseNormalizationLayer(Def def)
			: base(def)
		{
			Def opt = def;

			// required
			this.k = opt.k;
			this.n = opt.n;
			this.alpha = opt.alpha;
			this.beta = opt.beta;

			// computed
			this.out_sx = opt.in_sx;
			this.out_sy = opt.in_sy;
			this.out_depth = opt.in_depth;
			//this.layer_type = "lrn";

			// checks
			if (this.n % 2 == 0)
			{
				//console.log('WARNING n should be odd for LRN layer');
				throw new Exception();
			}
		}

		public override Vol forward(Vol V, bool is_training)
		{
			this.in_act = V;

			Vol A = V.cloneAndZero();
			this.S_cache_ = V.cloneAndZero();
			int n2 = (int)Math.Floor(this.n / 2);
			for (int x = 0; x < V.sx; x++)
			{
				for (int y = 0; y < V.sy; y++)
				{
					for (int i = 0; i < V.depth; i++)
					{

						float ai = V.get(x, y, i);

						// normalize in a window of size n
						float den = 0.0f;
						for (int j = Math.Max(0, i - n2); j <= Math.Min(i + n2, V.depth - 1); j++)
						{
							float aa = V.get(x, y, j);
							den += aa * aa;
						}
						den *= this.alpha / this.n;
						den += this.k;
						this.S_cache_.set(x, y, i, den); // will be useful for backprop
						den = (float)Math.Pow(den, this.beta);
						A.set(x, y, i, ai / den);
					}
				}
			}

			this.out_act = A;
			return this.out_act; // dummy identity function for now
		}
		public override float backward(DataSet del_y)
		{
			// evaluate gradient wrt data
			Vol V = this.in_act; // we need to set dw of this
			V.dw = Util.zeros(V.w.Length); // zero out gradient wrt data
			Vol A = this.out_act; // computed in forward pass 

			int n2 = (int)Math.Floor(this.n / 2);
			for (int x = 0; x < V.sx; x++)
			{
				for (int y = 0; y < V.sy; y++)
				{
					for (int i = 0; i < V.depth; i++)
					{

						float chain_grad = this.out_act.get_grad(x, y, i);
						float S = this.S_cache_.get(x, y, i);
						float SB = (float)Math.Pow(S, this.beta);
						float SB2 = SB * SB;

						// normalize in a window of size n
						for (int j = Math.Max((int)0, i - n2); j <= Math.Min(i + n2, V.depth - 1); j++)
						{
							float aj = V.get(x, y, j);
							float g = -aj * this.beta * (float)Math.Pow(S, this.beta - 1) * this.alpha / this.n * 2 * aj;
							if (j == i) g += SB;
							g /= SB2;
							g *= chain_grad;
							V.add_grad(x, y, j, g);
						}

					}
				}
			}
			return 0;
		}
		public override List<ParamsAndGrads> getParamsAndGrads()
		{
			return new List<ParamsAndGrads>();
		}
		//toJSON: function() {
		//  json.k = this.k;
		//  json.n = this.n;
		//  json.alpha = this.alpha; // normalize by size
		//  json.beta = this.beta;
		//  json.out_sx = this.out_sx; 
		//  json.out_sy = this.out_sy;
		//  json.out_depth = this.out_depth;
		//  json.layer_type = this.layer_type;
		//  return json;
		//},
		//fromJSON: function(json) {
		//  this.k = json.k;
		//  this.n = json.n;
		//  this.alpha = json.alpha; // normalize by size
		//  this.beta = json.beta;
		//  this.out_sx = json.out_sx; 
		//  this.out_sy = json.out_sy;
		//  this.out_depth = json.out_depth;
		//  this.layer_type = json.layer_type;
		//}
	}
	*/
	//MaxoutLayer
	/*
	class MaxoutLayer : Layer
	{
		public override void getRange(Range r)
		{
		}
		public override void save(BinaryWriter s)
		{
		}
		public override void save(StreamWriter s)
		{
		}
		public override void load(BinaryReader s)
		{
		}
		public override void load(StreamReader s)
		{
		}
		// Implements Maxout nnonlinearity that computes
		// x -> max(x)
		// where x is a vector of size group_size. Ideally of course,
		// the input size should be exactly divisible by group_size
		int group_size;
		int[] switches;

		public MaxoutLayer(Def def)
			: base(def)
		{
			Def opt = def;

			// required
			this.group_size = opt.group_size != 0 ? opt.group_size : 2;

			// computed
			this.out_sx = opt.in_sx;
			this.out_sy = opt.in_sy;
			this.out_depth = (int)Math.Floor((float)opt.in_depth / this.group_size);
			//this.layer_type = "maxout";

			this.switches = new int[this.out_sx * this.out_sy * this.out_depth];//Util.zeros(this.out_sx*this.out_sy*this.out_depth); // useful for backprop
		}
		public override Vol forward(Vol V, bool is_training)
		{
			this.in_act = V;
			int N = this.out_depth;
			Vol V2 = new Vol(this.out_sx, this.out_sy, this.out_depth, 0.0f);

			// optimization branch. If we're operating on 1D arrays we dont have
			// to worry about keeping track of x,y,d coordinates inside
			// input volumes. In convnets we do :(
			if (this.out_sx == 1 && this.out_sy == 1)
			{
				for (int i = 0; i < N; i++)
				{
					int ix = i * this.group_size; // base index offset
					float a = V.w[ix];
					int ai = 0;
					for (int j = 1; j < this.group_size; j++)
					{
						float a2 = V.w[ix + j];
						if (a2 > a)
						{
							a = a2;
							ai = j;
						}
					}
					V2.w[i] = a;
					this.switches[i] = ix + ai;
				}
			}
			else
			{
				int n = 0; // counter for switches
				for (int x = 0; x < V.sx; x++)
				{
					for (int y = 0; y < V.sy; y++)
					{
						for (int i = 0; i < N; i++)
						{
							int ix = i * this.group_size;
							float a = V.get(x, y, ix);
							int ai = 0;
							for (int j = 1; j < this.group_size; j++)
							{
								float a2 = V.get(x, y, ix + j);
								if (a2 > a)
								{
									a = a2;
									ai = j;
								}
							}
							V2.set(x, y, i, a);
							this.switches[n] = ix + ai;
							n++;
						}
					}
				}

			}
			this.out_act = V2;
			return this.out_act;
		}
		public override float backward(DataSet del)
		{
			Vol V = this.in_act; // we need to set dw of this
			Vol V2 = this.out_act;
			int N = this.out_depth;
			V.dw = Util.zeros(V.w.Length); // zero out gradient wrt data

			// pass the gradient through the appropriate switch
			if (this.out_sx == 1 && this.out_sy == 1)
			{
				for (int i = 0; i < N; i++)
				{
					float chain_grad = V2.dw[i];
					V.dw[this.switches[i]] = chain_grad;
				}
			}
			else
			{
				// bleh okay, lets do this the hard way
				int n = 0; // counter for switches
				for (int x = 0; x < V2.sx; x++)
				{
					for (int y = 0; y < V2.sy; y++)
					{
						for (int i = 0; i < N; i++)
						{
							float chain_grad = V2.get_grad(x, y, i);
							V.set_grad(x, y, this.switches[n], chain_grad);
							n++;
						}
					}
				}
			}
			return 0;
		}
		public override List<ParamsAndGrads> getParamsAndGrads()
		{
			return new List<ParamsAndGrads>();
		}
		//toJSON: function() {
		//  json.out_depth = this.out_depth;
		//  json.out_sx = this.out_sx;
		//  json.out_sy = this.out_sy;
		//  json.layer_type = this.layer_type;
		//  json.group_size = this.group_size;
		//  return json;
		//},
		//fromJSON: function(json) {
		//  this.out_depth = json.out_depth;
		//  this.out_sx = json.out_sx;
		//  this.out_sy = json.out_sy;
		//  this.layer_type = json.layer_type; 
		//  this.group_size = json.group_size;
		//  this.switches = global.zeros(this.group_size);
		//}
	}*/
	//SigmoidLayer
	/*
	class SigmoidLayer : Layer
	{
		public override void getRange(Range r)
		{
		}
		public override void save(BinaryWriter s)
		{
		}
		public override void save(StreamWriter s)
		{
		}
		public override void load(BinaryReader s)
		{
		}
		public override void load(StreamReader s)
		{
		}
		// Implements Sigmoid nnonlinearity elementwise
		// x -> 1/(1+e^(-x))
		// so the output is between 0 and 1.
		public SigmoidLayer(Def def)
			: base(def)
		{
			Def opt = def;

			// computed
			this.out_sx = opt.in_sx;
			this.out_sy = opt.in_sy;
			this.out_depth = opt.in_depth;
			//this.layer_type = "sigmoid";
		}
		public override Vol forward(Vol V, bool is_training)
		{
			this.in_act = V;
			Vol V2 = V.cloneAndZero();
			int N = V.w.Length;
			float[] V2w = V2.w;
			float[] Vw = V.w;
			for (int i = 0; i < N; i++)
			{
				V2w[i] = 1.0f / (1.0f + (float)Math.Exp(-Vw[i]));
			}
			this.out_act = V2;
			return this.out_act;
		}
		public override float backward(DataSet y)
		{
			Vol V = this.in_act; // we need to set dw of this
			Vol V2 = this.out_act;
			int N = V.w.Length;
			V.dw = Util.zeros(N); // zero out gradient wrt data
			for (int i = 0; i < N; i++)
			{
				float v2wi = V2.w[i];
				V.dw[i] = v2wi * (1.0f - v2wi) * V2.dw[i];
			}
			return 0;
		}
		public override List<ParamsAndGrads> getParamsAndGrads()
		{
			return new List<ParamsAndGrads>();
		}
		//toJSON: function() {
		//  json.out_depth = this.out_depth;
		//  json.out_sx = this.out_sx;
		//  json.out_sy = this.out_sy;
		//  json.layer_type = this.layer_type;
		//  return json;
		//},
		//fromJSON: function(json) {
		//  this.out_depth = json.out_depth;
		//  this.out_sx = json.out_sx;
		//  this.out_sy = json.out_sy;
		//  this.layer_type = json.layer_type; 
		//}
	}*/
	//SVMLayer
	/*
	class SVMLayer : Layer
	{
		public override void getRange(Range r)
		{
		}
		public override void save(BinaryWriter s)
		{
		}
		public override void save(StreamWriter s)
		{
		}
		public override void load(BinaryReader s)
		{
		}
		public override void load(StreamReader s)
		{
		}
		public int num_inputs;

		public SVMLayer(Def def)
			: base(def)
		{
			Def opt = def;

			// computed
			this.num_inputs = opt.in_sx * opt.in_sy * opt.in_depth;
			this.out_depth = this.num_inputs;
			this.out_sx = 1;
			this.out_sy = 1;
			//this.layer_type = "svm";
		}

		public override Vol forward(Vol V, bool is_training)
		{
			this.in_act = V;
			this.out_act = V; // nothing to do, output raw scores
			return V;
		}
		public override float backward(DataSet y)
		{

			// compute and accumulate gradient wrt weights and bias of this layer
			Vol x = this.in_act;
			x.dw = Util.zeros(x.w.Length); // zero out the gradient of input Vol

			float yscore = x.w[y.predict]; // score of ground truth
			float margin = 1.0f;
			float loss = 0.0f;
			for (int i = 0; i < this.out_depth; i++)
			{
				if (-yscore + x.w[i] + margin > 0)
				{
					// violating example, apply loss
					// I love hinge loss, by the way. Truly.
					// Seriously, compare this SVM code with Softmax forward AND backprop code above
					// it's clear which one is superior, not only in code, simplicity
					// and beauty, but also in practice.
					x.dw[i] += 1;
					x.dw[y.predict] -= 1;
					loss += -yscore + x.w[i] + margin;
				}
			}

			return loss;
		}
		public override List<ParamsAndGrads> getParamsAndGrads()
		{
			return new List<ParamsAndGrads>();
		}
		//toJSON: function() {
		//  json.out_depth = this.out_depth;
		//  json.out_sx = this.out_sx;
		//  json.out_sy = this.out_sy;
		//  json.layer_type = this.layer_type;
		//  json.num_inputs = this.num_inputs;
		//  return json;
		//},
		//fromJSON: function(json) {
		//  this.out_depth = json.out_depth;
		//  this.out_sx = json.out_sx;
		//  this.out_sy = json.out_sy;
		//  this.layer_type = json.layer_type;
		//  this.num_inputs = json.num_inputs;
		//}
	}*/

}
