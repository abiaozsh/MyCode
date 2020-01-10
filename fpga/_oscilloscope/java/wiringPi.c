#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/mman.h>

#ifndef	TRUE
#  define	TRUE	(1==1)
#  define	FALSE	(!TRUE)
#endif

// Pin modes
#define	INPUT			 0
#define	OUTPUT			 1

#define	LOW			 0
#define	HIGH			 1

// Pi model types and version numbers
//	Intended for the GPIO program Use at your own risk.

#define	PI_MODEL_A		 0
#define	PI_MODEL_B		 1
#define	PI_MODEL_AP		 2
#define	PI_MODEL_BP		 3
#define	PI_MODEL_2		 4
#define	PI_ALPHA		 5
#define	PI_MODEL_CM		 6
#define	PI_MODEL_07		 7
#define	PI_MODEL_3		 8
#define	PI_MODEL_ZERO		 9
#define	PI_MODEL_CM3		10
#define	PI_MODEL_ZERO_W		12
#define	PI_MODEL_3P 		13

#define	PI_VERSION_1		0
#define	PI_VERSION_1_1		1
#define	PI_VERSION_1_2		2
#define	PI_VERSION_2		3

#define	PI_MAKER_SONY		0
#define	PI_MAKER_EGOMAN		1
#define	PI_MAKER_EMBEST		2
#define	PI_MAKER_UNKNOWN	3



#define	ENV_DEBUG	"WIRINGPI_DEBUG"
#define	ENV_CODES	"WIRINGPI_CODES"
#define	ENV_GPIOMEM	"WIRINGPI_GPIOMEM"


// Extend wiringPi with other pin-based devices and keep track of
//	them in this structure

struct wiringPiNodeStruct *wiringPiNodes = NULL ;

// BCM Magic

#define	BCM_PASSWORD		0x5A000000


// The BCM2835 has 54 GPIO pins.
//	BCM2835 data sheet, Page 90 onwards.
//	There are 6 control registers, each control the functions of a block
//	of 10 pins.
//	Each control register has 10 sets of 3 bits per GPIO pin - the ALT values
//
//	000 = GPIO Pin X is an input
//	001 = GPIO Pin X is an output
//	100 = GPIO Pin X takes alternate function 0
//	101 = GPIO Pin X takes alternate function 1
//	110 = GPIO Pin X takes alternate function 2
//	111 = GPIO Pin X takes alternate function 3
//	011 = GPIO Pin X takes alternate function 4
//	010 = GPIO Pin X takes alternate function 5
//
// So the 3 bits for port X are:
//	X / 10 + ((X % 10) * 3)

// Port function select bits

#define	FSEL_INPT		0b000
#define	FSEL_OUTP		0b001
#define	FSEL_ALT0		0b100
#define	FSEL_ALT1		0b101
#define	FSEL_ALT2		0b110
#define	FSEL_ALT3		0b111
#define	FSEL_ALT4		0b011
#define	FSEL_ALT5		0b010

// Access from ARM Running Linux
//	Taken from Gert/Doms code. Some of this is not in the manual
//	that I can find )-:
//
// Updates in September 2015 - all now static variables (and apologies for the caps)
//	due to the Pi v2, v3, etc. and the new /dev/gpiomem interface

static volatile unsigned int GPIO_PADS ;
static volatile unsigned int GPIO_CLOCK_BASE ;
static volatile unsigned int GPIO_BASE ;
static volatile unsigned int GPIO_TIMER ;
static volatile unsigned int GPIO_PWM ;

#define	PAGE_SIZE		(4*1024)
#define	BLOCK_SIZE		(4*1024)

static unsigned int usingGpioMem    = FALSE ;
static          int wiringPiSetuped = FALSE ;

// PWM
//	Word offsets into the PWM control region

#define	PWM_CONTROL 0
#define	PWM_STATUS  1
#define	PWM0_RANGE  4
#define	PWM0_DATA   5
#define	PWM1_RANGE  8
#define	PWM1_DATA   9

//	Clock regsiter offsets

#define	PWMCLK_CNTL	40
#define	PWMCLK_DIV	41

