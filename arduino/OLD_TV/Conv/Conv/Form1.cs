using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Conv
{
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();
		}

		private void Form1_Load(object sender, EventArgs e)
		{
			Bitmap bmp = (Bitmap)Image.FromFile("F:\\2.png");

			String s = "";
			for (int j = 0; j < 16; j++)
			{

				int v1 = 0;
				for (int i = 0; i < 8; i++)
				{
					v1 += ((bmp.GetPixel(i + 0, j).R > 128) ? 0 : (1 << i));
				}
				int v2 = 0;
				for (int i = 0; i < 8; i++)
				{
					v2 += ((bmp.GetPixel(i + 8, j).R > 128) ? 0 : (1 << i));
				}
				s += "data[" + (j * 2 + 0) + "] = " + v1 + ";";
				s += "data[" + (j * 2 + 1) + "] = " + v2 + ";\r\n";
			}
			textBox1.Text = s;
		}
	}
}
