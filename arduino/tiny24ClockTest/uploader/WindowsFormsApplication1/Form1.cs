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

		DateTime[] time = new DateTime[256];
		int idx = 0;

		int flg = 0;
		int err = 0;

		public void MyThread()
		{
			StringBuilder _sb = new StringBuilder();
			while (true)
			{
				try
				{
					while (port.BytesToRead > 0)
					{
						var c = port.ReadByte();
						time[idx] = DateTime.Now;
						idx++;
						idx &= 255;

						if (c == 0x55)
						{
							if (flg != 1)
							{
								err++;
							}
							flg = 0;
						}
						else if (c == 0xAA)
						{
							if (flg != 0)
							{
								err++;
							}
							flg = 1;
						}
						else
						{
							err++;
						}

						{
							Cinvokes ivk = new Cinvokes(Updata);
							try
							{
								this.BeginInvoke(ivk, null);
							}
							catch (Exception ex)
							{
							}
						}
					}

					Thread.Sleep(1);
				}
				catch (Exception ex) { }
			}

		}

		delegate void Cinvokes();

		private void Updata()
		{
			//30.517578125 times/s
			//61.03515625  times/s   60.898A  61.31B 61.28C
			textBox2.Text = err.ToString()+"\r\n";

			textBox2.Text += ((double)256 * 1000.0 / time[(idx - 1) & 255].Subtract(time[idx & 255]).TotalMilliseconds).ToString() + "\r\n";

			textBox2.Text += ((double)256 * 1000.0 / 30.517578125 * 8000000 / time[(idx - 1) & 255].Subtract(time[idx & 255]).TotalMilliseconds).ToString();
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
				port = new SerialPort(textBox1.Text, 9600, Parity.None, 8, StopBits.One);
				port.Open();

				ThreadStart thr_start_func = new ThreadStart(MyThread);
				Thread fThread = new Thread(thr_start_func);
				fThread.Start();
			}

		}


	}
}
