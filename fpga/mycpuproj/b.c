//函数指针
//变量指针
//中断向量
//浮点数

volatile int temp;
int cnt = 0;

int __main(){
  
  return;
}

void test(int* p1,int* p2){
  int val = 0;

    while(1){
    int val1 = *p1;
    if(val1!=val){
      val=val1;
      cnt++;
      *p2 = cnt;
      temp = cnt;
    }
  }

}

int main(){
  int* p1 = (int*)(0x02001000);
  int* p2 = (int*)(0x02001010);
  test(p1,p2);
}
