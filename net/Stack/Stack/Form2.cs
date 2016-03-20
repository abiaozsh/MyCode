using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Imaging;
using System.Threading;

namespace Stack
{
	public partial class Form2 : Form
	{
		public Form2()
		{
			InitializeComponent();
		}

		public Bitmap ArrayToBitmap(int[] img, int w, int h)
		{
			Bitmap bmp = new Bitmap(w, h, PixelFormat.Format24bppRgb);

			Rectangle rect = new Rectangle(0, 0, w, h);
			System.Drawing.Imaging.BitmapData bmpData = bmp.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadWrite, bmp.PixelFormat);
			IntPtr ptr = bmpData.Scan0;
			int bytes = Math.Abs(bmpData.Stride) * h;
			byte[] rgbValues = new byte[bytes];

			for (int i = 0; i < h; i++)
			{
				int idx = (Math.Abs(bmpData.Stride) * i);
				for (int j = 0; j < w; j++)
				{
					byte val = (byte)(img[j + i * w] / 3);
					rgbValues[idx++] = val;
					rgbValues[idx++] = val;
					rgbValues[idx++] = val;
				}
			}
			System.Runtime.InteropServices.Marshal.Copy(rgbValues, 0, ptr, bytes);

			bmp.UnlockBits(bmpData);

			return bmp;
		}

		public Bitmap ArrayToBitmap(long[] img, int w, int h, long max)
		{
			Bitmap bmp = new Bitmap(w, h, PixelFormat.Format24bppRgb);

			Rectangle rect = new Rectangle(0, 0, w, h);
			System.Drawing.Imaging.BitmapData bmpData = bmp.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadWrite, bmp.PixelFormat);
			IntPtr ptr = bmpData.Scan0;
			int bytes = Math.Abs(bmpData.Stride) * h;
			byte[] rgbValues = new byte[bytes];

			for (int i = 0; i < h; i++)
			{
				int idx = (Math.Abs(bmpData.Stride) * i);
				for (int j = 0; j < w; j++)
				{
					byte val = (byte)(img[j + i * w] * 255 / max);
					rgbValues[idx++] = val;
					rgbValues[idx++] = val;
					rgbValues[idx++] = val;
				}
			}
			System.Runtime.InteropServices.Marshal.Copy(rgbValues, 0, ptr, bytes);

			bmp.UnlockBits(bmpData);

			return bmp;
		}

		private int[] BitmapToArray(Bitmap bmp, int w, int h, int basex, int basey)
		{
			int[] img = new int[w * h];

			Rectangle rect = new Rectangle(0, basey, bmp.Width, basey + h);
			System.Drawing.Imaging.BitmapData bmpData = bmp.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadWrite, bmp.PixelFormat);
			IntPtr ptr = bmpData.Scan0;
			int bytes = Math.Abs(bmpData.Stride) * h;
			byte[] rgbValues = new byte[bytes];
			System.Runtime.InteropServices.Marshal.Copy(ptr, rgbValues, 0, bytes);
			for (int i = 0; i < h; i++)
			{
				int idx = (Math.Abs(bmpData.Stride) * i) + basex * 3;
				for (int j = 0; j < w; j++)
				{
					int c = 0;
					c += rgbValues[idx++];
					c += rgbValues[idx++];
					c += rgbValues[idx++];
					img[j + i * w] = c;
				}
			}
			bmp.UnlockBits(bmpData);
			return img;
		}




		private void button1_Click(object sender, EventArgs e)
		{
			StringBuilder offset = new StringBuilder();
			int basex = 80;
			int basey = 145;
			long mindiff;
			int minx = basex;
			int miny = basey;
			Bitmap bmp1 = (Bitmap)Image.FromFile(@"E:\bmp\aa000.bmp");
			int[] imgBase = BitmapToArray(bmp1, 64, 64, basex, basey);
			pictureBox1.Image = ArrayToBitmap(imgBase, 64, 64);
            for (int i = 0; i < 378; i++)
			{
				int offx;
				int offy;
				Proc(minx, miny, imgBase, @"E:\bmp\aa" + i.ToString().PadLeft(3, '0') + ".bmp", out mindiff, out offx, out offy);
				minx += offx;
				miny += offy;
				string s = i + "," + (minx - basex) + "," + (miny - basey) + "," + mindiff;
				this.Text = s;
				offset.AppendLine(s);

				Application.DoEvents();
			}

			textBox1.Text = offset.ToString();
		}

		public void Proc(int basex, int basey, int[] imgBase, string compImg, out long mindiff, out int minx, out int miny)
		{
			int threads = 4;


			Bitmap bmp2 = (Bitmap)Image.FromFile(compImg);
			int[] imgComp = BitmapToArray(bmp2, 128, 128, basex - 32, basey - 32);
			pictureBox2.Image = ArrayToBitmap(imgComp, 128, 128);

			long[] diff = new long[64 * 64];
			long maxdiff = 0;
			mindiff = long.MaxValue;
			minx = 0;
			miny = 0;

			ThProc[] p = new ThProc[threads];
			for (int i = 0; i < threads; i++)
			{
				p[i] = new ThProc();
				p[i].imgBase = imgBase;
				p[i].imgComp = imgComp;
				p[i].diff = diff;
			}

			p[0].start = 0;
			p[0].end = 16;
			p[1].start = 16;
			p[1].end = 32;
			p[2].start = 32;
			p[2].end = 48;
			p[3].start = 48;
			p[3].end = 64;

			for (int i = 0; i < threads; i++)
			{
				Thread t = new Thread(new ThreadStart(p[i].th));
				t.Start();
			}

			while (true)
			{
				int done = 1;
				for (int i = 0; i < threads; i++)
				{
					if (p[i].done == 0)
					{
						done = 0;
						break;
					}
				}
				if (done == 1)
				{
					break;
				}
				Thread.Sleep(1);
			}


			for (int i = 0; i < threads; i++)
			{
				if (p[i].maxdiff > maxdiff)
				{
					maxdiff = p[i].maxdiff;
				}
				if (p[i].mindiff < mindiff)
				{
					mindiff = p[i].mindiff;
					minx = p[i].minx;
					miny = p[i].miny;
				}
			}

			pictureBox3.Image = ArrayToBitmap(diff, 64, 64, maxdiff);

			minx -= 32;
			miny -= 32;
		}

		public class ThProc
		{
			public int start;
			public int end;
			public int[] imgBase;
			public int[] imgComp;
			public long maxdiff = 0;
			public long mindiff = long.MaxValue;
			public int minx = 0;
			public int miny = 0;
			public long[] diff;
			public int done = 0;

			public void th()
			{
				for (int i = start; i < end; i++)
				{
					for (int j = 0; j < 64; j++)
					{
						long diffn = 0;

						for (int y = 0; y < 64; y++)
						{
							int idx = y * 64;
							int idx2 = (y + j) * 128 + i;
							for (int x = 0; x < 64; x++)
							{
								int _diff = imgBase[idx++] - imgComp[idx2++];
								_diff = Math.Abs(_diff);
								diffn += _diff;
							}
						}
						if (diffn > maxdiff)
						{
							maxdiff = diffn;
						}
						if (diffn < mindiff)
						{
							mindiff = diffn;
							minx = i;
							miny = j;
						}

						diff[i + j * 64] = diffn;
					}
				}
				done = 1;
			}
		}

		private void Form2_Load(object sender, EventArgs e)
		{

		}
	}
}
