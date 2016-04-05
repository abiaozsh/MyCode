using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace Stack
{
	static class Program
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main()
		{
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			Application.Run(new Form1());
		}


	}
}
/*

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
 
 */