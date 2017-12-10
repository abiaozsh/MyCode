using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;

namespace ConsoleApplication2
{
	class Program
	{
		static void Main(string[] args)
		{
			FileStream _out = new FileStream("e:\\MNIST\\celebaHI.bin", FileMode.Create, FileAccess.Write);

			FileStream fs = new FileStream(@"E:\MNIST\CelebA\Anno\list_attr_celeba.txt", FileMode.Open, FileAccess.Read);
			StreamReader sr = new StreamReader(fs);
			string ss = sr.ReadToEnd();
			sr.Close();
			fs.Close();
			ss = ss.Replace("  ", " ");
			ss = ss.Replace("  ", " ");
			ss = ss.Replace(" ", ",");

			var s1 = ss.Split('\n');

			int OUTX = 160;//160 208
			int OUTY = 208;

			for (int i = 2; i < 1000; i++)//s1.Length - 1
			{
				var s = s1[i];
				string filename = s.Split(',')[0];

				Bitmap bmp = (Bitmap)Bitmap.FromFile(@"E:\MNIST\img_align_celeba_total\" + filename);
				BitmapData bitmapData = bmp.LockBits(new Rectangle(0, 0, bmp.Width, bmp.Height), ImageLockMode.ReadOnly, bmp.PixelFormat);

				byte[] Pixels = new byte[bmp.Width * bmp.Height * 3];
				Marshal.Copy(bitmapData.Scan0, Pixels, 0, Pixels.Length);
				bmp.UnlockBits(bitmapData);

				//Bitmap b2 = new Bitmap(OUTY,OUTX);

				int j = 0;//(bmp.Height - OUTY) / 2;
				int k = 0;//(bmp.Width - OUTX) / 2;

				byte[] buff = new byte[OUTX * OUTY * 3];
				for (int x = 0; x < OUTX; x++)
				{
					for (int y = 0; y < OUTY; y++)
					{
						int datax = k + y;
						int datay = j + x;
						byte r = Pixels[datax * bitmapData.Stride + datay * 3 + 0];
						byte g = Pixels[datax * bitmapData.Stride + datay * 3 + 1];
						byte b = Pixels[datax * bitmapData.Stride + datay * 3 + 2];
						_out.WriteByte(b);
						_out.WriteByte(g);
						_out.WriteByte(r);

						//b2.SetPixel(y, x, Color.FromArgb(b, g, r));
					}
				}
				if (i % 100 == 0)
				{
					Console.WriteLine(i);
				}
				//b2.Save("e:\\MNIST\\fdsa"+i+".bmp");

				_out.Flush();

			}

			_out.Close();
		}
	}
}
