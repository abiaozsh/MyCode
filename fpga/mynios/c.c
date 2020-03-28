//函数指针
//变量指针
//中断向量
//浮点数

int __main(){
  return;
}

int uart_read(){
  int* p1 = (int*)(0x02002000);//0x0200_2000
  while(1){
    int tmp = *p1;
    if(tmp&0x100){
      return tmp;
    }
  }
}

int uart_write(int val){
  int* p2 = (int*)(0x02002004);//???
  while((*p2) & 0x100);
  (*p2)=val;
}


int main(){

  while(1){
    
    char buff[4];
    buff[0] = (char)uart_read();
    buff[1] = (char)uart_read();
    buff[2] = (char)uart_read();
    buff[3] = (char)uart_read();

    uart_write(buff[0]+1);
    uart_write(buff[1]+1);
    uart_write(buff[2]+1);
    uart_write(buff[3]+1);
    
  }
}