#define	PWM0_MS_MODE    0x0080  // Run in MS mode
#define	PWM0_USEFIFO    0x0020  // Data from FIFO
#define	PWM0_REVPOLAR   0x0010  // Reverse polarity
#define	PWM0_OFFSTATE   0x0008  // Ouput Off state
#define	PWM0_REPEATFF   0x0004  // Repeat last value if FIFO empty
#define	PWM0_SERIAL     0x0002  // Run in serial mode
#define	PWM0_ENABLE     0x0001  // Channel Enable

#define	PWM1_MS_MODE    0x8000  // Run in MS mode
#define	PWM1_USEFIFO    0x2000  // Data from FIFO
#define	PWM1_REVPOLAR   0x1000  // Reverse polarity
#define	PWM1_OFFSTATE   0x0800  // Ouput Off state
#define	PWM1_REPEATFF   0x0400  // Repeat last value if FIFO empty
#define	PWM1_SERIAL     0x0200  // Run in serial mode
#define	PWM1_ENABLE     0x0100  // Channel Enable

// Timer
//	Word offsets

#define	TIMER_LOAD	(0x400 >> 2)
#define	TIMER_VALUE	(0x404 >> 2)
#define	TIMER_CONTROL	(0x408 >> 2)
#define	TIMER_IRQ_CLR	(0x40C >> 2)
#define	TIMER_IRQ_RAW	(0x410 >> 2)
#define	TIMER_IRQ_MASK	(0x414 >> 2)
#define	TIMER_RELOAD	(0x418 >> 2)
#define	TIMER_PRE_DIV	(0x41C >> 2)
#define	TIMER_COUNTER	(0x420 >> 2)

// Locals to hold pointers to the hardware

static volatile unsigned int *gpio ;
static volatile unsigned int *pwm ;
static volatile unsigned int *clk ;
static volatile unsigned int *pads ;
static volatile unsigned int *timer ;
static volatile unsigned int *timerIrqRaw ;

// Export variables for the hardware pointers

volatile unsigned int *_wiringPiTimerIrqRaw ;


// Data for use with the boardId functions.
//	The order of entries here to correspond with the PI_MODEL_X
//	and PI_VERSION_X defines in wiringPi.h
//	Only intended for the gpio command - use at your own risk!

// piGpioBase:
//	The base address of the GPIO memory mapped hardware IO


static volatile unsigned int piGpioBase = 0 ;

const char *piModelNames [16] =
{
  "Model A",	//  0
  "Model B",	//  1
  "Model A+",	//  2
  "Model B+",	//  3
  "Pi 2",	//  4
  "Alpha",	//  5
  "CM",		//  6
  "Unknown07",	// 07
  "Pi 3",	// 08
  "Pi Zero",	// 09
  "CM3",	// 10
  "Unknown11",	// 11
  "Pi Zero-W",	// 12
  "Pi 3+",	// 13
  "Unknown14",	// 14
  "Unknown15",	// 15
} ;

const char *piRevisionNames [16] =
{
  "00",
  "01",
  "02",
  "03",
  "04",
  "05",
  "06",
  "07",
  "08",
  "09",
  "10",
  "11",
  "12",
  "13",
  "14",
  "15",
} ;

const char *piMakerNames [16] =
{
  "Sony",	//	 0
  "Egoman",	//	 1
  "Embest",	//	 2
  "Unknown",	//	 3
  "Embest",	//	 4
  "Unknown05",	//	 5
  "Unknown06",	//	 6
  "Unknown07",	//	 7
  "Unknown08",	//	 8
  "Unknown09",	//	 9
  "Unknown10",	//	10
  "Unknown11",	//	11
  "Unknown12",	//	12
  "Unknown13",	//	13
  "Unknown14",	//	14
  "Unknown15",	//	15
} ;

const int piMemorySize [8] =
{
   256,		//	 0
   512,		//	 1
  1024,		//	 2
     0,		//	 3
     0,		//	 4
     0,		//	 5
     0,		//	 6
     0,		//	 7
} ;

// Time for easy calculations

static uint64_t epochMilli, epochMicro ;

// Misc

static volatile int    pinPass = -1 ;
//static pthread_mutex_t pinMutex ;

// Use /dev/gpiomem ?

int wiringPiTryGpioMem  = FALSE ;


// gpioToGPFSEL:
//	Map a BCM_GPIO pin to it's Function Selection
//	control port. (GPFSEL 0-5)
//	Groups of 10 - 3 bits per Function - 30 bits per port

