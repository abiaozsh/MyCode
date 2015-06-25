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

		private void timer1_Tick(object sender, EventArgs e)
		{
			DateTime now = DateTime.Now;
			textBox2.Text = now.ToString("yyyy-MM-dd HH:mm:ss");
			textBox1.Text = "33,";
			textBox1.Text += ((now.Second / 10) * 16 + (now.Second % 10)) + ",";
			textBox1.Text += ((now.Minute / 10) * 16 +  (now.Minute % 10)) + ",";
			textBox1.Text += ((now.Hour / 10) * 16 + (now.Hour % 10)) + ",";
			textBox1.Text += ((now.Day / 10) * 16 +  (now.Day % 10)) + ",";
			textBox1.Text += ((now.Month / 10) * 16 +  (now.Month % 10)) + ",";
			textBox1.Text += (((now.Year / 10) % 10) * 16 +  (now.Year % 10)) + ",";
			textBox1.Text += now.DayOfWeek.GetHashCode() + "";
			textBox1.SelectAll();
		}
	}
}
