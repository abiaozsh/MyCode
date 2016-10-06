using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;

namespace myd3d
{
	abstract class D3DObject
	{
		protected Matrix m = new Matrix();
		protected Material material = new Material();
		public D3DObject()
		{
			material.Ambient = Color.Black;
			material.Diffuse = Color.White;
			m.M11 =1;
			m.M22 =1;
			m.M33 =1;
			m.M44 =1;
		}
		public void reset_matrix()
		{
			m = new Matrix();
			m.M11 =1;
			m.M22 =1;
			m.M33 =1;
			m.M44 =1;
		}
		public void zoom(float x,float y,float z)
		{
			m.Multiply(Matrix.Scaling(x,y,z));
		}
		public void move(float x,float y,float z)
		{
			m.Multiply(Matrix.Translation(x,y,z));
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
		public void Rotation(float x,float y,float z,float val)
		{
			m.Multiply(Matrix.RotationAxis(new Vector3(x,y,z),val));
		}
		public abstract void Render(Device device);
	}

	class D3DCamera
	{
		public float x=3;
		public float y=0;
		public float z=3;
		public float direction=0;
		public float angle=0;
		public float zoom_level=1;
	}

	class D3DDevice
	{
		private float draw_width;
		private float draw_height;

		public D3DCamera cam;
		public Device device = null;
		private ArrayList objs = new ArrayList();
		public D3DDevice(System.Windows.Forms.Control render,float width,float height)
		{
			draw_width = width;
			draw_height = height;

			PresentParameters presentParams = new PresentParameters();

			presentParams.Windowed = true;
			presentParams.SwapEffect = SwapEffect.Flip;
			presentParams.EnableAutoDepthStencil = true;
			presentParams.AutoDepthStencilFormat = DepthFormat.D24X8;
			presentParams.BackBufferFormat = Microsoft.DirectX.Direct3D.Format.X8R8G8B8;
			presentParams.BackBufferCount = 2;
			presentParams.BackBufferWidth = (int)width;
			presentParams.BackBufferHeight = (int)height;

			if(device==null)try{device = new Device(0, DeviceType.Hardware,render,CreateFlags.PureDevice|CreateFlags.HardwareVertexProcessing, presentParams);}catch{}
//			if(device==null)try{device = new Device(0, DeviceType.Hardware,render,CreateFlags.HardwareVertexProcessing, presentParams);}catch{}
//			if(device==null)try{device = new Device(0, DeviceType.Hardware,render,CreateFlags.SoftwareVertexProcessing, presentParams);}catch{}
//			if(device==null)try{device = new Device(0, DeviceType.Software,render,CreateFlags.SoftwareVertexProcessing, presentParams);}catch{}
//			if(device==null)presentParams.AutoDepthStencilFormat = DepthFormat.D16;
//			if(device==null)try{device = new Device(0, DeviceType.Hardware,render,CreateFlags.PureDevice|CreateFlags.HardwareVertexProcessing, presentParams);}catch{}
//			if(device==null)try{device = new Device(0, DeviceType.Hardware,render,CreateFlags.HardwareVertexProcessing, presentParams);}catch{}
//			if(device==null)try{device = new Device(0, DeviceType.Hardware,render,CreateFlags.SoftwareVertexProcessing, presentParams);}catch{}
//			if(device==null)try{device = new Device(0, DeviceType.Software,render,CreateFlags.SoftwareVertexProcessing, presentParams);}catch{}

			device.SetSamplerState(0,SamplerStageStates.MinFilter,3);
			device.SetSamplerState(0,SamplerStageStates.MagFilter,3);
			device.SetSamplerState(0,SamplerStageStates.MipFilter,3);
			device.RenderState.CullMode = Cull.CounterClockwise;
			device.RenderState.ZBufferEnable = true;
			device.RenderState.Ambient =Color.White;
		}
		public void addObject(D3DObject obj)
		{
			objs.Add(obj);
		}
		public void removeObject(D3DObject obj)
		{
			objs.Remove(obj);
		}
		public bool containsObject(D3DObject obj)
		{
			return objs.Contains(obj);
		}