static uint8_t gpioToGPFSEL [] =
{
  0,0,0,0,0,0,0,0,0,0,
  1,1,1,1,1,1,1,1,1,1,
  2,2,2,2,2,2,2,2,2,2,
  3,3,3,3,3,3,3,3,3,3,
  4,4,4,4,4,4,4,4,4,4,
  5,5,5,5,5,5,5,5,5,5,
} ;


// gpioToShift
//	Define the shift up for the 3 bits per pin in each GPFSEL port

static uint8_t gpioToShift [] =
{
  0,3,6,9,12,15,18,21,24,27,
  0,3,6,9,12,15,18,21,24,27,
  0,3,6,9,12,15,18,21,24,27,
  0,3,6,9,12,15,18,21,24,27,
  0,3,6,9,12,15,18,21,24,27,
  0,3,6,9,12,15,18,21,24,27,
} ;

/*
 * piBoardId:
 *	Return the real details of the board we have.
 *
 *	This is undocumented and really only intended for the GPIO command.
 *	Use at your own risk!
 *
 *	Seems there are some boards with 0000 in them (mistake in manufacture)
 *	So the distinction between boards that I can see is:
 *
 *		0000 - Error
 *		0001 - Not used 
 *
 *	Original Pi boards:
 *		0002 - Model B,  Rev 1,   256MB, Egoman
 *		0003 - Model B,  Rev 1.1, 256MB, Egoman, Fuses/D14 removed.
 *
 *	Newer Pi's with remapped GPIO:
 *		0004 - Model B,  Rev 1.2, 256MB, Sony
 *		0005 - Model B,  Rev 1.2, 256MB, Egoman
 *		0006 - Model B,  Rev 1.2, 256MB, Egoman
 *
 *		0007 - Model A,  Rev 1.2, 256MB, Egoman
 *		0008 - Model A,  Rev 1.2, 256MB, Sony
 *		0009 - Model A,  Rev 1.2, 256MB, Egoman
 *
 *		000d - Model B,  Rev 1.2, 512MB, Egoman	(Red Pi, Blue Pi?)
 *		000e - Model B,  Rev 1.2, 512MB, Sony
 *		000f - Model B,  Rev 1.2, 512MB, Egoman
 *
 *		0010 - Model B+, Rev 1.2, 512MB, Sony
 *		0013 - Model B+  Rev 1.2, 512MB, Embest
 *		0016 - Model B+  Rev 1.2, 512MB, Sony
 *		0019 - Model B+  Rev 1.2, 512MB, Egoman
 *
 *		0011 - Pi CM,    Rev 1.1, 512MB, Sony
 *		0014 - Pi CM,    Rev 1.1, 512MB, Embest
 *		0017 - Pi CM,    Rev 1.1, 512MB, Sony
 *		001a - Pi CM,    Rev 1.1, 512MB, Egoman
 *
 *		0012 - Model A+  Rev 1.1, 256MB, Sony
 *		0015 - Model A+  Rev 1.1, 512MB, Embest
 *		0018 - Model A+  Rev 1.1, 256MB, Sony
 *		001b - Model A+  Rev 1.1, 256MB, Egoman
 *
 *	A small thorn is the olde style overvolting - that will add in
 *		1000000
 *
 *	The Pi compute module has an revision of 0011 or 0014 - since we only
 *	check the last digit, then it's 1, therefore it'll default to not 2 or
 *	3 for a	Rev 1, so will appear as a Rev 2. This is fine for the most part, but
 *	we'll properly detect the Compute Module later and adjust accordingly.
 *
 * And then things changed with the introduction of the v2...
 *
 * For Pi v2 and subsequent models - e.g. the Zero:
 *
 *   [USER:8] [NEW:1] [MEMSIZE:3] [MANUFACTURER:4] [PROCESSOR:4] [TYPE:8] [REV:4]
 *   NEW          23: will be 1 for the new scheme, 0 for the old scheme
 *   MEMSIZE      20: 0=256M 1=512M 2=1G
 *   MANUFACTURER 16: 0=SONY 1=EGOMAN 2=EMBEST
 *   PROCESSOR    12: 0=2835 1=2836
 *   TYPE         04: 0=MODELA 1=MODELB 2=MODELA+ 3=MODELB+ 4=Pi2 MODEL B 5=ALPHA 6=CM
 *   REV          00: 0=REV0 1=REV1 2=REV2
 *********************************************************************************
 */


