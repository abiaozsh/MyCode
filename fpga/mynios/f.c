//函数指针
//变量指针
//中断向量
//浮点数

#include <stdarg.h>

char alt_putchar(char a){
	uart_write(a);
}

void myprintf(const char* fmt, ... ){
	va_list args;
	va_start(args, fmt);
    const char *w;
    char c;

    // Process format string.
    w = fmt;
    while ((c = *w++) != 0)
    {
        // If not a format escape character, just print  character.  Otherwise, process format string.
        if (c != '%')
        {
            alt_putchar(c);
        }
        else
        {
            // Get format character.  If none available, processing is complete. 
            if ((c = *w++) != 0)
            {
                if (c == '%')
                {
                    // Process "%" escape sequence.
                    alt_putchar(c);
                } 
                else if (c == 'c')
                {
                    int v = va_arg(args, int);
                    alt_putchar(v);
                }
                else if (c == 'x')
                {
                    // Process hexadecimal number format. 
                    unsigned long v = va_arg(args, unsigned long);
                    unsigned long digit;
                    int digit_shift;

                    // If the number value is zero, just print and continue. 
                    if (v == 0)
                    {
                        alt_putchar('0');
                        continue;
                    }

                    // Find first non-zero digit. 
                    digit_shift = 28;
                    while (!(v & (0xF << digit_shift)))
                        digit_shift -= 4;

                    // Print digits. 
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
                    // Process string format. 
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
    int* p1 = (int*)(0x00000000);
    int* p2 = (int*)(0x00000004);
    *p2 = tmp;
    *p1 = tmp&0xFF;
  }
}
