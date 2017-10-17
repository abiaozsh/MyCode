using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
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

		byte[] bigImg;
		int bigImgW;

		private void Form1_Load(object sender, EventArgs e)
		{


			this.MouseWheel += Form1_MouseWheel;
			try
			{
				mnist.Init();
			}
			catch (Exception ex)
			{
				ex.ToString();
			}

			Bitmap bmp = (Bitmap)Bitmap.FromFile(@"E:\MNIST\无标题.png");

			pictureBox2.Image = bmp;
			pictureBox2.Width = bmp.Width;
			pictureBox2.Height = bmp.Height;

			pictureBox2.MouseMove += pictureBox2_MouseMove;

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

		void pictureBox2_MouseMove(object sender, MouseEventArgs e)
		{
			try
			{
				pictureBox1.Image = _getBmp(e.X, e.Y);
				var v1 = mnist.net.forward(_getImg(e.X, e.Y), false);
				String s = mnist.report(v1);
				textBox1.Text = (s);
			}
			catch (Exception ex)
			{
				ex.ToString();
			}
		}

		int idx = 10000;
		void Form1_MouseWheel(object sender, MouseEventArgs e)
		{
			if (e.Delta > 0) idx++; else idx--;
			pictureBox1.Image = mnist.getBmp(idx);
			int val = mnist.getLbl(idx);
			var v1 = mnist.net.forward(mnist.getImg(idx), false);
			String s = mnist.report(v1);
			textBox1.Text = (val + "\r\n" + s);

		}

		private void pictureBox1_Click(object sender, EventArgs e)
		{

		}

		private void button1_Click(object sender, EventArgs e)
		{
			for (int i = 0; i < 10000; i++)
			{
				mnist.train(i);
				Text = i.ToString();
				Application.DoEvents();
			}
			{
				FileStream fs = new FileStream("mnistnet.txt", FileMode.Create, FileAccess.Write);
				StreamWriter sw = new StreamWriter(fs);
				mnist.net.save(sw);
				sw.Flush();
				fs.Flush();
				fs.Close();
			}
		}

		private void button2_Click(object sender, EventArgs e)
		{
			//mnist.proc();

			FileStream fs = new FileStream("mnistnet.txt", FileMode.Open, FileAccess.Read);
			StreamReader sr = new StreamReader(fs);
			mnist.net.load(sr);
			fs.Close();

		}
	}
}
