using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ConvNet
{

	public class Trainer
	{
		public class Option
		{
			public float learning_rate = 0.01f;
			public float l1_decay = 0.0f;
			public float l2_decay = 0.0f;
			public int batch_size = 1;
			public string method = "sgd";
			public float momentum = 0.9f;
			public float ro = 0.95f;
			public float eps = 1e-6f;
		}

		public float learning_rate;
		public float l1_decay;
		public float l2_decay;
		public int batch_size;
		public string method;
		public float momentum;
		public float ro;
		public float eps;
		public int k = 0; //int?
		public List<float[]> gsum; //[]?
		public List<float[]> xsum; //[]?
		public Net net;
		public Trainer(Net net, Option options)
		{

			this.net = net;

			this.learning_rate = options.learning_rate;
			this.l1_decay = options.l1_decay;
			this.l2_decay = options.l2_decay;
			this.batch_size = options.batch_size;
			this.method = options.method; // sgd/adagrad/adadelta/windowgrad

			this.momentum = options.momentum;
			this.ro = options.ro; // used in adadelta
			this.eps = options.eps; // used in adadelta

			this.k = 0; // iteration counter
			this.gsum = new List<float[]>(); // last iteration gradients (used for momentum calculations)
			this.xsum = new List<float[]>(); // used in adadelta
		}

		public class Report
		{
			public float l2_decay_loss;
			public float l1_decay_loss;
			public float cost_loss;
			public float softmax_loss;
			public float loss;
		}
		public Report train(Vol x, DataSet y)
		{
			this.net.forward(x, true); // also set the flag that lets the net know we're just training
			float cost_loss = this.net.backward(y);
			float l2_decay_loss = 0.0f;
			float l1_decay_loss = 0.0f;

			this.k++;
			if (this.k % this.batch_size == 0)
			{

				List<ParamsAndGrads> pglist = this.net.getParamsAndGrads();

				// initialize lists for accumulators. Will only be done once on first iteration
				if (this.gsum.Count == 0 && (this.method != "sgd" || this.momentum > 0.0))
				{
					// only vanilla sgd doesnt need either lists
					// momentum needs gsum
					// adagrad needs gsum
					// adadelta needs gsum and xsum
					for (int i = 0; i < pglist.Count; i++)
					{
						this.gsum.Add(Util.zeros(pglist[i].Params.Length));
						if (this.method == "adadelta")
						{
							this.xsum.Add(Util.zeros(pglist[i].Params.Length));
						}
						else
						{
							this.xsum.Add(new float[0]); // conserve memory
						}
					}
				}

				// perform an update for all sets of weights
				for (int i = 0; i < pglist.Count; i++)
				{
					ParamsAndGrads pg = pglist[i]; // param, gradient, other options in future (custom learning rate etc)
					float[] p = pg.Params;
					float[] g = pg.grads;

					// learning rate for some parameters.
					float l2_decay_mul = pg.l2_decay_mul;//typeof pg.l2_decay_mul != "undefined" ? pg.l2_decay_mul : 1.0;
					float l1_decay_mul = pg.l1_decay_mul;//typeof pg.l1_decay_mul != "undefined" ? pg.l1_decay_mul : 1.0;
					float l2_decay = this.l2_decay * l2_decay_mul;
					float l1_decay = this.l1_decay * l1_decay_mul;

					int plen = p.Length;
					for (int j = 0; j < plen; j++)
					{
						l2_decay_loss += l2_decay * p[j] * p[j] / 2; // accumulate weight decay loss
						l1_decay_loss += l1_decay * Math.Abs(p[j]);
						float l1grad = l1_decay * (p[j] > 0 ? 1 : -1);
						float l2grad = l2_decay * (p[j]);

						float gij = (l2grad + l1grad + g[j]) / this.batch_size; // raw batch gradient

						float[] gsumi = this.gsum[i];
						float[] xsumi = this.xsum[i];
						if (this.method == "adagrad")
						{
							// adagrad update
							gsumi[j] = gsumi[j] + gij * gij;
							float dx = -this.learning_rate / (float)Math.Sqrt(gsumi[j] + this.eps) * gij;
							p[j] += dx;
						}
						else if (this.method == "windowgrad")
						{
							// this is adagrad but with a moving window weighted average
							// so the gradient is not accumulated over the entire history of the run. 
							// it's also referred to as Idea #1 in Zeiler paper on Adadelta. Seems reasonable to me!
							gsumi[j] = this.ro * gsumi[j] + (1 - this.ro) * gij * gij;
							float dx = -this.learning_rate / (float)Math.Sqrt(gsumi[j] + this.eps) * gij; // eps added for better conditioning
							p[j] += dx;
						}
						else if (this.method == "adadelta")
						{
							// assume adadelta if not sgd or adagrad
							gsumi[j] = this.ro * gsumi[j] + (1 - this.ro) * gij * gij;
							float dx = -(float)Math.Sqrt((xsumi[j] + this.eps) / (gsumi[j] + this.eps)) * gij;
							xsumi[j] = this.ro * xsumi[j] + (1 - this.ro) * dx * dx; // yes, xsum lags behind gsum by 1.
							p[j] += dx;
						}
						else
						{
							// assume SGD
							if (this.momentum > 0.0)
							{
								// momentum update
								float dx = this.momentum * gsumi[j] - this.learning_rate * gij; // step
								gsumi[j] = dx; // back this up for next iteration of momentum
								p[j] += dx; // apply corrected gradient
							}
							else
							{
								// vanilla sgd
								p[j] += -this.learning_rate * gij;
							}
						}
						g[j] = 0.0f; // zero out gradient so that we can begin accumulating anew
					}
				}
			}

			// appending softmax_loss for backwards compatibility, but from now on we will always use cost_loss
			// in future, TODO: have to completely redo the way loss is done around the network as currently 
			// loss is a bit of a hack. Ideally, user should specify arbitrary number of loss functions on any layer
			// and it should all be computed correctly and automatically. 

			return new Report(){
				l2_decay_loss= l2_decay_loss, 
				l1_decay_loss= l1_decay_loss,
				cost_loss= cost_loss, 
				softmax_loss= cost_loss, 
				loss= cost_loss + l1_decay_loss + l2_decay_loss
			};
		}

	}

}
