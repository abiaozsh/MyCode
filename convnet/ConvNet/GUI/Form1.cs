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

			pictureBox2.Image = bmp;
			pictureBox2.Width = bmp.Width;
			pictureBox2.Height = bmp.Height;

			bigImg = new byte[bmp.Width * bmp.Height];
			bigImgW = bmp.Width;

			pictureBox2.MouseMove += pictureBox2_MouseMove;
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
			pictureBox1.Image = _getBmp(e.X, e.Y);

			var img = _getImg(e.X, e.Y);

			var v10 = mainet.forward(img);

			String s = MNIST.report(v10, -1, -1);
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

		private void button1_Click(object sender, EventArgs e)
		{
			for (int i = 0; i < 1000; i++)
			{
				for (int k = 0; k < 100; k++)
				{
					mainet.train();
					this.Text = k + "";
					Application.DoEvents();
				}
				vis();
				save();
				float accu = mainet.test();
				this.textBox2.Text = i + "," + accu;
			}


			//float accu = mainet.test();
			//this.textBox2.Text =  "," + accu;
		}

		private void button2_Click(object sender, EventArgs e)
		{
			vis();
		}

		public void vis()
		{
			float scale = 255;
			string dir = @"vis\";
			mainet.cv1.vis(0, 0, scale).Save(dir + "0.bmp");
			mainet.cv1.vis(1, 0, scale).Save(dir + "1.bmp");
			mainet.cv1.vis(2, 0, scale).Save(dir + "2.bmp");
			mainet.cv1.vis(3, 0, scale).Save(dir + "3.bmp");
			mainet.cv1.vis(4, 0, scale).Save(dir + "4.bmp");
			mainet.cv1.vis(5, 0, scale).Save(dir + "5.bmp");
			mainet.cv1.vis(6, 0, scale).Save(dir + "6.bmp");
			mainet.cv1.vis(7, 0, scale).Save(dir + "7.bmp");
			mainet.cv1.vis(8, 0, scale).Save(dir + "8.bmp");
			mainet.cv1.vis(9, 0, scale).Save(dir + "9.bmp");
			mainet.cv1.vis(10, 0, scale).Save(dir + "10.bmp");
			mainet.cv1.vis(11, 0, scale).Save(dir + "11.bmp");
			mainet.cv1.vis(12, 0, scale).Save(dir + "12.bmp");
			mainet.cv1.vis(13, 0, scale).Save(dir + "13.bmp");
			mainet.cv1.vis(14, 0, scale).Save(dir + "14.bmp");
			mainet.cv1.vis(15, 0, scale).Save(dir + "15.bmp");
		}

		private void button4_Click(object sender, EventArgs e)
		{

			Util.load(@"..\main.txt", (s) =>
			{
				mainet.cv1.load(s);
				mainet.cv2.load(s);
				mainet.cv3.load(s);
				mainet.fc144.load(s);
				mainet.fc10.load(s);
			});
		}

		private void button3_Click(object sender, EventArgs e)
		{
			save();

		}

		public void save()
		{
			Util.save(@"main.txt", (s) =>
			{
				mainet.cv1.save(s);
				mainet.cv2.save(s);
				mainet.cv3.save(s);
				mainet.fc144.save(s);
				mainet.fc10.save(s);
			});
		}

	}
}
