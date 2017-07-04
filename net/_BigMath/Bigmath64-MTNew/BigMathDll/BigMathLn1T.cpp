//v1.2

#include <windows.h>
#include <process.h>

#define int __int64

extern "C" void add(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent, unsigned int* num_Num_Array, int num_Sign, int num_Exponent);
extern "C" void mul(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent, unsigned int* num_Num_Array, int num_Sign, int num_Exponent, unsigned int* buf_Num_Array);
extern "C" void divi(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent, unsigned int* num_Num_Array, int num_Sign, int num_Exponent, unsigned int* r, unsigned int* b);
extern "C" void incr(unsigned int* Num_Array, int* pExponent);

unsigned int* clone(int Precision, unsigned int* Num_Array);
unsigned int* newArray(int Precision, unsigned int n);

void Ln(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent)
{
	//TODO regular by pi

	//2±∂¡Ÿ ±ø’º‰
	unsigned int* r_value = new unsigned int[Precision + 2];
	unsigned int* b = new unsigned int[Precision];

	//double mx = x;
	unsigned int* num_mx = clone(Precision, Num_Array);
	int Snum_mx = (*pSign);
	int Enum_mx = (*pExponent);

	//double x = x;
	unsigned int* num_x = clone(Precision, Num_Array);
	int Snum_x = (*pSign);
	int Enum_x = (*pExponent);

	//double mi = 1;
	unsigned int* num_mi = newArray(Precision, 1);
	int Snum_mi = 1;
	int Enum_mi = 1;

	//b
	unsigned int* num_b = newArray(Precision, 1);
	int Snum_b = 1;
	int Enum_b = 1;

	int limit = Precision;

	//ret = x
	while(true)
	{
		//mx *= x;
		mul(limit, num_mx, &Snum_mx, &Enum_mx, num_x, Snum_x, Enum_x, b);

		//mi += one;
		incr(num_mi, &Enum_mi);

		//b = mx;
		for(int idx = 0; idx < Precision; idx++)
		{
			num_b[idx] = num_mx[idx];
		}
		Snum_b = Snum_mx;
		Enum_b = Enum_mx;


		//b /= mi;
		divi(limit, num_b, &Snum_b, &Enum_b, num_mi, Snum_mi, Enum_mi, r_value, b);

		//ret -= b;
		Snum_b = -Snum_b;
		add(Precision, Num_Array, pSign, pExponent, num_b, Snum_b, Enum_b);

				
		//mx *= x;
		mul(limit, num_mx, &Snum_mx, &Enum_mx, num_x, Snum_x, Enum_x, b);

		//mi += one;
		incr(num_mi, &Enum_mi);

		//b = mx;
		for(int idx = 0; idx < Precision; idx++)
		{
			num_b[idx] = num_mx[idx];
		}
		Snum_b = Snum_mx;
		Enum_b = Enum_mx;

		//b /= mi;
		divi(limit, num_b, &Snum_b, &Enum_b, num_mi, Snum_mi, Enum_mi, r_value, b);

		//ret += b;
		add(Precision, Num_Array, pSign, pExponent, num_b, Snum_b, Enum_b);

		if ((*pExponent) - Enum_b > (Precision - 2))
		{
			break;
		}

		limit = Precision - (*pExponent - Enum_b);

		if(limit > Precision) limit = Precision;
		if(limit <= 0) limit = 1;
	}

	delete r_value;
	delete b;

	delete num_mx;

	delete num_x;

	delete num_mi;

	delete num_b;

}

extern "C" __declspec(dllexport) void N_Ln(int* PSE,unsigned int* Num_Array)
{
	int Precision = PSE[0];
	int Sign = PSE[1];
	int Exponent = PSE[2];

	Ln(Precision, Num_Array, &Sign, &Exponent);

	PSE[1] = Sign;
	PSE[2] = Exponent;
}



/*
		//ln(a*b) = ln(a)+ln(b)
		public double ln(double x, double e)
		{
			double adj = 0;
			if (x <= 0) return 0;
			double one = 1;
			while (x < 0.5)
			{
				x *= e;
				adj--;
			}
			while (x > 1.5)
			{
				x /= e;
				adj++;
			}

			x -= one;
			//-1 < x < 1
			//-1^(n+1) x^n / n  (n=1)
			double mx = x;
			double mi = 1;
			double a;
			double ret = 0;
			for (int i = 2; i < 25; i++)
			{
				//+x^1/1
				a = mx;
				a /= mi;
				ret += a;
				mx *= x;
				mi += one;
				//-x^2/2
				a = mx;
				a /= mi;
				ret -= a;
				mx *= x;
				mi += one;

				textBox1.Text += i + "," + (ret + adj) + ",\r\n";
			}
			//ln(1+x)= x-x2/3+x3/3-...(-1)k-1*xk/k+... (|x|&lt;1)

			//ln(1.5)=(0.5) - 
			//ln(0.5)=(-0.5)


			return ret + adj;
		}
*/