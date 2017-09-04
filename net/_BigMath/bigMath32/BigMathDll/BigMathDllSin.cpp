//v1.2

#include <windows.h>
#include <process.h>

void add(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent, unsigned int* num_Num_Array, int num_Sign, int num_Exponent);
void divn(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent, unsigned int n);
void mul(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent, unsigned int* num_Num_Array, int num_Sign, int num_Exponent, unsigned int* buf_Num_Array);
void div(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent, unsigned int* num_Num_Array, int num_Sign, int num_Exponent, unsigned int* r_value, unsigned int* b);
void inc(unsigned int* Num_Array, int* pExponent);


unsigned int* clone(int Precision, unsigned int* Num_Array)
{
	int a1=0;
	int a2=1;
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

struct ArgList
{
	CRITICAL_SECTION cs1;
	int flag;

	int Precision;
	unsigned int* Num_Array;
	int Sign;
	int Exponent;

	unsigned int* r;
	unsigned int* b;

	unsigned int* num;
	int Snum;
	int Enum;

	unsigned int* num_i;
	int Snum_i;
	int Enum_i;

	unsigned int* num_a;
	int Snum_a;
	int Enum_a;

	unsigned int* num_b;
	int Snum_b;
	int Enum_b;

	unsigned int* num_x4;
	int Snum_x4;
	int Enum_x4;

	unsigned int* num2;
	int Snum2;
	int Enum2;
};


void sin_part1(LPVOID pParam)
{
	ArgList* argList = (ArgList*)pParam;
	EnterCriticalSection(&(argList->cs1));

	int limit = argList->Precision;

	//x4=x*x*x*x
	mul(limit, argList->num_x4, &argList->Snum_x4, &argList->Enum_x4, argList->num, argList->Snum, argList->Enum, argList->b);
	mul(limit, argList->num_x4, &argList->Snum_x4, &argList->Enum_x4, argList->num, argList->Snum, argList->Enum, argList->b);
	mul(limit, argList->num_x4, &argList->Snum_x4, &argList->Enum_x4, argList->num, argList->Snum, argList->Enum, argList->b);

	for (int i = 2 ; ; i+=2)
	{
		for(int idx = 0; idx < argList->Precision; idx++)
		{
			argList->num2[idx] = argList->num_a[idx];
		}
		argList->Snum2 = argList->Snum_a;
		argList->Enum2 = argList->Enum_a;

		//num2=a/b
		div(limit, argList->num2, &argList->Snum2, &argList->Enum2, argList->num_b, argList->Snum_b, argList->Enum_b, argList->r, argList->b);

		if ((argList->Exponent) - argList->Enum2 > (argList->Precision - 2))
		{
			break;
		}

		//result+=num2
		add(argList->Precision, argList->Num_Array, &argList->Sign, &argList->Exponent, argList->num2, argList->Snum2, argList->Enum2);

		limit = argList->Precision - (argList->Exponent - argList->Enum2);

		if(limit > argList->Precision) limit = argList->Precision;
		if(limit <= 0) limit = 1;

		//a=a*x4
		mul(limit, argList->num_a, &argList->Snum_a, &argList->Enum_a, argList->num_x4, argList->Snum_x4, argList->Enum_x4, argList->b);

		//b=b*(i)*(i+1)*(i+2)*(i+3)
		//i++
		inc(argList->num_i, &argList->Enum_i);
		//b=b*i
		mul(limit, argList->num_b, &argList->Snum_b, &argList->Enum_b, argList->num_i, argList->Snum_i, argList->Enum_i, argList->b);
		//i++
		inc(argList->num_i, &argList->Enum_i);
		//b=b*i
		mul(limit, argList->num_b, &argList->Snum_b, &argList->Enum_b, argList->num_i, argList->Snum_i, argList->Enum_i, argList->b);
		//i++
		inc(argList->num_i, &argList->Enum_i);
		//b=b*i
		mul(limit, argList->num_b, &argList->Snum_b, &argList->Enum_b, argList->num_i, argList->Snum_i, argList->Enum_i, argList->b);
		//i++
		inc(argList->num_i, &argList->Enum_i);
		//b=b*i
		mul(limit, argList->num_b, &argList->Snum_b, &argList->Enum_b, argList->num_i, argList->Snum_i, argList->Enum_i, argList->b);

	}

	argList->flag = 1;

	LeaveCriticalSection(&(argList->cs1));
	return;
}

void sinMT(unsigned int* Num_Array, int Precision, int* pSign, int* pExponent)
{
	//TODO regular by pi

	ArgList* argList = new ArgList();
	InitializeCriticalSection(&(argList->cs1));
	{
		argList->flag = 0;

		argList->r = new unsigned int[Precision + 1];
		argList->b = new unsigned int[Precision];
		argList->Precision = Precision;

		argList->Num_Array = newArray(Precision, 0);
		argList->Sign = 1;
		argList->Exponent = 0;

		argList->num = clone(Precision, Num_Array);
		argList->Snum = (*pSign);
		argList->Enum = (*pExponent);

		argList->num_i = newArray(Precision, 3);
		argList->Snum_i = 1;
		argList->Enum_i = 1;

		//init 3*x
		argList->num_a = clone(Precision, Num_Array);
		argList->Snum_a = (*pSign);
		argList->Enum_a = (*pExponent);
		mul(Precision, argList->num_a, &argList->Snum_a, &argList->Enum_a, Num_Array, *pSign, *pExponent, argList->b);
		mul(Precision, argList->num_a, &argList->Snum_a, &argList->Enum_a, Num_Array, *pSign, *pExponent, argList->b);

		//3!
		argList->num_b = newArray(Precision, 6);
		argList->Snum_b = 1;
		argList->Enum_b = 1;

		argList->num_x4 = clone(Precision, Num_Array);
		argList->Snum_x4 = (*pSign);
		argList->Enum_x4 = (*pExponent);

		argList->num2 = new unsigned int[Precision];
		argList->Snum2 = 1;
		argList->Enum2 = 1;
	}

	_beginthread(sin_part1, 0, argList);

	//2倍临时空间
	unsigned int* r = new unsigned int[Precision + 1];
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

	unsigned int* num_x4 = clone(Precision, Num_Array);
	int Snum_x4 = (*pSign);
	int Enum_x4 = (*pExponent);

	unsigned int* num2 = new unsigned int[Precision];
	int Snum2 = 1;
	int Enum2 = 1;

	int limit = Precision;
	//x4=x*x*x*x
	mul(limit, num_x4, &Snum_x4, &Enum_x4, Num_Array, *pSign, *pExponent, b);
	mul(limit, num_x4, &Snum_x4, &Enum_x4, Num_Array, *pSign, *pExponent, b);
	mul(limit, num_x4, &Snum_x4, &Enum_x4, Num_Array, *pSign, *pExponent, b);
	for (int i = 1 ; ; i+=2)
	{
		//a=a*x4
		mul(limit, num_a, &Snum_a, &Enum_a, num_x4, Snum_x4, Enum_x4, b);

		//b=b*(i)*(i+1)*(i+2)*(i+3)
		//i++
		inc(num_i, &Enum_i);
		//b=b*i
		mul(limit, num_b, &Snum_b, &Enum_b, num_i, Snum_i, Enum_i, b);
		//i++
		inc(num_i, &Enum_i);
		//b=b*i
		mul(limit, num_b, &Snum_b, &Enum_b, num_i, Snum_i, Enum_i, b);
		//i++
		inc(num_i, &Enum_i);
		//b=b*i
		mul(limit, num_b, &Snum_b, &Enum_b, num_i, Snum_i, Enum_i, b);
		//i++
		inc(num_i, &Enum_i);
		//b=b*i
		mul(limit, num_b, &Snum_b, &Enum_b, num_i, Snum_i, Enum_i, b);

		for(int idx = 0; idx < Precision; idx++)
		{
			num2[idx] = num_a[idx];
		}
		Snum2 = Snum_a;
		Enum2 = Enum_a;

		//num2=a/b
		div(limit, num2, &Snum2, &Enum2, num_b, Snum_b, Enum_b, r, b);

		if ((*pExponent) - Enum2 > (Precision - 1))
		{
			break;
		}
		//result+=num2
		add(Precision, Num_Array, pSign, pExponent, num2, Snum2, Enum2);

		limit = Precision - (*pExponent - Enum2);

		if(limit > Precision) limit = Precision;
		if(limit <= 0) limit = 1;
	}

	if(argList->flag==0)
	{
		EnterCriticalSection(&(argList->cs1));
		LeaveCriticalSection(&(argList->cs1));
	}

	while(argList->flag==0)
	{
		Sleep(10);
		EnterCriticalSection(&(argList->cs1));
		LeaveCriticalSection(&(argList->cs1));
	}

	add(Precision, Num_Array, pSign, pExponent, argList->Num_Array, -argList->Sign, argList->Exponent);

	DeleteCriticalSection(&(argList->cs1));

	delete argList->Num_Array;

	delete argList->r;

	delete argList->b;

	delete argList->num;

	delete argList->num_i;

	delete argList->num_a;

	delete argList->num_b;

	delete argList->num_x4;

	delete argList->num2;

	delete r;

	delete b;

	delete num;

	delete num_i;

	delete num_a;

	delete num_b;

	delete num_x4;

	delete num2;

	delete argList;
}

//simple but slow
void sin(unsigned int* Num_Array, int Precision, int* pSign, int* pExponent)
{
	//TODO regular by pi

	//2倍临时空间
	unsigned int* b = new unsigned int[Precision];

	unsigned int* num = clone(Precision, Num_Array);
	int Snum = (*pSign);
	int Enum = (*pExponent);

	unsigned int* num_a = clone(Precision, Num_Array);
	int Snum_a = (*pSign);
	int Enum_a = (*pExponent);

	int num_b = 1;

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

		num_b+=2;

		for(int idx = 0; idx < Precision; idx++)
		{
			num2[idx] = num_a[idx];
		}
		Snum2 = Snum_a;
		Enum2 = Enum_a;

		//num2=a/b
		int cnt=2;
		int idx=3;
		while(idx<=num_b)
		{
			int hi;
			int cnt2;
			_asm
			{
				mov eax,cnt
				cdq
				mov ecx,idx
				mul ecx
				mov hi,edx
				mov cnt2,eax
			}
			if(hi!=0)
			{
				divn(limit, num2, &Snum2, &Enum2, cnt);
				cnt = idx;
				idx++;
			}
			else
			{
				cnt = cnt2;
				idx++;
			}
		}
		divn(limit, num2, &Snum2, &Enum2, cnt);

		if (i % 2 == 1)
		{
			Snum2 = -Snum2;
		}

		if ((*pExponent) - Enum2 > (Precision - 1))
		{
			break;
		}
		//result+=num2
		add(Precision, Num_Array, pSign, pExponent, num2, Snum2, Enum2);

		limit = Precision - (*pExponent - Enum2);
		if(limit > Precision) limit = Precision;
		if(limit <= 0) limit = 1;
	}

	delete b;

	delete num;

	delete num_a;

	delete num_x2;

	delete num2;
}

