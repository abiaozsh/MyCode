//����ָ��
//����ָ��
//�ж�����
//������
#include "inc/io.h"
#include "inc/system.h"
#include "inc/uart.cpp"
#include "inc/print.cpp"


//int b[1000000];
char* bb = "bbbbaaaaa";

char* sdrambuff;

int main(){
  sdrambuff = (char*)(0);
  print("Hello from Nios II! demo b\r\n");
  while(1){
    scan(sdrambuff,-1,-1);
    print(sdrambuff);
    continue;
    char buff[5];
    scan(buff,4,-1);
    print(buff);
    
    //b[999999] = 1;
    print("\r\n");
    printInt(123);
    print("\r\n");
    printInt(buff[0]);
    print(" ");
    printByte(buff[0]);
    print("\r\n");
    
    printInt(buff[1]);
    print(" ");
    printByte(buff[1]);
    print("\r\n");
    
    print("\r\n");
    print("fdsa");
    print("\r\n");
    print(bb, 5);
    print("\r\n");
  }
}
