using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Runtime.InteropServices;

namespace ConvNet
{

	public abstract class Layer : Persistence
	{
		public int out_sx;
		public int out_sy;
		public int out_depth;
		//public string layer_type;
		public Vol in_act;
		public Vol out_act;
		public float l1_decay_mul;
		public float l2_decay_mul;
		public Def def;
		public Layer(Def def) { this.def = def; }
		public abstract Vol forward(Vol V, bool is_training);
		public abstract float backward(DataSet y);
		public abstract List<ParamsAndGrads> getParamsAndGrads();
		public abstract void getRange(Range r);
		public abstract void save(BinaryWriter s);
		public abstract void save(StreamWriter s);
		public abstract void load(BinaryReader s);
		public abstract void load(StreamReader s);
	}



	class FullyConnLayer : Layer
	{
		Vol[] filters;
		MyFloat[] filters_gsum; //[]?
		MyFloat[] filters_xsum; //[]?

		Vol bias;
		MyFloat bias_gsum; //[]?
		MyFloat bias_xsum; //[]?

		public int num_inputs;

		public FullyConnLayer(Def def)
			: base(def)
		{
			Def opt = def;

			// required
			// ok fine we will allow 'filters' as the word as well
			this.out_depth = opt.num_neurons != 0 ? opt.num_neurons : opt.filters;

			// optional 
			this.l1_decay_mul = opt.l1_decay_mul;
			this.l2_decay_mul = opt.l2_decay_mul;

			// computed
			this.num_inputs = opt.in_sx * opt.in_sy * opt.in_depth;
			this.out_sx = 1;
			this.out_sy = 1;

			// initializations
			filters = new Vol[out_depth];
			filters_gsum = new MyFloat[out_depth];
			filters_xsum = new MyFloat[out_depth];
			for (int i = 0; i < this.out_depth; i++)
			{
				filters[i] = new Vol(1, 1, this.num_inputs, null);
				filters_gsum[i] = MyFloat.getArray(num_inputs);
				filters_xsum[i] = MyFloat.getArray(num_inputs);
				for (int j = 0; j < num_inputs; j++)
				{
					filters_gsum[i][j] = 0.0f;
					filters_xsum[i][j] = 0.0f;
				}
			}

			this.bias = new Vol(1, 1, this.out_depth, opt.bias_pref);
			this.bias_gsum = MyFloat.getArray(out_depth); // last iteration gradients (used for momentum calculations)
			this.bias_xsum = MyFloat.getArray(out_depth); // used in adadelta

			for (int j = 0; j < num_inputs; j++)
			{
				bias_gsum[j] = 0.0f;
				bias_xsum[j] = 0.0f;
			}

			this.out_act = new Vol(1, 1, this.out_depth, 0.0f);
		}

