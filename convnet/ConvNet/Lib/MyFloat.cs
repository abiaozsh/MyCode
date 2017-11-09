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
		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern IntPtr allocfloat(int size);

		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern void freefloat(IntPtr point);

		//extern "C" __declspec(dllexport) cl_mem allocCLMEM(OpenCLBasic* oclobjects, cl_float* point, int size){
		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
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

		public IntPtr ori_p;
		unsafe float* p;
		public int size;
		public IntPtr p_cl_mem = IntPtr.Zero;

		public MyFloat(int len)
		{
			ori_p = allocfloat(len);//totalsize
			unsafe
			{
				p = (float*)ori_p.ToPointer();
			}
			size = len;

			if (OpenCL.oclobjects != IntPtr.Zero)
			{
				p_cl_mem = allocCLMEM(OpenCL.oclobjects, ori_p, size);
			}
		}

		~MyFloat()
		{
			try
			{
				if (OpenCL.oclobjects != IntPtr.Zero && p_cl_mem != IntPtr.Zero)
				{
					freeCLMEM(OpenCL.oclobjects, ori_p, p_cl_mem);
				}
				freefloat(ori_p);
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

		public float this[int idx]
		{
			get
			{
				float v;
				unsafe
				{
					v = p[idx];
				}
				return v;
			}
			set
			{
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


	}
}
