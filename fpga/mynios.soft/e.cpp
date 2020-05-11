#include "inc/io.h"
#include "inc/system.h"
#include "inc/system.cpp"
//#include "inc/uart.cpp"
//#include "inc/print.cpp"

#define PS2_TAB				9
#define PS2_ENTER			10
#define PS2_BACKSPACE			127
#define PS2_ESC				27
#define PS2_INSERT			0
#define PS2_DELETE			127
#define PS2_HOME			0
#define PS2_END				0
#define PS2_PAGEUP			25
#define PS2_PAGEDOWN			26
#define PS2_UPARROW			11
#define PS2_LEFTARROW			8
#define PS2_DOWNARROW			10
#define PS2_RIGHTARROW			21
#define PS2_F1				0
#define PS2_F2				0
#define PS2_F3				0
#define PS2_F4				0
#define PS2_F5				0
#define PS2_F6				0
#define PS2_F7				0
#define PS2_F8				0
#define PS2_F9				0
#define PS2_F10				0
#define PS2_F11				0
#define PS2_F12				0
#define PS2_SCROLL			0

int PS2Keymap_US[] = {
  // without shift
	0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
	0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, '`', 0,
	0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'q', '1', 0,
	0, 0, 'z', 's', 'a', 'w', '2', 0,
	0, 'c', 'x', 'd', 'e', '4', '3', 0,
	0, ' ', 'v', 'f', 't', 'r', '5', 0,
	0, 'n', 'b', 'h', 'g', 'y', '6', 0,
	0, 0, 'm', 'j', 'u', '7', '8', 0,
	0, ',', 'k', 'i', 'o', '0', '9', 0,
	0, '.', '/', 'l', ';', 'p', '-', 0,
	0, 0, '\'', 0, '[', '=', 0, 0,
	0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, ']', 0, '\\', 0, 0,
	0, 0, 0, 0, 0, 0, PS2_BACKSPACE, 0,
	0, '1', 0, '4', '7', 0, 0, 0,
	'0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
	PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	0, 0, 0, PS2_F7 };


void setIrq(int val){
  __asm__ __volatile__("setirq %[input1],%[input1],1"::[input1]"r"(val));
}
void clrIrq(int val){
  __asm__ __volatile__("setirq %[input1],%[input1],0"::[input1]"r"(val));
}

int skip;
int irq_proc(){
  int irq = IORD(IRQ, 0);
  if(irq&1){
    //timer
    IOWR(MYUART, 1 , 't');
    IOWR(IRQ, 0, 1);
  }
  if(irq&2){
    int tmp = IORD(MYUART, 0);
    IOWR(MYUART, 1 ,tmp);
    IOWR(IRQ, 0, 2);
  }
  if(irq&4){
    int tmp = IORD(MYKEYB, 0);
    if(tmp&0x400){
      if(skip){
        skip = 0;
      }else{
        tmp = (tmp>>1) & 0xFF;
        if(tmp == 0xF0 || tmp ==0xE0)
        {
          skip = 1;
        }
        else{
          tmp = PS2Keymap_US[tmp];
          IOWR(MYUART, 1 ,tmp);
        }
      }
    }
    IOWR(IRQ, 0, 4);
  }

  
  asm("reti");
}

int main(){
  
  IOWR(MYTIMER, 4, 1000000);
  IOWR(MYTIMER, 0, 0);
  setIrq((int)irq_proc);
  
  
  //print("Hello from video demo\r\n");
  
  while(1){};
}
