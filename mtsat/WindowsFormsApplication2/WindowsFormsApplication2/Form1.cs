using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net.Sockets;
using System.Net;
using System.IO;
using ICSharpCode.SharpZipLib.Zip;


namespace WindowsFormsApplication2
{
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();
		}

		string root = "";
		int x = 0;
		int y = 0;
		int oldx = 0;
		int oldy = 0;


		private void Form1_Load(object sender, EventArgs e)
		{
		}

		void Form1_Resize(object sender, System.EventArgs e)
		{
			draw(DateTime.Parse(textBox1.Text), comboBox1.Text);
		}

		void Form1_DragDrop(object sender, System.Windows.Forms.DragEventArgs e)
		{
			string[] files = (string[])e.Data.GetData("FileDrop");
			root = files[0];
			this.Text = root;
		}

		void Form1_DragEnter(object sender, System.Windows.Forms.DragEventArgs e)
		{
			e.Effect = DragDropEffects.All;
		}

		private void button1_Click(object sender, EventArgs e)
		{
			DateTime d = DateTime.Parse(textBox1.Text);
			d=d.AddHours(-1);
			textBox1.Text = d.ToString("yyyy-MM-dd HH:mm:ss");
			draw(d, comboBox1.Text);
		}

		public void draw(DateTime d, string type)
		{
			if (root == "") return;
			Bitmap bmp = new Bitmap(pictureBox1.Width, pictureBox1.Height);

			Image img = null;

			using (var input = new FileStream(root + "\\" + d.ToString("yyyyMM") + "\\" + d.ToString("yyyyMMdd") + ".zip", FileMode.Open, FileAccess.Read))
			using (ZipInputStream zip = new ZipInputStream(input))
			{
				while (true)
				{
					ZipEntry entry = zip.GetNextEntry();
					if (entry == null) break;
					if (entry.Name.StartsWith(d.ToString("yyyyMMddHH") + type))
					{
						img = Bitmap.FromStream(zip);
						break;
					}
				}
			}

			Graphics g = Graphics.FromImage(bmp);
			if (img != null)
			{
				g.DrawImage(img, x, y);
				g.Flush();

				pictureBox1.Image = bmp;
			}
		}

		private void button4_Click(object sender, EventArgs e)
		{
			DateTime d = DateTime.Parse(textBox1.Text);
			d=d.AddYears(1);
			textBox1.Text = d.ToString("yyyy-MM-dd HH:mm:ss");
			draw(d, comboBox1.Text);
		}


		void pictureBox1_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			x += (e.X - oldx);
			y += (e.Y - oldy);
			draw(DateTime.Parse(textBox1.Text), comboBox1.Text);
		}

		void pictureBox1_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			oldx = e.X;
			oldy = e.Y;
		}

		private void button5_Click(object sender, EventArgs e)
		{
			DateTime d = DateTime.Parse(textBox1.Text);
			d = d.AddHours(1);
			textBox1.Text = d.ToString("yyyy-MM-dd HH:mm:ss");
			draw(d, comboBox1.Text);
		}

		private void button6_Click(object sender, EventArgs e)
		{
			DateTime d = DateTime.Parse(textBox1.Text);
			d = d.AddDays(1);
			textBox1.Text = d.ToString("yyyy-MM-dd HH:mm:ss");
			draw(d, comboBox1.Text);

		}

		private void button7_Click(object sender, EventArgs e)
		{
			DateTime d = DateTime.Parse(textBox1.Text);
			d = d.AddMonths(1);
			textBox1.Text = d.ToString("yyyy-MM-dd HH:mm:ss");
			draw(d, comboBox1.Text);

		}

		private void button8_Click(object sender, EventArgs e)
		{
			DateTime d = DateTime.Parse(textBox1.Text);
			d = d.AddYears(1);
			textBox1.Text = d.ToString("yyyy-MM-dd HH:mm:ss");
			draw(d, comboBox1.Text);

		}

		private void button2_Click(object sender, EventArgs e)
		{
			DateTime d = DateTime.Parse(textBox1.Text);
			d = d.AddDays(-1);
			textBox1.Text = d.ToString("yyyy-MM-dd HH:mm:ss");
			draw(d, comboBox1.Text);

		}

		private void button3_Click(object sender, EventArgs e)
		{
			DateTime d = DateTime.Parse(textBox1.Text);
			d = d.AddMonths(-1);
			textBox1.Text = d.ToString("yyyy-MM-dd HH:mm:ss");
			draw(d, comboBox1.Text);

		}

	}
}
