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
            bb();
		}

		private void aa()
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

        private void bb()
        {
            Bitmap bmp = new Bitmap(@"..\..\..\..\bitmap.png");
            StringBuilder sb = new StringBuilder();

                for (int j = 0; j < 48; j++)
                {
                    string sbyte0 = "";
                    string sbyte1 = "";
                    string sbyte2 = "";
                    string sbyte3 = "";
                    string sbyte4 = "";
                    string sbyte5 = "";
                    int byte0 = 0;
                    int byte1 = 0;
                    int byte2 = 0;
                    int byte3 = 0;
                    int byte4 = 0;
                    int byte5 = 0;
                    for (int k = 0; k < 8; k++)
                    {
                        int val0 = bmp.GetPixel(j, k).R > 128 ? 0 : 1;
                        sbyte0 = val0 + sbyte0;
                        byte0 |= val0 << k;
                        int val1 = bmp.GetPixel(j, k + 8).R > 128 ? 0 : 1;
                        sbyte1 = val1 + sbyte1;
                        byte1 |= val1 << k;
                        int val2 = bmp.GetPixel(j, k + 16).R > 128 ? 0 : 1;
                        sbyte2 = val2 + sbyte2;
                        byte2 |= val2 << k;
                        int val3 = bmp.GetPixel(j, k + 24).R > 128 ? 0 : 1;
                        sbyte3 = val3 + sbyte3;
                        byte3 |= val3 << k;
                        int val4 = bmp.GetPixel(j, k + 32).R > 128 ? 0 : 1;
                        sbyte4 = val4 + sbyte4;
                        byte4 |= val4 << k;
                        int val5 = bmp.GetPixel(j, k + 40).R > 128 ? 0 : 1;
                        sbyte5 = val5 + sbyte5;
                        byte5 |= val5 << k;
                    }
                    sb.Append("//" + sbyte0 + "," + sbyte1 + "," + sbyte2 + "," + sbyte3 + "," + sbyte4 + "," + sbyte5 + "\r\n");
                    sb.Append(string.Format("0x{0:X2}", byte0) + ",");
                    sb.Append(string.Format("0x{0:X2}", byte1) + ",");
                    sb.Append(string.Format("0x{0:X2}", byte2) + ",");
                    sb.Append(string.Format("0x{0:X2}", byte3) + ",");
                    sb.Append(string.Format("0x{0:X2}", byte4) + ",");
                    sb.Append(string.Format("0x{0:X2}", byte5) + ",");
                    sb.Append("\r\n");
                }
            textBox2.Text = sb.ToString();
        }
    }
}
