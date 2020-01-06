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

// Debugging & Return codes

int wiringPiReturnCodes = FALSE ;

// Use /dev/gpiomem ?

int wiringPiTryGpioMem  = FALSE ;

// ISR Data

//static void (*isrFunctions [64])(void) ;


// Doing it the Arduino way with lookup tables...
//	Yes, it's probably more innefficient than all the bit-twidling, but it
//	does tend to make it all a bit clearer. At least to me!

// pinToGpio:
//	Take a Wiring pin (0 through X) and re-map it to the BCM_GPIO pin
//	Cope for 3 different board revisions here.

static int *pinToGpio ;

// Revision 1, 1.1:

//static int pinToGpioR1 [64] =
//{
//  17, 18, 21, 22, 23, 24, 25, 4,	// From the Original Wiki - GPIO 0 through 7:	wpi  0 -  7
//   0,  1,				// I2C  - SDA1, SCL1				wpi  8 -  9
//   8,  7,				// SPI  - CE1, CE0				wpi 10 - 11
//  10,  9, 11, 				// SPI  - MOSI, MISO, SCLK			wpi 12 - 14
//  14, 15,				// UART - Tx, Rx				wpi 15 - 16
//
//// Padding:
//
//      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// ... 31
//  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// ... 47
//  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// ... 63
//} ;

// Revision 2:
//  BCM  GPIO
//   0, 30
//   1, 31
//   2,  8
//   3,  9
//   4,  7
//   5, 21
//   6, 22
//   7, 11
//   8, 10
//   9, 13
//  10, 12
//  11, 14
//  12, 26
//  13, 23
//  14, 15
//  15, 16
//  16, 27
//  17,  0
//  18,  1
//  19, 24
//  20, 28
//  21, 29
//  22,  3
//  23,  4
//  24,  5
//  25,  6
//  26, 25
//  27,  2

//  28, 
//  29, 
//  30, 
//  31, 

static int pinToGpioR2 [64] =
{
  17, 18, 27, 22, 23, 24, 25, 4,	// From the Original Wiki - GPIO 0 through 7:	wpi  0 -  7
   2,  3,				// I2C  - SDA0, SCL0				wpi  8 -  9
   8,  7,				// SPI  - CE1, CE0				wpi 10 - 11
  10,  9, 11, 				// SPI  - MOSI, MISO, SCLK			wpi 12 - 14
  14, 15,				// UART - Tx, Rx				wpi 15 - 16
  28, 29, 30, 31,			// Rev 2: New GPIOs 8 though 11			wpi 17 - 20
   5,  6, 13, 19, 26,			// B+						wpi 21, 22, 23, 24, 25
  12, 16, 20, 21,			// B+						wpi 26, 27, 28, 29
   0,  1,				// B+						wpi 30, 31

// Padding:

  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// ... 47
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// ... 63
} ;


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
 * piBoardRev:
 *	Deprecated, but does the same as piGpioLayout
 *********************************************************************************
 */

int piBoardRev (void)
{
  return 0;//piGpioLayout () ;
}



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
static void piGpioLayoutOops (const char *why)
{
  fprintf (stderr, " -> %s\n", why) ;
  exit (EXIT_FAILURE) ;
}