		public void Render()
		{
			device.Clear(ClearFlags.Target | ClearFlags.ZBuffer, System.Drawing.Color.Gray, 1.0f, 0);
			device.BeginScene();
			//device.RenderState.Ambient = Color.White;

			float x1=(float)(cam.x+Math.Sin(cam.direction)*Math.Cos(cam.angle));
			float y1=(float)(cam.y+Math.Sin(cam.angle));
			float z1=(float)(cam.z+Math.Cos(cam.direction)*Math.Cos(cam.angle));

			device.Transform.View = Matrix.LookAtLH(new Vector3(cam.x,cam.y,cam.z),new Vector3(x1,y1,z1), new Vector3(0,1,0));
			float zoomlv = 1/cam.zoom_level;
			device.Transform.Projection = Matrix.PerspectiveLH(zoomlv,zoomlv*(draw_height/draw_width),1f,1000f);

			foreach(D3DObject obj in objs)
			{
				obj.Render(device);
			}
			device.EndScene();
			device.Present();
		}
	}

	class Terrain:D3DObject
	{
		VertexBuffer vbA;
		VertexBuffer vbB;
		VertexBuffer vbC;
		VertexBuffer vbD;
		Texture texture;
		int width;
		int heigh;
		
		float[,] alti;

		public Terrain(Device device,string texture_file,string terrain_file):base()
		{
			texture = TextureLoader.FromFile(device,texture_file);
			System.IO.BinaryReader br = new System.IO.BinaryReader(System.IO.File.OpenRead(terrain_file));
			width=br.ReadInt16();
			heigh=width;
			alti = new float[width,heigh];

			for(int j=0;j<heigh;j++)
			{
				for(int i=0;i<width;i++)
				{
					alti[i,j]=(float)br.ReadInt16()/100;
				}
			}

			br.Close();

			CustomVertex.PositionTextured [,] base_verts = new Microsoft.DirectX.Direct3D.CustomVertex.PositionTextured[width,heigh];
			
			vbA = new VertexBuffer(typeof(CustomVertex.PositionTextured),(width-1)*heigh/2*3, device, Microsoft.DirectX.Direct3D.Usage.None, CustomVertex.PositionTextured.Format, Pool.Default);
			vbB = new VertexBuffer(typeof(CustomVertex.PositionTextured),(width-1)*heigh/2*3, device, Microsoft.DirectX.Direct3D.Usage.None, CustomVertex.PositionTextured.Format, Pool.Default);
			vbC = new VertexBuffer(typeof(CustomVertex.PositionTextured),(width-1)*heigh/2*3, device, Microsoft.DirectX.Direct3D.Usage.None, CustomVertex.PositionTextured.Format, Pool.Default);
			vbD = new VertexBuffer(typeof(CustomVertex.PositionTextured),(width-1)*heigh/2*3, device, Microsoft.DirectX.Direct3D.Usage.None, CustomVertex.PositionTextured.Format, Pool.Default);

			CustomVertex.PositionTextured [] verts;

			
			for(int i=0;i<width;i++)
			{
				for(int j=0;j<heigh;j++)
				{
					if(j%2==0)
					{
						base_verts[i,j].X  = i;
						base_verts[i,j].Y  = alti[i,j];
						base_verts[i,j].Z  = j * 0.866f;
						base_verts[i,j].Tu = (i)/10f;
						base_verts[i,j].Tv = (j * 0.866f)/10f;
					}
					else
					{
						base_verts[i,j].X  = i + 0.5f;
						base_verts[i,j].Y  = alti[i,j];
						base_verts[i,j].Z  = j * 0.866f;
						base_verts[i,j].Tu = (i + 0.5f)/10f;
						base_verts[i,j].Tv = (j * 0.866f)/10f;
					}
				}
			}

			int idx;

			idx=0;
			verts = (CustomVertex.PositionTextured[])vbA.Lock(0,0);
			for(int i=0;i<width-1;i++)
			{
				for(int j=0;j<heigh/2;j++)
				{
					verts[idx++]=base_verts[i,j*2+1];
					verts[idx++]=base_verts[i+1,j*2];
					verts[idx++]=base_verts[i,j*2];
				}
			}

			vbA.Unlock();
	
			idx=0;
			verts = (CustomVertex.PositionTextured[])vbB.Lock(0,0);
			for(int i=0;i<width-1;i++)
			{
				for(int j=0;j<heigh/2;j++)
				{
					verts[idx++]=base_verts[i,j*2+1];
					verts[idx++]=base_verts[i+1,j*2+1];
					verts[idx++]=base_verts[i+1,j*2];
				}
			}
			vbB.Unlock();

			idx=0;
			verts = (CustomVertex.PositionTextured[])vbC.Lock(0,0);
			for(int i=0;i<width-1;i++)
			{
				for(int j=0;j<heigh/2-1;j++)
				{
					verts[idx++]=base_verts[i,j*2+2];
					verts[idx++]=base_verts[i+1,j*2+2];
					verts[idx++]=base_verts[i,j*2+1];
				}
			}
			vbC.Unlock();

			idx=0;
			verts = (CustomVertex.PositionTextured[])vbD.Lock(0,0);
			for(int i=0;i<width-1;i++)
			{
				for(int j=0;j<heigh/2-1;j++)
				{
					verts[idx++]=base_verts[i+1,j*2+2];
					verts[idx++]=base_verts[i+1,j*2+1];
					verts[idx++]=base_verts[i,j*2+1];
				}
			}

			vbD.Unlock();
		}