extern "C" __declspec(dllexport) void N_Sin(int* PSE,unsigned int* Num_Array)
{
	int Precision = PSE[0];
	int Sign = PSE[1];
	int Exponent = PSE[2];

	//if(Precision>20)
	//{
	//	sinMT(Num_Array, Precision, &Sign, &Exponent);
	//}
	//else
	//{
		sin(Num_Array, Precision, &Sign, &Exponent);
	//}

	PSE[1] = Sign;
	PSE[2] = Exponent;

}




void atn(unsigned int* Num_Array, int Precision, int* pSign, int* pExponent)
{
	//2倍临时空间
	unsigned int* r = new unsigned int[Precision + 1];
	unsigned int* b = new unsigned int[Precision];

	unsigned int* num = clone(Precision, Num_Array);
	int Snum = (*pSign);
	int Enum = (*pExponent);

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

		inc(num_b, &Enum_b);
		inc(num_b, &Enum_b);

		for(int idx = 0; idx < Precision; idx++)
		{
			num2[idx] = num_a[idx];
		}
		Snum2 = Snum_a;
		Enum2 = Enum_a;

		//num2=a/b
		div(limit, num2, &Snum2, &Enum2, num_b, Snum_b, Enum_b, r, b);

		if (i % 2 == 1)
		{
			Snum2 = -Snum2;
		}

		if ((*pExponent) - Enum2 > (Precision - 1))
		{
			break;
		}
		//result+=num2
		add(Precision, Num_Array, pSign, pExponent, num2, Snum2, Enum2);

		limit = Precision - (*pExponent - Enum2);

		if(limit > Precision) limit = Precision;
		if(limit <= 0) limit = 1;

	}

	delete r;
	delete b;

	delete num;

	delete num_a;

	delete num_b;

	delete num_x2;

	delete num2;
}






