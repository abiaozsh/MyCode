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
	//in_depth % 8 == 0)  有问题 ok

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
		public float l1_decay_mul;
		public float l2_decay_mul;
		public abstract bool inited();
		public abstract void init();
		public abstract Vol forward(Instance instance, Vol V);
		public abstract void backward(Instance instance);

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
			public float loss;
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
			public MyFloat filters_dw;
			public MyFloat bias_dw;
			public Instance actIns;
		}

		public abstract void train(TrainableInstance instance, Trainer trainer, float oneBatchSize);
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
			//float[] V2w = V2.w;
			for (int i = 0; i < N; i++)
			{
				if (V.w[i] < 0)
				{
					instance.out_act.w[i] = 0; // threshold at 0
				}
				else
				{
					instance.out_act.w[i] = V.w[i];
				}
			}
			//this.out_act = V2;
			return instance.out_act;
		}
		public override void backward(Instance instance)
		{
			//Vol V = this.in_act; // we need to set dw of this
			//Vol V2 = this.out_act;
			int N = instance.in_act.w.size;
			//V.dw = Util.zeros(N); // zero out gradient wrt data
			for (int i = 0; i < N; i++)
			{
				//in_act.dw[i] = 0;
				if (instance.out_act.w[i] <= 0)
				{
					instance.in_act.dw[i] = 0; // threshold
				}
				else
				{
					instance.in_act.dw[i] = instance.out_act.dw[i];
				}
			}
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
		public override void backward(Instance instance)
		{
			//V.dw = Util.zeros(N); // zero out gradient wrt data
			for (int i = 0; i < in_size; i++)
			{
				float v2wi = instance.out_act.w[i];
				instance.in_act.dw[i] = (1.0f - v2wi * v2wi) * instance.out_act.dw[i];
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

		public class PoolInstance : Instance
		{
			public int[] switchx;
			public int[] switchy;
		}

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
			PoolInstance ins = new PoolInstance();
			ins.out_act = new Vol(out_sx, out_sy, out_depth, 0.0f);
			ins.switchx = new int[out_sx * out_sy * out_depth];//Util.zeros(this.out_sx * this.out_sy * this.out_depth);
			ins.switchy = new int[out_sx * out_sy * out_depth];//Util.zeros(this.out_sx * this.out_sy * this.out_depth);
			return ins;
		}

		public override Vol forward(Instance instance, Vol V)
		{
			PoolInstance poolInstance = (PoolInstance)instance;
			instance.in_act = V;
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
						float a = instance.in_act.get(x, y, d); // hopefully small enough ;\
						int winx = 0;
						int winy = 0;
						for (int fx = 0; fx < stride; fx++)
						{
							for (int fy = 0; fy < stride; fy++)
							{
								int oy = y + fy;
								int ox = x + fx;
								if (oy >= 0 && oy < instance.in_act.sy && ox >= 0 && ox < instance.in_act.sx)
								{
									float v = instance.in_act.get(ox, oy, d);
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
						poolInstance.switchx[n] = winx;
						poolInstance.switchy[n] = winy;
						n++;
						instance.out_act.set(ax, ay, d, a);
					}
				}
			};
			return instance.out_act;
		}
		public override void backward(Instance instance)
		{
			PoolInstance poolInstance = (PoolInstance)instance;
			// pooling layers have no parameters, so simply compute 
			// gradient wrt data here
			for (int i = 0; i < instance.in_act.dw.size; i++)
			{
				instance.in_act.dw[i] = 0;
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
						float chain_grad = instance.out_act.get_grad(ax, ay, d);
						instance.in_act.add_grad(poolInstance.switchx[n], poolInstance.switchy[n], d, chain_grad);
						n++;
					}
				}
			};
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

		public override Vol forward(Instance instance, Vol V)
		{
			instance.in_act = V;

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
				instance.out_act.w[i] = e;
			}

			// normalize and output to sum to one
			for (int i = 0; i < this.out_depth; i++)
			{
				instance.out_act.w[i] /= esum;
				//out_act.w[i] = es[i];
			}

			//this.es = es; // save these for backprop
			//this.out_act = A;
			return instance.out_act;
		}
		public override void backward(Instance instance)
		{
			LastInstance ins = (LastInstance)instance;
			// compute and accumulate gradient wrt weights and bias of this layer
			Vol x = instance.in_act;

			for (int i = 0; i < x.dw.size; i++)
			{
				x.dw[i] = 0;
			}

			for (int i = 0; i < this.out_depth; i++)
			{
				float indicator = (i == ins.y.predict) ? 1.0f : 0.0f;
				float mul = -(indicator - instance.out_act.w[i]);
				x.dw[i] = mul;
			}

			// loss is the class negative log likelihood
			ins.loss = -(float)Math.Log(instance.out_act.w[ins.y.predict]);
		}
	}
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

		public override Vol forward(Instance instance, Vol V)
		{
			instance.in_act = V;
			return V; // identity function
		}
		// y is a list here of size num_inputs
		public override void backward(Instance instance)
		{
			LastInstance ins = (LastInstance)instance;
			// compute and accumulate gradient wrt weights and bias of this layer
			float loss = 0.0f;
			for (int i = 0; i < this.out_depth; i++)
			{
				float dy = instance.in_act.w[i] - ins.y.data.w[i];
				instance.in_act.dw[i] = dy;
				loss += 2 * dy * dy;
			}
			ins.loss = loss;
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
		public override Instance getInstance()
		{
			return null;
		}

		public override Vol forward(Instance instance, Vol V)
		{
			//this.out_act = V;
			//return this.out_act; // dummy identity function for now
			return V;
		}

		public override void backward(Instance instance)
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
			for (int i = 0; i < N; i++)
			{
				instance.out_act.w[i] = instance.in_act.w[i];
			}
			return instance.out_act;
		}
		public override void backward(Instance instance)
		{
			Vol V = instance.in_act; // we need to set dw of this
			int N = V.w.size;
			for (int i = 0; i < N; i++)
			{
				instance.in_act.dw[i] = instance.out_act.dw[i];
			}
		}
	}




}
