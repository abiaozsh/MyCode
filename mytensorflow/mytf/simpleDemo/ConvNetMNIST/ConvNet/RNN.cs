using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ConvNet;

namespace Lib
{

	public class Model : Dictionary<String, Mat>
	{
		//Dictionary<String,String> d;
		//public Mat this[String key] { get; set; }

	}

	public class Mat
	{
		public int n;
		public int d;
		public MyFloat w;
		public MyFloat dw;
		public Object parent;

		public Mat(int n, int d, Object parent)
		{
		this.parent = parent;
			this.n = n;
			this.d = d;
			this.w = new MyFloat(n * d);
			this.dw = new MyFloat(n * d);
		}
	}


	public class Graph
	{

		public bool needs_backprop;
		public List<Backward> backprop = new List<Backward>();//??

		public Graph(bool needs_backprop)
		{
			this.needs_backprop = needs_backprop;
		}
		public void backward()
		{
			for (var i = this.backprop.Count - 1; i >= 0; i--)
			{
				this.backprop[i].backward(); // tick!
			}
		}

		public interface Backward
		{
			void backward();
		}

		class MulBackward : Backward
		{
			public Mat m1;
			public Mat m2;
			public Mat _out;
			public void backward()
			{
				//Parallel.For(0, m1.n, (i) =>
				for (var i = 0; i < m1.n; i++)
				{
					int m2_d_i = m2.d * i;
					int m1_d_i = m1.d * i;
					// loop over rows of m1
					for (var j = 0; j < m2.d; j++)
					{ // loop over cols of m2
						for (var k = 0; k < m1.d; k++)
						{ // dot product loop
							var b = _out.dw[m2_d_i + j];
							m1.dw[m1_d_i + k] += m2.w[m2.d * k + j] * b;
							m2.dw[m2.d * k + j] += m1.w[m1_d_i + k] * b;
						}
					}
				}
				//);
			}
		}

		public Mat mul(Mat m1, Mat m2)
		{
			// multiply matrices m1 * m2
			//assert(m1.d === m2.n, 'matmul dimensions misaligned');

			var _out = new Mat(m1.n, m2.d,"temp mul");
			//Parallel.For(0, m1.n, (i) =>
			for (var i = 0; i < m1.n; i++)
			{
				int m1_d_i = m1.d * i;
				int m2_d_i = m2.d * i;
				// loop over rows of m1
				for (int j = 0; j < m2.d; j++)
				{ // loop over cols of m2
					var dot = 0.0f;
					for (int k = 0; k < m1.d; k++)
					{ // dot product loop
						dot += m1.w[m1_d_i + k] * m2.w[m2.d * k + j];
					}
					_out.w[m2_d_i + j] = dot;
				}
			}
			//);

			if (this.needs_backprop)
			{
				MulBackward backward = new MulBackward();
				backward.m1 = m1;
				backward.m2 = m2;
				backward._out = _out;
				backprop.Add(backward);
			}
			return _out;
		}

		class AddBackward : Backward
		{
			public Mat m1;
			public Mat m2;
			public Mat _out;
			public void backward()
			{
				for (int i = 0; i < m1.w.size; i++)
				{
					m1.dw[i] += _out.dw[i];
					m2.dw[i] += _out.dw[i];
				}
			}
		}

		public Mat add(Mat m1, Mat m2)
		{
			//assert(m1.w.length === m2.w.length);
			var _out = new Mat(m1.n, m1.d, "temp add");
			for (int i = 0; i < m1.w.size; i++)
			{
				_out.w[i] = m1.w[i] + m2.w[i];
			}
			if (this.needs_backprop)
			{
				AddBackward backward = new AddBackward();
				backward.m1 = m1;
				backward.m2 = m2;
				backward._out = _out;
				backprop.Add(backward);
			}
			return _out;
		}

		class SigBackward : Backward
		{
			public Mat m;
			public Mat _out;
			public int n;
			public void backward()
			{
				for (var i = 0; i < n; i++)
				{
					// grad for z = tanh(x) is (1 - z^2)
					var mwi = _out.w[i];
					m.dw[i] += mwi * (1.0f - mwi) * _out.dw[i];
				}
			}
		}

