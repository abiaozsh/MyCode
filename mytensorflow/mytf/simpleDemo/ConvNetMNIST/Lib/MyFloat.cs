using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace ConvNet
{
	public class MyFloat : Persistence
	{
		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern IntPtr allocfloat(int size);

		[DllImport("dllLib.dll", CallingConvention = CallingConvention.Cdecl)]
		static extern void freefloat(IntPtr point);


		public IntPtr p;
		private unsafe float* _p;
		public int size;

		public float this[int idx]
		{
			get
			{
				
				if (idx >= size || idx < 0)
				{
					throw new Exception();
				}

				float v;
				unsafe
				{
					v = _p[idx];
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
					_p[idx] = value;
				}
			}
		}

		public MyFloat(int size)
		{
			p = allocfloat(size);//totalsize
			unsafe
			{
				_p = (float*)p.ToPointer();
				//for (int i = 0; i < size; i++)
				//{
				//	p[i] = 0.0f;
				//}
			}
			this.size = size;
		}

		~MyFloat()
		{
			freefloat(p);
		}


		//public void save(TextWriter s)
		//{
		//	for (int i = 0; i < size; i++)
		//	{
		//		s.WriteLine(this[i]);
		//	}
		//}
		public void load(TextReader s)
		{
			for (int i = 0; i < size; i++)
			{
				string v = s.ReadLine();
				this[i] = float.Parse(v);
			}
		}

		public void load(BinaryReader s)
		{
			for (int i = 0; i < size; i++)
			{
				this[i] = s.ReadSingle();
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
