#define CUDA


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
#if CUDA
		[DllImport("cuda2Lib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern IntPtr allocfloat(int size);

		[DllImport("cuda2Lib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern void freefloat(IntPtr point);


		[DllImport("cuda2Lib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern IntPtr allocDeviceMemory(int size);


		[DllImport("cuda2Lib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern void freeDeviceMemory(IntPtr point);


		[DllImport("cuda2Lib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern int memcpyHostToDevice(IntPtr hostPoint, IntPtr devicePoint, uint size);


		[DllImport("cuda2Lib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern int memcpyDeviceToHost(IntPtr devicePoint, IntPtr hostPoint, uint size);




#elif OpenCL
		//extern "C" __declspec(dllexport) cl_mem allocCLMEM(OpenCLBasic* oclobjects, cl_float* point, int size){
		[DllImport("cuda2Lib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern IntPtr allocCLMEM(IntPtr oclobjects, IntPtr point, int size);

		//extern "C" __declspec(dllexport) void freeCLMEM(OpenCLBasic* oclobjects, cl_float* point, cl_mem p_cl_mem)
		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern void freeCLMEM(IntPtr oclobjects, IntPtr point, IntPtr p_cl_mem);

		//extern "C" __declspec(dllexport) void copyToCLMEM(OpenCLBasic* oclobjects, const cl_float* point, cl_mem p_cl_mem, int size)
		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern void copyToCLMEM(IntPtr oclobjects, IntPtr point, IntPtr p_cl_mem, int size);

		//extern "C" __declspec(dllexport) void copyFromCLMEM(OpenCLBasic* oclobjects, cl_float* point, cl_mem p_cl_mem, int size)
		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern void copyFromCLMEM(IntPtr oclobjects, IntPtr point, IntPtr p_cl_mem, int size);
		public void copyToCLMEM()
		{
			if (OpenCL.oclobjects != IntPtr.Zero && p_cl_mem != IntPtr.Zero)
			{
				copyToCLMEM(OpenCL.oclobjects, ori_p, p_cl_mem, size);
			}
		}
		public void copyFromCLMEM()
		{
			if (OpenCL.oclobjects != IntPtr.Zero && p_cl_mem != IntPtr.Zero)
			{
				copyFromCLMEM(OpenCL.oclobjects, ori_p, p_cl_mem, size);
			}
		}

#else
		[DllImport("cpuLib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern IntPtr allocfloat(int size);

		[DllImport("cpuLib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern void freefloat(IntPtr point);

		static IntPtr allocDeviceMemory(int size)
		{
			return IntPtr.Zero;
		}
		static void freeDeviceMemory(IntPtr point) { }
		static int memcpyHostToDevice(IntPtr hostPoint, IntPtr devicePoint, uint size) { return 0; }
		static int memcpyDeviceToHost(IntPtr devicePoint, IntPtr hostPoint, uint size) { return 0; }
#endif




		//public void copyToDeviceMEM()
		//{
		//	memcpyHostToDevice(HostMemPoint, DeviceMemPoint, (uint)size);
		//}
		//public void copyFromDeviceMEM()
		//{
		//	memcpyDeviceToHost(DeviceMemPoint, HostMemPoint, (uint)size);
		//}

		//public static void test()
		//{
		//	IntPtr[] ps = new IntPtr[100000];
		//
		//	for (int i = 0; i < 100000; i++)
		//	{
		//		int len = 4096;
		//		ps[i] = allocfloat(len);
		//	}
		//
		//	for (int i = 0; i < 100000; i++)
		//	{
		//		freefloat(ps[i]);
		//	}
		//}

		private IntPtr HostMemPoint;
		private unsafe float* p;
		public int size;
		private IntPtr DeviceMemPoint = IntPtr.Zero;

		private bool needCopyToDevice = true;
		private bool needCopyToHost = false;

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
			if (needCopyToDevice) {
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

		public MyFloat(int len)
		{
			HostMemPoint = allocfloat(len);//totalsize
			unsafe
			{
				p = (float*)HostMemPoint.ToPointer();
			}
			size = len;

			DeviceMemPoint = allocDeviceMemory(size);
		}

		~MyFloat()
		{
			try
			{
				freeDeviceMemory(DeviceMemPoint);
				freefloat(HostMemPoint);
			}
			catch
			{
				Util.log("err");
			}
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
