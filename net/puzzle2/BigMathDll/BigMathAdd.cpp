//v1.3asm

#define int __int64

//add
extern "C" __declspec(dllexport) void N_Add(int* Data, int* Length, int* Idx, int* result)
{
	for (int times = 0; times < 100000000; times++)
	{
		int board = 0;
		int last = 0;
		int error = 0;
		for (int i = 0; i < 12; i++)
		{
			int mask = Data[i*256+Idx[i]];
			if ((board & mask) == 0)
			{
				board |= mask;
			}
			else
			{
				last = i;
				error = 1;
				break;
			}
		}
		if (error != 0)
		{
			if (last < 11)
			{
				Idx[last + 1] = 0;
			}
			for (int i = last; i >= 0; i--)
			{
				Idx[i]++;
				if (Idx[i] >= Length[i])
				{
					Idx[i] = 0;
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			result[0] = 1;
			return;
		}
	}
	result[0] = 0;
	return;
}