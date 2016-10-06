using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using System.Collections.Generic;

namespace myd3d
{
	public delegate void SetObject();

	public abstract class D3DObject
	{
		public Device device;
		public bool visible = true;
		public Matrix m = new Matrix();
		public D3DObject(Device device)
		{
			this.device = device;
			m.M11 = 1;
			m.M22 = 1;
			m.M33 = 1;
			m.M44 = 1;
		}
		public void reset_matrix()
		{
			m = new Matrix();
			m.M11 = 1;
			m.M22 = 1;
			m.M33 = 1;
			m.M44 = 1;
		}
		public void zoom(float x, float y, float z)
		{
			m.Multiply(Matrix.Scaling(x, y, z));
		}
		public void move(float x, float y, float z)
		{
			m.Multiply(Matrix.Translation(x, y, z));
		}
		public void move(Vector3 vec)
		{
			m.Multiply(Matrix.Translation(vec));
		}
		public void RotationX(float val)
		{
			m.Multiply(Matrix.RotationX(val));
		}
		public void RotationY(float val)
		{
			m.Multiply(Matrix.RotationY(val));
		}
		public void RotationZ(float val)
		{
			m.Multiply(Matrix.RotationZ(val));
		}
		public void Rotation(float x, float y, float z, float val)
		{
			m.Multiply(Matrix.RotationAxis(new Vector3(x, y, z), val));
		}
		public abstract void Render();
		public abstract void Distory();
	}

	public class D3DCamera
	{
		public float x = 50;
		public float y = 0;
		public float z = 50;
		public float direction = 4;
		public float angle = 0;
		public float zoom_level = 1;
		public void reset()
		{
			x = 50;
			y = 0;
			z = 50;
			direction = 4;
			angle = 0;
			zoom_level = 1;
		}
		public D3DCamera Clone()
		{
			D3DCamera ret = new D3DCamera();
			ret.x = x;
			ret.y = y;
			ret.z = z;
			ret.direction = direction;
			ret.angle = angle;
			ret.zoom_level = zoom_level;
			return ret;
		}
	}

	public class D3DDevice
	{
		private float ratio;
		PictureBox render1;
		PictureBox render2;

		public D3DCamera cam = new D3DCamera();
		public Device device = null;
		ArrayList objs = new ArrayList();
		SetObject setObject;

		bool keyW = false;
		bool keyS = false;
		bool keyA = false;
		bool keyD = false;
		bool keyE = false;
		bool keyC = false;
		bool mouseRelease = true;
		int mousex;
		int mousey;
		float speed = 2f;
		Timer t = new Timer();

		public D3DDevice(Form form, PictureBox render1, PictureBox render2, SetObject setObject)
		{
			ratio = (float)render1.Height / (float)render1.Width;

			PresentParameters presentParams = new PresentParameters();

			presentParams.Windowed = true;
			presentParams.SwapEffect = SwapEffect.Flip;
			presentParams.EnableAutoDepthStencil = true;
			presentParams.AutoDepthStencilFormat = DepthFormat.D24X8;
			presentParams.BackBufferFormat = Microsoft.DirectX.Direct3D.Format.X8R8G8B8;
			presentParams.BackBufferCount = 2;
			presentParams.BackBufferWidth = render1.Width;
			presentParams.BackBufferHeight = render1.Height;

			device = new Device(0, DeviceType.Hardware, render1, CreateFlags.SoftwareVertexProcessing, presentParams);

			{
				device.SamplerState[0].MinFilter = TextureFilter.Anisotropic;
				device.SamplerState[0].MagFilter = TextureFilter.Anisotropic;
				device.SamplerState[0].MipFilter = TextureFilter.Anisotropic;
				//device.SetSamplerState(0,SamplerStageStates.MinFilter,3);
				//device.SetSamplerState(0,SamplerStageStates.MagFilter,3);
				//device.SetSamplerState(0,SamplerStageStates.MipFilter,3);
			}

			device.RenderState.CullMode = Cull.CounterClockwise;
			device.RenderState.ZBufferEnable = true;
			//device.RenderState.Ambient = Color.White;
			device.RenderState.Lighting = false;

			//init
			{
				t.Interval = 30;
				t.Tick += new EventHandler(t_Tick);

				this.render1 = render1;
				this.render2 = render2;
				this.setObject = setObject;

				form.KeyDown += new KeyEventHandler(form_KeyDown);
				form.KeyUp += new KeyEventHandler(form_KeyUp);
				form.KeyPreview = true;

				render1.MouseMove += new MouseEventHandler(render_MouseMove);
				render1.MouseWheel += new MouseEventHandler(render_MouseWheel);
				render1.MouseDown += new MouseEventHandler(render_MouseDown);
				render1.MouseUp += new MouseEventHandler(render_MouseUp);
				if (render2 != null)
				{
					render2.MouseMove += new MouseEventHandler(render_MouseMove);
					render2.MouseWheel += new MouseEventHandler(render_MouseWheel);
					render2.MouseDown += new MouseEventHandler(render_MouseDown);
					render2.MouseUp += new MouseEventHandler(render_MouseUp);
				}
			}
		}

