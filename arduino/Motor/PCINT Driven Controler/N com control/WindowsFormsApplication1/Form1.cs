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
					port = new SerialPort("COM4", 9600, Parity.None, 8, StopBits.One);
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
		int[] datas = new int[800];
		int datasIdx = 0;
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
					datas[datasIdx++] = buff[0] + ((int)buff[1] << 8);
					if (datasIdx >= 800)
					{
						datasIdx = 0;
					}

					int idx = datasIdx;
					for (int i = 0; i < 12; i++)
					{
						idx--;
						if (idx < 0)
						{
							idx = 799;
						}
						data += datas[idx];

					}
					data /= 12;
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
				//try
				//{
				//	//COM4为Arduino使用的串口号，需根据实际情况调整
				//	port = new SerialPort("COM4", 9600, Parity.None, 8, StopBits.One);
				//	port.Open();
				//	port.DataReceived += new SerialDataReceivedEventHandler(port_DataReceived);
				//}
				//catch { }
			}
		}

		Bitmap bmp;
		Graphics g;
		private void Form1_Load(object sender, EventArgs e)
		{
			bmp = new Bitmap(800, 256);
			g = Graphics.FromImage(bmp);
			pictureBox1.Image = bmp;
		}

		const byte CMD_SENDDATA1Xa = 1; /* 0~255 1x */
		const byte CMD_SENDDATA1Xb = 2; /* 256~511 1x */
		const byte CMD_SENDDATA2X = 3; /* 512~1023 2x */
		const byte CMD_SENDDATA4X = 4; /* 1024~2047 4x */
		const byte CMD_SENDDATA8X = 5; /* 2048~4095 8x */
		const byte CMD_SENDDATA16X = 6; /* 4096~8191 16x */
		const byte CMD_START = 7; /* on/off */
		const byte CMD_STOP = 8;
		const byte CMD_SETMAXPWR = 9;
		const byte CMD_LINEUP = 10;
		const byte CMD_PITCH = 11; /* PITCH */
		const byte CMD_REVERSE = 12; /* REVERSE */
		const byte CMD_SETCPU = 13;

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
				textBox1.Text = (data / 2).ToString() + " " + 3333333f / (data) + "rpm  " + 1666666f / (targetSpeed) + "rpm";
			}
			if (comboBox1.Text == "15")
			{
				textBox1.Text = (data / 2).ToString() + " " + 4000000f / (data) + "rpm  " + 2000000f / (targetSpeed) + "rpm";
			}
			if (comboBox1.Text == "12")
			{
				textBox1.Text = (data / 2).ToString() + " " + 5000000f / (data) + "rpm  " + 2500000f / (targetSpeed) + "rpm";
			}
			if (comboBox1.Text == "9")
			{
				textBox1.Text = (data / 2).ToString() + " " + 6666666f / (data) + "rpm  " + 3333333f / (targetSpeed) + "rpm";
			}
			if (comboBox1.Text == "6")
			{
				textBox1.Text = (data / 2).ToString() + " " + 10000000f / (data) + "rpm  " + 5000000f / (targetSpeed) + "rpm";
			}
			if (comboBox1.Text == "3")
			{
				textBox1.Text = (data / 2).ToString() + " " + 20000000f / (data) + "rpm  " + 10000000f / (targetSpeed) + "rpm";
			}
			//Random r = new Random();
			//datas[datasIdx++] = (ushort)r.Next();
			//if (datasIdx >= 800)
			//{
			//	datasIdx = 0;
			//}


			g.Clear(Color.Gray);
			Pen p = new Pen(Color.Black);
			Pen pr = new Pen(Color.Red);
			int idx = datasIdx;
			for (int i = 0; i < 800; i++)
			{
				double val = Math.Log((double)datas[idx] / 50f + 1) * 35;

				if (datas[idx] > 8192)
				{
					g.DrawLine(pr, i, 0, i, (float)val);
				}
				else
				{
					g.DrawLine(p, i, 0, i, (float)val);
				}
				idx--;
				if (idx < 0)
				{
					idx = 799;
				}
			}
			g.Flush();
			pictureBox1.Refresh();
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
			byte data = 0;
			if (trackBar2.Value == 256)
			{
				trackBar2.Value = 256;
				data = 0;
			}
			else if (trackBar2.Value >= 224)
			{
				trackBar2.Value = 224;
				data = 1;
			}
			else if (trackBar2.Value >= 192)
			{
				trackBar2.Value = 192;
				data = 2;
			}
			else if (trackBar2.Value >= 160)
			{
				trackBar2.Value = 160;
				data = 3;
			}
			else if (trackBar2.Value >= 128)
			{
				trackBar2.Value = 128;
				data = 4;
			}
			else if (trackBar2.Value >= 96)
			{
				trackBar2.Value = 96;
				data = 5;
			}
			else if (trackBar2.Value >= 64)
			{
				trackBar2.Value = 64;
				data = 6;
			}
			else if (trackBar2.Value >= 32)
			{
				trackBar2.Value = 32;
				data = 7;
			}
			else if (trackBar2.Value >= 16)
			{
				trackBar2.Value = 16;
				data = 8;
			}
			else if (trackBar2.Value >= 8)
			{
				trackBar2.Value = 8;
				data = 9;
			}
			else if (trackBar2.Value >= 4)
			{
				trackBar2.Value = 4;
				data = 10;
			}
			else if (trackBar2.Value >= 2)
			{
				trackBar2.Value = 2;
				data = 11;
			}
			else
			{
				trackBar2.Value = 1;
				data = 12;
			}

			this.Text = trackBar2.Value.ToString();

			Send(CMD_SETMAXPWR);
			Send(data);

		}

		private void button4_Click(object sender, EventArgs e)
		{
			Send(CMD_SETCPU);
			Send(byte.Parse(textBox2.Text));

		}

		private void button6_Click_1(object sender, EventArgs e)
		{
			Send(0x55);
		}


	}
}


