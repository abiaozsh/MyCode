//函数指针
//变量指针
//中断向量
//浮点数

#include <stdarg.h>

char alt_putchar(char a){
	return a;
}

void myprintf(const char* fmt, ... ){
	va_list args;
	va_start(args, fmt);
    const char *w;
    char c;

    /* Process format string. */
    w = fmt;
    while ((c = *w++) != 0)
    {
        /* If not a format escape character, just print  */
        /* character.  Otherwise, process format string. */
        if (c != '%')
        {
            alt_putchar(c);
        }
        else
        {
            /* Get format character.  If none     */
            /* available, processing is complete. */
            if ((c = *w++) != 0)
            {
                if (c == '%')
                {
                    /* Process "%" escape sequence. */
                    alt_putchar(c);
                } 
                else if (c == 'c')
                {
                    int v = va_arg(args, int);
                    alt_putchar(v);
                }
                else if (c == 'x')
                {
                    /* Process hexadecimal number format. */
                    unsigned long v = va_arg(args, unsigned long);
                    unsigned long digit;
                    int digit_shift;

                    /* If the number value is zero, just print and continue. */
                    if (v == 0)
                    {
                        alt_putchar('0');
                        continue;
                    }

                    /* Find first non-zero digit. */
                    digit_shift = 28;
                    while (!(v & (0xF << digit_shift)))
                        digit_shift -= 4;

                    /* Print digits. */
                    for (; digit_shift >= 0; digit_shift -= 4)
                    {
                        digit = (v & (0xF << digit_shift)) >> digit_shift;
                        if (digit <= 9)
                            c = '0' + digit;
                        else
                            c = 'a' + digit - 10;
                        alt_putchar(c);
                    }
                }
                else if (c == 's')
                {
                    /* Process string format. */
                    char *s = va_arg(args, char *);

                    while(*s)
                      alt_putchar(*s++);
                }
            }
            else
            {
                break;
            }
        }
    }
}


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
  myprintf("ffdsa",cnt2);
}

