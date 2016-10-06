using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;

namespace myd3d
{
	public class Form1 : System.Windows.Forms.Form
	{
		private System.ComponentModel.IContainer components;
	
		public Form1()
		{
			InitializeComponent();
		}
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			this.timer1 = new System.Windows.Forms.Timer(this.components);
			// 
			// timer1
			// 
			this.timer1.Interval = 50;
			this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 12);
			this.ClientSize = new System.Drawing.Size(242, 233);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
			this.Name = "Form1";
			this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
			this.Text = "3D";
			this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyDown);
			this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseDown);
			this.Load += new System.EventHandler(this.Form1_Load);
			this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyUp);
			this.MouseWheel += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseWheel);
			this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseMove);

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

		Model moon;
		Model earth;
		Model room;
		Model bullet;
		Model box;
		Terrain floor;

		D3DCamera cam;

		float mouseX=0;
		float mouseY=0;
		float tick=0;
		float step=1;
		float speed=0.2f;
		bool keyW=false;
		bool keyS=false;
		bool keyA=false;
		bool keyD=false;
		bool keyE=false;
		bool keyC=false;

		int bullet_life;
		float bullet_x;
		float bullet_y;
		float bullet_z;
		float bullet_direction;
		float bullet_angle;
		float bullet_speed;
		float bullet_gravity_speedup;

		int s=-1;
		bool fly=false;

		private void Form1_Load(object sender, System.EventArgs e)
		{
			this.ClientSize = new System.Drawing.Size(800,600);
			this.CenterToParent();

			device = new D3DDevice(this,this.ClientSize.Width,this.ClientSize.Height);
			cam = new D3DCamera();
			device.cam = cam;

			///models init-----------------------------------------------------------------

			moon = new Model(device.device,"moon.x");
			device.addObject(moon);

			earth= new Model(device.device,"earth.x");
			device.addObject(earth);

			room= new Model(device.device,"room.x");
			device.addObject(room);

			box= new Model(device.device,"BISMARK_.x");
			device.addObject(box);

			bullet= new Model(device.device,"bullet.x");

			floor = new Terrain(device.device,"ground.bmp","a.map");
			device.addObject(floor);
			///models init-----------------------------------------------------------------
			
			timer1.Enabled =true;
			System.Windows.Forms.Cursor.Hide();
		}

		private void Form1_MouseDown(object sender, MouseEventArgs e)
		{
			bullet_life=100;
			bullet_speed=1f;
			bullet_gravity_speedup=0;
			if(s==1)
			{
				bullet_x=(float)(cam.x+0.3*Math.Sin(cam.direction-Math.PI/2)/2);
				bullet_z=(float)(cam.z+0.3*Math.Cos(cam.direction-Math.PI/2)/2);
			}
			else
			{
				bullet_x=(float)(cam.x-0.3*Math.Sin(cam.direction-Math.PI/2)/2);
				bullet_z=(float)(cam.z-0.3*Math.Cos(cam.direction-Math.PI/2)/2);
			}
			s*=-1;
			bullet_y=cam.y;
			bullet_direction=cam.direction;
			bullet_angle=cam.angle;
			if(!device.containsObject(bullet))
			{
				device.addObject(bullet);
			}
		}
		private void Form1_MouseMove(object sender, MouseEventArgs e)
		{
			int baseX = this.Left;
			int baseY = this.Top;

			float wid = this.ClientSize.Width;
			float hei = this.ClientSize.Height;
			float dx;
			float dy;
			dx=e.X-wid/2;
			dy=e.Y-hei/2;
			if(dx!=0||dy!=0)
			{
				mouseX+=dx/cam.zoom_level/500;
				if(mouseX<=0)mouseX=3.14f*2;
				if(mouseX>3.141*2)mouseX=0;

				mouseY+=dy/cam.zoom_level/500;
				if(mouseY<-1.57)mouseY=-1.57f;
				if(mouseY>1.57)mouseY=1.57f;

				cam.direction=mouseX;
				cam.angle=-mouseY;
				System.Windows.Forms.Cursor.Position = new Point(baseX+(int)(wid/2),baseY+(int)(hei/2));
			}
		}

		private void Form1_KeyDown(object sender, KeyEventArgs e)
		{
			if(e.KeyCode==Keys.Escape)
			{
				this.Dispose(true);return;
			}
			if(e.KeyCode==Keys.Space)step=0;
			if(e.KeyCode==Keys.W)keyW=true;
			if(e.KeyCode==Keys.S)keyS=true;
			if(e.KeyCode==Keys.A)keyA=true;
			if(e.KeyCode==Keys.D)keyD=true;
			if(e.KeyCode==Keys.E)keyE=true;
			if(e.KeyCode==Keys.C)keyC=true;
			if(e.KeyCode==Keys.ControlKey)fly=!fly;
		}
		private void Form1_KeyUp(object sender, KeyEventArgs e)
		{
			if(e.KeyCode==Keys.Space)step=1;
			if(e.KeyCode==Keys.W)keyW=false;
			if(e.KeyCode==Keys.S)keyS=false;
			if(e.KeyCode==Keys.A)keyA=false;
			if(e.KeyCode==Keys.D)keyD=false;
			if(e.KeyCode==Keys.E)keyE=false;
			if(e.KeyCode==Keys.C)keyC=false;
		}

		private void Form1_MouseWheel(object sender, MouseEventArgs e)
		{
			if(e.Delta>0)
			{
				cam.zoom_level*=2;
			}
			else
			{
				if(cam.zoom_level>1)cam.zoom_level/=2;
			}
		}
		private void timer1_Tick(object sender, System.EventArgs e)
		{
			if(fly)
			{
				if(keyW)
				{
					cam.x=(float)(cam.x+speed*Math.Sin(cam.direction)*Math.Cos(cam.angle));
					cam.y=(float)(cam.y+speed*Math.Sin(cam.angle));
					cam.z=(float)(cam.z+speed*Math.Cos(cam.direction)*Math.Cos(cam.angle));
				}
				if(keyS)
				{
					cam.x=(float)(cam.x-speed*Math.Sin(cam.direction)*Math.Cos(cam.angle));
					cam.y=(float)(cam.y-speed*Math.Sin(cam.angle));
					cam.z=(float)(cam.z-speed*Math.Cos(cam.direction)*Math.Cos(cam.angle));
				}
				if(keyE)
				{
					cam.y=cam.y+speed;
				}
				if(keyC)
				{
					cam.y=cam.y-speed;
				}
			}
			else
			{
				if(keyW)
				{
					cam.x=(float)(cam.x+speed*Math.Sin(cam.direction));
					cam.z=(float)(cam.z+speed*Math.Cos(cam.direction));
				}
				if(keyS)
				{
					cam.x=(float)(cam.x-speed*Math.Sin(cam.direction));
					cam.z=(float)(cam.z-speed*Math.Cos(cam.direction));
				}
				cam.y=floor.get_alti(cam.x,cam.z)+2;
			}
			if(keyA)
			{
				cam.x=(float)(cam.x+speed*Math.Sin(cam.direction-Math.PI/2)/2);
				cam.z=(float)(cam.z+speed*Math.Cos(cam.direction-Math.PI/2)/2);
			}
			if(keyD)
			{
				cam.x=(float)(cam.x+speed*Math.Sin(cam.direction+Math.PI/2)/2);
				cam.z=(float)(cam.z+speed*Math.Cos(cam.direction+Math.PI/2)/2);
			}

			tick+=step;
			if(bullet_life!=0)
			{
				bullet_speed*=0.9f;
				bullet_gravity_speedup+=0.002f;
				bullet_life--;
				if(bullet_life==0)
				{
					device.removeObject(bullet);
				}
				bullet_x=(float)(bullet_x+bullet_speed*Math.Sin(bullet_direction)*Math.Cos(bullet_angle));
				bullet_y=(float)(bullet_y+bullet_speed*Math.Sin(bullet_angle)-bullet_gravity_speedup);
				bullet_z=(float)(bullet_z+bullet_speed*Math.Cos(bullet_direction)*Math.Cos(bullet_angle));
			}
			this.setLight(device.device.Lights);
			this.setObject();
			device.Render();
		}

		private void setLight(Microsoft.DirectX.Direct3D.LightsCollection Lights)
		{
			Lights[0].Type = LightType.Point;
			Lights[0].Diffuse = Color.Red;
			Lights[0].Ambient = Color.White;
			Lights[0].Attenuation0 = 1f;
			Lights[0].Attenuation1 = 1f;
			Lights[0].Attenuation2 = 1f;
			Lights[0].Range= 10;
			Lights[0].Position = new Vector3(bullet_x,bullet_y,bullet_z);

			if(bullet_life!=0)
			{
				Lights[0].Enabled = true;
			}
			else
			{
				Lights[0].Enabled = false;
			}

			Lights[1].Type = LightType.Point;
			Lights[1].Diffuse = Color.Pink;
			Lights[1].Ambient = Color.White;
			Lights[1].Attenuation0 = 0.001f;
			Lights[1].Attenuation1 = 0.001f;
			Lights[1].Attenuation2 = 0.001f;
			Lights[1].Range= 100;
			Lights[1].Position = new Vector3(30,5,30);
			Lights[1].Enabled = true;
		
		}

		private void setObject()
		{

			earth.reset_matrix();
			earth.zoom(0.2f,0.2f,0.2f);
			earth.RotationY(-(float)tick/10);
			earth.RotationX((float)(Math.PI*0.15));
			earth.move(30,5.01f,30);

			box.reset_matrix();
			box.zoom(0.0005f,0.0005f,0.0025f);
			box.move(5,10,5);

			moon.reset_matrix();
			moon.zoom(0.05f,0.05f,0.05f);
			moon.RotationY((float)tick/20);
			moon.move((float)(Math.Sin(-tick/300)*20),0,(float)(Math.Cos(-tick/300)*20));
			moon.move(30,10f,30);


			room.reset_matrix();
			room.zoom(0.02f,0.02f,0.02f);
			room.move(30,1.01f,30);

			if(bullet_life!=0)
			{
				bullet.reset_matrix();
				bullet.RotationZ(3.14f);
				bullet.RotationZ(bullet_angle);
				bullet.RotationY(bullet_direction-(float)(3.14/2));
				bullet.zoom(0.02f,0.02f,0.02f);
				bullet.move(bullet_x,bullet_y,bullet_z);
			}
			floor.reset_matrix();
			floor.move(0,0,0);

		}
	}
}
