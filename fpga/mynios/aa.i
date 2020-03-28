# 1 "a.c"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "a.c"
﻿



int __main(){
  return;
}

int readKey(){
  int* p1 = (int*)(0x02001000);
  return *p1;
}

int writeDebug(int val){
  int* p2 = (int*)(0x02001010);
  *p2 = val;
}

int main(){
  int val = 0;
  int cnt = 0;
  while(1){
    int val1 = readKey();
    if(val1!=val){
      val=val1;
      cnt++;
      writeDebug(cnt);
    }
  }
}
