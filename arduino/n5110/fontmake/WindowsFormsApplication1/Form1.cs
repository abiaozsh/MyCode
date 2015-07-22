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
			aa();
		}

		private void aa()
		{
			Bitmap bmp = new Bitmap(@"..\..\..\..\font.png");
			StringBuilder sb = new StringBuilder();
			for (int i = 0; i < 96; i++)
			{
				sb.Append("//" + ((char)(i + 32)) + "\r\n");
				for (int j = 0; j < 8; j++)
				{
					string sbyte0 = "";
					string sbyte1 = "";
					int byte0 = 0;
					int byte1 = 0;
					for (int k = 0; k < 8; k++)
					{
						int val0 = bmp.GetPixel(i * 8 + j + 1, k + 1).R > 128 ? 0 : 1;
						sbyte0 = (val0 == 1 ? "1" : "0") + sbyte0;
						byte0 |= val0 << k;
						int val1 = bmp.GetPixel(i * 8 + j + 1, k + 1 + 8).R > 128 ? 0 : 1;
						sbyte1 = (val1 == 1 ? "1" : "0") + sbyte1;
						byte1 |= val1 << k;
					}
					//sb.Append("//" + sbyte1 + "," + sbyte0 + "\r\n");
					sb.Append(string.Format("0x{0:X2}", byte0) + ",");
					sb.Append(string.Format("0x{0:X2}", byte1) + ",");
					//sb.Append("\r\n");
				}
				sb.Append("\r\n");
			}
			textBox1.Text = sb.ToString();
		}

	}
}
