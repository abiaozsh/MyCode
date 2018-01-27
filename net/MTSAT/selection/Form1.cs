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


namespace selection
{
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();
		}


		private void Form1_Load(object sender, EventArgs e)
		{
			try{
			pictureBox1.Image = Bitmap.FromFile("..\\..\\earth.png");
			}catch{
			}
			try{
			pictureBox1.Image = Bitmap.FromFile("earth.png");
			}catch{
			}
			pictureBox1.MouseUp += new MouseEventHandler(pictureBox1_MouseUp);
		}

		void pictureBox1_MouseUp(object sender, MouseEventArgs e)
		{
			textBox2.Text = e.X * 20 + "," + e.Y * 20;
		}

		void pictureBox1_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			textBox1.Text = e.X * 20 + "," + e.Y * 20;
		}

		private void pictureBox1_Click(object sender, EventArgs e)
		{

		}


	}
}