		public float get_alti(float x,float z)
		{
			try
			{
				float temp_z=z;float temp_x=x;

				float[] mx=new float[6];
				float[] my=new float[6];
				float[] m_alti=new float[6];
				float[] m_dist=new float[6];

				z=z/0.866f;

				if(((int)z)%2==0)
				{
					mx[0]=(int)x-1;		my[0]=((int)z)*0.866f;
					mx[1]=(int)x;		my[1]=((int)z)*0.866f;
					mx[2]=(int)x+1;		my[2]=((int)z)*0.866f;
					mx[3]=(int)x-0.5f;	my[3]=((int)z+1)*0.866f;
					mx[4]=(int)x+0.5f;	my[4]=((int)z+1)*0.866f;
					mx[5]=(int)x+1.5f;	my[5]=((int)z+1)*0.866f;
				}
				else
				{
					mx[0]=(int)x-0.5f;	my[0]=((int)z)*0.866f;
					mx[1]=(int)x+0.5f;	my[1]=((int)z)*0.866f;
					mx[2]=(int)x+1.5f;	my[2]=((int)z)*0.866f;
					mx[3]=(int)x-1;		my[3]=((int)z+1)*0.866f;
					mx[4]=(int)x;		my[4]=((int)z+1)*0.866f;
					mx[5]=(int)x+1;		my[5]=((int)z+1)*0.866f;
				}

				for(int i=0;i<6;i++)
				{
					if(mx[i]<0||my[i]<0)
					{
						m_dist[i]=float.MaxValue;
					}
					else
					{
						m_dist[i]=(mx[i]-x)*(mx[i]-x)+(my[i]-temp_z)*(my[i]-temp_z);
						if((int)(my[i]/0.866f)%2==0)
						{
							m_alti[i]=alti[(int)(mx[i]),(int)(my[i]/0.866f)];
						}
						else
						{
							m_alti[i]=alti[(int)(mx[i]-0.5),(int)(my[i]/0.866f)];
						}
					}
				}

				for(int i=0;i<6;i++)
				{
					for(int j=4;j>=0;j--)
					{
						if(m_dist[j]>=m_dist[j+1])
						{
							float t;
							t=m_dist[j];m_dist[j]=m_dist[j+1];m_dist[j+1]=t;
							t=m_alti[j];m_alti[j]=m_alti[j+1];m_alti[j+1]=t;
							t=mx[j];mx[j]=mx[j+1];mx[j+1]=t;
							t=my[j];my[j]=my[j+1];my[j+1]=t;
						}
					}
				}

				float x1=mx[0];float y1=m_alti[0];float z1=my[0];
				float x2=mx[1];float y2=m_alti[1];float z2=my[1];
				float x3=mx[2];float y3=m_alti[2];float z3=my[2];
				float a;float b;float c;

				b = ((y1 - y2) * (x2 - x3) / (x1 - x2) - (y2 - y3)) / ((z1 - z2) * (x2 - x3) / (x1 - x2) - (z2 - z3));
				a = ((y1 - y2) - b * (z1 - z2)) / (x1 - x2);
				c = y1 - a * x1 - b * z1;
				return temp_x*a+temp_z*b+c;
			}
			catch
			{
				return 0;
			}
		}

