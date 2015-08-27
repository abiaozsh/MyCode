using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Imaging;
using System.IO.Ports;

namespace dsound
{
	public partial class Form2 : Form
	{
		public Form2()
		{
			InitializeComponent();
		}
		SoundRecord sr = new SoundRecord();
		//dsound2.SoundRecord sr2 = new dsound2.SoundRecord();

		private void Form2_Load(object sender, EventArgs e)
		{

			try
			{
				this.FormClosed += new FormClosedEventHandler(Form1_FormClosed);

				if (port == null)
				{
					//COM4为Arduino使用的串口号，需根据实际情况调整
					port = new SerialPort("COM6", 250000, Parity.None, 8, StopBits.One);
					port.Open();
				}
			}
			catch
			{
				MessageBox.Show("error");
			}
			sr.RecStart();
			//sr2.RecStart();

			Timer t = new Timer();
			t.Interval = 40;
			t.Tick += new EventHandler(t_Tick);
			t.Start();
		}

		void t_Tick(object sender, EventArgs e)
		{
			var spect = sr.spect;
			var curLine = sr.curLine;
			{
				Bitmap p1 = new Bitmap(512, 256);
				int line = 0;//curLine

				BitmapData bd = p1.LockBits(new Rectangle(0, 0, 512, 256), System.Drawing.Imaging.ImageLockMode.ReadWrite, p1.PixelFormat);

				IntPtr ptr = bd.Scan0;

				for (int i = 0; i < 512; i++)
				{
					if (line == -1)
					{
						line = 511;
					}
					for (int j = 0; j < 256; j++)
					{
						int v = spect[line, j];
						if (v > 255) v = 255;
						ary[i + j * 512] = Color.FromArgb(v, v, v).ToArgb();
					}
					line--;
				}

				System.Runtime.InteropServices.Marshal.Copy(ary, 0, ptr, 512 * 256);

				p1.UnlockBits(bd);

				pictureBox1.Image = p1;
			}
			{
				int len = 512;

				Bitmap p1 = new Bitmap(1, len);
				int line = curLine;

				BitmapData bd = p1.LockBits(new Rectangle(0, 0, 1, len), System.Drawing.Imaging.ImageLockMode.ReadWrite, p1.PixelFormat);

				IntPtr ptr = bd.Scan0;

				byte[] data = new byte[len];
				for (int i = 0; i < 1; i++)
				{
					line--;
					if (line == -1)
					{
						line = 511;
					}
					for (int j = 0; j < len; j++)
					{
						int v = spect[line, j];
						if (v > 255) v = 255;
						ary[i + j * 1] = Color.FromArgb(v, v, v).ToArgb();
						data[j] = (byte)spect[line, j];
						if (data[j] == 0) data[j] = 1;
					}
				}
				Send(data);

				System.Runtime.InteropServices.Marshal.Copy(ary, 0, ptr, 1 * len);

				p1.UnlockBits(bd);

				pictureBox2.Image = p1;
			}
		}
		void Form1_FormClosed(object sender, FormClosedEventArgs e)
		{
			if (port != null && port.IsOpen)
			{
				port.Close();
			}
			//sr2.RecStop();
		}

		int[] ary = new int[512 * 512];

		SerialPort port;


		private void Send(byte[] data)
		{
			if (port != null && port.IsOpen)
			{
				port.Write(data, 0, 48);
			}
		}



	}
}
