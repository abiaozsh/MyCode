using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();
		}

		private void Form1_Load(object sender, EventArgs e)
		{
		}

		private void button1_Click(object sender, EventArgs e)
		{
			Bitmap bitmap = new Bitmap("d:\\无标题.bmp");

			StringBuilder sb = new StringBuilder();

			sb.Append("//    00,01,02,03,04,05,06,07,08,09,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51\r\n");
			for (int i = 0; i < 100; i++)
			{
				if (i < 10)
				{
					sb.Append("/*0" + i + "*/");
				}
				else
				{
					sb.Append("/*" + i + "*/");
				}
				for (int j = 0; j < 51; j++)
				{
					var c = bitmap.GetPixel(i, j);
					if ((c.ToArgb() & 0xFFFFFF) != 0)
					{
						sb.Append("01,");
					}
					else
					{
						sb.Append("00,");
					}
				}
				sb.Append("\r\n");
			}
			textBox1.Text = sb.ToString().TrimEnd(',');
		}
	}
}
