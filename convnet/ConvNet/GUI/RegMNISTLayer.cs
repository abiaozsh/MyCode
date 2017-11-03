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
		}

		MNIST mnist = new MNIST();


		private void button1_Click(object sender, EventArgs e)
		{
			for (int k = 0; k < 5000; k++)
			{
				for (int i = 0; i < 1; i++)
				{
					regNet.train();
					Text = k + "," + i;
					Application.DoEvents();
				}
				regNet.forward(MNISTData.getImg(0));
				pictureBox1.Image = regNet.in_layer.out_act.vis(0);
				pictureBox4.Image = regNet.ucv2.out_act.vis(0);


				regNet.forward(MNISTData.getImg(1));
				pictureBox2.Image = regNet.in_layer.out_act.vis(0);
				pictureBox3.Image = regNet.ucv2.out_act.vis(0);

				regNet.forward(MNISTData.getImg(2));
				pictureBox5.Image = regNet.in_layer.out_act.vis(0);
				pictureBox6.Image = regNet.ucv2.out_act.vis(0);

				//Util.save("net.txt", (s) =>
				//{
				//	mainNet.cv1.save(s);
				//	mainNet.cv2.save(s);
				//	mainNet.cv3.save(s);
				//	mainNet.fc144.save(s);
				//});

			}


		}
	}
}
