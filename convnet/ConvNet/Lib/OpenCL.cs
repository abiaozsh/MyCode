using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace Lib
{
	public class OpenCL
	{
		public static IntPtr oclobjects = IntPtr.Zero;

		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern IntPtr _getPlatforms();
		public static string getPlatforms()
		{
			IntPtr ip = _getPlatforms();
			StringBuilder sb = new StringBuilder();
			unsafe
			{
				byte* p;
				p = (byte*)ip.ToPointer();

				for (int i = 0; i < 4096; i++)
				{
					if (p[i] == 0) break;
					sb.Append((char)p[i]);
				}
			}
			return sb.ToString();
		}


		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern IntPtr _getDevices(int platform);
		public static string getDevices(int platform)
		{
			IntPtr ip = _getDevices(platform);
			StringBuilder sb = new StringBuilder();
			unsafe
			{
				byte* p;
				p = (byte*)ip.ToPointer();

				for (int i = 0; i < 4096; i++)
				{
					if (p[i] == 0) break;
					sb.Append((char)p[i]);
				}
			}
			return sb.ToString();
		}



		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern IntPtr init(int platform, int device);

		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern IntPtr getKernel(IntPtr oclobjects, byte[] filename, byte[] skernel);

		//[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		//private static extern IntPtr getKernel(IntPtr oclobjects, byte[] code, byte[] skernel);

		public static IntPtr getKernel(IntPtr oclobjects, string filename, string kernel)
		{
			return getKernel(oclobjects, Encoding.Unicode.GetBytes(filename), Encoding.ASCII.GetBytes(kernel));
		}

		//extern "C" __declspec(dllexport) void runKernel(OpenCLBasic* oclobjects, cl_kernel kernel, int threads, cl_mem p_cl_mem1, cl_mem p_cl_mem2,int param1,float param2)
		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		//public static extern void runKernel(IntPtr kernel, int threads, int _params, int[] param);
		public static extern int runKernel(IntPtr oclobjects, IntPtr kernel, int threads, IntPtr p_cl_mem1, IntPtr p_cl_mem2, int param1, float param2);

		//init(0, 1);
		//init(1, 0);


	}
}
