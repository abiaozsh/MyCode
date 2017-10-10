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
		private void Form1_Load(object sender, EventArgs e)
		{
			//this.MouseWheel += Form1_MouseWheel;
			try
			{
				mnist.Init();
				for (int i = 0; i < 1000; i++)
				{
					mnist.train(i);
				}

				for (int i = 0; i < 50; i++)
				{
					int val = mnist.getLbl(2000+i);
					var v1 = mnist.net.forward(mnist.getImg(2000+i), false);
					String s = v1.w.ToString();
					Console.WriteLine(val);
				}

			}
			catch (Exception ex)
			{
				ex.ToString();
			}
		}

		//int idx = 10000;
		//void Form1_MouseWheel(object sender, MouseEventArgs e)
		//{
		//if(e.Delta>0)idx++;else idx--;
		//pictureBox1.Image = mnist.getBmp(idx);
		//
		//this.Text = idx + "," + mnist.getLbl(idx);
		//
		//}

		private void pictureBox1_Click(object sender, EventArgs e)
		{

		}
	}
}
