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
		public Form3()
		{
			InitializeComponent();
		}
		int[] RBase;
		int[] GBase;
		int[] BBase;

		private void button1_Click(object sender, EventArgs e)
		{
			RBase = new int[640 * 480];
			GBase = new int[640 * 480];
			BBase = new int[640 * 480];

			string[] soff = textBox1.Text.Split('\n');
			for (int i = 0; i < 378; i++)
			{

				int offx = int.Parse(soff[i].Split(',')[1]);
				int offy = int.Parse(soff[i].Split(',')[2]);


				Stack(RBase, GBase, BBase, @"E:\bmp\aa" + i.ToString().PadLeft(3, '0') + ".bmp", offx, offy);
			}

			FileStream fs = new FileStream(@"e:\bmp\out.dat", FileMode.Create, FileAccess.Write);
			StreamWriter sw = new StreamWriter(fs);
			for (int i = 0; i < 640 * 480; i++)
			{
				sw.WriteLine(RBase[i] + "," + GBase[i] + "," + BBase[i]);
			}
			sw.Flush();
			fs.Close();

		}

		private void button2_Click(object sender, EventArgs e)
		{
			RBase = new int[640 * 480];
			GBase = new int[640 * 480];
			BBase = new int[640 * 480];

			FileStream fs = new FileStream(@"e:\bmp\out.dat", FileMode.Open, FileAccess.Read);
			StreamReader sr = new StreamReader(fs);
			int max = 0;
			for (int i = 0; i < 640 * 480; i++)
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
			pictureBox1.Image = ArrayToBitmap(RBase, GBase, BBase, 640, 480, trackBar2.Value, trackBar1.Value);

		}


		public Bitmap ArrayToBitmap(int[] R, int[] G, int[] B, int w, int h, int top, int bottom)
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

			bmp.Save(@"e:\bmp\out.bmp", ImageFormat.Bmp);

			return bmp;
		}
		public byte Conv(int val, int top, int bottom)
		{
			long v = val;
			v -= bottom;
			v *= 256;
			v /= (top - bottom);
			if (v < 0) v = 0;
			if (v > 255) v = 255;
			return (byte)v;
		}

		private void BitmapToArray(Bitmap bmp, int w, int h, out int[] R, out int[] G, out int[] B)
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
					idx2++;
				}
			}
			bmp.UnlockBits(bmpData);
		}

		public void Stack(int[] RBase, int[] GBase, int[] BBase, string filename, int offx, int offy)
		{


			Bitmap bmp2 = (Bitmap)Image.FromFile(filename);
			int[] R;
			int[] G;
			int[] B;
			BitmapToArray(bmp2, 640, 480, out R, out G, out B);

			for (int j = 0; j < 480; j++)
			{
				if (offx > 0)
				{
					int idx = j * 640;
					int idx2 = (j + offy) * 640 + offx;
					if (j + offy >= 480 || j + offy < 0) continue;

					for (int i = 0; i < 640 - Math.Abs(offx); i++)
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
					int idx = j * 640 - offx;
					int idx2 = (j + offy) * 640;
					if (j + offy >= 480 || j + offy < 0) continue;

					for (int i = 0; i < 640 - Math.Abs(offx); i++)
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
			pictureBox1.Image = ArrayToBitmap(RBase, GBase, BBase, 640, 480, trackBar2.Value, trackBar1.Value);

		}

		private void trackBar1_Scroll(object sender, EventArgs e)
		{
			pictureBox1.Image = ArrayToBitmap(RBase, GBase, BBase, 640, 480, trackBar2.Value, trackBar1.Value);

		}

		private void trackBar2_Scroll(object sender, EventArgs e)
		{
			pictureBox1.Image = ArrayToBitmap(RBase, GBase, BBase, 640, 480, trackBar2.Value, trackBar1.Value);

		}

	}
}
