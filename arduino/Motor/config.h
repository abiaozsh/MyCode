
//2 1 0
//5 4 3 2 1 0
#define PORT6O PORTA
#define DDR6O DDRA

#define DDR3I DDRB
#define PIN3I PINB

#define boardNew
//#define boardold


#ifdef boardold
  #define BP1D _BV(5)
  #define BP1U _BV(4)
  #define BP1A _BV(0)
  #define BP2D _BV(3)
  #define BP2U _BV(2)
  #define BP2A _BV(1)
  #define BP3D _BV(1)
  #define BP3U _BV(0)
  #define BP3A _BV(2)
  
  �±۳���������
  uint8_t PWR_ON[] = {
    BP1U + BP1D + BP3D, // 1-2 a
    BP1U + BP1D + BP2D, // 1-3 b
    BP2U + BP1D + BP2D, // 2-3 c
    BP2U + BP3D + BP2D, // 2-1 d
    BP3U + BP3D + BP2D, // 3-1 e
    BP3U + BP1D + BP3D  // 3-2 f
  };
  uint8_t PWR_OFF[] = {
    0    + BP1D + BP3D, // 1-2
    0    + BP1D + BP2D, // 1-3
    0    + BP1D + BP2D, // 2-3
    0    + BP3D + BP2D, // 2-1
    0    + BP3D + BP2D, // 3-1
    0    + BP1D + BP3D  // 3-2
  };
#endif

#ifdef boardNew
  #define BP1U _BV(5)
  #define BP1D _BV(4)
  #define BP1A _BV(0)
  #define BP2U _BV(3)
  #define BP2D _BV(2)
  #define BP2A _BV(1)
  #define BP3U _BV(1)
  #define BP3D _BV(0)
  #define BP3A _BV(2)


  //�±۳���������
  uint8_t PWR_ON[] = {
    BP1U + BP2D, // 1-2 a
    BP1U + BP3D, // 1-3 b
    BP2U + BP3D, // 2-3 c
    BP2U + BP1D, // 2-1 d
    BP3U + BP1D, // 3-1 e
    BP3U + BP2D  // 3-2 f
  };
  uint8_t PWR_OFF[] = {
    0    + BP2D, // 1-2
    0    + BP3D, // 1-3
    0    + BP3D, // 2-3
    0    + BP1D, // 2-1
    0    + BP1D, // 3-1
    0    + BP2D  // 3-2
  };
#endif