//v1.3asm

#define int __int64

// 加法，被加数高幂＋加数低幂 或者 等幂相加 100 + 10 －>110
extern "C" int addA(unsigned int* Num_Array, unsigned int* num_Num_Array, int cnt, int disExp);
// 低幂加高幂 10 + 100  例：0.01x + 0.10 -> 0.11 ， x被丢弃
extern "C" int addB(unsigned int* Num_Array, unsigned int* num_Num_Array, int cnt, int disExp);
//高幂减低幂 100 - 10
extern "C" void subA(unsigned int* Num_Array, unsigned int* num_Num_Array, int cnt, int disExp);
//低幂减高幂 10 - 100
extern "C" void subB(unsigned int* Num_Array, unsigned int* num_Num_Array, int cnt, int disExp);
//99-88 
extern "C" void subC(unsigned int* Num_Array, unsigned int* num_Num_Array, int cnt);
//88-99
extern "C" void subD(unsigned int* Num_Array, unsigned int* num_Num_Array, int cnt);

extern "C" void incr(unsigned int* Num_Array, int* pExponent);

extern "C" void add(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent, unsigned int* num_Num_Array, int num_Sign, int num_Exponent)
{
	//符号相等 加法
	if ((*pSign) == num_Sign)
	{
		if ((*pExponent) >= num_Exponent)
		{
			//幂差
			int disExp = (*pExponent) - num_Exponent;

			//低幂足够大，使加法有意义
			if (disExp < Precision)
			{
				//叠合部分
				int cnt = Precision - disExp;

				//进位标志
				int carry = addA(Num_Array, num_Num_Array, cnt, disExp);

				//全部结束后仍有进位
				if (carry == 1)
				{
					//全体向后移一位
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
			int disExp = num_Exponent - (*pExponent);

			// 两数偏差小于精度,加法有意义
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
			else // 加法无意义，两数阶差太大，将加数赋给被加数
			{
				for (int i = 0; i < Precision; i++)
				{
					Num_Array[i] = num_Num_Array[i];
				}
				(*pExponent) = num_Exponent;
			}
		}
	}
	else// 符号相反 减法
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
	//符号相反
	if (Sign != num_Sign)
	{
		return Sign;//1 比 -1, -1 比 1
	}
	else// 符号等，再判
	{
		if (Exponent > num_Exponent)
		{
			//10 比 1
			//-10 比 -1
			return Sign;
		}
		else if (Exponent < num_Exponent)
		{
			//1 比 10
			//-1 比 -10
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
				//2 比 1
				//-2 比 -1
				return Sign;
			}
			else if (f_comp == -1)
			{
				//1 比 2
				//-1 比 -2
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

	add(Precision, Num_Array, &Sign, &Exponent, num_Num_Array, num_Sign, num_Exponent);

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