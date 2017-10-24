using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
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
		Cifar c = new Cifar();
		private void Form3_Load(object sender, EventArgs e)
		{

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
			this.Text = classify[c.getLbl(idx)];
			this.BackgroundImage = c.getBmp(idx);
		}
	}
}
