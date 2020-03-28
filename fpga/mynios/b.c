//函数指针
//变量指针
//中断向量
//浮点数

//void interruptCall(){
//  push all reg
//  xor rc, rc
//  
//  pop all reg
//}

int __main(){
  return;
}

int out32(int addr,int val){
  asm("mov rb, DWORD PTR [ebp+8]");//addr
  asm("mov ra, DWORD PTR [ebp+12]");//val
  asm("out32 ra, rb");
}
int in32(int addr){
  asm("mov rb, DWORD PTR [ebp+8]");//addr
  asm("in32 eax, rb");
}

int uart_read(){
  while(1){
    int tmp = in32(0x02002000);
    if(tmp&0x100){
      return tmp;
    }
  }
}

int uart_write(int val){
  while((in32(0x02002004)) & 0x100);
  out32(0x02002004,val);
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
  if(val<0){
    uart_write('+');
  }else{
    uart_write('-');
  }
	int num = val;
	for(idx = 0; idx < 10 ; idx++)
	{
		int outNum = 0;
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


int b[1000000];
char* bb = "bbbbaaaaa";

int main(){
  while(1){

    char buff[5];
    buff[0] = (char)uart_read();
    buff[1] = (char)uart_read();
    buff[2] = (char)uart_read();
    buff[3] = (char)uart_read();
    buff[4] = 0;
    b[999999] = 1;
    //printInt(buff[0]);
    
    myprintf(buff);
    myprintf("fdsa");
    myprintf2(bb, 5);
  }
}