		//[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		//private static extern void fwd(int out_depth, int num_inputs, float[] A_w, float[] V_w, float[] filters_w, float[] biases_w);
		//public override Vol forward(Vol V, bool is_training)
		//{
		//	this.in_act = V;
		//	Vol A = new Vol(1, 1, this.out_depth, 0.0f);
		//
		//	fwd(this.out_depth, this.num_inputs, A.w, V.w, this.filters_w, this.biases_w);
		//
		//	this.out_act = A;
		//	return this.out_act;
		//}
		public override Vol forward(Vol V, bool is_training)
		{
			this.in_act = V;

			for (int i = 0; i < this.out_depth; i++)
			{
				float a = 0.0f;
				MyFloat wi = this.filters[i].w;
				//int iw = i * this.num_inputs;
				for (int d = 0; d < this.num_inputs; d++)
				{
					a += V.w[d] * wi[d]; // for efficiency use Vols directly for now
				}
				//a += this.biases.w[i];
				a += this.bias.w[i];
				this.out_act.w[i] = a;
			}

			return this.out_act;
		}
		public override float backward(DataSet y)
		{
			Vol V = this.in_act;
			//V.dw = Util.zeros(V.w.Length); // zero out the gradient in input Vol
			for (int d = 0; d < V.w.size; d++)
			{
				V.dw[d] = 0;
			}

			// compute gradient wrt weights and data
			for (int i = 0; i < this.out_depth; i++)
			{
				//Vol tfi = this.filters[i];
				//int iw = i * this.num_inputs;
				MyFloat wi = this.filters[i].w;
				MyFloat dwi = this.filters[i].dw;
				float chain_grad = this.out_act.dw[i];
				for (int d = 0; d < this.num_inputs; d++)
				{
					V.dw[d] += wi[d] * chain_grad; // grad wrt input data
					dwi[d] += V.w[d] * chain_grad; // grad wrt params
				}
				this.bias.dw[i] += chain_grad;
			}
			return 0;
		}
		public override List<ParamsAndGrads> getParamsAndGrads()
		{
			List<ParamsAndGrads> response = new List<ParamsAndGrads>();
			for (int i = 0; i < this.out_depth; i++)
			{
				//int iw = i * this.num_inputs;
				//response.Add(new ParamsAndGrads() { Params = this.filters[i].w, grads = this.filters[i].dw, l1_decay_mul = this.l1_decay_mul, l2_decay_mul = this.l2_decay_mul });
				response.Add(new ParamsAndGrads()
				{
					params_ = this.filters[i].w,
					grads_ = this.filters[i].dw,
					params_size = this.num_inputs,
					gsum = filters_gsum[i],
					xsum = filters_xsum[i],
					l1_decay_mul = this.l1_decay_mul,
					l2_decay_mul = this.l2_decay_mul
				});
			}
			//response.Add(new ParamsAndGrads() { Params = this.biases.w, grads = this.biases.dw, l1_decay_mul = 0.0f, l2_decay_mul = 0.0f });
			response.Add(new ParamsAndGrads()
			{
				params_ = this.bias.w,
				grads_ = this.bias.dw,
				params_size = this.out_depth,
				gsum = bias_gsum,
				xsum = bias_xsum,
				l1_decay_mul = 0.0f,
				l2_decay_mul = 0.0f
			});
			return response;
		}

		public override void getRange(Range r)
		{
			//biases.getRange(r);
			//for (int i = 0; i < filters.Count; i++)
			//{
			//	filters[i].getRange(r);
			//}
		}
		public override void save(BinaryWriter s)
		{
			//biases.save(s);
			//for (int i = 0; i < filters.Count; i++)
			//{
			//	filters[i].save(s);
			//}
		}
		public override void save(StreamWriter s)
		{
			//biases.save(s);
			//for (int i = 0; i < filters.Count; i++)
			//{
			//	filters[i].save(s);
			//}
		}
		public override void load(BinaryReader s)
		{
			//biases.load(s);
			//for (int i = 0; i < filters.Count; i++)
			//{
			//	filters[i].load(s);
			//}
		}
		public override void load(StreamReader s)
		{
			//biases.load(s);
			//for (int i = 0; i < filters.Count; i++)
			//{
			//	filters[i].load(s);
			//}
		}
	}
	/*
	class TanhLayer : Layer
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
		// Implements Tanh nnonlinearity elementwise
		// x -> tanh(x) 
		// so the output is between -1 and 1.
		public TanhLayer(Def def)
			: base(def)
		{
			Def opt = def;
			// computed
			this.out_sx = opt.in_sx;
			this.out_sy = opt.in_sy;
			this.out_depth = opt.in_depth;
			//this.layer_type = "tanh";
		}

		public float tanh(float x)
		{
			float y = (float)Math.Exp(2 * x);
			return (y - 1) / (y + 1);
		}
		public override Vol forward(Vol V, bool is_training)
		{
			this.in_act = V;
			Vol V2 = V.cloneAndZero();
			int N = V.w.Length;
			for (int i = 0; i < N; i++)
			{
				V2.w[i] = tanh(V.w[i]);
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
				V.dw[i] = (1.0f - v2wi * v2wi) * V2.dw[i];
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
		//}
		//fromJSON: function(json) {
		//  this.out_depth = json.out_depth;
		//  this.out_sx = json.out_sx;
		//  this.out_sy = json.out_sy;
		//  this.layer_type = json.layer_type; 
		//}
	}*/
	class ReluLayer : Layer
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
		// Implements ReLU nonlinearity elementwise
		// x -> max(0, x)
		// the output is in [0, inf)
		public ReluLayer(Def def)
			: base(def)
		{
			Def opt = def;

			// computed
			this.out_sx = opt.in_sx;
			this.out_sy = opt.in_sy;
			this.out_depth = opt.in_depth;
			//this.layer_type = "relu";

			this.out_act = new Vol(out_sx, out_sy, out_depth, null);
		}

