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

namespace GUI
{
	public partial class Form4 : Form
	{
		public Form4()
		{
			InitializeComponent();
		}
		MNIST3D mnist = new MNIST3D();
		private void Form4_Load(object sender, EventArgs e)
		{
			mnist.Init();

		}

		int[] status = new int[4];

		private void timer1_Tick(object sender, EventArgs e)
		{
			this.Text = status[0] + "," + status[1] + "," + status[2] + "," + status[3];
		}

		public delegate void AddHandler();

		private void button1_Click(object sender, EventArgs e)
		{
			//AddHandler handler = new AddHandler(() =>
			//{
			//	Parallel.For(0, 3, (num) =>
			//	{
			//		MNIST3D mymnist3d = new MNIST3D();
			//		mymnist3d.Init();
					for (int k = 0; k < 40; k++)
					{
						for (int j = 0; j < 1000; j++)
						{
							mnist.coTrainNet.train();
							//status[num] = j;
							Text = ""+j;
							Application.DoEvents();
						}
						FileStream fs = new FileStream("mnist3d" + textBox1.Text + "_" + k + ".txt", FileMode.Create, FileAccess.Write);
						StreamWriter sw = new StreamWriter(fs);
						mnist.coTrainNet.save(sw);
						sw.Flush();
						fs.Flush();
						fs.Close();
					}
			//	});
			//});

			//handler.BeginInvoke(null,null);
			
		}

		private void button2_Click(object sender, EventArgs e)
		{
			{
				FileStream fs = new FileStream(textBox1.Text + ".txt", FileMode.Open, FileAccess.Read);
				StreamReader sr = new StreamReader(fs);
				mnist.coTrainNet.load(sr);
				fs.Close();
			}

			{
				FileStream fs = new FileStream("data.txt", FileMode.Create, FileAccess.Write);
				StreamWriter sw = new StreamWriter(fs);
				string txt = mnist.display();
				sw.Write(txt);
				sw.Flush();
				fs.Flush();
				fs.Close();
			}



		}

	}
}