void piBoardId (int *model, int *rev, int *mem, int *maker, int *warranty)
{
  FILE *cpuFd ;
  char line [120] ;
  char *c ;
  unsigned int revision ;
  int bRev, bType,  bMfg, bMem, bWarranty ;//bProc,

//	Will deal with the properly later on - for now, lets just get it going...
//  unsigned int modelNum ;

  //(void)piGpioLayout () ;	// Call this first to make sure all's OK. Don't care about the result.

  if ((cpuFd = fopen ("/proc/cpuinfo", "r")) == NULL)
    piGpioLayoutOops ("Unable to open /proc/cpuinfo") ;

  while (fgets (line, 120, cpuFd) != NULL)
    if (strncmp (line, "Revision", 8) == 0)
      break ;

  fclose (cpuFd) ;

  if (strncmp (line, "Revision", 8) != 0)
    piGpioLayoutOops ("No \"Revision\" line") ;

// Chomp trailing CR/NL

  for (c = &line [strlen (line) - 1] ; (*c == '\n') || (*c == '\r') ; --c)
    *c = 0 ;
  
// Need to work out if it's using the new or old encoding scheme:

// Scan to the first character of the revision number

  for (c = line ; *c ; ++c)
    if (*c == ':')
      break ;

  if (*c != ':')
    piGpioLayoutOops ("Bogus \"Revision\" line (no colon)") ;

// Chomp spaces

  ++c ;
  while (isspace (*c))
    ++c ;

  if (!isxdigit (*c))
    piGpioLayoutOops ("Bogus \"Revision\" line (no hex digit at start of revision)") ;

  revision = (unsigned int)strtol (c, NULL, 16) ; // Hex number with no leading 0x

// Check for new way:

  if ((revision &  (1 << 23)) != 0)	// New way
  {

    bRev      = (revision & (0x0F <<  0)) >>  0 ;
    bType     = (revision & (0xFF <<  4)) >>  4 ;
    //bProc     = (revision & (0x0F << 12)) >> 12 ;	// Not used for now.
    bMfg      = (revision & (0x0F << 16)) >> 16 ;
    bMem      = (revision & (0x07 << 20)) >> 20 ;
    bWarranty = (revision & (0x03 << 24)) != 0 ;
    
    *model    = bType ;
    *rev      = bRev ;
    *mem      = bMem ;
    *maker    = bMfg  ;
    *warranty = bWarranty ;

  }
  else					// Old way
  {

    if (!isdigit (*c))
      piGpioLayoutOops ("Bogus \"Revision\" line (no digit at start of revision)") ;

// Make sure its long enough

    if (strlen (c) < 4)
      piGpioLayoutOops ("Bogus \"Revision\" line (not long enough)") ;

// If longer than 4, we'll assume it's been overvolted

    *warranty = strlen (c) > 4 ;
  
// Extract last 4 characters:

    c = c + strlen (c) - 4 ;

// Fill out the replys as appropriate

    /**/ if (strcmp (c, "0002") == 0) { *model = PI_MODEL_B  ; *rev = PI_VERSION_1   ; *mem = 0 ; *maker = PI_MAKER_EGOMAN  ; }
    else if (strcmp (c, "0003") == 0) { *model = PI_MODEL_B  ; *rev = PI_VERSION_1_1 ; *mem = 0 ; *maker = PI_MAKER_EGOMAN  ; }

    else if (strcmp (c, "0004") == 0) { *model = PI_MODEL_B  ; *rev = PI_VERSION_1_2 ; *mem = 0 ; *maker = PI_MAKER_SONY    ; }
    else if (strcmp (c, "0005") == 0) { *model = PI_MODEL_B  ; *rev = PI_VERSION_1_2 ; *mem = 0 ; *maker = PI_MAKER_EGOMAN  ; }
    else if (strcmp (c, "0006") == 0) { *model = PI_MODEL_B  ; *rev = PI_VERSION_1_2 ; *mem = 0 ; *maker = PI_MAKER_EGOMAN  ; }

    else if (strcmp (c, "0007") == 0) { *model = PI_MODEL_A  ; *rev = PI_VERSION_1_2 ; *mem = 0 ; *maker = PI_MAKER_EGOMAN  ; }
    else if (strcmp (c, "0008") == 0) { *model = PI_MODEL_A  ; *rev = PI_VERSION_1_2 ; *mem = 0 ; *maker = PI_MAKER_SONY ;  ; }
    else if (strcmp (c, "0009") == 0) { *model = PI_MODEL_A  ; *rev = PI_VERSION_1_2 ; *mem = 0 ; *maker = PI_MAKER_EGOMAN  ; }

    else if (strcmp (c, "000d") == 0) { *model = PI_MODEL_B  ; *rev = PI_VERSION_1_2 ; *mem = 1 ; *maker = PI_MAKER_EGOMAN  ; }
    else if (strcmp (c, "000e") == 0) { *model = PI_MODEL_B  ; *rev = PI_VERSION_1_2 ; *mem = 1 ; *maker = PI_MAKER_SONY    ; }
    else if (strcmp (c, "000f") == 0) { *model = PI_MODEL_B  ; *rev = PI_VERSION_1_2 ; *mem = 1 ; *maker = PI_MAKER_EGOMAN  ; }

    else if (strcmp (c, "0010") == 0) { *model = PI_MODEL_BP ; *rev = PI_VERSION_1_2 ; *mem = 1 ; *maker = PI_MAKER_SONY    ; }
    else if (strcmp (c, "0013") == 0) { *model = PI_MODEL_BP ; *rev = PI_VERSION_1_2 ; *mem = 1 ; *maker = PI_MAKER_EMBEST  ; }
    else if (strcmp (c, "0016") == 0) { *model = PI_MODEL_BP ; *rev = PI_VERSION_1_2 ; *mem = 1 ; *maker = PI_MAKER_SONY    ; }
    else if (strcmp (c, "0019") == 0) { *model = PI_MODEL_BP ; *rev = PI_VERSION_1_2 ; *mem = 1 ; *maker = PI_MAKER_EGOMAN  ; }

    else if (strcmp (c, "0011") == 0) { *model = PI_MODEL_CM ; *rev = PI_VERSION_1_1 ; *mem = 1 ; *maker = PI_MAKER_SONY    ; }
    else if (strcmp (c, "0014") == 0) { *model = PI_MODEL_CM ; *rev = PI_VERSION_1_1 ; *mem = 1 ; *maker = PI_MAKER_EMBEST  ; }
    else if (strcmp (c, "0017") == 0) { *model = PI_MODEL_CM ; *rev = PI_VERSION_1_1 ; *mem = 1 ; *maker = PI_MAKER_SONY    ; }
    else if (strcmp (c, "001a") == 0) { *model = PI_MODEL_CM ; *rev = PI_VERSION_1_1 ; *mem = 1 ; *maker = PI_MAKER_EGOMAN  ; }

    else if (strcmp (c, "0012") == 0) { *model = PI_MODEL_AP ; *rev = PI_VERSION_1_1 ; *mem = 0 ; *maker = PI_MAKER_SONY    ; }
    else if (strcmp (c, "0015") == 0) { *model = PI_MODEL_AP ; *rev = PI_VERSION_1_1 ; *mem = 1 ; *maker = PI_MAKER_EMBEST  ; }
    else if (strcmp (c, "0018") == 0) { *model = PI_MODEL_AP ; *rev = PI_VERSION_1_1 ; *mem = 0 ; *maker = PI_MAKER_SONY    ; }
    else if (strcmp (c, "001b") == 0) { *model = PI_MODEL_AP ; *rev = PI_VERSION_1_1 ; *mem = 0 ; *maker = PI_MAKER_EGOMAN  ; }

    else                              { *model = 0           ; *rev = 0              ; *mem =   0 ; *maker = 0 ;               }
  }
}
 


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


