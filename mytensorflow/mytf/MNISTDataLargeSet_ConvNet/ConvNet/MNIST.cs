using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;

namespace GUI
{
	public class Util
	{
		public static bool useSSE = true;

		public delegate void DoSave(StreamWriter sw);
		public static void save(string filename, DoSave sv)
		{
			try
			{
				FileStream fs = new FileStream(filename, FileMode.Create, FileAccess.Write);
				StreamWriter sw = new StreamWriter(fs);
				sv(sw);
				sw.Flush();
				fs.Flush();
				fs.Close();
			}
			catch
			{
			}
		}

		public delegate void DoLoad(StreamReader sr);
		public static void load(string filename, DoLoad ld)
		{
			try
			{
				FileStream fs = new FileStream(filename, FileMode.Open, FileAccess.Read);
				StreamReader sr = new StreamReader(fs);
				ld(sr);
				fs.Close();
			}
			catch (Exception ex)
			{
				ex.ToString();
			}
		}
		public static void log(string txt)
		{
			try
			{
				FileStream fs = new FileStream("log.txt", FileMode.Append, FileAccess.Write);
				StreamWriter sw = new StreamWriter(fs);
				sw.WriteLine(txt);
				sw.Flush();
				fs.Flush();
				fs.Close();
			}
			catch
			{
			}
		}

		static Random rnd = new Random();
		public static float randf(float a, float b)
		{
			return (float)rnd.NextDouble() * (b - a) + a;

		}
		public static int randi(float a, float b)
		{
			return (int)Math.Floor(rnd.NextDouble() * (b - a) + a);
		}

	}

	public class MNISTData
	{
		public static byte[] boundary;
		public static byte[] buffimg = new byte[1024 * 1024 * 64];
		public static byte[] bufflbl = new byte[1024 * 1024];
		public static int Count = 70000;
		public static void makeBoundary()
		{
			for (int i = 0; i < 70000; i++)
			{
				//Bitmap b = _______getBmp(i);
				//int l = _______getLbl(i);
				//b.Save(@"E:\MNIST\imgs\" + l + "\\" + i + ".bmp", System.Drawing.Imaging.ImageFormat.Bmp);
			}


			Util.save(@"..\boundary.txt", (s) =>
			{
				for (int i = 0; i < 70000; i++)
				{
					//Bitmap b = _______getBmp(i);
					//int l = _______getLbl(i);
					//b.Save(@"E:\MNIST\imgs\" + l + "\\" + i + ".bmp", System.Drawing.Imaging.ImageFormat.Bmp);
			
					int left = 100;
					int top = 100;
					int right = 0;
					int bottom = 0;
			
					for (int y = 0; y < 28; y++)
					{
						for (int x = 0; x < 28; x++)
						{
							int pixel = buffimg[i * 28 * 28 + y * 28 + x];
							if (pixel > 4)
							{
								if (y < top) top = y;
								if (y > bottom) bottom = y;
			
								if (x < left) left = x;
								if (x > right) right = x;
							}
						}
					}
			
					s.WriteLine(left);
					s.WriteLine(top);
					s.WriteLine(right);
					s.WriteLine(bottom);
				}
			});
		}

		public static void loadBoundary()
		{
			boundary = new byte[70000 * 4];
			Util.load(@"..\boundary.txt", (s) =>
			{
				for (int i = 0; i < 70000; i++)
				{
					boundary[i * 4] = (byte)int.Parse(s.ReadLine());//left
					boundary[i * 4 + 1] = (byte)int.Parse(s.ReadLine());//top
					boundary[i * 4 + 2] = (byte)int.Parse(s.ReadLine());//right
					boundary[i * 4 + 3] = (byte)int.Parse(s.ReadLine());//bottom
				}
			});
		}
		public static void getBoundary(int idx, out int left, out int top, out int right, out int bottom)
		{
			left = boundary[idx * 4];
			top = boundary[idx * 4 + 1];
			right = boundary[idx * 4 + 2];
			bottom = boundary[idx * 4 + 3];
		}

