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
			Bitmap bitmap = new Bitmap(textBox3.Text);

			StringBuilder sb = new StringBuilder();

			sb.Append("int cnt = " + bitmap.Width + ";\r\n");
			sb.Append("PROGMEM prog_uint8_t data[] = {");
			for (int i = 0; i < bitmap.Width; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					var c = bitmap.GetPixel(i, j);
					byte br;
					byte bg;
					byte bb;
					if(checkBox1.Checked)
					{
						br = (byte)(int.Parse(textBox2.Text) - ((int)c.R) * int.Parse(textBox2.Text) / 256 - 1);
						bg = (byte)(int.Parse(textBox2.Text) - ((int)c.G) * int.Parse(textBox2.Text) / 256 - 1);
						bb = (byte)(int.Parse(textBox2.Text) - ((int)c.B) * int.Parse(textBox2.Text) / 256 - 1);
					}
					else
					{
						br = (byte)(((int)c.R) * int.Parse(textBox2.Text) / 256);
						bg = (byte)(((int)c.G) * int.Parse(textBox2.Text) / 256);
						bb = (byte)(((int)c.B) * int.Parse(textBox2.Text) / 256);
					}
					sb.Append("0x" + (br).ToString("X2") + ",");
					sb.Append("0x" + (bg).ToString("X2") + ",");
					sb.Append("0x" + (bb).ToString("X2") + ",");
				}
			}
			textBox1.Text = sb.ToString().TrimEnd().TrimEnd(',') + "};\r\n";
		}
	}
}
