using System;
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
		//public override void save(TextWriter s)
		//{
		//	bias_w.save(s);
		//	filters_w.save(s);
		//}
		public override void load(TextReader s)
		{
			bias_w.load(s);
			filters_w.load(s);
		}
		public override void load(BinaryReader s)
		{
			bias_w.load(s);
			filters_w.load(s);
		}
		//Vol[] filters;
		public MyFloat filters_w;

		public MyFloat bias_w;

		public int num_inputs;

		public ActivationLayer act;

		public FullyConnLayer(int outDepth = 0, ActivationLayer act = null)
		{

			// required
			// ok fine we will allow 'filters' as the word as well
			this.out_depth = outDepth;

			// optional 
			//this.l1_decay_mul = opt.l1_decay_mul;
			//this.l2_decay_mul = opt.l2_decay_mul;

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

			//filters = new Vol[out_depth];
			//filters[i] = new Vol(1, 1, this.num_inputs, null);
			filters_w = new MyFloat(num_inputs * out_depth);
			bias_w = new MyFloat(out_depth);

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
