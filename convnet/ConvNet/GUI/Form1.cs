using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
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
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();
		}
		MNIST mnist = new MNIST();
		MNIST.MainNet mainet;

		Bitmap baseimg;

		byte[] bigImg;
		int bigImgW;

		private void Form1_Load(object sender, EventArgs e)
		{
			MNISTData.proc();

			Bitmap bmp = (Bitmap)Bitmap.FromFile(@"E:\MNIST\无标题.png");

			bigImg = new byte[bmp.Width * bmp.Height];
			bigImgW = bmp.Width;

			try
			{
				for (int j = 0; j < bmp.Height; j++)
				{
					for (int i = 0; i < bmp.Width; i++)
					{
						bigImg[j * bigImgW + i] = bmp.GetPixel(i, j).R;
					}
				}
			}
			catch (Exception ex)
			{
				ex.ToString();
			}

			mainet = new MNIST.MainNet();
			mainet.init();



		}

		void pictureBox2_MouseMove(object sender, MouseEventArgs e)
		{
			var ins = mainet.getInstance();

			pictureBox1.Image = _getBmp(e.X, e.Y);

			var img = _getImg(e.X, e.Y);

			var v10 = mainet.forward(ins, img);

			String s = MNISTData.report(v10, -1, -1);
			textBox2.Text = (s);


		}

		Vol v = new Vol(28, 28, 1, 0.0f);
		public Vol _getImg(int x, int y)
		{
			for (var i = 0; i < 28; i++)
			{
				for (var j = 0; j < 28; j++)
				{
					int idx = (i + x) + (j + y) * bigImgW;
					int val = 0;
					if (idx < bigImg.Length)
					{
						val = bigImg[idx];
					}
					v.w[i + j * 28] = (float)val / 255.0f;
				}
			}
			return v;
		}
		public Bitmap _getBmp(int x, int y)
		{
			Bitmap b = new Bitmap(28, 28);

			for (int i = 0; i < 28; i++)
			{
				for (int j = 0; j < 28; j++)
				{
					int idx = (i + x) + (j + y) * bigImgW;
					int val = 0;
					if (idx < bigImg.Length)
					{
						val = bigImg[idx];
					}
					b.SetPixel(i, j, Color.FromArgb(val, val, val));
				}
			}
			return b;
		}


		bool stop;

		private void button3_Click(object sender, EventArgs e)
		{
			stop = true;
		}


	}
}
