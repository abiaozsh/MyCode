using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace WindowsFormsApplication1
{
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();
		}

		public struct DATAS
		{
			public DateTime time;
			public int temp;
			public int pres;
		}

		static int w = 1000;
		static int h = 300;
		double x = w / 2;
		double y = h / 2;
		double sx = 0.05;
		double sy = 0.5;

		private void Form1_Load(object sender, EventArgs e)
		{
		}

		private void button8_Click(object sender, EventArgs e)
		{
			sy *= 1.1; draw();
		}
		private void button6_Click(object sender, EventArgs e)
		{
			sy *= 0.9; draw();
		}

		List<DATAS> datas = new List<DATAS>();
		DateTime?[] dates;
		int?[] values;
		static Color[] clist = { Color.Red, Color.Blue, Color.Green, Color.Purple, Color.Pink, Color.Yellow };
		void draw()
		{
			Bitmap bmp = new Bitmap(w, h);
			dates = new DateTime?[w];
			values = new int?[w];
			//bmp.SetPixel(100,100,Color.Red);
			var starttime = DateTime.MaxValue;
			var startval = int.MaxValue;
			var endtime = DateTime.MinValue;
			var endval = int.MinValue;
			double Scale = 5;

			foreach (var data in datas)
			{
				if (startval > data.pres)
				{
					startval = data.pres;
				}
				if (starttime > data.time)
				{
					starttime = data.time;
				}
				if (endval < data.pres)
				{
					endval = data.pres;
				}
				if (endtime < data.time)
				{
					endtime = data.time;
				}
			}
			var starttime0 = starttime;
			starttime = DateTime.FromBinary(starttime.ToBinary() / 2 + endtime.ToBinary() / 2);
			startval = startval / 2 + endval / 2;
			foreach (var data in datas)
			{
				double xxx = ((data.time.Ticks - starttime.Ticks) / 10 / 1000 / 1000) / Scale;
				double yyy = (startval - data.pres) / 10f;

				xxx *= sx;
				yyy *= sy;
				xxx += x;
				yyy += y;

				if (xxx >= 0 && xxx < w && yyy >= 0 && yyy < h)
				{
					bmp.SetPixel((int)xxx, (int)yyy, ((data.time.Date.Subtract(starttime0.Date).Days & 1) == 1) ? Color.Red : Color.Blue);
				}
				if (xxx >= 0 && xxx < w)
				{
					dates[(int)xxx] = data.time;
					values[(int)xxx] = data.pres;
				}
			}

			pictureBox1.Image = bmp;
		}
		void Form1_MouseWheel(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			if (e.Delta > 0)
			{
				sx *= 1.1;
				sy *= 1.1;
			}
			else
			{
				sx *= 0.9;
				sy *= 0.9;
			}
			draw();
		}

		int tx;
		int ty;
		void pictureBox1_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			x += (e.X - tx);
			y += (e.Y - ty);
			draw();
		}

		void pictureBox1_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			tx = e.X;
			ty = e.Y;
		}

		void pictureBox1_MouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			if (dates != null && dates[e.X] != null)
			{
				textBox3.Text = dates[e.X].Value.ToString("yyyy-MM-dd HH:mm:ss");
				textBox4.Text = values[e.X].Value.ToString();
			}
		}

		void Form1_DragEnter(object sender, System.Windows.Forms.DragEventArgs e)
		{
			e.Effect = DragDropEffects.All;
		}

		void Form1_DragDrop(object sender, System.Windows.Forms.DragEventArgs e)
		{
			string[] files = (string[])e.Data.GetData("FileDrop");

			datas = new List<DATAS>();
			foreach (string fn in files)
			{
				try
				{
					FileStream fs = new FileStream(fn, FileMode.Open, FileAccess.Read);
					StreamReader sr = new StreamReader(fs);
					while (true)
					{
						string line = sr.ReadLine();
						if (line == null) break;
						if (line.Contains(":"))
						{
							string filename = fn.Substring(fn.LastIndexOf("\\") + 1);
							DateTime t = DateTime.Parse(filename.Substring(0, 4) + "-" + filename.Substring(4, 2) + "-" + filename.Substring(6, 2) + " " + line);
							string line0 = sr.ReadLine();
							int temp = int.Parse(line0.Split(',')[0]);
							int pres = int.Parse(line0.Split(',')[1]);
							int totalT = temp;
							int totalP = pres;
							for (int i = 1; i < 50; i++)
							{
								string linen = sr.ReadLine();
								int tempn = temp + int.Parse(linen.Split(',')[0]);
								int presn = pres + int.Parse(linen.Split(',')[1]);
								totalT += tempn;
								totalP += presn;
							}
							DATAS ds = new DATAS();
							ds.time = t;
							ds.pres = totalP / 5;
							ds.temp = totalT / 5;
							datas.Add(ds);
						}
					}
				}
				catch (Exception ex)
				{
				}
			}
			draw();
		}

	}
}