		public Mat sigmoid(Mat m)
		{
			// sigmoid nonlinearity
			var _out = new Mat(m.n, m.d, "temp sig");
			var n = m.w.size;
			for (var i = 0; i < n; i++)
			{
				// helper function for computing sigmoid
				//return 1.0f / (1.0f + (float)Math.Exp(-x));
				_out.w[i] = 1.0f / (1.0f + (float)Math.Exp(-m.w[i]));//sig(m.w[i]);
			}

			if (this.needs_backprop)
			{
				SigBackward backward = new SigBackward();
				backward.m = m;
				backward._out = _out;
				backward.n = n;
				backprop.Add(backward);
			}
			return _out;
		}

		class EltmulBackward : Backward
		{
			public Mat m1;
			public Mat m2;
			public Mat _out;
			public void backward()
			{
				for (int i = 0, n = m1.w.size; i < n; i++)
				{
					m1.dw[i] += m2.w[i] * _out.dw[i];
					m2.dw[i] += m1.w[i] * _out.dw[i];
				}
			}
		}

		public Mat eltmul(Mat m1, Mat m2)
		{
			//assert(m1.w.length === m2.w.length);
			var _out = new Mat(m1.n, m1.d, "temp eltmul");
			for (int i = 0, n = m1.w.size; i < n; i++)
			{
				_out.w[i] = m1.w[i] * m2.w[i];
			}
			if (this.needs_backprop)
			{
				EltmulBackward backward = new EltmulBackward();
				backward.m1 = m1;
				backward.m2 = m2;
				backward._out = _out;
				backprop.Add(backward);
			}
			return _out;
		}

		class TanhBackward : Backward
		{
			public Mat m;
			public Mat _out;
			public int n;
			public void backward()
			{
				for (var i = 0; i < n; i++)
				{
					// grad for z = tanh(x) is (1 - z^2)
					var mwi = _out.w[i];
					m.dw[i] += (1.0f - mwi * mwi) * _out.dw[i];
				}
			}
		}

		public Mat tanh(Mat m)
		{
			// tanh nonlinearity
			var _out = new Mat(m.n, m.d, "temp tanh");
			var n = m.w.size;
			for (var i = 0; i < n; i++)
			{
				_out.w[i] = (float)Math.Tanh(m.w[i]);
			}

			if (this.needs_backprop)
			{
				TanhBackward backward = new TanhBackward();
				backward.m = m;
				backward._out = _out;
				backward.n = n;
				backprop.Add(backward);
			}
			return _out;
		}

		class ReluBackward : Backward
		{
			public Mat m;
			public Mat _out;
			public int n;
			public void backward()
			{

				for (var i = 0; i < n; i++)
				{
					m.dw[i] += m.w[i] > 0.0f ? _out.dw[i] : 0.0f;
				}

			}
		}

		public Mat relu(Mat m)
		{
			var _out = new Mat(m.n, m.d, "temp relu");
			var n = m.w.size;
			for (var i = 0; i < n; i++)
			{
				_out.w[i] = Math.Max(0, m.w[i]); // relu
			}
			if (this.needs_backprop)
			{
				ReluBackward backward = new ReluBackward();
				backward.m = m;
				backward._out = _out;
				backward.n = n;
				backprop.Add(backward);
			}
			return _out;
		}

		class RowPluckBackward : Backward
		{
			public Mat m;
			public Mat _out;
			public int d;
			public int ix;
			public void backward()
			{

				for (int i = 0, n = d; i < n; i++) { m.dw[d * ix + i] += _out.dw[i]; }

			}
		}

		public Mat rowPluck(Mat m, int ix)
		{
			// pluck a row of m with index ix and return it as col vector
			//assert(ix >= 0 && ix < m.n);
			var d = m.d;
			var _out = new Mat(d, 1, "temp rowPluck");
			for (int i = 0, n = d; i < n; i++) { _out.w[i] = m.w[d * ix + i]; } // copy over the data

			if (this.needs_backprop)
			{
				RowPluckBackward backward = new RowPluckBackward();
				backward.m = m;
				backward._out = _out;
				backward.d = d;
				backward.ix = ix;
				backprop.Add(backward);
			}
			return _out;
		}


	}


