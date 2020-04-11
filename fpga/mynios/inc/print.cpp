
int print(const char* str){
  int idx = 0;
  while(1){
    char tmp = str[idx];
    if(tmp=='\0')break;
    uart_write(tmp);
    idx++;
  }
}

int print(const char* str, int len){
  int idx = 0;
  int i;
  for(i=0;i<len;i++){
    char tmp = str[i];
    uart_write(tmp);
  }
}

int equal(const char* a,const  char* b, int maxlen){
  int i = 0;
  while(1){
    if(a[i]!=b[i]){
      return 0;
    }
    if(a[i]=='\0' && b[i]=='\0'){
      break;
    }
    if(a[i]=='\0' || b[i]=='\0'){
      return 0;
    }
    
    i++;
    if(maxlen>0){
      if(i>=maxlen){
        break;
      }
    }
  }
  return 1;
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
  
  uart_write('0');
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

void printInt(int val, int newline){
  printInt(val);
  uart_write('\r');
  uart_write('\n');
}

int scanInt(){
  char buff[10];
  int i;
  int j;
  int idx = 0;
  int neg = 0;
  for(i = 0; i < 10 ; i++)
  {
    char v = uart_read();
    if(i==0 && v=='-'){
      neg = 1;
    }else{
      if(v<'0'||v>'9'){
        break;
      }
      buff[idx++] = v;
    }
  }
  
  int val = 0;
  
  for(i = 0; i < idx ; i++)
  {
    for(j = 0; j < (int)(buff[i]-'0') ; j++)
    {
      val+=num10s[10-idx+i];
    }
  }
  if(neg){
    val=-val;
  }
  return val;
}

int printByte(int val){
  char* chardata = "0123456789ABCDEF";
  uart_write(chardata[(val>>4)&0x0F]);
  uart_write(chardata[(val)&0x0F]);
}



void memcpy(void *dst, const void *src, int len){
  for(int i=0;i<len;i++){
    ((char*)(dst))[i] = ((char*)(src))[i];
  }
}

