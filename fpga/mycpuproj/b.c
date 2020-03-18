//函数指针
//变量指针
//中断向量
//浮点数

int __main(){
  return;
}

int main(){
  int* p1 = (int*)(0x02002000);//0x0200_2000
  int* p2 = (int*)(0x02002004);//???

  while(1){
    int tmp = *p1;
    if(tmp&0x100){
      tmp++;
      while((*p2) & 0x100);
      (*p2)=tmp;
      tmp++;
      while((*p2) & 0x100);
      (*p2)=tmp;
      tmp++;
      while((*p2) & 0x100);
      (*p2)=tmp;
      tmp++;
      while((*p2) & 0x100);
      (*p2)=tmp;

      
    }
  }
}
