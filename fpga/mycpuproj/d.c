//函数指针
//变量指针
//中断向量
//浮点数

volatile char v1;
volatile short v2;
volatile int v3;
volatile long long v4;
char i1;
short i2;
int i3;
long long i4;

char ii1 = 1;
short ii2 = 2;
int ii3 = 3;
long long ii4 = 4;

int __main(){
  asm("aaamov ra,rb");
  return;
}
int test2(char* txt){
  return txt[1];
}

int test(){
  return test2("fdsabb");
}

int main(){
  char* a = "fdsa";
  char b = a[3];
  char* c = "bbbb";
  char d = c[3];
  
  v1 = d;
  v2 = d;
  v3 = d;
  v4 = d;
  i1 = d;
  i2 = d;
  i3 = d;
  i4 = d;
  ii1 = d;
  ii2 = d;
  ii3 = d;
  ii4 = d;

  int iarr[] = {1,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5};
  const int iarrc[] = {1,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5,2,3,4,5};
  
  ii4 = iarr[10];
  ii4 = iarrc[10];

  test();
}

