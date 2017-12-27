//v1.2

#include <windows.h>
#include <process.h>

#define int __int64

extern "C" void add(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent, unsigned int* num_Num_Array, int num_Sign, int num_Exponent);
extern "C" void mul(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent, unsigned int* num_Num_Array, int num_Sign, int num_Exponent, unsigned int* buf_Num_Array);
extern "C" void divi(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent, unsigned int* num_Num_Array, int num_Sign, int num_Exponent, unsigned int* r, unsigned int* b);
extern "C" void incr(unsigned int* Num_Array, int* pExponent);

void sin2T(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent);
void sin4T(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent);

unsigned int* clone(int Precision, unsigned int* Num_Array)
{
	
	unsigned int* ret = new unsigned int[Precision];
	for(int i=0;i<Precision;i++)
	{
		ret[i] = Num_Array[i];
	}
	return ret;
}

unsigned int* newArray(int Precision, unsigned int n)
{
	unsigned int* ret = new unsigned int[Precision];
	ret[0] = n;
	for(int i=1;i<Precision;i++)
	{
		ret[i] = 0;
	}
	return ret;
}

void sin1T(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent)
{
	//TODO regular by pi

	//2±¶ÁÙÊ±¿Õ¼ä
	unsigned int* r_value = new unsigned int[Precision + 2];
	unsigned int* b = new unsigned int[Precision];

	unsigned int* num = clone(Precision, Num_Array);
	int Snum = (*pSign);
	int Enum = (*pExponent);

	unsigned int* num_i = newArray(Precision, 1);
	int Snum_i = 1;
	int Enum_i = 1;

	unsigned int* num_a = clone(Precision, Num_Array);
	int Snum_a = (*pSign);
	int Enum_a = (*pExponent);

	unsigned int* num_b = newArray(Precision, 1);
	int Snum_b = 1;
	int Enum_b = 1;

	unsigned int* num_x2 = clone(Precision, Num_Array);
	int Snum_x2 = (*pSign);
	int Enum_x2 = (*pExponent);

	unsigned int* num2 = newArray(Precision, 1);
	int Snum2 = 1;
	int Enum2 = 1;

	int limit = Precision;

	//x2=x*x
	mul(limit, num_x2, &Snum_x2, &Enum_x2, num, Snum, Enum, b);

	for (int i = 1 ; ; i++)
	{
		//a=a*x2
		mul(limit, num_a, &Snum_a, &Enum_a, num_x2, Snum_x2, Enum_x2, b);

		//b=b*(i)*(i+1)
		//i++
		incr(num_i, &Enum_i);
		//b=b*i
		mul(limit, num_b, &Snum_b, &Enum_b, num_i, Snum_i, Enum_i, b);
		//i++
		incr(num_i, &Enum_i);
		//b=b*i
		mul(limit, num_b, &Snum_b, &Enum_b, num_i, Snum_i, Enum_i, b);

		for(int idx = 0; idx < Precision; idx++)
		{
			num2[idx] = num_a[idx];
		}
		Snum2 = Snum_a;
		Enum2 = Enum_a;

		//num2=a/b
		divi(limit, num2, &Snum2, &Enum2, num_b, Snum_b, Enum_b, r_value, b);

		if (i % 2 == 1)
		{
			Snum2 = -Snum2;
		}
		//result+=num2
		add(Precision, Num_Array, pSign, pExponent, num2, Snum2, Enum2);

		limit = Precision + Enum2;

		if (limit < 2)
		{
			break;
		}

		if ((*pExponent) - Enum2 > (Precision - 2))
		{
			break;
		}

		if(limit > Precision) limit = Precision;
		if(limit <= 0) limit = 1;
	}

	delete r_value;
	delete b;

	delete num;

	delete num_i;

	delete num_a;

	delete num_b;

	delete num_x2;

	delete num2;
}

extern "C" __declspec(dllexport) void N_Sin(int* PSE,unsigned int* Num_Array,int threads)
{
	int Precision = PSE[0];
	int Sign = PSE[1];
	int Exponent = PSE[2];

	if(threads==4)
	{
		sin4T( Precision, Num_Array, &Sign, &Exponent);
	}
	if(threads==2)
	{
		sin2T( Precision, Num_Array, &Sign, &Exponent);
	}
	else
	{
		sin1T( Precision, Num_Array, &Sign, &Exponent);
	}

	PSE[1] = Sign;
	PSE[2] = Exponent;
}