		public void Start()
		{

			t.Start();
		}

		public void addObject(D3DObject obj)
		{
			objs.Add(obj);
		}
		public void removeObject(D3DObject obj)
		{
			obj.Distory();
			objs.Remove(obj);
		}
		public void removeAllObject()
		{
			foreach (D3DObject obj in objs)
			{
				obj.Distory();
			}
			objs.Clear();
		}
		public bool containsObject(D3DObject obj)
		{
			return objs.Contains(obj);
		}


		void t_Tick(object sender, EventArgs e)
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
				cam.x = (float)(cam.x + speed * Math.Sin(cam.direction - Math.PI * 0.5) * 0.5);
				cam.z = (float)(cam.z + speed * Math.Cos(cam.direction - Math.PI * 0.5) * 0.5);
			}
			if (keyD)
			{
				cam.x = (float)(cam.x + speed * Math.Sin(cam.direction + Math.PI * 0.5) * 0.5);
				cam.z = (float)(cam.z + speed * Math.Cos(cam.direction + Math.PI * 0.5) * 0.5);
			}

			setObject();

			float size = 0.3f;
			{
				D3DCamera cam1 = cam.Clone();
				cam1.x = (float)(cam1.x + size * Math.Sin(cam1.direction - Math.PI * 0.5) * 0.5);
				cam1.z = (float)(cam1.z + size * Math.Cos(cam1.direction - Math.PI * 0.5) * 0.5);
				cam1.direction += 0.05f;
				Render(device, cam1, render1, objs, ratio);
			}