	public class Solver
	{
		float decay_rate;
		float smooth_eps;
		Model step_cache;
		public Solver()
		{
			this.decay_rate = 0.999f;
			this.smooth_eps = 1e-8f;
			this.step_cache = new Model();
		}

		public float step(Model model, float step_size, float regc, float clipval)
		{
			// perform parameter update
			//var solver_stats = {};
			int num_clipped = 0;
			int num_tot = 0;
			foreach (var k in model)
			{
				//if(model.hasOwnProperty(k)) {
				var m = k.Value;//model[k]; // mat ref
				if (!(this.step_cache.ContainsKey(k.Key)))
				{
					this.step_cache[k.Key] = new Mat(m.n, m.d, this.step_cache);
				}
				var s = this.step_cache[k.Key];
				for (int i = 0, n = m.w.size; i < n; i++)
				{

					// rmsprop adaptive learning rate
					var mdwi = m.dw[i];
					s.w[i] = s.w[i] * this.decay_rate + (1.0f - this.decay_rate) * mdwi * mdwi;

					// gradient clip
					if (mdwi > clipval)
					{
						mdwi = clipval;
						num_clipped++;
					}
					if (mdwi < -clipval)
					{
						mdwi = -clipval;
						num_clipped++;
					}
					num_tot++;

					// update (and regularize)
					m.w[i] += -step_size * mdwi / (float)Math.Sqrt(s.w[i] + this.smooth_eps) - regc * m.w[i];
					m.dw[i] = 0.0f; // reset gradients for next iteration
				}
				//}
			}
			float ratio_clipped = num_clipped * 1.0f / num_tot;
			return ratio_clipped;
		}


	}


	public class RNN
	{
		public static void fillRand(Mat m, float lo, float hi)
		{
			for (int i = 0; i < m.w.size; i++)
			{
				m.w[i] = Util.randf(lo, hi);
			}
		}

		public static Mat RandMat(int n, int d, int mu, float std,Object parent)
		{
			var m = new Mat(n, d, parent);
			fillRand(m, -std, std); // kind of :P
			return m;
		}

		public static Model initLSTM(int input_size, int[] hidden_sizes, int output_size)
		{
			// hidden size should be a list
			int hidden_size = 0;
			Model model = new Model();
			for (var d = 0; d < hidden_sizes.Length; d++)
			{ // loop over depths
				var prev_size = d == 0 ? input_size : hidden_sizes[d - 1];
				hidden_size = hidden_sizes[d];

				// gates parameters
				model["Wix" + d] = RandMat(hidden_size, prev_size, 0, 0.08f, "Wix" + d);
				model["Wih" + d] = RandMat(hidden_size, hidden_size, 0, 0.08f,"Wih" + d);
				model["bi" + d] = new Mat(hidden_size, 1,"bi" + d);
				model["Wfx" + d] = RandMat(hidden_size, prev_size, 0, 0.08f, "Wfx" + d);
				model["Wfh" + d] = RandMat(hidden_size, hidden_size, 0, 0.08f, "Wfh" + d);
				model["bf" + d] = new Mat(hidden_size, 1, "bf" + d);
				model["Wox" + d] = RandMat(hidden_size, prev_size, 0, 0.08f, "Wox" + d);
				model["Woh" + d] = RandMat(hidden_size, hidden_size, 0, 0.08f, "Woh" + d);
				model["bo" + d] = new Mat(hidden_size, 1, "bo" + d);
				// cell write params
				model["Wcx" + d] = RandMat(hidden_size, prev_size, 0, 0.08f, "Wcx" + d);
				model["Wch" + d] = RandMat(hidden_size, hidden_size, 0, 0.08f, "Wch" + d);
				model["bc" + d] = new Mat(hidden_size, 1, "bc" + d);
			}
			// decoder params
			model["Whd"] = RandMat(output_size, hidden_size, 0, 0.08f, "Whd");
			model["bd"] = new Mat(output_size, 1, "bd");
			return model;
		}

