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
		public class MainNet : Net
		{
			public ConvLayer cv1;
			public ConvLayer cv2;
			public FullyConnLayer fc0;
			public FullyConnLayer fc1;

			public void init()
			{
				cv1 = new ConvLayer(filterSize : 5, out_depth :16, stride : 1, act: new ReluLayer());
				cv2 = new ConvLayer(filterSize: 5, out_depth: 32, stride: 1, act: new ReluLayer());
				fc0 = new FullyConnLayer(outDepth: 64, act: new ReluLayer());
				fc1 = new FullyConnLayer(outDepth: 10);

				Add(new InputLayer(out_sx: 28, out_sy: 28, out_depth: 1));
				Add(cv1);
				Add(new PoolLayer(stride: 2));
				Add(cv2);
				Add(new PoolLayer(stride: 2));
				Add(fc0);
				Add(fc1);
				Add(new SoftmaxLayer());

				Util.load(@"..\..\test.txt",(s)=>{
					cv1.load(s);
					cv2.load(s);
					fc0.load(s);
					fc1.load(s);
				});

			}

		}
		MainNet mainet;

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
			pictureBox2.Image = bmp;
			pictureBox2.MouseMove+=pictureBox2_MouseMove;

			mainet = new MainNet();
			mainet.init();



		}

		void pictureBox2_MouseMove(object sender, MouseEventArgs e)
		{
			var ins = mainet.getInstance();

			pictureBox1.Image = _getBmp(e.X, e.Y);

			var img = _getImg(e.X, e.Y);

			var v10 = mainet.forward(ins, img);

			String s = MNISTData.report(v10);
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
					v.w[i + j * 28] = (float)val / 255.0f-0.5f;
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

		private void pictureBox2_Click(object sender, EventArgs e)
		{

		}


	}
}
