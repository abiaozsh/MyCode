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
		public string[] files;
		public int basex;
		public int basey;
		public int width;
		public int height;
		List<Report> reports = new List<Report>();

		public Form2()
		{
			InitializeComponent();
		}

		private static int[] BitmapToArray(Bitmap bmp, int w, int h, int basex, int basey)
		{
			int[] img = new int[w * h];

			Rectangle rect = new Rectangle(0, basey, bmp.Width, basey + h);
			System.Drawing.Imaging.BitmapData bmpData = bmp.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadOnly, bmp.PixelFormat);

			int bits = 0;
			if (bmp.PixelFormat == PixelFormat.Format24bppRgb)
			{
				bits = 3;
			}
			if (bmp.PixelFormat == PixelFormat.Format32bppRgb)
			{
				bits = 4;
			}
			IntPtr ptr = bmpData.Scan0;
			int bytes = Math.Abs(bmpData.Stride) * h;
			byte[] rgbValues = new byte[bytes];
			System.Runtime.InteropServices.Marshal.Copy(ptr, rgbValues, 0, bytes);
			for (int i = 0; i < h; i++)
			{
				int idx = (Math.Abs(bmpData.Stride) * i) + basex * bits;
				for (int j = 0; j < w; j++)
				{
					int c = 0;
					c += rgbValues[idx++];
					c += rgbValues[idx++];
					c += rgbValues[idx++];
					if (bmp.PixelFormat == PixelFormat.Format32bppRgb)
					{
						idx++;
					}
					img[j + i * w] = c;
				}
			}
			bmp.UnlockBits(bmpData);
			return img;
		}




		private void button1_Click(object sender, EventArgs e)
		{
			StringBuilder offset = new StringBuilder();
			Bitmap bmp1 = (Bitmap)Image.FromFile(files[0]);
			int[] imgBase = BitmapToArray(bmp1, 64, 64, basex, basey);
			int threads = 4;
			ThProc[] ps = new ThProc[threads];
			for (int i = 0; i < threads; i++)
			{
				ps[i] = new ThProc();
				ps[i].imgBase = imgBase;
				ps[i].files = new List<string>();
				ps[i].basex = basex;
				ps[i].basey = basey;
			}

			Array.Sort(files);

			int threadIdx = 0;
			for (int i = 0; i < files.Length; i++)
			{
				ps[threadIdx].files.Add(files[i]);
				threadIdx++;
				if (threadIdx == threads)
				{
					threadIdx = 0;
				}
			}
			for (int i = 0; i < threads; i++)
			{
				Thread t = new Thread(new ThreadStart(ps[i].th));
				t.Start();
			}
			while (true)
			{
				int done = 1;
				for (int i = 0; i < threads; i++)
				{
					if (ps[i].done == 0)
					{
						done = 0;
						this.Text = ps[i].prog;
						Application.DoEvents();
						break;
					}
				}
				if (done == 1)
				{
					break;
				}
				Thread.Sleep(100);
			}

			for (int i = 0; i < threads; i++)
			{
				reports.AddRange(ps[i].reports);
			}

			reports.Sort(
				delegate(Report c1, Report c2)
				{
					return c1.file.CompareTo(c2.file);
				});

			trackBar1.Minimum = 0;
			trackBar1.Maximum = reports.Count - 1;

			this.Text = "done";
		}


		public class ThProc
		{
			public int[] imgBase;
			public List<Report> reports;
			public List<string> files;
			public int done = 0;
			public int basex;
			public int basey;
			public string prog;

			public void th()
			{
				reports = new List<Report>();

				int minx = basex;
				int miny = basey;

				foreach (var item in files)
				{
					long mindiff;
					int offx;
					int offy;
					try
					{
						Form2.Proc(minx, miny, imgBase, item, out mindiff, out offx, out offy);
					}
					catch
					{
						minx = basex;
						miny = basey;
						Form2.Proc(minx, miny, imgBase, item, out mindiff, out offx, out offy);
					}
					minx += offx;
					miny += offy;
					Report r = new Report();
					r.offx = (minx - basex);
					r.offy = (miny - basey);
					r.mindiff = mindiff;
					r.file = item;
					reports.Add(r);
					prog = item;
				}
				done = 1;
			}
		}

		public static void Proc(int basex, int basey, int[] imgBase, string compImg, out long mindiff, out int minx, out int miny)
		{
			Bitmap bmp2 = (Bitmap)Image.FromFile(compImg);
			int[] imgComp = BitmapToArray(bmp2, 128, 128, basex - 32, basey - 32);

			long[] diff = new long[64 * 64];
			long maxdiff = 0;
			mindiff = long.MaxValue;
			minx = 0;
			miny = 0;

			for (int i = 0; i < 64; i++)
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

			minx -= 32;
			miny -= 32;
		}


		private void Form2_Load(object sender, EventArgs e)
		{

		}

		private void button2_Click(object sender, EventArgs e)
		{
			Form3 f = new Form3();
			f.reports = reports;
			f.width = width;
			f.height = height;
			f.Show();
		}

		private void trackBar1_Scroll(object sender, EventArgs e)
		{
			Calc();
		}

		public void Calc()
		{
			Report report = reports[trackBar1.Value];
			Bitmap bmp1 = (Bitmap)Image.FromFile(reports[0].file);
			int[] Rbase;
			int[] Gbase;
			int[] Bbase;
			Form3.BitmapToArray(bmp1, width, height, out Rbase, out Gbase, out Bbase);


			Bitmap bmp2 = (Bitmap)Image.FromFile(report.file);
			int[] R;
			int[] G;
			int[] B;
			Form3.BitmapToArray(bmp2, width, height, out R, out G, out B);

			Form3.Stack(Rbase, Gbase, Bbase, R, G, B, report.offx, report.offy, width, height);

			var bmp = Form3.ArrayToBitmap(Rbase, Gbase, Bbase, width, height, 512, 0);

			//reports
			pictureBox1.Image = bmp;
			this.Text = trackBar1.Value.ToString();
		}

		int clickx;
		int clicky;

		void pictureBox1_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			if (reports == null || reports.Count == 0) return;
			Report report = reports[trackBar1.Value];
			report.offx += (clickx - e.X);
			report.offy += (clicky - e.Y);
			Calc();
		}

		void pictureBox1_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			clickx = e.X;
			clicky = e.Y;
		}

		void Form2_KeyPress(object sender, System.Windows.Forms.KeyPressEventArgs e)
		{
			Report report = reports[trackBar1.Value];
			if (e.KeyChar == 'w')
			{
				report.offy++;
			}
			if (e.KeyChar == 's')
			{
				report.offy--;
			}
			if (e.KeyChar == 'a')
			{
				report.offx++;
			}
			if (e.KeyChar == 'd')
			{
				report.offx--;
			}
			Calc();
			e.Handled = true;
		}

		private void button4_Click(object sender, EventArgs e)
		{
			reports = new List<Report>();
			string[] soff = textBox1.Text.Split('\n');
			for (int i = 0; i < soff.Length; i++)
			{
				Report r = new Report();
				r.file = soff[i].Split(',')[0];
				r.offx = int.Parse(soff[i].Split(',')[1]);
				r.offy = int.Parse(soff[i].Split(',')[2]);
				r.mindiff = int.Parse(soff[i].Split(',')[3]);

				reports.Add(r);
			}

			trackBar1.Minimum = 0;
			trackBar1.Maximum = reports.Count - 1;
		}

		private void button3_Click(object sender, EventArgs e)
		{

			StringBuilder sb = new StringBuilder();
			foreach (var item in reports)
			{
				sb.AppendLine(item.file + "," + item.offx + "," + item.offy + "," + item.mindiff);
			}
			textBox1.Text = sb.ToString();

		}


	}

	public class Report
	{
		public string file;
		public int offx;
		public int offy;
		public long mindiff;
	}

}
