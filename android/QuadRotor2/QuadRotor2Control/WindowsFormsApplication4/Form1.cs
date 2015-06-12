﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net.Sockets;
using System.Net;
using System.Threading;

namespace WindowsFormsApplication4
{
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();
		}
		Socket sSocket;

		public string ComputerIP = "192.168.43.46";
		public string PhoneIP = "192.168.43.1";
		//public string ComputerIP = "192.168.0.10";
		//public string PhoneIP = "192.168.0.100";
		//private Device applicationDevice = null;

		private void Form1_Load(object sender, EventArgs e)
		{
			Bitmap b = new Bitmap(100, 100);
			b.SetPixel(50, 50, Color.Red);
			pictureBox2.Image = b;
			try
			{
				int port = 6000;
				string host = ComputerIP;

				IPAddress ip = IPAddress.Parse(host);
				IPEndPoint ipe = new IPEndPoint(ip, port);

				sSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
				sSocket.Bind(ipe);
				sSocket.Listen(0);

				Thread t = new Thread(NetThread);
				t.Start();
			}
			catch (Exception ex)
			{
				ex.ToString();
			}

			/*
			// Enumerate joysticks in the system.
			foreach (DeviceInstance instance in Manager.GetDevices(DeviceClass.GameControl, EnumDevicesFlags.AttachedOnly))
			{
				// Create the device.  Just pick the first one
				applicationDevice = new Device(instance.InstanceGuid);
				break;
			}

			// Set the data format to the c_dfDIJoystick pre-defined format.
			applicationDevice.SetDataFormat(DeviceDataFormat.Joystick);
			// Set the cooperative level for the device.
			applicationDevice.SetCooperativeLevel(this, CooperativeLevelFlags.Exclusive | CooperativeLevelFlags.Foreground);
			*/



		}


		void Form1_KeyPress(object sender, System.Windows.Forms.KeyPressEventArgs e)
		{
		}
		public void IntToByte(int val, byte[] ret, int offset)
		{
			ret[offset + 3] = (byte)(val >> 24 & 0xff);
			ret[offset + 2] = (byte)(val >> 16 & 0xff);
			ret[offset + 1] = (byte)(val >> 8 & 0xff);
			ret[offset + 0] = (byte)(val & 0xff);
		}
		public int ByteToInt(byte[] src, int offset)
		{
			int value;
			value = (int)((src[offset] & 0xFF)
					| ((src[offset + 1] & 0xFF) << 8)
					| ((src[offset + 2] & 0xFF) << 16)
					| ((src[offset + 3] & 0xFF) << 24));
			return value;
		}

		public static void putDouble(byte[] bb, double x, int index)
		{
			// byte[] b = new byte[8];  
			long l = BitConverter.DoubleToInt64Bits(x);
			for (int i = 0; i < 8; i++)
			{
				bb[index + i] = (byte)l;
				l = l >> 8;
			}
		}
		public static double getDouble(byte[] b, int index)
		{
			long l;
			l = b[0 + index];
			l &= 0xff;
			l |= ((long)b[1 + index] << 8);
			l &= 0xffff;
			l |= ((long)b[2 + index] << 16);
			l &= 0xffffff;
			l |= ((long)b[3 + index] << 24);
			l &= 0xffffffffl;
			l |= ((long)b[4 + index] << 32);
			l &= 0xffffffffffl;
			l |= ((long)b[5 + index] << 40);
			l &= 0xffffffffffffl;
			l |= ((long)b[6 + index] << 48);
			l &= 0xffffffffffffffl;
			l |= ((long)b[7 + index] << 56);
			return BitConverter.Int64BitsToDouble(l);
		}

		public void Send(int param1, double param2)
		{
			try
			{
				byte[] data = new byte[9];

				data[0] = (byte)param1;
				putDouble(data, param2, 1);
				int port = 5000;
				string host = PhoneIP;//服务器端ip地址

				IPAddress ip = IPAddress.Parse(host);
				IPEndPoint ipe = new IPEndPoint(ip, port);

				Socket clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
				clientSocket.Connect(ipe);

				clientSocket.Send(data);

				clientSocket.Close();

				textBox1.Focus();
			}
			catch
			{
			}
		}

		public void NetThread()
		{
			while (true)
			{
				//receive message
				Socket serverSocket = sSocket.Accept();
				//byte[] recByte = new byte[1024 * 1024];

				//int bytes = serverSocket. Receive(recByte, recByte.Length, 0);
				NetworkStream ns = new NetworkStream(serverSocket);

				byte[] buff = new byte[64];

				ns.Read(buff, 0, 64);

				Image img = Image.FromStream(ns);

				ns.Close();
				serverSocket.Close();

				var tempBitmap = new Bitmap(320, 240);
				var gTempBitmap = System.Drawing.Graphics.FromImage(tempBitmap);
				gTempBitmap.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.High;
				gTempBitmap.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;
				gTempBitmap.Clear(Color.White);
				gTempBitmap.DrawImage(img, new Rectangle(0, 0, 320, 240), new Rectangle(0, 0, img.Width, img.Height), GraphicsUnit.Pixel);
				gTempBitmap.Dispose();
				tempBitmap.RotateFlip(RotateFlipType.Rotate180FlipX);
				pictureBox1.Image = tempBitmap;


				procBuff(buff);

			}
		}

		double CurrGryox;
		double CurrGryoy;
		double CurrGryoz;

		delegate void Cinvokes();

		public void procBuff(byte[] buff)
		{
			CurrGryox = getDouble(buff, 0);
			CurrGryoy = getDouble(buff, 8);
			CurrGryoz = getDouble(buff, 16);

			Cinvokes ivk = new Cinvokes(Updata);

			this.BeginInvoke(ivk, null);
		}

		private void Updata()
		{
			label1.Text = CurrGryox.ToString();
			label2.Text = CurrGryoy.ToString();
			label3.Text = CurrGryoz.ToString();
		}

		int ADJXC = 1;
		int ADJYC = 2;
		int ADJZC = 3;
		int ADJPWR = 4;
		int CALI = 5;
		int RST = 6;
		int START = 7;
		int STOP = 8;
		int TEST = 9;
		int SETPWR = 10;
		int ADJXT = 11;
		int ADJYT = 12;
		int ADJZT = 13;

		private void button1_Click(object sender, EventArgs e)
		{
			Send(CALI, 0);
		}

		private void button2_Click(object sender, EventArgs e)
		{
			Send(RST, 0);
		}

		private void button3_Click(object sender, EventArgs e)
		{
			Send(START, 0);
		}

		private void button4_Click(object sender, EventArgs e)
		{
			Send(STOP, 0);
		}

		private void button5_Click(object sender, EventArgs e)
		{
			Send(TEST, 1);
		}

		private void button6_Click(object sender, EventArgs e)
		{
			Send(TEST, 2);
		}

		private void button7_Click(object sender, EventArgs e)
		{
			Send(TEST, 3);
		}

		private void button8_Click(object sender, EventArgs e)
		{
			Send(TEST, 4);
		}

		void textBox1_KeyPress(object sender, System.Windows.Forms.KeyPressEventArgs e)
		{


			if (e.KeyChar == 'r')
			{
				Send(ADJPWR, 1);
			}
			if (e.KeyChar == 'f')
			{
				Send(ADJPWR, -1);
			}
		}

		private void trackBar1_Scroll(object sender, EventArgs e)
		{
			Send(SETPWR, trackBar1.Value);//0~1000
		}

		int x;
		int y;
		int down = 0;

		void pictureBox2_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			x = e.X;
			y = e.Y;
			down = 1;
		}

		void pictureBox2_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			if (x == e.X && y == e.Y)
			{
				Send(ADJXT, x - 50);
				Send(ADJYT, y - 50);
			}
			else
			{
				Send(ADJXC, 0);
				Send(ADJYC, 0);
			}
			down = 0;
		}

		void pictureBox2_MouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			if (down == 1)
			{
				//Send(ADJXC, ((double)(x - e.X)) / 1000);
				//Send(ADJYC, ((double)(y - e.Y)) / 1000);
			}
		}

		private void trackBar4_Scroll(object sender, EventArgs e)
		{
			label4.Text = trackBar4.Value.ToString();
			Send(ADJXC, (double)trackBar4.Value / 10000);
		}

		private void trackBar3_Scroll(object sender, EventArgs e)
		{
			label5.Text = trackBar3.Value.ToString();
			Send(ADJXC, (double)trackBar3.Value / 10000);
		}

	}
}