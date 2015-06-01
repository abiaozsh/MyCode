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

			try
			{
				this.FormClosed += new FormClosedEventHandler(Form1_FormClosed);

				fs = new FileStream("d:\\data22.txt", FileMode.Create, FileAccess.Write);
				sw = new StreamWriter(fs);
				if (port == null)
				{
					//COM4为Arduino使用的串口号，需根据实际情况调整
					port = new SerialPort("COM12", 9600, Parity.None, 8, StopBits.One);
					port.Open();
				}
			}
			catch
			{
				MessageBox.Show("error");
			}
		}

		void Form1_FormClosed(object sender, FormClosedEventArgs e)
		{
			if (port != null && port.IsOpen)
			{
				port.Close();
			}
		}

		private void PortWrite(int chn, int data)
		{
			byte[] d = new byte[2];
			d[0] = (byte)(0x050 | chn);
			d[1] = (byte)(data & 0x0FF);
			if (port != null && port.IsOpen)
			{
				port.Write(d, 0, 1);
				Thread.Sleep(10);
				port.Write(d, 1, 1);
			}
		}

		private void Form1_Load(object sender, EventArgs e)
		{
		}

		private void trackBar1_Scroll(object sender, EventArgs e)
		{
			int v = trackBar1.Value;
			if (checkBox1.Checked)
			{
				this.Text = v.ToString();
				PortWrite(1, v);
			}
			else
			{
				this.Text = (2000 + (v >> 3)).ToString();
				PortWrite(5, v);
			}
		}

		private void trackBar2_Scroll(object sender, EventArgs e)
		{
			int v = trackBar2.Value;
			if (checkBox1.Checked)
			{
				this.Text = v.ToString();
				PortWrite(2, v);
			}
			else
			{
				this.Text = (2000 + (v >> 3)).ToString();
				PortWrite(6, v);
			}
		}

		private void trackBar3_Scroll(object sender, EventArgs e)
		{
			int v = trackBar3.Value;
			if (checkBox1.Checked)
			{
				this.Text = v.ToString();
				PortWrite(3, v);
			}
			else
			{
				this.Text = (2000 + (v >> 3)).ToString();
				PortWrite(7, v);
			}
		}

		private void trackBar4_Scroll(object sender, EventArgs e)
		{
			int v = trackBar4.Value;
			if (checkBox1.Checked)
			{
				this.Text = v.ToString();
				PortWrite(4, v);
			}
			else
			{
				this.Text = (2000 + (v >> 3)).ToString();
				PortWrite(8, v);
			}
		}


		public void aa()
		{
			//string s = "";
			string s2 = "";
			//double start = 300;//rpm
			//double end = 20000;//rpm

			double start = 39;//rpm
			double end = 600;//rpm

			//8000tick/p ->
			//1000p/8s ->
			//

			double count = 128;
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
				inc *= 0.983;
			}

			textBox1.Text = s2;
		}

		private void button1_Click(object sender, EventArgs e)
		{
			aa();
		}

	}
}


