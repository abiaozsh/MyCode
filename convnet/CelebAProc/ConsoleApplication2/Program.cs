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

			FileStream fs = new FileStream(@"E:\MNIST\CelebA\Anno\list_attr_celeba.txt", FileMode.Open, FileAccess.Read);
			StreamReader sr = new StreamReader(fs);
			string ss = sr.ReadToEnd();
			sr.Close();
			fs.Close();
			ss = ss.Replace("  ", " ");
			ss = ss.Replace("  ", " ");
			ss = ss.Replace(" ", ",");

			var s1 = ss.Split('\n');
			int s1Index = 2;

			for (int i = 0; i < 49; i++)
			{
				s1Index = proc(s1, s1Index, i);
			}
		}

		static int proc(string[] s1, int s1Index, int num)
		{

			int OUTW = 160;
			int OUTH = 208;

			FileStream _out = new FileStream("e:\\MNIST\\celebaHI\\" + num + ".bin", FileMode.Create, FileAccess.Write);
			for (int i = 0; i < 4096; i++)//256*16
			{
				var s = s1[s1Index++];
				string filename = s.Split(',')[0];

				Bitmap bmp = (Bitmap)Bitmap.FromFile(@"E:\MNIST\img_align_celeba_total\" + filename);
				BitmapData bitmapData = bmp.LockBits(new Rectangle(0, 0, bmp.Width, bmp.Height), ImageLockMode.ReadOnly, bmp.PixelFormat);

				byte[] Pixels = new byte[bmp.Width * bmp.Height * 3];
				Marshal.Copy(bitmapData.Scan0, Pixels, 0, Pixels.Length);
				bmp.UnlockBits(bitmapData);

				//Bitmap b2 = new Bitmap(OUTY,OUTX);

				int j = 0;//(bmp.Height - OUTY) / 2;
				int k = 0;//(bmp.Width - OUTX) / 2;

				byte[] buff = new byte[OUTW * OUTH * 3];

				for (int x = 0; x < OUTW; x++)
				{
					for (int y = 0; y < OUTH; y++)
					{
						int datax = k + y;
						int datay = j + x;
						byte b = Pixels[datax * bitmapData.Stride + datay * 3 + 0];
						byte g = Pixels[datax * bitmapData.Stride + datay * 3 + 1];
						byte r = Pixels[datax * bitmapData.Stride + datay * 3 + 2];

						buff[y * OUTW * 3 + x * 3 + 0] = r;
						buff[y * OUTW * 3 + x * 3 + 1] = g;
						buff[y * OUTW * 3 + x * 3 + 2] = b;

						//b2.SetPixel(y, x, Color.FromArgb(b, g, r));
					}
				}
				_out.Write(buff, 0, OUTH * OUTW * 3);
				_out.Flush();


				if (i % 64 == 0)
				{
					Console.WriteLine(num+","+i);
				}
				//b2.Save("e:\\MNIST\\fdsa"+i+".bmp");
			}
			_out.Close();
			return s1Index;
		}
	}
}