		override public void Render(Device device)
		{
			device.Transform.World = m;
			material.Ambient = Color.White;
			material.Diffuse = Color.White;
			device.Material = material;
			device.SetTexture(0,texture);
			device.VertexFormat = CustomVertex.PositionTextured.Format;
			device.SetStreamSource(0, vbA, 0);
			device.DrawPrimitives(PrimitiveType.TriangleList,0,(width-1)*heigh/2);
			device.SetStreamSource(0, vbB, 0);
			device.DrawPrimitives(PrimitiveType.TriangleList,0,(width-1)*heigh/2);
			device.SetStreamSource(0, vbC, 0);
			device.DrawPrimitives(PrimitiveType.TriangleList,0,(width-1)*heigh/2);
			device.SetStreamSource(0, vbD, 0);
			device.DrawPrimitives(PrimitiveType.TriangleList,0,(width-1)*heigh/2);
		}
	}

	class Model:D3DObject
	{
		Mesh mesh = null;
		//Material[] meshMaterials;
		Texture[] meshTextures;
		public Model(Device device,string filename):base()
		{
			ExtendedMaterial[] materials = null;
			mesh = Mesh.FromFile(filename, MeshFlags.SystemMemory, device, out materials);
			//mesh = Mesh.LoadHierarchyFromFile(filename, MeshFlags.Managed, device, out materials);
			meshTextures  = new Texture[materials.Length];
			//meshMaterials = new Material[materials.Length];
			for( int i=0; i<materials.Length; i++ )
			{
				//meshMaterials[i] = materials[i].Material3D;
				//meshMaterials[i].Ambient = Color.Gray;//meshMaterials[i].Diffuse;
				//meshMaterials[i].Diffuse = Color.Gray;
				if(materials[i].TextureFilename!=null)
				{
					try
					{
						meshTextures[i] = TextureLoader.FromFile(device, materials[i].TextureFilename);
					}
					catch
					{}
				}
			}
		}
		override public void Render(Device device)
		{
			device.Transform.World = m;
			for( int i=0; i<meshTextures.Length; i++ )
			{
				device.Material = material;
				device.SetTexture(0, meshTextures[i]);
				//device.SetTextureStageState(0,TextureStageStates.BumpEnvironmentMaterial00,0);
				mesh.DrawSubset(i);
			}
		}
	}

	class Line:D3DObject
	{
		public int start=0;
		public int count=0;
		public VertexBuffer vertexBuffer = null;
		public CustomVertex.PositionColored [] verts;
		public Line(int numb,Device device):base()
		{
			vertexBuffer = new VertexBuffer(typeof(CustomVertex.PositionColored),numb, device, 0, CustomVertex.PositionColored.Format, Pool.Default);
			count=numb;
		}
		public void lock_verts()
		{
			verts = (CustomVertex.PositionColored[])vertexBuffer.Lock(0,0);
		}
		public void unlock_verts()
		{
			vertexBuffer.Unlock();
			verts=null;
		}
		override public void Render(Device device)
		{
			device.SetStreamSource(0,vertexBuffer,0);
			device.VertexFormat = CustomVertex.PositionColored.Format;
			device.DrawPrimitives(PrimitiveType.LineList,start,count);
		}
	}
}

