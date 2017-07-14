//v1.51
#include <intrin.h>

//int BitScanReverse(unsigned int num)
//{
//	if(num==0)return 0;
//	int y = 32;
//	unsigned int num2 = num;
//	while((num2&0x80000000)==0)
//	{
//		y--;
//		num2 = num2 << 1;
//	}
//	return y;
//}

void div(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent, unsigned int* num_Num_Array, int num_Sign, int num_Exponent, unsigned int* r, unsigned int* b)
{
	if(num_Num_Array[0]==0)throw "divide by zero";

	int cutOff = 0;

	(*pExponent) = (*pExponent) - num_Exponent + 1;
	(*pSign) = (*pSign) * num_Sign;

	//规范化 移位
	int shift;
	unsigned long shift2;
	{
	_BitScanReverse(&shift2, num_Num_Array[0]);
	shift2++;
	}

	//int shift;
	//int shift2;
	//{
	//	shift2 = BitScanReverse(num_Num_Array[0]);
	//}

	shift = 32 - shift2;
	if(shift!=0)
	{
		unsigned int c = num_Num_Array[0];
		for (int i = 0; i < Precision - 1; i++)
		{
			unsigned int d = c;
			c = num_Num_Array[i + 1];
			b[i] = (d << shift) | (c >> shift2);
			if(b[i]!=0)
			{
				cutOff = i;
			}
		}
		b[Precision - 1] = num_Num_Array[Precision - 1] << shift;

		c = Num_Array[0];
		r[0] = 0 | (c >> shift2);
		for (int i = 0; i < Precision; i++)
		{
			unsigned int d = c;
			c = Num_Array[i + 1];
			r[i + 1] = (d << shift) | (c >> shift2);
		}
		r[Precision] = Num_Array[Precision - 1] << shift;
	}
	else
	{
		r[0]=0;
		for (int i = 0; i < Precision; i++)
		{
			b[i] = num_Num_Array[i];
			if(b[i]!=0)
			{
				cutOff = i;
			}
			r[i+1] = Num_Array[i];
		}
	}
	cutOff = Precision - cutOff - 1;

	//运算
	for (int idx_main = 0; idx_main < Precision; idx_main++)
	{

		int len = Precision - (cutOff > idx_main ? cutOff : idx_main);
		
		unsigned int q;//商
		_asm
		{
			mov ecx,idx_main
			mov ebx,r
			mov eax,[ebx + ecx*4 + 4]//rl = r[idx_main + 1]
			mov edx,[ebx + ecx*4]//rh = r[idx_main]
			mov ebx,b//b[0]
			mov ecx,[ebx]
			cmp edx,ecx
			jz else1
				div ecx
				jmp endif1
			else1:
				mov eax,0xFFFFFFFF
			endif1:
			mov q,eax
		}

		// 乘减
		unsigned int* p1 = &r[idx_main];//idx_main + 1 - 1
		unsigned int* p2 = &b[-1];
		_asm
		{
			mov edi,p1
			mov esi,p2
			mov ecx,len
			mov ebx,0
			lp:
				mov eax,q
				cdq
				mul DWORD PTR [esi+ecx*4]
				add eax,ebx
				adc edx,0
				mov ebx,edx
				mov edx,eax
				sub eax,[edi+ecx*4]
				not edx
				neg eax
				mov [edi+ecx*4],eax
				cmp edx,eax
				adc ebx,0
			dec ecx
			jnz lp
			sub [edi],ebx
		}

		//加回 0,1,2 times
		while (r[idx_main]>0x7FFFFFFF)
		{
			_asm
			{
				mov edi,p1
				mov esi,p2
				mov ecx,len
				clc
				lp2:
					mov eax,[edi+ecx*4]
					adc eax,[esi+ecx*4]
					mov [edi+ecx*4],eax
				dec ecx
				jnz lp2
				mov eax,[edi]
				adc eax,0
				mov [edi],eax
			}
			q--;
		}

		Num_Array[idx_main] = q;
	}

	if(Num_Array[0]==0)
	{
		(*pExponent)--;
		for (int idx_main = 0; idx_main < Precision - 1; idx_main++)
		{
			Num_Array[idx_main] = Num_Array[idx_main + 1];
		}
	}
}

void divn(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent, unsigned int n)
{
	_asm
	{
		mov esi,Num_Array
		mov edi,Num_Array//写结果
		mov eax,[esi]
		mov edx,0
		div n
		//mov edx,edx 余数变高部

		cmp eax,0
		mov ebx,pExponent
		jz jp0
			//结果不是0时
			mov [edi],eax//保存结果
			add edi,4//移动指针
			inc DWORD PTR [ebx]//抵消(幂-1)
		jp0:
		//结果是0时
		//不保存结果
		//不移动指针
		dec DWORD PTR [ebx]//幂-1

		add esi,4
		mov ecx,Precision
		dec ecx
		cmp ecx,0
		jz ext
		lp:
			mov eax,[esi]
			div n
			mov [edi],eax
			add esi,4
			add edi,4
		dec ecx
		jnz lp
		ext:
	}
}

extern "C" __declspec(dllexport) void N_Div(int* PSE,unsigned int* Num_Array, int* num_PSE,unsigned int* num_Num_Array)
{
	int Precision = PSE[0];
	int Sign = PSE[1];
	int Exponent = PSE[2];
	int num_Precision = num_PSE[0];
	int num_Sign = num_PSE[1];
	int num_Exponent = num_PSE[2];

	unsigned int* r = new unsigned int[Precision + 1];
	unsigned int* b = new unsigned int[Precision];

	div(Precision, Num_Array, &Sign, &Exponent, num_Num_Array, num_Sign, num_Exponent, r, b);

	delete r;
	delete b;

	PSE[1] = Sign;
	PSE[2] = Exponent;
}

extern "C" __declspec(dllexport) void N_Divn(int* PSE,unsigned int* Num_Array, unsigned int n)
{
	int Precision = PSE[0];
	int Sign = PSE[1];
	int Exponent = PSE[2];

	divn(Precision, Num_Array, &Sign, &Exponent, n);

	PSE[1] = Sign;
	PSE[2] = Exponent;
}
