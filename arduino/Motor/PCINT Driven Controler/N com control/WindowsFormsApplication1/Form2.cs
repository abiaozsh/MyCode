using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;

namespace WindowsFormsApplication1
{
	public partial class Form2 : Form
	{
		public Form2()
		{
			InitializeComponent();
		}
		SerialPort port;

		private void Form2_Load(object sender, EventArgs e)
		{
			this.FormClosed += new FormClosedEventHandler(Form2_FormClosed);

			//fs = new FileStream("d:\\data22.txt", FileMode.Create, FileAccess.Write);
			//sw = new StreamWriter(fs);
			if (port == null)
			{
				try
				{
					//COM4为Arduino使用的串口号，需根据实际情况调整
					port = new SerialPort("COM4", 9600, Parity.None, 8, StopBits.One);
					//port = new SerialPort("COM5", 115200, Parity.None, 8, StopBits.One);
					port.Open();
					//port.DataReceived += new SerialDataReceivedEventHandler(port_DataReceived);
				}
				catch
				{ }
			}
		}
		void Form2_FormClosed(object sender, FormClosedEventArgs e)
		{
			if (port != null && port.IsOpen)
			{
				port.Close();
			}
		}

		private void Send1(byte data)
		{
			byte[] d = new byte[1];
			d[0] = data;
			if (port != null && port.IsOpen)
			{
				port.Write(d, 0, 1);
			}
		}
		private void Send2(byte data)
		{
			Random r = new Random();
			byte[] d = new byte[2];
			d[0] = (byte)r.Next();
			d[1] = data;
			if (port != null && port.IsOpen)
			{
				port.Write(d, 0, 2);
			}
		}

		private void button1_Click(object sender, EventArgs e)
		{
			byte b = byte.Parse(textBox1.Text);
			b++;
			textBox1.Text = b.ToString();
			textBox2.Text = System.Convert.ToString(b, 2).PadLeft(8, '0').Replace('0','_');
			Send1(b);
		}


	}
}