		public class HCO
		{
			public List<Mat> h;
			public List<Mat> c;
			public Mat o;
		}
		public static HCO forwardLSTM(Graph G, Model model, int[] hidden_sizes, Mat x, HCO prev)
		{
			// forward prop for a single tick of LSTM
			// G is graph to append ops to
			// model contains LSTM parameters
			// x is 1D column vector with observation
			// prev is a struct containing hidden and cell
			// from previous iteration
			List<Mat> hidden_prevs;
			List<Mat> cell_prevs;

			if (prev == null)
			{
				hidden_prevs = new List<Mat>();
				cell_prevs = new List<Mat>();
				for (var d = 0; d < hidden_sizes.Length; d++)
				{
					hidden_prevs.Add(new Mat(hidden_sizes[d], 1, hidden_prevs));
					cell_prevs.Add(new Mat(hidden_sizes[d], 1, cell_prevs));
				}
			}
			else
			{
				hidden_prevs = prev.h;
				cell_prevs = prev.c;
			}

			List<Mat> hidden = new List<Mat>();
			List<Mat> cell = new List<Mat>();
			for (var d = 0; d < hidden_sizes.Length; d++)
			{

				var input_vector = d == 0 ? x : hidden[d - 1];
				var hidden_prev = hidden_prevs[d];
				var cell_prev = cell_prevs[d];

				Graph GC = new Graph(true);
				Graph GO = new Graph(true);
				Graph GF = new Graph(true);
				Graph GI = new Graph(true);
				Graph G3 = new Graph(true);
				Mat cell_write = null;
				Mat output_gate = null;
				Mat retain_cell = null;
				Mat input_gate = null;
				Parallel.For(0, 4, (i) =>
				{
					if (i == 0)
					{
						// input gate
						var h0 = GI.mul(model["Wix" + d], input_vector);
						var h1 = GI.mul(model["Wih" + d], hidden_prev);
						var h01 = GI.add(h0, h1);
						var tmp = GI.add(h01, model["bi" + d]);
						input_gate = GI.sigmoid(tmp);
					}
					if (i == 1)
					{
						// forget gate
						var h0 = GF.mul(model["Wfx" + d], input_vector);
						var h1 = GF.mul(model["Wfh" + d], hidden_prev);
						var h01 = GF.add(h0, h1);
						var tmp = GF.add(h01, model["bf" + d]);
						var forget_gate = GF.sigmoid(tmp);
						retain_cell = G3.eltmul(forget_gate, cell_prev); // what do we keep from cell
					}
					if (i == 2)
					{
						// output gate
						var h0 = GO.mul(model["Wox" + d], input_vector);
						var h1 = GO.mul(model["Woh" + d], hidden_prev);
						var h01 = GO.add(h0, h1);
						var tmp = GO.add(h01, model["bo" + d]);
						output_gate = GO.sigmoid(tmp);
					}
					if (i == 3)
					{
						// write operation on cells
						var h0 = GC.mul(model["Wcx" + d], input_vector);
						var h1 = GC.mul(model["Wch" + d], hidden_prev);
						var h01 = GC.add(h0, h1);
						var tmp = GC.add(h01, model["bc" + d]);
						cell_write = GC.tanh(tmp);
					}
				});

				G.backprop.AddRange(GI.backprop);
				G.backprop.AddRange(GF.backprop);
				G.backprop.AddRange(GO.backprop);
				G.backprop.AddRange(GC.backprop);

				G.backprop.AddRange(G3.backprop);
				
				// compute new cell activation
				var write_cell = G.eltmul(input_gate, cell_write); // what do we write to cell
				var cell_d = G.add(retain_cell, write_cell); // new cell contents

				// compute hidden state as gated, saturated cell activations
				var hidden_d = G.eltmul(output_gate, G.tanh(cell_d));

				hidden.Add(hidden_d);
				cell.Add(cell_d);
			}

			// one decoder to outputs at end
			var output = G.add(G.mul(model["Whd"], hidden[hidden.Count - 1]), model["bd"]);

			// return cell memory, hidden representation and output
			return new HCO() { h = hidden, c = cell, o = output };
		}

