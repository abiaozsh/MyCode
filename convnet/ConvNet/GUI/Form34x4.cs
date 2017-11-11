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
		Cifar.Lv2TrainNet regl2Net = new Cifar.Lv2TrainNet();
		Cifar.RegNet regNet = new Cifar.RegNet();

		private void Form3_Load(object sender, EventArgs e)
		{
			cifar.init();
			regl1Net.init();
			regl2Net.init();
			regNet.init();
		}
		bool stop = false;
		private void button1_Click(object sender, EventArgs e)
		{

		}

		private void button2_Click(object sender, EventArgs e)
		{
			stop = true;
		}



		private void button3_Click(object sender, EventArgs e)
		{
			while (true)
			{
				Net.Instance[] ins = new Net.Instance[10];
				for (int i = 0; i < 10; i++)
				{
					ins[i] = regNet.getInstance();
					ins[i].inact = new Vol(32, 32, 3, 0);
				}
				for (int i = 0; i < 10; i++)
				{
					regNet.train(ins[i], 0);
					regNet.train(ins[i], 1);
				}
				regNet.endofBatch(ins, 20);

				{
					Cifar.getImg(ins[0].inact, 0);
					var ret = regNet.forward(ins[0], ins[0].inact);

					pictureBox1.Image = ins[0].inact.visRGB(255);
					pictureBox2.Image = ret.visRGB(255);
				}

				{
					Cifar.getImg(ins[0].inact, 1);
					var ret = regNet.forward(ins[0], ins[0].inact);

					pictureBox3.Image = ins[0].inact.visRGB(255);
					pictureBox4.Image = ret.visRGB(255);
				}
				Application.DoEvents();
			}
		}

		private void button5_Click(object sender, EventArgs e)
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
					loss += regl1Net.train(insList[i], (int)(r.NextDouble() * Cifar.Count));
				}
				);
				n++;
				if (n == Cifar.Count * 2)
				{
					break;
				}

				regl1Net.endofBatch(insList, (32-4) * (32-4) * 10);

				Text = n + "," + loss;
				Application.DoEvents();
				if (n % 10 == 0)
				{
					test4();
					vis4();
					sw.Stop();
					this.textBox2.Text = sw.Elapsed.ToString();
					Util.save("CifarCV1.txt", (s) =>
					{
						regl1Net.cv1.save(s);
					});
					Util.save("CifarCV1_UFC.txt", (s) =>
					{
						regl1Net.ufc.save(s);
					});

					sw.Restart();
				}
				if (stop) { stop = false; break; }
			}
		}


		public void vis4()
		{
			float scale;
			float.TryParse(textBox1.Text, out scale);
			string dir = @"vis\";
			for (int i = 0; i < Cifar.Lv1TrainNet.lv1Filters; i++)
			{
				regl1Net.cv1.visRGB(i, scale).Save(dir + i + ".bmp");
			}
		}

		public void test4()
		{
			Net.Instance ins = regl1Net.getInstance();
			ins.inact = new Vol(4, 4, 3, 0.0f);
			Bitmap b = new Bitmap(300, 300);
			Graphics g = Graphics.FromImage(b);
			Random r = new Random();
			for (int i = 0; i < 10; i++)
			{
				int n = (int)(r.NextDouble() * 60000);
				int x = (int)(r.NextDouble() * (32-4));
				int y = (int)(r.NextDouble() * (32 - 4));

				Cifar.Lv1TrainNet.get4x4(ins.inact, n, x, y);
				g.DrawImage(ins.inact.visRGB(255), 10 + i * 10, 10);
				Vol ret = regl1Net.forward(ins, ins.inact);
				g.DrawImage(ret.visRGB(255), 10 + i * 10, 20);
			}
			g.Flush();
			g.Dispose();

			pictureBox7.Image = b;
		}

		private void button6_Click(object sender, EventArgs e)
		{
			Net.Instance[] insList = new Net.Instance[10];
			for (int i = 0; i < 10; i++)
			{
				insList[i] = regl2Net.getInstance();
				insList[i].inact = new Vol(8, 8, 3, 0.0f);
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
					int sample = 0;
					loss += regl2Net.train(insList[i], (int)(r.NextDouble() * Cifar.Count));
				}
				);
				n++;
				if (n >= Cifar.Count * 2)
				{
					break;
				}

				regl2Net.endofBatch(insList, (32 - 8) * (32 - 8) * 10);

				Text = n + "," + loss;
				Application.DoEvents();
				if (n % 10 == 0)
				{
					test8();
					vis8();
					sw.Stop();
					this.textBox2.Text = sw.Elapsed.ToString();

					sw.Restart();


					Util.save("CifarCV2.txt", (s) =>
					{
						regl2Net.cv2.save(s);
					});
					Util.save("CifarCV2_ufc.txt", (s) =>
					{
						regl2Net.ufc.save(s);
					});
				}
				if (stop) { stop = false; break; }
			}


		}

		public void vis8()
		{
			//float scale;
			//float.TryParse(textBox1.Text, out scale);
			string dir = @"vis\";
			float scale;
			float.TryParse(textBox1.Text, out scale);
			for (int i = 0; i < MNIST.Lv2TrainNet.Lv2filters; i++)
			{
				var v = regl2Net.vis(i);
				v.visRGB(scale).Save(dir + i + ".bmp");
			}
		}


		public void test8()
		{
			Net.Instance ins = regl2Net.getInstance();
			Vol v8 = new Vol(8, 8, 3, 0);
			Bitmap b = new Bitmap(300, 300);
			Graphics g = Graphics.FromImage(b);
			Random r = new Random();
			for (int i = 0; i < 10; i++)
			{
				int n = (int)(r.NextDouble() * 60000);
				int x = (int)(r.NextDouble() * (32-8));
				int y = (int)(r.NextDouble() * (32 - 8));


				Cifar.Lv2TrainNet.get8x8(v8,n, x, y);
				g.DrawImage(v8.visRGB(255), 40 + i * 20, 30);

				Vol ret = regl2Net.forward(ins, v8);
				g.DrawImage(ret.visRGB(255), 40 + i * 20, 50);
			}
			g.Flush();
			g.Dispose();

			pictureBox7.Image = b;
		}
	}
}
