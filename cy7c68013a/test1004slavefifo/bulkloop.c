#pragma NOIV               // Do not generate interrupt vectors

#include "..\inc\fx2.h"
#include "..\inc\fx2regs.h"
#include "..\inc\fx2sdly.h"            // SYNCDELAY macro
extern BOOL   GotSUD;         // Received setup data flag
extern BOOL   Sleep;
extern BOOL   Rwuen;
extern BOOL   Selfpwr;

BYTE   Configuration;      // Current configuration
BYTE   AlternateSetting;   // Alternate settings
#define EPXCFG_VALID 0x80
#define EPXCFG_IN 0x40
#define EPXCFG_OUT 0x00
#define EPXCFG_BULK 0x20
#define EPXCFG_SIZE512  0x00
#define EPXCFG_SIZE1024 0x08
#define EPXCFG_QUAD 0x00
#define EPXCFG_DOUBLE 0x02
#define EPXCFG_TRIPLE 0x03

void TD_Init(void)             // Called once at startup
{

  CPUCS = 0x10;SYNCDELAY; // CLKSPD[1:0]=10, for 48MHz operation, output CLKOUT
  // set the CPU clock to 48MHz
//  CPUCS &= ~bmCLKSPD0;SYNCDELAY;
//  CPUCS |= ~bmCLKSPD1;SYNCDELAY;

  //PINFLAGSAB = 0x08;            // FLAGA - EP2EF
  //SYNCDELAY;
  //PINFLAGSCD = 0x60;            // FLAGA - EP6PF
  //SYNCDELAY;
  //4 0 1 0 0 EP2 PF
  //5 0 1 0 1 EP4 PF
  //6 0 1 1 0 EP6 PF
  //7 0 1 1 1 EP8 PF
  //8 1 0 0 0 EP2 EF
  //9 1 0 0 1 EP4 EF
  //A 1 0 1 0 EP6 EF
  //B 1 0 1 1 EP8 EF
  //C 1 1 0 0 EP2 FF
  //D 1 1 0 1 EP4 FF
  //E 1 1 1 0 EP6 FF
  //F 1 1 1 1 EP8 FF
  PINFLAGSAB = 0xC8;SYNCDELAY; // FLAGA:8 1 0 0 0 EP2 EF     FLAGB:C 1 1 0 0 EP2 FF
  
  PINFLAGSCD = 0xEA;SYNCDELAY; // FLAGC:A 1 0 1 0 EP6 EF     FLAGD:E 1 1 1 0 EP6 FF
  
  PORTACFG |= 0x80;SYNCDELAY;
  //IFCONFIG = 0xE3;   // 11100011         // for async? for sync?
  //1	1	Slave FIFO

  //IFCLKSRC：FIFO时钟内部/外部时钟源选择，0--外部时钟源，1--内部时钟源。
  IFCONFIG |= 0x80;SYNCDELAY;

  //30/48MHZ：当IFCLKSRC=1时，选择30MHz/48MHz内部时钟频率，0--IFCLK 30MHz，1--IFCLK 48MHz。
  IFCONFIG |= 0x40;SYNCDELAY;

  //IFCLKOE：当IFCLKSRC=1时，IFCLK时钟输出使能，0--关闭，1--打开。
  IFCONFIG |= 0x20;SYNCDELAY;

  //IFCLKPOL：IFCLK输出反转使能，0--正常，1--反转。
  IFCONFIG &= ~0x10;SYNCDELAY;

  //ASYNC：Slave FIFO同步/异步工作方式选择，0同步，1异步。
  //IFCONFIG |= bmASYNC;SYNCDELAY;
  IFCONFIG  |= 0x08;SYNCDELAY;
  
  IFCONFIG |= 0x02;SYNCDELAY;
  IFCONFIG |= 0x01;SYNCDELAY;
  
  
  
  
  
  CPUCS |= 0x02;SYNCDELAY;
//  CLKOE off
//  CPUCS &= ~bmCLKOE;SYNCDELAY;

//  FIFOPINPOLAR = 0x20+0x10+0x08+0x04+0x02+0x01;SYNCDELAY;

  EP1OUTCFG = 0xA0;SYNCDELAY;//VALID 10 Bulk (default)
  EP1INCFG = 0xA0;SYNCDELAY;//VALID 10 Bulk (default)
  //REVCTL
//  REVCTL |= 1;SYNCDELAY;//bmDYN_OUT;
//  REVCTL |= 2;SYNCDELAY;//bmENH_PKT;

  // EP4 and EP8 are not used in this implementation...

  //EP2CFG = 0xA0;SYNCDELAY;                //out 512 bytes, 4x, bulk
  EP2CFG = EPXCFG_VALID + EPXCFG_OUT + EPXCFG_BULK + EPXCFG_SIZE1024 + EPXCFG_DOUBLE;SYNCDELAY;//out to cy 512*2
  //EP6CFG = 0xE0;SYNCDELAY;                // in 512 bytes, 4x, bulk
  EP6CFG = EPXCFG_VALID + EPXCFG_IN + EPXCFG_BULK + EPXCFG_SIZE1024 + EPXCFG_DOUBLE;SYNCDELAY;//0xE2;//in from cy 512*2
  EP4CFG = 0x02;SYNCDELAY;                //clear valid bit
  EP8CFG = 0x02;SYNCDELAY;                //clear valid bit
  

  SYNCDELAY;
  FIFORESET = 0x80;             // activate NAK-ALL to avoid race conditions
  SYNCDELAY;                    // see TRM section 15.14
  FIFORESET = 0x02;             // reset, FIFO 2
  SYNCDELAY;                    // 
  FIFORESET = 0x04;             // reset, FIFO 4
  SYNCDELAY;                    // 
  FIFORESET = 0x06;             // reset, FIFO 6
  SYNCDELAY;                    // 
  FIFORESET = 0x08;             // reset, FIFO 8
  SYNCDELAY;                    // 
  FIFORESET = 0x00;             // deactivate NAK-ALL

  
//  SYNCDELAY;
//OUTPKTEND = 0x82; // Arm both EP2 buffers to “prime the pump”
//SYNCDELAY;
//OUTPKTEND = 0x82;

  // handle the case where we were already in AUTO mode...
  // ...for example: back to back firmware downloads...
  SYNCDELAY;                    // 
  EP2FIFOCFG = 0x00;            // AUTOOUT=0, WORDWIDE=0

  // core needs to see AUTOOUT=0 to AUTOOUT=1 switch to arm endp's
  SYNCDELAY;                    // 
  EP2FIFOCFG = 0x11;            // AUTOOUT=1, WORDWIDE
  SYNCDELAY;                    // 
  EP6FIFOCFG = 0x0D;            // AUTOIN=1, ZEROLENIN=1, WORDWIDE=1
  SYNCDELAY;
  
  OEA |= 0x01;//A0_INT0;
  IOA &= ~0x01;
  OEA &= ~0x02;//A1_INT1
  //IOA &= ~0x01;
  OEA |= 0x08;//A3_WU2;
  IOA &= ~0x08;
}

