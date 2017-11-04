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
		Cifar.Reg1LBNet regNet = new Cifar.Reg1LBNet();
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
					for (int i = 0; i < 24; i++)
					{
						for (int j = 0; j < 24; j++)
						{
							regNet.train(n, i, j);
						}
						Text = n + "," + i;
						Application.DoEvents();
					}
				}

				Bitmap b = new Bitmap(300, 300);
				Graphics g = Graphics.FromImage(b);
				Random r = new Random();
				for (int i = 0; i < 10; i++)
				{
					int n = 0;
					int x = (int)(r.NextDouble() * 24);
					int y = (int)(r.NextDouble() * 24);

					Vol v = Cifar.get8x8(n, x, y);
					g.DrawImage(v.visRGB(), 12 + i * 12, 12);
					Vol ret = regNet.forward(v);
					g.DrawImage(ret.visRGB(), 12 + i * 12, 22);
				}

				for (int i = 0; i < 10; i++)
				{
					int n = 1;
					int x = (int)(r.NextDouble() * 24);
					int y = (int)(r.NextDouble() * 24);

					Vol v = Cifar.get8x8(n, x, y);
					g.DrawImage(v.visRGB(), 12 + i * 12, 42);
					Vol ret = regNet.forward(v);
					g.DrawImage(ret.visRGB(), 12 + i * 12, 52);
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
			pictureBox1.Image = Cifar.get4x4(1, 0, 0).visRGB();
			pictureBox2.Image = Cifar.getBmp(1);
		}
	}
}
