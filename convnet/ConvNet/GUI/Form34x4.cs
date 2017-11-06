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

		int idx = 10;
		Cifar cifar = new Cifar();
		Cifar.Reg1LNet regNet = new Cifar.Reg1LNet();
		private void Form3_Load(object sender, EventArgs e)
		{
			cifar.init();
			regNet.init();
		}
		bool stop = false;
		private void button1_Click(object sender, EventArgs e)
		{
			for (int k = 0; k < 5; k++)
			{
				for (int n = 0; n < 60000; n++)
				{
					for (int i = 0; i < 28; i++)
					{
						for (int j = 0; j < 28; j++)
						{
							regNet.train(n, i, j);
						}
					}
					Text = "" + n;
					Application.DoEvents();
					if (n % 5 == 0)
						test();
					if (stop) { stop = false; break; }
				}

				Util.save("lv1_4_" + k + ".txt", (s) =>
				{
					regNet.cv1.save(s);
				});
			}

		}

		public void test() {
			Bitmap b = new Bitmap(300, 300);
			Graphics g = Graphics.FromImage(b);
			Random r = new Random();
			for (int i = 0; i < 10; i++)
			{
				int n = (int)(r.NextDouble() * 60000);
				int x = (int)(r.NextDouble() * 28);
				int y = (int)(r.NextDouble() * 28);

				Vol v = Cifar.get4x4(n, x, y);
				g.DrawImage(v.visRGB(), 10 + i * 10, 10);
				Vol ret = regNet.forward(v);
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
