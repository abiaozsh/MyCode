using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
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
			float loss = 0;
			//for (int k = 0; k < 5; k++)
			{
				Random r = new Random();
				for (int n = 0; n < 10000; n++)
				{
					loss = regl1Net.train((int)(r.NextDouble() * Cifar.imgCount));

					Text = n + "," + loss;
					Application.DoEvents();
					if (n % 10 == 0)
					{
						test();
						vis();
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
				//vis();
			}

		}
		public void vis()
		{
			float scale;
			float.TryParse(textBox1.Text, out scale);
			string dir = @"vis\";
			for (int i = 0; i < 32; i++)
			{
				regl1Net.cv1.visRGB(i, scale).Save(dir + i + ".bmp");
			}
		}

		public void test()
		{
			Bitmap b = new Bitmap(300, 300);
			Graphics g = Graphics.FromImage(b);
			Random r = new Random();
			for (int i = 0; i < 10; i++)
			{
				int n = (int)(r.NextDouble() * 60000);
				int x = (int)(r.NextDouble() * 28);
				int y = (int)(r.NextDouble() * 28);

				Vol v = Cifar.Lv1TrainNet.get4x4(n, x, y);
				g.DrawImage(v.visRGB(), 10 + i * 10, 10);
				Vol ret = regl1Net.forward(v);
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

	}
}
