#pragma once
//v1.3asm

//The terms ��less�� and ��greater�� are used for comparisons of signed integers.
//The terms ��above�� and ��below�� are used for unsigned integers.

// �ӷ������������ݣ��������� ���� ������� 100 + 10 ��>110
int addA(unsigned int* Num_Array, unsigned int* num_Num_Array, int cnt, int disExp)
{
	int carry = 0;

	//���ϲ��ּӷ�
	unsigned int* p1 = &Num_Array[disExp - 1];
	unsigned int* p2 = &num_Num_Array[-1];
	_asm
	{
		//λ��
		mov ecx,cnt
		//������
		mov edi,p1
		//����
		mov esi,p2
		clc
		lp:
			//���뱻����
			mov eax,[edi+ecx*4]
			//������+����+��λ��־
			adc eax,[esi+ecx*4]
			//������
			mov [edi+ecx*4],eax
		dec ecx
		jnz lp
		mov edx,0
		adc edx,0
		//������λ��־
		mov carry,edx
	}


	//�������Ͻ�λ
	if (disExp>0)
	{
		unsigned int* p1 = &Num_Array[-1];
		_asm
		{
			mov ecx,disExp
			mov edi,p1
			//�����λ��־
			mov edx,carry
			rcr edx,1
			lpb:
				//���뱻����
				mov eax,[edi+ecx*4]
				//�ӽ�λ��־
				adc eax,0
				//������
				mov [edi+ecx*4],eax
				jnc ext
			dec ecx
			jnz lpb
			ext:
			mov edx,0
			adc edx,0
			mov carry,edx
		}
	}
	return carry;
}

// ���ݼӸ��� 10 + 100  ����0.01x + 0.10 -> 0.11 �� x������
int addB(unsigned int* Num_Array, unsigned int* num_Num_Array, int cnt, int disExp)
{
	int carry = 0;

	//���ϲ��ּӷ�
	{
		unsigned int* p1 = &Num_Array[-1];
		unsigned int* p2 = &num_Num_Array[disExp - 1];
		unsigned int* p3 = &Num_Array[disExp - 1];
		_asm
		{
			mov ecx,cnt
			//�������߲�
			mov edi,p1
			//����
			mov esi,p2
			//�������Ͳ���ԭ���ݱ�����
			mov ebx,p3
			//�����λ��־
			clc
			lp:
				//���뱻�����߲�
				mov eax,[edi+ecx*4]
				//������+����+��λ��־
				adc eax,[esi+ecx*4]
				//���浽�������Ͳ�
				mov [ebx+ecx*4],eax
			dec ecx
			jnz lp
			//������λ��־
			mov edx,0
			adc edx,0
			mov carry,edx
		}
	}

	//�������Ͻ�λ
	{
		unsigned int* p1 = &Num_Array[-1];
		unsigned int* p2 = &num_Num_Array[-1];
		_asm
		{
			mov ecx,disExp
			mov edi,p1
			mov esi,p2
			mov edx,carry
			rcr edx,1;�����λ��־
			lpb:
				mov eax,[esi+ecx*4]
				adc eax,0
				mov [edi+ecx*4],eax
			dec ecx
			jnz lpb
			mov edx,0
			adc edx,0
			mov carry,edx
		}
	}
	return carry;
}

//���ݼ����� 100 - 10
void subA(unsigned int* Num_Array, unsigned int* num_Num_Array, int cnt, int disExp)
{
	int carry = 0;

	{
		unsigned int* p1 = &Num_Array[disExp - 1];
		unsigned int* p2 = &num_Num_Array[-1];
		_asm
		{
			mov ecx,cnt
			mov edi,p1
			mov esi,p2
			clc
			lp:
				mov eax,[edi+ecx*4]
				sbb eax,[esi+ecx*4]
				mov [edi+ecx*4],eax
			dec ecx
			jnz lp
			mov edx,0
			adc edx,0
			mov carry,edx
		}
	}

	{
		unsigned int* p1 = &Num_Array[-1];
		_asm
		{
			mov ecx,disExp
			mov edi,p1
			mov edx,carry
			rcr edx,1;�����λ��־
			lpb:
				mov eax,[edi+ecx*4]
				sbb eax,0
				mov [edi+ecx*4],eax
				jnc ext
			dec ecx
			jnz lpb
			ext:
		}
	}
}

