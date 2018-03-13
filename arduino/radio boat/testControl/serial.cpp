#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

void loop();

int main(void) {
	CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	//CLKPR = 3;//1/8
	CLKPR = 0;//1/1 //8MHz

//1： 1us/8tick     8000000/65536  122.0703125  0.008192s     8.192ms
//2: 1us/tick   20ms/次 ICR1 = 20000

//WGM3210
//14 1110 Fast PWM ICR1 TOP TOP  
//Compare Output Mode, Fast PWM
//1 0 Clear OC1A/OC1B on Compare Match, set OC1A/OC1B at BOTTOM (non-inverting mode)

	TCCR1A = _BV(COM1A1) + _BV(COM1B1) + _BV(WGM11);
  TCCR1B = _BV(CS11) + _BV(WGM13) + _BV(WGM12);
	TIMSK1 = 0;
  ICR1 = 20000;
  //PA5 OC1B
  DDRA |= _BV(5);
  //PA6 OC1A
  DDRA |= _BV(6);
  

  sei();


  //ADMUX = POWER_IN;//A7 power
  DIDR0 |= _BV(0);//1
  DIDR0 |= _BV(1);//1
  ADCSRA = _BV(ADEN) | _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2);// | _BV(ADIE) | _BV(ADATE) _BV(ADSC) | 
  ADCSRB = _BV(ADLAR);
  

  
	loop();
}


uint8_t aread(uint8_t pin){
  ADMUX = pin;
  ADCSRA |= _BV(ADSC);while(ADCSRA & _BV(ADSC));
  return ADCH;
}

void loop() {
	for(;;)
	{
      uint8_t val0 = aread(0);
      uint8_t val1 = aread(1);

      uint16_t tempA = 0;
      uint16_t tempB = 0;
      
      uint32_t temp = val0;
      temp = temp * 2000;
      temp = temp>>8;
      
      tempA = 500;
      tempA = tempA + temp;
      
			OCR1A = tempA;//1000~2000;//1ms~2ms
			OCR1B = tempB;//0.5到2.5mS 对应 舵机的 -90度到+90度
	}
}
