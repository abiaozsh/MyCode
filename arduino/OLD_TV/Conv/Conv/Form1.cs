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
		StringBuilder sb = new StringBuilder();

		private void Form1_Load(object sender, EventArgs e)
		{

			string[] files = System.IO.Directory.GetFiles("e:\\img\\");
			int idx = 0;
			sb = new StringBuilder();
			foreach (var item in files)
			{
				proc(item, idx);
				idx++;
				if (idx > 111800) { break; }
			}
			textBox1.Text = sb.ToString();
		}

		void proc(string file, int idx)
		{
			Bitmap bmp = (Bitmap)Image.FromFile(file);
			Bitmap bmp1 = new Bitmap(32, 24);
			Bitmap bmpout = new Bitmap(16, 16);

			using (Graphics g = Graphics.FromImage(bmp1))
			{
				g.DrawImage(bmp, 0, 0, new Rectangle(0, 4, 32, 28), GraphicsUnit.Pixel);
			}
			using (Graphics g = Graphics.FromImage(bmpout))
			{
				g.DrawImage(bmp1, 0, 0, 16, 16);
			}

			//bmpout.Save("e:\\img2\\" + idx + ".bmp");
			pic2txt(bmpout);
		}
		void pic2txt(Bitmap bmp)
		{

			String s = "";
			for (int j = 0; j < 16; j++)
			{

				int v1 = 0;
				for (int i = 0; i < 8; i++)
				{
					v1 += ((bmp.GetPixel(i + 0, j).R < 128) ? 0 : (1 << i));
				}
				int v2 = 0;
				for (int i = 0; i < 8; i++)
				{
					v2 += ((bmp.GetPixel(i + 8, j).R < 128) ? 0 : (1 << i));
				}
				sb.Append(v1 + ",");
				sb.Append(v2 + ",");
			}
			sb.AppendLine();
		}
	}
}
