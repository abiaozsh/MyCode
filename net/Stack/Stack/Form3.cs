using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Imaging;
using System.IO;

namespace Stack
{
	public partial class Form3 : Form
	{
		public int width;
		public int height;
		public List<Report> reports = new List<Report>();


		public Form3()
		{
			InitializeComponent();
		}
		int[] RBase;
		int[] GBase;
		int[] BBase;

		private void button1_Click(object sender, EventArgs e)
		{
			RBase = new int[width * height];
			GBase = new int[width * height];
			BBase = new int[width * height];

			foreach (var item in reports)
			{

				int offx = item.offx;
				int offy = item.offy;

				Bitmap bmp2 = (Bitmap)Image.FromFile(item.file);
				int[] R;
				int[] G;
				int[] B;
				BitmapToArray(bmp2, width, height, out R, out G, out B);

				Stack(RBase, GBase, BBase, R, G, B, offx, offy, width, height);
			}

			FileStream fs = new FileStream(textBox2.Text, FileMode.Create, FileAccess.Write);
			StreamWriter sw = new StreamWriter(fs);
			for (int i = 0; i < width * height; i++)
			{
				sw.WriteLine(RBase[i] + "," + GBase[i] + "," + BBase[i]);
			}
			sw.Flush();
			fs.Close();

		}

		private void button2_Click(object sender, EventArgs e)
		{
			RBase = new int[width * height];
			GBase = new int[width * height];
			BBase = new int[width * height];

			FileStream fs = new FileStream(textBox2.Text, FileMode.Open, FileAccess.Read);
			StreamReader sr = new StreamReader(fs);
			int max = 0;
			for (int i = 0; i < width * height; i++)
			{
				string line = sr.ReadLine();

				RBase[i] = int.Parse(line.Split(',')[0]);
				if (RBase[i] > max)
				{
					max = RBase[i];
				}
				GBase[i] = int.Parse(line.Split(',')[1]);
				if (GBase[i] > max)
				{
					max = GBase[i];
				}
				BBase[i] = int.Parse(line.Split(',')[2]);
				if (BBase[i] > max)
				{
					max = BBase[i];
				}
			}
			fs.Close();
			trackBar1.Maximum = max;
			trackBar2.Maximum = max;
			trackBar2.Value = max;
			pictureBox1.Image = ArrayToBitmap(RBase, GBase, BBase, width, height, trackBar2.Value, trackBar1.Value);

		}


		public static Bitmap ArrayToBitmap(int[] R, int[] G, int[] B, int w, int h, int top, int bottom)
		{
			Bitmap bmp = new Bitmap(w, h, PixelFormat.Format24bppRgb);
			if (top - bottom <= 0) return bmp;

			Rectangle rect = new Rectangle(0, 0, w, h);
			System.Drawing.Imaging.BitmapData bmpData = bmp.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadWrite, bmp.PixelFormat);
			IntPtr ptr = bmpData.Scan0;
			int bytes = Math.Abs(bmpData.Stride) * h;
			byte[] rgbValues = new byte[bytes];

			for (int i = 0; i < h; i++)
			{
				int idx = (Math.Abs(bmpData.Stride) * i);
				int idx2 = i * w;
				for (int j = 0; j < w; j++)
				{
					rgbValues[idx++] = Conv(R[idx2], top, bottom);
					rgbValues[idx++] = Conv(G[idx2], top, bottom);
					rgbValues[idx++] = Conv(B[idx2], top, bottom);
					idx2++;
				}
			}
			System.Runtime.InteropServices.Marshal.Copy(rgbValues, 0, ptr, bytes);

			bmp.UnlockBits(bmpData);

			return bmp;
		}
		public static byte Conv(int val, int top, int bottom)
		{
			long v = val;
			v -= bottom;
			v *= 256;
			v /= (top - bottom);
			if (v < 0) v = 0;
			if (v > 255) v = 255;
			return (byte)v;
		}

		public static void BitmapToArray(Bitmap bmp, int w, int h, out int[] R, out int[] G, out int[] B)
		{
			R = new int[w * h];
			G = new int[w * h];
			B = new int[w * h];

			Rectangle rect = new Rectangle(0, 0, bmp.Width, bmp.Height);
			System.Drawing.Imaging.BitmapData bmpData = bmp.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadWrite, bmp.PixelFormat);
			IntPtr ptr = bmpData.Scan0;
			int bytes = Math.Abs(bmpData.Stride) * h;
			byte[] rgbValues = new byte[bytes];
			System.Runtime.InteropServices.Marshal.Copy(ptr, rgbValues, 0, bytes);
			for (int i = 0; i < h; i++)
			{
				int idx = (Math.Abs(bmpData.Stride) * i);
				int idx2 = i * w;
				for (int j = 0; j < w; j++)
				{
					R[idx2] = rgbValues[idx++];
					G[idx2] = rgbValues[idx++];
					B[idx2] = rgbValues[idx++];
					if (bmp.PixelFormat == PixelFormat.Format32bppRgb)
					{
						idx++;
					}
					idx2++;
				}
			}
			bmp.UnlockBits(bmpData);
		}

		public static void Stack(int[] RBase, int[] GBase, int[] BBase, int[] R, int[] G, int[] B, int offx, int offy, int width, int height)
		{

			for (int j = 0; j < height; j++)
			{
				if (offx > 0)
				{
					int idx = j * width;
					int idx2 = (j + offy) * width + offx;
					if (j + offy >= height || j + offy < 0) continue;

					for (int i = 0; i < width - Math.Abs(offx); i++)
					{
						RBase[idx] += R[idx2];
						GBase[idx] += G[idx2];
						BBase[idx] += B[idx2];
						idx++;
						idx2++;
					}
				}
				else
				{
					int idx = j * width - offx;
					int idx2 = (j + offy) * width;
					if (j + offy >= height || j + offy < 0) continue;

					for (int i = 0; i < width - Math.Abs(offx); i++)
					{
						RBase[idx] += R[idx2];
						GBase[idx] += G[idx2];
						BBase[idx] += B[idx2];
						idx++;
						idx2++;
					}
				}
			}

		}

		private void Form3_Load(object sender, EventArgs e)
		{

		}

		private void button3_Click(object sender, EventArgs e)
		{
			var bmp = ArrayToBitmap(RBase, GBase, BBase, width, height, trackBar2.Value, trackBar1.Value);
			pictureBox1.Image = bmp;
			bmp.Save(@"e:\bmp\out.bmp", ImageFormat.Bmp);

		}

		private void trackBar1_Scroll(object sender, EventArgs e)
		{
			pictureBox1.Image = ArrayToBitmap(RBase, GBase, BBase, width, height, trackBar2.Value, trackBar1.Value);

		}

		private void trackBar2_Scroll(object sender, EventArgs e)
		{
			pictureBox1.Image = ArrayToBitmap(RBase, GBase, BBase, width, height, trackBar2.Value, trackBar1.Value);

		}
	}
}
