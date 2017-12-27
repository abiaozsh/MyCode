//v1.2

#include <windows.h>
#include <process.h>
#include <stdio.h>

#define int __int64

extern "C" void add(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent, unsigned int* num_Num_Array, int num_Sign, int num_Exponent);
extern "C" void mul(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent, unsigned int* num_Num_Array, int num_Sign, int num_Exponent, unsigned int* buf_Num_Array);
extern "C" void divi(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent, unsigned int* num_Num_Array, int num_Sign, int num_Exponent, unsigned int* r, unsigned int* b);
extern "C" void incr(unsigned int* Num_Array, int* pExponent);

unsigned int* clone(int Precision, unsigned int* Num_Array);
unsigned int* newArray(int Precision, unsigned int n);

HANDLE m_hEvent[2];    

DWORD WINAPI MyThreadProc1(LPVOID lpParam){
	while(true){
		::Sleep(100);
		FILE* f = fopen("d:\\file1.txt","r");
		if(f){
		SetEvent(m_hEvent[0]);
		}
	}
	return 0;
}

DWORD WINAPI MyThreadProc2(LPVOID lpParam){
	while(true){
		::Sleep(100);
		FILE* f = fopen("d:\\file2.txt","r");
		if(f){
		SetEvent(m_hEvent[1]);
		}
	}
	return 0;
}


extern "C" __declspec(dllexport) void N_Test(int* PSE,unsigned int* Num_Array,int threads)
{
	int Precision = PSE[0];
	int Sign = PSE[1];
	int Exponent = PSE[2];

m_hEvent[0]=::CreateEvent(NULL, FALSE, FALSE, NULL);  
m_hEvent[1]=::CreateEvent(NULL, FALSE, FALSE, NULL);  
  
::CreateThread(NULL, 0, MyThreadProc1, NULL, 0, NULL);  
::CreateThread(NULL, 0, MyThreadProc2, NULL, 0, NULL);  

	int nIndex = ::WaitForMultipleObjects(2, m_hEvent, FALSE,INFINITE);     



	PSE[1] = Sign;
	PSE[2] = Exponent;
}