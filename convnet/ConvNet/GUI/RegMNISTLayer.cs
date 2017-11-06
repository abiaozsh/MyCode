using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using ConvNet;

namespace GUI
{
	public partial class RegMNISTLayer : Form
	{
		public RegMNISTLayer()
		{
			InitializeComponent();
		}

		MNIST.RegNet regNet;
		private void TrainFirst3Layer_Load(object sender, EventArgs e)
		{
			regNet = new MNIST.RegNet();
			regNet.init();

			Util.load("net.txt", (s) =>
			{
				regNet.cv1.load(s); regNet.cv1.noUpdate = true;
				regNet.cv2.load(s); regNet.cv2.noUpdate = true;
				regNet.cv3.load(s); regNet.cv3.noUpdate = true;
				regNet.fc144.load(s); regNet.fc144.noUpdate = true;
			});
		}

		MNIST mnist = new MNIST();

		int max = 30;
		int index = 0;
		private void button1_Click(object sender, EventArgs e)
		{
			int ll = 0;
			for (int k = 0; k < 5000; k++)
			{
				for (int m = 0; m < 20; m++)
				{
					index++;
					if (index >= max) index = 0;
					{
						regNet.train(index);
					}
				}
				Text = k + ","+max;
				Application.DoEvents();

				ll++;
				if (ll >= max) ll = 0;
				regNet.forward(MNISTData.getImg(ll));
				pictureBox1.Image = regNet.in_layer.out_act.vis(0);
				pictureBox4.Image = regNet.out_layer.in_act.vis(0);

				ll++;
				if (ll >= max) ll = 0;
				regNet.forward(MNISTData.getImg(ll));
				pictureBox2.Image = regNet.in_layer.out_act.vis(0);
				pictureBox3.Image = regNet.out_layer.in_act.vis(0);

				regNet.forward(MNISTData.getImg(max));
				pictureBox5.Image = regNet.in_layer.out_act.vis(0);
				pictureBox6.Image = regNet.out_layer.in_act.vis(0);

				//Util.save("net.txt", (s) =>
				//{
				//	regNet.cv1.save(s);
				//	regNet.cv2.save(s);
				//	regNet.cv3.save(s);
				//	regNet.fc144.save(s);
				//});

			}


		}

		private void button2_Click(object sender, EventArgs e)
		{
			max++;
		}
	}
}
