#define CUDA


using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using Lib;

namespace ConvNet
{

	public class FullyConnLayer : TrainableLayer
	{
		public string name;
		public override void save(TextWriter s)
		{
			filters_w.save(s);
			bias_w.save(s);
		}
		public override void load(TextReader s)
		{
			filters_w.load(s);
			bias_w.load(s);
		}
		//Vol[] filters;
		MyFloat filters_w;

		MyFloat[] filters_gsum; //[]?
		MyFloat[] filters_xsum; //[]?

		MyFloat bias_w;


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
			filters_w = new MyFloat(num_inputs * out_depth);
			Vol.init(filters_w, num_inputs * out_depth, null);



			bias_w = new MyFloat(out_depth);
			Vol.init(bias_w, out_depth, bias_pref);

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
			TrainableInstance ins = new TrainableInstance();
			ins.out_act = new Vol(1, 1, this.out_depth, 0.0f);

			ins.filters_dw = new MyFloat(num_inputs * out_depth);
			Vol.init(ins.filters_dw, num_inputs * out_depth, 0.0f);

			ins.bias_dw = new MyFloat(out_depth);
			Vol.init(ins.bias_dw, out_depth, 0.0f);

			if (act != null)
			{
				ins.actIns = act.getInstance();
			}

			return ins;
		}

