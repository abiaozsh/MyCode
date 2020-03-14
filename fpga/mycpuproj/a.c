//函数指针
//变量指针
//中断向量
//浮点数

int main(){
  int val = 0;
  int cnt = 0;
  int* p1 = (int*)(0x02001000);
  int* p2 = (int*)(0x02001010);
  while(1){
    int val1 = *p1;
    if(val1!=val){
      val=val1;
      cnt++;
      *p2 = cnt;
    }
  }
}
