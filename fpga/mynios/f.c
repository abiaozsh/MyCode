//函数指针
//变量指针
//中断向量
//浮点数
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "soft-fp\\soft-fp.h"
#include "soft-fp\\double.h"

DFtype __adddf3(DFtype a, DFtype b)
{
  FP_DECL_EX;
  FP_DECL_D(A); FP_DECL_D(B); FP_DECL_D(R);
  DFtype r;

  FP_INIT_ROUNDMODE;
  FP_UNPACK_SEMIRAW_D(A, a);
  FP_UNPACK_SEMIRAW_D(B, b);
  FP_ADD_D(R, A, B);
  FP_PACK_SEMIRAW_D(r, R);
  FP_HANDLE_EXCEPTIONS;

  return r;
}


int __main(){
  return;
}

int main(){

  fdsa();
  
  int* a = (int*)123;
  int* b = (int*)222;
  
  memcpy(a, b, 123);
  
}

