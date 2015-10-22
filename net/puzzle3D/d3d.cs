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
using ConsoleApplication1;

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
			this.textBox1 = new System.Windows.Forms.TextBox();
			this.button1 = new System.Windows.Forms.Button();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
			this.SuspendLayout();
			// 
			// pictureBox1
			// 
			this.pictureBox1.Location = new System.Drawing.Point(12, 12);
			this.pictureBox1.Name = "pictureBox1";
			this.pictureBox1.Size = new System.Drawing.Size(128, 96);
			this.pictureBox1.TabIndex = 0;
			this.pictureBox1.TabStop = false;
			this.pictureBox1.MouseMove += new System.Windows.Forms.MouseEventHandler(this.pictureBox1_MouseMove);
			this.pictureBox1.MouseUp += new System.Windows.Forms.MouseEventHandler(this.pictureBox1_MouseUp);
			// 
			// pictureBox2
			// 
			this.pictureBox2.Location = new System.Drawing.Point(156, 12);
			this.pictureBox2.Name = "pictureBox2";
			this.pictureBox2.Size = new System.Drawing.Size(128, 96);
			this.pictureBox2.TabIndex = 0;
			this.pictureBox2.TabStop = false;
			// 
			// pictureBox3
			// 
			this.pictureBox3.Location = new System.Drawing.Point(12, 126);
			this.pictureBox3.Name = "pictureBox3";
			this.pictureBox3.Size = new System.Drawing.Size(144, 96);
			this.pictureBox3.TabIndex = 0;
			this.pictureBox3.TabStop = false;
			// 
			// textBox1
			// 
			this.textBox1.Location = new System.Drawing.Point(12, 242);
			this.textBox1.Name = "textBox1";
			this.textBox1.Size = new System.Drawing.Size(294, 21);
			this.textBox1.TabIndex = 1;
			// 
			// button1
			// 
			this.button1.Location = new System.Drawing.Point(172, 189);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(75, 23);
			this.button1.TabIndex = 2;
			this.button1.Text = "button1";
			this.button1.UseVisualStyleBackColor = true;
			this.button1.Click += new System.EventHandler(this.button1_Click);
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(6, 14);
			this.ClientSize = new System.Drawing.Size(318, 289);
			this.Controls.Add(this.button1);
			this.Controls.Add(this.textBox1);
			this.Controls.Add(this.pictureBox2);
			this.Controls.Add(this.pictureBox3);
			this.Controls.Add(this.pictureBox1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.Name = "Form1";
			this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
			this.Text = "3D";
			this.Load += new System.EventHandler(this.Form1_Load);
			this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyDown);
			this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyUp);
			((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
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
		private PictureBox pictureBox3;
		private TextBox textBox1;
		private Button button1;

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
			CData2.init();
		}

		float[] cordinate = {
			0,0,0,

			-0.5f,-0.5f,-0.7071f,
			-0.5f,0.5f,-0.7071f,
			0.5f,-0.5f,-0.7071f,
			0.5f,0.5f,-0.7071f,

			-1f,-1f,-1.4142f,
			-1f,0f,-1.4142f,
			-1f,1f,-1.4142f,
			0f,-1f,-1.4142f,
			0f,0f,-1.4142f,
			0f,1f,-1.4142f,
			1f,-1f,-1.4142f,
			1f,0f,-1.4142f,
			1f,1f,-1.4142f,

			-1.5f,-1.5f,-2.1213f,
			-1.5f,-0.5f,-2.1213f,
			-1.5f,0.5f,-2.1213f,
			-1.5f,1.5f,-2.1213f,
			-0.5f,-1.5f,-2.1213f,
			-0.5f,-0.5f,-2.1213f,
			-0.5f,0.5f,-2.1213f,
			-0.5f,1.5f,-2.1213f,
			0.5f,-1.5f,-2.1213f,
			0.5f,-0.5f,-2.1213f,
			0.5f,0.5f,-2.1213f,
			0.5f,1.5f,-2.1213f,
			1.5f,-1.5f,-2.1213f,
			1.5f,-0.5f,-2.1213f,
			1.5f,0.5f,-2.1213f,
			1.5f,1.5f,-2.1213f,

			-2f,-2f,-2.8284f,
			-2f,-1f,-2.8284f,
			-2f,0f,-2.8284f,
			-2f,1f,-2.8284f,
			-2f,2f,-2.8284f,
			-1f,-2f,-2.8284f,
			-1f,-1f,-2.8284f,
			-1f,0f,-2.8284f,
			-1f,1f,-2.8284f,
			-1f,2f,-2.8284f,
			0f,-2f,-2.8284f,
			0f,-1f,-2.8284f,
			0f,0f,-2.8284f,
			0f,1f,-2.8284f,
			0f,2f,-2.8284f,
			1f,-2f,-2.8284f,
			1f,-1f,-2.8284f,
			1f,0f,-2.8284f,
			1f,1f,-2.8284f,
			1f,2f,-2.8284f,
			2f,-2f,-2.8284f,
			2f,-1f,-2.8284f,
			2f,0f,-2.8284f,
			2f,1f,-2.8284f,
			2f,2f,-2.8284f,

		};

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

		class Group
		{
			public List<Model> models;
			public ulong mask;
			public int idx;
			public bool lift = false;
		}
		List<Group> groups = new List<Group>();

		private void button1_Click(object sender, EventArgs e)
		{
			string s = textBox1.Text;
			int[] Idx = new int[12];
			for (int i = 0; i < 12; i++)
			{
				Idx[i] = int.Parse(s.Split(',')[i]);
			}

			ulong board = 0;
			int last = 11;
			for (int i = 0; i < 12; i++)
			{
				ulong mask = CData2.Data[i * 512 + Idx[i]];
				if ((board & mask) == 0)
				{
					board |= mask;
				}
				else
				{
					last = i;
					break;
				}
			}

			for (int i = 0; i <= last; i++)
			{
				Group group = new Group();
				group.mask = CData2.Data[i * 512 + Idx[i]];
				group.idx = i;
				group.models = new List<Model>();
				for (int i55 = 0; i55 < 55; i55++)
				{
					if ((group.mask & ((ulong)1 << i55)) != 0)
					{
						Model m = new Model(device, "moon.x", group.idx + ".bmp");
						group.models.Add(m);
						m.zoom(0.1f, 0.1f, 0.1f);
						m.move(cordinate[i55 * 3], cordinate[i55 * 3 + 2] + (group.lift ? 6 : 0), cordinate[i55 * 3 + 1]);
						device.addObject(m);
					}
				}
				groups.Add(group);
			}

			putBalls();
		}

		private void putBalls()
		{
			foreach (var group in groups)
			{
				int idx = 0;
				for (int i = 0; i < 55; i++)
				{
					if ((group.mask & ((ulong)1 << i)) != 0)
					{
						Model m = group.models[idx++];
						m.reset_matrix();
						m.zoom(0.1f, 0.1f, 0.1f);
						m.move(cordinate[i * 3], cordinate[i * 3 + 2] + (group.lift ? 6 : 0), cordinate[i * 3 + 1]);
					}
				}
			}
		}


		void pictureBox1_MouseUp(object sender, MouseEventArgs e)
		{
			if (e.Button == System.Windows.Forms.MouseButtons.Left)
			{
				D3DObject o = device.GetObject(mousex, mousey, pictureBox3);
				foreach (var item in groups)
				{
					foreach (var m in item.models)
					{
						if (o == m)
						{
							item.lift = !item.lift;
						}
					}
				}
				putBalls();
			}
		}
	}
}
