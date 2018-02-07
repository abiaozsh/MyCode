using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using ConvNet;

namespace GUI
{
	public partial class Form2 : Form
	{
		const int Z_DIM = 128;
		const int IMAGE_W = 192;
		const int IMAGE_H = 256;
		const int GF = 96;


		public Form2()
		{
			InitializeComponent();
		}
		MainNet net;

		public class MainNet : Net//net
		{

			public void init()
			{


				//GF = 96             # Dimension of G filters in first conv layer. default [64]
				//loadFromFile = ConvNet.openEmptyFileR('gan10g.txt')
				//gfc0 = ConvNet.addlist(glist,ConvNet.FC(inDepth = Z_DIM,outDepth = GF*8*3*4,loadFromFile = loadFromFile))
				//gdc0 = ConvNet.addlist(glist,ConvNet.DeConv(inDepth = GF*8,outDepth = GF*8,filterSize = 3,loadFromFile = loadFromFile))#4in
				//gdc1 = ConvNet.addlist(glist,ConvNet.DeConv(inDepth = GF*8,outDepth = GF*4,filterSize = 3,loadFromFile = loadFromFile))#8in
				//gdc2 = ConvNet.addlist(glist,ConvNet.DeConv(inDepth = GF*4,outDepth = GF*2,filterSize = 5,loadFromFile = loadFromFile))#16in
				//gdc3 = ConvNet.addlist(glist,ConvNet.DeConv(inDepth = GF*2,outDepth = GF*2,filterSize = 5,loadFromFile = loadFromFile))#32in
				//gdc4 = ConvNet.addlist(glist,ConvNet.DeConv(inDepth = GF*2,outDepth = GF*1,filterSize = 5,loadFromFile = loadFromFile))#32in
				//gdc5 = ConvNet.addlist(glist,ConvNet.DeConv(inDepth = GF*1,outDepth = IMAGE_CHANNEL,filterSize = 5,loadFromFile = loadFromFile))#64in
				//
				//_ret = gfc0.getLayer(z, isRelu=True, fixed = False)
				//_ret = ConvNet.FC2Conv_Reshape(_ret, 4, 3, GF*8)
				//_ret = gdc0.getLayer(_ret, height = 8, width = 6, convStride = 2, isRelu=True, fixed = False)
				//_ret = gdc1.getLayer(_ret, height = 16, width = 12, convStride = 2, isRelu=True, fixed = False)
				//_ret = gdc2.getLayer(_ret, height = 32, width = 24, convStride = 2, isRelu=True, fixed = False)
				//_ret = gdc3.getLayer(_ret, height = 64, width = 48, convStride = 2, isRelu=True, fixed = False)
				//_ret = gdc4.getLayer(_ret, height = 128, width = 96, convStride = 2, isRelu=True, fixed = False)
				//_ret = gdc5.getLayer(_ret, height = IMAGE_H, width = IMAGE_W, convStride = 2, isRelu=False, fixed = False)

				Add(new InputLayer(out_sx: 1, out_sy: 1, out_depth: Z_DIM));
				FullyConnLayer gfc0 = new FullyConnLayer(outDepth: GF * 8 * 3 * 4, act: new ReluLayer());
				Add(gfc0);
				Add(new ReshapeLayer(3, 4, GF * 8));
				DeConvLayer gdc0 = new DeConvLayer(3, 6, 8, GF * 8, 2, act: new ReluLayer());
				Add(gdc0);
				DeConvLayer gdc1 = new DeConvLayer(3, 12, 16, GF * 4, 2, act: new ReluLayer());
				Add(gdc1);
				DeConvLayer gdc2 = new DeConvLayer(5, 24, 32, GF * 2, 2, act: new ReluLayer());
				Add(gdc2);
				DeConvLayer gdc3 = new DeConvLayer(5, 48, 64, GF * 2, 2, act: new ReluLayer());
				Add(gdc3);
				DeConvLayer gdc4 = new DeConvLayer(5, 96, 128, GF * 1, 2, act: new ReluLayer());
				Add(gdc4);
				DeConvLayer gdc5 = new DeConvLayer(5, IMAGE_W, IMAGE_H, 3, 2);
				Add(gdc5);

				//Util.load(@"..\..\ganbetter96g.txt", (s) =>
				Util.bload(@"..\..\..\ganGood\gan11g4000.bin", (s) =>
				{
					gfc0.load(s);
					gdc0.load(s);
					gdc1.load(s);
					gdc2.load(s);
					gdc3.load(s);
					gdc4.load(s);
					gdc5.load(s);
				});
				//clip(gfc0);
				//clip(gdc0);
				//clip(gdc1);
				//clip(gdc2);
				//clip(gdc3);
				//clip(gdc4);
				//clip(gdc5);
			}
			void clip(DeConvLayer l)
			{
				clip(l.bias_w,0.001f,1000.0f);
				clip(l.filters_w, 0.01f, 100.0f);
			}
			void clip(FullyConnLayer l)
			{
				clip(l.bias_w, 0.001f, 1000.0f);
				clip(l.filters_w, 0.01f, 100.0f);
			}

