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
	public partial class Form38x8 : Form
	{
		public Form38x8()
		{
			InitializeComponent();
		}

		int idx = 10;
		Cifar cifar = new Cifar();
		Cifar.RegNet regNet = new Cifar.RegNet();
		private void Form3_Load(object sender, EventArgs e)
		{
			cifar.init();
			regNet.init();
		}

		bool stop = false;
		private void button1_Click(object sender, EventArgs e)
		{
			for (int k = 0; k < 5000000; k++)
			{
				for (int n = 0; n < 1; n++)
				{
					for (int i = 0; i < 1; i++)
					{
						for (int j = 0; j < 1; j++)
						{
							regNet.train(n);//, i, j
						}
					}
				}
				Text = k + ",";
				Application.DoEvents();

				Bitmap b = new Bitmap(300, 300);
				Graphics g = Graphics.FromImage(b);
				Random r = new Random();
				for (int i = 0; i < 1; i++)
				{
					int n = 0;
					int x = (int)(r.NextDouble() * 24);
					int y = (int)(r.NextDouble() * 24);

					Vol v = Cifar.getImg(n);//Cifar.get8x8(n, x, y);
					g.DrawImage(v.visRGB(), 10 + i * 40, 10);
					Vol ret = regNet.forward(v);
					g.DrawImage(ret.visRGB(), 10 + i * 40, 50);
				}

				for (int i = 0; i < 1; i++)
				{
					int n = 1;
					int x = (int)(r.NextDouble() * 24);
					int y = (int)(r.NextDouble() * 24);

					Vol v = Cifar.getImg(n); ;//Cifar.get8x8(n, x, y);
					g.DrawImage(v.visRGB(), 10 + i * 40, 100);
					Vol ret = regNet.forward(v);
					g.DrawImage(ret.visRGB(), 10 + i * 40, 150);
				}
				g.Flush();
				g.Dispose();

				pictureBox7.Image = b;

				if (stop) { stop = false; break; }
			}

		}

		private void button2_Click(object sender, EventArgs e)
		{
			stop = true;
		}

		private void button4_Click(object sender, EventArgs e)
		{
			pictureBox1.Image = Cifar.Lv1TrainNet.get4x4(1, 0, 0).visRGB();
			pictureBox2.Image = Cifar.getBmp(1);
		}

		private void button3_Click(object sender, EventArgs e)
		{
			for (int k = 0; k < 5000000; k++)
			{
				for (int n = 0; n < 2; n++)
				{
					for (int i = 0; i < 1; i++)
					{
						for (int j = 0; j < 1; j++)
						{
							regNet.train(n);//, i, j
						}
					}
					Text = n + ",";
					Application.DoEvents();
				}

				Bitmap b = new Bitmap(300, 300);
				Graphics g = Graphics.FromImage(b);
				Random r = new Random();
				for (int i = 0; i < 1; i++)
				{
					int n = 0;
					int x = (int)(r.NextDouble() * 24);
					int y = (int)(r.NextDouble() * 24);

					Vol v = Cifar.getImg(n);//Cifar.get8x8(n, x, y);
					g.DrawImage(v.visRGB(), 10 + i * 40, 10);
					Vol ret = regNet.forward(v);
					g.DrawImage(ret.visRGB(), 10 + i * 40, 50);
				}

				for (int i = 0; i < 1; i++)
				{
					int n = 1;
					int x = (int)(r.NextDouble() * 24);
					int y = (int)(r.NextDouble() * 24);

					Vol v = Cifar.getImg(n); ;//Cifar.get8x8(n, x, y);
					g.DrawImage(v.visRGB(), 10 + i * 40, 100);
					Vol ret = regNet.forward(v);
					g.DrawImage(ret.visRGB(), 10 + i * 40, 150);
				}
				g.Flush();
				g.Dispose();

				pictureBox7.Image = b;

				if (stop) { stop = false; break; }
			}

		}
	}
}
