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
using Electric;

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
			this.pictureBox1 = new System.Windows.Forms.PictureBox();
			this.pictureBox2 = new System.Windows.Forms.PictureBox();
			this.listBox1 = new System.Windows.Forms.ListBox();
			this.button1 = new System.Windows.Forms.Button();
			this.trackBar1 = new System.Windows.Forms.TrackBar();
			this.trackBar2 = new System.Windows.Forms.TrackBar();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.trackBar1)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.trackBar2)).BeginInit();
			this.SuspendLayout();
			// 
			// pictureBox1
			// 
			this.pictureBox1.Location = new System.Drawing.Point(12, 167);
			this.pictureBox1.Name = "pictureBox1";
			this.pictureBox1.Size = new System.Drawing.Size(749, 547);
			this.pictureBox1.TabIndex = 0;
			this.pictureBox1.TabStop = false;
			this.pictureBox1.MouseMove += new System.Windows.Forms.MouseEventHandler(this.pictureBox1_MouseMove);
			// 
			// pictureBox2
			// 
			this.pictureBox2.Location = new System.Drawing.Point(312, 1);
			this.pictureBox2.Name = "pictureBox2";
			this.pictureBox2.Size = new System.Drawing.Size(144, 96);
			this.pictureBox2.TabIndex = 0;
			this.pictureBox2.TabStop = false;
			// 
			// listBox1
			// 
			this.listBox1.FormattingEnabled = true;
			this.listBox1.ItemHeight = 12;
			this.listBox1.Location = new System.Drawing.Point(12, 1);
			this.listBox1.Name = "listBox1";
			this.listBox1.Size = new System.Drawing.Size(294, 64);
			this.listBox1.TabIndex = 1;
			// 
			// button1
			// 
			this.button1.Location = new System.Drawing.Point(12, 71);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(75, 23);
			this.button1.TabIndex = 2;
			this.button1.Text = "button1";
			this.button1.UseVisualStyleBackColor = true;
			this.button1.Click += new System.EventHandler(this.button1_Click);
			// 
			// trackBar1
			// 
			this.trackBar1.LargeChange = 10;
			this.trackBar1.Location = new System.Drawing.Point(93, 71);
			this.trackBar1.Maximum = 100;
			this.trackBar1.Name = "trackBar1";
			this.trackBar1.Size = new System.Drawing.Size(213, 42);
			this.trackBar1.TabIndex = 3;
			this.trackBar1.TickFrequency = 10;
			// 
			// trackBar2
			// 
			this.trackBar2.LargeChange = 10;
			this.trackBar2.Location = new System.Drawing.Point(93, 119);
			this.trackBar2.Maximum = 100;
			this.trackBar2.Name = "trackBar2";
			this.trackBar2.Size = new System.Drawing.Size(213, 42);
			this.trackBar2.TabIndex = 4;
			this.trackBar2.TickFrequency = 10;
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(6, 14);
			this.ClientSize = new System.Drawing.Size(887, 745);
			this.Controls.Add(this.trackBar2);
			this.Controls.Add(this.trackBar1);
			this.Controls.Add(this.button1);
			this.Controls.Add(this.listBox1);
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
			((System.ComponentModel.ISupportInitialize)(this.trackBar1)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.trackBar2)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		[STAThread]
		static void Main()
		{
			test.makedata();
			Application.Run(new Form1());
		}


		////////////////////////////////////////////////////////////////////
		D3DDevice device;

		public int traceIdx;
		bool keyDw = false;
		bool keyUp = false;
		bool keyRt = false;
		bool keyLt = false;
		private PictureBox pictureBox1;
		private PictureBox pictureBox2;

		private ListBox listBox1;
		private Button button1;
		private TrackBar trackBar1;

		private Line baseline;
		private TrackBar trackBar2;

		List<PSpot> spotList = new List<PSpot>();

		private void Form1_Load(object sender, System.EventArgs e)
		{
			device = new D3DDevice(this, pictureBox1, pictureBox2, new SetObject(this.setObject));
			init();
		}

		int spotIdx = 0;

		private void _addSpot(double posx, double posy, double posz, List<PSpot> spotList, double charge, string texture, bool _static)
		{
			PSpot sp = new PSpot();
			sp.pos.X = (float)posx;
			sp.pos.Y = (float)posy;
			sp.pos.Z = (float)posz;
			sp.charge = (float)charge;
			sp.model = new Model(device, "ball.x", texture);
			device.addObject(sp.model);

			System.Drawing.Font f = new System.Drawing.Font("simsun", 12);
			sp.sname = spotIdx.ToString();
			sp.name = new Text(device, (spotIdx++).ToString(), f, 0, 0.1f);
			device.addObject(sp.name);

			listBox1.Items.Add(sp);
			spotList.Add(sp);
		}

		private void init()
		{
			baseline = new Line(device);
			baseline.addLine(0, 0, 0, 1, 0, 0, 0xFF0000);
			baseline.addLine(0, 0, 0, 0, 1, 0, 0x00FF00);
			baseline.addLine(0, 0, 0, 0, 0, 1, 0x0000FF);
			baseline.Finish();
			device.addObject(baseline);
		}

		bool slowdown = true;

		private void Form1_KeyDown(object sender, KeyEventArgs e)
		{
			D3DCamera cam = device.cam;
			if (e.KeyCode == Keys.Escape)
			{
				this.Dispose(true); return;
			}
			if (e.KeyCode == Keys.F1)
			{
				slowdown = !slowdown;
			}
			if (e.KeyCode == Keys.Right) keyRt = true;
			if (e.KeyCode == Keys.Left) keyLt = true;
			if (e.KeyCode == Keys.Up) keyUp = true;
			if (e.KeyCode == Keys.Down) keyDw = true;


			if (e.KeyCode == Keys.F)
			{
				add(cam, 7, "F.jpg");
			}
			if (e.KeyCode == Keys.O)
			{
				add(cam, 6, "O.jpg");
			}
			if (e.KeyCode == Keys.N)
			{
				add(cam, 5, "N.jpg");
			}
			if (e.KeyCode == Keys.C)
			{
				add(cam, 4, "C.jpg");
			}
			if (e.KeyCode == Keys.B)
			{
				add(cam, 3, "B.jpg");
			}
			if (e.KeyCode == Keys.H)
			{
				add(cam, 1, "H.jpg");
			}
			if (e.KeyCode == Keys.Space)
			{
				D3DObject obj = device.GetObject(mousex, mousey);
				foreach (PSpot p in spotList)
				{
					if (p.model == obj)
					{
						listBox1.SelectedItem = p;
						listBox1.Refresh();
					}
				}
			}
			e.Handled = true;
		}

		private void add(D3DCamera cam, int n, string filename)
		{
			double x = cam.x + (float)(10 * Math.Sin(cam.direction) * Math.Cos(cam.angle));
			double y = cam.y + (float)(10 * Math.Sin(cam.angle));
			double z = cam.z + (float)(10 * Math.Cos(cam.direction) * Math.Cos(cam.angle));

			_addSpot(x, y, z, spotList, n, filename, false);
			Random r = new Random();
			for (int i = 0; i < n; i++)
			{
				double x1 = r.NextDouble() * 2 - 1;
				double y1 = r.NextDouble() * 2 - 1;
				double z1 = r.NextDouble() * 2 - 1;
				_addSpot(x + x1, y + y1, z + z1, spotList, -1, "E.jpg", false);
			}
		}

		int mousex = 0;
		int mousey = 0;
		private void Form1_KeyUp(object sender, KeyEventArgs e)
		{
			if (e.KeyCode == Keys.Right) keyRt = false;
			if (e.KeyCode == Keys.Left) keyLt = false;
			if (e.KeyCode == Keys.Up) keyUp = false;
			if (e.KeyCode == Keys.Down) keyDw = false;
		}

		private void setObject()
		{
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

			PSpot sp = (PSpot)listBox1.SelectedItem;
			if (keyUp)
			{
				float x = (float)(10 * Math.Sin(cam.direction) * Math.Cos(cam.angle));
				float y = (float)(10 * Math.Sin(cam.angle));
				float z = (float)(10 * Math.Cos(cam.direction) * Math.Cos(cam.angle));
				sp.pos.X += x / 10;
				sp.pos.Y += y / 10;
				sp.pos.Z += z / 10;
			}
			if (keyDw)
			{
				float x = (float)(10 * Math.Sin(cam.direction) * Math.Cos(cam.angle));
				float y = (float)(10 * Math.Sin(cam.angle));
				float z = (float)(10 * Math.Cos(cam.direction) * Math.Cos(cam.angle));
				sp.pos.X -= x / 10;
				sp.pos.Y -= y / 10;
				sp.pos.Z -= z / 10;
			}


			PSpot.Process(spotList, slowdown, 0.01f * this.trackBar1.Value, 0.001f * this.trackBar2.Value);

			foreach (PSpot p in spotList)
			{
				p.model.reset_matrix();
				p.model.visible = true;
				float size = 0.1f;
				if (p.charge < 0)
				{
					size = 0.02f;
				}
				p.model.zoom(size, size, size);
				p.model.move(p.pos);

				p.name.reset_matrix();
				p.name.visible = false;
				p.name.move(0.2f, 0.2f, 0.2f);
				p.name.zoom(0.4f, 0.4f, 0.4f);
				p.name.move(p.pos);
			}
		}

		private void button1_Click(object sender, EventArgs e)
		{
			PSpot sp = (PSpot)listBox1.SelectedItem;
			if (sp == null)
			{
				sp = (PSpot)listBox1.Items[0];
			}
			if (sp == null)
			{
				return;
			}
			device.removeObject(sp.model);
			device.removeObject(sp.name);
			spotList.Remove(sp);
			listBox1.Items.Remove(sp);
		}

		void pictureBox1_MouseMove(object sender, MouseEventArgs e)
		{
			mousex = e.X;
			mousey = e.Y;
		}
	}
}
