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

		MNIST.MainNet mainNet;
		private void TrainFirst3Layer_Load(object sender, EventArgs e)
		{
			mainNet = new MNIST.MainNet();
			mainNet.init();
		}

		MNIST mnist = new MNIST();


		private void button1_Click(object sender, EventArgs e)
		{
			float accu = 0;
			for (int k = 0; k < 5; k++)
			{
				for (int i = 0; i < 50; i++)
				{
					mainNet.train();
					Text = k + "," + i + "," + accu;
					Application.DoEvents();
				}
				accu = mainNet.test();
				Text = k + ",0," + accu;


				Util.save("net.txt", (s) =>
				{
					mainNet.cv1.save(s);
					mainNet.cv2.save(s);
					mainNet.cv3.save(s);
					mainNet.fc144.save(s);
				});

			}


		}
	}
}