			void clip(MyFloat a,float alpha,float beta)
			{
				for (int i = 0; i < a.size; i++)
				{
					var val = a[i];
					if (Math.Abs(val) < alpha)
					{
						a[i] = 0.0f;
					}
					else
					{
						a[i] = (float)(Math.Floor(val * beta) / beta);
					}
				}
			}
		}


		private void Form2_Load(object sender, EventArgs e)
		{
			net = new MainNet();
			net.init();
		}

		private void draw()
		{
		}

		private void pictureBox1_Click(object sender, EventArgs e)
		{

		}

		Vol v = new Vol(1, 1, Z_DIM, null);

		private void button1_Click(object sender, EventArgs e)
		{
			Vol.init(v.w, Z_DIM, -float.Parse(this.textBox3.Text), float.Parse(this.textBox3.Text));

			//Vol.init(v.w, 100, 0);

			//v.w[0] = 1;
			//v.w[1] = -1;

			Stopwatch sw = new Stopwatch();

			sw.Start();
			var ins = net.getInstance();

			Vol ret = net.forward(ins, v);
			sw.Stop();
			long t1 = sw.ElapsedMilliseconds;
			sw.Restart();
			pictureBox1.Image = ret.visRGB(255, 128);
			sw.Stop();
			long t2 = sw.ElapsedMilliseconds;

			this.Text = (t1 + "," + t2);
		}

		private void trackBar1_Scroll(object sender, EventArgs e)
		{
			v.set(0, 0, int.Parse(this.textBox1.Text), trackBar1.Value / 100.0f);

			var ins = net.getInstance();

			Vol ret = net.forward(ins, v);

			pictureBox1.Image = ret.visRGB(255, 128);
		}

		Vol add(Vol v1, Vol v2)
		{
			Vol ret = new Vol(1, 1, Z_DIM, null);

			for (int i = 0; i < Z_DIM; i++)
			{
				ret.w[i] = v1.w[i] + v2.w[i];
			}
			return ret;
		}
		Vol sub(Vol v1, Vol v2)
		{
			Vol ret = new Vol(1, 1, Z_DIM, null);

			for (int i = 0; i < Z_DIM; i++)
			{
				ret.w[i] = v1.w[i] - v2.w[i];
			}
			return ret;
		}
		Vol neg(Vol v1)
		{
			Vol ret = new Vol(1, 1, Z_DIM, null);

			for (int i = 0; i < Z_DIM; i++)
			{
				ret.w[i] = -v1.w[i];
			}
			return ret;
		}

		private void button2_Click(object sender, EventArgs e)
		{
			{
				var ins1 = net.getInstance();
				var ins2 = net.getInstance();
				var ins3 = net.getInstance();
				var ins4 = net.getInstance();

				Vol v1 = new Vol(1, 1, Z_DIM, null);
				Vol.init(v1.w, Z_DIM, -0.5f, 0.5f);
				Vol v2 = new Vol(1, 1, Z_DIM, null);
				Vol.init(v2.w, Z_DIM, -0.5f, 0.5f);

				Vol v3 = add(v1, v2);
				Vol v4 = sub(v1, v2);

				Vol ret1 = net.forward(ins1, v1);
				Vol ret2 = net.forward(ins2, v2);
				Vol ret3 = net.forward(ins3, v3);
				Vol ret4 = net.forward(ins4, v4);

				this.pictureBox1.Image = ret1.visRGB(256, 128);
				this.pictureBox2.Image = ret2.visRGB(256, 128);
				this.pictureBox3.Image = ret3.visRGB(256, 128);
				this.pictureBox4.Image = ret4.visRGB(256, 128);
			}
			{
				var ins1 = net.getInstance();
				var ins2 = net.getInstance();
				var ins3 = net.getInstance();
				var ins4 = net.getInstance();

				Vol v1 = new Vol(1, 1, Z_DIM, null);
				Vol.init(v1.w, Z_DIM, -1f, 1f);
				Vol v2 = new Vol(1, 1, Z_DIM, null);
				Vol.init(v2.w, Z_DIM, -1f, 1f);

				Vol v3 = neg(v1);
				Vol v4 = neg(v2);

				Vol ret1 = net.forward(ins1, v1);
				Vol ret2 = net.forward(ins2, v2);
				Vol ret3 = net.forward(ins3, v3);
				Vol ret4 = net.forward(ins4, v4);

				this.pictureBox5.Image = ret1.visRGB(256, 128);
				this.pictureBox6.Image = ret2.visRGB(256, 128);
				this.pictureBox7.Image = ret3.visRGB(256, 128);
				this.pictureBox8.Image = ret4.visRGB(256, 128);
			}
		}

	}
}