		public override Vol forward(Vol V, bool is_training)
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
		public override float backward(DataSet y)
		{
			Vol V = this.in_act; // we need to set dw of this
			Vol V2 = this.out_act;
			int N = V.w.size;
			//V.dw = Util.zeros(N); // zero out gradient wrt data
			for (int i = 0; i < N; i++)
			{
				V.dw[i] = 0;
				if (V2.w[i] <= 0) V.dw[i] = 0; // threshold
				else V.dw[i] = V2.dw[i];
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
	}
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

	class PoolLayer : Layer
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
		public int sx;
		public int sy;
		public int in_depth;
		public int in_sx;
		public int in_sy;
		public int stride;
		public int pad;
		public int[] switchx;
		public int[] switchy;

		public PoolLayer(Def def)
			: base(def)
		{
			Def opt = def;

			// required
			this.sx = opt.sx; // filter size
			this.in_depth = opt.in_depth;
			this.in_sx = opt.in_sx;
			this.in_sy = opt.in_sy;

			// optional
			this.sy = opt.sy != 0 ? opt.sy : this.sx;
			this.stride = opt.stride != 0 ? opt.stride : 2;
			this.pad = opt.pad != 0 ? opt.pad : 0; // amount of 0 padding to add around borders of input volume

			// computed
			this.out_depth = this.in_depth;
			this.out_sx = (int)Math.Floor((float)((this.in_sx + this.pad * 2 - this.sx) / this.stride + 1));
			this.out_sy = (int)Math.Floor((float)((this.in_sy + this.pad * 2 - this.sy) / this.stride + 1));
			//this.layer_type = "pool";
			// store switches for x,y coordinates for where the max comes from, for each output neuron
			this.switchx = new int[this.out_sx * this.out_sy * this.out_depth];//Util.zeros(this.out_sx * this.out_sy * this.out_depth);
			this.switchy = new int[this.out_sx * this.out_sy * this.out_depth];//Util.zeros(this.out_sx * this.out_sy * this.out_depth);

			this.out_act = new Vol(this.out_sx, this.out_sy, this.out_depth, 0.0f);
		}

		public override Vol forward(Vol V, bool is_training)
		{
			this.in_act = V;

			Vol A = this.out_act;//new Vol(this.out_sx, this.out_sy, this.out_depth, 0.0f);

			int n = 0; // a counter for switches
			for (int d = 0; d < this.out_depth; d++)
			{
				int x = -this.pad;
				int y = -this.pad;
				for (int ax = 0; ax < this.out_sx; x += this.stride, ax++)
				{
					y = -this.pad;
					for (int ay = 0; ay < this.out_sy; y += this.stride, ay++)
					{

						// convolve centered at this particular location
						float a = -99999; // hopefully small enough ;\
						int winx = -1;
						int winy = -1;
						for (int fx = 0; fx < this.sx; fx++)
						{
							for (int fy = 0; fy < this.sy; fy++)
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
						A.set(ax, ay, d, a);
					}
				}
			}
			//this.out_act = A;
			return this.out_act;
		}
		public override float backward(DataSet del)
		{
			// pooling layers have no parameters, so simply compute 
			// gradient wrt data here
			Vol V = this.in_act;
			//????????TODO 未初始化可能影响后续????????????????V.dw = Util.zeros(V.w.Length); // zero out gradient wrt data
			for (int i = 0; i < V.dw.size; i++)
			{
				V.dw[i] = 0;
			}

			Vol A = this.out_act; // computed in forward pass 

			int n = 0;
			for (int d = 0; d < this.out_depth; d++)
			{
				int x = -this.pad;
				int y = -this.pad;
				for (int ax = 0; ax < this.out_sx; x += this.stride, ax++)
				{
					y = -this.pad;
					for (int ay = 0; ay < this.out_sy; y += this.stride, ay++)
					{

						float chain_grad = this.out_act.get_grad(ax, ay, d);
						V.add_grad(this.switchx[n], this.switchy[n], d, chain_grad);
						n++;

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
		//  json.sx = this.sx;
		//  json.sy = this.sy;
		//  json.stride = this.stride;
		//  json.in_depth = this.in_depth;
		//  json.out_depth = this.out_depth;
		//  json.out_sx = this.out_sx;
		//  json.out_sy = this.out_sy;
		//  json.layer_type = this.layer_type;
		//  json.pad = this.pad;
		//  return json;
		//},
		//fromJSON: function(json) {
		//  this.out_depth = json.out_depth;
		//  this.out_sx = json.out_sx;
		//  this.out_sy = json.out_sy;
		//  this.layer_type = json.layer_type;
		//  this.sx = json.sx;
		//  this.sy = json.sy;
		//  this.stride = json.stride;
		//  this.in_depth = json.in_depth;
		//  this.pad = typeof json.pad !== 'undefined' ? json.pad : 0; // backwards compatibility
		//  this.switchx = global.zeros(this.out_sx*this.out_sy*this.out_depth); // need to re-init these appropriately
		//  this.switchy = global.zeros(this.out_sx*this.out_sy*this.out_depth);
		//}
	}

	class SoftmaxLayer : Layer
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
		// Layers that implement a loss. Currently these are the layers that 
		// can initiate a backward() pass. In future we probably want a more 
		// flexible system that can accomodate multiple losses to do multi-task
		// learning, and stuff like that. But for now, one of the layers in this
		// file must be the final layer in a Net.

		// This is a classifier, with N discrete classes from 0 to N-1
		// it gets a stream of N incoming numbers and computes the softmax
		// function (exponentiate and normalize to sum to 1 as probabilities should)
		public int num_inputs;
		public MyFloat es;
		public SoftmaxLayer(Def def)
			: base(def)
		{
			Def opt = def;

			// computed
			this.num_inputs = opt.in_sx * opt.in_sy * opt.in_depth;
			this.out_depth = this.num_inputs;
			this.out_sx = 1;
			this.out_sy = 1;
			//this.layer_type = "softmax";
			es = MyFloat.getArray(this.out_depth);

			this.out_act = new Vol(1, 1, this.out_depth, 0.0f);
		}

		public override Vol forward(Vol V, bool is_training)
		{
			this.in_act = V;

			Vol A = this.out_act; //new Vol(1, 1, this.out_depth, 0.0f);

			// compute max activation
			MyFloat As = V.w;
			float amax = V.w[0];
			for (int i = 1; i < this.out_depth; i++)
			{
				if (As[i] > amax) amax = As[i];
			}

			// compute exponentials (carefully to not blow up)

			float esum = 0.0f;
			for (int i = 0; i < this.out_depth; i++)
			{
				float e = (float)Math.Exp(As[i] - amax);
				esum += e;
				es[i] = e;
			}

			// normalize and output to sum to one
			for (int i = 0; i < this.out_depth; i++)
			{
				es[i] /= esum;
				A.w[i] = es[i];
			}

			//this.es = es; // save these for backprop
			//this.out_act = A;
			return this.out_act;
		}
		public override float backward(DataSet y)
		{

			// compute and accumulate gradient wrt weights and bias of this layer
			Vol x = this.in_act;
			//????????TODO 未初始化可能影响后续????????????????x.dw = Util.zeros(x.w.Length); // zero out the gradient of input Vol
			for (int i = 0; i < x.dw.size; i++)
			{
				x.dw[i] = 0;
			}

			for (int i = 0; i < this.out_depth; i++)
			{
				float indicator = (i == y.predict) ? 1.0f : 0.0f;
				float mul = -(indicator - this.es[i]);
				x.dw[i] = mul;
			}

			// loss is the class negative log likelihood
			return -(float)Math.Log(this.es[y.predict]);
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
	}
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

	class RegressionLayer : Layer
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
		// implements an L2 regression cost layer,
		// so penalizes \sum_i(||x_i - y_i||^2), where x is its input
		// and y is the user-provided array of "correct" values.
		int num_inputs;
		public RegressionLayer(Def def)
			: base(def)
		{
			Def opt = def;

			// computed
			this.num_inputs = opt.in_sx * opt.in_sy * opt.in_depth;
			this.out_depth = this.num_inputs;
			this.out_sx = 1;
			this.out_sy = 1;
			//this.layer_type = "regression";
		}

		public override Vol forward(Vol V, bool is_training)
		{
			this.in_act = V;
			this.out_act = V;
			return V; // identity function
		}
		// y is a list here of size num_inputs
		public override float backward(DataSet y)
		{

			// compute and accumulate gradient wrt weights and bias of this layer
			Vol x = this.in_act;
			//x.dw = Util.zeros(x.w.Length); // zero out the gradient of input Vol
			float loss = 0.0f;
			//if (y != null)
			//{
			for (int i = 0; i < this.out_depth; i++)
			{
				float dy = x.w[i] - y.data[i];
				x.dw[i] = dy;
				loss += 2 * dy * dy;
			}
			//}
			//else
			//{
			//	// assume it is a struct with entries .dim and .val
			//	// and we pass gradient only along dimension dim to be equal to val
			//	int i = y.dim;
			//	v_ar yi = y.val;
			//	v_ar dy = x.w[i] - yi;
			//	x.dw[i] = dy;
			//	loss += 2 * dy * dy;
			//}
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
	}

	class ConvLayer : Layer
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
		// This file contains all layers that do dot products with input,
		// but usually in a different connectivity pattern and weight sharing
		// schemes: 
		// - FullyConn is fully connected dot products 
		// - ConvLayer does convolutions (so weight sharing spatially)
		// putting them together in one file because they are very similar
		int sx;
		int sy;
		int in_depth;
		int in_sx;
		int in_sy;
		int stride;
		int pad;
		Vol[] filters;
		MyFloat[] filters_gsum; //[]?
		MyFloat[] filters_xsum; //[]?
		Vol biases;
		MyFloat bias_gsum; //[]?
		MyFloat bias_xsum; //[]?
		public ConvLayer(Def def)
			: base(def)
		{

			Def opt = def;

			// required
			this.out_depth = opt.filters;
			this.sx = opt.sx; // filter size. Should be odd if possible, it's cleaner.
			this.sy = opt.sy != 0 ? opt.sy : this.sx;
			this.in_depth = opt.in_depth;
			this.in_sx = opt.in_sx;
			this.in_sy = opt.in_sy;

			// optional
			this.stride = opt.stride != 0 ? opt.stride : 1; // stride at which we apply filters to input volume
			this.pad = opt.pad; // amount of 0 padding to add around borders of input volume
			this.l1_decay_mul = opt.l1_decay_mul;
			this.l2_decay_mul = opt.l2_decay_mul;

			// computed
			// note we are doing floor, so if the strided convolution of the filter doesnt fit into the input
			// volume exactly, the output volume will be trimmed and not contain the (incomplete) computed
			// final application.
			this.out_sx = (int)(Math.Floor((float)((this.in_sx + this.pad * 2 - this.sx) / this.stride + 1)));
			this.out_sy = (int)(Math.Floor((float)((this.in_sy + this.pad * 2 - this.sy) / this.stride + 1)));
			//this.layer_type = "conv";

			// initializations
			float bias = opt.bias_pref;
			this.filters = new Vol[out_depth];
			filters_gsum = new MyFloat[out_depth];
			filters_xsum = new MyFloat[out_depth];
			for (int i = 0; i < this.out_depth; i++)
			{
				this.filters[i] = new Vol(this.sx, this.sy, this.in_depth, null);
				filters_gsum[i] = MyFloat.getArray(sx * sy * in_depth);
				filters_xsum[i] = MyFloat.getArray(sx * sy * in_depth);
				for (int j = 0; j < sx * sy * in_depth; j++)
				{
					filters_gsum[i][j] = 0.0f;
					filters_xsum[i][j] = 0.0f;
				}
			}
			this.biases = new Vol(1, 1, this.out_depth, bias);
			this.bias_gsum = MyFloat.getArray(out_depth); // last iteration gradients (used for momentum calculations)
			this.bias_xsum = MyFloat.getArray(out_depth); // used in adadelta

			this.out_act = new Vol(this.out_sx, this.out_sy, this.out_depth, 0.0f);
		}

		public override Vol forward(Vol V, bool is_training)
		{
			// optimized code by @mdda that achieves 2x speedup over previous version

			this.in_act = V;
			Vol A = this.out_act;//new Vol(this.out_sx, this.out_sy, this.out_depth, 0.0f);

			int V_sx = V.sx;
			int V_sy = V.sy;
			int xy_stride = this.stride;

			for (int d = 0; d < this.out_depth; d++)
			{
				Vol f = this.filters[d];
				int x = -this.pad;
				int y = -this.pad;
				for (int ay = 0; ay < this.out_sy; y += xy_stride, ay++)
				{  // xy_stride
					x = -this.pad | 0;
					for (int ax = 0; ax < this.out_sx; x += xy_stride, ax++)
					{  // xy_stride

						// convolve centered at this particular location
						float a = 0.0f;
						for (int fy = 0; fy < f.sy; fy++)
						{
							int oy = y + fy; // coordinates in the original input array coordinates
							for (int fx = 0; fx < f.sx; fx++)
							{
								int ox = x + fx;
								if (oy >= 0 && oy < V_sy && ox >= 0 && ox < V_sx)
								{
									for (int fd = 0; fd < f.depth; fd++)
									{
										// avoid function call overhead (x2) for efficiency, compromise modularity :(
										a += f.w[((f.sx * fy) + fx) * f.depth + fd] * V.w[((V_sx * oy) + ox) * V.depth + fd];
									}
								}
							}
						}
						a += this.biases.w[d];
						A.set(ax, ay, d, a);
					}
				}
			}
			//this.out_act = A;
			return this.out_act;
		}
		public override float backward(DataSet del)
		{
			Vol V = this.in_act;
			//????????TODO 未初始化可能影响后续????????????????V.dw = Util.zeros(V.w.Length); // zero out gradient wrt bottom data, we're about to fill it
			for (int i = 0; i < V.dw.size; i++)
			{
				V.dw[i] = 0;
			}
			int V_sx = V.sx;
			int V_sy = V.sy;
			int xy_stride = this.stride;

			for (int d = 0; d < this.out_depth; d++)
			{
				Vol f = this.filters[d];
				int x = -this.pad;
				int y = -this.pad;
				for (int ay = 0; ay < this.out_sy; y += xy_stride, ay++)
				{  // xy_stride
					x = -this.pad;
					for (int ax = 0; ax < this.out_sx; x += xy_stride, ax++)
					{  // xy_stride

						// convolve centered at this particular location
						float chain_grad = this.out_act.get_grad(ax, ay, d); // gradient from above, from chain rule
						for (int fy = 0; fy < f.sy; fy++)
						{
							int oy = y + fy; // coordinates in the original input array coordinates
							for (int fx = 0; fx < f.sx; fx++)
							{
								int ox = x + fx;
								if (oy >= 0 && oy < V_sy && ox >= 0 && ox < V_sx)
								{
									for (int fd = 0; fd < f.depth; fd++)
									{
										// avoid function call overhead (x2) for efficiency, compromise modularity :(
										int ix1 = ((V_sx * oy) + ox) * V.depth + fd;
										int ix2 = ((f.sx * fy) + fx) * f.depth + fd;
										f.dw[ix2] += V.w[ix1] * chain_grad;
										V.dw[ix1] += f.w[ix2] * chain_grad;
									}
								}
							}
						}
						this.biases.dw[d] += chain_grad;
					}
				}
			}
			return 0;
		}
		public override List<ParamsAndGrads> getParamsAndGrads()
		{
			List<ParamsAndGrads> response = new List<ParamsAndGrads>();
			for (int i = 0; i < this.out_depth; i++)
			{
				response.Add(new ParamsAndGrads()
				{
					params_ = this.filters[i].w,
					grads_ = this.filters[i].dw,
					params_size = sx * sy * in_depth,
					gsum = filters_gsum[i],
					xsum = filters_xsum[i],
					l2_decay_mul = this.l2_decay_mul,
					l1_decay_mul = this.l1_decay_mul
				});
			}
			response.Add(new ParamsAndGrads()
			{
				params_ = this.biases.w,
				grads_ = this.biases.dw,
				params_size = this.out_depth,
				gsum = bias_gsum,
				xsum = bias_xsum,
				l1_decay_mul = 0.0f,
				l2_decay_mul = 0.0f
			});
			return response;
		}
	}



	class InputLayer : Layer
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
		public InputLayer(Def def)
			: base(def)
		{
			Def opt = def;

			// this is a bit silly but lets allow people to specify either ins or outs
			this.out_sx = opt.out_sx != 0 ? opt.out_sx : opt.in_sx;
			this.out_sy = opt.out_sy != 0 ? opt.out_sy : opt.in_sy;
			this.out_depth = opt.out_depth != 0 ? opt.out_depth : opt.in_depth;
			//this.layer_type = "input";
		}
		public override Vol forward(Vol V, bool is_training)
		{
			this.in_act = V;
			this.out_act = V;
			return this.out_act; // dummy identity function for now
		}

		public override float backward(DataSet y)
		{
			return 0;
		}
		public override List<ParamsAndGrads> getParamsAndGrads()
		{
			return new List<ParamsAndGrads>();
		}
	}
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
}
