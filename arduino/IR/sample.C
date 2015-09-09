


#include <reg51.h>
#include <intrins.h>

#define uchar unsigned char
#define uint  unsigned int
#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();};

void delay(uchar x);  //x*0.14MS
void delay1(int ms);
void beep();

sbit IRIN = P3^2;         //红外接收器数据线
sbit BEEP = P1^5;         //蜂鸣器驱动线
sbit RELAY= P1^4;         //继电器驱动线

uchar IRCOM[7];

/*******************************************************************/
main()
{
   uchar m;
  
    IE = 0x81;                 //允许总中断中断,使能 INT0 外部中断
    TCON = 0x01;               //触发方式为脉冲负边沿触发
    
    IRIN=1;                    //I/O口初始化
    BEEP=1;
    RELAY=1; 
	
        

} //end main
/**********************************************************/
void IR_IN() interrupt 0 using 0
{
  unsigned char j,k,N=0;
     EX0 = 0;   
	 delay(15);
	 if (IRIN==1) 
     { EX0 =1;
	   return;
	  } 
                           //确认IR信号出现
  while (!IRIN)            //等IR变为高电平，跳过9ms的前导低电平信号。
    {delay(1);}

 for (j=0;j<4;j++)         //收集四组数据
 { 
  for (k=0;k<8;k++)        //每组数据有8位
  {
    while (IRIN)            //等 IR 变为低电平，跳过4.5ms的前导高电平信号。
    {
      delay(1);
    }
    while (!IRIN)          //等 IR 变为高电平
    {
      delay(1);
    }
    while (IRIN)           //计算IR高电平时长
    {
      delay(1);
      N++;           
      if (N>=30)
      { 
        EX0=1;
        return;
      }                  //0.14ms计数过长自动离开。
    }                        //高电平计数完毕                
    IRCOM[j]=IRCOM[j] >> 1;                  //数据最高位补“0”
    if (N>=8) {IRCOM[j] = IRCOM[j] | 0x80;}  //数据最高位补“1”
    N=0;
  }//end for k
 }//end for j
   
   if (IRCOM[2]!=~IRCOM[3])
   { EX0=1;
     return; }

   IRCOM[5]=IRCOM[2] & 0x0F;     //取键码的低四位
   IRCOM[6]=IRCOM[2] >> 4;       //右移4次，高四位变为低四位

   if(IRCOM[5]>9)
    { IRCOM[5]=IRCOM[5]+0x37;}
   else
	  IRCOM[5]=IRCOM[5]+0x30;

   if(IRCOM[6]>9)
    { IRCOM[6]=IRCOM[6]+0x37;}
   else
	  IRCOM[6]=IRCOM[6]+0x30;

     lcd_pos(0x4b);             
     lcd_wdat(IRCOM[6]);        //第一位数显示 
     lcd_pos(0x4c);             
     lcd_wdat(IRCOM[5]);        //第二位数显示

     beep();
     EX0 = 1; 
} 

/**********************************************************/
void delay(unsigned char x)    //x*0.14MS
{
 unsigned char i;
  while(x--)
 {
  for (i = 0; i<13; i++) {}
 }
}
