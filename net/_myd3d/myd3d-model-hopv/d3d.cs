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

		int tick = 1;
		int tickInc = 1;
		bool keyDw = false;
		bool keyUp = false;
		bool keyRt = false;
		bool keyLt = false;
		private PictureBox pictureBox1;
		private PictureBox pictureBox2;

		bool start = false;

		private void Form1_Load(object sender, System.EventArgs e)
		{
			try
			{
				device = new D3DDevice(this, pictureBox1, pictureBox2, new SetObject(this.setObject));
			}
			catch (Exception ex)
			{
				string s = ex.ToString();
			}
			///models init----------

			init();

			///models init---------

		}


		double wave_1s(int n, int Z, double r, double x, double y, double z)
		{
			double rho = 2.0 * (double)Z * r / (double)n;
			return 2.0 * Math.Sqrt(Z * Z * Z) * Math.Exp(-rho / 2.0) * Math.Sqrt(1 / (4.0 * Math.PI));
		}

		double wave_2s(int n, int Z, double r, double x, double y, double z)
		{
			double rho = 2.0 * (double)Z * r / (double)n;
			return ((1.0 / (2.0 * Math.Sqrt(2))) * (2.0 - rho) * Math.Sqrt(Z * Z * Z) * Math.Exp(-rho / 2.0) *  //R
					Math.Sqrt(1.0 / (4.0 * Math.PI))); //Y
		}

		double wave_2p_x(int n, int Z, double r, double x, double y, double z)
		{
			double rho = 2.0 * (double)Z * r / (double)n;
			return (1.0 / (2.0 * Math.Sqrt(6.0))) * rho * Math.Sqrt(Z * Z * Z) * Math.Exp(-rho / 2.0) * //R
					Math.Sqrt(3.0) * x / r * Math.Sqrt(1.0 / (4.0 * Math.PI)); //Y
		}
		double wave_2p_y(int n, int Z, double r, double x, double y, double z)
		{
			double rho = 2.0 * (double)Z * r / (double)n;
			return (1.0 / (2.0 * Math.Sqrt(6.0))) * rho * Math.Sqrt(Z * Z * Z) * Math.Exp(-rho / 2.0) * //R
					Math.Sqrt(3.0) * y / r * Math.Sqrt(1.0 / (4.0 * Math.PI)); //Y
		}
		double wave_2p_z(int n, int Z, double r, double x, double y, double z)
		{
			double rho = 2.0 * (double)Z * r / (double)n;
			return (1.0 / (2.0 * Math.Sqrt(6.0))) * rho * Math.Sqrt(Z * Z * Z) * Math.Exp(-rho / 2.0) * //R
					Math.Sqrt(3.0) * z / r * Math.Sqrt(1.0 / (4.0 * Math.PI)); //Y
		}

		double wave_3dR(int Z, double rho)
		{
			return 1 / (9 * Math.Sqrt(30)) * rho * rho * Math.Sqrt(Z * Z * Z) * Math.Exp(-rho / 2);
		}

		double wave_3d_xy(int n, int Z, double r, double x, double y, double z)
		{
			double rho = 2.0 * (double)Z * r / (double)n;
			return wave_3dR(Z, rho) * Math.Sqrt(60 / 4) * x * y / (r * r) * Math.Sqrt(1 / (4 * Math.PI));
		}


		private void init()
		{
			Random r = new Random();

			Point point = new Point(device.device);
			for (int i = 0; i < 10000000; i++)
			{
				double x = (r.NextDouble() - 0.5) * 100;
				double y = (r.NextDouble() - 0.5) * 100;
				double z = (r.NextDouble() - 0.5) * 100;

				{
					double rho = Math.Sqrt(x * x + y * y + z * z);
					int opt_n = 2;
					double val = wave_2s(opt_n, 1, rho, x, y, z);// + wave_2p_x(opt_n, 1, rho, x, y, z);// 
					//double val = wave_3d_xy(opt_n, 1, rho, x, y, z) + wave_2p_z(opt_n, 1, rho, x, y, z);


					//if (val > 0.01)
					//{
					//    point.addPoint(x, y, z, 0x0000FF);
					//}
					//if (val < -0.01)
					//{
					//    point.addPoint(x, y, z, 0xFF0000);
					//}

					if (val > 0)
					{
						if ((1 - val) < r.NextDouble())
						{
							point.addPoint(x, y, z, 0x0000FF);
						}
					}
					else
					{
						if ((-1 - val) > -r.NextDouble())
						{
							point.addPoint(x, y, z, 0xFF0000);
						}
					}

				}

			}
			point.Finish();
			device.addObject(point);



			//Text text = new Text(device.device, "fdsa",);


			//device.addObject(text);


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

		}


		//private void setLight(Microsoft.DirectX.Direct3D.Lights lights)
		//{
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
		//}

		private void setObject()
		{
			if (keyUp)
			{
				tickInc++;
			}
			if (keyDw)
			{
				tickInc--;
			}

		}

		private void pictureBox1_Click(object sender, EventArgs e)
		{

		}
	}
}
