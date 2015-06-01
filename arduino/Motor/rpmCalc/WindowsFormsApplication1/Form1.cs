using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.Threading;
using System.IO;

namespace WindowsFormsApplication1
{

	public partial class Form1 : Form
	{
		SerialPort port;
		FileStream fs;
		StreamWriter sw;
		private delegate void FlushClient(string s);
		public Form1()
		{
			InitializeComponent();
		}

		void Form1_FormClosed(object sender, FormClosedEventArgs e)
		{
		}

		private void Form1_Load(object sender, EventArgs e)
		{
		}

		public string aa(double start, double end)
		{
			string s2 = "";
			//8000tick/p ->
			//1000p/8s ->
			double count = 256;
			double targetrpm = start;
			double inc = ((end - start)) / count;
			for (double i = 0; i < count; i++)
			{
				double rps = targetrpm / 60;
				double rpms = rps / 1000;
				double rpus = rpms / 1000;
				double uspr = 1 / rpus;
				double uspp3 = uspr / 18;
				double uspp4 = uspr / 24;
				//s += targetrpm + ",";
				s2 += (int)(uspp4) + ",";
				targetrpm += inc;
				//inc *= 0.995;
			}
			return s2;
		}

		private void button1_Click(object sender, EventArgs e)
		{
			textBox1.Text = aa(39, 9800);
			textBox2.Text = aa(9800, 40000);
		}

	}
}


