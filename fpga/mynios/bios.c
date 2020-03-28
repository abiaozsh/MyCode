//函数指针
//变量指针
//中断向量
//浮点数

int __main(){
  return;
}

int uart_read(){
  int* p1 = (int*)(0x02002000);//0x0200_2000
  int tmp = *p1;
  while(1){
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
    int tmp = uart_read();
    tmp++;
    uart_write(tmp);
    tmp++;
    uart_write(tmp);
    tmp++;
    uart_write(tmp);
    tmp++;
    uart_write(tmp);
  }
}
