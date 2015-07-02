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

		private void timer1_Tick(object sender, EventArgs e)
		{
			Bitmap bmp = new Bitmap(@"..\..\..\..\font.png");
			StringBuilder sb = new StringBuilder();
			for (int i = 0; i < 10; i++)
			{
				sb.Append("//" + (i + 1) + "\r\n");
				for (int j = 0; j < 12; j++)
				{
					string sbyte0 = "";
					string sbyte1 = "";
					string sbyte2 = "";
					int byte0 = 0;
					int byte1 = 0;
					int byte2 = 0;
					for (int k = 0; k < 8; k++)
					{
						int val0 = bmp.GetPixel(i * 11 + j, k).R > 128 ? 0 : 1;
						sbyte0 = val0 + sbyte0;
						byte0 |= val0 << k;
						int val1 = bmp.GetPixel(i * 11 + j, k + 8).R > 128 ? 0 : 1;
						sbyte1 = val1 + sbyte1;
						byte1 |= val1 << k;
						int val2 = bmp.GetPixel(i * 11 + j, k + 16).R > 128 ? 0 : 1;
						sbyte2 = val2 + sbyte2;
						byte2 |= val2 << k;
					}
					sb.Append("//" + sbyte2 + "," + sbyte1 + "," + sbyte0 + "\r\n");
					sb.Append(string.Format("0x{0:X2}", byte0) + ",");
					sb.Append(string.Format("0x{0:X2}", byte1) + ",");
					sb.Append(string.Format("0x{0:X2}", byte2) + ",");
					sb.Append("\r\n");
				}
			}
			textBox1.Text = sb.ToString();
		}
	}
}
