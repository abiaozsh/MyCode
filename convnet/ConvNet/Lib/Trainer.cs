using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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
			Stopwatch sw = new Stopwatch();
			sw.Start();
			this.net.forward(x, true); // also set the flag that lets the net know we're just training
			long t1 = sw.ElapsedTicks;
			float cost_loss = this.net.backward(y);
			long t2 = sw.ElapsedTicks;
			float l2_decay_loss = 0.0f;
			float l1_decay_loss = 0.0f;

			this.k++;
			if (this.k % this.batch_size == 0)
			{
				float one_batch_size = 1.0f / batch_size;

				List<ParamsAndGrads> pglist = this.net.getParamsAndGrads();


				if (this.method == "adagrad")
				{
					//1
					// perform an update for all sets of weights
					for (int i = 0; i < pglist.Count; i++)
					{
						ParamsAndGrads pg = pglist[i]; // param, gradient, other options in future (custom learning rate etc)

						// learning rate for some parameters.
						float l2_decay_mul = pg.l2_decay_mul;//typeof pg.l2_decay_mul != "undefined" ? pg.l2_decay_mul : 1.0;
						float l1_decay_mul = pg.l1_decay_mul;//typeof pg.l1_decay_mul != "undefined" ? pg.l1_decay_mul : 1.0;
						float l2_decay = this.l2_decay * l2_decay_mul;
						float l1_decay = this.l1_decay * l1_decay_mul;
						for (int j = 0; j < pg.params_size; j++)
						{
							//l2_decay_loss += l2_decay * pg.params_[j + pg.params_idx] * pg.params_[j + pg.params_idx] / 2; // accumulate weight decay loss
							//l1_decay_loss += l1_decay * Math.Abs(pg.params_[j + pg.params_idx]);
							float l1grad = l1_decay * (pg.params_[j] > 0 ? 1 : -1);
							float l2grad = l2_decay * (pg.params_[j]);

							float gij = (l2grad + l1grad + pg.grads_[j]) * one_batch_size; // raw batch gradient

							// adagrad update
							pg.gsum[j] = pg.gsum[j] + gij * gij;
							float dx = -this.learning_rate / (float)Math.Sqrt(pg.gsum[j] + this.eps) * gij;
							pg.params_[j] += dx;

							pg.grads_[j] = 0.0f; // zero out gradient so that we can begin accumulating anew
						}
					}
				}
				else if (this.method == "windowgrad")
				{

					//2
					// perform an update for all sets of weights
					for (int i = 0; i < pglist.Count; i++)
					{
						ParamsAndGrads pg = pglist[i]; // param, gradient, other options in future (custom learning rate etc)

						// learning rate for some parameters.
						float l2_decay_mul = pg.l2_decay_mul;//typeof pg.l2_decay_mul != "undefined" ? pg.l2_decay_mul : 1.0;
						float l1_decay_mul = pg.l1_decay_mul;//typeof pg.l1_decay_mul != "undefined" ? pg.l1_decay_mul : 1.0;
						float l2_decay = this.l2_decay * l2_decay_mul;
						float l1_decay = this.l1_decay * l1_decay_mul;
						for (int j = 0; j < pg.params_size; j++)
						{
							//l2_decay_loss += l2_decay * pg.params_[j + pg.params_idx] * pg.params_[j + pg.params_idx] / 2; // accumulate weight decay loss
							//l1_decay_loss += l1_decay * Math.Abs(pg.params_[j + pg.params_idx]);
							float l1grad = l1_decay * (pg.params_[j] > 0 ? 1 : -1);
							float l2grad = l2_decay * (pg.params_[j]);

							float gij = (l2grad + l1grad + pg.grads_[j]) * one_batch_size; // raw batch gradient

							// this is adagrad but with a moving window weighted average
							// so the gradient is not accumulated over the entire history of the run. 
							// it's also referred to as Idea #1 in Zeiler paper on Adadelta. Seems reasonable to me!
							pg.gsum[j] = this.ro * pg.gsum[j] + (1 - this.ro) * gij * gij;
							float dx = -this.learning_rate / (float)Math.Sqrt(pg.gsum[j] + this.eps) * gij; // eps added for better conditioning
							pg.params_[j] += dx;

							pg.grads_[j] = 0.0f; // zero out gradient so that we can begin accumulating anew
						}
					}
				}
				else if (this.method == "adadelta")
				{

					//3
					// perform an update for all sets of weights
					Parallel.For(0, pglist.Count, (i) =>//for (int i = 0; i < pglist.Count; i++)
					{
						ParamsAndGrads pg = pglist[i]; // param, gradient, other options in future (custom learning rate etc)

						// learning rate for some parameters.
						float l2_decay_mul = pg.l2_decay_mul;//typeof pg.l2_decay_mul != "undefined" ? pg.l2_decay_mul : 1.0;
						float l1_decay_mul = pg.l1_decay_mul;//typeof pg.l1_decay_mul != "undefined" ? pg.l1_decay_mul : 1.0;
						float l2_decay = this.l2_decay * l2_decay_mul;
						float l1_decay = this.l1_decay * l1_decay_mul;
						for (int j = 0; j < pg.params_size; j++)
						{
							//l2_decay_loss += l2_decay * pg.params_[j + pg.params_idx] * pg.params_[j + pg.params_idx] / 2; // accumulate weight decay loss
							//l1_decay_loss += l1_decay * Math.Abs(pg.params_[j + pg.params_idx]);
							float l1grad = l1_decay * (pg.params_[j] > 0 ? 1 : -1);
							float l2grad = l2_decay * (pg.params_[j]);

							float gij = (l2grad + l1grad + pg.grads_[j]) * one_batch_size; // raw batch gradient

							// assume adadelta if not sgd or adagrad
							pg.gsum[j] = this.ro * pg.gsum[j] + (1 - this.ro) * gij * gij;
							float dx = -(float)Math.Sqrt((pg.xsum[j] + this.eps) / (pg.gsum[j] + this.eps)) * gij;
							pg.xsum[j] = this.ro * pg.xsum[j] + (1 - this.ro) * dx * dx; // yes, xsum lags behind gsum by 1.
							pg.params_[j] += dx;

							pg.grads_[j] = 0.0f; // zero out gradient so that we can begin accumulating anew
						}
					});
				}
				else
				{
					// assume SGD
					if (this.momentum > 0.0)
					{
						//4
						// perform an update for all sets of weights
						Parallel.For(0, pglist.Count, (i) =>//for (int i = 0; i < pglist.Count; i++)
						{
							ParamsAndGrads pg = pglist[i]; // param, gradient, other options in future (custom learning rate etc)

							// learning rate for some parameters.
							float l2_decay_mul = pg.l2_decay_mul;//typeof pg.l2_decay_mul != "undefined" ? pg.l2_decay_mul : 1.0;
							float l1_decay_mul = pg.l1_decay_mul;//typeof pg.l1_decay_mul != "undefined" ? pg.l1_decay_mul : 1.0;
							float l2_decay = this.l2_decay * l2_decay_mul;
							float l1_decay = this.l1_decay * l1_decay_mul;
							for (int j = 0; j < pg.params_size; j++)
							{
								//l2_decay_loss += l2_decay * pg.params_[j + pg.params_idx] * pg.params_[j + pg.params_idx] / 2; // accumulate weight decay loss
								//l1_decay_loss += l1_decay * Math.Abs(pg.params_[j + pg.params_idx]);
								float l1grad = l1_decay * (pg.params_[j] > 0 ? 1 : -1);
								float l2grad = l2_decay * (pg.params_[j]);

								float gij = (l2grad + l1grad + pg.grads_[j]) * one_batch_size; // raw batch gradient

								// momentum update
								float dx = this.momentum * pg.gsum[j] - this.learning_rate * gij; // step
								pg.gsum[j] = dx; // back this up for next iteration of momentum
								pg.params_[j] += dx; // apply corrected gradient

								pg.grads_[j] = 0.0f; // zero out gradient so that we can begin accumulating anew
							}
						});
					}
					else
					{
						//5
						// perform an update for all sets of weights
						for (int i = 0; i < pglist.Count; i++)
						{
							ParamsAndGrads pg = pglist[i]; // param, gradient, other options in future (custom learning rate etc)

							// learning rate for some parameters.
							float l2_decay_mul = pg.l2_decay_mul;//typeof pg.l2_decay_mul != "undefined" ? pg.l2_decay_mul : 1.0;
							float l1_decay_mul = pg.l1_decay_mul;//typeof pg.l1_decay_mul != "undefined" ? pg.l1_decay_mul : 1.0;
							float l2_decay = this.l2_decay * l2_decay_mul;
							float l1_decay = this.l1_decay * l1_decay_mul;
							for (int j = 0; j < pg.params_size; j++)
							{
								//l2_decay_loss += l2_decay * pg.params_[j + pg.params_idx] * pg.params_[j + pg.params_idx] / 2; // accumulate weight decay loss
								//l1_decay_loss += l1_decay * Math.Abs(pg.params_[j + pg.params_idx]);
								float l1grad = l1_decay * (pg.params_[j] > 0 ? 1 : -1);
								float l2grad = l2_decay * (pg.params_[j]);

								float gij = (l2grad + l1grad + pg.grads_[j]) * one_batch_size; // raw batch gradient

								float tmp = -this.learning_rate * gij;
								// momentum update
								float dx = this.momentum * pg.gsum[j] + tmp; // step
								pg.gsum[j] = dx; // back this up for next iteration of momentum
								pg.params_[j] += dx; // apply corrected gradient
								// vanilla sgd
								pg.params_[j] += tmp;

								pg.grads_[j] = 0.0f; // zero out gradient so that we can begin accumulating anew
							}
						}
					}
				}


				long t3 = sw.ElapsedTicks;

				long tt = t1 + t2 + t3;
				tt++;
				//Console.WriteLine(tt);
			}

			// appending softmax_loss for backwards compatibility, but from now on we will always use cost_loss
			// in future, TODO: have to completely redo the way loss is done around the network as currently 
			// loss is a bit of a hack. Ideally, user should specify arbitrary number of loss functions on any layer
			// and it should all be computed correctly and automatically. 

			return new Report()
			{
				l2_decay_loss = l2_decay_loss,
				l1_decay_loss = l1_decay_loss,
				cost_loss = cost_loss,
				softmax_loss = cost_loss,
				loss = cost_loss + l1_decay_loss + l2_decay_loss
			};
		}

	}

}
