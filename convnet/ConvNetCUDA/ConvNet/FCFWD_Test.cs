using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ConvNet
{
    public class FCFWD_Test
    {

        static int cnt = 256;

        public static void Test()
        {

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

            }

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
                        //Console.WriteLine("--ok-- :" + i + "[" + (insList[0].out_act.w[i] - insList[1].out_act.w[i]) + "]" + "," + insList[0].out_act.w[i] + "---" + insList[1].out_act.w[i]);
                    }
                }

            }

            Console.WriteLine("ok");

            Console.ReadLine();
            return;

        }

    }
}