#define DATH IOA |= 0x01
#define DATL IOA &=~0x01
#define INFF (IOA & 0x02)
#define CLKH IOA |= 0x08
#define CLKL IOA &=~0x08

volatile BYTE clk;

void TD_Poll(void)             // Called repeatedly while the device is idle
{
	BYTE a;
	BYTE b;
	BYTE i;
	BYTE mask;
	
	clk = !clk;
	if(clk){
		CLKH;
	}else{
		CLKL;
	
		if(INFF){// in from fpga
			//EZUSB_ReadI2C(BTN_ADDR,0x01,&buttons);	// Read button states
			a=0;
			b=0;
			CLKH;
			mask = 0x01;
			CLKL;
			for(i=0;i<8;i++){
				if(INFF){
					a|=mask;
				}
				CLKH;
				mask<<=1;
				CLKL;
			}
			mask = 0x01;
			for(i=0;i<8;i++){
				if(INFF){
					b|=mask;
				}
				CLKH;
				mask<<=1;
				CLKL;
			}

			EP1INBUF[0] = a;
			EP1INBUF[1] = b;
			EP1INBC = 2;
		}

		if(!(EP1OUTCS & bmEPBUSY))	// Is there something available
		{
			a = EP1OUTBUF[0];//cmd
			b = EP1OUTBUF[1];//dat
			EP1OUTBC = 0;				//Rearm endpoint buffer
			
			DATH;
			CLKH;
			mask = 0x01;
			CLKL;
			for(i=0;i<8;i++){
				if(a&mask){
					DATH;
				}else{
					DATL;
				}
				CLKH;
				mask<<=1;
				CLKL;
			}
			mask = 0x01;
			for(i=0;i<8;i++){
				if(b&mask){
					DATH;
				}else{
					DATL;
				}
				CLKH;
				mask<<=1;
				CLKL;
			}
			DATL;
		}
		
	}

}

