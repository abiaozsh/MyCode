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
using System.Threading;
using System.Runtime.InteropServices;

namespace myd3d
{
	public class Form1 : System.Windows.Forms.Form
	{
		private System.ComponentModel.IContainer components;

		[DllImport("kernel32.dll")]
		static extern uint GetTickCount();

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
			this.textBox1 = new System.Windows.Forms.TextBox();
			this.button1 = new System.Windows.Forms.Button();
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
			// textBox1
			// 
			this.textBox1.Location = new System.Drawing.Point(312, 12);
			this.textBox1.Multiline = true;
			this.textBox1.Name = "textBox1";
			this.textBox1.Size = new System.Drawing.Size(205, 48);
			this.textBox1.TabIndex = 3;
			// 
			// button1
			// 
			this.button1.Location = new System.Drawing.Point(313, 67);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(75, 23);
			this.button1.TabIndex = 4;
			this.button1.Text = "button1";
			this.button1.UseVisualStyleBackColor = true;
			this.button1.Click += new System.EventHandler(this.button1_Click);
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(6, 14);
			this.ClientSize = new System.Drawing.Size(525, 129);
			this.Controls.Add(this.button1);
			this.Controls.Add(this.textBox1);
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

		string title;
		int tick = 1;
		int tickInc = 1;
		bool keyDw = false;
		bool keyUp = false;
		bool keyRt = false;
		bool keyLt = false;
		private PictureBox pictureBox1;
		private PictureBox pictureBox2;

		bool start = false;

		private TextBox textBox1;
		private Button button1;
		List<PSpot> traceList = new List<PSpot>();
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


			init1();

			//Thread t = new Thread(proc);
			proc();
			//t.Start();
			{
				//init();
				//save();
				//return;
			}
			//load();


			///models init---------
			device.Start();

			foreach (PSpot p in traceList)
			{
				p.lineCount = 2;
				//device.addObject(p.line);
			}

		}

		private PSpot addSpot(double posx, double posy, double posz, double headingx, double headingy, double headingz, List<PSpot> spotList, List<PSpot> traceList, int total, double mass)
		{
			PSpot sp = new PSpot();
			sp.pos.X = (float)posx;
			sp.pos.Y = (float)posy;
			sp.pos.Z = (float)posz;
			sp.heading.X = (float)headingx;
			sp.heading.Y = (float)headingy;
			sp.heading.Z = (float)headingz;
			sp.mass = (float)mass;
			sp.trace = new Trace[total];

			sp.model = new Model(device.device, "ball.x");
			//sp.model = new Model(device.device, "moon.x");
			device.addObject(sp.model);

			traceList.Add(sp);
			spotList.Add(sp);
			return sp;
		}

		int total = 100;

		private void init1()
		{
			Random rnd = new Random();

			double x0 = 50;
			double y0 = 0;
			double z0 = 0;
			double mx0 = 0;
			double my0 = 0;
			double mz0 = -0.15;

			for (int i = 0; i < 200; i++)
			{
				double ang = rnd.NextDouble() * Math.PI * 2;
				double dist = rnd.NextDouble() * 50;

				double x = Math.Sin(ang) * dist;
				double z = Math.Cos(ang) * dist;
				double y = rnd.NextDouble() * 20 - 10;



				addSpot(
					x + x0,
					y + y0,
					z + z0,
					rnd.NextDouble() * 0.03 + mx0,
					rnd.NextDouble() * 0.03 + my0,
					rnd.NextDouble() * 0.03 + mz0,
					spotList,
					traceList,
					total,
					1);

			}

		}

		double Vcount = 0;
		double Vvalue = 0;
		private void proc()
		{

			float momentumx = 0;
			float momentumy = 0;
			float momentumz = 0;
			float mass = 0;
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

			uint a = GetTickCount();
			for (int cnt = 0; cnt < total; cnt++)
			{

				#region proc
				PSpot.ProcessGravity(spotList);

				{
					foreach (PSpot s in traceList)
					{
						s.trace[cnt] = new Trace();
						s.trace[cnt].pos = s.pos;
						s.trace[cnt].mass = s.mass;
					}
				}
				#endregion
				title = cnt.ToString();
				this.Text = title;
				Application.DoEvents();
			}
			uint b = GetTickCount();

			Vcount++;
			Vvalue += (b - a);
			title += ":" + (Vvalue / Vcount);


		}

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
			if (e.KeyCode == Keys.D0)
			{
				foreach (PSpot p in traceList)
				{
					p.line.visible = false;
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

			//double r;
			//double ¦Õ;
			//double ¦È;
			//Color c;
			//getPos(traceList[1].trace[tick], out r, out ¦Õ, out ¦È, out c);


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

		private void setLight(Microsoft.DirectX.Direct3D.Lights lights)
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

			this.Text = title + " " + tick.ToString() + " " + tickInc.ToString();
			Application.DoEvents();


			float scale = 1;
			foreach (PSpot p in traceList)
			{
				if (p.trace[tick] != null)
				{
					//p.model.visible = p.trace[tick].mass > 10;
					p.model.reset_matrix();
					float size = (float)(Math.Pow(p.trace[tick].mass, 0.33333)) / 20;
					if (size < 0.01f)
					{
						size = 0.01f;
					}
					p.model.zoom(size / 20, size / 20, size / 20);
					Vector3 vec = p.trace[tick].pos;
					vec.Multiply(scale);
					p.model.move(vec);

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
							p.line.addNode(p.trace[p.trace.Length - 1].pos, Color.FromArgb(0, r, g, b).ToArgb());
						}
						else if (tick - i < 0)
						{
							p.line.addNode(p.trace[0].pos, Color.FromArgb(0, r, g, b).ToArgb());
						}
						else
						{
							p.line.addNode(p.trace[tick - i].pos, Color.FromArgb(0, r, g, b).ToArgb());
						}
					}
					p.line.Finish();
					device.addObject(p.line);

					p.line.visible = p.line.visible && p.trace[tick].mass != 0;
					p.line.reset_matrix();
					p.line.zoom(scale, scale, scale);
					p.line.move(0, 0, 0);
				}
			}

		}

		private void button1_Click(object sender, EventArgs e)
		{

		}
	}
}
