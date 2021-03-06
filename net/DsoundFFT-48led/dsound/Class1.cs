﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.DirectX.DirectSound;
using System.Threading;
using System.Diagnostics;

namespace dsound
{
	public class fft
	{
		static float PI = 3.1415926535897931f;
		static float HALFPI = PI / 2;
		public static Complex ww(int n, int power)
		{

			float u = 2 * PI * power / (float)n;

			float threshold = 0.0000000001f;
			if (Math.Abs(u - 0) < threshold)
				return new Complex(1, 0);
			else if (Math.Abs(u - HALFPI) < threshold)
				return new Complex(0, 1);
			else if (Math.Abs(u - PI) < threshold)
				return new Complex(-1, 0);
			else if (Math.Abs(u - 3 * HALFPI) < threshold)
				return new Complex(0, -1);
			else if (Math.Abs(u - 4 * HALFPI) < threshold)
				return new Complex(1, 0);
			else
				return new Complex((float)Math.Cos(u), (float)Math.Sin(u));
		}

		public static void FFT(int n, int[] a, Complex w, Complex[] V)
		{
			if (n == 1)
			{
				V[0] = new Complex(a[0], 0);
			}
			else
			{
				int[] b;
				Complex[] U = new Complex[n / 2];
				Complex[] W = new Complex[n / 2];
				b = new int[n / 2];
				for (int i = 0; i < n / 2; i++)
					b[i] = a[i * 2];
				FFT(n / 2, b, w.times(w), U);

				for (int i = 0; i < n / 2; i++)
					b[i] = a[i * 2 + 1];
				FFT(n / 2, b, w.times(w), W);

				for (int j = 0; j < n / 2; j++)
				{
					V[j] = U[j].plus(w.power(j).times(W[j]));
					V[j + n / 2] = U[j].minus(w.power(j).times(W[j]));
				}
			}
		}

		public static void IFFT(int n, Complex[] a, Complex w, Complex[] V)
		{
			if (n == 1)
			{
				a[0] = V[0];
			}
			else
			{
				Complex[] b;
				Complex[] B = new Complex[n / 2];
				Complex[] C = new Complex[n / 2];
				b = new Complex[n / 2];
				for (int i = 0; i < n / 2; i++)
					b[i] = V[i * 2];
				IFFT(n / 2, B, w.times(w), b);

				for (int i = 0; i < n / 2; i++)
					b[i] = V[i * 2 + 1];
				IFFT(n / 2, C, w.times(w), b);

				for (int j = 0; j < n / 2; j++)
				{
					a[j] = B[j].plus(w.power(j).times(C[j]));
					a[j + n / 2] = B[j].minus(w.power(j).times(C[j]));
				}
			}
		}
	}

	public class Complex
	{
		private float x, y;
		public Complex(float u, float v)
		{
			x = u;
			y = v;
		}
		public float real()
		{
			return x;
		}
		public float imag()
		{
			return y;
		}
		public float mod()
		{
			if (x != 0 || y != 0)
			{
				return (float)Math.Sqrt(x * x + y * y);
			}
			else
			{
				return 0f;
			}
		}
		public static float atan2(float y, float x)
		{
			float a;

			if (Math.Abs(x) >= Math.Abs(y))
			{
				a = (float)Math.Atan(y / x);
				if (x < 0.0)
				{
					if (y >= 0.0) a += (float)Math.PI;
					else a -= (float)Math.PI;
				}
			}
			else
			{
				a = -(float)Math.Atan(x / y);
				if (y < 0.0) a -= (float)Math.PI / 2;
				else a += (float)Math.PI / 2;
			}
			return a;
		}

		public Complex plus(Complex w)
		{
			return new Complex(x + w.real(), y + w.imag());
		}
		public Complex minus(Complex w)
		{
			return new Complex(x - w.real(), y - w.imag());
		}
		public Complex times(Complex w)
		{
			return new Complex(x * w.real() - y * w.imag(), x * w.imag() + y * w.real());
		}
		public Complex power(int power)
		{
			float r = (float)Math.Pow(this.mod(), power);
			float theta = atan2(y, x) * power;
			return new Complex(r * (float)Math.Cos(theta), r * (float)Math.Sin(theta));
		}
	}