/*
 * delayMicroseconds:
 *	This is somewhat intersting. It seems that on the Pi, a single call
 *	to nanosleep takes some 80 to 130 microseconds anyway, so while
 *	obeying the standards (may take longer), it's not always what we
 *	want!
 *
 *	So what I'll do now is if the delay is less than 100uS we'll do it
 *	in a hard loop, watching a built-in counter on the ARM chip. This is
 *	somewhat sub-optimal in that it uses 100% CPU, something not an issue
 *	in a microcontroller, but under a multi-tasking, multi-user OS, it's
 *	wastefull, however we've no real choice )-:
 *
 *      Plan B: It seems all might not be well with that plan, so changing it
 *      to use gettimeofday () and poll on that instead...
 *********************************************************************************
 */

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


/*
 * millis:
 *	Return a number of milliseconds as an unsigned int.
 *	Wraps at 49 days.
 *********************************************************************************
 */

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


/*
 * micros:
 *	Return a number of microseconds as an unsigned int.
 *	Wraps after 71 minutes.
 *********************************************************************************
 */

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
  int   model, rev, mem, maker, overVolted ;

// It's actually a fatal error to call any of the wiringPiSetup routines more than once,
//	(you run out of file handles!) but I'm fed-up with the useless twats who email
//	me bleating that there is a bug in my code, so screw-em.

  if (wiringPiSetuped)
    return 0 ;

  wiringPiSetuped = TRUE ;


  if (getenv (ENV_CODES) != NULL)
    wiringPiReturnCodes = TRUE ;


// Get the board ID information. We're not really using the information here,
//	but it will give us information like the GPIO layout scheme (2 variants
//	on the older 26-pin Pi's) and the GPIO peripheral base address.
//	and if we're running on a compute module, then wiringPi pin numbers
//	don't really many anything, so force native BCM mode anyway.

  piBoardId (&model, &rev, &mem, &maker, &overVolted) ;
  printf ("model must be 9(pi zero):%d\n",model) ;

  //if ((model == PI_MODEL_CM) || (model == PI_MODEL_CM3))
  //  wiringPiMode = WPI_MODE_GPIO ;
  //else
    
  //wiringPiMode = WPI_MODE_PINS ;

  ///**/ if (piGpioLayout () == 1)	// A, B, Rev 1, 1.1
  //{
  //   pinToGpio =  pinToGpioR1 ;
  //}
  //else 					// A2, B2, A+, B+, CM, Pi2, Pi3, Zero
  //{
     pinToGpio =  pinToGpioR2 ;
  //}

// ...

  switch (model)
  {
    case PI_MODEL_A:	case PI_MODEL_B:
    case PI_MODEL_AP:	case PI_MODEL_BP:
    case PI_ALPHA:	case PI_MODEL_CM:
    case PI_MODEL_ZERO:	case PI_MODEL_ZERO_W:
      piGpioBase = GPIO_PERI_BASE_OLD ;
      break ;

    default:
      piGpioBase = GPIO_PERI_BASE_NEW ;
      break ;
  }

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
