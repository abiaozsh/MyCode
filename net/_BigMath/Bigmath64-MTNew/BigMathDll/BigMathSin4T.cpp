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

struct ArgList
{
	CRITICAL_SECTION cs;
	int flag;

	int Precision;

	//主累加器
	unsigned int* Num_Array;int Sign;int Exponent;

	unsigned int* num;int Snum;int Enum;

	unsigned int* num_i;int Snum_i;int Enum_i;

	unsigned int* num_a;int Snum_a;int Enum_a;

	unsigned int* num_b;int Snum_b;int Enum_b;

	unsigned int* num_x8;int Snum_x8;int Enum_x8;

	unsigned int* num2;int Snum2;int Enum2;

	unsigned int* r_value;
	unsigned int* b;
};


void sin_part4T(LPVOID pParam)
{
	ArgList* argList = (ArgList*)pParam;
	EnterCriticalSection(&(argList->cs));

	int limit = argList->Precision;

	while(true)
	{

		for(int idx = 0; idx < argList->Precision; idx++)
		{
			argList->num2[idx] = argList->num_a[idx];
		}
		argList->Snum2 = argList->Snum_a;
		argList->Enum2 = argList->Enum_a;

		//num2=a/b
		divi(limit, argList->num2, &argList->Snum2, &argList->Enum2, argList->num_b, argList->Snum_b, argList->Enum_b, argList->r_value, argList->b);

		if ((argList->Exponent) - argList->Enum2 > (argList->Precision - 2))
		{
			break;
		}

		//result+=num2
		add(argList->Precision, argList->Num_Array, &argList->Sign, &argList->Exponent, argList->num2, argList->Snum2, argList->Enum2);

		limit = argList->Precision - (argList->Exponent - argList->Enum2);

		if(limit > argList->Precision) limit = argList->Precision;
		if(limit <= 0) limit = 1;

		//a=a*x^8
		mul(limit, argList->num_a, &argList->Snum_a, &argList->Enum_a, argList->num_x8, argList->Snum_x8, argList->Enum_x8, argList->b);
		
		//b=b*(i)*(i+1)*(i+2)*(i+3)*(i+4)*(i+5)*(i+6)*(i+7)
		//i++
		incr(argList->num_i, &argList->Enum_i);
		//b=b*i
		mul(limit, argList->num_b, &argList->Snum_b, &argList->Enum_b, argList->num_i, argList->Snum_i, argList->Enum_i, argList->b);
		//i++
		incr(argList->num_i, &argList->Enum_i);
		//b=b*i
		mul(limit, argList->num_b, &argList->Snum_b, &argList->Enum_b, argList->num_i, argList->Snum_i, argList->Enum_i, argList->b);
		//i++
		incr(argList->num_i, &argList->Enum_i);
		//b=b*i
		mul(limit, argList->num_b, &argList->Snum_b, &argList->Enum_b, argList->num_i, argList->Snum_i, argList->Enum_i, argList->b);
		//i++
		incr(argList->num_i, &argList->Enum_i);
		//b=b*i
		mul(limit, argList->num_b, &argList->Snum_b, &argList->Enum_b, argList->num_i, argList->Snum_i, argList->Enum_i, argList->b);
		//i++
		incr(argList->num_i, &argList->Enum_i);
		//b=b*i
		mul(limit, argList->num_b, &argList->Snum_b, &argList->Enum_b, argList->num_i, argList->Snum_i, argList->Enum_i, argList->b);
		//i++
		incr(argList->num_i, &argList->Enum_i);
		//b=b*i
		mul(limit, argList->num_b, &argList->Snum_b, &argList->Enum_b, argList->num_i, argList->Snum_i, argList->Enum_i, argList->b);
		//i++
		incr(argList->num_i, &argList->Enum_i);
		//b=b*i
		mul(limit, argList->num_b, &argList->Snum_b, &argList->Enum_b, argList->num_i, argList->Snum_i, argList->Enum_i, argList->b);
		//i++
		incr(argList->num_i, &argList->Enum_i);
		//b=b*i
		mul(limit, argList->num_b, &argList->Snum_b, &argList->Enum_b, argList->num_i, argList->Snum_i, argList->Enum_i, argList->b);
	}

	argList->flag = 1;

	LeaveCriticalSection(&(argList->cs));
}





