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
			//主网络，多层卷积 +全连接 收窄到2维神经元 全连接 再多层反卷积 输出图像

			public ConvLayer cv1;
			public ConvLayer cv2;
			public ConvLayer cv3;
			public FullyConnLayer fc2;
			public DeConvLayer ucv3;
			public DeConvLayer ucv2;
			public DeConvLayer ucv1;

			public static int l1Filter = 16;
			public static int l2Filter = 32;
			public static int l3Filter = 64;
			public void init()
			{

				cv1 = new ConvLayer(filterSize: 5, out_depth: 32, stride: 1, padding: true, act: new ReluLayer());
				cv2 = new ConvLayer(filterSize: 5, out_depth: 32, stride: 1, padding: true, act: new ReluLayer());
				cv3 = new ConvLayer(filterSize: 5, out_depth: 64, stride: 1, padding: true, act: new ReluLayer());
				fc2 = new FullyConnLayer(num_neurons: 2048, act: new ReluLayer());
				ucv3 = new DeConvLayer(filterSize: 5, out_sx: 8, out_sy: 8, out_depth: 64, stride: 2,pad:0, act: new ReluLayer());
				ucv2 = new DeConvLayer(filterSize: 5, out_sx: 16, out_sy: 16, out_depth: 64, stride: 2,pad:0, act: new ReluLayer());
				ucv1 = new DeConvLayer(filterSize: 5, out_sx: 32, out_sy: 32, out_depth: 3, stride: 2,pad:0);

				Add(new InputLayer(out_sx: 32, out_sy: 32, out_depth: 3));

				Add(cv1);
				Add(new PoolLayer(stride: 2));

				Add(cv2);
				Add(new PoolLayer(stride: 2));

				Add(cv3);
				Add(new PoolLayer(stride: 2));

				Add(fc2);
				Add(new ReshapeLayer(out_sx: 4, out_sy: 4, out_depth: 128));
				Add(ucv3);
				Add(ucv2);
				Add(ucv1);


				Util.load(@"..\Cifar.txt", (s) =>
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
		}

		private void button5_Click(object sender, EventArgs e)
		{
			var ins = net.getInstance();

			Vol v = new Vol(32,32,3,0);
			Cifar.getImg(v,int.Parse(textBox1.Text));

			pictureBox1.Image = v.visRGB(255);
			
			var ret = net.forward(ins,v);

			pictureBox2.Image = ret.visRGB(255);


		}


	}
}
