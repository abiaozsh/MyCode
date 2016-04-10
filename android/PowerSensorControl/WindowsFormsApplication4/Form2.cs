using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace WindowsFormsApplication4
{
	public partial class Form2 : Form
	{
		public Form2()
		{
			InitializeComponent();
		}

		struct Data
		{
			public DateTime t;
			public double p;
			public long raw;
		}

		List<Data> datas = new List<Data>();
		DateTime start;
		DateTime end;

		private void button1_Click(object sender, EventArgs e)
		{
			LoadData(@"E:\data\0409.txt");
			LoadData(@"E:\data\0408.txt");
            LoadData(@"E:\data\0410.txt");


			datas.Sort(delegate(Data a, Data b)
			{
				return a.raw.CompareTo(b.raw);
			});

			long lastval = 0;
			for (int i = 0; i < datas.Count; i++)
			{
				Data d = datas[i];
				d.p = 2250000.0 / (d.raw - lastval);// (w)
				datas[i] = d;
				lastval = d.raw;
			}

			start = datas[0].t;
			end = datas[datas.Count - 1].t;

			Draw();

			textBox1.KeyPress += new KeyPressEventHandler(textBox1_KeyPress);
			pictureBox1.MouseMove += new MouseEventHandler(pictureBox1_MouseMove);
			pictureBox1.MouseDown += new MouseEventHandler(pictureBox1_MouseDown);
			pictureBox1.MouseUp += new MouseEventHandler(pictureBox1_MouseUp);
			pictureBox1.MouseWheel += new MouseEventHandler(pictureBox1_MouseWheel);
		}

		void LoadData(string file)
		{
			FileStream fs = new FileStream(file, FileMode.Open, FileAccess.Read);
			StreamReader sr = new StreamReader(fs);

			while (true)
			{
				string line = sr.ReadLine();
				if (line == null) break;
				long val = long.Parse(line);

				Data d;
				d.t = new DateTime(1970, 1, 1).AddHours(8).AddMilliseconds(val);
				d.raw = val;
				d.p = 0;

				datas.Add(d);
			}
			fs.Close();
		}

		void pictureBox1_MouseWheel(object sender, MouseEventArgs e)
		{

		}

		void pictureBox1_MouseUp(object sender, MouseEventArgs e)
		{
			double ms = end.Subtract(start).TotalMilliseconds;
			ms = ms * (oldx - e.X) / 1024;
			start = start.AddMilliseconds(ms);
			end = end.AddMilliseconds(ms);
			Draw();
		}

		int oldx;
		void pictureBox1_MouseDown(object sender, MouseEventArgs e)
		{
			oldx = e.X;
		}

		void pictureBox1_MouseMove(object sender, MouseEventArgs e)
		{
			double ms = end.Subtract(start).TotalMilliseconds;
			double currentms = ms * e.X / 1024.0;
			DateTime current = start.AddMilliseconds(currentms);
			label1.Text = current.ToString("MM-dd HH:mm:ss fff") + "\r\n" + tempData[e.X].ToString();
			label1.Top = e.Y;
			label1.Left = e.X;

		}


		double[] tempData = new double[1024];

		void textBox1_KeyPress(object sender, KeyPressEventArgs e)
		{
			double ms = end.Subtract(start).TotalMilliseconds;
			ms *= 0.1;
			if (e.KeyChar == 'w')
			{
				start = start.AddMilliseconds(ms);
				end = end.AddMilliseconds(-ms);
			}
			if (e.KeyChar == 's')
			{
				start = start.AddMilliseconds(-ms);
				end = end.AddMilliseconds(ms);
			}
			Draw();
		}

		void Form2_KeyPress(object sender, KeyPressEventArgs e)
		{
		}

		void Draw()
		{
			Bitmap b = new Bitmap(1024, 500);
			Graphics g = Graphics.FromImage(b);
			Pen p = new Pen(Color.Red);

			double ms = end.Subtract(start).TotalMilliseconds;
			for (int i = 0; i < 1024; i++)
			{
				double currentms = ms * i / 1024.0;
				DateTime current = start.AddMilliseconds(currentms);

				double power = 0;
				for (int j = 0; j < datas.Count; j++)
				{
					if (current.CompareTo(datas[j].t) < 0)
					{
						power = datas[j].p;
						break;
					}
				}

				tempData[i] = power;

				g.DrawLine(p, i, 500, i, (float)(500 - power / 10));

			}
			g.Flush();
			pictureBox1.Image = b;
		}

		private void Form2_Load(object sender, EventArgs e)
		{

		}

		private void pictureBox1_Click(object sender, EventArgs e)
		{

		}

	}
}
