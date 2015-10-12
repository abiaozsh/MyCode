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

			this.FormClosed += new FormClosedEventHandler(Form1_FormClosed);

			//fs = new FileStream("d:\\data22.txt", FileMode.Create, FileAccess.Write);
			//sw = new StreamWriter(fs);
			if (port == null)
			{
				//COM4为Arduino使用的串口号，需根据实际情况调整
				port = new SerialPort("COM5", 115200, Parity.None, 8, StopBits.One);
				port.Open();
				port.DataReceived += new SerialDataReceivedEventHandler(port_DataReceived);
			}
		}
		int datareceive = 0;
		byte[] buff = new byte[2];
		int data = 0;
		void port_DataReceived(object sender, SerialDataReceivedEventArgs e)
		{
			if (port.BytesToRead > 0)
			{
				port.Read(buff, datareceive, 1);
				datareceive++;
				if (datareceive >= 2)
				{
					data = buff[0] + ((int)buff[1] << 8);

					textBox1.Text = data.ToString();
					datareceive &= 1;
				}
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
				Thread.Sleep(10);
				port.Write(d, 1, 1);
			}
		}

		private void Send(byte data)
		{
			byte[] d = new byte[1];
			d[0] = data;
			Console.WriteLine(data);
			if (port != null && port.IsOpen)
			{
				//Thread.Sleep(5);
				port.Write(d, 0, 1);
			}
		}

		private void Form1_Load(object sender, EventArgs e)
		{
		}
		byte CMD_SENDDATA1Xa = 10;  /*0~255       1x*/
		byte CMD_SENDDATA1Xb = 11;  /*256~511     1x*/
		byte CMD_SENDDATA2X = 12;  /*512~1023    2x*/
		byte CMD_SENDDATA4X = 13;  /*1024~2047   4x*/
		byte CMD_SENDDATA8X = 14;  /*2048~4095   8x*/
		byte CMD_SENDDATA16X = 15;  /*4096~8191  16x*/
		byte CMD_SENDDATA32X = 16;  /*8192~16383 32x*/
		byte CMD_FORCE = 20;  /*on/off        */

		private void trackBar1_Scroll(object sender, EventArgs e)
		{
			int v = trackBar1.Value;
			this.Text = v.ToString();
			if (v < 256)
			{
				Send(CMD_SENDDATA1Xa);
				Send((byte)(v));
			}
			else if (v < 512)
			{
				Send(CMD_SENDDATA1Xb);
				Send((byte)(v - 256));
			}
			else if (v < 1024)
			{
				Send(CMD_SENDDATA2X);
				Send((byte)((v - 512) / 2));
			}
			else if (v < 2048)
			{
				Send(CMD_SENDDATA4X);
				Send((byte)((v - 1024) / 4));
			}
			else if (v < 4096)
			{
				Send(CMD_SENDDATA8X);
				Send((byte)((v - 2048) / 8));
			}
			else if (v < 8192)
			{
				Send(CMD_SENDDATA16X);
				Send((byte)((v - 4096) / 16));
			}
			else if (v < 16384)
			{
				Send(CMD_SENDDATA32X);
				Send((byte)((v - 8192) / 32));
			}
			else
			{
			}


		}

		private void checkBox1_CheckedChanged(object sender, EventArgs e)
		{
			byte[] buff = new byte[1];
			if (checkBox1.Checked)
			{
				Send(CMD_FORCE);
				Send(1);
			}
			else
			{
				Send(CMD_FORCE);
				Send(0);
			}
		}

		private void button1_Click(object sender, EventArgs e)
		{
			datareceive++;
		}


	}
}


