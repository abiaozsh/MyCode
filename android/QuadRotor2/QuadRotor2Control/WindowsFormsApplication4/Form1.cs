using System;
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
using System.Runtime.InteropServices;

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
			}
			catch
			{
			}
		}

		public void NetThread()
		{
			while (true)
			{
				byte[] buff1;
				byte[] buff2 = new byte[0];
				int len;
				int cnt;
				try
				{
					//receive message
					Socket serverSocket = sSocket.Accept();

					NetworkStream ns = new NetworkStream(serverSocket);

					buff1 = new byte[2];

					cnt = ns.Read(buff1, 0, 2);
					len = ((int)buff1[0]) + (((int)buff1[1]) << 8);
					buff2 = new byte[len];


					ns.Read(buff2, 0, len);

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
				}
				catch (Exception ex)
				{
					//procBuff(System.Text.Encoding.Default.GetBytes(ex.ToString()));
				}
				procBuff(buff2);
			}
		}

		string txt;

		delegate void Cinvokes();

		public void procBuff(byte[] buff)
		{

			txt = System.Text.Encoding.Default.GetString(buff);

			Cinvokes ivk = new Cinvokes(Updata);

			this.BeginInvoke(ivk, null);
		}

		private void Updata()
		{
			textBox1.Text = txt;
		}

		int ADJXC = 1;
		int ADJYC = 2;
		int ADJZC = 3;
		int ADJXT = 4;
		int ADJYT = 5;
		int ADJZT = 6;
		int ADJPWR = 7;
		int CALI = 8;
		int RST = 9;
		int PWRON = 10;
		int PWROFF = 11;
		int SETPWR = 12;
		int SETSTARTPWR1 = 13;
		int SETSTARTPWR2 = 14;
		int SETSTARTPWR3 = 15;
		int SETSTARTPWR4 = 16;
		int SETMINPWR = 17;
		int RST2 = 18;
		int LFW = 19;
		int LBK = 20;
		int LOF = 21;
		int RFW = 22;
		int RBK = 23;
		int ROF = 24;
		int PUSHDATA = 25;

		private void button1_Click(object sender, EventArgs e)
		{
			Send(CALI, 0);
		}

		private void button2_Click(object sender, EventArgs e)
		{
			Send(RST, 0);
		}

		private void button5_Click(object sender, EventArgs e)
		{
			Send(RST2, 0);
		}

		private void button3_Click(object sender, EventArgs e)
		{
			Send(PWRON, 0);
		}

		private void button4_Click(object sender, EventArgs e)
		{
			Send(PWROFF, 0);
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

		int down = 0;

		void pictureBox2_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
		{

			down = 1;
		}

		void pictureBox2_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			//if (x == e.X && y == e.Y)
			//{
			//	Send(ADJXT, x - 50);
			//	Send(ADJYT, y - 50);
			//}
			//else
			//{
			//	Send(ADJXC, 0);
			//	Send(ADJYC, 0);
			//}
			down = 0;
		}

		void pictureBox2_MouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			if (down == 1)
			{
				int x;
				int y;
				x = e.X;
				y = e.Y;
				Send(ADJXC, (50f - x) / 100f);
				Send(ADJYC, (y - 50f) / 100f);
			}
		}

		private void trackBar4_Scroll(object sender, EventArgs e)
		{
			label4.Text = trackBar4.Value.ToString();
			Send(ADJYC, (double)trackBar4.Value / 100);
		}

		private void trackBar3_Scroll(object sender, EventArgs e)
		{
			label5.Text = trackBar3.Value.ToString();
			Send(ADJXC, (double)trackBar3.Value / 100);
		}

		private void start1_CheckedChanged(object sender, EventArgs e)
		{
			Send(SETSTARTPWR1, start1.Checked ? 20000 : -1);
		}
		private void pwr1_Scroll(object sender, EventArgs e)
		{
			Send(SETSTARTPWR1, pwr1.Value);
		}

		private void start2_CheckedChanged(object sender, EventArgs e)
		{
			Send(SETSTARTPWR2, start2.Checked ? 20000 : -1);
		}
		private void pwr2_Scroll(object sender, EventArgs e)
		{
			Send(SETSTARTPWR2, pwr2.Value);
		}

		private void start3_CheckedChanged(object sender, EventArgs e)
		{
			Send(SETSTARTPWR3, start3.Checked ? 20000 : -1);
		}
		private void pwr3_Scroll(object sender, EventArgs e)
		{
			Send(SETSTARTPWR3, pwr3.Value);
		}

		private void start4_CheckedChanged(object sender, EventArgs e)
		{
			Send(SETSTARTPWR4, start4.Checked ? 20000 : -1);
		}
		private void pwr4_Scroll(object sender, EventArgs e)
		{
			Send(SETSTARTPWR4, pwr4.Value);
		}


		private void trackBar1_Scroll(object sender, EventArgs e)
		{
			Send(SETMINPWR, trackBar1.Value);
		}

		private void trackBar2_Scroll(object sender, EventArgs e)
		{
			Send(SETPWR, trackBar2.Value);
		}

		void textBox2_KeyDown(object sender, System.Windows.Forms.KeyEventArgs e)
		{
			if (e.KeyCode == Keys.Q)
			{
				Send(LFW, 0);
			}
			if (e.KeyCode == Keys.A)
			{
				Send(LBK, 0);
			}
			if (e.KeyCode == Keys.W)
			{
				Send(RFW, 0);
			}
			if (e.KeyCode == Keys.S)
			{
				Send(RBK, 0);
			}
			Send(PUSHDATA, 0);
		}

		void textBox2_KeyUp(object sender, System.Windows.Forms.KeyEventArgs e)
		{
			if (e.KeyCode == Keys.Q)
			{
				Send(LOF, 0);
			}
			if (e.KeyCode == Keys.A)
			{
				Send(LOF, 0);
			}
			if (e.KeyCode == Keys.W)
			{
				Send(ROF, 0);
			}
			if (e.KeyCode == Keys.S)
			{
				Send(ROF, 0);
			}
			Send(PUSHDATA, 0);
		}

		private void pictureBox2_Click(object sender, EventArgs e)
		{

		}

        int x;
        int y;
        int u;
        int v;
        private void timer1_Tick(object sender, EventArgs e)
        {
            joy.JOYINFOEX info = new joy.JOYINFOEX();
            info.dwSize = Marshal.SizeOf(typeof(joy.JOYINFOEX));
            info.dwFlags = 0x00000080; 
            joy.joyGetPosEx(0, ref info);

             x = info.dwXpos;
             y = info.dwYpos;
             u = info.dwZpos;
             v = info.dwRpos;

             this.Text = x.ToString() + " " + y.ToString() + " " + u.ToString() + " " + v.ToString() + " ";
        }

	}
}
