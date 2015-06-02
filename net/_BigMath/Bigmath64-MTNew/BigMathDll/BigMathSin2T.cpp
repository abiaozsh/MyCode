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

	//Ö÷ÀÛ¼ÓÆ÷
	unsigned int* Num_Array;int Sign;int Exponent;

	unsigned int* num;int Snum;int Enum;

	unsigned int* num_i;int Snum_i;int Enum_i;

	unsigned int* num_a;int Snum_a;int Enum_a;

	unsigned int* num_b;int Snum_b;int Enum_b;

	unsigned int* num_x4;int Snum_x4;int Enum_x4;

	unsigned int* num2;int Snum2;int Enum2;

	unsigned int* r_value;
	unsigned int* b;
};


void sin_part2T(LPVOID pParam)
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

		//a=a*x4
		mul(limit, argList->num_a, &argList->Snum_a, &argList->Enum_a, argList->num_x4, argList->Snum_x4, argList->Enum_x4, argList->b);
		
		//b=b*(i)*(i+1)*(i+2)*(i+3)
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





void sin2T(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent)
{
	//TODO regular by pi
	ArgList* argList2 = new ArgList();
	InitializeCriticalSection(&(argList2->cs));
	{
		argList2->flag = 0;

		argList2->r_value = new unsigned int[Precision + 2];
		argList2->b = new unsigned int[Precision];
		argList2->Precision = Precision;
		
		argList2->num2 = newArray(Precision, 0);
		argList2->Snum2 = 1;
		argList2->Enum2 = 1;

		argList2->Num_Array = newArray(Precision, 0);argList2->Sign = 1;argList2->Exponent = 0;

		argList2->num = clone(Precision, Num_Array);argList2->Snum = (*pSign);argList2->Enum = (*pExponent);

		//3
		argList2->num_i = newArray(Precision, 3);argList2->Snum_i = 1;argList2->Enum_i = 1;

		//3!
		argList2->num_b = newArray(Precision, 6);argList2->Snum_b = 1;argList2->Enum_b = 1;

		//init x^3
		argList2->num_a = clone(Precision, Num_Array);argList2->Snum_a = (*pSign);argList2->Enum_a = (*pExponent);
		mul(Precision, argList2->num_a, &argList2->Snum_a, &argList2->Enum_a, Num_Array, *pSign, *pExponent, argList2->b);
		mul(Precision, argList2->num_a, &argList2->Snum_a, &argList2->Enum_a, Num_Array, *pSign, *pExponent, argList2->b);

		//x^4
		argList2->num_x4 = clone(Precision, Num_Array);argList2->Snum_x4 = (*pSign);argList2->Enum_x4 = (*pExponent);
		mul(argList2->Precision, argList2->num_x4, &argList2->Snum_x4, &argList2->Enum_x4, argList2->num_a, argList2->Snum_a, argList2->Enum_a, argList2->b);
	}

	ArgList* argList1 = new ArgList();
	InitializeCriticalSection(&(argList1->cs));
	{
		argList1->flag = 0;
		argList1->r_value = new unsigned int[Precision + 2];
		argList1->b = new unsigned int[Precision];
		argList1->Precision = Precision;
				//temp
		argList1->num2 = newArray(Precision, 0);argList1->Snum2 = 1;argList1->Enum2 = 1;

		//0
		argList1->Num_Array = newArray(Precision, 0);argList1->Sign = 1;argList1->Exponent = 0;

		//x
		argList1->num = clone(Precision, Num_Array);argList1->Snum = (*pSign);argList1->Enum = (*pExponent);

		//i=5
		argList1->num_i = newArray(Precision, 5);argList1->Snum_i = 1;argList1->Enum_i = 1;

		//b=5!
		argList1->num_b = newArray(Precision, 120);argList1->Snum_b = 1;argList1->Enum_b = 1;

		//x^4
		argList1->num_x4 = clone(Precision, argList2->num_x4);argList1->Snum_x4 = argList2->Snum_x4;argList1->Enum_x4 = argList2->Enum_x4;

		//a=x^5
		argList1->num_a = clone(Precision, Num_Array);argList1->Snum_a = (*pSign);argList1->Enum_a = (*pExponent);
		mul(argList1->Precision, argList1->num_a, &argList1->Snum_a, &argList1->Enum_a, argList1->num_x4, argList1->Snum_x4, argList1->Enum_x4, argList1->b);
	}

	_beginthread(sin_part2T, 0, argList1);
	_beginthread(sin_part2T, 0, argList2);

	if(argList1->flag==0 || argList2->flag==0)
	{
		EnterCriticalSection(&(argList1->cs));
		LeaveCriticalSection(&(argList1->cs));

		EnterCriticalSection(&(argList2->cs));
		LeaveCriticalSection(&(argList2->cs));
	}

	while(argList1->flag==0 || argList2->flag==0)
	{
		Sleep(10);
		EnterCriticalSection(&(argList1->cs));
		LeaveCriticalSection(&(argList1->cs));

		EnterCriticalSection(&(argList2->cs));
		LeaveCriticalSection(&(argList2->cs));
	}

	DeleteCriticalSection(&(argList1->cs));
	DeleteCriticalSection(&(argList2->cs));

	add(Precision, Num_Array, pSign, pExponent, argList1->Num_Array, argList1->Sign, argList1->Exponent);
	add(Precision, Num_Array, pSign, pExponent, argList2->Num_Array, -argList2->Sign, argList2->Exponent); //¸º°ë

	delete argList1->Num_Array;
	delete argList1->r_value;
	delete argList1->b;
	delete argList1->num;
	delete argList1->num_i;
	delete argList1->num_a;
	delete argList1->num_b;
	delete argList1->num_x4;
	delete argList1->num2;

	delete argList2->Num_Array;
	delete argList2->r_value;
	delete argList2->b;
	delete argList2->num;
	delete argList2->num_i;
	delete argList2->num_a;
	delete argList2->num_b;
	delete argList2->num_x4;
	delete argList2->num2;

	delete argList1;
	delete argList2;
}
