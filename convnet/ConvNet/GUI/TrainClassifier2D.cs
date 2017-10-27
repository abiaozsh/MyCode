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
	public partial class TrainClassifier2D : Form
	{
		public TrainClassifier2D()
		{
			InitializeComponent();
		}
		//MNIST mnist = new MNIST();
		MNIST.Classifier2DNetTrainer net;
		MNIST._2D_10NetTrainer net2d210;
		MNIST._2D_ImgNetTrainer net2d2Img;

		MNIST._2D_ImgNet net2d2ImgTester;
		MNIST._2D_10Net net2d210Tester;
		private void TrainClassifier2D_Load(object sender, EventArgs e)
		{
			label1.ForeColor = MNIST.cDict[0];
			label2.ForeColor = MNIST.cDict[1];
			label3.ForeColor = MNIST.cDict[2];
			label4.ForeColor = MNIST.cDict[3];
			label5.ForeColor = MNIST.cDict[4];
			label6.ForeColor = MNIST.cDict[5];
			label7.ForeColor = MNIST.cDict[6];
			label8.ForeColor = MNIST.cDict[7];
			label9.ForeColor = MNIST.cDict[8];
			label10.ForeColor = MNIST.cDict[9];
			
			
			net = new MNIST.Classifier2DNetTrainer();
			net.init();

			net2d210 = new MNIST._2D_10NetTrainer();
			net2d210.init();

			net2d2Img = new MNIST._2D_ImgNetTrainer();
			net2d2Img.init();

			MNIST.Img_2DNet img_2d = new MNIST.Img_2DNet();
			img_2d.init();
			pictureBox3.Image = img_2d.display(2000);

			pictureBox3.MouseMove += pictureBox3_MouseMove;


			net2d2ImgTester = new MNIST._2D_ImgNet();
			net2d2ImgTester.init(net2d2Img.fc1, net2d2Img.ucv0, net2d2Img.ucv1, net2d2Img.ucv2);

			net2d210Tester = new MNIST._2D_10Net();
			net2d210Tester.init(net2d210.fc20A, net2d210.fc20B, net2d210.fc10);

		}
		private void button1_Click(object sender, EventArgs e)
		{
			float accu = 0;
			for (int k = 0; k < 20; k++)
			{
				for (int i = 0; i < 100; i++)
				{
					net.train();
					Text = k + "," + i + "," + accu;
					Application.DoEvents();
				}
				accu = net.test();
				Text = k + ",0," + accu;

				pictureBox3.Image = net.display();

				Util.save("net2Dclassifier.txt", (s) =>
				{
					net.save(s);
				});
			}
		}

		private void button2_Click(object sender, EventArgs e)
		{
			float accu = 0;
			for (int k = 0; k < 20; k++)
			{
				for (int i = 0; i < 100; i++)
				{
					net2d210.train();
					Text = k + "," + i + "," + accu;
					Application.DoEvents();
				}
				accu = net2d210.test();
				Text = k + ",0," + accu;

				Util.save("2D210.txt", (s) =>
				{
					net2d210.save(s);
				});
			}
		}

		private void button3_Click(object sender, EventArgs e)
		{
			for (int k = 0; k < 20; k++)
			{
				for (int i = 0; i < 100; i++)
				{
					net2d2Img.train();
					Text = k + "," + i;
					Application.DoEvents();
				}

				Util.save("2D2Img.txt", (s) =>
				{
					net2d2Img.save(s);
				});
			}
		}


		void pictureBox3_MouseMove(object sender, MouseEventArgs e)
		{
			pictureBox4.Image = net2d2ImgTester.proc(e.X, e.Y);

			textBox1.Text = net2d210Tester.proc(e.X, e.Y);

		}


	}
}
