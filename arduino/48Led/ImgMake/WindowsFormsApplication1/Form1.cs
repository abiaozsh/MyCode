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
            bb();
		}

        private void bb()
        {
            Bitmap bmp = new Bitmap(@"..\..\..\..\bitmap.png");
            StringBuilder sb = new StringBuilder();

            for (int j = 0; j < bmp.Width; j++)
			{
                for (int k = 0; k < 48; k++)
                {
					int val0 = bmp.GetPixel(j, k).R;
					sb.Append(string.Format("0x{0:X2}", val0) + ",");
				}
				sb.Append("\r\n");
			}

            textBox2.Text = sb.ToString();
        }
    }
}
