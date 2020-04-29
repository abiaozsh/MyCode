//中断向量
//浮点数

//#define DEBUG

#include "inc/io.h"
#include "inc/system.h"
#include "inc/uart.cpp"
#include "inc/print.cpp"
#include "inc/soft-fp/soft-fp.h"
//#include "inc/soft-fp/clzsi2.c"
//#include "inc/soft-fp/fixdfsi.c"
//#include "inc/soft-fp/muldf3.c"

int main()
{
  //int aa = scanInt();
  //int bb = scanInt();
  //int cc = aa/bb;
  //
  //printInt(cc);

  
  double aa;
  print("aa?\r\n");
  int v1 = scanInt();
  aa=v1?3.6:1.7;
  
  double bb;
  print("bb?\r\n");
  int v2 = scanInt();
  bb=v2?3.6:1.7;

  double cc = aa*bb;
  double dd = aa+bb;
  int val = (int)(cc);
  printInt(val);print("\r\n");
  val = (int)(dd);
  printInt(val);print("\r\n");

  asm("hlt 1");
  int v3 = v1/v2;
  printInt(v3);print("\r\n");
  while(1);
  return 0;
  
}
