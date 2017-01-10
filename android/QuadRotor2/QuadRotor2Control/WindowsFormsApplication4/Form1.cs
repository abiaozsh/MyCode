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
using System.IO;

namespace WindowsFormsApplication4
{
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();
		}
		Socket sSocket;

		public string ComputerIP = "192.168.43.151";
		public byte[] byteIP = { 192, 168, 43, 151 };
		public string PhoneIP = "192.168.43.1";

		//public string ComputerIP = "192.168.0.10";
		//public byte[] byteIP = { 192, 168, 0, 1 };
		//public string PhoneIP = "192.168.0.6";

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
		}

		void Form1_FormClosed(object sender, System.Windows.Forms.FormClosedEventArgs e)
		{
			Application.Exit();
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
		public void SendIP()
		{
			try
			{
				byte[] data = new byte[9];
				data[1] = byteIP[0];
				data[2] = byteIP[1];
				data[3] = byteIP[2];
				data[4] = byteIP[3];

				data[0] = (byte)SETIP;

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

		public bool FLIPH = false;
		public bool FLIPV = false;

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

					int type = ns.ReadByte();

					if (type == 1)
					{
						buff1 = new byte[2];

						cnt = ns.Read(buff1, 0, 2);
						len = ((int)buff1[0]) + (((int)buff1[1]) << 8);
						buff2 = new byte[len];

						ns.Read(buff2, 0, len);

						txt = System.Text.Encoding.Default.GetString(buff2);

						Cinvokes ivk = new Cinvokes(Updata);
						try
						{
							this.BeginInvoke(ivk, null);
						}
						catch (Exception ex)
						{
							break;
						}
					}
					else if (type == 2)
					{
						Image img = Image.FromStream(ns);

						ns.Close();
						serverSocket.Close();

						var tempBitmap = new Bitmap(640, 480);
						var gTempBitmap = System.Drawing.Graphics.FromImage(tempBitmap);
						gTempBitmap.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.High;
						gTempBitmap.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;
						gTempBitmap.Clear(Color.White);
						//gTempBitmap.DrawImage(img, new Rectangle(0, 0, 640, 480), new Rectangle(0, 0, img.Width, img.Height), GraphicsUnit.Pixel);
						gTempBitmap.DrawImage(img, new Rectangle(0, 0, 64, 48), new Rectangle(0, 0, img.Width, img.Height), GraphicsUnit.Pixel);
						gTempBitmap.Dispose();

						if (FLIPH)
						{
							tempBitmap.RotateFlip(RotateFlipType.RotateNoneFlipX);
						}

						if (FLIPV)
						{
							tempBitmap.RotateFlip(RotateFlipType.RotateNoneFlipY);
						}

						pictureBox1.Image = tempBitmap;
					}
				}
				catch (Exception ex)
				{
					//procBuff(System.Text.Encoding.Default.GetBytes(ex.ToString()));
				}
			}
		}

		string txt;

		delegate void Cinvokes();

		private void Updata()
		{
			textBox1.Text = txt;
		}

		int ADJXC = 1;
		int ADJYC = 2;
		int ADJZC = 3;
		int ADJXT = 4;
		int ADJYT = 5;
		int SETCAMB = 6;
		int ADJPWR = 7;
		int SETCAMF = 8;
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
		int CON = 25;
		int COF = 26;
		int PUSHDATA = 27;
		int SETIP = 28;
		int CAMON = 29;
		int CAMOFF = 30;
		int TAKEPIC = 31;
		int SETQUALITY = 32;
		int SETLED = 33;


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

		private void trackBar3_Scroll(object sender, EventArgs e)
		{
			this.Text = trackBar3.Value.ToString();
			Send(ADJZC, (double)trackBar3.Value);
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

		int oldval = 0;
		void SendA(int val)
		{
			if (val != oldval)
			{
				Send(val, 0);
				Send(PUSHDATA, 0);
				textBox1.Text += val + "\r\n";
				oldval = val;
			}
		}
		void textBox2_KeyDown(object sender, System.Windows.Forms.KeyEventArgs e)
		{
			switch (e.KeyCode)
			{
				case Keys.Q: SendA(LBK); break;
				case Keys.A: SendA(LFW); break;
				case Keys.W: SendA(RBK); break;
				case Keys.S: SendA(RFW); break;
				case Keys.Space: SendA(CON); break;
				case Keys.Up: SendA(LBK); SendA(RBK); break;
				case Keys.Down: SendA(LFW); SendA(RFW); break;
				case Keys.Left: SendA(LFW); SendA(RBK); break;
				case Keys.Right: SendA(LBK); SendA(RFW); break;
			}
		}

		void textBox2_KeyUp(object sender, System.Windows.Forms.KeyEventArgs e)
		{
			switch (e.KeyCode)
			{
				case Keys.Q: SendA(LOF); break;
				case Keys.A: SendA(LOF); break;
				case Keys.W: SendA(ROF); break;
				case Keys.S: SendA(ROF); break;
				case Keys.Space: SendA(COF); break;
				case Keys.Up: SendA(LOF); SendA(ROF); break;
				case Keys.Down: SendA(LOF); SendA(ROF); break;
				case Keys.Left: SendA(LOF); SendA(ROF); break;
				case Keys.Right: SendA(LOF); SendA(ROF); break;
			}
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

		private void radioButton2_CheckedChanged(object sender, EventArgs e)
		{
			Send(SETCAMB, 0);

		}

		private void radioButton1_CheckedChanged(object sender, EventArgs e)
		{
			Send(SETCAMF, 0);

		}

		private void pictureBox1_Click(object sender, EventArgs e)
		{

		}

		private void button1_Click(object sender, EventArgs e)
		{
			FLIPH = !FLIPH;
		}

		private void button6_Click(object sender, EventArgs e)
		{
			FLIPV = !FLIPV;
		}

		private void textBox2_TextChanged(object sender, EventArgs e)
		{

		}

		private void button7_Click(object sender, EventArgs e)
		{
			SendIP();
		}

		private void button8_Click(object sender, EventArgs e)
		{
			Send(CAMON, 0);
		}

		private void button9_Click(object sender, EventArgs e)
		{
			Send(CAMOFF, 0);
		}

		private void button10_Click(object sender, EventArgs e)
		{
			timer2.Interval = (int)(1000 / int.Parse(textBox3.Text));
			timer2.Start();
		}

		private void timer2_Tick(object sender, EventArgs e)
		{
			Send(TAKEPIC, 0);
		}

		private void button11_Click(object sender, EventArgs e)
		{
			Send(SETQUALITY, double.Parse(textBox3.Text));
		}

		private void checkBox1_CheckedChanged(object sender, EventArgs e)
		{
			if (checkBox1.Checked)
			{
				Send(SETLED, 1);
			}
			else
			{
				Send(SETLED, 0);
			}
		}

	}
}
