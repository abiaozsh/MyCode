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

		void Form1_FormClosed(object sender, FormClosedEventArgs e)
		{
			if (port != null && port.IsOpen)
			{
				port.Close();
			}
		}
		public string com = "COM4";
		private void Form1_Load(object sender, EventArgs e)
		{
			textBox1.Text = com;
		}

		private void button1_Click(object sender, EventArgs e)
		{
			if (port == null)
			{
				//COM4为Arduino使用的串口号，需根据实际情况调整  115200    9600
				port = new SerialPort(textBox1.Text, 115200, Parity.None, 8, StopBits.One);
				port.Open();
			}

		}

		private void button2_Click(object sender, EventArgs e)
		{
			if (port != null && port.IsOpen)
			{
				port.Close();
				port = null;
			}
		}
		void portWrite(byte val1)
		{
			byte[] buff = new byte[1];
			buff[0] = val1;
			if (port != null)
			{
				port.Write(buff, 0, 1);
			}
		}



		private void button3_Click(object sender, EventArgs e)
		{
		}

		private void textBox3_TextChanged(object sender, EventArgs e)
		{

		}

		byte[] readFromPort(int count)
		{
			var _sb = new byte[count];
			int idx = 0;
			while (true)
			{
				if (port.BytesToRead > 0)
				{
					_sb[idx++] = (byte)port.ReadByte();
					count--;
					if (count <= 0) return _sb;
				}
			}
		}

		private void button4_Click(object sender, EventArgs e)
		{
			portWrite(0);
			byte[] buff = readFromPort(256);
			bool error = false;
			for (var i = 0; i < 256; i++)
			{
				if (buff[i] != i)
				{
					error = true;
				}
			}

			textBox2.Text += error;

			for (var i = 0; i < 256; i++)
			{
				portWrite((byte)i);
			}
			byte[] result = readFromPort(1);

			textBox2.Text += result;

			byte[] buff2 = readFromPort(256);
			bool error2 = false;
			for (var i = 0; i < 256; i++)
			{
				if (buff2[i] != i)
				{
					error2 = true;
				}
			}
			textBox2.Text += error2;

		}


	}
}
