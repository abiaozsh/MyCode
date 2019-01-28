using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WindowsFormsApplication1
{
	public struct Complex
	{
		public double a;
		public double b;
		public Complex(double a, double b)
		{
			this.a = a;
			this.b = b;
		}
		public static implicit operator Complex(double r)
		{
			Complex ret;
			ret.a = r;
			ret.b = 0;
			return ret;
		}
		public static Complex operator +(Complex z1, Complex z2)
		{
			Complex ret;
			ret.a = z1.a + z2.a;
			ret.b = z1.b + z2.b;
			return ret;
		}
		public static Complex operator -(Complex z1, Complex z2)
		{
			Complex ret;
			ret.a = z1.a - z2.a;
			ret.b = z1.b - z2.b;
			return ret;
		}
		public static Complex operator *(Complex z1, Complex z2)
		{
			Complex ret;
			//(a1+b1i)*(a2+b2i)
			//a1a2-b1b2+a1b2i+b1a2i
			ret.a = z1.a * z2.a - z1.b * z2.b;
			ret.b = z1.a * z2.b + z2.a * z1.b;
			return ret;
		}
		public static Complex operator /(Complex z1, Complex z2)
		{
			Complex ret;
			//(a1+b1i)/(a2+b2i)
			//(a1+b1i)(a2-b2i)/(a2+b2i)(a2-b2i)
			//(a1a2+b1b2+a2b1i-a1b2i)/(a2a2+b2b2)
			double n = (z2.a * z2.a + z2.b * z2.b);
			if (n == 0)
			{
				n = 0.001;
			}
			else
			{
				n = 1 / n;
			}
			ret.a = (z1.a * z2.a + z1.b * z2.b) * n;
			ret.b = (z2.a * z1.b - z1.a * z2.b) * n;
			return ret;
		}
		public static Complex operator ^(Complex z, Complex a)
		{
			//w=z^a=e^(alnz)
			return e(a * log(z));
		}
		public static Complex e(Complex z)
		{
			//xout = 0;
			//yout = 0;
			//e^z=e^(x+iy)=e^x(cosy+isiny)
			//
			Complex ret;
			double e = Math.Exp(z.a);

			ret.a = Math.Cos(z.b) * e;
			ret.b = Math.Sin(z.b) * e;
			return ret;
		}
		public static Complex sin(Complex z)
		{
			Complex ret;
			//sinz=[e^(iz)-e^(-iz)]/(2i)
			//=[e^(-b+ia)-e^(b-ia)]/(2i)
			//=[e^(-b)(cosa+isina)-e^b(cosa-isina)]/(2i)
			//=[(e^(-b)-e^b)cosa+i(e^(-b)sina+e^bsina)]/(2i)
			//=(-2i)[(e^(-b)-e^b)cosa+i(e^(-b)sina+e^bsina)]/(2i*-2i)
			//=(-i)[(e^(-b)-e^b)cosa+i(e^(-b)sina+e^bsina)]/2
			//=-(e^(-b)-e^b)cosa/2*i+(e^(-b)sina+e^bsina)/2
			//=(e^(-b)sina+e^bsina)/2-(e^(-b)-e^b)cosa/2*i
			double e1 = Math.Exp(z.b);
			double e2 = Math.Exp(-z.b);

			//(e^(-b)sina+e^bsina)/2
			ret.a = (e2 + e1) * Math.Sin(z.a) / 2;
			//-(e^(-b)-e^b)cosa/2
			ret.b = -(e2 - e1) * Math.Cos(z.a) / 2;
			return ret;
		}
		public static Complex log(Complex z)
		{
			//xout = 0;
			//yout = 0;
			//e^z=e^(x+iy)=e^x(cosy+isiny)
			//

			Complex ret;
			ret.a = Math.Log(Math.Sqrt(z.a * z.a + z.b * z.b));
			ret.b = Math.Atan2(z.b, z.a);
			return ret;
		}
		public static int getColor(Complex c)
		{
			double x = c.a;
			double y = c.b;
            double h = (Math.Atan2(y, x) + (Math.PI)) / (Math.PI) * 3;
			double d = Math.Sqrt(x * x + y * y);
			double v = Math.Atan(d) / Math.PI * 2;
			double s;
			if (d < 1)
			{
				s = 1;
			}
			else
			{
				double l = Math.Log(d);
				s = (3 + (l - Math.Floor(l))) / 4;
			}

			double r = 0.0;
			double g = 0.0;
			double b = 0.0;

			int flag = (int)Math.Abs(h);

			double f = h - flag;

			double p = v * (1 - s);

			double q = v * (1 - f * s);

			double t = v * (1 - (1 - f) * s);

			switch (flag)//公式算法
			{
				case 0:
					b = p;
					g = t;
					r = v;
					break;
				case 1:
					b = p;
					g = v;
					r = q;
					break;
				case 2:
					b = t;
					g = v;
					r = p;
					break;
				case 3:
					b = v;
					g = q;
					r = p;
					break;
				case 4:
					b = v;
					g = p;
					r = t;
					break;
				case 5:
					b = q;
					g = p;
					r = v;
					break;
				default:
					b = p;
					g = t;
					r = v;
					break;
			}

			int B = (int)(b * 256.0);
			int G = (int)(g * 256.0);
			int R = (int)(r * 256.0);


			if (R > 255) R = 255;
			if (G > 255) G = 255;
			if (B > 255) B = 255;
			if (R < 0) R = 0;
			if (G < 0) G = 0;
			if (B < 0) B = 0;
			return (255 << 24) |
				(R << 16) |
				(G << 8) |
				(B);
		}
	}
}