//10 - 100
void subB(unsigned int* Num_Array, unsigned int* num_Num_Array, int cnt, int disExp)
{
	int carry = 0;
	{
		unsigned int* p1 = &Num_Array[-1];
		unsigned int* p2 = &num_Num_Array[disExp - 1];
		unsigned int* p3 = &Num_Array[disExp - 1];
		_asm
		{
			mov ecx,cnt
			mov edi,p1
			mov esi,p2
			mov ebx,p3
			clc
			lp:
				mov eax,[esi+ecx*4]
				sbb eax,[edi+ecx*4]
				mov [ebx+ecx*4],eax
			dec ecx
			jnz lp
			//last one
			mov edx,0
			adc edx,0
			mov carry,edx
		}
	}

	{
		unsigned int* p1 = &Num_Array[-1];
		unsigned int* p2 = &num_Num_Array[-1];
		_asm
		{
			mov ecx,disExp
			mov edi,p1
			mov esi,p2
			mov edx,carry
			rcr edx,1;�����λ��־
			lpb:
				mov eax,[esi+ecx*4]
				sbb eax,0
				mov [edi+ecx*4],eax
			dec ecx
			jnz lpb
		}
	}
}

//99-88
void subC(unsigned int* Num_Array, unsigned int* num_Num_Array, int cnt)
{
	unsigned int* p1 = &Num_Array[-1];
	unsigned int* p2 = &num_Num_Array[-1];
	_asm
	{
		mov ecx,cnt
		mov edi,p1
		mov esi,p2
		clc
		lp:
			mov eax,[edi+ecx*4]
			sbb eax,[esi+ecx*4]
			mov [edi+ecx*4],eax
		dec ecx
		jnz lp
	}

}

//88-99
int subD(unsigned int* Num_Array, unsigned int* num_Num_Array, int cnt)
{
	unsigned int* p1 = &Num_Array[-1];
	unsigned int* p2 = &num_Num_Array[-1];
	_asm
	{
		mov ecx,cnt
		mov edi,p1
		mov esi,p2
		clc
		lp:
			mov eax,[esi+ecx*4]
			sbb eax,[edi+ecx*4]
			mov [edi+ecx*4],eax
		dec ecx
		jnz lp
	}
}

