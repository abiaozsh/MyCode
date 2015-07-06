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
			Bitmap bitmap = new Bitmap("..\\..\\..\\..\\无标题.bmp");//100*100
			Bitmap outbitmap = new Bitmap(505, 505);//"..\\..\\out.bmp"
			Graphics g = Graphics.FromImage(outbitmap);
			int size = 1000;
			int leds = 10;

			int[] data = new int[size * leds];
			for (int i = 0; i < size; i++)
			{
				double angle = Math.PI / 2 - Math.PI / size * i * 2;
				for (int j = 0; j < leds; j++)
				{
					double x = 50 + Math.Sin(angle) * (50 / leds) * j;
					double y = 50 + Math.Cos(angle) * (50 / leds) * j;
					var c = bitmap.GetPixel((int)x, (int)y);
					if (c.B > 128)
					{
						data[i * leds + j] = 1;
					}
					else
					{
						data[i * leds + j] = 0;
					}
				}
			}

			Pen p = new Pen(Color.Black);
			for (int i = 0; i < size; i++)
			{
				for (int j = 0; j < leds; j++)
				{
					if (data[i * leds + j] == 0)
					{
						g.DrawArc(p, 250f - (500f / leds) * j / 2, 250f - (500f / leds) * j / 2, (500f / leds) * j+1, (500f / leds) * j+1, (360f / size * i), 360f / size);

					}
				}
			}
			g.Flush();
			outbitmap.Save("..\\..\\..\\..\\out.bmp");

			StringBuilder sb = new StringBuilder();

			sb.Append("//    00,01,02,03,04,05,06,07,08,09,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51\r\n");
			for (int i = 0; i < 100; i++)
			{
			}
			textBox1.Text = sb.ToString().TrimEnd(',');
		}
	}
}
