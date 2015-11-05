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
				try
				{
					//COM4为Arduino使用的串口号，需根据实际情况调整
					port = new SerialPort("COM4", 115200, Parity.None, 8, StopBits.One);
					//port = new SerialPort("COM5", 115200, Parity.None, 8, StopBits.One);
					port.Open();
					port.DataReceived += new SerialDataReceivedEventHandler(port_DataReceived);
				}
				catch
				{ }
			}
		}


		int datareceive = 0;
		byte[] buff = new byte[2];
		int data = 0;
		void port_DataReceived(object sender, SerialDataReceivedEventArgs e)
		{
			while (port.BytesToRead > 0)
			{
				datareceive &= 1;
				port.Read(buff, datareceive, 1);
				datareceive++;
				if (datareceive >= 2)
				{
					data = buff[0] + ((int)buff[1] << 8);

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
			else
			{
				try
				{
					//COM4为Arduino使用的串口号，需根据实际情况调整
					port = new SerialPort("COM5", 115200, Parity.None, 8, StopBits.One);
					port.Open();
					port.DataReceived += new SerialDataReceivedEventHandler(port_DataReceived);
				}
				catch { }
			}
		}

		private void Form1_Load(object sender, EventArgs e)
		{
		}
		const byte CMD_FORCE = 20;  /*on/off        */

		const byte CMD_SENDDATA1Xa = 10;  /*0~255       1x*/
		const byte CMD_SENDDATA1Xb = 11;  /*256~511     1x*/
		const byte CMD_SENDDATA2X = 12;  /*512~1023    2x*/
		const byte CMD_SENDDATA4X = 13;  /*1024~2047   4x*/
		const byte CMD_SENDDATA8X = 14;  /*2048~4095   8x*/
		const byte CMD_SENDDATA16X = 15;  /*4096~8191  16x*/
		const byte CMD_SENDDATA32X = 16;  /*8192~16383 32x*/
		const byte CMD_START = 20;  /*on/off        */
		const byte CMD_STOP = 25;
		const byte CMD_SETMAXPWR = 30;
		const byte CMD_LINEUP = 40;
		const byte CMD_PITCH = 50;  /*PITCH          */
		const byte CMD_REVERSE = 60;  /*REVERSE        */


		int targetSpeed;
		private void trackBar1_Scroll(object sender, EventArgs e)
		{
			int v = trackBar1.Value;
			byte cmd = 0;
			byte val = 0;
			if (v < 256)
			{
				cmd = CMD_SENDDATA1Xa;
				val = ((byte)(v));
			}
			else if (v < 512)
			{
				cmd = CMD_SENDDATA1Xb;
				val = ((byte)(v - 256));
			}
			else if (v < 768)
			{
				cmd = CMD_SENDDATA2X;
				val = ((byte)((v - 512)));
			}
			else if (v < 1024)
			{
				cmd = (CMD_SENDDATA4X);
				val = ((byte)((v - 768)));
			}
			else if (v < 1280)
			{
				cmd = (CMD_SENDDATA8X);
				val = ((byte)((v - 1024)));
			}
			else if (v < 1536)
			{
				cmd = (CMD_SENDDATA16X);
				val = ((byte)((v - 1280)));
			}
			else
			{
			}
			Send(cmd);
			Send(val);
			int vv = val;

			switch (cmd)
			{
				case CMD_SENDDATA1Xa://   10  /*0~255       1x*/
					vv = vv;
					break;
				case CMD_SENDDATA1Xb://   11  /*256~511     1x*/
					vv = vv + 256;
					break;
				case CMD_SENDDATA2X://    12  /*512~1023    2x*/
					vv = (vv << 1) + 512;
					break;
				case CMD_SENDDATA4X://    13  /*1024~2047   4x*/
					vv = (vv << 2) + 1024;
					break;
				case CMD_SENDDATA8X://    14  /*2048~4095   8x*/
					vv = (vv << 3) + 2048;
					break;
				case CMD_SENDDATA16X://   15  /*4096~8191  16x*/
					vv = (vv << 4) + 4096;
					break;
			}
			targetSpeed = vv;
			this.Text = vv.ToString();
		}

		private void button1_Click(object sender, EventArgs e)
		{
			datareceive++;
		}

		private void timer1_Tick(object sender, EventArgs e)
		{
			if (data == 0)
			{
				data = 1;
			}
			if (targetSpeed == 0)
			{
				targetSpeed = 1;
			}
			if (comboBox1.Text == "18")
			{
				textBox1.Text = (data / 8 / 2).ToString() + " " + 26666666f / (data) + "rpm  " + 1666666f / (targetSpeed) + "rpm";
			}
			if (comboBox1.Text == "15")
			{
				textBox1.Text = (data / 8 / 2).ToString() + " " + 32000000f / (data) + "rpm  " + 2000000f / (targetSpeed) + "rpm";
			}
			if (comboBox1.Text == "12")
			{
				textBox1.Text = (data / 8 / 2).ToString() + " " + 40000000f / (data) + "rpm  " + 2500000f / (targetSpeed) + "rpm";
			}
			if (comboBox1.Text == "9")
			{
				textBox1.Text = (data / 8 / 2).ToString() + " " + 53333333f / (data) + "rpm  " + 3333333f / (targetSpeed) + "rpm";
			}
			if (comboBox1.Text == "6")
			{
				textBox1.Text = (data / 8 / 2).ToString() + " " + 80000000f / (data) + "rpm  " + 5000000f / (targetSpeed) + "rpm";
			}
			if (comboBox1.Text == "3")
			{
				textBox1.Text = (data / 8 / 2).ToString() + " " + 160000000f / (data) + "rpm  " + 10000000f / (targetSpeed) + "rpm";
			}

		}

		private void button2_Click(object sender, EventArgs e)
		{
			Send(CMD_START);
			Send(250);
		}

		private void button3_Click(object sender, EventArgs e)
		{
			Send(CMD_STOP);
			Send(0);
		}

		private void button5_Click(object sender, EventArgs e)
		{
			Send(CMD_LINEUP);
			Send(byte.Parse(textBox3.Text));
		}

		private void button6_Click(object sender, EventArgs e)
		{

		}

		private void textBox4_TextChanged(object sender, EventArgs e)
		{

		}

		private void checkBox2_CheckedChanged(object sender, EventArgs e)
		{
			Send(CMD_PITCH);
			Send((byte)(checkBox2.Checked ? 1 : 0));
		}

		private void checkBox1_CheckedChanged(object sender, EventArgs e)
		{
			Send(CMD_REVERSE);
			Send((byte)(checkBox1.Checked ? 1 : 0));
		}

		private void textBox1_TextChanged(object sender, EventArgs e)
		{

		}

		private void trackBar2_Scroll(object sender, EventArgs e)
		{
			Send(CMD_SETMAXPWR);
			Send((byte)trackBar2.Value);

		}


	}
}