void add(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent, unsigned int* num_Num_Array, int num_Sign, int num_Exponent)
{
	//������� �ӷ�
	if ((*pSign) == num_Sign)
	{
		if ((*pExponent) >= num_Exponent)
		{
			//�ݲ�
			int disExp = (*pExponent) - num_Exponent;

			//�����㹻��ʹ�ӷ�������
			if (disExp<Precision)
			{
				//���ϲ���
				int cnt = Precision - disExp;

				//��λ��־
				int carry = addA(Num_Array, num_Num_Array, cnt, disExp);

				//ȫ�����������н�λ
				if (carry == 1)
				{
					//ȫ�������һλ
					(*pExponent)++;
					for (int i = Precision - 1; i > 0; i--)
					{
						Num_Array[i] = Num_Array[i - 1];
					}
					Num_Array[0] = 1;
				}
			}
		}
		else
		{
			int disExp = num_Exponent - *pExponent;

			// ����ƫ��С�ھ���,�ӷ�������
			if (disExp < Precision)
			{
				int cnt = Precision - disExp;

				int carry = addB(Num_Array, num_Num_Array, cnt, disExp);

				if (carry == 1)
				{
					(*pExponent) = num_Exponent + 1;
					for (int i = Precision - 1; i > 0; i--)
					{
						Num_Array[i] = Num_Array[i - 1];
					}
					Num_Array[0] = 1;
				}
				else
				{
					(*pExponent) = num_Exponent;
				}
			}
			else // �ӷ������壬�����ײ�̫�󣬽���������������
			{
				for (int i = 0; i < Precision; i++)
				{
					Num_Array[i] = num_Num_Array[i];
				}
				(*pExponent) = num_Exponent;
			}
		}
	}
	else// �����෴ ����
	{
		if ((*pExponent) > num_Exponent)
		{
			int disExp = (*pExponent) - num_Exponent;

			if (disExp < Precision)
			{
				int cnt = Precision - disExp;

				subA(Num_Array, num_Num_Array, cnt, disExp);

				int f_exp = 0;
				for (int i = 0; i < Precision; i++)
				{
					if (Num_Array[i] != 0)
					{
						f_exp = i;
						break;
					}
				}
				if (f_exp != 0)
				{
					(*pExponent) -= f_exp;
					for (int i = 0; i < Precision - f_exp; i++)
					{
						Num_Array[i] = Num_Array[i + f_exp];
					}
					for (int i = Precision - f_exp; i < Precision; i++)
					{
						Num_Array[i] = 0;
					}
				}
			}
		}
		else if (num_Exponent > (*pExponent))
		{
			int disExp = num_Exponent - (*pExponent);

			if (disExp < Precision)
			{
				int cnt = Precision - disExp;

				subB(Num_Array, num_Num_Array, cnt, disExp);

				int f_exp = 0;
				for (int i = 0; i < Precision; i++)
				{
					if (Num_Array[i] != 0)
					{
						f_exp = i;
						break;
					}
				}
				if (f_exp != 0)
				{
					(*pExponent) = num_Exponent - f_exp;
					for (int i = 0; i < Precision - f_exp; i++)
					{
						Num_Array[i] = Num_Array[i + f_exp];
					}
					for (int i = Precision - f_exp; i < Precision; i++)
					{
						Num_Array[i] = 0;
					}
				}
				else
				{
					(*pExponent) = num_Exponent;
				}
			}
			else // ���������壬�����ײ�̫�󣬽���������������
			{
				for (int i = 0; i < Precision; i++)
				{
					Num_Array[i] = num_Num_Array[i];
				}
				(*pExponent) = num_Exponent;
			}
			(*pSign) = -(*pSign);
		}
		else// (Exponent == num_Exponent)
		{
			int f_comp = 0;
			for (int i = 0; i < Precision; i++)
			{
				if (Num_Array[i] > num_Num_Array[i])
				{
					f_comp = 1;
					break;
				}
				if (Num_Array[i] < num_Num_Array[i])
				{
					f_comp = -1;
					break;
				}
			}
			if (f_comp == 1)
			{
				subC(Num_Array, num_Num_Array, Precision);
				int f_exp = 0;
				for (int i = 0; i < Precision; i++)
				{
					if (Num_Array[i] != 0)
					{
						f_exp = i;
						break;
					}
				}
				if (f_exp != 0)
				{
					*pExponent -= f_exp;
					for (int i = 0; i < Precision - f_exp; i++)
					{
						Num_Array[i] = Num_Array[i + f_exp];
					}
					for (int i = Precision - f_exp; i < Precision; i++)
					{
						Num_Array[i] = 0;
					}
				}
			}
			else if (f_comp == -1)
			{
				subD(Num_Array, num_Num_Array, Precision);
				int f_exp = 0;
				for (int i = 0; i < Precision; i++)
				{
					if (Num_Array[i] != 0)
					{
						f_exp = i;
						break;
					}
				}
				if (f_exp != 0)
				{
					*pExponent -= f_exp;
					for (int i = 0; i < Precision - f_exp; i++)
					{
						Num_Array[i] = Num_Array[i + f_exp];
					}
					for (int i = Precision - f_exp; i < Precision; i++)
					{
						Num_Array[i] = 0;
					}
				}
				(*pSign) = -(*pSign);
			}
			else if (f_comp == 0)
			{
				(*pSign) = 1;
				(*pExponent) = 0;
				for (int i = 0; i < Precision; i++)
				{
					Num_Array[i] = 0;
				}
			}
		}
	}
}

extern "C" int comp(int Precision, unsigned int* Num_Array, int Sign, int Exponent, unsigned int* num_Num_Array, int num_Sign, int num_Exponent)
{
	//�����෴
	if (Sign != num_Sign)
	{
		return Sign;//1 �� -1, -1 �� 1
	}
	else// ���ŵȣ�����
	{
		if (Exponent > num_Exponent)
		{
			//10 �� 1
			//-10 �� -1
			return Sign;
		}
		else if (Exponent < num_Exponent)
		{
			//1 �� 10
			//-1 �� -10
			return -Sign;
		}
		else// (Exponent == num_Exponent)
		{
			int f_comp = 0;
			for (int i = 0; i < Precision; i++)
			{
				if (Num_Array[i] > num_Num_Array[i])
				{
					f_comp = 1;
					break;
				}
				if (Num_Array[i] < num_Num_Array[i])
				{
					f_comp = -1;
					break;
				}
			}
			if (f_comp == 1)
			{
				//2 �� 1
				//-2 �� -1
				return Sign;
			}
			else if (f_comp == -1)
			{
				//1 �� 2
				//-1 �� -2
				return -Sign;
			}
			else if (f_comp == 0)
			{
				return 0;
			}
		}
	}
}

