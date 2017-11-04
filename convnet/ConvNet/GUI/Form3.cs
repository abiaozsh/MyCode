using System;
using System.Collections.Generic;
using System.ComponentModel;
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
	public partial class Form3 : Form
	{
		public Form3()
		{
			InitializeComponent();
		}

		int idx = 10;
		Cifar cifar = new Cifar();
		Cifar.RegNet regNet = new Cifar.RegNet();
		private void Form3_Load(object sender, EventArgs e)
		{
			cifar.init();
			regNet.init();
			this.MouseWheel += Form3_MouseWheel;
		}
		string[] classify = new string[]{
"airplane",
"automobile",
"bird",
"cat",
"deer",
"dog",
"frog",
"horse",
"ship",
"truck"
		};
		void Form3_MouseWheel(object sender, MouseEventArgs e)
		{
			if (e.Delta > 0)
			{
				idx++;
			}
			else
			{
				idx--;
			}
			this.Text = classify[cifar.getLbl(idx)];
			this.BackgroundImage = Cifar.getBmp(idx);
		}

		bool stop = false;
		private void button1_Click(object sender, EventArgs e)
		{
			for (int k = 0; k < 5000; k++)
			{
				for (int i = 0; i < 1; i++)
				{
					regNet.train(2);
					Text = k + "," + i;
					Application.DoEvents();
				}
				regNet.forward(Cifar.getImg(0));
				pictureBox1.Image = regNet.in_layer.out_act.visRGB();
				pictureBox4.Image = regNet.ucv2.out_act.visRGB();


				regNet.forward(Cifar.getImg(1));
				pictureBox2.Image = regNet.in_layer.out_act.visRGB();
				pictureBox3.Image = regNet.ucv2.out_act.visRGB();

				if (stop) { stop = false; break; }
			}

		}

		private void button2_Click(object sender, EventArgs e)
		{
			stop = true;
			//	{
			//		FileStream fs = new FileStream(textBox1.Text + ".txt", FileMode.Open, FileAccess.Read);
			//		StreamReader sr = new StreamReader(fs);
			//		cifar.mainNet.load(sr);
			//		fs.Close();
			//	}
			//
			//	Bitmap b = cifar.display(this, 200);
			//
			//	b.Save(@"e:\out.jpg", ImageFormat.Jpeg);
		}

		private void button3_Click(object sender, EventArgs e)
		{
			for (int k = 0; k < 5000; k++)
			{
				for (int i = 0; i < 1; i++)
				{
					regNet.train(3);
					Text = k + "," + i;
					Application.DoEvents();
				}
				regNet.forward(Cifar.getImg(0));
				pictureBox1.Image = regNet.in_layer.out_act.visRGB();
				pictureBox4.Image = regNet.ucv2.out_act.visRGB();


				regNet.forward(Cifar.getImg(1));
				pictureBox2.Image = regNet.in_layer.out_act.visRGB();
				pictureBox3.Image = regNet.ucv2.out_act.visRGB();

				regNet.forward(Cifar.getImg(2));
				pictureBox5.Image = regNet.in_layer.out_act.visRGB();
				pictureBox6.Image = regNet.ucv2.out_act.visRGB();

				//Util.save("net.txt", (s) =>
				//{
				//	mainNet.cv1.save(s);
				//	mainNet.cv2.save(s);
				//	mainNet.cv3.save(s);
				//	mainNet.fc144.save(s);
				//});
				if (stop) { stop = false; break; }

			}

		}

	}
}
