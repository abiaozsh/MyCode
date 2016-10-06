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
			this.timer1 = new System.Windows.Forms.Timer(this.components);
			this.pictureBox1 = new System.Windows.Forms.PictureBox();
			this.pictureBox2 = new System.Windows.Forms.PictureBox();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
			this.SuspendLayout();
			// 
			// timer1
			// 
			this.timer1.Interval = 50;
			this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
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
			this.MouseWheel += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseWheel);
			this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseUp);
			this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyUp);
			this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseMove);
			this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyDown);
			this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseDown);
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
		private System.Windows.Forms.Timer timer1;


		////////////////////////////////////////////////////////////////////
		D3DDevice device;

		D3DCamera cam;

		int tick = 1;
		int tickInc = 1;
		float speed = 2f;
		bool keyDw = false;
		bool keyUp = false;
		bool keyRt = false;
		bool keyLt = false;
		bool keyW = false;
		bool keyS = false;
		bool keyA = false;
		bool keyD = false;
		bool keyE = false;
		bool keyC = false;
		private PictureBox pictureBox1;
		private PictureBox pictureBox2;

		bool start = false;

		private void Form1_Load(object sender, System.EventArgs e)
		{
			this.CenterToParent();

			device = new D3DDevice(pictureBox1, pictureBox2);
			cam = new D3DCamera();
			device.camCenter = cam;

			///models init----------

			init();

			///models init---------

			timer1.Enabled = true;
		}


		private Vector3 getMidPoint(Vector3 v2, Vector3 v1)
		{
			//µ¥±ß³¤
			double d5 = 1 - 1 / (Math.Cos(Math.PI / 5) * Math.Cos(Math.PI / 5) * 4);
			float x = (float)(v1.X - (v1.X - v2.X) * d5);
			float y = (float)(v1.Y - (v1.Y - v2.Y) * d5);
			float z = (float)(v1.Z - (v1.Z - v2.Z) * d5);
			return new Vector3(x, y, z);

		}

		private void init()
		{
			//points[00] = new Vector3(0, 1, fi);
			//points[01] = new Vector3(fi, 0, 1);
			//points[02] = new Vector3(fi, 0, -1);
			//points[03] = new Vector3(1, fi, 0);
			//points[04] = new Vector3(0, 1, -fi);
			//points[05] = new Vector3(-1, fi, 0);
			//points[06] = new Vector3(-fi, 0, -1);
			//points[07] = new Vector3(-fi, 0, 1);
			//points[08] = new Vector3(-1, -fi, 0);
			//points[09] = new Vector3(0, -1, fi);
			//points[10] = new Vector3(1, -fi, 0);
			//points[11] = new Vector3(0, -1, -fi);

			double fi = (1 + Math.Sqrt(5)) / 2;
			Vector3 point0 = new Vector3(0, 0, 0);
			Vector3 pointx = new Vector3(10, 0, 0);
			Vector3 pointy = new Vector3(0, 10, 0);
			Vector3 pointz = new Vector3(0, 0, 10);
			Vector3[] points = new Vector3[12];
			Vector3[] pointMs = new Vector3[10];
			Vector3[] pointNs = new Vector3[10];
			{

				#region 0
				double x = (float)Math.Sqrt(1 + fi * fi);
				double a = Math.Sin(2 * Math.Asin(1 / x)) * x;
				double b = Math.Cos(2 * Math.Asin(1 / x)) * x;

				double alpha = 2 * Math.Asin(1 / x);

				points[00] = new Vector3(0, (float)x, 0);
				points[01] = new Vector3((float)(Math.Sin(Math.PI / 5 * 2 * 0) * a), (float)b, (float)(Math.Cos(Math.PI / 5 * 2 * 0) * a));
				points[03] = new Vector3((float)(Math.Sin(Math.PI / 5 * 2 * 1) * a), (float)b, (float)(Math.Cos(Math.PI / 5 * 2 * 1) * a));
				points[05] = new Vector3((float)(Math.Sin(Math.PI / 5 * 2 * 2) * a), (float)b, (float)(Math.Cos(Math.PI / 5 * 2 * 2) * a));
				points[07] = new Vector3((float)(Math.Sin(Math.PI / 5 * 2 * 3) * a), (float)b, (float)(Math.Cos(Math.PI / 5 * 2 * 3) * a));
				points[09] = new Vector3((float)(Math.Sin(Math.PI / 5 * 2 * 4) * a), (float)b, (float)(Math.Cos(Math.PI / 5 * 2 * 4) * a));
				points[02] = new Vector3((float)(Math.Sin(Math.PI / 5 * 2 * 0 + Math.PI / 5) * a), (float)-b, (float)(Math.Cos(Math.PI / 5 * 2 * 0 + Math.PI / 5) * a));
				points[04] = new Vector3((float)(Math.Sin(Math.PI / 5 * 2 * 1 + Math.PI / 5) * a), (float)-b, (float)(Math.Cos(Math.PI / 5 * 2 * 1 + Math.PI / 5) * a));
				points[06] = new Vector3((float)(Math.Sin(Math.PI / 5 * 2 * 2 + Math.PI / 5) * a), (float)-b, (float)(Math.Cos(Math.PI / 5 * 2 * 2 + Math.PI / 5) * a));
				points[08] = new Vector3((float)(Math.Sin(Math.PI / 5 * 2 * 3 + Math.PI / 5) * a), (float)-b, (float)(Math.Cos(Math.PI / 5 * 2 * 3 + Math.PI / 5) * a));
				points[10] = new Vector3((float)(Math.Sin(Math.PI / 5 * 2 * 4 + Math.PI / 5) * a), (float)-b, (float)(Math.Cos(Math.PI / 5 * 2 * 4 + Math.PI / 5) * a));
				points[11] = new Vector3(0, (float)-x, 0);


				#endregion

				#region 0
				pointMs[0] = getMidPoint(points[11], points[01]);
				pointMs[1] = getMidPoint(points[00], points[02]);
				pointMs[2] = getMidPoint(points[11], points[03]);
				pointMs[3] = getMidPoint(points[00], points[04]);
				pointMs[4] = getMidPoint(points[11], points[05]);
				pointMs[5] = getMidPoint(points[00], points[06]);
				pointMs[6] = getMidPoint(points[11], points[07]);
				pointMs[7] = getMidPoint(points[00], points[08]);
				pointMs[8] = getMidPoint(points[11], points[09]);
				pointMs[9] = getMidPoint(points[00], points[10]);

				pointNs[0] = getMidPoint(points[01], points[11]);
				pointNs[1] = getMidPoint(points[02], points[00]);
				pointNs[2] = getMidPoint(points[03], points[11]);
				pointNs[3] = getMidPoint(points[04], points[00]);
				pointNs[4] = getMidPoint(points[05], points[11]);
				pointNs[5] = getMidPoint(points[06], points[00]);
				pointNs[6] = getMidPoint(points[07], points[11]);
				pointNs[7] = getMidPoint(points[08], points[00]);
				pointNs[8] = getMidPoint(points[09], points[11]);
				pointNs[9] = getMidPoint(points[10], points[00]);
				#endregion

			}

			{
				//Line lineC = new Line( device.device);
				Line line = new Line( device.device);
				Face face = new Face( device.device);

				#region 4
				line.addLine(points[00], points[01], 0x000000);
				line.addLine(points[00], points[03], 0x000000);
				line.addLine(points[00], points[05], 0x000000);
				line.addLine(points[00], points[07], 0x000000);
				line.addLine(points[00], points[09], 0x000000);
				line.addLine(points[11], points[02], 0x000000);
				line.addLine(points[11], points[04], 0x000000);
				line.addLine(points[11], points[06], 0x000000);
				line.addLine(points[11], points[08], 0x000000);
				line.addLine(points[11], points[10], 0x000000);
				line.addLine(points[09], points[01], 0x000000);
				line.addLine(points[01], points[03], 0x000000);
				line.addLine(points[03], points[05], 0x000000);
				line.addLine(points[05], points[07], 0x000000);
				line.addLine(points[07], points[09], 0x000000);
				line.addLine(points[10], points[02], 0x000000);
				line.addLine(points[02], points[04], 0x000000);
				line.addLine(points[04], points[06], 0x000000);
				line.addLine(points[06], points[08], 0x000000);
				line.addLine(points[08], points[10], 0x000000);
				line.addLine(points[01], points[02], 0x000000);
				line.addLine(points[02], points[03], 0x000000);
				line.addLine(points[03], points[04], 0x000000);
				line.addLine(points[04], points[05], 0x000000);
				line.addLine(points[05], points[06], 0x000000);
				line.addLine(points[06], points[07], 0x000000);
				line.addLine(points[07], points[08], 0x000000);
				line.addLine(points[08], points[09], 0x000000);
				line.addLine(points[09], points[10], 0x000000);
				line.addLine(points[10], points[01], 0x000000);
				#endregion

				#region 1
				line.addLine(points[00], pointMs[1], 0x000000);
				line.addLine(points[01], pointMs[1], 0x000000);
				line.addLine(points[03], pointMs[1], 0x000000);
				line.addLine(points[00], pointMs[3], 0x000000);
				line.addLine(points[03], pointMs[3], 0x000000);
				line.addLine(points[05], pointMs[3], 0x000000);
				line.addLine(points[00], pointMs[5], 0x000000);
				line.addLine(points[05], pointMs[5], 0x000000);
				line.addLine(points[07], pointMs[5], 0x000000);
				line.addLine(points[00], pointMs[7], 0x000000);
				line.addLine(points[07], pointMs[7], 0x000000);
				line.addLine(points[09], pointMs[7], 0x000000);
				line.addLine(points[00], pointMs[9], 0x000000);
				line.addLine(points[09], pointMs[9], 0x000000);
				line.addLine(points[01], pointMs[9], 0x000000);
				line.addLine(points[11], pointMs[0], 0x000000);
				line.addLine(points[02], pointMs[0], 0x000000);
				line.addLine(points[10], pointMs[0], 0x000000);
				line.addLine(points[11], pointMs[2], 0x000000);
				line.addLine(points[04], pointMs[2], 0x000000);
				line.addLine(points[02], pointMs[2], 0x000000);
				line.addLine(points[11], pointMs[4], 0x000000);
				line.addLine(points[06], pointMs[4], 0x000000);
				line.addLine(points[04], pointMs[4], 0x000000);
				line.addLine(points[11], pointMs[6], 0x000000);
				line.addLine(points[08], pointMs[6], 0x000000);
				line.addLine(points[06], pointMs[6], 0x000000);
				line.addLine(points[11], pointMs[8], 0x000000);
				line.addLine(points[10], pointMs[8], 0x000000);
				line.addLine(points[08], pointMs[8], 0x000000);
				#endregion

				#region 3
				line.addLine(points[01], pointNs[1], 0x000000);
				line.addLine(points[02], pointNs[1], 0x000000);
				line.addLine(points[03], pointNs[1], 0x000000);
				line.addLine(points[02], pointNs[2], 0x000000);
				line.addLine(points[03], pointNs[2], 0x000000);
				line.addLine(points[04], pointNs[2], 0x000000);
				line.addLine(points[03], pointNs[3], 0x000000);
				line.addLine(points[04], pointNs[3], 0x000000);
				line.addLine(points[05], pointNs[3], 0x000000);
				line.addLine(points[04], pointNs[4], 0x000000);
				line.addLine(points[05], pointNs[4], 0x000000);
				line.addLine(points[06], pointNs[4], 0x000000);
				line.addLine(points[05], pointNs[5], 0x000000);
				line.addLine(points[06], pointNs[5], 0x000000);
				line.addLine(points[07], pointNs[5], 0x000000);
				line.addLine(points[06], pointNs[6], 0x000000);
				line.addLine(points[07], pointNs[6], 0x000000);
				line.addLine(points[08], pointNs[6], 0x000000);
				line.addLine(points[07], pointNs[7], 0x000000);
				line.addLine(points[08], pointNs[7], 0x000000);
				line.addLine(points[09], pointNs[7], 0x000000);
				line.addLine(points[08], pointNs[8], 0x000000);
				line.addLine(points[09], pointNs[8], 0x000000);
				line.addLine(points[10], pointNs[8], 0x000000);
				line.addLine(points[09], pointNs[9], 0x000000);
				line.addLine(points[10], pointNs[9], 0x000000);
				line.addLine(points[01], pointNs[9], 0x000000);
				line.addLine(points[10], pointNs[0], 0x000000);
				line.addLine(points[01], pointNs[0], 0x000000);
				line.addLine(points[02], pointNs[0], 0x000000);
				#endregion

				//lineC.addLine(point0, pointx, 0xFF0000);
				//lineC.addLine(point0, pointy, 0x00FF00);
				//lineC.addLine(point0, pointz, 0x0000FF);

				//lineC.addLine(point0, pointNs[9], 0xFFFFFF);

				#region 5
				face.addFace(points[10], pointMs[0], points[02], 0xFF8080);
				face.addFace(points[02], pointMs[0], points[11], 0x80FF80);
				face.addFace(points[11], pointMs[0], points[10], 0x8080FF);
				face.addFace(points[01], pointMs[1], points[00], 0xFF8080);
				face.addFace(points[00], pointMs[1], points[03], 0x80FF80);
				face.addFace(points[03], pointMs[1], points[01], 0x8080FF);
				face.addFace(points[02], pointMs[2], points[04], 0xFF8080);
				face.addFace(points[04], pointMs[2], points[11], 0x80FF80);
				face.addFace(points[11], pointMs[2], points[02], 0x8080FF);
				face.addFace(points[03], pointMs[3], points[00], 0xFF8080);
				face.addFace(points[00], pointMs[3], points[05], 0x80FF80);
				face.addFace(points[05], pointMs[3], points[03], 0x8080FF);
				face.addFace(points[04], pointMs[4], points[06], 0xFF8080);
				face.addFace(points[06], pointMs[4], points[11], 0x80FF80);
				face.addFace(points[11], pointMs[4], points[04], 0x8080FF);
				face.addFace(points[05], pointMs[5], points[00], 0xFF8080);
				face.addFace(points[00], pointMs[5], points[07], 0x80FF80);
				face.addFace(points[07], pointMs[5], points[05], 0x8080FF);
				face.addFace(points[06], pointMs[6], points[08], 0xFF8080);
				face.addFace(points[08], pointMs[6], points[11], 0x80FF80);
				face.addFace(points[11], pointMs[6], points[06], 0x8080FF);
				face.addFace(points[07], pointMs[7], points[00], 0xFF8080);
				face.addFace(points[00], pointMs[7], points[09], 0x80FF80);
				face.addFace(points[09], pointMs[7], points[07], 0x8080FF);
				face.addFace(points[08], pointMs[8], points[10], 0xFF8080);
				face.addFace(points[10], pointMs[8], points[11], 0x80FF80);
				face.addFace(points[11], pointMs[8], points[08], 0x8080FF);
				face.addFace(points[09], pointMs[9], points[00], 0xFF8080);
				face.addFace(points[00], pointMs[9], points[01], 0x80FF80);
				face.addFace(points[01], pointMs[9], points[09], 0x8080FF);
				#endregion

				#region 6
				face.addFace(points[10], pointNs[0], points[01], 0xFF8080);
				face.addFace(points[01], pointNs[0], points[02], 0x80FF80);
				face.addFace(points[02], pointNs[0], points[10], 0x8080FF);
				face.addFace(points[01], pointNs[1], points[03], 0xFF8080);
				face.addFace(points[02], pointNs[1], points[01], 0x80FF80);
				face.addFace(points[03], pointNs[1], points[02], 0x8080FF);
				face.addFace(points[02], pointNs[2], points[03], 0xFF8080);
				face.addFace(points[03], pointNs[2], points[04], 0x80FF80);
				face.addFace(points[04], pointNs[2], points[02], 0x8080FF);
				face.addFace(points[03], pointNs[3], points[05], 0xFF8080);
				face.addFace(points[04], pointNs[3], points[03], 0x80FF80);
				face.addFace(points[05], pointNs[3], points[04], 0x8080FF);
				face.addFace(points[04], pointNs[4], points[05], 0xFF8080);
				face.addFace(points[05], pointNs[4], points[06], 0x80FF80);
				face.addFace(points[06], pointNs[4], points[04], 0x8080FF);
				face.addFace(points[05], pointNs[5], points[07], 0xFF8080);
				face.addFace(points[06], pointNs[5], points[05], 0x80FF80);
				face.addFace(points[07], pointNs[5], points[06], 0x8080FF);
				face.addFace(points[06], pointNs[6], points[07], 0xFF8080);
				face.addFace(points[07], pointNs[6], points[08], 0x80FF80);
				face.addFace(points[08], pointNs[6], points[06], 0x8080FF);
				face.addFace(points[07], pointNs[7], points[09], 0xFF8080);
				face.addFace(points[08], pointNs[7], points[07], 0x80FF80);
				face.addFace(points[09], pointNs[7], points[08], 0x8080FF);
				face.addFace(points[08], pointNs[8], points[09], 0xFF8080);
				face.addFace(points[09], pointNs[8], points[10], 0x80FF80);
				face.addFace(points[10], pointNs[8], points[08], 0x8080FF);
				face.addFace(points[09], pointNs[9], points[01], 0xFF8080);
				face.addFace(points[10], pointNs[9], points[09], 0x80FF80);
				face.addFace(points[01], pointNs[9], points[10], 0x8080FF);
				#endregion

				//lineC.Finish();
				line.Finish();
				face.Finish();

				//line.RotationX((float)-Math.PI / 4);
				device.addObject(face);
				device.addObject(line);
				//lineC.zoom(10, 10, 10);
				//device.addObject(lineC);
			}

			Model model;

			//model = new Model(device.device, "moon.x", "moon0.bmp");
			//model.zoom(0.02f, 0.02f, 0.02f);
			//model.move(0, 0, 0);
			//device.addObject(model);

			//for (int i = 0; i < 12; i++)
			//{
			//    model = new Model(device.device, "moon.x", "moon" + i + ".bmp");
			//    model.zoom(0.02f, 0.02f, 0.02f);
			//    model.move(points[i].X, points[i].Y, points[i].Z);
			//    //model.RotationX((float)-Math.PI / 4);
			//    device.addObject(model);
			//}
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
			if (e.KeyCode == Keys.W) keyW = true;
			if (e.KeyCode == Keys.S) keyS = true;
			if (e.KeyCode == Keys.A) keyA = true;
			if (e.KeyCode == Keys.D) keyD = true;
			if (e.KeyCode == Keys.E) keyE = true;
			if (e.KeyCode == Keys.C) keyC = true;
		}
		private void Form1_KeyUp(object sender, KeyEventArgs e)
		{
			if (e.KeyCode == Keys.Right) keyRt = false;
			if (e.KeyCode == Keys.Left) keyLt = false;
			if (e.KeyCode == Keys.Up) keyUp = false;
			if (e.KeyCode == Keys.Down) keyDw = false;
			if (e.KeyCode == Keys.W) keyW = false;
			if (e.KeyCode == Keys.S) keyS = false;
			if (e.KeyCode == Keys.A) keyA = false;
			if (e.KeyCode == Keys.D) keyD = false;
			if (e.KeyCode == Keys.E) keyE = false;
			if (e.KeyCode == Keys.C) keyC = false;

			//this.Text = cam.x.ToString() + " " + cam.y.ToString() + " " + cam.z.ToString();

		}
		private void Form1_MouseMove(object sender, MouseEventArgs e)
		{
			if (!mouseRelease)
			{
				float dx = Cursor.Position.X - mousex;
				float dy = Cursor.Position.Y - mousey;
				if (dx != 0 || dy != 0)
				{
					float mX = cam.direction;
					float mY = -cam.angle;

					mX += dx / cam.zoom_level / 500;
					if (mX <= 0) mX = 3.14f * 2;
					if (mX > 3.141 * 2) mX = 0;

					mY += dy / cam.zoom_level / 500;
					if (mY < -1.57) mY = -1.57f;
					if (mY > 1.57) mY = 1.57f;

					cam.direction = mX;
					cam.angle = -mY;
					System.Windows.Forms.Cursor.Position = new Point(mousex, mousey);
				}
			}
		}
		private void Form1_MouseWheel(object sender, MouseEventArgs e)
		{
			if (e.Delta > 0)
			{
				cam.zoom_level *= 2;
			}
			else
			{
				if (cam.zoom_level > 1) cam.zoom_level /= 2;
			}
		}
		private void Form1_MouseDown(object sender, MouseEventArgs e)
		{
			mousex = Cursor.Position.X;
			mousey = Cursor.Position.Y;
			mouseRelease = false;
			System.Windows.Forms.Cursor.Hide();
		}
		private void Form1_MouseUp(object sender, MouseEventArgs e)
		{
			mouseRelease = true;
			System.Windows.Forms.Cursor.Show();
		}

		private void timer1_Tick(object sender, System.EventArgs e)
		{
			{
				if (keyW)
				{
					cam.x = (float)(cam.x + speed * Math.Sin(cam.direction) * Math.Cos(cam.angle));
					cam.y = (float)(cam.y + speed * Math.Sin(cam.angle));
					cam.z = (float)(cam.z + speed * Math.Cos(cam.direction) * Math.Cos(cam.angle));
				}
				if (keyS)
				{
					cam.x = (float)(cam.x - speed * Math.Sin(cam.direction) * Math.Cos(cam.angle));
					cam.y = (float)(cam.y - speed * Math.Sin(cam.angle));
					cam.z = (float)(cam.z - speed * Math.Cos(cam.direction) * Math.Cos(cam.angle));
				}
				if (keyE)
				{
					cam.y = cam.y + speed;
				}
				if (keyC)
				{
					cam.y = cam.y - speed;
				}
			}
			if (keyA)
			{
				cam.x = (float)(cam.x + speed * Math.Sin(cam.direction - Math.PI / 2) / 2);
				cam.z = (float)(cam.z + speed * Math.Cos(cam.direction - Math.PI / 2) / 2);
			}
			if (keyD)
			{
				cam.x = (float)(cam.x + speed * Math.Sin(cam.direction + Math.PI / 2) / 2);
				cam.z = (float)(cam.z + speed * Math.Cos(cam.direction + Math.PI / 2) / 2);
			}


			if (keyUp)
			{
				tickInc++;
			}
			if (keyDw)
			{
				tickInc--;
			}

			this.setLight(device.device.Lights);
			this.setObject(tick);
			device.Render();
		}

		//private void setLight(Microsoft.DirectX.Direct3D.Lights lights)
		private void setLight(Microsoft.DirectX.Direct3D.LightsCollection lights)
		{
			//lights[0].Type = LightType.Point;
			//lights[0].Diffuse = Color.Red;
			//lights[0].Ambient = Color.White;
			//lights[0].Attenuation0 = 1f;
			//lights[0].Attenuation1 = 1f;
			//lights[0].Attenuation2 = 1f;
			//lights[0].Range = 1;
			//lights[0].XPosition = 0;
			//lights[0].YPosition = 0;
			//lights[0].ZPosition = 0;
			//lights[0].Enabled = true;
		}

		private void setObject(int idx)
		{

		}
	}
}