void pinMode (int pin, int mode)
{
  int    fSel, shift ;//, alt
  
    fSel    = gpioToGPFSEL [pin] ;
    shift   = gpioToShift  [pin] ;

    if (mode == INPUT)
      *(gpio + fSel) = (*(gpio + fSel) & ~(7 << shift)) ; // Sets bits to zero = input
    else if (mode == OUTPUT)
      *(gpio + fSel) = (*(gpio + fSel) & ~(7 << shift)) | (1 << shift) ;
}


short read16(){
  return (short)(*(gpio + 13));
  
}

int digitalRead (int pin)
{

    if ((*(gpio + 13) & (1 << (pin & 31))) != 0)
      return HIGH ;
    else
      return LOW ;
}

void digitalWrite (int pin, int value)
{

    if (value == LOW)
      *(gpio + 10) = 1 << (pin & 31) ;
    else
      *(gpio + 7) = 1 << (pin & 31) ;
}



/*
 * initialiseEpoch:
 *	Initialise our start-of-time variable to be the current unix
 *	time in milliseconds and microseconds.
 *********************************************************************************
 */

static void initialiseEpoch (void)
{
#ifdef	OLD_WAY
  struct timeval tv ;

  gettimeofday (&tv, NULL) ;
  epochMilli = (uint64_t)tv.tv_sec * (uint64_t)1000    + (uint64_t)(tv.tv_usec / 1000) ;
  epochMicro = (uint64_t)tv.tv_sec * (uint64_t)1000000 + (uint64_t)(tv.tv_usec) ;
#else
  struct timespec ts ;

  clock_gettime (CLOCK_MONOTONIC_RAW, &ts) ;
  epochMilli = (uint64_t)ts.tv_sec * (uint64_t)1000    + (uint64_t)(ts.tv_nsec / 1000000L) ;
  epochMicro = (uint64_t)ts.tv_sec * (uint64_t)1000000 + (uint64_t)(ts.tv_nsec /    1000L) ;
#endif
}


/*
 * delay:
 *	Wait for some number of milliseconds
 *********************************************************************************
 */

void delay (unsigned int howLong)
{
  struct timespec sleeper, dummy ;

  sleeper.tv_sec  = (time_t)(howLong / 1000) ;
  sleeper.tv_nsec = (long)(howLong % 1000) * 1000000 ;

  nanosleep (&sleeper, &dummy) ;
}


void delayMicrosecondsHard (unsigned int howLong)
{
  struct timeval tNow, tLong, tEnd ;

  gettimeofday (&tNow, NULL) ;
  tLong.tv_sec  = howLong / 1000000 ;
  tLong.tv_usec = howLong % 1000000 ;
  timeradd (&tNow, &tLong, &tEnd) ;

  while (timercmp (&tNow, &tEnd, <))
    gettimeofday (&tNow, NULL) ;
}

void delayMicroseconds (unsigned int howLong)
{
  struct timespec sleeper ;
  unsigned int uSecs = howLong % 1000000 ;
  unsigned int wSecs = howLong / 1000000 ;

  /**/ if (howLong ==   0)
    return ;
  else if (howLong  < 100)
    delayMicrosecondsHard (howLong) ;
  else
  {
    sleeper.tv_sec  = wSecs ;
    sleeper.tv_nsec = (long)(uSecs * 1000L) ;
    nanosleep (&sleeper, NULL) ;
  }
}

unsigned int millis (void)
{
  uint64_t now ;

#ifdef	OLD_WAY
  struct timeval tv ;

  gettimeofday (&tv, NULL) ;
  now  = (uint64_t)tv.tv_sec * (uint64_t)1000 + (uint64_t)(tv.tv_usec / 1000) ;

#else
  struct  timespec ts ;

  clock_gettime (CLOCK_MONOTONIC_RAW, &ts) ;
  now  = (uint64_t)ts.tv_sec * (uint64_t)1000 + (uint64_t)(ts.tv_nsec / 1000000L) ;
#endif

  return (uint32_t)(now - epochMilli) ;
}


