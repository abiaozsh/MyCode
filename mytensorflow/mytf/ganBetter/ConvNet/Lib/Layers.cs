using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using System.Drawing;
using System.Diagnostics;

namespace ConvNet
{

	public abstract class Layer
	{
		public Layer in_layer;
		public Layer out_layer;

		protected bool _inited = false;
		public int out_sx;
		public int out_sy;
		public int out_depth;
		public abstract bool inited();
		public abstract void init();
		public abstract Vol forward(Instance ins, Vol V);

		public class Instance
		{
			public Vol in_act;
			public Vol out_act;
		}
		public abstract Instance getInstance();
	}

	public abstract class LastLayer : Layer
	{
		public class LastInstance : Instance
		{
			public void setData(DataSet y)
			{
				this.y = y;
			}
			public DataSet y;
		}

	}
	public abstract class ActivationLayer : Layer
	{
	}
	public abstract class TrainableLayer : Layer, Persistence
	{
		public abstract void save(TextWriter s);
		public abstract void load(TextReader s);

		public class TrainableInstance : Instance
		{
			public Instance actIns;
			public Instance poolIns;
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

			_inited = true;
		}
		public override Instance getInstance()
		{
			Instance ins = new Instance();
			ins.out_act = new Vol(out_sx, out_sy, out_depth, null);
			return ins;
		}

		public override Vol forward(Instance instance, Vol V)
		{
			instance.in_act = V;

			//Vol V2 = V.clone();

			int N = V.w.size;
			for (int i = 0; i < N; i++)
			{
				if (V.w[i] < 0)
				{
					instance.out_act.w[i] = V.w[i]*0.2f;
				}
				else
				{
					instance.out_act.w[i] = V.w[i];
				}
			}
			//this.out_act = V2;
			return instance.out_act;
		}
	}
	public class TanhLayer : ActivationLayer
	{
		int in_size;

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
			in_size = in_layer.out_sx * in_layer.out_sy * in_layer.out_depth;
			//float y = (float)Math.Exp(2 * x);
			//return (y - 1) / (y + 1);
			_inited = true;
		}
		public override Instance getInstance()
		{
			Instance ins = new Instance();
			ins.out_act = new Vol(out_sx, out_sy, out_depth, null);
			return ins;
		}

		public override Vol forward(Instance instance, Vol V)
		{
			instance.in_act = V;
			int N = V.w.size;
			for (int i = 0; i < N; i++)
			{
				instance.out_act.w[i] = (float)Math.Tanh(V.w[i]);
			}
			return instance.out_act;
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

			_inited = true;
		}
		public override Instance getInstance()
		{
			Instance ins = new Instance();
			ins.out_act = new Vol(out_sx, out_sy, out_depth, 0.0f);
			return ins;
		}

		public override Vol forward(Instance ins, Vol V)
		{
			ins.in_act = V;
			for (int d = 0; d < this.out_depth; d++)
			{
				int x = 0;
				int y = 0;
				for (int ax = 0; ax < out_sx; x += stride, ax++)
				{
					y = 0;
					for (int ay = 0; ay < out_sy; y += stride, ay++)
					{
						float a = ins.in_act.w[((in_sx * y) + x) * in_depth + d];
						for (int fx = 0; fx < stride; fx++)
						{
							for (int fy = 0; fy < stride; fy++)
							{
								int oy = y + fy;
								int ox = x + fx;
								{
									float v = ins.in_act.w[((in_sx * oy) + ox) * in_depth + d];
									if (v > a)
									{
										a = v;
									}
								}
							}
						}
						ins.out_act.set(ax, ay, d, a);
					}
				}
			};
			return ins.out_act;
		}
	}
	public class SoftmaxLayer : LastLayer
	{
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
			_inited = true;
		}
		public override Instance getInstance()
		{
			LastInstance ins = new LastInstance();
			ins.out_act = new Vol(1, 1, out_depth, 0.0f);
			return ins;
		}

		public override Vol forward(Instance ins, Vol V)
		{
			ins.in_act = V;

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
				ins.out_act.w[i] = e;
			}

			// normalize and output to sum to one
			for (int i = 0; i < this.out_depth; i++)
			{
				ins.out_act.w[i] /= esum;
				//out_act.w[i] = es[i];
			}

			//this.es = es; // save these for backprop
			//this.out_act = A;
			return ins.out_act;
		}
	}
	//mse = tf.reduce_sum(tf.square(y_ -  y))
	public class RegressionLayer : LastLayer
	{
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
		public override Instance getInstance()
		{
			LastInstance ins = new LastInstance();
			//out does not need init
			return ins;
		}

		public override Vol forward(Instance ins, Vol V)
		{
			ins.in_act = V;
			return V; // identity function
		}
		// y is a list here of size num_inputs
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
		public override Instance getInstance()
		{
			return null;
		}

		public override Vol forward(Instance ins, Vol V)
		{
			//this.out_act = V;
			//return this.out_act; // dummy identity function for now
			return V;
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
			_inited = true;
		}
		public override Instance getInstance()
		{
			Instance ins = new Instance();
			ins.out_act = new Vol(out_sx, out_sy, out_depth, 0.0f);
			return ins;
		}

		public override Vol forward(Instance instance, Vol V)
		{
			instance.in_act = V;
			int N = V.w.size;
			//for (int i = 0; i < N; i++)
			//{
			//	instance.out_act.w[i] = instance.in_act.w[i];
			//}
			instance.out_act.w = instance.in_act.w;
			return instance.out_act;
		}
	}


}