BOOL TD_Suspend(void)          // Called before the device goes into suspend mode
{
   return(TRUE);
}

BOOL TD_Resume(void)          // Called after the device resumes
{
   return(TRUE);
}

//-----------------------------------------------------------------------------
// Device Request hooks
//   The following hooks are called by the end point 0 device request parser.
//-----------------------------------------------------------------------------

BOOL DR_GetDescriptor(void)
{
   return(TRUE);
}

BOOL DR_SetConfiguration(void)   // Called when a Set Configuration command is received
{
   Configuration = SETUPDAT[2];
   return(TRUE);            // Handled by user code
}

BOOL DR_GetConfiguration(void)   // Called when a Get Configuration command is received
{
   EP0BUF[0] = Configuration;
   EP0BCH = 0;
   EP0BCL = 1;
   return(TRUE);            // Handled by user code
}

BOOL DR_SetInterface(void)       // Called when a Set Interface command is received
{
   AlternateSetting = SETUPDAT[2];
   return(TRUE);            // Handled by user code
}

BOOL DR_GetInterface(void)       // Called when a Set Interface command is received
{
   EP0BUF[0] = AlternateSetting;
   EP0BCH = 0;
   EP0BCL = 1;
   return(TRUE);            // Handled by user code
}

BOOL DR_GetStatus(void)
{
   return(TRUE);
}

BOOL DR_ClearFeature(void)
{
   return(TRUE);
}

BOOL DR_SetFeature(void)
{
   return(TRUE);
}

BOOL DR_VendorCmnd(void)
{
  //BYTE tmp;
  //
  //switch (SETUPDAT[1])
  //{
  //   case VR_NAKALL_ON:
  //      tmp = FIFORESET;
  //      tmp |= bmNAKALL;      
  //      SYNCDELAY;                    
  //      FIFORESET = tmp;
  //      break;
  //   case VR_NAKALL_OFF:
  //      tmp = FIFORESET;
  //      tmp &= ~bmNAKALL;      
  //      SYNCDELAY;                    
  //      FIFORESET = tmp;
  //      break;
  //   default:
  //      return(TRUE);
  //}
  //
  //return(FALSE);
   return(TRUE);
}

//-----------------------------------------------------------------------------
// USB Interrupt Handlers
//   The following functions are called by the USB interrupt jump table.
//-----------------------------------------------------------------------------

// Setup Data Available Interrupt Handler
void ISR_Sudav(void) interrupt 0
{
   GotSUD = TRUE;            // Set flag
   EZUSB_IRQ_CLEAR();
   USBIRQ = bmSUDAV;         // Clear SUDAV IRQ
}