//add
extern "C" __declspec(dllexport) void N_Add(int* PSE,unsigned int* Num_Array, int* num_PSE,unsigned int* num_Num_Array)
{
	int Precision = PSE[0];
	int Sign = PSE[1];
	int Exponent = PSE[2];
	int num_Precision = num_PSE[0];
	int num_Sign = num_PSE[1];
	int num_Exponent = num_PSE[2];

	if (Precision == num_Precision)
	{
		add(Precision, Num_Array, &Sign, &Exponent, num_Num_Array, num_Sign, num_Exponent);
	}

	PSE[1] = Sign;
	PSE[2] = Exponent;
}

//add
extern "C" __declspec(dllexport) int N_Comp(int* PSE,unsigned int* Num_Array, int* num_PSE,unsigned int* num_Num_Array)
{
	int Precision = PSE[0];
	int Sign = PSE[1];
	int Exponent = PSE[2];

	int num_Precision = num_PSE[0];
	int num_Sign = num_PSE[1];
	int num_Exponent = num_PSE[2];

	return comp(Precision, Num_Array, Sign, Exponent, num_Num_Array, num_Sign, num_Exponent);

	return 0;
}
void inc(unsigned int* Num_Array, int* pExponent)
{
	//int carry = 0;
	_asm
	{
		//int cnt = *pExponent;
		mov ebx,pExponent
		mov ecx,[ebx]
		cmp ecx,0
		//if (cnt>0)
		//{
		jle jp1
			//unsigned int* p1 = &Num_Array[-1];
			mov edi,Num_Array
			sub edi,4
			stc
			lpa:
				mov eax,[edi+ecx*4]
				adc eax,0
				mov [edi+ecx*4],eax
				jnc ext
			dec ecx
			jnz lpa
			ext:
			mov edx,0
			adc edx,0
			//mov carry,edx
			//}
		jp1:

		cmp edx,1
		//if (carry == 1)
		//{
		jne jp2
			//*pExponent++;
			inc [ebx]

			//for (int i = *pExponent - 1; i > 0; i--)
			//{
			//	Num_Array[i] = Num_Array[i - 1];
			//}
			mov ecx,[ebx]
			dec ecx
			//mov edi,Num_Array
			mov esi,edi
			add edi,4
			lpb:
				mov eax,[esi+ecx*4]
				mov [edi+ecx*4],eax
			dec ecx
			jnz lpb
			//Num_Array[0] = 1;
			mov [edi],1
		jp2:
	}
}

void addn(unsigned int* Num_Array, int* pExponent, unsigned int n)
{
	//int carry = 0;
	_asm
	{
		//int cnt = *pExponent;
		mov ebx,pExponent
		mov ecx,[ebx]
		cmp ecx,0
		//if (cnt>0)
		//{
		jle jp1
			//unsigned int* p1 = &Num_Array[-1];
			mov edi,Num_Array
			sub edi,4
			clc
			mov edx,n
			lpa:
				mov eax,[edi+ecx*4]
				adc eax,edx
				mov [edi+ecx*4],eax
				jnc ext
				mov edx,0
			dec ecx
			jnz lpa
			ext:
			mov edx,0
			adc edx,0
			//mov carry,edx
			//}
		jp1:

		cmp edx,1
		//if (carry == 1)
		//{
		jne jp2
			//*pExponent++;
			inc [ebx]

			//for (int i = *pExponent - 1; i > 0; i--)
			//{
			//	Num_Array[i] = Num_Array[i - 1];
			//}
			mov ecx,[ebx]
			dec ecx
			//mov edi,Num_Array
			mov esi,edi
			add edi,4
			lpb:
				mov eax,[esi+ecx*4]
				mov [edi+ecx*4],eax
			dec ecx
			jnz lpb
			//Num_Array[0] = 1;
			mov [edi],1
		jp2:
	}
}

//add
extern "C" __declspec(dllexport) void N_Inc(int* PSE,unsigned int* Num_Array)
{
	int Precision = PSE[0];
	int Sign = PSE[1];
	int Exponent = PSE[2];

	inc(Num_Array, &Exponent);

	PSE[2] = Exponent;
}

//add
extern "C" __declspec(dllexport) void N_Addn(int* PSE,unsigned int* Num_Array, unsigned int n)
{
	int Precision = PSE[0];
	int Sign = PSE[1];
	int Exponent = PSE[2];

	addn(Num_Array, &Exponent, n);

	PSE[2] = Exponent;
}
