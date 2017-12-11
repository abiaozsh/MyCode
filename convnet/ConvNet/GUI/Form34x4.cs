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
		RegNet net = new RegNet();

		public class RegNet : Net//net
		{
			public InputLayer inputL;
			public ConvLayer cv1;
			public ConvLayer cv2;
			public ConvLayer cv3;
			public FullyConnLayer fc2;
			public ReshapeLayer reshape;
			public DeConvLayer ucv3;
			public DeConvLayer ucv2;
			public DeConvLayer ucv1;

			public static int l1Filter = 16;
			public static int l2Filter = 32;
			public static int l3Filter = 64;
			public void init()
			{
				//inputL = new InputLayer(out_sx: 24, out_sy: 32, out_depth: 3);
				//cv1 = new ConvLayer(filterSize: 5, out_depth: 32, stride: 1, act: new ReluLayer());
				//cv2 = new ConvLayer(filterSize: 5, out_depth: 32, stride: 1, act: new ReluLayer());
				//cv3 = new ConvLayer(filterSize: 5, out_depth: 64, stride: 1, act: new ReluLayer());
				//fc2 = new FullyConnLayer(outDepth: 3 * 4 * 128, act: new ReluLayer());
				//reshape = new ReshapeLayer(out_sx: 3, out_sy: 4, out_depth: 128);
				//ucv3 = new DeConvLayer(filterSize: 5, out_sx: 6, out_sy: 8, out_depth: 64, stride: 2, act: new ReluLayer());
				//ucv2 = new DeConvLayer(filterSize: 5, out_sx: 12, out_sy: 16, out_depth: 64, stride: 2, act: new ReluLayer());
				//ucv1 = new DeConvLayer(filterSize: 5, out_sx: 24, out_sy: 32, out_depth: 3, stride: 2);

				inputL = new InputLayer(out_sx: 32, out_sy: 24, out_depth: 3);
				cv1 = new ConvLayer(filterSize: 5, out_depth: 32, stride: 1, act: new ReluLayer());
				cv2 = new ConvLayer(filterSize: 5, out_depth: 32, stride: 1, act: new ReluLayer());
				cv3 = new ConvLayer(filterSize: 5, out_depth: 64, stride: 1, act: new ReluLayer());
				fc2 = new FullyConnLayer(outDepth: 4 * 3 * 128, act: new ReluLayer());
				reshape = new ReshapeLayer(out_sx: 4, out_sy: 3, out_depth: 128);
				ucv3 = new DeConvLayer(filterSize: 5, out_sx: 8, out_sy: 6, out_depth: 64, stride: 2, act: new ReluLayer());
				ucv2 = new DeConvLayer(filterSize: 5, out_sx: 16, out_sy: 12, out_depth: 64, stride: 2, act: new ReluLayer());
				ucv1 = new DeConvLayer(filterSize: 5, out_sx: 32, out_sy: 24, out_depth: 3, stride: 2);

				Add(inputL);

				Add(cv1);
				Add(new PoolLayer(stride: 2));

				Add(cv2);
				Add(new PoolLayer(stride: 2));

				Add(cv3);
				Add(new PoolLayer(stride: 2));

				Add(fc2);
				Add(reshape);
				Add(ucv3);
				Add(ucv2);
				Add(ucv1);

				Util.load(@"..\cifar.txt", (s) =>
				{
					cv1.load(s);
					cv2.load(s);
					cv3.load(s);
					fc2.load(s);
					ucv3.load(s);
					ucv2.load(s);
					ucv1.load(s);
				});

			}

		}

		private void Form3_Load(object sender, EventArgs e)
		{
			cifar.init();
			net.init();


			pictureBox1.Image = Cifar.imgs[2].visRGB(255,128);

			var ins = net.getInstance();

			pictureBox2.Image = net.forward(ins, Cifar.imgs[2]).visRGB(255, 128);

		}

		private void button5_Click(object sender, EventArgs e)
		{
			var ins = net.getInstance();

			Vol v = new Vol(32, 32, 3, 0);
			//Cifar.getImg(v, int.Parse(textBox1.Text));

			pictureBox1.Image = v.visRGB(255,128);

			Stopwatch sw = new Stopwatch();
			sw.Start();
			var ret = net.forward(ins, v);
			sw.Stop();

			textBox2.Text = sw.ElapsedMilliseconds + "";

			pictureBox2.Image = ret.visRGB(255, 128);

			for (int i = 0; i < 32; i++)
			{
				net.cv1.visRGB(i, 255 * 5).Save(@"..\vis\" + i + ".bmp");
			}
		}

		Vol _v;
		private void button6_Click(object sender, EventArgs e)
		{
			var ins = net.getInstance();

			Vol v = new Vol(32, 32, 3, 0);
			//Cifar.getImg(v, int.Parse(textBox1.Text));

			pictureBox1.Image = v.visRGB(255, 128);

			//_v = net.forward(ins, v, null, net.fc64);

			TextWriter tw = new StringWriter();
			_v.save(tw);
			textBox3.Text = tw.ToString();
		}

		private void button1_Click(object sender, EventArgs e)
		{
			var ins = net.getInstance();

			TextReader tr = new StringReader(textBox3.Text);
			_v.load(tr);
			tr.Close();

			var ret = net.forward(ins, _v, net.fc2, null);

			pictureBox2.Image = ret.visRGB(255, 128);

		}

		private void button3_Click(object sender, EventArgs e)
		{
		}

		private void button2_Click(object sender, EventArgs e)
		{
		}

		private void textBox4_TextChanged(object sender, EventArgs e)
		{
			int idx = 0;
			int.TryParse(textBox4.Text, out idx);
			this.trackBar1.Value = (int)_v.w[idx];
		}

		private void textBox5_TextChanged(object sender, EventArgs e)
		{
			int idx = 0;
			int.TryParse(textBox5.Text, out idx);
			this.trackBar2.Value = (int)_v.w[idx];
		}

		private void trackBar1_Scroll(object sender, EventArgs e)
		{
			int idx = 0;
			int.TryParse(textBox4.Text, out idx);
			_v.w[idx] = this.trackBar1.Value;
			TextWriter tw = new StringWriter();
			_v.save(tw);
			textBox3.Text = tw.ToString();
			button1_Click(null, null);
		}

		private void trackBar2_Scroll(object sender, EventArgs e)
		{
			int idx = 0;
			int.TryParse(textBox5.Text, out idx);
			_v.w[idx] = this.trackBar2.Value;
			TextWriter tw = new StringWriter();
			_v.save(tw);
			textBox3.Text = tw.ToString();
			button1_Click(null, null);
		}


	}
}
