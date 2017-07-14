#pragma once
//v1.31
void mul(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent, unsigned int* num_Num_Array, int num_Sign, int num_Exponent, unsigned int* buf_Num_Array)
{
	int cutOff = 0;

	for (int i = 0; i < Precision; i++)
	{
		buf_Num_Array[i] = Num_Array[i];
		if(Num_Array[i]!=0)
		{
			cutOff = i;
		}
		Num_Array[i] = 0;
	}
	cutOff = Precision - cutOff - 1;

	int temp_Exponent = (*pExponent) + num_Exponent;
	*pExponent = 0;

	*pSign = (*pSign) * num_Sign;

	for (int idxMain = 0; idxMain < Precision; idxMain++)
	{
		int n = num_Num_Array[idxMain];

		if (n!=0)
		{
			int shift = idxMain + (*pExponent);//(*pExponent)修正量
			int cnt = Precision - (cutOff > shift ? cutOff : shift);

			int carry = 0;
			if (cnt>0)
			{
				unsigned int* p0 = &Num_Array[shift - 1];
				unsigned int* p1 = &buf_Num_Array[-1];
				_asm
				{
					mov ecx,cnt
					mov edi,p0
					mov esi,p1
					mov ebx,0
					lp:
						mov eax,n
						cdq
						mul DWORD PTR [esi+ecx*4] // low:eax,high:edx
						add eax,[edi+ecx*4]
						adc edx,0
						add eax,ebx
						adc edx,0
						mov [edi+ecx*4],eax
						mov ebx,edx
					dec ecx
					jnz lp
					mov carry,ebx
				}
			}

			//继续进位
			if (shift>0)
			{
				unsigned int* p0 = &Num_Array[-1];
				_asm
				{
					mov ecx,shift
					mov edi,p0
					mov edx,carry
					lp2:
						mov eax,[edi+ecx*4]
						add eax,edx
						mov [edi+ecx*4],eax
						mov edx,0
						jnc ext
						mov edx,1
					dec ecx
					jnz lp2
					ext:
					mov carry,edx
				}
			}

			if (carry!=0)
			{
				(*pExponent)++;
				for (int i = Precision - 1; i > 0; i--)
				{
					Num_Array[i] = Num_Array[i - 1];
				}
				Num_Array[0] = carry;

			}
		}
	}

	(*pExponent) += temp_Exponent - 1;
}

//mul
extern "C" __declspec(dllexport) void N_Mul(int* PSE,unsigned int* Num_Array, int* num_PSE,unsigned int* num_Num_Array)
{
	int Precision = PSE[0];
	int Sign = PSE[1];
	int Exponent = PSE[2];
	int num_Precision = num_PSE[0];
	int num_Sign = num_PSE[1];
	int num_Exponent = num_PSE[2];

	unsigned int* buf_Num_Array = new unsigned int[Precision];

	mul(Precision, Num_Array, &Sign, &Exponent, num_Num_Array, num_Sign, num_Exponent, buf_Num_Array);

	delete buf_Num_Array;
	PSE[1] = Sign;
	PSE[2] = Exponent;
}

void muln(int Precision, unsigned int* Num_Array, int* pExponent, unsigned int n)
{
	int temp_Exponent = *pExponent;

	(*pExponent) = 0;

	int cnt = Precision;

	unsigned int* p0 = &Num_Array[-1];
	int carry = 0;
	_asm
	{
		mov ecx,cnt
		mov edi,p0
		mov ebx,0
		lp:
			mov eax,n
			cdq
			mul DWORD PTR [edi+ecx*4] // l:eax,h:edx
			add eax,ebx // + carry
			adc edx,0
			mov [edi+ecx*4],eax
			mov ebx,edx
		dec ecx
		jnz lp
		mov carry,ebx
	}

	if (carry!=0)
	{
		(*pExponent)++;
		for (int i = Precision - 1; i > 0; i--)
		{
			Num_Array[i] = Num_Array[i - 1];
		}
		Num_Array[0] = carry;

	}

	(*pExponent) += temp_Exponent;
}

//mul n
extern "C" __declspec(dllexport) void N_Muln(int* PSE,unsigned int* Num_Array, unsigned int n)
{
	int Precision = PSE[0];
	int Exponent = PSE[2];

	muln(Precision, Num_Array, &Exponent, n);

	PSE[2] = Exponent;
}