void sin4T(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent)
{
	//TODO regular by pi
	ArgList* argList3 = new ArgList();
	InitializeCriticalSection(&(argList3->cs));
	{
		argList3->flag = 0;

		argList3->r_value = new unsigned int[Precision + 2];
		argList3->b = new unsigned int[Precision];
		argList3->Precision = Precision;
		
		argList3->num2 = newArray(Precision, 0);
		argList3->Snum2 = 1;
		argList3->Enum2 = 1;

		argList3->Num_Array = newArray(Precision, 0);argList3->Sign = 1;argList3->Exponent = 0;

		argList3->num = clone(Precision, Num_Array);argList3->Snum = (*pSign);argList3->Enum = (*pExponent);

		//3
		argList3->num_i = newArray(Precision, 3);argList3->Snum_i = 1;argList3->Enum_i = 1;

		//3!
		argList3->num_b = newArray(Precision, 6);argList3->Snum_b = 1;argList3->Enum_b = 1;

		//init x^3
		argList3->num_a = clone(Precision, Num_Array);argList3->Snum_a = (*pSign);argList3->Enum_a = (*pExponent);
		mul(Precision, argList3->num_a, &argList3->Snum_a, &argList3->Enum_a, Num_Array, *pSign, *pExponent, argList3->b);
		mul(Precision, argList3->num_a, &argList3->Snum_a, &argList3->Enum_a, Num_Array, *pSign, *pExponent, argList3->b);

	}

	ArgList* argList5 = new ArgList();
	InitializeCriticalSection(&(argList5->cs));
	{
		argList5->flag = 0;
		argList5->r_value = new unsigned int[Precision + 2];
		argList5->b = new unsigned int[Precision];
		argList5->Precision = Precision;
				//temp
		argList5->num2 = newArray(Precision, 0);argList5->Snum2 = 1;argList5->Enum2 = 1;

		//0
		argList5->Num_Array = newArray(Precision, 0);argList5->Sign = 1;argList5->Exponent = 0;

		//x
		argList5->num = clone(Precision, Num_Array);argList5->Snum = (*pSign);argList5->Enum = (*pExponent);

		//i=5
		argList5->num_i = newArray(Precision, 5);argList5->Snum_i = 1;argList5->Enum_i = 1;

		//b=5!
		argList5->num_b = newArray(Precision, 120);argList5->Snum_b = 1;argList5->Enum_b = 1;

		//a=x^5
		argList5->num_a = clone(Precision, argList3->num_a);argList5->Snum_a = argList3->Snum_a;argList5->Enum_a = argList3->Enum_a;
		mul(Precision, argList5->num_a, &argList5->Snum_a, &argList5->Enum_a, Num_Array, *pSign, *pExponent, argList3->b);
		mul(Precision, argList5->num_a, &argList5->Snum_a, &argList5->Enum_a, Num_Array, *pSign, *pExponent, argList3->b);
	}

	ArgList* argList7 = new ArgList();
	InitializeCriticalSection(&(argList7->cs));
	{
		argList7->flag = 0;

		argList7->r_value = new unsigned int[Precision + 2];
		argList7->b = new unsigned int[Precision];
		argList7->Precision = Precision;
		
		argList7->num2 = newArray(Precision, 0);
		argList7->Snum2 = 1;
		argList7->Enum2 = 1;

		argList7->Num_Array = newArray(Precision, 0);argList7->Sign = 1;argList7->Exponent = 0;

		argList7->num = clone(Precision, Num_Array);argList7->Snum = (*pSign);argList7->Enum = (*pExponent);

		//i=7
		argList7->num_i = newArray(Precision, 7);argList7->Snum_i = 1;argList7->Enum_i = 1;

		//b=7!
		argList7->num_b = newArray(Precision, 5040);argList7->Snum_b = 1;argList7->Enum_b = 1;

		//a=x^7
		argList7->num_a = clone(Precision, argList5->num_a);argList7->Snum_a = argList5->Snum_a;argList7->Enum_a = argList5->Enum_a;
		mul(Precision, argList7->num_a, &argList7->Snum_a, &argList7->Enum_a, Num_Array, *pSign, *pExponent, argList3->b);
		mul(Precision, argList7->num_a, &argList7->Snum_a, &argList7->Enum_a, Num_Array, *pSign, *pExponent, argList3->b);

	}
	//x^8 = a*x
	argList7->num_x8 = clone(Precision, argList7->num_a);argList7->Snum_x8 = argList7->Snum_a;argList7->Enum_x8 = argList7->Enum_a;
	mul(argList7->Precision, argList7->num_x8, &argList7->Snum_x8, &argList7->Enum_x8, Num_Array, (*pSign), (*pExponent), argList7->b);

	argList3->num_x8 = clone(Precision, argList7->num_x8);argList3->Snum_x8 = argList7->Snum_x8;argList3->Enum_x8 = argList7->Enum_x8;
	argList5->num_x8 = clone(Precision, argList7->num_x8);argList5->Snum_x8 = argList7->Snum_x8;argList5->Enum_x8 = argList7->Enum_x8;

	ArgList* argList9 = new ArgList();
	InitializeCriticalSection(&(argList9->cs));
	{
		argList9->flag = 0;
		argList9->r_value = new unsigned int[Precision + 2];
		argList9->b = new unsigned int[Precision];
		argList9->Precision = Precision;
				//temp
		argList9->num2 = newArray(Precision, 0);argList9->Snum2 = 1;argList9->Enum2 = 1;

		//0
		argList9->Num_Array = newArray(Precision, 0);argList9->Sign = 1;argList9->Exponent = 0;

		//x
		argList9->num = clone(Precision, Num_Array);argList9->Snum = (*pSign);argList9->Enum = (*pExponent);

		//i=9
		argList9->num_i = newArray(Precision, 9);argList9->Snum_i = 1;argList9->Enum_i = 1;

		//b=9!
		argList9->num_b = newArray(Precision, 362880);argList9->Snum_b = 1;argList9->Enum_b = 1;

		//x^8
		argList9->num_x8 = clone(Precision, argList7->num_x8);argList9->Snum_x8 = argList7->Snum_x8;argList9->Enum_x8 = argList7->Enum_x8;

		//a=x^9 = x^8 * x
		argList9->num_a = clone(Precision, argList9->num_x8);argList9->Snum_a = argList9->Snum_x8;argList9->Enum_a = argList9->Enum_x8;
		mul(argList9->Precision, argList9->num_a, &argList9->Snum_a, &argList9->Enum_a, Num_Array, (*pSign), (*pExponent), argList9->b);
	}


	_beginthread(sin_part4T, 0, argList3);
	_beginthread(sin_part4T, 0, argList5);
	_beginthread(sin_part4T, 0, argList7);
	_beginthread(sin_part4T, 0, argList9);

	if(argList3->flag==0 || argList5->flag==0 || argList7->flag==0 || argList9->flag==0)
	{
		EnterCriticalSection(&(argList3->cs));
		LeaveCriticalSection(&(argList3->cs));

		EnterCriticalSection(&(argList5->cs));
		LeaveCriticalSection(&(argList5->cs));

		EnterCriticalSection(&(argList7->cs));
		LeaveCriticalSection(&(argList7->cs));

		EnterCriticalSection(&(argList9->cs));
		LeaveCriticalSection(&(argList9->cs));
	}

	while(argList3->flag==0 || argList5->flag==0 || argList7->flag==0 || argList9->flag==0)
	{
		Sleep(10);

		EnterCriticalSection(&(argList3->cs));
		LeaveCriticalSection(&(argList3->cs));

		EnterCriticalSection(&(argList5->cs));
		LeaveCriticalSection(&(argList5->cs));

		EnterCriticalSection(&(argList7->cs));
		LeaveCriticalSection(&(argList7->cs));

		EnterCriticalSection(&(argList9->cs));
		LeaveCriticalSection(&(argList9->cs));
	}

	DeleteCriticalSection(&(argList3->cs));
	DeleteCriticalSection(&(argList5->cs));
	DeleteCriticalSection(&(argList7->cs));
	DeleteCriticalSection(&(argList9->cs));

	add(Precision, Num_Array, pSign, pExponent, argList3->Num_Array, -argList3->Sign, argList3->Exponent); //负半
	add(Precision, Num_Array, pSign, pExponent, argList5->Num_Array, argList5->Sign, argList5->Exponent);
	add(Precision, Num_Array, pSign, pExponent, argList7->Num_Array, -argList7->Sign, argList7->Exponent); //负半
	add(Precision, Num_Array, pSign, pExponent, argList9->Num_Array, argList9->Sign, argList9->Exponent);

	delete argList3->Num_Array;
	delete argList3->r_value;
	delete argList3->b;
	delete argList3->num;
	delete argList3->num_i;
	delete argList3->num_a;
	delete argList3->num_b;
	delete argList3->num_x8;
	delete argList3->num2;

	delete argList5->Num_Array;
	delete argList5->r_value;
	delete argList5->b;
	delete argList5->num;
	delete argList5->num_i;
	delete argList5->num_a;
	delete argList5->num_b;
	delete argList5->num_x8;
	delete argList5->num2;

	delete argList7->Num_Array;
	delete argList7->r_value;
	delete argList7->b;
	delete argList7->num;
	delete argList7->num_i;
	delete argList7->num_a;
	delete argList7->num_b;
	delete argList7->num_x8;
	delete argList7->num2;

	delete argList9->Num_Array;
	delete argList9->r_value;
	delete argList9->b;
	delete argList9->num;
	delete argList9->num_i;
	delete argList9->num_a;
	delete argList9->num_b;
	delete argList9->num_x8;
	delete argList9->num2;

	delete argList3;
	delete argList5;
	delete argList7;
	delete argList9;
}
