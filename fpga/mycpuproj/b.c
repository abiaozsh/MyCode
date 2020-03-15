//函数指针
//变量指针
//中断向量
//浮点数

volatile char temp1;
volatile short temp2;
volatile int temp3;
volatile long long temp4;
int cnt = 0;
int cnt1 = 0;

int __main(){
  asm("aaamov ra,rb");
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
      temp1 = (char)cnt;
      temp2 = (short)cnt;
      temp3 = (int)cnt;
      temp4 = (long long)cnt;
    }
  }

}

int cnt2 = 0;
char cnt3=0;
int main(){
  int* p1 = (int*)(0x02001000);
  int* p2 = (int*)(0x02001010);
  cnt2++;
  test(p1,p2);
  
  
}