		public static void makeTrainData()
		{
			FileStream fsdat = new FileStream(@"E:\MNIST\MNISTLargeSetDat.bin",  FileMode.Create, FileAccess.Write);
			FileStream fslbl = new FileStream(@"E:\MNIST\MNISTLargeSetLbl.bin", FileMode.Create, FileAccess.Write);
			byte[] data = new byte[28 * 28];
			Random r = new Random();
			for (int i = 0; i < 2000000; i++)
			{
				int idx = (int)(r.NextDouble() * (Count - 1));
				rotate(idx, data);
				int lbl = getLbl(idx);
				fslbl.WriteByte((byte)lbl);
				fsdat.Write(data, 0, 28 * 28);
				if (i % 10000 == 0) Console.WriteLine(i);
			}
			fsdat.Flush();
			fsdat.Close();
			fslbl.Flush();
			fslbl.Close();
		}

		public static Random rnd = new Random();
		public static void rotate(int idx, byte[] outdata)
		{
			int Width = 28;
			int Heigh = 28;
			int Selectx1;
			int Selectx2;
			int Selecty1;
			int Selecty2;
			getBoundary(idx, out Selectx1, out Selecty1, out Selectx2, out Selecty2);

			double Wid = Math.Abs(Selectx1 - Selectx2);
			double Hei = Math.Abs(Selecty1 - Selecty2);
			int xC = (Selectx1 + Selectx2) / 2;
			int yC = (Selecty1 + Selecty2) / 2;
			//r = (Math.Min(Wid, Hei) + 1) ^ 2;
			double r1 = Math.Max(Wid, Hei);


			double Angle = (rnd.NextDouble() - 0.5) / 4;
			int shiftX = (int)((rnd.NextDouble()) * (28 - Wid) * 0.92 + Wid / 2) + 1;
			int shiftY = (int)((rnd.NextDouble()) * (28 - Hei) * 0.92 + Hei / 2) + 1;
			//double Angle = 0;
			//int shiftX = (int)(Wid / 2);
			//int shiftY = (int)(Hei / 2);

			double s = Angle;// / HUDU
			double cs = Math.Cos(s);
			double ss = Math.Sin(s);
			int pageidx = idx * 28 * 28;
			for (double j = -r1 + 1; j < r1 - 1; j += 1)
			{
				for (double i = -r1 + 1; i <= r1 - 1; i += 1)
				{
					double x = i * cs + j * ss;
					double y = j * cs - i * ss;
					int x_big = (int)(x) + 1;
					int y_big = (int)(y) + 1;
					int x_sml = (int)(x);
					int y_sml = (int)(y);
					int a_color = 0;
					int b_color = 0;
					int c_color = 0;
					int d_color = 0;
					if (x_big + xC >= 0 && x_big + xC < Width && y_big + yC >= 0 && y_big + yC < Heigh)
					{
						//int temp_color = indata[(x_big + xC) * Width + y_big + yC];
						a_color = buffimg[pageidx + (y_big + yC) * 28 + x_big + xC];
						//a_color_b = (temp_color And &HFF)
						//a_color_g = ((temp_color And 65280) \ &H100)
						//a_color_r = ((temp_color And &HFF0000) \ &H10000)
					}
					if (x_big + xC >= 0 && x_big + xC < Width && y_sml + yC >= 0 && y_sml + yC < Heigh)
					{
						//int temp_color = indata[(x_big + xC) * Width + y_sml + yC];
						b_color = buffimg[pageidx + (y_sml + yC) * 28 + x_big + xC];
						//b_color_b = (temp_color And &HFF)
						//b_color_g = ((temp_color And 65280) \ &H100)
						//b_color_r = ((temp_color And &HFF0000) \ &H10000)
					}
					if (x_sml + xC >= 0 && x_sml + xC < Width && y_big + yC >= 0 && y_big + yC < Heigh)
					{
						//int temp_color = indata[(x_sml + xC) * Width + y_big + yC];
						c_color = buffimg[pageidx + (y_big + yC) * 28 + x_sml + xC];
						//c_color_b = (temp_color And &HFF)
						//c_color_g = ((temp_color And 65280) \ &H100)
						//c_color_r = ((temp_color And &HFF0000) \ &H10000)
					}
					if (x_sml + xC >= 0 && x_sml + xC < Width && y_sml + yC >= 0 && y_sml + yC < Heigh)
					{
						//int temp_color = indata[(x_sml + xC) * Width + y_sml + yC];
						d_color = buffimg[pageidx + (y_sml + yC) * 28 + x_sml + xC];
						//d_color_b = (temp_color And &HFF)
						//d_color_g = ((temp_color And 65280) \ &H100)
						//d_color_r = ((temp_color And &HFF0000) \ &H10000)
					}

					double t_color = (a_color * (y - y_sml) + b_color * (y_big - y)) * (x - x_sml) + (c_color * (y - y_sml) + d_color * (y_big - y)) * (x_big - x);
					//t_color_g = (a_color_g * (y - y_sml) + b_color_g * (y_big - y)) * (x - x_sml) + (c_color_g * (y - y_sml) + d_color_g * (y_big - y)) * (x_big - x)
					//t_color_b = (a_color_b * (y - y_sml) + b_color_b * (y_big - y)) * (x - x_sml) + (c_color_b * (y - y_sml) + d_color_b * (y_big - y)) * (x_big - x)
					//t_color = t_color_r * &H10000 + t_color_g * &H100 + t_color_b
					int targetX = (int)(i + shiftX);
					int targetY = (int)(j + shiftY);
					//if (i + xC >= 0 && i + xC <= Width && j + yC >= 0 && j + yC <= Heigh)
					//{
					//	outdata[((int)i + xC) * Width + (int)j + yC] = (int)t_color;
					//}
					if (targetX >= 0 && targetX < Width && targetY >= 0 && targetY < Heigh)
					{
						if (t_color > 255) { 
						t_color = 255;
						}
						if (t_color < 0)
						{
							t_color = 0;
						}
						outdata[targetY * Width + targetX] = (byte)t_color;
					}
				}
			}

		}

