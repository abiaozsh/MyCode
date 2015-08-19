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
		DxCapture capture;

		private void Form2_Load(object sender, EventArgs e)
		{
			capture = new DxCapture();
			capture.init(this);
			capture.start();


			try
			{
				this.FormClosed += new FormClosedEventHandler(Form1_FormClosed);

				if (port == null)
				{
					//COM4为Arduino使用的串口号，需根据实际情况调整
					port = new SerialPort("COM12", 9600, Parity.None, 8, StopBits.One);
					port.Open();
				}
			}
			catch
			{
				MessageBox.Show("error");
			}

		}
		void Form1_FormClosed(object sender, FormClosedEventArgs e)
		{
			if (port != null && port.IsOpen)
			{
				port.Close();
			}
		}

		int[,] spect = new int[512, 512];
		int[] wave = new int[512];
		int curLine = 0;

		int[] ary = new int[512 * 512];

		SerialPort port;

		public void timer1_Tick()
		{
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
					}
				}
				Send(data);

				System.Runtime.InteropServices.Marshal.Copy(ary, 0, ptr, 1 * len);

				p1.UnlockBits(bd);

				pictureBox2.Image = p1;
			}
		}
		private void Send(byte[] data)
		{
			if (port != null && port.IsOpen)
			{
				port.Write(data, 0, 48);
			}
		}


		public void proc1(short[] array, int n)
		{
			int[] a = new int[n];

			for (int i = 0; i < n; i++)
			{
				a[i] = array[i];//最接近整数
			}

			Complex[] A = new Complex[n];
			Complex w = fft.ww(n, 1);
			fft.FFT(n, a, w, A);
			float[] result = new float[n];

			float max = 0.1f;
			for (int i = 1; i < n / 2; i++)
			{
				float v = Math.Abs(A[i].real());
				result[i] = v;//最接近整数
				if (v > max)
				{
					max = v;
				}
			}
			//max = 1110.1f;
			{
				for (int i = 1; i < 256; i++)
				{
					if (i >= array.Length)
					{
						break;
					}
					float j = result[i] / max;

					spect[curLine, i] = (int)(j * 255);
				}
			}

			curLine++;
			if (curLine == 512)
			{
				curLine = 0;
			}
		}

		public void proc(short[] array, int n)
		{
			long[] real = new long[n];

			for (int i = 0; i < n; i++)
			{
				real[i] = array[i];
			}

			FastFFT.fft(real, n);

			int[] result = new int[n];

			for (int i = 1; i < n; i++)
			{
				int v = (int)real[i];
				if (v < 0)
				{
					v = -v;
				}
				result[i] = v;
			}
			{
				for (int i = 2; i < 256; i++)
				{
					if (i >= array.Length)
					{
						break;
					}
					int j = (result[i] *128/n);

					spect[curLine, i] = (int)(j);
				}
			}

			curLine++;
			if (curLine == 512)
			{
				curLine = 0;
			}
		}


	}
}
