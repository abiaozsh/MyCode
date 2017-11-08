using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using ConvNet;

namespace GUI
{
	public partial class Form34x4 : Form
	{
		public Form34x4()
		{
			InitializeComponent();
		}

		Cifar cifar = new Cifar();
		Cifar.Lv1TrainNet regl1Net = new Cifar.Lv1TrainNet();
		private void Form3_Load(object sender, EventArgs e)
		{
			cifar.init();
			regl1Net.init();
		}
		bool stop = false;
		private void button1_Click(object sender, EventArgs e)
		{
			Net.Instance[] insList = new Net.Instance[10];
			for (int i = 0; i < 10; i++)
			{
				insList[i] = regl1Net.getInstance();
				insList[i].inact = new Vol(4, 4, 3, 0.0f);
			}

			float loss = 0;
			Stopwatch sw = new Stopwatch();
			sw.Start();
			int n = 0;
			while (true)
			{
				loss = 0;
				
				Parallel.For(0, 10, (i) =>
				//for (int i = 0; i < 10; i++)
				{
					Random r = new Random();
					loss += regl1Net.train(insList[i], (int)(r.NextDouble() * Cifar.imgCount));
				}
				);
				n ++;
				if (n == Cifar.imgCount * 2 / 10)
				{
					break;
				}

				regl1Net.endofBatch(insList, 28 * 28 * 10);

				Text = n + "," + loss;
				Application.DoEvents();
				if (n % 10 == 0)
				{
					test();
					vis();
					sw.Stop();
					this.textBox2.Text = sw.Elapsed.ToString();

					sw.Restart();
				}
				if (stop) { stop = false; break; }
			}

			Util.save("CifarCV1.txt", (s) =>
			{
				regl1Net.cv1.save(s);
			});
			Util.save("CifarUFC.txt", (s) =>
			{
				regl1Net.ufc.save(s);
			});

		}
		public void vis()
		{
			float scale;
			float.TryParse(textBox1.Text, out scale);
			string dir = @"vis\";
			for (int i = 0; i < regl1Net.lv1Filters; i++)
			{
				regl1Net.cv1.visRGB(i, scale).Save(dir + i + ".bmp");
			}
		}

		public void test()
		{
			Net.Instance ins = regl1Net.getInstance();
			ins.inact = new Vol(4, 4, 3, 0.0f);
			Bitmap b = new Bitmap(300, 300);
			Graphics g = Graphics.FromImage(b);
			Random r = new Random();
			for (int i = 0; i < 10; i++)
			{
				int n = (int)(r.NextDouble() * 60000);
				int x = (int)(r.NextDouble() * 28);
				int y = (int)(r.NextDouble() * 28);

				Cifar.Lv1TrainNet.get4x4(ins.inact, n, x, y);
				g.DrawImage(ins.inact.visRGB(), 10 + i * 10, 10);
				Vol ret = regl1Net.forward(ins, ins.inact);
				g.DrawImage(ret.visRGB(), 10 + i * 10, 20);
			}
			g.Flush();
			g.Dispose();

			pictureBox7.Image = b;
		}

		private void button2_Click(object sender, EventArgs e)
		{
			stop = true;
		}


		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern void SSETest(
			int num_inputs,
			IntPtr p_in_act_w,
			IntPtr p_filters_w,
			IntPtr p_out_act_w
		);
		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern void SSETest2(
			int num_inputs,
			IntPtr p_in_act_w,
			IntPtr p_filters_w,
			IntPtr p_out_act_w
		);


		private void button3_Click(object sender, EventArgs e)
		{
		}

	}
}
