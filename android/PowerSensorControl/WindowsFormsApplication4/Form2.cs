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
			public long glitch;
			public long range;
		}

		List<Data> datas = new List<Data>();
		double start;
		double end;

		private void button1_Click(object sender, EventArgs e)
		{
			string dir = @"E:\data\";

			string[] files = Directory.GetFiles(dir);

			foreach (string file in files)
			{
				LoadData(file);
			}

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

			if (true)
			{

				for (int i = 0; i < datas.Count - 7; i++)
				{
					long data0 = datas[i].raw;
					long data1 = datas[i + 1].raw;
					long data2 = datas[i + 2].raw;
					long data3 = datas[i + 3].raw;
					long data4 = datas[i + 4].raw;
					long data5 = datas[i + 5].raw;
					long data6 = datas[i + 6].raw;

					long space0 = data1 - data0;
					long space1 = data2 - data1;
					long space2 = data4 - data2;
					long space3 = data5 - data4;
					long space4 = data6 - data5;

					long avg = space0 + space1 + space3 + space4;
					long range = Math.Abs(space0 * 4 - avg) +
						Math.Abs(space1 * 4 - avg) +
						Math.Abs(space3 * 4 - avg) +
						Math.Abs(space4 * 4 - avg);
					Data temp = datas[i + 3];
					temp.range = range;
					temp.glitch = Math.Abs(space2 * 4 - avg);
					datas[i + 3] = temp;
				}




				StringBuilder report = new StringBuilder();

				for (int i = 0; i < datas.Count; i++)
				{
					long diff;
					if (i == 0)
					{
						diff = 0;
					}
					else
					{
						diff = (datas[i].raw - datas[i - 1].raw);
					}
					double avg = 0;
					if (i >= 2 && i < datas.Count - 2)
					{
						avg += datas[i - 2].glitch;
						avg += datas[i - 1].glitch;
						avg += datas[i + 1].glitch;
						avg += datas[i + 2].glitch;
					}
					report.Append(datas[i].raw).Append("\t");
					report.Append(diff).Append("\t");
					report.Append(datas[i].t.ToString("MM-dd HH:mm:ss fff")).Append("\t");
					report.Append(((float)datas[i].p)).Append("\t");
					report.Append(datas[i].range).Append("\t");
					report.Append(datas[i].glitch).Append("\t");
					report.Append((avg / datas[i].glitch)).Append("\t");
					report.Append(((avg / datas[i].glitch) > 40 ? "GGG" : ""));

					report.AppendLine();
				}
				//textBox2.Text = report.ToString();
			}

			List<Data> new_datas = new List<Data>();
			for (int i = 0; i < datas.Count; i++)
			{
				double avg = 0;
				if (i >= 2 && i < datas.Count - 2)
				{
					avg += datas[i - 2].glitch;
					avg += datas[i - 1].glitch;
					avg += datas[i + 1].glitch;
					avg += datas[i + 2].glitch;
				}
				if (avg / datas[i].glitch < 100)
				{
					new_datas.Add(datas[i]);
				}
			}

			datas = new_datas;

			start = datas[0].raw;
			end = datas[datas.Count - 1].raw;

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
				d.range = 0;
				d.glitch = 0;

				datas.Add(d);
			}
			fs.Close();
		}

		void pictureBox1_MouseWheel(object sender, MouseEventArgs e)
		{

		}

		void pictureBox1_MouseUp(object sender, MouseEventArgs e)
		{
			double ms = end - start;
			ms = ms * (oldx - e.X) / 1024;
			start = start + ms;
			end = end + ms;
			Draw();
		}

		int oldx;
		void pictureBox1_MouseDown(object sender, MouseEventArgs e)
		{
			oldx = e.X;
		}

		void pictureBox1_MouseMove(object sender, MouseEventArgs e)
		{
			double ms = end - start;
			double currentms = start + ms * e.X / 1024.0;
			DateTime current = new DateTime(1970, 1, 1).AddHours(8).AddMilliseconds(currentms);
			label1.Text = current.ToString("MM-dd HH:mm:ss fff") + "\r\n" + tempData[e.X].ToString();
			label1.Top = e.Y;
			label1.Left = e.X;
		}


		double[] tempData = new double[1024];

		void textBox1_KeyPress(object sender, KeyPressEventArgs e)
		{
			double ms = end - start;
			ms *= 0.1;
			if (e.KeyChar == 'w')
			{
				start = start + ms;
				end = end - ms;
			}
			if (e.KeyChar == 's')
			{
				start = start - ms;
				end = end + ms;
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
			Pen p1 = new Pen(Color.Red);
			Pen p2 = new Pen(Color.Green);

			double ms = end - start;
			for (int i = 0; i < 1024; i++)
			{
				double currentms = ms * i / 1024.0;
				double nextms = ms * (i + 1) / 1024.0;

				double current = start + currentms;

				double power = 0;
				int idxs = FindTop(start + currentms);
				int idxe = FindBottom(start + nextms);

				if (idxs != -1 && idxe != -1)
				{
					long diff = datas[idxe].raw - datas[idxs].raw;
					power = 2250000.0 / (diff / (idxe - idxs));
				}

				tempData[i] = power;

				if (((long)Math.Floor(((currentms + start) / 1000 / 3600 + 8) / 24)) % 2 == 0)
				{
					g.DrawLine(p1, i, 500, i, (float)(500 - power / 10));
				}
				else
				{
					g.DrawLine(p2, i, 500, i, (float)(500 - power / 10));
				}
			}
			g.Flush();
			pictureBox1.Image = b;
		}

		private int FindTop(double val)
		{
			int idx = datas.Count / 2;
			int max = datas.Count;
			int min = 0;
			while (true)
			{
				if (idx < 0 || idx + 1 >= datas.Count)
				{
					return -1;
				}
				if (val > datas[idx].raw && val <= datas[idx + 1].raw)
				{
					return idx;
				}
				else if (val <= datas[idx].raw)
				{
					int tmp = (min + idx) / 2;
					max = idx;
					if (tmp == idx) return -1;
					idx = tmp;
				}
				else if (val > datas[idx + 1].raw)
				{
					int tmp = (max + idx) / 2;
					min = idx;
					if (tmp == idx) return -1;
					idx = tmp;
				}
			}
		}
		private int FindBottom(double val)
		{
			int idx = datas.Count / 2;
			int max = datas.Count;
			int min = 0;
			while (true)
			{
				if (idx - 1 < 0 || idx >= datas.Count)
				{
					return -1;
				}
				if (val < datas[idx].raw && val >= datas[idx - 1].raw)
				{
					return idx;
				}
				else if (val >= datas[idx].raw)
				{
					int tmp = (max + idx) / 2;
					min = idx;
					if (tmp == idx) return -1;
					idx = tmp;
				}
				else if (val < datas[idx - 1].raw)
				{
					int tmp = (min + idx) / 2;
					max = idx;
					if (tmp == idx) return -1;
					idx = tmp;
				}
			}
		}



		private void Form2_Load(object sender, EventArgs e)
		{

		}

		private void pictureBox1_Click(object sender, EventArgs e)
		{

		}

	}
}
