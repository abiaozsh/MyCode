//v1.2

#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <iostream>
using namespace std; 

#define int __int64

#define DEBUG

extern "C" void add(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent, unsigned int* num_Num_Array, int num_Sign, int num_Exponent);
extern "C" void mul(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent, unsigned int* num_Num_Array, int num_Sign, int num_Exponent, unsigned int* buf_Num_Array);
extern "C" void divi(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent, unsigned int* num_Num_Array, int num_Sign, int num_Exponent, unsigned int* r, unsigned int* b);
extern "C" void incr(unsigned int* Num_Array, int* pExponent);

unsigned int* clone(int Precision, unsigned int* Num_Array);

unsigned int* newArray(int Precision, unsigned int n);

struct ThreadArg
{
	int id;

	HANDLE* doneEvent;
	HANDLE* startEvent;

	int job;//1 计算 0 终止
	int done;
	int free;

	int idx;

	int limit;

	unsigned int* num_b;int Snum_b;int Enum_b;

	unsigned int* num2;int Snum2;int Enum2;

	unsigned int* r_value;
	unsigned int* b;
};

DWORD WINAPI ThreadProc(LPVOID lpParam){
	ThreadArg* threadArg = (ThreadArg*)lpParam;
	
	#ifdef DEBUG
	printf("thread %d inited\r\n",threadArg->id);
	#endif

	threadArg->free = 1;
	while(true){
		int nIndex = ::WaitForMultipleObjects(1, threadArg->startEvent, FALSE,100);
		
		if(threadArg->job){
			#ifdef DEBUG
			printf("thread start work %d ,%d\r\n",threadArg->id,threadArg->idx);
			#endif

			//num2=a/b
			divi(threadArg->limit, threadArg->num2, &threadArg->Snum2, &threadArg->Enum2, threadArg->num_b, threadArg->Snum_b, threadArg->Enum_b, threadArg->r_value, threadArg->b);

			#ifdef DEBUG
			printf("threadwork done  %d ,%d\r\n",threadArg->id,threadArg->idx);
			#endif

			threadArg->done = 1;
			threadArg->free = 1;
			SetEvent(*(threadArg->doneEvent));
		}else{
			break;
		}
	}
	return 0;
}

void sinMT(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent,int threads)
{
	HANDLE* doneEventList;
	doneEventList = new HANDLE[threads];


	HANDLE* startEventList;
	startEventList = new HANDLE[threads];

	ThreadArg** threadArgList;
	threadArgList = new ThreadArg*[threads];

	#ifdef DEBUG
	printf("start\r\n");
	#endif

	for(int i=0;i<threads;i++){
		doneEventList[i] = ::CreateEvent(NULL, FALSE, FALSE, NULL);  
		startEventList[i] = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		threadArgList[i] = new ThreadArg();
		threadArgList[i]->id = i;
		threadArgList[i]->done = 0;
		threadArgList[i]->free = 0;
		threadArgList[i]->job = 1;
		threadArgList[i]->doneEvent = &doneEventList[i];
		threadArgList[i]->startEvent = &startEventList[i];
		threadArgList[i]->r_value = new unsigned int[Precision + 2];
		threadArgList[i]->b = new unsigned int[Precision];
		threadArgList[i]->num_b = new unsigned int[Precision];
		threadArgList[i]->num2 = new unsigned int[Precision];

		::CreateThread(NULL, 0, ThreadProc, threadArgList[i], 0, NULL);  
	}

	#ifdef DEBUG
	printf("init done\r\n");
	#endif

	//TODO regular by pi

	//临时空间
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

	int limit = Precision;

	//x2=x*x
	mul(limit, num_x2, &Snum_x2, &Enum_x2, num, Snum, Enum, b);

	int idx = 1;

	int toend = 0;
	while(true)
	{

		while(!toend)
		{
			ThreadArg* freeThread = NULL;

			//取空线程
			for(int i=0;i<threads;i++){
				if(threadArgList[i]->free == 1 && threadArgList[i]->done == 0){
					freeThread = threadArgList[i];
					break;
				}
			}
			
			//有空线程，准备数据
			if(freeThread){
				#ifdef DEBUG
				printf("get free thread %d, %d\r\n",freeThread->id,idx);
				#endif
				//累加
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


				limit = Precision + (Enum_a-Enum_b);
				if(limit > Precision) limit = Precision;
				if(limit <= 0) limit = 1;

				if (limit > 2)
				{
					//result+=num2
					freeThread->limit = limit;

					for(int i=0;i<Precision;i++)
					{
						freeThread->num2[i] = num_a[i];
						freeThread->num_b[i] = num_b[i];
					}
					freeThread->Snum2 = Snum_a;
					freeThread->Enum2 = Enum_a;

					freeThread->Snum_b = Snum_b;
					freeThread->Enum_b = Enum_b;

					freeThread->idx = idx;
					idx++;
					//调用线程
					freeThread->done = 0;
					freeThread->free = 0;

					#ifdef DEBUG
					printf("start free thread %d, %d\r\n",freeThread->id,freeThread->idx);
					#endif

					SetEvent(*(freeThread->startEvent));
				}else{
					toend = 1;
					break;
				}
			}else{
				break;
			}
		}

		//等待完成线程
		::WaitForMultipleObjects(threads, doneEventList, FALSE,100);

		while(true){
			//取完成线程数据
			ThreadArg* doneThread = NULL;

			//取空线程
			for(int i=0;i<threads;i++){
				if(threadArgList[i]->done){
					doneThread = threadArgList[i];
					break;
				}
			}
			if(doneThread){
			
				if (doneThread->idx % 2 == 1)
				{
					doneThread->Snum2 = -(doneThread->Snum2);
				}
				#ifdef DEBUG
				printf("addback %d, %d ",doneThread->id,doneThread->idx);
				#endif

				#ifdef DEBUG
				printf("exp before: %d, %d\r\n",*pExponent,doneThread->Enum2);
				#endif

				add(Precision, Num_Array, pSign, pExponent, doneThread->num2, doneThread->Snum2, doneThread->Enum2);

				#ifdef DEBUG
				printf("exp after: %d, %d\r\n",*pExponent,doneThread->Enum2);
				#endif

				doneThread->done = 0;
			}
			else
			{
				break;
			}
		}

		if(toend){
			int freeThreads = 0;
			for(int i=0;i<threads;i++){
				if(threadArgList[i]->free){
					freeThreads++;
				}
			}
			if(freeThreads == threads){
				break;
			}
		}
	}

	delete b;

	delete num;

	delete num_i;

	delete num_a;

	delete num_b;

	delete num_x2;

	delete doneEventList;

	delete startEventList;

	for(int i=0;i<threads;i++){
		threadArgList[i]->job = 0;
		SetEvent(*(threadArgList[i]->startEvent));

		delete threadArgList[i]->num_b;
		delete threadArgList[i]->num2;
		delete threadArgList[i]->r_value;
		delete threadArgList[i]->b;
		delete threadArgList[i];
	}
	delete threadArgList;
}

extern "C" __declspec(dllexport) void N_SinMT(int* PSE,unsigned int* Num_Array,int threads)
{
	int Precision = PSE[0];
	int Sign = PSE[1];
	int Exponent = PSE[2];

	#ifdef DEBUG
	printf("\r\n");
	#endif
	sinMT(Precision, Num_Array, &Sign, &Exponent, threads);

	PSE[1] = Sign;
	PSE[2] = Exponent;

}