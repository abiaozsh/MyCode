using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace ConvNet
{
	public class MyFloat
	{
		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern IntPtr allocfloat();//int param

		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern IntPtr allocfloat2();//int param


		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern int getArraySize();

		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern IntPtr init(int platform, int device);


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
		public static extern IntPtr getKernel(byte[] filename, byte[] skernel);

		public static IntPtr getKernel(string filename, string kernel)
		{
			return getKernel(Encoding.Unicode.GetBytes(filename), Encoding.ASCII.GetBytes(kernel));
		}

		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern void runKernel(IntPtr kernel, int threads, int _params, int[] param);


		//static int totalsize = 1024 * 1024;
		static unsafe float* p;
		public static int idx = 0;

		public int pos;
		public int size;

		public static void Init()
		{
			IntPtr f = allocfloat();//totalsize
			unsafe
			{
				p = (float*)f.ToPointer();
			}

			//init(0, 1);
			//init(1, 0);
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
		public static MyFloat getArray(int size)
		{
			if (idx + size > getArraySize()) return null;
			MyFloat ret = new MyFloat();
			ret.size = size;
			ret.pos = idx;
			idx += size;
			return ret;
		}

		public float this[int idx]
		{
			get
			{
				float v;
				unsafe
				{
					v = p[pos + idx];
				}
				return v;
			}
			set
			{
				unsafe
				{
					p[pos + idx] = value;
				}
			}
		}
	}
}
