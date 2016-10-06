using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using Gravity;
using System.Collections.Generic;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using System.IO;

namespace myd3d
{
	public class Form1 : System.Windows.Forms.Form
	{
		private System.ComponentModel.IContainer components;
		private Line baseline;

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
			this.pictureBox1 = new System.Windows.Forms.PictureBox();
			this.pictureBox2 = new System.Windows.Forms.PictureBox();
			this.pictureBox3 = new System.Windows.Forms.PictureBox();
			this.pictureBox4 = new System.Windows.Forms.PictureBox();
			this.textBox1 = new System.Windows.Forms.TextBox();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).BeginInit();
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
			// pictureBox3
			// 
			this.pictureBox3.Location = new System.Drawing.Point(312, 12);
			this.pictureBox3.Name = "pictureBox3";
			this.pictureBox3.Size = new System.Drawing.Size(200, 100);
			this.pictureBox3.TabIndex = 1;
			this.pictureBox3.TabStop = false;
			// 
			// pictureBox4
			// 
			this.pictureBox4.Location = new System.Drawing.Point(12, 114);
			this.pictureBox4.Name = "pictureBox4";
			this.pictureBox4.Size = new System.Drawing.Size(1000, 200);
			this.pictureBox4.TabIndex = 2;
			this.pictureBox4.TabStop = false;
			// 
			// textBox1
			// 
			this.textBox1.Location = new System.Drawing.Point(552, 13);
			this.textBox1.Multiline = true;
			this.textBox1.Name = "textBox1";
			this.textBox1.Size = new System.Drawing.Size(205, 48);
			this.textBox1.TabIndex = 3;
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(6, 14);
			this.ClientSize = new System.Drawing.Size(1025, 324);
			this.Controls.Add(this.textBox1);
			this.Controls.Add(this.pictureBox4);
			this.Controls.Add(this.pictureBox3);
			this.Controls.Add(this.pictureBox2);
			this.Controls.Add(this.pictureBox1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.Name = "Form1";
			this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
			this.Text = "3D";
			this.Load += new System.EventHandler(this.Form1_Load);
			this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyUp);
			this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyDown);
			((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		[STAThread]
		static void Main()
		{
			Application.Run(new Form1());
		}

		////////////////////////////////////////////////////////////////////
		D3DDevice device;

		int tick = 1;
		int tickInc = 1;
		bool keyDw = false;
		bool keyUp = false;
		bool keyRt = false;
		bool keyLt = false;
		private PictureBox pictureBox1;
		private PictureBox pictureBox2;

		bool start = false;

		List<PSpot> traceList = new List<PSpot>();
		private PictureBox pictureBox3;
		private PictureBox pictureBox4;
		private TextBox textBox1;
		List<PSpot> spotList = new List<PSpot>();

		private void save()
		{
			IFormatter formatter = new BinaryFormatter();
			Stream stream = new FileStream("d:\\save.bin", FileMode.Create, FileAccess.Write, FileShare.None);
			formatter.Serialize(stream, traceList);
			stream.Close();
		}

		private void load()
		{
			IFormatter formatter = new BinaryFormatter();
			Stream stream = new FileStream("d:\\save.bin", FileMode.Open, FileAccess.Read, FileShare.Read);
			traceList = (List<PSpot>)formatter.Deserialize(stream);
			stream.Close();

			foreach (PSpot p in traceList)
			{
				p.model = new Model(device.device, "moon.x");
				device.addObject(p.model);
			}
		}

		private void Form1_Load(object sender, System.EventArgs e)
		{

			device = new D3DDevice(this, pictureBox1, pictureBox2, this.setObject);

			///models init----------
			this.Show();

			{
				init();
				//save();
				//return;
			}
			//load();

			device.Start();
			baseline = new Line(device.device);
			baseline.addLine(0, 0, 0, 1, 0, 0, 0xFF0000);
			baseline.addLine(0, 0, 0, 0, 1, 0, 0x00FF00);
			baseline.addLine(0, 0, 0, 0, 0, 1, 0x0000FF);
			baseline.Finish();
			device.addObject(baseline);

			foreach (PSpot p in traceList)
			{
				p.lineCount = 2;
				//device.addObject(p.line);
			}

			///models init---------

		}

		private PSpot addSpot(double posx, double posy, double posz, double headingx, double headingy, double headingz, List<PSpot> spotList, List<PSpot> traceList, int total, double mass)
		{
			PSpot sp = new PSpot();
			sp.pos.X = posx;
			sp.pos.Y = posy;
			sp.pos.Z = posz;
			sp.heading.X = headingx;
			sp.heading.Y = headingy;
			sp.heading.Z = headingz;
			sp.mass = mass;
			sp.trace = new Trace[total];

			sp.model = new Model(device.device, "moon.x");
			device.addObject(sp.model);

			traceList.Add(sp);
			spotList.Add(sp);
			return sp;
		}

		private void init()
		{
			int total = 10000;
			Random rnd = new Random();

			PSpot earth;
			PSpot earth2;
			PSpot moon;
			PSpot sp1;
			PSpot sp2;
			PSpot sp3;

			{
				double m = 500;
				double G = 0.01;
				double x0 = 0;
				double y0 = 0;
				double z0 = 0;
				double mx0 = 0;
				double my0 = 0;
				double mz0 = 0;
				earth = addSpot(x0, y0, z0, mx0, my0, mz0, spotList, traceList, total, m);

				{
					double ang = 0;
					double dist = 20;

					double x = Math.Sin(ang) * dist;
					double z = Math.Cos(ang) * dist;
					double y = 0;

					double speed2 = G * m / (dist);
					double speed = Math.Sqrt(speed2);

					double qx = Math.Sin(ang + Math.PI / 2) * speed;
					double qz = Math.Cos(ang + Math.PI / 2) * speed;

					moon = addSpot(x + x0, y + y0, z + z0, qx + mx0, 0 + my0, qz + mz0, spotList, traceList, total, 0.000001);

				}

				{
					double ang = 0;
					double dist = 20;

					double x = Math.Sin(ang) * dist;
					double z = Math.Cos(ang) * dist;
					double y = 0;

					double speed2 = G * m / (dist);
					double speed = Math.Sqrt(speed2);

					double qx = Math.Sin(ang + Math.PI / 2) * speed;
					double qz = Math.Cos(ang + Math.PI / 2) * speed;

					sp1 = addSpot(x + x0, y + y0, z + z0, qx + mx0, 0 + my0, qz + mz0, spotList, traceList, total, 0.000001);

				}

				{
					double ang = 0;
					double dist = 20;

					double x = Math.Sin(ang) * dist;
					double z = Math.Cos(ang) * dist;
					double y = 0;

					double speed2 = G * m / (dist);
					double speed = Math.Sqrt(speed2);

					double qx = Math.Sin(ang + Math.PI / 2) * speed;
					double qz = Math.Cos(ang + Math.PI / 2) * speed;

					sp2 = addSpot(x + x0, y + y0, z + z0, qx + mx0, 0 + my0, qz + mz0, spotList, traceList, total, 0.000001);

				}

				{
					double ang = 0;
					double dist = 20;

					double x = Math.Sin(ang) * dist;
					double z = Math.Cos(ang) * dist;
					double y = 0;

					double speed2 = G * m / (dist)/7;
					double speed = Math.Sqrt(speed2);

					double qx = Math.Sin(ang + Math.PI / 2) * speed;
					double qz = Math.Cos(ang + Math.PI / 2) * speed;

					sp3 = addSpot(x + x0, y + y0, z + z0, qx + mx0, 0 + my0, qz + mz0, spotList, traceList, total, 0.000001);

				}

			}


			double momentumx = 0;
			double momentumy = 0;
			double momentumz = 0;
			double mass = 0;
			foreach (PSpot s in traceList)
			{
				momentumx += s.heading.X * s.mass;
				momentumy += s.heading.Y * s.mass;
				momentumz += s.heading.Z * s.mass;
				mass += s.mass;
			}

			foreach (PSpot s in traceList)
			{
				s.heading.X -= momentumx / mass;
				s.heading.Y -= momentumy / mass;
				s.heading.Z -= momentumz / mass;
			}

			for (int cnt = 0; cnt < total; cnt++)
			{
				//earth.posx = 0;
				//earth.posy = 0;
				//earth.posz = 0;


				if (cnt == 317)
				{
					sp1.heading.X *= 0.9665;
					sp1.heading.Y *= 0.9665;
					sp1.heading.Z *= 0.9665;

					sp1.heading.X -= 0.1;
				}

				if (cnt == 317)
				{
					sp2.heading.X *= 0.99 - 0.01024;
					sp2.heading.Y *= 0.99 - 0.01024;
					sp2.heading.Z *= 0.99 - 0.01024;
					sp2.heading.Y += 0.1;
				}

				//if (cnt == 1319)
				//{
				//    double speed = 0.2;
				//    double direction = 1.558001;
				//    double angle = -0.1120005;
				//    sp1.headingx += (speed * Math.Sin(direction) * Math.Cos(angle));
				//    sp1.headingy += (speed * Math.Sin(angle));
				//    sp1.headingz += (speed * Math.Cos(direction) * Math.Cos(angle));
				//}

				#region proc
				{
					this.Text = cnt.ToString();
					Application.DoEvents();
				}
				PSpot.ProcessGravity(spotList);

				{
					//posion exchange
					double basex = moon.pos.X;
					double basey = moon.pos.Y;
					double basez = moon.pos.Z;

					double r;
					double ¦Õ;
					double ¦È;
					getPos(earth.pos.X - basex, earth.pos.Y - basey, earth.pos.Z - basez, out r, out ¦Õ, out ¦È);

					foreach (PSpot s in traceList)
					{
						s.trace[cnt] = new Trace();

						double newx = s.pos.X - basex;
						double newy = s.pos.Y - basey;
						double newz = s.pos.Z - basez;

						double r2;
						double ¦Õ2;
						double ¦È2;
						getPos(newx, newy, newz, out r2, out ¦Õ2, out ¦È2);

						¦Õ2 = ¦Õ2 - ¦Õ;
						if (double.IsNaN(¦Õ2))
						{
							¦Õ2 = 0;
						}

						s.trace[cnt].pos = s.pos;

						//s.trace[cnt].x = (float)newx;
						//s.trace[cnt].y = (float)newy;
						//s.trace[cnt].z = (float)newz;

						//s.trace[cnt].pos.X = (float)(Math.Sin(¦Õ2) * r2);
						//s.trace[cnt].pos.Y = (float)newy;
						//s.trace[cnt].pos.Z = (float)(Math.Cos(¦Õ2) * r2);


						s.trace[cnt].mass = s.mass;
					}
				}
				#endregion


			}


			b = new Bitmap(400, 100);
			pictureBox3.Image = b;

			bs = new Bitmap(1000, 200);
			pictureBox4.Image = bs;

		}

		private void getPos(double x, double y, double z, out double r, out double ¦Õ, out double ¦È)
		{
			Color c;
			Trace t = new Trace();
			t.pos.X = (float)x;
			t.pos.Y = (float)y;
			t.pos.Z = (float)z;
			getPos(t, out r, out ¦Õ, out ¦È, out c);
		}
		private void getPos(Trace sp1, out double r, out double ¦Õ, out double ¦È, out Color c)
		{
			//double r;// = sqrt(x * 2 + y * 2 + z * 2);
			//double ¦Õ;// = arctan(y / x);
			//double ¦È;// = arccos(z / r);

			r = Math.Sqrt(sp1.pos.X * sp1.pos.X + sp1.pos.Y * sp1.pos.Y + sp1.pos.Z * sp1.pos.Z);


			if (sp1.pos.X >= 0 && sp1.pos.Z >= 0)
			{
				if (Math.Abs(sp1.pos.X) > Math.Abs(sp1.pos.Z))
				{
					¦Õ = Math.PI / 2 + Math.Atan(sp1.pos.Z / sp1.pos.X);
				}
				else
				{
					¦Õ = Math.PI - Math.Atan(sp1.pos.X / sp1.pos.Z);
				}
			}
			else if (sp1.pos.X >= 0 && sp1.pos.Z < 0)
			{
				if (Math.Abs(sp1.pos.X) > Math.Abs(sp1.pos.Z))
				{
					¦Õ = Math.PI / 2 + Math.Atan(sp1.pos.Z / sp1.pos.X);
				}
				else
				{
					¦Õ = -Math.Atan(sp1.pos.X / sp1.pos.Z);
				}
			}
			else if (sp1.pos.X < 0 && sp1.pos.Z >= 0)
			{
				if (Math.Abs(sp1.pos.X) > Math.Abs(sp1.pos.Z))
				{
					¦Õ = Math.PI + Math.PI / 2 + Math.Atan(sp1.pos.Z / sp1.pos.X);
				}
				else
				{
					¦Õ = Math.PI - Math.Atan(sp1.pos.X / sp1.pos.Z);
				}
			}
			else if (sp1.pos.X < 0 && sp1.pos.Z < 0)
			{
				if (Math.Abs(sp1.pos.X) > Math.Abs(sp1.pos.Z))
				{
					¦Õ = Math.PI + Math.PI / 2 + Math.Atan(sp1.pos.Z / sp1.pos.X);
				}
				else
				{
					¦Õ = 2 * Math.PI - Math.Atan(sp1.pos.X / sp1.pos.Z);
				}
			}
			else
			{
				¦Õ = 0;
			}
			c = Color.Red;
			¦È = Math.Acos(sp1.pos.Y / r);

		}

		Bitmap b;
		Bitmap bs;

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
			if (e.KeyCode == Keys.F2)
			{
				b = new Bitmap(400, 100);
				pictureBox3.Image = b;

				bs = new Bitmap(1000, 200);
				pictureBox4.Image = bs;

			}
			if (e.KeyCode == Keys.Right) keyRt = true;
			if (e.KeyCode == Keys.Left) keyLt = true;
			if (e.KeyCode == Keys.Up) keyUp = true;
			if (e.KeyCode == Keys.Down) keyDw = true;
			if (e.KeyCode == Keys.D0)
			{
				foreach (PSpot p in traceList)
				{
					//device.removeObject(p.line);
					p.line.visible = false;
					//device.addObject(p.line);
				}
			}
			if (e.KeyCode == Keys.D1)
			{
				foreach (PSpot p in traceList)
				{
					device.removeObject(p.line);
					p.lineCount = (e.KeyCode - Keys.D0) * 10;
					device.addObject(p.line);
				}
			}
			if (e.KeyCode >= Keys.D2 && e.KeyCode <= Keys.D9)
			{
				foreach (PSpot p in traceList)
				{
					device.removeObject(p.line);
					p.lineCount = (e.KeyCode - Keys.D0) * 100;
					device.addObject(p.line);
				}
			}

			double r;
			double ¦Õ;
			double ¦È;
			Color c;
			getPos(traceList[1].trace[tick], out r, out ¦Õ, out ¦È, out c);


			//b.SetPixel((int)(¦Õ * 100 / Math.PI), (int)(¦È * 100 / Math.PI), c);
			//pictureBox3.Image = b;

			//int s = (int)(100*Math.Sqrt(

			//    (traceList[1].trace[tick + 1].x - traceList[1].trace[tick].x) * (traceList[1].trace[tick + 1].x - traceList[1].trace[tick].x)+
			//    (traceList[1].trace[tick + 1].y - traceList[1].trace[tick].y) * (traceList[1].trace[tick + 1].y - traceList[1].trace[tick].y)+
			//    (traceList[1].trace[tick + 1].z - traceList[1].trace[tick].z) * (traceList[1].trace[tick + 1].z - traceList[1].trace[tick].z)

			//    ));

			//bs.SetPixel(tick/2, s, c);
			//pictureBox4.Image = bs;

		}
		private void Form1_KeyUp(object sender, KeyEventArgs e)
		{
			if (e.KeyCode == Keys.Right) keyRt = false;
			if (e.KeyCode == Keys.Left) keyLt = false;
			if (e.KeyCode == Keys.Up) keyUp = false;
			if (e.KeyCode == Keys.Down) keyDw = false;
		}

		//private void setLight(Microsoft.DirectX.Direct3D.Lights lights)
		private void setLight(Microsoft.DirectX.Direct3D.LightsCollection lights)
		{
			lights[0].Type = LightType.Point;
			lights[0].Diffuse = Color.Red;
			lights[0].Ambient = Color.White;
			lights[0].Attenuation0 = 1f;
			lights[0].Attenuation1 = 1f;
			lights[0].Attenuation2 = 1f;
			lights[0].Range = 1;
			lights[0].XPosition = 0;
			lights[0].YPosition = 0;
			lights[0].ZPosition = 0;
			lights[0].Enabled = true;
		}

		bool trace = true;
		private void setObject()
		{
			this.textBox1.Text = "double direction = " + device.cam.direction + ";\r\n" + "double angle = " + device.cam.angle + ";";
			D3DCamera cam = device.cam;
			{
				float x = cam.x + (float)(10 * Math.Sin(cam.direction) * Math.Cos(cam.angle));
				float y = cam.y + (float)(10 * Math.Sin(cam.angle));
				float z = cam.z + (float)(10 * Math.Cos(cam.direction) * Math.Cos(cam.angle));
				//baseline.zoom(1, 1, 1);
				baseline.reset_matrix();
				baseline.zoom(2, 2, 2);
				baseline.move(x, y, z);
			}

			if (keyUp)
			{
				tickInc++;
			}
			if (keyDw)
			{
				tickInc--;
			}

			if (start && tick + tickInc < traceList[0].trace.Length - 1 && tick + tickInc > 0)
			{
				tick += tickInc;
			}
			if (keyRt)
			{
				if (tick + tickInc < traceList[0].trace.Length - 1 && tick + tickInc > 0)
				{
					tick += tickInc;
				}
			}
			if (keyLt)
			{
				if (tick - tickInc < traceList[0].trace.Length - 1 && tick - tickInc > 0)
				{
					tick -= tickInc;
				}
			}

			this.Text = tick.ToString() + " " + tickInc.ToString();
			Application.DoEvents();


			float scale = 1;
			foreach (PSpot p in traceList)
			{
				p.model.visible = p.trace[tick].mass != 0;
				p.model.reset_matrix();
				float size = (float)(Math.Pow(p.trace[tick].mass, 0.33333)) / 20;
				if (size < 0.01f)
				{
					size = 0.01f;
				}
				p.model.zoom(size, size, size);
				p.model.move(p.trace[tick].pos);

				if (p.line != null)
				{
					device.removeObject(p.line);
				}
				p.line = new Chain(device.device);

				int r1 = Color.White.R;
				int g1 = Color.White.G;
				int b1 = Color.White.B;

				int r2 = Color.Gray.R;
				int g2 = Color.Gray.G;
				int b2 = Color.Gray.B;
				for (int i = 0; i < p.lineCount; i++)
				{
					int r = (r1 + (r2 - r1) * i / p.lineCount);
					int g = (g1 + (g2 - g1) * i / p.lineCount);
					int b = (b1 + (b2 - b1) * i / p.lineCount);
					//verts[i].Color = Color.FromArgb(0, r, g, b).ToArgb();
					if (tick - i >= p.trace.Length)
					{
						p.line.addNode(new Vector3((float)p.trace[p.trace.Length - 1].pos.X, (float)p.trace[p.trace.Length - 1].pos.Y, (float)p.trace[p.trace.Length - 1].pos.Z), Color.FromArgb(0, r, g, b).ToArgb());
					}
					else if (tick - i < 0)
					{
						p.line.addNode(new Vector3((float)p.trace[0].pos.X, (float)p.trace[0].pos.Y, (float)p.trace[0].pos.Z), Color.FromArgb(0, r, g, b).ToArgb());
					}
					else
					{
						p.line.addNode(new Vector3((float)p.trace[tick - i].pos.X, (float)p.trace[tick - i].pos.Y, (float)p.trace[tick - i].pos.Z), Color.FromArgb(0, r, g, b).ToArgb());
					}
				}
				p.line.Finish();
				device.addObject(p.line);

				p.line.visible = p.line.visible && p.trace[tick].mass != 0;
				p.line.reset_matrix();
				p.line.move(0, 0, 0);

			}

		}
	}
}
