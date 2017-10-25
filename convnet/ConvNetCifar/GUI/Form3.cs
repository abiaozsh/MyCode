using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

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
		private void Form3_Load(object sender, EventArgs e)
		{
			cifar.init();
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
			this.BackgroundImage = cifar.getBmp(idx);
		}

		private void button1_Click(object sender, EventArgs e)
		{
			for (int k = 0; k < 2000; k++)
			{
				for (int j = 0; j < 100; j++)
				{
					cifar.mainNet.train();
					Text = j.ToString();
					Application.DoEvents();
				}
				FileStream fs = new FileStream("cifar" + textBox1.Text + k + ".txt", FileMode.Create, FileAccess.Write);
				StreamWriter sw = new StreamWriter(fs);
				cifar.mainNet.save(sw);
				sw.Flush();
				fs.Flush();
				fs.Close();
			}

		}

		private void button2_Click(object sender, EventArgs e)
		{
			{
				FileStream fs = new FileStream(textBox1.Text + ".txt", FileMode.Open, FileAccess.Read);
				StreamReader sr = new StreamReader(fs);
				cifar.mainNet.load(sr);
				fs.Close();
			}

			Bitmap b = cifar.display(this, 200);

			b.Save(@"e:\out.jpg", ImageFormat.Jpeg);
		}
	}
}
