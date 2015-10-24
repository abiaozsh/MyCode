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

			//fs = new FileStream("d:\\data22.txt", FileMode.Create, FileAccess.Write);
			//sw = new StreamWriter(fs);
			if (port == null)
			{
				try
				{
					//COM4为Arduino使用的串口号，需根据实际情况调整
					port = new SerialPort("COM8", 9600, Parity.None, 8, StopBits.One);
					//port = new SerialPort("COM5", 115200, Parity.None, 8, StopBits.One);
					port.Open();
					port.DataReceived += new SerialDataReceivedEventHandler(port_DataReceived);
				}
				catch
				{ }
			}
		}

		byte[] buff1 = new byte[200000];
		int nidx = 0;
		void port_DataReceived(object sender, SerialDataReceivedEventArgs e)
		{
			while (port.BytesToRead > 0)
			{
				port.Read(buff1, nidx++, 1);
			}
		}

		private void Form1_Load(object sender, EventArgs e)
		{



		}

		private void timer1_Tick(object sender, EventArgs e)
		{
			StringBuilder sb = new StringBuilder();
			for (int i = 0; i < nidx; i++)
			{
				sb.Append(i + "\t" + Convert.ToString(buff1[i], 2) + "\r\n");
			}
			textBox1.Text = sb.ToString();
		}

	}
}


