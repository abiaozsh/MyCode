using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace Stack
{
	public partial class Form1 : Form
	{
		string[] files;
		int x;
		int y;
		int width;
		int height;

		public Form1()
		{
			InitializeComponent();
		}

		private void Form1_Load(object sender, EventArgs e)
		{
		}
		void pictureBox1_MouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			if (e.Button != System.Windows.Forms.MouseButtons.None)
			{
				pictureBox1.Refresh();
				pictureBox2.Location = new Point(e.X + pictureBox1.Location.X, e.Y + pictureBox1.Location.Y);
				this.Text = e.X + "," + e.Y;
				x = e.X;
				y = e.Y;

			}
		}


		void Form1_DragDrop(object sender, System.Windows.Forms.DragEventArgs e)
		{
			var dir = (string[])e.Data.GetData(DataFormats.FileDrop);

			files = Directory.GetFiles(dir[0]);

			Image bmp = Image.FromFile(files[0]);

			width = bmp.Width;
			height = bmp.Height;

			pictureBox1.Image = bmp;

			pictureBox2.BringToFront();
			pictureBox1.SendToBack();

		}


		void Form1_DragEnter(object sender, System.Windows.Forms.DragEventArgs e)
		{
			e.Effect = DragDropEffects.All;
		}

		private void pictureBox1_Click(object sender, EventArgs e)
		{

		}

		private void button1_Click(object sender, EventArgs e)
		{
			this.Hide();
			Form2 f = new Form2();
			f.files = files;
			f.basex = x;
			f.basey = y;
			f.width = width;
			f.height = height;

			f.Show();
		}

	}
}