	public class FastFFT
	{
		/*
	运算过程中所有浮点数都乘以256并取整，输入和输出数据也是乘以256之后的整型数据。
	sin和cos采用查表实现，精确到1度。程序适用于对精度要求不高的FFT计算，例如音频播放器的频谱显示等。
	采用更大的取整系数（例如65536）并增加sin、cos表的精度可以提高这个整型FFT计算的精度。
	应用转化成整型计算的FFT需要注意的问题是，整型FFT的动态范围会比较小，这是由定点数的性质决定的，
	因此如果计算对在很大的动态范围内的精度有要求，则整型FFT不适用。*/
		static long[] SIN_TABLE256 = {0, 4, 9, 13, 18, 22, 27, 31, 36, 40,
                          44, 49, 53, 58, 62, 66, 71, 75, 79, 83,
                          88, 92, 96, 100, 104, 108, 112, 116, 120, 124,
                          128, 132, 136, 139, 143, 147, 150, 154, 158, 161,
                          165, 168, 171, 175, 178, 181, 184, 187, 190, 193,
                          196, 199, 202, 204, 207, 210, 212, 215, 217, 219,
                          222, 224, 226, 228, 230, 232, 234, 236, 237, 239,
                          241, 242, 243, 245, 246, 247, 248, 249, 250, 251,
                          252, 253, 254, 254, 255, 255, 255, 256, 256, 256,
                          256};

		static long fastsin256(long degree)
		{
			long result, neg = 0;
			if (degree < 0)
				degree = -degree + 180;
			if (degree >= 360)
				degree %= 360;
			if (degree >= 180)
			{
				degree -= 180;
				neg = 1;
			}
			if ((degree >= 0) && (degree <= 90))
				result = SIN_TABLE256[degree];
			else
				result = SIN_TABLE256[180 - degree];
			if (neg == 0)
				return result;
			else
				return -result;
		}

		static long fastcos256(long degree)
		{
			return fastsin256(degree + 90);
		}

		public static void fft(long[] x, int n)
		{
			int i, j, k, m = 0, i1, i2, i3, i4, n1, n2, n4;
			long a, e, cc, ss, xt, t1, t2;
			for (j = 1, i = 1; i < 16; i++)
			{
				m = i;
				j = (j << 1);
				if (j == n)
					break;
			}
			n1 = n - 1;
			for (j = 0, i = 0; i < n1; i++)
			{
				if (i < j)
				{
					xt = x[j];
					x[j] = x[i];
					x[i] = xt;
				}
				k = (n >> 1);
				while (k < (j + 1))
				{
					j -= k;
					k = (k >> 1);
				}
				j += k;
			}
			for (i = 0; i < n; i += 2)
			{
				xt = x[i];
				x[i] += x[i + 1];
				x[i + 1] = xt - x[i + 1];
			}
			n2 = 1;
			for (k = 2; k <= m; k++)
			{
				n4 = n2;
				n2 = (n4 << 1);
				n1 = (n2 << 1);
				e = 360 / n1;
				for (i = 0; i < n; i += n1)
				{
					xt = x[i];
					x[i] += x[i + n2];
					x[i + n2] = xt - x[i + n2];
					x[i + n2 + n4] = -x[i + n2 + n4];
					a = e;
					for (j = 1; j <= (n4 - 1); j++)
					{
						i1 = i + j;
						i2 = i - j + n2;
						i3 = i + j + n2;
						i4 = i - j + n1;
						cc = fastcos256(a);
						ss = fastsin256(a);
						a += e;
						t1 = cc * x[i3] + ss * x[i4];
						t2 = ss * x[i3] - cc * x[i4];
						t1 = (t1 >> 8);
						t2 = (t2 >> 8);
						x[i4] = x[i2] - t2;
						x[i3] = -x[i2] - t2;
						x[i2] = x[i1] - t1;
						x[i1] = x[i1] + t1;
					}
				}
			}
		}
	}
}