		//#if CUDA
		[DllImport("CUDA2Lib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern int FCFWD(
			int x,
			int y,
			int out_depth,
			int num_inputs,
			IntPtr p_in_act_w,
			IntPtr p_filters_w,
			IntPtr p_bias_w,
			IntPtr p_out_act_w
		);
		//#else
		[DllImport("CPULib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern void FCFWD(
			int out_depth,
			int num_inputs,
			IntPtr p_in_act_w,
			IntPtr p_filters_w,
			IntPtr p_bias_w,
			IntPtr p_out_act_w
		);
		//#endif

		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern IntPtr GK_FCBWD(IntPtr oclobjects);
		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern int RK_FCBWD(
			IntPtr oclobjects,
			IntPtr kernel,

			int out_depth,
			int num_inputs,
			IntPtr p_in_act_w,
			IntPtr p_filters_w,
			IntPtr p_in_act_dw,
			IntPtr p_out_act_dw,
			IntPtr p_filters_dw,
			IntPtr p_bias_dw
		);

		public override Vol forward(Instance instance, Vol V)
		{
			instance.in_act = V;

			//16*16: 1700,2170,2399 /  5
			//if (OpenCL.oclobjects != IntPtr.Zero)
			//{
			//	long t1, t2, t3;
			//	Stopwatch sw = new Stopwatch();
			//	sw.Start();
			//	instance.in_act.w.copyToCLMEM();
			//	filters_w.copyToCLMEM();
			//	bias_w.copyToCLMEM();
			//	//instance.out_act.w.copyToCLMEM();
			//	sw.Stop();
			//	t1 = sw.ElapsedTicks;
			//	sw.Restart();
			//	int err = RK_FCFWD(OpenCL.oclobjects, kernel_FCFWD,x,y, out_depth, num_inputs, instance.in_act.w.p_cl_mem, filters_w.p_cl_mem, bias_w.p_cl_mem, instance.out_act.w.p_cl_mem);
			//	sw.Stop();
			//	t2 = sw.ElapsedTicks;
			//    sw.Restart();
			//	//instance.in_act.w.copyFromCLMEM();
			//	//filters_w.copyFromCLMEM();
			//	//bias_w.copyFromCLMEM();
			//	instance.out_act.w.copyFromCLMEM();
			//	sw.Stop();
			//	t3 = sw.ElapsedTicks;
			//
			//	Console.WriteLine(t1 + "," + t2 + "," + t3);
			//
			//}
			//else
			{
				//long aa = Stopwatch.Frequency;
				//long t1 = 0, t2 = 0, t3 = 0;
				//Stopwatch sw = new Stopwatch();
				//sw.Start();
				FCFWD(1, 1, out_depth, num_inputs, instance.in_act.w.getHostMemPointReadOnly(), filters_w.getHostMemPointReadOnly(), bias_w.getHostMemPointReadOnly(), instance.out_act.w.getHostMemPointReadWrite());
				//sw.Stop();
				//t3 = sw.ElapsedTicks;

				//Console.WriteLine(t1 + "," + t2 + "," + t3);

			}

			if (act != null)
			{
				return act.forward(((TrainableInstance)instance).actIns, instance.out_act);
			}
			else
			{
				return instance.out_act;
			}
		}

		public Vol forward(Instance instance, Vol V, int x, int y, bool acc)
		{
			instance.in_act = V;

			//16*16: 1700,2170,2399 /  5
			if (acc)
			{
				long t1, t2, t3;
				Stopwatch sw = new Stopwatch();
				sw.Start();
				//instance.in_act.w.copyToDeviceMEM();
				//filters_w.copyToDeviceMEM();
				//bias_w.copyToDeviceMEM();

				var p_in_act = instance.in_act.w.getDeviceMemPointReadOnly();
				var p_filters_w = filters_w.getDeviceMemPointReadOnly();
				var p_bias_w = bias_w.getDeviceMemPointReadOnly();
				var p_out_act = instance.out_act.w.getDeviceMemPointReadWrite();

				sw.Stop();
				t1 = sw.ElapsedTicks;
				sw.Restart();
				int err = FCFWD(x, y, out_depth, num_inputs, p_in_act, p_filters_w, p_bias_w, p_out_act);
				Console.WriteLine(err);
				sw.Stop();
				t2 = sw.ElapsedTicks;
				sw.Restart();
				//instance.in_act.w.copyFromCLMEM();
				//filters_w.copyFromCLMEM();
				//bias_w.copyFromCLMEM();
				//instance.out_act.w.copyFromDeviceMEM();
				sw.Stop();
				t3 = sw.ElapsedTicks;

				Console.WriteLine(t1 + "," + t2 + "," + t3);
				Util.log(t1 + "," + t2 + "," + t3);

			}
			else
			{

				long aa = Stopwatch.Frequency;
				long t1 = 0, t2 = 0, t3 = 0;
				Stopwatch sw = new Stopwatch();
				sw.Start();
				FCFWD(out_depth, num_inputs, instance.in_act.w.getHostMemPointReadOnly(), filters_w.getHostMemPointReadOnly(), bias_w.getHostMemPointReadOnly(), instance.out_act.w.getHostMemPointReadWrite());
				sw.Stop();
				t3 = sw.ElapsedTicks;

				Console.WriteLine(t1 + "," + t2 + "," + t3);

			}

			if (act != null)
			{
				return act.forward(((TrainableInstance)instance).actIns, instance.out_act);
			}
			else
			{
				return instance.out_act;
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

		public override void backward(Instance instance)
		{
			if (noUpdate) return;

			TrainableInstance trainableInstance = ((TrainableInstance)instance);

			if (act != null)
			{
				act.backward(trainableInstance.actIns);
			}

			//if (false)//OpenCL.oclobjects != IntPtr.Zero)
			//{
			//	for (int i = 0; i < num_inputs; i++)
			//	{
			//		//in_act.dw[d] = 0;
			//		in_act.dw[i] = 0;
			//	}
			//	//check(in_act.w);
			//	//check(filters.w);
			//	//check(bias.w);
			//	//check(out_act.w);
			//	long t1, t2, t3;
			//	Stopwatch sw = new Stopwatch();
			//	sw.Start();
			//
			//	in_act.w.copyToCLMEM();
			//	filters_w.copyToCLMEM();
			//	in_act.dw.copyToCLMEM();
			//	out_act.dw.copyToCLMEM();
			//	filters_dw.copyToCLMEM();
			//	bias_dw.copyToCLMEM();
			//
			//	sw.Stop();
			//	t1 = sw.ElapsedTicks;
			//	sw.Start();
			//	int err = RK_FCBWD(OpenCL.oclobjects, kernel_FCBWD, out_depth, num_inputs, in_act.w.p_cl_mem, filters_w.p_cl_mem, in_act.dw.p_cl_mem, out_act.dw.p_cl_mem, filters_dw.p_cl_mem, bias_dw.p_cl_mem);
			//	sw.Stop();
			//	t2 = sw.ElapsedTicks;
			//	sw.Start();
			//
			//	in_act.w.copyFromCLMEM();
			//	filters_w.copyToCLMEM();
			//	in_act.dw.copyToCLMEM();
			//	out_act.dw.copyToCLMEM();
			//	filters_dw.copyToCLMEM();
			//	bias_dw.copyToCLMEM();
			//
			//	sw.Stop();
			//	t3 = sw.ElapsedTicks;
			//	//sw.Start();
			//
			//	//Console.WriteLine(t1 + "," + t2 + "," + t3);
			//	//check(in_act.w);
			//	//check(filters.w);
			//	//check(bias.w);
			//	//check(out_act.w);
			//	//Console.WriteLine(err);
			//}
			//else
			{
				//long aa = Stopwatch.Frequency;
				//long t1 = 0, t2 = 0, t3 = 0;
				//Stopwatch sw = new Stopwatch();
				//sw.Start();
				FCBWD(out_depth, num_inputs, instance.in_act.w.getHostMemPointReadWrite(), filters_w.getHostMemPointReadWrite(), instance.in_act.dw.getHostMemPointReadWrite(), instance.out_act.dw.getHostMemPointReadWrite(), trainableInstance.filters_dw.getHostMemPointReadWrite(), trainableInstance.bias_dw.getHostMemPointReadWrite());
				//sw.Stop();
				//t3 = sw.ElapsedTicks;
				//if (t3 < -10000)
				//{
				//	throw new Exception("slow");
				//}
			}
		}
		public bool noUpdate = false;
		public override void train(TrainableInstance instance, Trainer trainer, float oneBatchSize)
		{
			if (noUpdate) return;
			for (int i = 0; i < this.out_depth; i++)
			{
				//int iw = i * this.num_inputs;
				//response.Add(new ParamsAndGrads() { Params = this.filters[i].w, grads = this.filters[i].dw, l1_decay_mul = this.l1_decay_mul, l2_decay_mul = this.l2_decay_mul });
				trainer.train(
					num_inputs,//params_size = 
					this.filters_w,//params_ = 
					instance.filters_dw,//grads_ = 
					i * num_inputs,//params_idx = 
					i * num_inputs,//grads_idx = 
					filters_gsum[i],//gsum = 
					filters_xsum[i],//xsum = 
					this.l1_decay_mul,//l1_decay_mul = 
					this.l2_decay_mul,//l2_decay_mul = 
					oneBatchSize
				);
			}
			//response.Add(new ParamsAndGrads() { Params = this.biases.w, grads = this.biases.dw, l1_decay_mul = 0.0f, l2_decay_mul = 0.0f });
			trainer.train(
				out_depth,//params_size = 
				this.bias_w,//params_ = 
				instance.bias_dw,//grads_ = 
				0,//params_idx = 
				0,//grads_idx = 
				bias_gsum,//gsum = 
				bias_xsum,//xsum = 
				0.0f,//l1_decay_mul = 
				0.0f,//l2_decay_mul = 
				oneBatchSize
			);
		}
	}
}