		//public static Model initRNN(int input_size, int[] hidden_sizes, int output_size)
		//{
		//	// hidden size should be a list
		//	int hidden_size = 0;
		//	Model model = new Model();
		//	for (var d = 0; d < hidden_sizes.Length; d++)
		//	{ // loop over depths
		//		var prev_size = d == 0 ? input_size : hidden_sizes[d - 1];
		//		hidden_size = hidden_sizes[d];
		//		model["Wxh" + d] = RandMat(hidden_size, prev_size, 0, 0.08f);
		//		model["Whh" + d] = RandMat(hidden_size, hidden_size, 0, 0.08f);
		//		model["bhh" + d] = new Mat(hidden_size, 1);
		//	}
		//	// decoder params
		//	model["Whd"] = RandMat(output_size, hidden_size, 0, 0.08f);
		//	model["bd"] = new Mat(output_size, 1);
		//	return model;
		//}

		//public static HCO forwardRNN(Graph G, Model model, int[] hidden_sizes, Mat x, HCO prev)
		//{
		//	// forward prop for a single tick of RNN
		//	// G is graph to append ops to
		//	// model contains RNN parameters
		//	// x is 1D column vector with observation
		//	// prev is a struct containing hidden activations from last step
		//
		//	List<Mat> hidden_prevs;
		//
		//	if (prev == null)
		//	{
		//		hidden_prevs = new List<Mat>();
		//		for (var d = 0; d < hidden_sizes.Length; d++)
		//		{
		//			hidden_prevs.Add(new Mat(hidden_sizes[d], 1));
		//		}
		//	}
		//	else
		//	{
		//		hidden_prevs = prev.h;
		//	}
		//
		//	List<Mat> hidden = new List<Mat>();
		//	for (var d = 0; d < hidden_sizes.Length; d++)
		//	{
		//
		//		var input_vector = d == 0 ? x : hidden[d - 1];
		//		var hidden_prev = hidden_prevs[d];
		//
		//		var h0 = G.mul(model["Wxh" + d], input_vector);
		//		var h1 = G.mul(model["Whh" + d], hidden_prev);
		//		var h01 = G.add(h0, h1);
		//		var tmp = G.add(h01, model["bhh" + d]);
		//		var hidden_d = G.relu(tmp);
		//
		//		hidden.Add(hidden_d);
		//	}
		//
		//	// one decoder to outputs at end
		//	var tmp2 = G.mul(model["Whd"], hidden[hidden.Count - 1]);
		//	var output = G.add(tmp2, model["bd"]);
		//
		//	// return cell memory, hidden representation and output
		//	return new HCO() { h = hidden, o = output };
		//}


		public static Mat softmax(Mat m)
		{
			var _out = new Mat(m.n, m.d, "temp softmax"); // probability volume
			var maxval = -999999.0f;
			for (int i = 0, n = m.w.size; i < n; i++) { if (m.w[i] > maxval) maxval = m.w[i]; }

			var s = 0.0f;
			for (int i = 0, n = m.w.size; i < n; i++)
			{
				_out.w[i] = (float)Math.Exp(m.w[i] - maxval);
				s += _out.w[i];
			}
			for (int i = 0, n = m.w.size; i < n; i++) { _out.w[i] /= s; }

			// no backward pass here needed
			// since we will use the computed probabilities outside
			// to set gradients directly on m
			return _out;
		}

		public static int maxi(MyFloat w)
		{
			// argmax of array w
			var maxv = w[0];
			var maxix = 0;
			for (int i = 1, n = w.size; i < n; i++)
			{
				var v = w[i];
				if (v > maxv)
				{
					maxix = i;
					maxv = v;
				}
			}
			return maxix;
		}

		public static int samplei(MyFloat w)
		{
			// sample argmax from w, assuming w are 
			// probabilities that sum to one
			var r = Util.randf(0, 1);
			var x = 0.0;
			var i = 0;
			while (true)
			{
				x += w[i];
				if (x > r) { return i; }
				i++;
			}
			//return w.size - 1; // pretty sure we should never get here?
		}

	}
}
