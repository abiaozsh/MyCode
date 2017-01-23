using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.Threading;
using System.IO;
using System.Globalization;

namespace WindowsFormsApplication1
{

	public partial class Form1 : Form
	{
		SerialPort port;
		public Form1()
		{
			InitializeComponent();

			this.FormClosed += new FormClosedEventHandler(Form1_FormClosed);
		}

		byte[] buff = new byte[1024 * 1024];
		int buffIdx = 0;

		public void MyThread()
		{
			StringBuilder _sb = new StringBuilder();
			while (true)
			{
				try
				{
					while (port.BytesToRead > 0)
					{
						byte c = (byte)port.ReadByte();
						buff[buffIdx++] = c;
					}

					Thread.Sleep(10);
				}
				catch (Exception ex) { }
			}

		}

		delegate void Cinvokes();

		private void Updata()
		{
		}

		void Form1_FormClosed(object sender, FormClosedEventArgs e)
		{
			if (port != null && port.IsOpen)
			{
				port.Close();
			}
		}

		private void Form1_Load(object sender, EventArgs e)
		{
		}

		private void button1_Click(object sender, EventArgs e)
		{
			if (port == null)
			{
				//COM4为Arduino使用的串口号，需根据实际情况调整
				port = new SerialPort(textBox1.Text, 115200, Parity.None, 8, StopBits.One);
				port.Open();

				ThreadStart thr_start_func = new ThreadStart(MyThread);
				Thread fThread = new Thread(thr_start_func);
				fThread.Start();
			}

		}

		void send(byte d, int val)
		{
			byte[] buff = new byte[3];
			buff[0] = d;
			buff[1] = (byte)(val >> 8);
			buff[2] = (byte)(val & 0x0FF);
			port.Write(buff, 0, 3);

		}
		private void button2_Click(object sender, EventArgs e)
		{
			textBox2.Text = "";
			for (int i = 980; i <= 1200; i++)
			{
				bool res = test(i);
				textBox2.Text = i + "," + (res ? "MM" : "  ") + "\r\n" + textBox2.Text;
				Application.DoEvents();
			}
		}

		bool test(int i)
		{
			//uint16_t timing_adj[] = {  138,  277,  416,  555,  694,  833,  972, 1111, 1250, 1388};
			//send(0, 136);//(80+192)/2=136
			//send(1, 269);//(210+329)/2=269
			//send(2, i);//(351+461)/2=406
			send(7, i);
			send(99, 0);
			Thread.Sleep(100);

			buffIdx = 0;
			if (buff[221] != 85) return false;
			if (buff[222] != 170) return false;
			return true;
		}

		private void button3_Click(object sender, EventArgs e)
		{
			send(0, 136);//(80+192)/2=136
			send(1, 310);
			send(99, 0);
			Thread.Sleep(200);

			string sb = Encoding.ASCII.GetString(buff, 0, buffIdx);
			string[] s = sb.Replace("\n", "\r\n").Split('[');
			textBox2.Text = s[0];
			textBox2.Text += "\r\n";

			textBox2.Text += conv(0x55) + "\r\n";
			textBox2.Text += conv(buff[221]) + "\r\n";
			textBox2.Text += (buff[221] == 85 ? "ok" : "ng") + "\r\n";
			textBox2.Text += conv(0xAA) + "\r\n";
			textBox2.Text += conv(buff[222]) + "\r\n";
			textBox2.Text += (buff[222] == 170 ? "ok" : "ng") + "\r\n";

			buffIdx = 0;
		}

		string conv(int v)
		{
			string ret = "";
			for (int i = 0; i < 8; i++)
			{
				if ((v & 1) == 1)
				{
					ret = "1" + ret;
				}
				else
				{
					ret = "0" + ret;
				}
				v >>= 1;
			}
			return ret;
		}
	}
}
