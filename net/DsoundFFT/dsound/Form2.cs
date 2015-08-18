﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Imaging;

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

		}

		int[,] spect = new int[512, 512];
		int[] wave = new int[512];
		int curLine = 0;

		int[] ary = new int[512 * 512];
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

				System.Runtime.InteropServices.Marshal.Copy(ary, 0, ptr, 1 * len);

				p1.UnlockBits(bd);

				pictureBox2.Image = p1;
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
				for (int i = 1; i < n / 2; i++)
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
			long[] real = new long[512];

			for (int i = 0; i < n; i++)
			{
				real[i] = array[i] * 256;
			}

			FastFFT.fft(real, 512);

			double max = 65536*2;
			double multi = 256;

			int[] result = new int[n];

			for (int i = 1; i < n; i++)
			{
				int v = (int)real[i];
				if (v < 0)
				{
					v = -v;
				}
				result[i] = v;//最接近整数
				//if (v > max)
				//{
				//	max = v;
				//}
			}
			{
				for (int i = 2; i < n / 2; i++)
				{
					if (i >= array.Length)
					{
						break;
					}
					int j = (int)(result[i] * multi / max);

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
