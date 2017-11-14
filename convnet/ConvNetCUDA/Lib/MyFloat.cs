using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using Lib;

namespace ConvNet
{
	public class MyFloat : Persistence
	{
		[DllImport("CPU2Lib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern IntPtr allocfloat(int size);

		[DllImport("CPU2Lib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern void freefloat(IntPtr point);


		[DllImport("cuda2Lib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern IntPtr allocDeviceMemory(int size);

		[DllImport("cuda2Lib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern void freeDeviceMemory(IntPtr point);

		[DllImport("cuda2Lib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern int memcpyHostToDevice(IntPtr hostPoint, IntPtr devicePoint, uint size);

		[DllImport("cuda2Lib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern int memcpyDeviceToHost(IntPtr devicePoint, IntPtr hostPoint, uint size);


		private IntPtr HostMemPoint;
		private IntPtr DeviceMemPoint = IntPtr.Zero;
		private unsafe float* p;
		public int size;

		bool needCopyToDevice = true;
		bool needCopyToHost = false;

		public IntPtr getHostMemPointReadOnly()
		{
			if (needCopyToHost)
			{
				memcpyDeviceToHost(DeviceMemPoint, HostMemPoint, (uint)size);
				needCopyToHost = false;
			}
			return HostMemPoint;
		}
		public IntPtr getHostMemPointReadWrite()
		{
			needCopyToDevice = true;
			if (needCopyToHost)
			{
				memcpyDeviceToHost(DeviceMemPoint, HostMemPoint, (uint)size);
				needCopyToHost = false;
			}
			return HostMemPoint;
		}
		public IntPtr getHostMemPointWriteOnly()
		{
			needCopyToDevice = true;
			needCopyToHost = false;
			return HostMemPoint;
		}


		//设备只读，不需要回拷
		public IntPtr getDeviceMemPointReadOnly()
		{
			if (needCopyToDevice)
			{
				memcpyHostToDevice(HostMemPoint, DeviceMemPoint, (uint)size);
				needCopyToDevice = false;
			}
			return DeviceMemPoint;
		}

		//设备读写，用值时需要读回
		public IntPtr getDeviceMemPointReadWrite()
		{
			needCopyToHost = true;
			if (needCopyToDevice)
			{
				memcpyHostToDevice(HostMemPoint, DeviceMemPoint, (uint)size);
				needCopyToDevice = false;
			}
			return DeviceMemPoint;
		}
		//设备只写,
		public IntPtr getDeviceMemPointWriteOnly()
		{
			needCopyToHost = true;
			needCopyToDevice = false;
			return DeviceMemPoint;
		}

		public float this[int idx]
		{
			get
			{
				//用值时需要读回
				if (needCopyToHost)
				{
					memcpyDeviceToHost(DeviceMemPoint, HostMemPoint, (uint)size);
					needCopyToHost = false;
				}

				if (idx >= size || idx < 0)
				{
					throw new Exception();
				}

				float v;
				unsafe
				{
					v = p[idx];
				}
				return v;
			}
			set
			{
				//脏数据
				needCopyToDevice = true;
				if (idx >= size || idx < 0)
				{
					throw new Exception();
				}
				unsafe
				{
					p[idx] = value;
				}
			}
		}

		public MyFloat(int size)
		{
			HostMemPoint = allocfloat(size);//totalsize
			unsafe
			{
				p = (float*)HostMemPoint.ToPointer();
				for (int i = 0; i < size; i++)
				{
					p[i] = 0.0f;
				}
			}
			this.size = size;
			//DeviceMemPoint = allocDeviceMemory(size);
		}

		~MyFloat()
		{
			//try
			//{
				//freeDeviceMemory(DeviceMemPoint);
				freefloat(HostMemPoint);
			//}
			//catch
			//{
			//	Util.log("err");
			//}
		}


		public void save(TextWriter s)
		{
			for (int i = 0; i < size; i++)
			{
				s.WriteLine(this[i]);
			}
		}
		public void load(TextReader s)
		{
			for (int i = 0; i < size; i++)
			{
				string v = s.ReadLine();
				this[i] = float.Parse(v);
			}
		}


		public override string ToString()
		{
			StringBuilder sb = new StringBuilder();
			for (int i = 0; i < size; i++)
			{
				sb.Append("[" + i + ":" + this[i] + "]\r\n");
			}
			return sb.ToString();
		}

	}
}
