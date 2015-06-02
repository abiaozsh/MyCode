//v1.51

#include "intrin.h"

#define int __int64

extern "C" int divcore(unsigned int* r, unsigned int* b, int idx_main, int len);

extern "C" void divi(int Precision, unsigned int* Num_Array, int* pSign, int* pExponent, unsigned int* num_Num_Array, int num_Sign, int num_Exponent, unsigned int* r, unsigned int* b)
{
    //if(num_Num_Array[0]==0)throw "divide by zero";
    (*pExponent) = (*pExponent) - num_Exponent + 1;
    (*pSign) = (*pSign) * num_Sign;

    //规范化 移位
    int shift;
    unsigned long shift2;

    _BitScanReverse64(&shift2, num_Num_Array[0]);

    shift2++;
    shift = 64 - shift2;
   
    int cutOff = 0;
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
        for (int i = 0; i < Precision; i++)
        {
            b[i] = num_Num_Array[i];
            if(b[i]!=0)
            {
                cutOff = i;
            }
        }

        r[0]=0;
        for (int i = 0; i < Precision; i++)
        {
            r[i+1] = Num_Array[i];
        }
    }
    cutOff = Precision - cutOff - 1;

    //运算
    for (int idx_main = 0; idx_main < Precision; idx_main++)
    {
        int len = Precision - (cutOff > idx_main ? cutOff : idx_main);

        int q = divcore(r, b, idx_main, len);

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

    divi(Precision, Num_Array, &Sign, &Exponent, num_Num_Array, num_Sign, num_Exponent, r, b);

    delete r;
    delete b;

    PSE[1] = Sign;
    PSE[2] = Exponent;
}

extern "C" void divn(int Precision, unsigned int* Num_Array, int* pExponent, unsigned int n);

extern "C" __declspec(dllexport) void N_Divn(int* PSE,unsigned int* Num_Array, unsigned int n)
{
    int Precision = PSE[0];
    int Sign = PSE[1];
    int Exponent = PSE[2];

    divn(Precision, Num_Array, &Exponent, n);

    PSE[2] = Exponent;
}