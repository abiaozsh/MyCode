using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using System.Collections.Generic;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using System.IO;

namespace myd3d
{
	public class Form1 : System.Windows.Forms.Form
	{
		private System.ComponentModel.IContainer components;

		public Form1()
		{
			InitializeComponent();
		}
		protected override void Dispose(bool disposing)
		{
			if (disposing)
			{
				if (components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose(disposing);
		}

		#region Windows
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			this.pictureBox1 = new System.Windows.Forms.PictureBox();
			this.pictureBox2 = new System.Windows.Forms.PictureBox();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
			this.SuspendLayout();
			// 
			// pictureBox1
			// 
			this.pictureBox1.Location = new System.Drawing.Point(12, 12);
			this.pictureBox1.Name = "pictureBox1";
			this.pictureBox1.Size = new System.Drawing.Size(144, 96);
			this.pictureBox1.TabIndex = 0;
			this.pictureBox1.TabStop = false;
			// 
			// pictureBox2
			// 
			this.pictureBox2.Location = new System.Drawing.Point(162, 12);
			this.pictureBox2.Name = "pictureBox2";
			this.pictureBox2.Size = new System.Drawing.Size(144, 96);
			this.pictureBox2.TabIndex = 0;
			this.pictureBox2.TabStop = false;
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(6, 14);
			this.ClientSize = new System.Drawing.Size(318, 172);
			this.Controls.Add(this.pictureBox2);
			this.Controls.Add(this.pictureBox1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.Name = "Form1";
			this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
			this.Text = "3D";
			this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyUp);
			this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyDown);
			this.Load += new System.EventHandler(this.Form1_Load);
			((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		[STAThread]
		static void Main()
		{
			Application.Run(new Form1());
		}


		////////////////////////////////////////////////////////////////////
		D3DDevice device;

		double tick = 0;
		double tickInc = 0;
		float speed = 1f;
		bool keyDw = false;
		bool keyUp = false;
		bool keyRt = false;
		bool keyLt = false;
		private PictureBox pictureBox1;
		private PictureBox pictureBox2;

		bool start = false;

		private void Form1_Load(object sender, System.EventArgs e)
		{
			this.CenterToParent();

			device = new D3DDevice(this,pictureBox1, pictureBox2,this.setObject);

			///models init----------

			init();

			///models init---------
		}

		private void init()
		{



		}

		bool mouseRelease = true;
		int mousex;
		int mousey;
		private void Form1_KeyDown(object sender, KeyEventArgs e)
		{
			if (e.KeyCode == Keys.Escape)
			{
				this.Dispose(true); return;
			}
			if (e.KeyCode == Keys.F1)
			{
				tick = 0;
				start = !start;
			}
			if (e.KeyCode == Keys.Right) keyRt = true;
			if (e.KeyCode == Keys.Left) keyLt = true;
			if (e.KeyCode == Keys.Up) keyUp = true;
			if (e.KeyCode == Keys.Down) keyDw = true;
		}
		private void Form1_KeyUp(object sender, KeyEventArgs e)
		{
			if (e.KeyCode == Keys.Right) keyRt = false;
			if (e.KeyCode == Keys.Left) keyLt = false;
			if (e.KeyCode == Keys.Up) keyUp = false;
			if (e.KeyCode == Keys.Down) keyDw = false;
		}

		private void setObject()
		{
			tick += tickInc;
			if (keyRt)
			{
				tickInc += 0.01;
			}
			if (keyLt)
			{
				tickInc -= 0.01;
			}

			device.removeAllObject();

			double len = 2;

			Line line = new Line(device.device);
			double fi0 = tick / 1f;
			double t = 2;
			double u = 2;
			{
				float z = -4;
				double a = Math.PI / 2 + Math.PI / 4;
				double fi = fi0 + Math.PI / t * 0 + a * u;
				cylinder(line, len, a, fi, z, 0, 1);
			}

			{
				float z = -3;
				double a = Math.PI / 2 - Math.PI / 4;
				double fi = fi0 + Math.PI / t * 1 + a * u;
				cylinder(line, len, a, fi, z, 1, 0);
			}

			{
				float z = -2;
				double a = Math.PI / 2 + Math.PI / 4;
				double fi = fi0 + Math.PI / t * 2 + a * u;
				cylinder(line, len, a, fi, z, 0, 0);
			}

			{
				float z = -1;
				double a = Math.PI / 2 - Math.PI / 4;
				double fi = fi0 + Math.PI / t * 3 + a * u;
				cylinder(line, len, a, fi, z, 1, 0);
			}

			{
				float z = 0;
				double a = Math.PI / 2 + Math.PI / 4;
				double fi = fi0 + Math.PI / t * 4 + a * u;
				cylinder(line, len, a, fi, z, 0, 0);
			}

			{
				float z = 1;
				double a = Math.PI / 2 - Math.PI / 4;
				double fi = fi0 + Math.PI / t * 5 + a * u;
				cylinder(line, len, a, fi, z, 1, 0);
			}

			{
				float z = 2;
				double a = Math.PI / 2 + Math.PI / 4;
				double fi = fi0 + Math.PI / t * 6 + a * u;
				cylinder(line, len, a, fi, z, 0, 0);
			}

			{
				float z = 3;
				double a = Math.PI / 2 - Math.PI / 4;
				double fi = fi0 + Math.PI / t * 7 + a * u;
				cylinder(line, len, a, fi, z, 1, 0);
			}

			line.addLine(0, 0, -4, 0, 0, 3, 0x000000);

			line.Finish();
			device.addObject(line);

		}

		private void cylinder(Line line, double len, double a, double fi, float z, int phase, int print)
		{

			double v = Math.Sin(fi) * Math.Cos(a) + Math.Cos(fi) * Math.Sin(a);
			double n = (Math.Sqrt(len * len + v * v - 1) + v);
			double x = Math.Cos(a) * n;
			double y = Math.Sin(a) * n;

			line.addLine(Math.Sin(fi), Math.Cos(fi), z + 0.1, 0, 0, z + 0.1, 0x00FF00);
			line.addLine(Math.Sin(fi), Math.Cos(fi), z - 0.1, 0, 0, z - 0.1, 0x00FF00);
			line.addLine(Math.Sin(fi), Math.Cos(fi), z, x, y, z, 0x0000FF);

			{
				double n2 = (Math.Sqrt((len + 1) * (len + 1) + v * v - 1) + v);
				double x2 = Math.Cos(a) * n2;
				double y2 = Math.Sin(a) * n2;
				line.addLine(x2, y2, z + 0.1f, x, y, z + 0.1f, 0x404040);
				line.addLine(x2, y2, z - 0.1f, x, y, z - 0.1f, 0x404040);
				line.addLine(x2, y2, z + 0.3f, x, y, z + 0.3f, 0x404040);
				line.addLine(x2, y2, z - 0.3f, x, y, z - 0.3f, 0x404040);

				line.addLine(x2, y2, z + 0.3f, x2, y2, z - 0.3f, 0x404040);

				Color Hot = Color.FromArgb(255, 243, 21);
				Color Ash = Color.FromArgb(170, 118, 106);
				Color Air = Color.FromArgb(77, 153, 223);

				if (phase == 0)
				{
					double df = (fi + Math.PI / 4) / (Math.PI * 4);
					df = df - Math.Floor(df);

					int red = 0;
					int green = 0;
					int blue = 0;
					if (print == 1)
					{
						this.Text = df.ToString();
					}
					if (df > 0 && df < 0.25)
					{
						red = (int)(Hot.R + (Ash.R - Hot.R) * df / 0.25);
						green = (int)(Hot.G + (Ash.G - Hot.G) * df / 0.25);
						blue = (int)(Hot.B + (Ash.B - Hot.B) * df / 0.25);
					}
					else if (df > 0.25 && df < 0.5)
					{
						red = Ash.R;
						green = Ash.G;
						blue = Ash.B;
					}
					else
					{
						red = Air.R;
						green = Air.G;
						blue = Air.B;
					}

					int color = red * 0x10000 + green * 0x100 + blue;
					{
						double x1 = Math.Cos(a) * 4.1;
						double y1 = Math.Sin(a) * 4.1;
						Face face = new Face(device.device);

						face.addFace(
							x1, y1, z + 0.3,
							x2, y2, z + 0.3,
							x2, y2, z - 0.3,
							color);

						face.addFace(
							x1, y1, z - 0.3,
							x1, y1, z + 0.3,
							x2, y2, z - 0.3,
							color);

						face.Finish();
						device.addObject(face);
					}
					if (df < 0.15 || df > 0.95)
					{
						double x1 = Math.Cos(a) * 4.5;
						double y1 = Math.Sin(a) * 4.5;
						float size = 0.1f;
						line.addLine(x1, y1, z, x1 + size, y1, z, 0xFF0000);
						line.addLine(x1, y1, z, x1 - size, y1, z, 0xFF0000);
						line.addLine(x1, y1, z, x1, y1 + size, z, 0xFF0000);
						line.addLine(x1, y1, z, x1, y1 - size, z, 0xFF0000);
						line.addLine(x1, y1, z, x1, y1, z + size, 0xFF0000);
						line.addLine(x1, y1, z, x1, y1, z - size, 0xFF0000);
					}

					//valve
					double top1;
					double top2;
					if (df < 0.25)
					{
						top1 = 1;
						top2 = 1;
					}
					else if (df > 0.25 && df < 0.5)
					{
						top1 = 1;
						top2 = 1.1;
					}
					else if (df > 0.5 && df < 0.75)
					{
						top1 = 1.1;
						top2 = 1;
					}
					else
					{
						top1 = 1;
						top2 = 1;
					}
					{
						double xa1 = Math.Cos(a) * 4.4;
						double ya1 = Math.Sin(a) * 4.4;
						double xa2 = Math.Cos(a) * 4.2;
						double ya2 = Math.Sin(a) * 4.2;
						line.addLine(xa1 * top1, ya1 * top1, z + 0.2f, xa2 * top1, ya2 * top1, z + 0.2, 0x4040FF);
						line.addLine(xa1 * top2, ya1 * top2, z - 0.2f, xa2 * top2, ya2 * top2, z - 0.2, 0xFF4040);
					}
					{
						double xa1 = Math.Cos(a) * 4.2;
						double ya1 = Math.Sin(a) * 4.2;
						double xa2 = Math.Cos(a) * 4.2;
						double ya2 = Math.Sin(a) * 4.2;
						line.addLine(xa1 * top1, ya1 * top1, z + 0.4f, xa2 * top1, ya2 * top1, z + 0.05, 0x4040FF);
						line.addLine(xa1 * top2, ya1 * top2, z - 0.4f, xa2 * top2, ya2 * top2, z - 0.05, 0xFF4040);
					}
				}
				else
				{
					double df = (fi - Math.PI / 4) / (Math.PI * 4);
					df = df - Math.Floor(df);

					int red = 0;
					int green = 0;
					int blue = 0;
					if (print == 1)
					{
						this.Text = df.ToString();
					}
					if (df > 0 && df < 0.25)
					{
						red = (int)(Hot.R + (Ash.R - Hot.R) * df / 0.25);
						green = (int)(Hot.G + (Ash.G - Hot.G) * df / 0.25);
						blue = (int)(Hot.B + (Ash.B - Hot.B) * df / 0.25);
					}
					else if (df > 0.25 && df < 0.5)
					{
						red = Ash.R;
						green = Ash.G;
						blue = Ash.B;
					}
					else
					{
						red = Air.R;
						green = Air.G;
						blue = Air.B;
					}

					int color = red * 0x10000 + green * 0x100 + blue;
					{
						double x1 = Math.Cos(a) * 4.1;
						double y1 = Math.Sin(a) * 4.1;
						Face face = new Face(device.device);

						face.addFace(
							x2, y2, z + 0.3,
							x1, y1, z + 0.3,
							x2, y2, z - 0.3,
							color);

						face.addFace(
							x1, y1, z + 0.3,
							x1, y1, z - 0.3,
							x2, y2, z - 0.3,
							color);

						face.Finish();
						device.addObject(face);
					}
					if (df < 0.15 || df > 0.95)
					{
						double x1 = Math.Cos(a) * 4.5;
						double y1 = Math.Sin(a) * 4.5;
						float size = 0.1f;
						line.addLine(x1, y1, z, x1 + size, y1, z, 0xFF0000);
						line.addLine(x1, y1, z, x1 - size, y1, z, 0xFF0000);
						line.addLine(x1, y1, z, x1, y1 + size, z, 0xFF0000);
						line.addLine(x1, y1, z, x1, y1 - size, z, 0xFF0000);
						line.addLine(x1, y1, z, x1, y1, z + size, 0xFF0000);
						line.addLine(x1, y1, z, x1, y1, z - size, 0xFF0000);
					}

					//valve
					double top1;
					double top2;
					if (df < 0.25)
					{
						top1 = 1;
						top2 = 1;
					}
					else if (df > 0.25 && df < 0.5)
					{
						top1 = 1;
						top2 = 1.1;
					}
					else if (df > 0.5 && df < 0.75)
					{
						top1 = 1.1;
						top2 = 1;
					}
					else
					{
						top1 = 1;
						top2 = 1;
					}
					{
						double xa1 = Math.Cos(a) * 4.4;
						double ya1 = Math.Sin(a) * 4.4;
						double xa2 = Math.Cos(a) * 4.2;
						double ya2 = Math.Sin(a) * 4.2;
						line.addLine(xa1 * top1, ya1 * top1, z + 0.2f, xa2 * top1, ya2 * top1, z + 0.2, 0x4040FF);
						line.addLine(xa1 * top2, ya1 * top2, z - 0.2f, xa2 * top2, ya2 * top2, z - 0.2, 0xFF4040);
					}
					{
						double xa1 = Math.Cos(a) * 4.2;
						double ya1 = Math.Sin(a) * 4.2;
						double xa2 = Math.Cos(a) * 4.2;
						double ya2 = Math.Sin(a) * 4.2;
						line.addLine(xa1 * top1, ya1 * top1, z + 0.4f, xa2 * top1, ya2 * top1, z + 0.05, 0x4040FF);
						line.addLine(xa1 * top2, ya1 * top2, z - 0.4f, xa2 * top2, ya2 * top2, z - 0.05, 0xFF4040);
					}
				}
			}

			//case
			{
				double x1 = Math.Cos(a) * 4.1;
				double y1 = Math.Sin(a) * 4.1;
				double x2 = Math.Cos(a) * 1;
				double y2 = Math.Sin(a) * 1;
				line.addLine(x1, y1, z + 0.4, x2, y2, z + 0.4, 0x505050);
				line.addLine(x1, y1, z - 0.4, x2, y2, z - 0.4, 0x505050);
			}


		}

	}
}
