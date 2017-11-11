using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using Lib;

namespace ConvNet
{
	class Program
	{
		static int cnt = 2048;

		public static void MakeData()
		{
			var inp = new InputLayer(1, 1, cnt);
			var fc = new FullyConnLayer(num_neurons: cnt, bias_pref: 0.1f);
			fc.in_layer = inp;
			fc.init();

			var ins = fc.getInstance();
			ins.out_act = new Vol(1, 1, cnt, 9.9f);


			Util.save("testData.txt", (s) =>
			{
				fc.save(s);
				ins.out_act.save(s);
			});

		}

		public static void CUDATest()
		{
			int n = init(0);

			var inp = new InputLayer(1, 1, cnt);
			var fc = new FullyConnLayer(num_neurons: cnt, bias_pref: 0.1f);
			fc.in_layer = inp;
			fc.init();

			Layer.Instance[] insList = new Layer.Instance[2];
			insList[0] = fc.getInstance();
			insList[1] = fc.getInstance();

			insList[0].out_act = new Vol(1, 1, cnt, 9.9f);
			insList[1].out_act = new Vol(1, 1, cnt, 9.9f);

			var in_act = new Vol(1, 1, cnt, null);

			Console.WriteLine("start");

			//Util.load(@"..\testData.txt", (s) =>
			//{
			//	fc.load(s);
			//	insList[0].out_act.load(s);
			//});
			//Util.load(@"..\testData.txt", (s) =>
			//{
			//	fc.load(s);
			//	insList[1].out_act.load(s);
			//});


			StringBuilder[] sb = new StringBuilder[2];
			for (int i = 0; i < 2; i++)
			{
				sb[i] = new StringBuilder();
			}

			StringBuilder sb0 = new StringBuilder();

			int[] idx = new int[4];
			Random r = new Random();

			idx[0] = (int)(r.NextDouble() * cnt);
			idx[1] = (int)(r.NextDouble() * cnt);
			idx[2] = (int)(r.NextDouble() * cnt);
			idx[3] = (int)(r.NextDouble() * cnt);

			for (int i = 0; i < 20; i++)
			{
				Vol o;
				if (i >= 10)
				{
					o = fc.forward(insList[0], in_act, i, cnt, true);
				}
				else
				{
					o = fc.forward(insList[1], in_act, i, cnt, false);
				}

				//if (i == 3)
				//{
				//	for (int j = 0; j < cnt; j++)
				//	{
				//		sb[0].Append(o.w[j] + ",");
				//	}
				//	sb0.AppendLine(o.w[idx[0]] + "," + o.w[idx[1]] + "," + o.w[idx[2]] + "," + o.w[idx[3]]);
				//}
				//if (i == 16)
				//{
				//	for (int j = 0; j < cnt; j++)
				//	{
				//		sb[1].Append(o.w[j] + ",");
				//	}
				//	sb0.AppendLine(o.w[idx[0]] + "," + o.w[idx[1]] + "," + o.w[idx[2]] + "," + o.w[idx[3]]);
				//}
			}

			Util.save("1.txt", (s) =>
			{
				s.Write(sb[0].ToString());
			});
			Util.save("2.txt", (s) =>
			{
				s.Write(sb[1].ToString());
			});

			for (int i = 0; i < insList[0].out_act.w.size; i++)
			{
				if (insList[0].out_act.w[i] != insList[1].out_act.w[i])
				{
					if (insList[0].out_act.w[i] - insList[1].out_act.w[i] > 0.000001)
					{
						Console.WriteLine("--err--:" + i + "[" + (insList[0].out_act.w[i] - insList[1].out_act.w[i]) + "]" + "," + insList[0].out_act.w[i] + "---" + insList[1].out_act.w[i]);
					}
					else if (insList[0].out_act.w[i] - insList[1].out_act.w[i] < -0.000001)
					{
						Console.WriteLine("--err-- :" + i + "[" + (insList[0].out_act.w[i] - insList[1].out_act.w[i]) + "]" + "," + insList[0].out_act.w[i] + "---" + insList[1].out_act.w[i]);
					}
					else
					{
						Console.WriteLine("--ok-- :" + i + "[" + (insList[0].out_act.w[i] - insList[1].out_act.w[i]) + "]" + "," + insList[0].out_act.w[i] + "---" + insList[1].out_act.w[i]);
					}
				}

			}

			Console.WriteLine("ok");
			Console.WriteLine(sb0.ToString());

			Console.ReadLine();
			return;

		}

		public static void Verify()
		{

		}

		[DllImport("cuda2Lib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern int init(int device);

		static void Main(string[] args)
		{
			//MakeData();

			CUDATest();

			Verify();
		}
	}
}
