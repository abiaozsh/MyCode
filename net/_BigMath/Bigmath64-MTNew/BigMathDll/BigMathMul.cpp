//v1.31
#define int __int64

extern "C" int mulcore(unsigned int* Num_Array,unsigned int* buf_Num_Array, int cnt, int shift, int n);

extern "C" void mul(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent, unsigned int* num_Num_Array, int num_Sign, int num_Exponent, unsigned int* buf_Num_Array)
{
    int cutOff = 0;

    for (int i = 0; i < Precision; i++)
    {
        buf_Num_Array[i] = Num_Array[i];
        if(buf_Num_Array[i]!=0)
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
            int shift = idxMain + (*pExponent);//(*pExponent)ÐÞÕýÁ¿
            int cnt = Precision - (cutOff > shift ? cutOff : shift);

            int carry = mulcore(Num_Array, buf_Num_Array, cnt, shift, n);

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

extern "C" void muln(int Precision, unsigned int* Num_Array, int* pExponent, unsigned int n);

//mul n
extern "C" __declspec(dllexport) void N_Muln(int* PSE, unsigned int* Num_Array, unsigned int n)
{
    int Precision = PSE[0];
    int Exponent = PSE[2];

    muln(Precision, Num_Array, &Exponent, n);

    PSE[2] = Exponent;
}