struct ArgListAtn
{
	CRITICAL_SECTION cs1;
	int flag;

	int Precision;
	unsigned int* Num_Array;
	int Sign;
	int Exponent;

	unsigned int* r;
	unsigned int* b;

	unsigned int* num;
	int Snum;
	int Enum;

	unsigned int* num_a;
	int Snum_a;
	int Enum_a;

	unsigned int* num_b;
	int Snum_b;
	int Enum_b;

	unsigned int* num_x4;
	int Snum_x4;
	int Enum_x4;

	unsigned int* num2;
	int Snum2;
	int Enum2;
};

void atn_part1(LPVOID pParam)
{
	ArgListAtn* argList = (ArgListAtn*)pParam;
	EnterCriticalSection(&(argList->cs1));

	int limit = argList->Precision;

	//x4=x*x*x*x
	mul(limit, argList->num_x4, &argList->Snum_x4, &argList->Enum_x4, argList->num, argList->Snum, argList->Enum, argList->b);
	mul(limit, argList->num_x4, &argList->Snum_x4, &argList->Enum_x4, argList->num, argList->Snum, argList->Enum, argList->b);
	mul(limit, argList->num_x4, &argList->Snum_x4, &argList->Enum_x4, argList->num, argList->Snum, argList->Enum, argList->b);

	for (int i = 2 ; ; i+=2)
	{
		for(int idx = 0; idx < argList->Precision; idx++)
		{
			argList->num2[idx] = argList->num_a[idx];
		}
		argList->Snum2 = argList->Snum_a;
		argList->Enum2 = argList->Enum_a;

		//num2=a/b
		div(limit, argList->num2, &argList->Snum2, &argList->Enum2, argList->num_b, argList->Snum_b, argList->Enum_b, argList->r, argList->b);

		if ((argList->Exponent) - argList->Enum2 > (argList->Precision - 2))
		{
			break;
		}

		//result+=num2
		add(argList->Precision, argList->Num_Array, &argList->Sign, &argList->Exponent, argList->num2, argList->Snum2, argList->Enum2);

		limit = argList->Precision - (argList->Exponent - argList->Enum2);

		if(limit > argList->Precision) limit = argList->Precision;
		if(limit <= 0) limit = 1;

		//a=a*x4
		mul(limit, argList->num_a, &argList->Snum_a, &argList->Enum_a, argList->num_x4, argList->Snum_x4, argList->Enum_x4, argList->b);

		inc(argList->num_b, &argList->Enum_b);
		inc(argList->num_b, &argList->Enum_b);
		inc(argList->num_b, &argList->Enum_b);
		inc(argList->num_b, &argList->Enum_b);

	}

	argList->flag = 1;

	LeaveCriticalSection(&(argList->cs1));
	return;
}