unsigned int micros (void)
{
  uint64_t now ;
#ifdef	OLD_WAY
  struct timeval tv ;

  gettimeofday (&tv, NULL) ;
  now  = (uint64_t)tv.tv_sec * (uint64_t)1000000 + (uint64_t)tv.tv_usec ;
#else
  struct  timespec ts ;

  clock_gettime (CLOCK_MONOTONIC_RAW, &ts) ;
  now  = (uint64_t)ts.tv_sec * (uint64_t)1000000 + (uint64_t)(ts.tv_nsec / 1000) ;
#endif


  return (uint32_t)(now - epochMicro) ;
}


 
#define	GPIO_PERI_BASE_OLD	0x20000000
#define	GPIO_PERI_BASE_NEW	0x3F000000

volatile unsigned int* wiringPiSetup (void)
{
  int   fd ;
  //int   model, rev, mem, maker, overVolted ;

// It's actually a fatal error to call any of the wiringPiSetup routines more than once,
//	(you run out of file handles!) but I'm fed-up with the useless twats who email
//	me bleating that there is a bug in my code, so screw-em.

  if (wiringPiSetuped)
    return 0 ;

  wiringPiSetuped = TRUE ;


  //piBoardId (&model, &rev, &mem, &maker, &overVolted) ;
  //printf ("model must be 9(pi zero):%d\n",model) ;
/*
/proc/cpuinfo

processor       : 0
model name      : ARMv6-compatible processor rev 7 (v6l)
BogoMIPS        : 997.08
Features        : half thumb fastmult vfp edsp java tls
CPU implementer : 0x41
CPU architecture: 7
CPU variant     : 0x0
CPU part        : 0xb76
CPU revision    : 7

Hardware        : BCM2835
Revision        : 900093
Serial          : 0000000000310186
*/


  //switch (model)
  //{
  //  case PI_MODEL_A:	case PI_MODEL_B:
  //  case PI_MODEL_AP:	case PI_MODEL_BP:
  //  case PI_ALPHA:	case PI_MODEL_CM:
  //  case PI_MODEL_ZERO:	case PI_MODEL_ZERO_W:
      piGpioBase = GPIO_PERI_BASE_OLD ;
  //    break ;
  //
  //  default:
  //    piGpioBase = GPIO_PERI_BASE_NEW ;
  //    break ;
  //}

// Open the master /dev/ memory control device
// Device strategy: December 2016:
//	Try /dev/mem. If that fails, then 
//	try /dev/gpiomem. If that fails then game over.

  if ((fd = open ("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC)) < 0)
  {
    if ((fd = open ("/dev/gpiomem", O_RDWR | O_SYNC | O_CLOEXEC) ) >= 0)	// We're using gpiomem
    {
      piGpioBase   = 0 ;
      usingGpioMem = TRUE ;
    }
  }

// Set the offsets into the memory interface.

  GPIO_PADS 	  = piGpioBase + 0x00100000 ;
  GPIO_CLOCK_BASE = piGpioBase + 0x00101000 ;
  GPIO_BASE	  = piGpioBase + 0x00200000 ;
  GPIO_TIMER	  = piGpioBase + 0x0000B000 ;
  GPIO_PWM	  = piGpioBase + 0x0020C000 ;

// Map the individual hardware components

//	GPIO:

  gpio = (uint32_t *)mmap(0, BLOCK_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, GPIO_BASE) ;

//	PWM

  pwm = (uint32_t *)mmap(0, BLOCK_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, GPIO_PWM) ;
 
//	Clock control (needed for PWM)

  clk = (uint32_t *)mmap(0, BLOCK_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, GPIO_CLOCK_BASE) ;
 
//	The drive pads

  pads = (uint32_t *)mmap(0, BLOCK_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, GPIO_PADS) ;

//	The system timer

  timer = (uint32_t *)mmap(0, BLOCK_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, GPIO_TIMER) ;

// Set the timer to free-running, 1MHz.
//	0xF9 is 249, the timer divide is base clock / (divide+1)
//	so base clock is 250MHz / 250 = 1MHz.

  *(timer + TIMER_CONTROL) = 0x0000280 ;
  *(timer + TIMER_PRE_DIV) = 0x00000F9 ;
  timerIrqRaw = timer + TIMER_IRQ_RAW ;

// Export the base addresses for any external software that might need them


  initialiseEpoch () ;

  return gpio;
}