		static MNISTData()
		{
			FileStream fsimg1 = new FileStream(@"E:\MNIST\train-images.idx3-ubyte", FileMode.Open, FileAccess.Read);
			FileStream fslbl1 = new FileStream(@"E:\MNIST\train-labels.idx1-ubyte", FileMode.Open, FileAccess.Read);
			FileStream fsimg2 = new FileStream(@"E:\MNIST\t10k-images.idx3-ubyte", FileMode.Open, FileAccess.Read);
			FileStream fslbl2 = new FileStream(@"E:\MNIST\t10k-labels.idx1-ubyte", FileMode.Open, FileAccess.Read);

			fsimg1.Read(buffimg, 0, 16);//head
			int len = fsimg1.Read(buffimg, 0, 1024 * 1024 * 64);
			fsimg2.Read(buffimg, len, 16);
			fsimg2.Read(buffimg, len, 1024 * 1024 * 64 - len);

			fslbl1.Read(bufflbl, 0, 8);//head
			len = fslbl1.Read(bufflbl, 0, 1024 * 1024);
			fslbl2.Read(bufflbl, len, 8);
			fslbl2.Read(bufflbl, len, 1024 * 1024 - len);

			fsimg1.Close();
			fslbl1.Close();
			fsimg2.Close();
			fslbl2.Close();

			try
			{
				loadBoundary();
			}
			catch { }

		}

		static public Bitmap getBmp(int idx)
		{
			Bitmap b = new Bitmap(28, 28);

			for (int j = 0; j < 28; j++)
			{
				for (int i = 0; i < 28; i++)
				{
					int val = buffimg[idx * 28 * 28 + j * 28 + i];
					b.SetPixel(i, j, Color.FromArgb(val, val, val));
				}
			}
			return b;
		}
		static public Bitmap getBmp(int[] data)
		{
			//idx = MNIST.getGoodData(idx);
			Bitmap b = new Bitmap(28, 28);

			for (int j = 0; j < 28; j++)
			{
				for (int i = 0; i < 28; i++)
				{
					int val = data[j * 28 + i];
					if (val > 255) val = 255;
					if (val < 0) val = 0;
					b.SetPixel(i, j, Color.FromArgb(val, val, val));
				}
			}
			return b;
		}
		static public int getLbl(int idx)
		{
			//idx = MNIST.getGoodData(idx);
			return bufflbl[idx];
		}
		public class Pack
		{
			public float predict;
			public int val;
		}

	}

}
