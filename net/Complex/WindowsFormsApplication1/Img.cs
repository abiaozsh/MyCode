using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{

	public delegate Complex Func(Complex z);

	public delegate void RefreshEventHandler();

	public class Img
	{
		public int Size;


		public double scale = 1;
		public Complex vect;
		public double shiftX = 0;
		public double shiftY = 0;
		public double currentShiftX = 0;
		public double currentShiftY = 0;

		PictureBox pictureBox;
		bool isVectInput;
		public Func func;

		public event RefreshEventHandler hRefresh;

		public Img(PictureBox pictureBox, Func func, bool isVectInput = false)
		{
			this.pictureBox = pictureBox;
            Size = pictureBox.Width;
			this.func = func;
			this.isVectInput = isVectInput;
			pictureBox.MouseMove += MouseMove;
			if (!isVectInput)
			{
				pictureBox.MouseUp += MouseUp;
				pictureBox.MouseDown += MouseDown;
			}
			pictureBox.MouseWheel += MouseWheel;
		}

		int baseX;
		int baseY;
		bool mouseDown = false;
		void MouseDown(object sender, MouseEventArgs e)
		{
			baseX = e.X;
			baseY = Size - e.Y - 1;
			mouseDown = true;
		}

		void MouseUp(object sender, MouseEventArgs e)
		{
			shiftX += (e.X - baseX) * scale;
			shiftY += (Size - e.Y - 1 - baseY) * scale;
			mouseDown = false;
			if (hRefresh != null)
			{
				hRefresh.Invoke();
			}
		}

		void MouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
		{
            pictureBox.Focus();
			if (e.Button == System.Windows.Forms.MouseButtons.Left)
			{
				if (isVectInput)
				{
					double size2 = Size / 2.0;
					double x = (e.X - size2) / size2;
					double y = (Size - e.Y - 1 - size2) / size2;
					vect.a = x * scale;
					vect.b = y * scale;
					if (hRefresh != null)
					{
						hRefresh.Invoke();
					}
					refresh();
				}
				else
				{
					currentShiftX = shiftX + (e.X - baseX) * scale;
					currentShiftY = shiftY + (Size - e.Y - 1 - baseY) * scale;
					refresh();
				}
			}
		}
		void MouseWheel(object sender, MouseEventArgs e)
		{
			if (e.Delta > 0)
			{
				scale *= 0.9;
			}
			else
			{
				scale *= 1.1;
			}
			refresh(true);
		}


		int[] b = null;
		public void refresh(bool force = false)
		{
			try
			{
				if (!isVectInput || b == null || force)
				{
					b = new int[Size * Size];
					Parallel.For(0, 4, n =>
					{
						for (int i = n * Size / 4; i < n * Size / 4 + Size / 4; i++)
						{
							for (int j = 0; j < Size; j++)
							{
								double s2 = 1.0 / Size;
								double x = ((i - currentShiftX / scale) * 2 - Size) * s2;
								double y = ((j - currentShiftY / scale) * 2 - Size) * s2;
								Complex z = new Complex(x * scale, y * scale);

								var z1 = func(z);

								b[i + (Size - j - 1) * Size] = Complex.getColor(z1);
							}
						}
					});
				}

				Draw(b);
			}
			catch (Exception ex)
			{
				ex.ToString();
			}
		}

		public void Draw(int[] array)
		{
			Bitmap b = new Bitmap(Size, Size);
			var l = b.LockBits(new Rectangle(0, 0, Size, Size), System.Drawing.Imaging.ImageLockMode.WriteOnly, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
			Marshal.Copy(array, 0, l.Scan0, Size * Size); b.UnlockBits(l);

			Graphics g = Graphics.FromImage(b);
			Pen p = new Pen(Color.White);
			g.DrawLine(
				p,
				(int)(Size / 2),
				(int)(Size / 2),
				(int)((vect.a * Size / scale + Size) / 2),
				(int)((-vect.b * Size / scale + Size) / 2)
			);
			Font f = new Font("宋体", 8);
			if (isVectInput)
			{
				g.DrawString(vect.a + "," + vect.b, f, new SolidBrush(Color.White), 5, 5);
			}
			else
			{
				float x1 = (float)(((-currentShiftX) * 2) / Size);
				float y1 = (float)(((-currentShiftY) * 2) / Size);
				g.DrawString(x1 + "," + y1, f, new SolidBrush(Color.White), 5, 5);


				double s2 = 1.0 / Size;
				double x = ((Size / 2 - currentShiftX / scale) * 2 - Size) * s2;
				double y = ((Size / 2 - currentShiftY / scale) * 2 - Size) * s2;
				Complex z = new Complex(x * scale, y * scale);

				var z1 = func(z);

				g.DrawString((float)z1.a + "," + (float)z1.b, f, new SolidBrush(Color.White), 5, 15);

			}
			if (mouseDown)
			{
				g.DrawLine(
					p,
					(int)(0),
					(int)(Size / 2),
					(int)(Size),
					(int)(Size / 2)
				);
				g.DrawLine(
					p,
					(int)(Size / 2),
					(int)(0),
					(int)(Size / 2),
					(int)(Size)
				);

			}
			g.Flush();
			g.Dispose();
			pictureBox.Image = b;
		}

	}
}