void atnMT(unsigned int* Num_Array, int Precision, int* pSign, int* pExponent)
{
	//TODO regular by pi

	ArgListAtn* argList = new ArgListAtn();
	InitializeCriticalSection(&(argList->cs1));
	{
		argList->flag = 0;

		argList->r = new unsigned int[Precision + 1];
		argList->b = new unsigned int[Precision];
		argList->Precision = Precision;

		argList->Num_Array = newArray(Precision, 0);
		argList->Sign = 1;
		argList->Exponent = 0;

		argList->num = clone(Precision, Num_Array);
		argList->Snum = (*pSign);
		argList->Enum = (*pExponent);

		//init 3*x
		argList->num_a = clone(Precision, Num_Array);
		argList->Snum_a = (*pSign);
		argList->Enum_a = (*pExponent);
		mul(Precision, argList->num_a, &argList->Snum_a, &argList->Enum_a, Num_Array, *pSign, *pExponent, argList->b);
		mul(Precision, argList->num_a, &argList->Snum_a, &argList->Enum_a, Num_Array, *pSign, *pExponent, argList->b);

		//3
		argList->num_b = newArray(Precision, 3);
		argList->Snum_b = 1;
		argList->Enum_b = 1;

		argList->num_x4 = clone(Precision, Num_Array);
		argList->Snum_x4 = (*pSign);
		argList->Enum_x4 = (*pExponent);

		argList->num2 = new unsigned int[Precision];
		argList->Snum2 = 1;
		argList->Enum2 = 1;
	}

	_beginthread(atn_part1, 0, argList);

	//2倍临时空间
	unsigned int* r = new unsigned int[Precision + 1];
	unsigned int* b = new unsigned int[Precision];

	unsigned int* num = clone(Precision, Num_Array);
	int Snum = (*pSign);
	int Enum = (*pExponent);

	unsigned int* num_a = clone(Precision, Num_Array);
	int Snum_a = (*pSign);
	int Enum_a = (*pExponent);

	unsigned int* num_b = newArray(Precision, 1);
	int Snum_b = 1;
	int Enum_b = 1;

	unsigned int* num_x4 = clone(Precision, Num_Array);
	int Snum_x4 = (*pSign);
	int Enum_x4 = (*pExponent);

	unsigned int* num2 = new unsigned int[Precision];
	int Snum2 = 1;
	int Enum2 = 1;

	int limit = Precision;
	//x4=x*x*x*x
	mul(limit, num_x4, &Snum_x4, &Enum_x4, Num_Array, *pSign, *pExponent, b);
	mul(limit, num_x4, &Snum_x4, &Enum_x4, Num_Array, *pSign, *pExponent, b);
	mul(limit, num_x4, &Snum_x4, &Enum_x4, Num_Array, *pSign, *pExponent, b);
	for (int i = 1 ; ; i+=2)
	{
		//a=a*x4
		mul(limit, num_a, &Snum_a, &Enum_a, num_x4, Snum_x4, Enum_x4, b);

		inc(num_b, &Enum_b);
		inc(num_b, &Enum_b);
		inc(num_b, &Enum_b);
		inc(num_b, &Enum_b);

		for(int idx = 0; idx < Precision; idx++)
		{
			num2[idx] = num_a[idx];
		}
		Snum2 = Snum_a;
		Enum2 = Enum_a;

		//num2=a/b
		div(limit, num2, &Snum2, &Enum2, num_b, Snum_b, Enum_b, r, b);

		if ((*pExponent) - Enum2 > (Precision - 1))
		{
			break;
		}
		//result+=num2
		add(Precision, Num_Array, pSign, pExponent, num2, Snum2, Enum2);

		limit = Precision - (*pExponent - Enum2);

		if(limit > Precision) limit = Precision;
		if(limit <= 0) limit = 1;
	}

	if(argList->flag==0)
	{
		EnterCriticalSection(&(argList->cs1));
		LeaveCriticalSection(&(argList->cs1));
	}

	while(argList->flag==0)
	{
		Sleep(10);
		EnterCriticalSection(&(argList->cs1));
		LeaveCriticalSection(&(argList->cs1));
	}

	DeleteCriticalSection(&(argList->cs1));

	add(Precision, Num_Array, pSign, pExponent, argList->Num_Array, -argList->Sign, argList->Exponent);

	delete argList->Num_Array;

	delete argList->r;

	delete argList->b;

	delete argList->num;

	delete argList->num_a;

	delete argList->num_b;

	delete argList->num_x4;

	delete argList->num2;

	delete r;

	delete b;

	delete num;

	delete num_a;

	delete num_b;

	delete num_x4;

	delete num2;

	delete argList;
}




