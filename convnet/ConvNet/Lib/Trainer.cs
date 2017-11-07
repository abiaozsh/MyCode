using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConvNet
{
	public abstract class Trainer
	{
		public int count = 0;
		public int batchSize;
		protected float oneBatchSize;

		public Trainer(int batchSize)
		{
			this.batchSize = batchSize;
			oneBatchSize = 1.0f / batchSize;

			//this.net = net;
			//
			//this.learning_rate = options.learning_rate;
			//this.l1_decay = options.l1_decay;
			//this.l2_decay = options.l2_decay;
			//this.batch_size = options.batch_size;
			//this.method = options.method; // sgd/adagrad/adadelta/windowgrad
			//
			//this.momentum = options.momentum;
			//this.ro = options.ro; // used in adadelta
			//this.eps = options.eps; // used in adadelta
			//
			//this.k = 0; // iteration counter
		}

		public abstract void train(int params_size, MyFloat params_, MyFloat grads_, int params_idx, int grads_idx, MyFloat gsum, MyFloat xsum, float l1_decay_mul, float l2_decay_mul);
	}

	//public abstract class TrainData { 
	//}
	//public class ParamsAndGrads
	//{
	//	public int params_size;
	//	public MyFloat params_;
	//	public MyFloat grads_;
	//	public int params_idx = 0;
	//	public int grads_idx = 0;
	//	public MyFloat gsum; //[]?
	//	public MyFloat xsum; //[]?
	//}

	public class AdaDeltaTrainer : Trainer
	{
		public float l1_decay = 0.0f;
		public float l2_decay = 0.0f;
		public float ro = 0.95f;
		public float eps = 1e-6f;

		//FullyConnLayer(int num_neurons = 0, float bias_pref = 0.0f, ActivationLayer act = null)
		public AdaDeltaTrainer(int batchSize)
			: base(batchSize)
		{
		}

		//public float l1_decay_mul = 1.0f;public float l2_decay_mul = 1.0f;
		public override void train(int params_size, MyFloat params_, MyFloat grads_, int params_idx, int grads_idx, MyFloat gsum, MyFloat xsum, float l1_decay_mul, float l2_decay_mul)
		{
			//ParamsAndGrads pg = pglist[i]; // param, gradient, other options in future (custom learning rate etc)
			// learning rate for some parameters.
			//float l2_decay_mul = pg.l2_decay_mul;//typeof pg.l2_decay_mul != "undefined" ? pg.l2_decay_mul : 1.0;
			//float l1_decay_mul = pg.l1_decay_mul;//typeof pg.l1_decay_mul != "undefined" ? pg.l1_decay_mul : 1.0;

			//float l2_decay = this.l2_decay * l2_decay_mul;
			//float l1_decay = this.l1_decay * l1_decay_mul;
			for (int j = 0; j < params_size; j++)
			{
				//l2_decay_loss += l2_decay * pg.params_[j + pg.params_idx] * pg.params_[j + pg.params_idx] / 2; // accumulate weight decay loss
				//l1_decay_loss += l1_decay * Math.Abs(pg.params_[j + pg.params_idx]);
				
				//float l1grad = l1_decay * (params_[j + params_idx] > 0 ? 1 : -1);
				//float l2grad = l2_decay * (params_[j + params_idx]);

				//l2grad + l1grad + 
				float gij = (grads_[j + grads_idx]) * oneBatchSize; // raw batch gradient

				// assume adadelta if not sgd or adagrad
				gsum[j] = this.ro * gsum[j] + (1 - this.ro) * gij * gij;
				float dx = -(float)Math.Sqrt((xsum[j] + this.eps) / (gsum[j] + this.eps)) * gij;
				xsum[j] = this.ro * xsum[j] + (1 - this.ro) * dx * dx; // yes, xsum lags behind gsum by 1.
				params_[j + params_idx] += dx;

				grads_[j + grads_idx] = 0.0f; // zero out gradient so that we can begin accumulating anew
			}

		}
	}

	public class SGDTrainer : Trainer
	{
		public float l1_decay = 0.0f;
		public float l2_decay = 0.0f;
		public float learning_rate = 0.01f;
		public float momentum = 0.9f;

		public SGDTrainer(int batchSize)
			: base(batchSize)
		{
		}

		//public float l1_decay_mul = 1.0f;public float l2_decay_mul = 1.0f;
		public override void train(int params_size, MyFloat params_, MyFloat grads_, int params_idx, int grads_idx, MyFloat gsum, MyFloat xsum, float l1_decay_mul, float l2_decay_mul)
		{
			// learning rate for some parameters.
			float l2_decay = this.l2_decay * l2_decay_mul;
			float l1_decay = this.l1_decay * l1_decay_mul;
			for (int j = 0; j < params_size; j++)
			{
				//l2_decay_loss += l2_decay * pg.params_[j + pg.params_idx] * pg.params_[j + pg.params_idx] / 2; // accumulate weight decay loss
				//l1_decay_loss += l1_decay * Math.Abs(pg.params_[j + pg.params_idx]);
				float l1grad = l1_decay * (params_[j + params_idx] > 0 ? 1 : -1);
				float l2grad = l2_decay * (params_[j + params_idx]);

				float gij = (l2grad + l1grad + grads_[j + grads_idx]) * oneBatchSize; // raw batch gradient

				// momentum update
				float dx = this.momentum * gsum[j] - this.learning_rate * gij; // step
				if (dx > 100) { 
				throw new Exception();
				}
				gsum[j] = dx; // back this up for next iteration of momentum
				params_[j + params_idx] += dx; // apply corrected gradient

				grads_[j + grads_idx] = 0.0f; // zero out gradient so that we can begin accumulating anew
			}
		}
	}

					

	//	public class AaaTrainer
	//	{
	//		public class Option
	//		{
	//			public float learning_rate = 0.01f;
	//			public float l1_decay = 0.0f;
	//			public float l2_decay = 0.0f;
	//			public int batch_size = 1;
	//			public string method = "sgd";
	//			public float momentum = 0.9f;
	//			public float ro = 0.95f;
	//			public float eps = 1e-6f;
	//		}
	//
	//		public float learning_rate;
	//		public float l1_decay;
	//		public float l2_decay;
	//		public int batch_size;
	//		public string method;
	//		public float momentum;
	//		public float ro;
	//		public float eps;
	//		public int k = 0; //int?
	//		public Net net;
	//		public void Trainer(Net net, Option options)
	//		{
	//
	//			this.net = net;
	//
	//			this.learning_rate = options.learning_rate;
	//			this.l1_decay = options.l1_decay;
	//			this.l2_decay = options.l2_decay;
	//			this.batch_size = options.batch_size;
	//			this.method = options.method; // sgd/adagrad/adadelta/windowgrad
	//
	//			this.momentum = options.momentum;
	//			this.ro = options.ro; // used in adadelta
	//			this.eps = options.eps; // used in adadelta
	//
	//			this.k = 0; // iteration counter
	//		}
	//
	//		public class Report
	//		{
	//			public float l2_decay_loss;
	//			public float l1_decay_loss;
	//			public float cost_loss;
	//			public float softmax_loss;
	//			public float loss;
	//		}
	//
	//		
	//		private void endofBatch(int batchSize)
	//		{
	//			//float one_batch_size = 1.0f / batchSize;
	//			//
	//			//List<ParamsAndGrads> pglist = this.net.getParamsAndGrads();
	//			//
	//			if (this.method == "adagrad")
	//			{
	//				//1
	//				// perform an update for all sets of weights
	//				//for (int i = 0; i < pglist.Count; i++)
	//				//{
	//				//	ParamsAndGrads pg = pglist[i]; // param, gradient, other options in future (custom learning rate etc)
	//				//
	//				//	// learning rate for some parameters.
	//				//	float l2_decay_mul = pg.l2_decay_mul;//typeof pg.l2_decay_mul != "undefined" ? pg.l2_decay_mul : 1.0;
	//				//	float l1_decay_mul = pg.l1_decay_mul;//typeof pg.l1_decay_mul != "undefined" ? pg.l1_decay_mul : 1.0;
	//				//	float l2_decay = this.l2_decay * l2_decay_mul;
	//				//	float l1_decay = this.l1_decay * l1_decay_mul;
	//				//	for (int j = 0; j < pg.params_size; j++)
	//				//	{
	//				//		//l2_decay_loss += l2_decay * pg.params_[j + pg.params_idx] * pg.params_[j + pg.params_idx] / 2; // accumulate weight decay loss
	//				//		//l1_decay_loss += l1_decay * Math.Abs(pg.params_[j + pg.params_idx]);
	//				//		float l1grad = l1_decay * (pg.params_[j] > 0 ? 1 : -1);
	//				//		float l2grad = l2_decay * (pg.params_[j]);
	//				//
	//				//		float gij = (l2grad + l1grad + pg.grads_[j]) * one_batch_size; // raw batch gradient
	//				//
	//				//		// adagrad update
	//				//		pg.gsum[j] = pg.gsum[j] + gij * gij;
	//				//		float dx = -this.learning_rate / (float)Math.Sqrt(pg.gsum[j] + this.eps) * gij;
	//				//		pg.params_[j] += dx;
	//				//
	//				//		pg.grads_[j] = 0.0f; // zero out gradient so that we can begin accumulating anew
	//				//	}
	//				//}
	//			}
	//			else if (this.method == "windowgrad")
	//			{
	//
	//				//2
	//				// perform an update for all sets of weights
	//				//for (int i = 0; i < pglist.Count; i++)
	//				//{
	//				//	ParamsAndGrads pg = pglist[i]; // param, gradient, other options in future (custom learning rate etc)
	//				//
	//				//	// learning rate for some parameters.
	//				//	float l2_decay_mul = pg.l2_decay_mul;//typeof pg.l2_decay_mul != "undefined" ? pg.l2_decay_mul : 1.0;
	//				//	float l1_decay_mul = pg.l1_decay_mul;//typeof pg.l1_decay_mul != "undefined" ? pg.l1_decay_mul : 1.0;
	//				//	float l2_decay = this.l2_decay * l2_decay_mul;
	//				//	float l1_decay = this.l1_decay * l1_decay_mul;
	//				//	for (int j = 0; j < pg.params_size; j++)
	//				//	{
	//				//		//l2_decay_loss += l2_decay * pg.params_[j + pg.params_idx] * pg.params_[j + pg.params_idx] / 2; // accumulate weight decay loss
	//				//		//l1_decay_loss += l1_decay * Math.Abs(pg.params_[j + pg.params_idx]);
	//				//		float l1grad = l1_decay * (pg.params_[j] > 0 ? 1 : -1);
	//				//		float l2grad = l2_decay * (pg.params_[j]);
	//				//
	//				//		float gij = (l2grad + l1grad + pg.grads_[j]) * one_batch_size; // raw batch gradient
	//				//
	//				//		// this is adagrad but with a moving window weighted average
	//				//		// so the gradient is not accumulated over the entire history of the run. 
	//				//		// it's also referred to as Idea #1 in Zeiler paper on Adadelta. Seems reasonable to me!
	//				//		pg.gsum[j] = this.ro * pg.gsum[j] + (1 - this.ro) * gij * gij;
	//				//		float dx = -this.learning_rate / (float)Math.Sqrt(pg.gsum[j] + this.eps) * gij; // eps added for better conditioning
	//				//		pg.params_[j] += dx;
	//				//
	//				//		pg.grads_[j] = 0.0f; // zero out gradient so that we can begin accumulating anew
	//				//	}
	//				//}
	//			}
	//			else if (this.method == "adadelta")
	//			{
	//
	////				//3
	////				// perform an update for all sets of weights
	////				for (int i = 0; i < pglist.Count; i++)//Parallel.For(0, pglist.Count, (i) =>//
	////				{
	////					ParamsAndGrads pg = pglist[i]; // param, gradient, other options in future (custom learning rate etc)
	////
	////					// learning rate for some parameters.
	////					float l2_decay_mul = pg.l2_decay_mul;//typeof pg.l2_decay_mul != "undefined" ? pg.l2_decay_mul : 1.0;
	////					float l1_decay_mul = pg.l1_decay_mul;//typeof pg.l1_decay_mul != "undefined" ? pg.l1_decay_mul : 1.0;
	////					float l2_decay = this.l2_decay * l2_decay_mul;
	////					float l1_decay = this.l1_decay * l1_decay_mul;
	////					for (int j = 0; j < pg.params_size; j++)
	////					{
	////						//l2_decay_loss += l2_decay * pg.params_[j + pg.params_idx] * pg.params_[j + pg.params_idx] / 2; // accumulate weight decay loss
	////						//l1_decay_loss += l1_decay * Math.Abs(pg.params_[j + pg.params_idx]);
	////						float l1grad = l1_decay * (pg.params_[j + pg.params_idx] > 0 ? 1 : -1);
	////						float l2grad = l2_decay * (pg.params_[j + pg.params_idx]);
	////
	////						float gij = (l2grad + l1grad + pg.grads_[j + pg.grads_idx]) * one_batch_size; // raw batch gradient
	////
	////						// assume adadelta if not sgd or adagrad
	////						pg.gsum[j] = this.ro * pg.gsum[j] + (1 - this.ro) * gij * gij;
	////						float dx = -(float)Math.Sqrt((pg.xsum[j] + this.eps) / (pg.gsum[j] + this.eps)) * gij;
	////						pg.xsum[j] = this.ro * pg.xsum[j] + (1 - this.ro) * dx * dx; // yes, xsum lags behind gsum by 1.
	////						pg.params_[j + pg.params_idx] += dx;
	////
	////						pg.grads_[j + pg.grads_idx] = 0.0f; // zero out gradient so that we can begin accumulating anew
	////					}
	////				}//);
	//			}
	//			else
	//			{
	//				// assume SGD
	//				if (this.momentum > 0.0)
	//				{
	////					//4
	////					// perform an update for all sets of weights
	////					for (int i = 0; i < pglist.Count; i++)
	////					{
	////						ParamsAndGrads pg = pglist[i]; // param, gradient, other options in future (custom learning rate etc)
	////				
	////						// learning rate for some parameters.
	////						float l2_decay_mul = pg.l2_decay_mul;//typeof pg.l2_decay_mul != "undefined" ? pg.l2_decay_mul : 1.0;
	////						float l1_decay_mul = pg.l1_decay_mul;//typeof pg.l1_decay_mul != "undefined" ? pg.l1_decay_mul : 1.0;
	////						float l2_decay = this.l2_decay * l2_decay_mul;
	////						float l1_decay = this.l1_decay * l1_decay_mul;
	////						for (int j = 0; j < pg.params_size; j++)
	////						{
	////							//l2_decay_loss += l2_decay * pg.params_[j + pg.params_idx] * pg.params_[j + pg.params_idx] / 2; // accumulate weight decay loss
	////							//l1_decay_loss += l1_decay * Math.Abs(pg.params_[j + pg.params_idx]);
	////							float l1grad = l1_decay * (pg.params_[j + pg.params_idx] > 0 ? 1 : -1);
	////							float l2grad = l2_decay * (pg.params_[j + pg.params_idx]);
	////
	////							float gij = (l2grad + l1grad + pg.grads_[j + pg.grads_idx]) * one_batch_size; // raw batch gradient
	////				
	////							// momentum update
	////							float dx = this.momentum * pg.gsum[j] - this.learning_rate * gij; // step
	////							pg.gsum[j] = dx; // back this up for next iteration of momentum
	////							pg.params_[j + pg.params_idx] += dx; // apply corrected gradient
	////
	////							pg.grads_[j + pg.grads_idx] = 0.0f; // zero out gradient so that we can begin accumulating anew
	////						}
	////					}
	//				}
	//				else
	//				{
	//					//5
	//					// perform an update for all sets of weights
	////					for (int i = 0; i < pglist.Count; i++)
	////					{
	////						ParamsAndGrads pg = pglist[i]; // param, gradient, other options in future (custom learning rate etc)
	////				
	////						// learning rate for some parameters.
	////						float l2_decay_mul = pg.l2_decay_mul;//typeof pg.l2_decay_mul != "undefined" ? pg.l2_decay_mul : 1.0;
	////						float l1_decay_mul = pg.l1_decay_mul;//typeof pg.l1_decay_mul != "undefined" ? pg.l1_decay_mul : 1.0;
	////						float l2_decay = this.l2_decay * l2_decay_mul;
	////						float l1_decay = this.l1_decay * l1_decay_mul;
	////						for (int j = 0; j < pg.params_size; j++)
	////						{
	////							//l2_decay_loss += l2_decay * pg.params_[j + pg.params_idx] * pg.params_[j + pg.params_idx] / 2; // accumulate weight decay loss
	////							//l1_decay_loss += l1_decay * Math.Abs(pg.params_[j + pg.params_idx]);
	////							float l1grad = l1_decay * (pg.params_[j + pg.params_idx] > 0 ? 1 : -1);
	////							float l2grad = l2_decay * (pg.params_[j + pg.params_idx]);
	////
	////							float gij = (l2grad + l1grad + pg.grads_[j + pg.grads_idx]) * one_batch_size; // raw batch gradient
	////				
	////							float tmp = -this.learning_rate * gij;
	////							// momentum update
	////							float dx = this.momentum * pg.gsum[j] + tmp; // step
	////							pg.gsum[j] = dx; // back this up for next iteration of momentum
	////							pg.params_[j + pg.params_idx] += dx; // apply corrected gradient
	////							// vanilla sgd
	////							pg.params_[j] += tmp;
	////
	////							pg.grads_[j + pg.grads_idx] = 0.0f; // zero out gradient so that we can begin accumulating anew
	////						}
	////					}
	//				}
	//			}
	//
	//
	//			//long t3 = sw.ElapsedTicks;
	//			//
	//			//long tt = t1 + t2 + t3;
	//			//tt++;
	//			//Console.WriteLine(tt);
	//		}
	//
	//	}
	//
}
