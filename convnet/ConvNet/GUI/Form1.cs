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
		MNIST.Lv1TrainNet train1net;
		MNIST.Lv2TrainNet train2net;
		MNIST.Lv3TrainNet train3net;

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


			train1net = new MNIST.Lv1TrainNet();
			train1net.init();

			train2net = new MNIST.Lv2TrainNet();
			train2net.init();

			train3net = new MNIST.Lv3TrainNet();
			train3net.init();

			
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

		bool stop;

		private void button3_Click(object sender, EventArgs e)
		{
			stop = true;
		}


		private void button5_Click(object sender, EventArgs e)
		{
			float loss = 0;

			//for (int k = 0; k < 5; k++)
			{
				for (int n = 0; n < MNISTData.Count; n++)
				{
					loss = train1net.train(n);

					Text = n + "," + loss;
					Application.DoEvents();
					if (n % 10 == 0)
						test4();
					if (stop) { stop = false; break; }
				}


				Util.save("cv1.txt", (s) =>
				{
					train1net.cv1.save(s);
				});
				Util.save("cv1_ufc.txt", (s) =>
				{
					train1net.ufc.save(s);
				});
			}

		}
		public void test4()
		{
			Vol v4 = new Vol(4, 4, 1, 0);

			Bitmap b = new Bitmap(300, 300);
			Graphics g = Graphics.FromImage(b);
			Random r = new Random();
			for (int i = 0; i < 10; i++)
			{
				int n = (int)(r.NextDouble() * 60000);
				int x = (int)(r.NextDouble() * 28);
				int y = (int)(r.NextDouble() * 28);

				Vol v = MNISTData.getImg(n);

				MNIST.Lv1TrainNet.get4x4(v, v4, x, y);
				g.DrawImage(v4.vis(0), 40 + i * 10, 30);

				Vol ret = train1net.forward(v4);
				g.DrawImage(ret.vis(0), 40 + i * 10, 40);
			}
			g.Flush();
			g.Dispose();

			pictureBox2.Image = b;
		}

		private void button6_Click(object sender, EventArgs e)
		{
			float loss = 0;

			//for (int k = 0; k < 5; k++)
			{
				Random r = new Random();
				for (int n = 0; n < 100000; n++)
				{
					loss = train2net.train((int)(r.NextDouble() * MNISTData.Count));

					Text = n + "," + loss;
					Application.DoEvents();
					Util.log(loss + "");
					if (n % 10 == 0)
						test8();
					if (stop) { stop = false; break; }
				}


				Util.save("cv2.txt", (s) =>
				{
					train2net.cv2.save(s);
				});
				Util.save("cv2_ufc.txt", (s) =>
				{
					train2net.ufc.save(s);
				});
			}
		}

		Vol v8 = new Vol(8, 8, 1, 0);
		public void test8()
		{

			Bitmap b = new Bitmap(300, 300);
			Graphics g = Graphics.FromImage(b);
			Random r = new Random();
			for (int i = 0; i < 10; i++)
			{
				int n = (int)(r.NextDouble() * 60000);
				int x = (int)(r.NextDouble() * 28);
				int y = (int)(r.NextDouble() * 28);

				Vol v = MNISTData.getImg(n);

				MNIST.Lv2TrainNet.get8x8(v, v8, x, y);
				g.DrawImage(v8.vis(0), 40 + i * 10, 30);

				Vol ret = train2net.forward(v8);
				g.DrawImage(ret.vis(0), 40 + i * 10, 40);
			}
			g.Flush();
			g.Dispose();

			pictureBox2.Image = b;
		}
		private void button4_Click(object sender, EventArgs e)
		{
			float loss = 0;

			//for (int k = 0; k < 5; k++)
			{
				Random r = new Random();
				for (int n = 0; n < 100000; n++)
				{
					loss = train3net.train((int)(r.NextDouble() * MNISTData.Count));

					Text = n + "," + loss;
					Application.DoEvents();
					Util.log(loss + "");
					test16();
					if (stop) { stop = false; break; }
				}


				Util.save("cv3.txt", (s) =>
				{
					train3net.cv3.save(s);
				});
				Util.save("cv3_ufc.txt", (s) =>
				{
					train3net.ufc.save(s);
				});
			}
		}
		Vol v16 = new Vol(16, 16, 1, 0);
		public void test16()
		{

			Bitmap b = new Bitmap(300, 300);
			Graphics g = Graphics.FromImage(b);
			Random r = new Random();
			for (int i = 0; i < 10; i++)
			{
				int n = (int)(r.NextDouble() * 60000);
				int x = (int)(r.NextDouble() * 28);
				int y = (int)(r.NextDouble() * 28);

				Vol v = MNISTData.getImg(n);

				MNIST.Lv3TrainNet.get16x16(v, v16, x, y);
				g.DrawImage(v16.vis(0), 40 + i * 20, 30);

				Vol ret = train3net.forward(v16);
				g.DrawImage(ret.vis(0), 40 + i * 20, 50);
			}
			g.Flush();
			g.Dispose();

			pictureBox2.Image = b;
		}

	}
}
