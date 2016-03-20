using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Stack
{
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();
		}

		private void Form1_Load(object sender, EventArgs e)
		{
			Image bmp = Image.FromFile(@"E:\bmp\aa000.bmp");

			pictureBox1.Image = bmp;

            pictureBox2.BringToFront();
			pictureBox1.SendToBack();
		}
		void pictureBox1_MouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			if (e.Button != System.Windows.Forms.MouseButtons.None)
			{
				pictureBox1.Refresh();
                pictureBox2.Location = new Point(e.X + pictureBox1.Location.X, e.Y + pictureBox1.Location.Y);
				this.Text = e.X+"," + e.Y;
			}
		}

	}
}
