using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

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

		public void save(TextWriter s)
		{
			w.save(s);
		}
		public void load(TextReader s)
		{
			w.load(s);
		}

		public Vol(float[] list)
		{
			this.sx = 1;
			this.sy = 1;
			this.depth = list.Length;

			w = MyFloat.getArray(this.depth);//new float[this.depth];
			dw = MyFloat.getArray(this.depth); //new float[this.depth];

			for (int i = 0; i < this.depth; i++)
			{
				w[i] = list[i];
			}
		}

		static bool return_v = false;
		static float v_val = 0.0f;
		static Random _rnd = new Random();
		public static float gaussRandom()
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
		public Vol cloneAndZero() { return new Vol(this.sx, this.sy, this.depth, 0.0f); }

		public Vol clone()
		{
			Vol V = new Vol(this.sx, this.sy, this.depth, 0.0f);
			int n = this.w.size;
			for (int i = 0; i < n; i++) { V.w[i] = this.w[i]; }
			return V;
		}
		public float get(int x, int y, int d)
		{
			int ix = ((this.sx * y) + x) * this.depth + d;
			return this.w[ix];
		}
		public void set(int x, int y, int d, float v)
		{
			int ix = ((this.sx * y) + x) * this.depth + d;
			this.w[ix] = v;
		}
		public void add_grad(int x, int y, int d, float v)
		{
			int ix = ((this.sx * y) + x) * this.depth + d;
			this.dw[ix] += v;
		}
		public float get_grad(int x, int y, int d)
		{
			int ix = ((this.sx * y) + x) * this.depth + d;
			return this.dw[ix];
		}
		public void set_grad(int x, int y, int d, float v)
		{
			int ix = ((this.sx * y) + x) * this.depth + d;
			this.dw[ix] = v;
		}

	}
	public class DataSet
	{
		//Classification (SVM/Softmax) and Regression (L2) cost functions

		//Regression (L2)
		//public int dim;
		//public float val;
		public float[] data;

		//Classification (SVM/Softmax)
		public int predict;
	}
	public class ParamsAndGrads
	{
		public int params_size;
		public MyFloat params_;
		public MyFloat grads_;
		//public int iw;
		public MyFloat gsum; //[]?
		public MyFloat xsum; //[]?
		public float l1_decay_mul = 1.0f;
		public float l2_decay_mul = 1.0f;
	}
	public class Def
	{
		public int in_sx;
		public int in_sy;
		public int in_depth;
		public int out_sx;
		public int out_sy;
		public int out_depth;
		public string type;
		//public string activation;
		public int num_neurons;
		public int num_classes;
		public float bias_pref;
		public bool tensor;
		public int group_size;
		public float? drop_prob;// = 0.5
		public int filters;

		public float l1_decay_mul = 0.0f;
		public float l2_decay_mul = 1.0f;

		public float k;
		public float n;
		public float alpha;
		public float beta;

		public int sx;
		public int sy;
		public int stride;
		public int pad;

	}

	public class Net
	{
		public List<Layer> layers;

		public void save(TextWriter s)
		{
			for (int i = 0; i < layers.Count; i++)
			{
				layers[i].save(s);
			}
		}
		public void load(TextReader s)
		{
			for (int i = 0; i < layers.Count; i++)
			{
				layers[i].load(s);
			}
		}


		public Net()
		{
			layers = new List<Layer>();
		}

		// desugar syntactic for adding activations and dropouts
		private List<Def> desugar(List<Def> defs)
		{
			List<Def> new_defs = new List<Def>();
			for (int i = 0; i < defs.Count; i++)
			{
				Def def = defs[i];
		
				//if (def.type == "softmax" || def.type == "svm")
				//{
				//	// add an fc layer here, there is no reason the user should
				//	// have to worry about this and we almost always want to
				//	new_defs.Add(new Def() { type = "fc", num_neurons = def.num_classes });
				//}
		
				//if (def.type == "regression")
				//{
				//	// add an fc layer here, there is no reason the user should
				//	// have to worry about this and we almost always want to
				//	new_defs.Add(new Def() { type = "fc", num_neurons = def.num_neurons });
				//}
		
				//if ((def.type == "fc" || def.type == "conv") && def.bias_pref == 0.0)
				//{
				//	//def.bias_pref = 0.0;
				//	if (def.activation == "relu")
				//	{
				//		def.bias_pref = 0.1; // relus like a bit of positive bias to get gradients early
				//		// otherwise it's technically possible that a relu unit will never turn on (by chance)
				//		// and will never get any gradient and never contribute any computation. Dead relu.
				//	}
				//}
		
				// if(def.tensor != null) {
				// apply quadratic transform so that the upcoming multiply will include
				// quadratic terms, equivalent to doing a tensor product
				if (def.tensor)
				{
					new_defs.Add(new Def() { type = "quadtransform" });
				}
				//}
		
				new_defs.Add(def);
		
				//if (def.activation != null)
				//{
				//	if (def.activation == "relu") { new_defs.Add(new Def() { type = "relu" }); }
				//	else if (def.activation == "sigmoid") { new_defs.Add(new Def() { type = "sigmoid" }); }
				//	else if (def.activation == "tanh") { new_defs.Add(new Def() { type = "tanh" }); }
				//	else if (def.activation == "maxout")
				//	{
				//		// create maxout activation, and pass along group size, if provided
				//		var gs = def.group_size > 0 ? def.group_size : 2;
				//		new_defs.Add(new Def() { type = "maxout", group_size = gs });
				//	}
				//	else
				//	{
				//		//console.log("ERROR unsupported activation " + def.activation);
				//		throw new Exception();
				//	}
				//}
				if (def.drop_prob != null && def.type != "dropout")
				{
					new_defs.Add(new Def() { type = "dropout", drop_prob = def.drop_prob });
				}
		
			}
			return new_defs;
		}


		public void makeLayers(List<Def> defs)
		{
		
			// few checks for now
			//if(defs.length<2) {console.log('ERROR! For now at least have input and softmax layers.');}
			//if(defs[0].type !== 'input') {console.log('ERROR! For now first layer should be input.');}
		
			//defs = desugar(defs);
		
			// create the layers
			this.layers = new List<Layer>();
		
			for (int i = 0; i < defs.Count; i++)
			{
				Def def = defs[i];
				if (i > 0)
				{
					Layer prev = this.layers[i - 1];
					def.in_sx = prev.out_sx;
					def.in_sy = prev.out_sy;
					def.in_depth = prev.out_depth;
				}
		
				switch (def.type)
				{
					case "fc": this.layers.Add(new FullyConnLayer(def)); break;
					//case "lrn": this.layers.Add(new LocalResponseNormalizationLayer(def)); break;
					//case "dropout": this.layers.Add(new DropoutLayer(def)); break;
					case "input": this.layers.Add(new InputLayer(def)); break;
					case "softmax": this.layers.Add(new SoftmaxLayer(def)); break;
					case "regression": this.layers.Add(new RegressionLayer(def)); break;
					case "conv": this.layers.Add(new ConvLayer(def)); break;
					case "pool": this.layers.Add(new PoolLayer(def)); break;
					case "relu": this.layers.Add(new ReluLayer(def)); break;
					//case "sigmoid": this.layers.Add(new SigmoidLayer(def)); break;
					//case "tanh": this.layers.Add(new TanhLayer(def)); break;
					//case "maxout": this.layers.Add(new MaxoutLayer(def)); break;
					//case "quadtransform": this.layers.Add(new QuadTransformLayer(def)); break;
					//case "svm": this.layers.Add(new SVMLayer(def)); break;
					default: //console.log('ERROR: UNRECOGNIZED LAYER TYPE!');
						throw new Exception();
				}
			}
		}

		// forward prop the network. A trainer will pass in is_training = true
		public Vol forward(Vol V, bool is_training)
		{
			//if(typeof(is_training)==='undefined') is_training = false;
			Vol act = this.layers[0].forward(V, is_training);
			for (int i = 1; i < this.layers.Count; i++)
			{
				act = this.layers[i].forward(act, is_training);
			}
			return act;
		}
		//		public     getCostLoss: function(V, y) {
		//      this.forward(V, false);
		//      var N = this.layers.length;
		//      var loss = this.layers[N-1].backward(y);
		//      return loss;
		//    },

		// backprop: compute gradients wrt all parameters
		public float backward(DataSet y)
		{
			int N = this.layers.Count;
			float loss = this.layers[N - 1].backward(y); // last layer assumed softmax
			for (int i = N - 2; i >= 0; i--)
			{ // first layer assumed input
				this.layers[i].backward(null);
			}
			return loss;
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
