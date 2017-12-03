﻿using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace ConvNet
{

	public class FullyConnLayer : TrainableLayer
	{
		public string name;
		public override void save(TextWriter s)
		{
			bias_w.save(s);
			filters_w.save(s);
		}
		public override void load(TextReader s)
		{
			bias_w.load(s);
			filters_w.load(s);
		}
		//Vol[] filters;
		public MyFloat filters_w;

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

			if (act != null)
			{
				ins.actIns = act.getInstance();
			}

			return ins;
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

		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern void SSE_FCFWD(
			int out_depth,
			int num_inputs,
			IntPtr p_in_act_w,
			IntPtr p_filters_w,
			IntPtr p_bias_w,
			IntPtr p_out_act_w
		);


		public override Vol forward(Instance ins, Vol V)
		{
			ins.in_act = V;

				if (Util.useSSE && num_inputs % 8 == 0)
				{
					SSE_FCFWD(
						out_depth,
						num_inputs,
						ins.in_act.w.p,
						filters_w.p,
						bias_w.p,
						ins.out_act.w.p);
				}
				else
				{
					FCFWD(
						out_depth,
						num_inputs,
						ins.in_act.w.p,
						filters_w.p,
						bias_w.p,
						ins.out_act.w.p);
				}

			if (act != null)
			{
				return act.forward(((TrainableInstance)ins).actIns, ins.out_act);
			}
			else
			{
				return ins.out_act;
			}
		}



	}
}