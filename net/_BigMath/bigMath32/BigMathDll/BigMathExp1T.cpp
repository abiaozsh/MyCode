//v1.2

#include <windows.h>
#include <process.h>
 
void add(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent, unsigned int* num_Num_Array, int num_Sign, int num_Exponent);
void mul(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent, unsigned int* num_Num_Array, int num_Sign, int num_Exponent, unsigned int* buf_Num_Array);
void div(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent, unsigned int* num_Num_Array, int num_Sign, int num_Exponent, unsigned int* r_value, unsigned int* b);
void inc(unsigned int* Num_Array, int* pExponent);

unsigned int* clone(int Precision, unsigned int* Num_Array);
unsigned int* newArray(int Precision, unsigned int n);

void Exp(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent)
{
	//2±∂¡Ÿ ±ø’º‰
	unsigned int* r_value = new unsigned int[Precision + 2];
	unsigned int* b = new unsigned int[Precision];

	//double num_1 = 2;
	unsigned int* num_1 = newArray(Precision, 1);
	int Snum_1 = 1;
	int Enum_1 = 1;

	//double mx = x;
	unsigned int* num_mx = clone(Precision, Num_Array);
	int Snum_mx = (*pSign);
	int Enum_mx = (*pExponent);

	//double x = x;
	unsigned int* num_x = clone(Precision, Num_Array);
	int Snum_x = (*pSign);
	int Enum_x = (*pExponent);

	////ex = (1+x)+x2/2!+x3/3!+...+xn/n!+...
	add(Precision, Num_Array, pSign, pExponent, num_1, Snum_1, Enum_1);

	//double mi = 2;
	unsigned int* num_i = newArray(Precision, 1);
	int Snum_i = 1;
	int Enum_i = 1;

	unsigned int* num_mi = newArray(Precision, 1);
	int Snum_mi = 1;
	int Enum_mi = 1;


	unsigned int* num_b = newArray(Precision, 1);
	int Snum_b = 1;
	int Enum_b = 1;

	int limit = Precision;

	while(true)
	{
		//mx *= x;
		mul(limit, num_mx, &Snum_mx, &Enum_mx, num_x, Snum_x, Enum_x, b);

		//i += one;
		inc(num_i, &Enum_i);
		//mi *= i;
		mul(limit, num_mi, &Snum_mi, &Enum_mi, num_i, Snum_i, Enum_i, b);

		//b = mx;
		for(int idx = 0; idx < Precision; idx++)
		{
			num_b[idx] = num_mx[idx];
		}
		Snum_b = Snum_mx;
		Enum_b = Enum_mx;

		//b /= mi;
		div(limit, num_b, &Snum_b, &Enum_b, num_mi, Snum_mi, Enum_mi, r_value, b);

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
	delete num_i;
	delete num_mi;
	delete num_b;
}

extern "C" __declspec(dllexport) void N_Exp(int* PSE,unsigned int* Num_Array)
{
	int Precision = PSE[0];
	int Sign = PSE[1];
	int Exponent = PSE[2];

	Exp(Precision, Num_Array, &Sign, &Exponent);

	PSE[1] = Sign;
	PSE[2] = Exponent;
}



/*
		//e^(a+b) = e^a*e^b
		public double E(double x, double e)
		{
			double adj = 1;
			double one = 1;
			//double minusone = -1;
			//while (x < -e)
			//{
			//	x += one;
			//	adj /= e;
			//}
			//while (x > e)
			//{
			//	x += minusone;
			//	adj *= e;
			//}

			double ret = one;

			//ex = (1+x)+x2/2!+x3/3!+...+xn/n!+...

			double mx = x;
			double mi = one;

			double b;
			double i = 2;
			for (int idx = 0; idx < 25; idx++)
			{
				b = mx;
				b /= mi;
				ret += b;
				mx *= x;
				mi *= i;
				i += one;

				textBox1.Text += i + "," + ret * adj + ",\r\n";
			}
			return ret * adj;
		}
*/