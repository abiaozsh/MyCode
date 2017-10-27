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
	public partial class TrainFirst3Layer : Form
	{
		public TrainFirst3Layer()
		{
			InitializeComponent();
		}

		MNIST.First3CVNet first3CVNet;
		private void TrainFirst3Layer_Load(object sender, EventArgs e)
		{
			first3CVNet = new MNIST.First3CVNet();
			first3CVNet.init();
		}

		MNIST mnist = new MNIST();


		private void button1_Click(object sender, EventArgs e)
		{
			float accu = 0;
			for (int k = 0; k < 5; k++)
			{
				for (int i = 0; i < 50; i++)
				{
					first3CVNet.train();
					Text = k + "," + i + "," + accu;
					Application.DoEvents();
				}
				accu = first3CVNet.test();
				Text = k + ",0," + accu;


				Util.save("net.txt", (s) =>
				{
					first3CVNet.cv1.save(s);
					first3CVNet.cv2.save(s);
					first3CVNet.cv3.save(s);
					first3CVNet.fc144.save(s);
				});

			}


		}
	}
}
