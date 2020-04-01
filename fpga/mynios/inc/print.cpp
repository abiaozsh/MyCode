
int print(char* str){
  int idx = 0;
  while(1){
    char tmp = str[idx];
    if(tmp=='\0')break;
    uart_write(tmp);
    idx++;
  }
}

int print(char* str, int len){
  int idx = 0;
  int i;
  for(i=0;i<len;i++){
    char tmp = str[i];
    if(tmp=='\0')break;
    uart_write(tmp);
  }
}

int scan(char* buff, int maxlen, int timeout){
  int idx = 0;
  while(1){
    int c = uart_read(timeout);
    if(c==-1){
      return 0;
    }
    if(maxlen>=0){
      if(idx>=maxlen){
        buff[idx] = (char)0;
        return 0;
      }
    }
    buff[idx] = (char)c;
    if(c==0){
      return 0;
    }
    idx++;
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
  if(val>=0){
    num = val;
    uart_write('+');
  }else{
    num = -val;
    uart_write('-');
  }
  
  int outNum;
  int flg = 0;
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
    if(flg==1){
      uart_write('0' + outNum);
    }else{
      if(outNum!=0){
        uart_write('0' + outNum);
        flg = 1;
      }
    }
  }
}

int printByte(int val){
  char* chardata = "0123456789ABCDEF";
  uart_write(chardata[(val>>4)&0x0F]);
  uart_write(chardata[(val)&0x0F]);
}
