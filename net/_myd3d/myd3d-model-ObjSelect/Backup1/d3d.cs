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
			this.pictureBox1 = new System.Windows.Forms.PictureBox();
			this.pictureBox2 = new System.Windows.Forms.PictureBox();
			this.pictureBox3 = new System.Windows.Forms.PictureBox();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
			this.SuspendLayout();
			// 
			// pictureBox1
			// 
			this.pictureBox1.Location = new System.Drawing.Point(12, 12);
			this.pictureBox1.Name = "pictureBox1";
			this.pictureBox1.Size = new System.Drawing.Size(144, 96);
			this.pictureBox1.TabIndex = 0;
			this.pictureBox1.TabStop = false;
			this.pictureBox1.MouseMove += new System.Windows.Forms.MouseEventHandler(this.pictureBox1_MouseMove);
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
			this.pictureBox3.Location = new System.Drawing.Point(12, 114);
			this.pictureBox3.Name = "pictureBox3";
			this.pictureBox3.Size = new System.Drawing.Size(144, 96);
			this.pictureBox3.TabIndex = 0;
			this.pictureBox3.TabStop = false;
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(6, 14);
			this.ClientSize = new System.Drawing.Size(318, 222);
			this.Controls.Add(this.pictureBox2);
			this.Controls.Add(this.pictureBox3);
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
			((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
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

		int tick = 1;
		int tickInc = 1;
		bool keyDw = false;
		bool keyUp = false;
		bool keyRt = false;
		bool keyLt = false;
		private PictureBox pictureBox1;
		private PictureBox pictureBox2;
		private PictureBox pictureBox3;

		bool start = false;

		private void Form1_Load(object sender, System.EventArgs e)
		{
			device = new D3DDevice(this, pictureBox1, pictureBox2, new SetObject(this.setObject));

			///models init----------

			init();

			///models init---------

		}

		private void init()
		{
			{
				Terrain t = new Terrain(device, "ground.bmp", "a.map");
				device.addObject(t);

				Text text = new Text(device, "fdsa", new System.Drawing.Font("simsun", 11), Color.Red, 1, 1);
				device.addObject(text);

				{
					Model m = new Model(device, "moon.x", "earth.bmp");
					m.zoom(0.1f, 0.1f, 0.1f);
					m.move(-1, 0, 0);
					device.addObject(m);
				}

				{
					Model m = new Model(device, "Dolphin1.x", "earth.bmp");
					m.zoom(0.01f, 0.01f, 0.01f);
					m.move(0, -1, 0);
					device.addObject(m);
				}

				{
					Model m = new Model(device, "Dolphin2.x", "earth.bmp");
					m.zoom(0.01f, 0.01f, 0.01f);
					m.move(0, -2, 0);
					device.addObject(m);
				}

				{
					Model m = new Model(device, "Dolphin3.x", "earth.bmp");
					m.zoom(0.01f, 0.01f, 0.01f);
					m.move(0, -3, 0);
					device.addObject(m);
				}

				Face f = new Face(device);
				f.addFace(0, 0, 0, 5, 5, 5, -5, 5, 5, 0x0000FF);
				f.Finish();
				device.addObject(f);


				Point p = new Point(device);
				p.addPoint(0, 6, 0, 0xFF0000);
				p.Finish();
				device.addObject(p);

				Line l = new Line(device);
				l.addLine(0, 0, 0, -5, -5, 5, 0x00FF00);
				l.Finish();
				device.addObject(l);


			}


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

		}
		private void Form1_KeyUp(object sender, KeyEventArgs e)
		{
			if (e.KeyCode == Keys.Right) keyRt = false;
			if (e.KeyCode == Keys.Left) keyLt = false;
			if (e.KeyCode == Keys.Up) keyUp = false;
			if (e.KeyCode == Keys.Down) keyDw = false;

			this.Text = device.cam.x.ToString() + " " + device.cam.y.ToString() + " " + device.cam.z.ToString();

			//this.Text = (new Bitmap(pictureBox1.Image)).GetPixel(10,10).G.ToString();

		}


		//private void setLight(Microsoft.DirectX.Direct3D.Lights lights)
		//private void setLight(Microsoft.DirectX.Direct3D.LightsCollection lights)
		//{
		//    lights[0].Type = LightType.Point;
		//    lights[0].Diffuse = Color.Red;
		//    lights[0].Ambient = Color.White;
		//    lights[0].Attenuation0 = 1f;
		//    lights[0].Attenuation1 = 1f;
		//    lights[0].Attenuation2 = 1f;
		//    lights[0].Range = 1;
		//    lights[0].XPosition = 0;
		//    lights[0].YPosition = 0;
		//    lights[0].ZPosition = 0;
		//    lights[0].Enabled = true;
		//}

		private void setObject()
		{
				D3DObject o = device.GetObject(mousex, mousey, pictureBox3);
				if (o != null)
				{
					this.Text = o.ToString();
				}
			if (keyUp)
			{
				tickInc++;
			}
			if (keyDw)
			{
				tickInc--;
			}
		}


		int mousex = 0;
		int mousey = 0;

		void pictureBox1_MouseMove(object sender, MouseEventArgs e)
		{
			mousex = e.X;
			mousey = e.Y;
		}

	}
}
