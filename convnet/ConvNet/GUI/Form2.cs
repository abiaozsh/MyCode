using System;
using System.Collections.Generic;
using System.ComponentModel;
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
		public Form2()
		{
			InitializeComponent();
		}
		MainNet net;

		public class MainNet : Net//net
		{
			public void init()
			{
				int Z_DIM = 100;
				int IMAGE_W = 160;
				int IMAGE_H = 208;
				int t2 = IMAGE_H / 2;
				int t4 = IMAGE_H / 4;
				int t8 = IMAGE_H / 8;
				int t16 = IMAGE_H / 16;
				int s2 = IMAGE_W / 2;
				int s4 = IMAGE_W / 4;
				int s8 = IMAGE_W / 8;
				int s16 = IMAGE_W / 16;
				int GF = 64;

				//gfc0 = ConvNet.FC(inDepth = Z_DIM,outDepth = GF*8*t16*s16,loadFromFile = loadFromFile)
				//gdc0 = ConvNet.DeConv(inDepth = GF*8,outDepth = GF*4,filterSize = 5,loadFromFile = loadFromFile)
				//gdc1 = ConvNet.DeConv(inDepth = GF*4,outDepth = GF*2,filterSize = 5,loadFromFile = loadFromFile)
				//gdc2 = ConvNet.DeConv(inDepth = GF*2,outDepth = GF*1,filterSize = 5,loadFromFile = loadFromFile)
				//gdc3 = ConvNet.DeConv(inDepth = GF*1,outDepth = IMAGE_CHANNEL,filterSize = 5,loadFromFile = loadFromFile)

				//    _ret = gfc0.getLayer(z, isRelu=True, fixed = False)
				//    _ret = ConvNet.FC2Conv_Reshape(_ret, t16, s16, GF*8)
				//    _ret = gdc0.getLayer(_ret, height = t8, width = s8, convStride = 2, isRelu=True, fixed = False)
				//    _ret = gdc1.getLayer(_ret, height = t4, width = s4, convStride = 2, isRelu=True, fixed = False)
				//    _ret = gdc2.getLayer(_ret, height = t2, width = s2, convStride = 2, isRelu=True, fixed = False)
				//    _ret = gdc3.getLayer(_ret, height = IMAGE_H, width = IMAGE_W, convStride = 2, isRelu=False, fixed = False)


				Add(new InputLayer(out_sx: 1, out_sy: 1, out_depth: Z_DIM));
				FullyConnLayer gfc0 = new FullyConnLayer(outDepth: GF * 8 * s16 * t16, act: new ReluLayer());
				Add(gfc0);
				Add(new ReshapeLayer(s16, t16, GF * 8));
				DeConvLayer gdc0 = new DeConvLayer(5, s8, t8, GF * 4, 2, act: new ReluLayer());
				Add(gdc0);
				DeConvLayer gdc1 = new DeConvLayer(5, s4, t4, GF * 2, 2, act: new ReluLayer());
				Add(gdc1);
				DeConvLayer gdc2 = new DeConvLayer(5, s2, t2, GF * 1, 2, act: new ReluLayer());
				Add(gdc2);
				DeConvLayer gdc3 = new DeConvLayer(5, IMAGE_W, IMAGE_H, 3, 2);
				Add(gdc3);

				Util.load(@"..\gan9g.txt", (s) =>
				{
					gfc0.load(s);
					gdc0.load(s);
					gdc1.load(s);
					gdc2.load(s);
					gdc3.load(s);
				});


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

		private void button1_Click(object sender, EventArgs e)
		{
			Vol v = new Vol(1, 1, 100, null);
			Vol.init(v.w, 100, -1, 1);

			//Vol.init(v.w, 100, 0);

			//v.w[0] = 1;
			//v.w[1] = -1;

			var ins = net.getInstance();

			Vol ret = net.forward(ins, v);


			pictureBox1.Image = ret.visRGB(255,128);
		}

	}
}
