void abort(void)
{
  while(1);
}
//TODO 要加上sdram的偏移，和考虑私有空间的情况
void jmp(int val){
  __asm__ __volatile__("jmp %[input1]"::[input1]"r"(val));
}

void setOff(int val){
  __asm__ __volatile__("stoff %[input1]"::[input1]"r"(val));
}

void* __eof__();
int malloc_index = 0;
void* malloc(int size){
  size = (size & (~0x03))+4;
  int idx = (int)__eof__();
  idx += malloc_index;
  malloc_index += size;
  return (void*)idx;
}

void mfree(int size){
  malloc_index -= size;
}

void flushCache(void* addr){
  int tmp = (int)addr;
  tmp = tmp >> 10;//TODO 要加上sdram的偏移，和考虑私有空间的情况
  #ifdef sti
  int status = sti(0);
  #endif
  IOWR(CACHE_CTL, 0, 0x80000000 | tmp);
  volatile int a = *((int*)addr);
  IOWR(CACHE_CTL, 0, 0);
  #ifdef sti
  if(status){
    sti(1);
  }
  #endif
}

void memcpy(void *dst, const void *src, int len){
  for(int i=0;i<len;i++){
    ((char*)(dst))[i] = ((char*)(src))[i];
  }
}

int getSeed(){
  int v = 0;
  for(int i=0;i<32;i++){
    for(int j=0;j<20;j++){
      volatile int a = j;
    }
    v <<= 1;
    int r = IORD(RND, 0);
    v |= r;
  }
  return v;
}

unsigned long long __rnd_seed__;
#define __rnd_multiplier 0x5DEECE66DLL
#define __rnd_addend 0xBLL
#define __rnd_mask = ((1LL << 48) - 1)

void rndSetSeed(unsigned int seed) {
    __rnd_seed__ = (seed ^ __rnd_multiplier) & (((1LL << 48) - 1));
}
unsigned long long mul64bit(unsigned long long a, unsigned long long b){
  unsigned long long result = 0;
  unsigned long long aa = a;
  unsigned long long bb = b;
  for(int i=0;i<64;i++){
    if(aa & 1){
      result += bb;
    }
    aa>>=1;
    bb<<=1;
  }
  return result;
}
int rnd() {
unsigned long long oldseed, nextseed;
  oldseed = __rnd_seed__;
  nextseed = (mul64bit(oldseed, __rnd_multiplier) + __rnd_addend) & (((1LL << 48) - 1));
  __rnd_seed__ = nextseed;
  return (unsigned int) (nextseed >> (48 - 32));
}