extern "C" __declspec(dllexport) void N_Atn(int* PSE,unsigned int* Num_Array)
{
	int Precision = PSE[0];
	int Sign = PSE[1];
	int Exponent = PSE[2];

	if(Precision>50)
	{
		atnMT(Num_Array,Precision,&Sign,&Exponent);
	}
	else
	{
		atn(Num_Array,Precision,&Sign,&Exponent);
	}

	PSE[1] = Sign;
	PSE[2] = Exponent;
}









/*

void sin(unsigned int* Num_Array, int Precision, int* pSign, int* pExponent)
{
	//TODO regular by pi

	//2倍临时空间
	unsigned int* r = new unsigned int[Precision + 1];
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
		inc(num_i, &Enum_i);
		//b=b*i
		mul(limit, num_b, &Snum_b, &Enum_b, num_i, Snum_i, Enum_i, b);
		//i++
		inc(num_i, &Enum_i);
		//b=b*i
		mul(limit, num_b, &Snum_b, &Enum_b, num_i, Snum_i, Enum_i, b);

		for(int idx = 0; idx < Precision; idx++)
		{
			num2[idx] = num_a[idx];
		}
		Snum2 = Snum_a;
		Enum2 = Enum_a;

		//num2=a/b
		div(limit, num2, &Snum2, &Enum2, num_b, Snum_b, Enum_b, r, b);

		if (i % 2 == 1)
		{
			Snum2 = -Snum2;
		}

		if ((*pExponent) - Enum2 > (Precision - 1))
		{
			break;
		}
		//result+=num2
		add(Precision, Num_Array, pSign, pExponent, num2, Snum2, Enum2);

		limit = Precision - (*pExponent - Enum2);

		if(limit > Precision) limit = Precision;
		if(limit <= 0) limit = 1;
	}

	delete r;
	delete b;

	delete num;

	delete num_i;

	delete num_a;

	delete num_b;

	delete num_x2;

	delete num2;
}

struct ArgList
{
	CRITICAL_SECTION cs1;
	int flag;

	int Precision;
	unsigned int* Num_Array;
	int Sign;
	int Exponent;

	unsigned int* r;
	unsigned int* b;

	unsigned int* num;
	int Snum;
	int Enum;

	unsigned int* num_i;
	int Snum_i;
	int Enum_i;

	unsigned int* num_a;
	int Snum_a;
	int Enum_a;

	unsigned int* num_b;
	int Snum_b;
	int Enum_b;

	unsigned int* num_x4;
	int Snum_x4;
	int Enum_x4;

	unsigned int* num2;
	int Snum2;
	int Enum2;
};


void sin_part1(LPVOID pParam)
{
	ArgList* argList = (ArgList*)pParam;
	EnterCriticalSection(&(argList->cs1));

	int limit = argList->Precision;

	//x4=x*x*x*x
	mul(limit, argList->num_x4, &argList->Snum_x4, &argList->Enum_x4, argList->num, argList->Snum, argList->Enum, argList->b);
	mul(limit, argList->num_x4, &argList->Snum_x4, &argList->Enum_x4, argList->num, argList->Snum, argList->Enum, argList->b);
	mul(limit, argList->num_x4, &argList->Snum_x4, &argList->Enum_x4, argList->num, argList->Snum, argList->Enum, argList->b);

	for (int i = 2 ; ; i+=2)
	{
		for(int idx = 0; idx < argList->Precision; idx++)
		{
			argList->num2[idx] = argList->num_a[idx];
		}
		argList->Snum2 = argList->Snum_a;
		argList->Enum2 = argList->Enum_a;

		//num2=a/b
		div(limit, argList->num2, &argList->Snum2, &argList->Enum2, argList->num_b, argList->Snum_b, argList->Enum_b, argList->r, argList->b);

		if ((argList->Exponent) - argList->Enum2 > (argList->Precision - 2))
		{
			break;
		}

		//result+=num2
		add(argList->Precision, argList->Num_Array, &argList->Sign, &argList->Exponent, argList->num2, argList->Snum2, argList->Enum2);

		limit = argList->Precision - (argList->Exponent - argList->Enum2);

		if(limit > argList->Precision) limit = argList->Precision;
		if(limit <= 0) limit = 1;

		//a=a*x4
		mul(limit, argList->num_a, &argList->Snum_a, &argList->Enum_a, argList->num_x4, argList->Snum_x4, argList->Enum_x4, argList->b);

		//b=b*(i)*(i+1)*(i+2)*(i+3)
		//i++
		inc(argList->num_i, &argList->Enum_i);
		//b=b*i
		mul(limit, argList->num_b, &argList->Snum_b, &argList->Enum_b, argList->num_i, argList->Snum_i, argList->Enum_i, argList->b);
		//i++
		inc(argList->num_i, &argList->Enum_i);
		//b=b*i
		mul(limit, argList->num_b, &argList->Snum_b, &argList->Enum_b, argList->num_i, argList->Snum_i, argList->Enum_i, argList->b);
		//i++
		inc(argList->num_i, &argList->Enum_i);
		//b=b*i
		mul(limit, argList->num_b, &argList->Snum_b, &argList->Enum_b, argList->num_i, argList->Snum_i, argList->Enum_i, argList->b);
		//i++
		inc(argList->num_i, &argList->Enum_i);
		//b=b*i
		mul(limit, argList->num_b, &argList->Snum_b, &argList->Enum_b, argList->num_i, argList->Snum_i, argList->Enum_i, argList->b);

	}

	argList->flag = 1;

	LeaveCriticalSection(&(argList->cs1));
	return;
}

int sinMT(unsigned int* Num_Array, int Precision, int* pSign, int* pExponent)
{
	//TODO regular by pi

	ArgList* argList = new ArgList();
	InitializeCriticalSection(&(argList->cs1));
	{
		argList->flag = 0;

		argList->r = new unsigned int[Precision + 1];
		argList->b = new unsigned int[Precision];
		argList->Precision = Precision;

		argList->Num_Array = newArray(Precision, 0);
		argList->Sign = 1;
		argList->Exponent = 0;

		argList->num = clone(Precision, Num_Array);
		argList->Snum = (*pSign);
		argList->Enum = (*pExponent);

		argList->num_i = newArray(Precision, 3);
		argList->Snum_i = 1;
		argList->Enum_i = 1;

		//init 3*x
		argList->num_a = clone(Precision, Num_Array);
		argList->Snum_a = (*pSign);
		argList->Enum_a = (*pExponent);
		mul(Precision, argList->num_a, &argList->Snum_a, &argList->Enum_a, Num_Array, *pSign, *pExponent, argList->b);
		mul(Precision, argList->num_a, &argList->Snum_a, &argList->Enum_a, Num_Array, *pSign, *pExponent, argList->b);

		//3!
		argList->num_b = newArray(Precision, 6);
		argList->Snum_b = 1;
		argList->Enum_b = 1;

		argList->num_x4 = clone(Precision, Num_Array);
		argList->Snum_x4 = (*pSign);
		argList->Enum_x4 = (*pExponent);

		argList->num2 = new unsigned int[Precision];
		argList->Snum2 = 1;
		argList->Enum2 = 1;
	}

	_beginthread(sin_part1, 0, argList);

	//2倍临时空间
	unsigned int* r = new unsigned int[Precision + 1];
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

	unsigned int* num_x4 = clone(Precision, Num_Array);
	int Snum_x4 = (*pSign);
	int Enum_x4 = (*pExponent);

	unsigned int* num2 = new unsigned int[Precision];
	int Snum2 = 1;
	int Enum2 = 1;
	int v=0;
	int limit = Precision;
	//x4=x*x*x*x
	mul(limit, num_x4, &Snum_x4, &Enum_x4, Num_Array, *pSign, *pExponent, b);
	mul(limit, num_x4, &Snum_x4, &Enum_x4, Num_Array, *pSign, *pExponent, b);
	mul(limit, num_x4, &Snum_x4, &Enum_x4, Num_Array, *pSign, *pExponent, b);
	for (int i = 1 ; ; i+=2)
	{
		//a=a*x4
		mul(limit, num_a, &Snum_a, &Enum_a, num_x4, Snum_x4, Enum_x4, b);

		//b=b*(i)*(i+1)*(i+2)*(i+3)
		//i++
		inc(num_i, &Enum_i);
		//b=b*i
		mul(limit, num_b, &Snum_b, &Enum_b, num_i, Snum_i, Enum_i, b);
		//i++
		inc(num_i, &Enum_i);
		//b=b*i
		mul(limit, num_b, &Snum_b, &Enum_b, num_i, Snum_i, Enum_i, b);
		//i++
		inc(num_i, &Enum_i);
		//b=b*i
		mul(limit, num_b, &Snum_b, &Enum_b, num_i, Snum_i, Enum_i, b);
		//i++
		inc(num_i, &Enum_i);
		//b=b*i
		mul(limit, num_b, &Snum_b, &Enum_b, num_i, Snum_i, Enum_i, b);

		v+=4;

		for(int idx = 0; idx < Precision; idx++)
		{
			num2[idx] = num_a[idx];
		}
		Snum2 = Snum_a;
		Enum2 = Enum_a;

		//num2=a/b
		div(limit, num2, &Snum2, &Enum2, num_b, Snum_b, Enum_b, r, b);

		if ((*pExponent) - Enum2 > (Precision - 1))
		{
			break;
		}
		//result+=num2
		add(Precision, Num_Array, pSign, pExponent, num2, Snum2, Enum2);

		limit = Precision - (*pExponent - Enum2);

		if(limit > Precision) limit = Precision;
		if(limit <= 0) limit = 1;
	}

	if(argList->flag==0)
	{
		EnterCriticalSection(&(argList->cs1));
		LeaveCriticalSection(&(argList->cs1));
	}

	while(argList->flag==0)
	{
		Sleep(10);
		EnterCriticalSection(&(argList->cs1));
		LeaveCriticalSection(&(argList->cs1));
	}

	add(Precision, Num_Array, pSign, pExponent, argList->Num_Array, -argList->Sign, argList->Exponent);

	DeleteCriticalSection(&(argList->cs1));

	delete argList->Num_Array;

	delete argList->r;

	delete argList->b;

	delete argList->num;

	delete argList->num_i;

	delete argList->num_a;

	delete argList->num_b;

	delete argList->num_x4;

	delete argList->num2;

	delete r;

	delete b;

	delete num;

	delete num_i;

	delete num_a;

	delete num_b;

	delete num_x4;

	delete num2;

	delete argList;

	return v;
}


*/