// Setup Token Interrupt Handler
void ISR_Sutok(void) interrupt 0
{
   EZUSB_IRQ_CLEAR();
   USBIRQ = bmSUTOK;         // Clear SUTOK IRQ
}

void ISR_Sof(void) interrupt 0
{
   EZUSB_IRQ_CLEAR();
   USBIRQ = bmSOF;            // Clear SOF IRQ
}

void ISR_Ures(void) interrupt 0
{
   // whenever we get a USB reset, we should revert to full speed mode
   pConfigDscr = pFullSpeedConfigDscr;
   ((CONFIGDSCR xdata *) pConfigDscr)->type = CONFIG_DSCR;
   pOtherConfigDscr = pHighSpeedConfigDscr;
   ((CONFIGDSCR xdata *) pOtherConfigDscr)->type = OTHERSPEED_DSCR;
   
   EZUSB_IRQ_CLEAR();
   USBIRQ = bmURES;         // Clear URES IRQ
}

void ISR_Susp(void) interrupt 0
{
   Sleep = TRUE;
   EZUSB_IRQ_CLEAR();
   USBIRQ = bmSUSP;
}

void ISR_Highspeed(void) interrupt 0
{
   if (EZUSB_HIGHSPEED())
   {
      pConfigDscr = pHighSpeedConfigDscr;
      ((CONFIGDSCR xdata *) pConfigDscr)->type = CONFIG_DSCR;
      pOtherConfigDscr = pFullSpeedConfigDscr;
      ((CONFIGDSCR xdata *) pOtherConfigDscr)->type = OTHERSPEED_DSCR;
   }

   EZUSB_IRQ_CLEAR();
   USBIRQ = bmHSGRANT;
}
void ISR_Ep0ack(void) interrupt 0
{
}
void ISR_Stub(void) interrupt 0
{
}
void ISR_Ep0in(void) interrupt 0
{
}
void ISR_Ep0out(void) interrupt 0
{
}
void ISR_Ep1in(void) interrupt 0
{
}
void ISR_Ep1out(void) interrupt 0
{
}
void ISR_Ep2inout(void) interrupt 0
{
}
void ISR_Ep4inout(void) interrupt 0
{
}
void ISR_Ep6inout(void) interrupt 0
{
}
void ISR_Ep8inout(void) interrupt 0
{
}
void ISR_Ibn(void) interrupt 0
{
}
void ISR_Ep0pingnak(void) interrupt 0
{
}
void ISR_Ep1pingnak(void) interrupt 0
{
}
void ISR_Ep2pingnak(void) interrupt 0
{
}
void ISR_Ep4pingnak(void) interrupt 0
{
}
void ISR_Ep6pingnak(void) interrupt 0
{
}
void ISR_Ep8pingnak(void) interrupt 0
{
}
void ISR_Errorlimit(void) interrupt 0
{
}
void ISR_Ep2piderror(void) interrupt 0
{
}
void ISR_Ep4piderror(void) interrupt 0
{
}
void ISR_Ep6piderror(void) interrupt 0
{
}
void ISR_Ep8piderror(void) interrupt 0
{
}
void ISR_Ep2pflag(void) interrupt 0
{
}
void ISR_Ep4pflag(void) interrupt 0
{
}
void ISR_Ep6pflag(void) interrupt 0
{
}
void ISR_Ep8pflag(void) interrupt 0
{
}
void ISR_Ep2eflag(void) interrupt 0
{
}
void ISR_Ep4eflag(void) interrupt 0
{
}
void ISR_Ep6eflag(void) interrupt 0
{
}
void ISR_Ep8eflag(void) interrupt 0
{
}
void ISR_Ep2fflag(void) interrupt 0
{
}
void ISR_Ep4fflag(void) interrupt 0
{
}
void ISR_Ep6fflag(void) interrupt 0
{
}
void ISR_Ep8fflag(void) interrupt 0
{
}
void ISR_GpifComplete(void) interrupt 0
{
}
void ISR_GpifWaveform(void) interrupt 0
{
}