			if (render2 != null)
			{
				D3DCamera cam2 = cam.Clone();
				cam2.x = (float)(cam2.x + size * Math.Sin(cam2.direction + Math.PI * 0.5) * 0.5);
				cam2.z = (float)(cam2.z + size * Math.Cos(cam2.direction + Math.PI * 0.5) * 0.5);
				cam2.direction -= 0.05f;
				Render(device, cam2, render2, objs, ratio);
			}
		}

		private void form_KeyDown(object sender, KeyEventArgs e)
		{
			if (e.KeyCode == Keys.W) keyW = true;
			if (e.KeyCode == Keys.S) keyS = true;
			if (e.KeyCode == Keys.A) keyA = true;
			if (e.KeyCode == Keys.D) keyD = true;
			if (e.KeyCode == Keys.E) keyE = true;
			if (e.KeyCode == Keys.C) keyC = true;
		}

		private void form_KeyUp(object sender, KeyEventArgs e)
		{
			if (e.KeyCode == Keys.W) keyW = false;
			if (e.KeyCode == Keys.S) keyS = false;
			if (e.KeyCode == Keys.A) keyA = false;
			if (e.KeyCode == Keys.D) keyD = false;
			if (e.KeyCode == Keys.E) keyE = false;
			if (e.KeyCode == Keys.C) keyC = false;
		}

		private void render_MouseMove(object sender, MouseEventArgs e)
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

		private void render_MouseWheel(object sender, MouseEventArgs e)
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

		private void render_MouseDown(object sender, MouseEventArgs e)
		{
			mousex = Cursor.Position.X;
			mousey = Cursor.Position.Y;
			mouseRelease = false;
			System.Windows.Forms.Cursor.Hide();
		}

		private void render_MouseUp(object sender, MouseEventArgs e)
		{
			mouseRelease = true;
			System.Windows.Forms.Cursor.Show();
		}

		private static void Render(Device device, D3DCamera cam, PictureBox render, ArrayList objs, float ratio)
		{
			try
			{
				device.Clear(ClearFlags.Target | ClearFlags.ZBuffer, System.Drawing.Color.Gray, 1.0f, 0);
				device.BeginScene();

				float x1 = (float)(cam.x + Math.Sin(cam.direction) * Math.Cos(cam.angle));
				float y1 = (float)(cam.y + Math.Sin(cam.angle));
				float z1 = (float)(cam.z + Math.Cos(cam.direction) * Math.Cos(cam.angle));

				device.Transform.View = Matrix.LookAtLH(new Vector3(cam.x, cam.y, cam.z), new Vector3(x1, y1, z1), new Vector3(0, 1, 0));
				float zoomlv = 1 / cam.zoom_level;
				device.Transform.Projection = Matrix.PerspectiveLH(zoomlv, zoomlv * ratio, 1f, 100000f);

				foreach (D3DObject obj in objs)
				{
					if (obj.visible)
					{
						device.Transform.World = obj.m;
						obj.Render();
					}
				}
				device.EndScene();
				device.Present(render);
			}
			catch
			{
			}
		}
	}

	public class Terrain : D3DObject
	{
		VertexBuffer vbA;
		VertexBuffer vbB;
		VertexBuffer vbC;
		VertexBuffer vbD;
		Texture texture;
		int width;
		int heigh;
		protected Material material = new Material();

		float[,] alti;

		public Terrain(Device device, string texture_file, string terrain_file)
			: base(device)
		{
			material.Ambient = Color.White;
			material.Diffuse = Color.White;

			texture = TextureLoader.FromFile(device, texture_file);
			System.IO.BinaryReader br = new System.IO.BinaryReader(System.IO.File.OpenRead(terrain_file));
			width = br.ReadInt16();
			heigh = width;
			alti = new float[width, heigh];

			for (int j = 0; j < heigh; j++)
			{
				for (int i = 0; i < width; i++)
				{
					alti[i, j] = (float)br.ReadInt16() / 100;
				}
			}
			br.Close();

			CustomVertex.PositionTextured[,] base_verts = new Microsoft.DirectX.Direct3D.CustomVertex.PositionTextured[width, heigh];

			vbA = new VertexBuffer(typeof(CustomVertex.PositionTextured), (width - 1) * heigh / 2 * 3, device, Microsoft.DirectX.Direct3D.Usage.DoNotClip, CustomVertex.PositionTextured.Format, Pool.Default);
			vbB = new VertexBuffer(typeof(CustomVertex.PositionTextured), (width - 1) * heigh / 2 * 3, device, Microsoft.DirectX.Direct3D.Usage.DoNotClip, CustomVertex.PositionTextured.Format, Pool.Default);
			vbC = new VertexBuffer(typeof(CustomVertex.PositionTextured), (width - 1) * heigh / 2 * 3, device, Microsoft.DirectX.Direct3D.Usage.DoNotClip, CustomVertex.PositionTextured.Format, Pool.Default);
			vbD = new VertexBuffer(typeof(CustomVertex.PositionTextured), (width - 1) * heigh / 2 * 3, device, Microsoft.DirectX.Direct3D.Usage.DoNotClip, CustomVertex.PositionTextured.Format, Pool.Default);

			CustomVertex.PositionTextured[] verts;

			for (int i = 0; i < width; i++)
			{
				for (int j = 0; j < heigh; j++)
				{
					if (j % 2 == 0)
					{
						base_verts[i, j].X = i;
						base_verts[i, j].Y = alti[i, j];
						base_verts[i, j].Z = j * 0.866f;
						base_verts[i, j].Tu = (i) / 10f;
						base_verts[i, j].Tv = (j * 0.866f) / 10f;
					}
					else
					{
						base_verts[i, j].X = i + 0.5f;
						base_verts[i, j].Y = alti[i, j];
						base_verts[i, j].Z = j * 0.866f;
						base_verts[i, j].Tu = (i + 0.5f) / 10f;
						base_verts[i, j].Tv = (j * 0.866f) / 10f;
					}
				}
			}

			int idx;

			idx = 0;
			verts = (CustomVertex.PositionTextured[])vbA.Lock(0, 0);
			for (int i = 0; i < width - 1; i++)
			{
				for (int j = 0; j < heigh / 2; j++)
				{
					verts[idx++] = base_verts[i, j * 2 + 1];
					verts[idx++] = base_verts[i + 1, j * 2];
					verts[idx++] = base_verts[i, j * 2];
				}
			}
			vbA.Unlock();

			idx = 0;
			verts = (CustomVertex.PositionTextured[])vbB.Lock(0, 0);
			for (int i = 0; i < width - 1; i++)
			{
				for (int j = 0; j < heigh / 2; j++)
				{
					verts[idx++] = base_verts[i, j * 2 + 1];
					verts[idx++] = base_verts[i + 1, j * 2 + 1];
					verts[idx++] = base_verts[i + 1, j * 2];
				}
			}
			vbB.Unlock();

			idx = 0;
			verts = (CustomVertex.PositionTextured[])vbC.Lock(0, 0);
			for (int i = 0; i < width - 1; i++)
			{
				for (int j = 0; j < heigh / 2 - 1; j++)
				{
					verts[idx++] = base_verts[i, j * 2 + 2];
					verts[idx++] = base_verts[i + 1, j * 2 + 2];
					verts[idx++] = base_verts[i, j * 2 + 1];
				}
			}
			vbC.Unlock();

			idx = 0;
			verts = (CustomVertex.PositionTextured[])vbD.Lock(0, 0);
			for (int i = 0; i < width - 1; i++)
			{
				for (int j = 0; j < heigh / 2 - 1; j++)
				{
					verts[idx++] = base_verts[i + 1, j * 2 + 2];
					verts[idx++] = base_verts[i + 1, j * 2 + 1];
					verts[idx++] = base_verts[i, j * 2 + 1];
				}
			}
			vbD.Unlock();

		}

		public float get_alti(float x, float z)
		{
			try
			{
				float temp_z = z; float temp_x = x;

				float[] mx = new float[6];
				float[] my = new float[6];
				float[] m_alti = new float[6];
				float[] m_dist = new float[6];

				z = z / 0.866f;

				if (((int)z) % 2 == 0)
				{
					mx[0] = (int)x - 1; my[0] = ((int)z) * 0.866f;
					mx[1] = (int)x; my[1] = ((int)z) * 0.866f;
					mx[2] = (int)x + 1; my[2] = ((int)z) * 0.866f;
					mx[3] = (int)x - 0.5f; my[3] = ((int)z + 1) * 0.866f;
					mx[4] = (int)x + 0.5f; my[4] = ((int)z + 1) * 0.866f;
					mx[5] = (int)x + 1.5f; my[5] = ((int)z + 1) * 0.866f;
				}
				else
				{
					mx[0] = (int)x - 0.5f; my[0] = ((int)z) * 0.866f;
					mx[1] = (int)x + 0.5f; my[1] = ((int)z) * 0.866f;
					mx[2] = (int)x + 1.5f; my[2] = ((int)z) * 0.866f;
					mx[3] = (int)x - 1; my[3] = ((int)z + 1) * 0.866f;
					mx[4] = (int)x; my[4] = ((int)z + 1) * 0.866f;
					mx[5] = (int)x + 1; my[5] = ((int)z + 1) * 0.866f;
				}

				for (int i = 0; i < 6; i++)
				{
					if (mx[i] < 0 || my[i] < 0)
					{
						m_dist[i] = float.MaxValue;
					}
					else
					{
						m_dist[i] = (mx[i] - x) * (mx[i] - x) + (my[i] - temp_z) * (my[i] - temp_z);
						if ((int)(my[i] / 0.866f) % 2 == 0)
						{
							m_alti[i] = alti[(int)(mx[i]), (int)(my[i] / 0.866f)];
						}
						else
						{
							m_alti[i] = alti[(int)(mx[i] - 0.5), (int)(my[i] / 0.866f)];
						}
					}
				}

				for (int i = 0; i < 6; i++)
				{
					for (int j = 4; j >= 0; j--)
					{
						if (m_dist[j] >= m_dist[j + 1])
						{
							float t;
							t = m_dist[j]; m_dist[j] = m_dist[j + 1]; m_dist[j + 1] = t;
							t = m_alti[j]; m_alti[j] = m_alti[j + 1]; m_alti[j + 1] = t;
							t = mx[j]; mx[j] = mx[j + 1]; mx[j + 1] = t;
							t = my[j]; my[j] = my[j + 1]; my[j + 1] = t;
						}
					}
				}

				float x1 = mx[0]; float y1 = m_alti[0]; float z1 = my[0];
				float x2 = mx[1]; float y2 = m_alti[1]; float z2 = my[1];
				float x3 = mx[2]; float y3 = m_alti[2]; float z3 = my[2];
				float a; float b; float c;

				float t1 = y1 - y2;
				float t2 = 1 / (x1 - x2);
				float t3 = z1 - z2;
				float t4 = t2 * (x2 - x3);
				b = (t1 * t4 - y2 + y3) / (t3 * t4 - z2 + z3);
				a = (t1 - b * t3) * t2;
				c = y1 - a * x1 - b * z1;
				return temp_x * a + temp_z * b + c;
			}
			catch
			{
				return 0;
			}
		}

		public override void Render()
		{
			device.Material = material;
			device.SetTexture(0, texture);
			device.VertexFormat = CustomVertex.PositionTextured.Format;
			device.SetStreamSource(0, vbA, 0);
			device.DrawPrimitives(PrimitiveType.TriangleList, 0, (width - 1) * heigh / 2);
			device.SetStreamSource(0, vbB, 0);
			device.DrawPrimitives(PrimitiveType.TriangleList, 0, (width - 1) * heigh / 2);
			device.SetStreamSource(0, vbC, 0);
			device.DrawPrimitives(PrimitiveType.TriangleList, 0, (width - 1) * heigh / 2);
			device.SetStreamSource(0, vbD, 0);
			device.DrawPrimitives(PrimitiveType.TriangleList, 0, (width - 1) * heigh / 2);
		}
		public override void Distory()
		{
			vbA.Dispose();
			vbB.Dispose();
			vbC.Dispose();
			vbD.Dispose();
			texture.Dispose();
		}
	}

	public class Model : D3DObject
	{
		Mesh mesh = null;
		Material[] meshMaterials;
		Texture[] meshTextures;
		public Model(Device device, string filename)
			: base(device)
		{
			ExtendedMaterial[] materials = null;
			mesh = Mesh.FromFile(filename, MeshFlags.SystemMemory, device, out materials);
			//?mesh = Mesh.LoadHierarchyFromFile(filename, MeshFlags.Managed, device, out materials);
			meshTextures = new Texture[materials.Length];
			meshMaterials = new Material[materials.Length];
			for (int i = 0; i < materials.Length; i++)
			{
				meshMaterials[i] = materials[i].Material3D;
				meshMaterials[i].Ambient = Color.White;
				meshMaterials[i].Diffuse = Color.White;

				if (materials[i].TextureFilename != null)
				{
					try
					{
						meshTextures[i] = TextureLoader.FromFile(device, materials[i].TextureFilename);
					}
					catch
					{ }
				}
			}
		}
		public Model(Device device, string filename, string filename2)
			: base(device)
		{
			ExtendedMaterial[] materials = null;
			mesh = Mesh.FromFile(filename, MeshFlags.SystemMemory, device, out materials);
			meshTextures = new Texture[materials.Length];
			meshMaterials = new Material[materials.Length];
			for (int i = 0; i < materials.Length; i++)
			{
				meshMaterials[i] = materials[i].Material3D;
				meshMaterials[i].Ambient = Color.White;
				meshMaterials[i].Diffuse = Color.White;

				try
				{
					meshTextures[i] = TextureLoader.FromFile(device, filename2);
				}
				catch
				{ }
			}
		}
		public override void Render()
		{
			for (int i = 0; i < meshTextures.Length; i++)
			{
				device.Material = meshMaterials[i];
				device.SetTexture(0, meshTextures[i]);
				//bumpMap
				//device.SetTextureStageState(0,TextureStageStates.BumpEnvironmentMaterial00,0);
				mesh.DrawSubset(i);
			}
		}
		public override void Distory()
		{
			mesh.Dispose();
			foreach (Texture text in meshTextures)
			{
				text.Dispose();
			}
		}
	}

	public class Text : D3DObject
	{
		Mesh mesh = null;
		//Æ«²î,¼·Ñ¹
		public Text(Device device, string text, System.Drawing.Font f, float deviation, float extrusion)
			: base(device)
		{
			mesh = Mesh.TextFromFont(device, f, text, deviation, extrusion);
		}
		public override void Render()
		{
			mesh.DrawSubset(0);
		}
		public override void Distory()
		{
			mesh.Dispose();
		}
	}

	public class Line : D3DObject
	{
		public int count = 0;
		public VertexBuffer vertexBuffer = null;
		List<CustomVertex.PositionColored> vertexList;
		CustomVertex.PositionColored[] vertexArray;
		private int index;
		private bool finished = false;
		public Line(int count, Device device)
			: base(device)
		{
			vertexBuffer = new VertexBuffer(typeof(CustomVertex.PositionColored), count * 2, device, 0, CustomVertex.PositionColored.Format, Pool.Default);
			this.count = count;
			vertexArray = (CustomVertex.PositionColored[])vertexBuffer.Lock(0, LockFlags.None);
			index = 0;
		}
		public Line(Device device)
			: base(device)
		{
			vertexList = new List<CustomVertex.PositionColored>();
			this.count = -1;
		}
		public void addLine(Vector3 v1, Vector3 v2, int c)
		{
			if (count == -1)
			{
				{
					CustomVertex.PositionColored vertex = new CustomVertex.PositionColored();
					vertex.SetPosition(v1);
					//vertex.Position=(v1);
					vertex.Color = c;
					vertexList.Add(vertex);
				}
				{
					CustomVertex.PositionColored vertex = new CustomVertex.PositionColored();
					vertex.SetPosition(v2);
					//vertex.Position=(v2);
					vertex.Color = c;
					vertexList.Add(vertex);
				}
			}
			else
			{
				if (index < vertexArray.Length)
				{
					vertexArray[index].SetPosition(v1);
					//vertexArray[index].Position=(v1);
					vertexArray[index++].Color = c;
					vertexArray[index].SetPosition(v2);
					//vertexArray[index].Position=(v2);
					vertexArray[index++].Color = c;
				}
			}
		}
		public void addLine(double x1, double y1, double z1, double x2, double y2, double z2, int c)
		{
			addLine(new Vector3((float)x1, (float)y1, (float)z1), new Vector3((float)x2, (float)y2, (float)z2), c);
		}
		public void Finish()
		{
			if (finished)
			{
				return;
			}
			if (count == -1)
			{
				if (vertexList.Count > 0)
				{
					vertexBuffer = new VertexBuffer(typeof(CustomVertex.PositionColored), vertexList.Count, device, 0, CustomVertex.PositionColored.Format, Pool.Default);
					vertexArray = (CustomVertex.PositionColored[])vertexBuffer.Lock(0, LockFlags.None);
					for (int i = 0; i < vertexList.Count; i++)
					{
						vertexArray[i] = vertexList[i];
					}
					vertexBuffer.Unlock();
					count = vertexList.Count / 2;
				}
			}
			else
			{
				vertexBuffer.Unlock();
			}
			finished = true;
		}
		public override void Render()
		{
			if (finished)
			{
				device.SetStreamSource(0, vertexBuffer, 0);
				device.VertexFormat = CustomVertex.PositionColored.Format;
				device.DrawPrimitives(PrimitiveType.LineList, 0, count);
			}
		}
		public override void Distory()
		{
			vertexBuffer.Dispose();
		}
	}

	public class Chain : D3DObject
	{
		public int count = 0;
		public VertexBuffer vertexBuffer = null;
		List<CustomVertex.PositionColored> vertexList;
		CustomVertex.PositionColored[] vertexArray;
		private int index;
		private bool finished = false;
		public Chain(int count, Device device)
			: base(device)
		{
			vertexBuffer = new VertexBuffer(typeof(CustomVertex.PositionColored), count * 2, device, 0, CustomVertex.PositionColored.Format, Pool.Default);
			this.count = count;
			vertexArray = (CustomVertex.PositionColored[])vertexBuffer.Lock(0, LockFlags.None);
			index = 0;
		}
		public Chain(Device device)
			: base(device)
		{
			vertexList = new List<CustomVertex.PositionColored>();
			this.count = -1;
		}
		public void addNode(Vector3 v1, int c)
		{
			if (count == -1)
			{
				{
					CustomVertex.PositionColored vertex = new CustomVertex.PositionColored();
					vertex.SetPosition(v1);
					//vertex.Position=(v1);
					vertex.Color = c;
					vertexList.Add(vertex);
				}
			}
			else
			{
				if (index < vertexArray.Length)
				{
					vertexArray[index].SetPosition(v1);
					//vertexArray[index].Position=(v1);
					vertexArray[index++].Color = c;
				}
			}
		}
		public void addNode(double x1, double y1, double z1, int c)
		{
			addNode(new Vector3((float)x1, (float)y1, (float)z1), c);
		}
		public void Finish()
		{
			if (finished)
			{
				return;
			}
			if (count == -1)
			{
				if (vertexList.Count > 0)
				{
					try
					{
						vertexBuffer = new VertexBuffer(typeof(CustomVertex.PositionColored), vertexList.Count, device, 0, CustomVertex.PositionColored.Format, Pool.Default);
						vertexArray = (CustomVertex.PositionColored[])vertexBuffer.Lock(0, LockFlags.None);
						for (int i = 0; i < vertexList.Count; i++)
						{
							vertexArray[i] = vertexList[i];
						}
						vertexBuffer.Unlock();
						count = vertexList.Count - 1;
					}
					catch
					{
					}
				}
			}
			else
			{
				vertexBuffer.Unlock();
			}
			finished = true;
		}
		public override void Render()
		{
			if (finished)
			{
				device.SetTexture(0, null);
				device.SetStreamSource(0, vertexBuffer, 0);
				device.VertexFormat = CustomVertex.PositionColored.Format;
				device.DrawPrimitives(PrimitiveType.LineStrip, 0, count);
			}
		}
		public override void Distory()
		{
			try
			{
				vertexBuffer.Dispose();
			}
			catch
			{
			}
		}
	}

	public class Face : D3DObject
	{
		public int count = 0;
		public VertexBuffer vertexBuffer = null;
		List<CustomVertex.PositionColored> vertexList;
		CustomVertex.PositionColored[] vertexArray;
		private int index;
		private bool finished = false;
		public Face(Device device)
			: base(device)
		{
			vertexList = new List<CustomVertex.PositionColored>();
			this.count = -1;
		}
		public void addFace(Vector3 v1, Vector3 v2, Vector3 v3, int c)
		{
			if (count == -1)
			{
				{
					CustomVertex.PositionColored vertex = new CustomVertex.PositionColored();
					vertex.SetPosition(v1);
					//vertex.Position=(v1);
					vertex.Color = c;
					vertexList.Add(vertex);
				}
				{
					CustomVertex.PositionColored vertex = new CustomVertex.PositionColored();
					vertex.SetPosition(v2);
					//vertex.Position=(v2);
					vertex.Color = c;
					vertexList.Add(vertex);
				}
				{
					CustomVertex.PositionColored vertex = new CustomVertex.PositionColored();
					vertex.SetPosition(v3);
					//vertex.Position=(v3);
					vertex.Color = c;
					vertexList.Add(vertex);
				}
			}
			else
			{
				if (index < vertexArray.Length)
				{
					vertexArray[index].SetPosition(v1);
					//vertexArray[index].Position=(v1);
					vertexArray[index++].Color = c;
					vertexArray[index].SetPosition(v2);
					//vertexArray[index].Position=(v2);
					vertexArray[index++].Color = c;
					vertexArray[index].SetPosition(v3);
					//vertexArray[index].Position=(v3);
					vertexArray[index++].Color = c;
				}
			}
		}
		public void addFace(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, int c)
		{
			addFace(
				new Vector3((float)x1, (float)y1, (float)z1),
				new Vector3((float)x2, (float)y2, (float)z2),
				new Vector3((float)x3, (float)y3, (float)z3),
				c);
		}
		public void Finish()
		{
			if (finished)
			{
				return;
			}
			if (count == -1)
			{
				if (vertexList.Count > 0)
				{
					vertexBuffer = new VertexBuffer(typeof(CustomVertex.PositionColored), vertexList.Count, device, 0, CustomVertex.PositionColored.Format, Pool.Default);
					vertexArray = (CustomVertex.PositionColored[])vertexBuffer.Lock(0, LockFlags.None);
					for (int i = 0; i < vertexList.Count; i++)
					{
						vertexArray[i] = vertexList[i];
					}
					vertexBuffer.Unlock();
					count = vertexList.Count / 3;
				}
			}
			else
			{
				vertexBuffer.Unlock();
			}
			finished = true;
		}
		public override void Render()
		{
			if (finished)
			{
				device.SetStreamSource(0, vertexBuffer, 0);
				device.VertexFormat = CustomVertex.PositionColored.Format;
				device.DrawPrimitives(PrimitiveType.TriangleList, 0, count);
			}
		}
		public override void Distory()
		{
			vertexBuffer.Dispose();
		}
	}

}

