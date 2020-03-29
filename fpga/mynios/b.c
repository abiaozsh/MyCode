//函数指针
//变量指针
//中断向量
//浮点数
#include "inc/io.h"
#include "inc/system.h"

int uart_read(int timeout){
  if(timeout!=-1){
    IOWR(MYTIMER, 0 ,0);
  }
  while(1){
    if(timeout!=-1){
      int timer = IORD(MYTIMER, 0);
      if(timer>1000000){
        return 0;
      }
    }
    int tmp = IORD(MYUART, 0);
    if(tmp&0x100){
      return tmp;
    }
  }
}

int uart_write(int val){
  while((IORD(MYUART, 1)) & 0x100);
  IOWR(MYUART, 1 ,val);
}

int myprintf(char* str){
  int idx = 0;
  while(1){
    char tmp = str[idx];
    if(tmp=='\0')break;
    uart_write(tmp);
    idx++;
  }
}

int myprintf2(char* str, int len){
  int idx = 0;
  int i;
  for(i=0;i<len;i++){
    char tmp = str[i];
    if(tmp=='\0')break;
    uart_write(tmp);
  }
}


int num10s[] = {
1000000000,
100000000,
10000000,
1000000,
100000,
10000,
1000,
100,
10,
1,
};

void printInt(int val)
{
  int idx;
  int i;
  int num;
  if(val<0){
    num = val;
    uart_write('+');
  }else{
    num = -val;
    uart_write('-');
  }
	
	int outNum;
	for(idx = 0; idx < 10 ; idx++)
	{
		outNum = 0;
		int CmpNum = num10s[idx];
		for(i = 0; i < 10 ; i++)
		{
			if(num>=CmpNum)
			{
				num -= CmpNum;
				outNum++;
			}
			else
			{
				break;
			}
		}
    uart_write('0' + outNum);
	}
}


//int b[1000000];
char* bb = "bbbbaaaaa";

int main(){
  while(1){

    char buff[5];
    buff[0] = (char)uart_read(-1);
    buff[1] = (char)uart_read(-1);
    buff[2] = (char)uart_read(-1);
    buff[3] = (char)uart_read(-1);
    buff[4] = 0;
    //b[999999] = 1;
    printInt(123);
    myprintf("\r\n");
    printInt(buff[0]);
    myprintf("\r\n");

    myprintf(buff);
    myprintf("\r\n");
    myprintf("fdsa");
    myprintf("\r\n");
    myprintf2(bb, 5);
    myprintf("\r\n");
  }
}
