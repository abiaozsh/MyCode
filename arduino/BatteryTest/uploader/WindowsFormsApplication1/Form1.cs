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


		Queue<String> lines = new Queue<string>();

		public void MyThread()
		{
			StringBuilder _sb = new StringBuilder();
			while (true)
			{
				try
				{
					while (port.BytesToRead > 0)
					{
						var c = (char)port.ReadChar();

						_sb.Append(c);
						if (c == '\n')
						{
							lines.Enqueue(_sb.ToString());
							_sb.Clear();
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

					Thread.Sleep(100);
				}
				catch (Exception ex) { }
			}

		}

		DateTime startTime;
		double accuPwr;
		int dataCnt;
		bool started;
		delegate void Cinvokes();

		private void Updata()
		{
			while (lines.Count>0)
			{
				string line = lines.Dequeue();
				if (line == "STOP\n")
				{
                    break;
				}
				StringBuilder sb = new StringBuilder();

				string[] values = line.Split('\t');


				//SerialSend('c'); SerialSend(':'); SendInt(curr, 5); SerialSend('\t');
				//SerialSend('v'); SerialSend(':'); SendInt(volt, 5); SerialSend('\t');
				//SerialSend('m'); SerialSend(':'); SendInt(mosVolt, 5); SerialSend('\t');
				//SerialSend('u'); SerialSend(':'); SendInt(cmax, 5); SerialSend('\t');
				//SerialSend('d'); SerialSend(':'); SendInt(cmin, 5); SerialSend('\t');
				//SerialSend('\n');
				double c = getValue(values[0]);
				double v = getValue(values[1]);
				double m = getValue(values[2]);
				double u = getValue(values[3]);
				double d = getValue(values[4]);

                sb.AppendLine("max:" + u * 0.00813802083 + " A");
                sb.AppendLine("min:" + d * 0.00813802083 + " A");

				if (c != 0 && !started)
				{
					started = true;
					startTime = DateTime.Now;
					accuPwr = 0;
					dataCnt = 0;
				}
				sb.AppendLine(line);

				if (started)
				{
					double curr = c * 0.00813802083;//A
					double volt = v * 0.0048828125;//V
					double pwr = curr * volt;//w

					sb.AppendLine("curr:" + curr + " A");
					sb.AppendLine("volt:" + volt + " V");
					sb.AppendLine("pwr:" + pwr + " W");
					accuPwr += pwr;
					dataCnt++;
					double currentWork = (accuPwr / dataCnt) * (DateTime.Now.Subtract(startTime).TotalSeconds);
					sb.AppendLine("Work:" + currentWork + " ws");
					sb.AppendLine("Work:" + currentWork / 3600 + " wh");
					sb.AppendLine("Work:" + currentWork / 3600 / 3.7 + " Ah");
				}

				textBox2.Text = sb.ToString();
			}
		}

		double getValue(string str)
		{
            try
            {
                string s = str.Substring(2).TrimStart('0');
                if (s == "")
                {
                    return 0;
                }
                else
                {
                    return double.Parse(s);
                }
            }
            catch {
                return 0;
            }
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
			started = false;
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


	}
}
