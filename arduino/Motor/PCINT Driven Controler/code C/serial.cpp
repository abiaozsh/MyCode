#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define CLKON PORTD |= _BV(3)
#define CLKOFF PORTD &= ~_BV(3)
#define CLKFLIP PIND |= _BV(3)
#define DATON PORTD |= _BV(2)
#define DATOFF PORTD &= ~_BV(2)

#define _DELAY_ delay();

void delay()
{
  for(uint16_t i = 0;i< 200;i++)
  {
    asm volatile("nop");
  }
}

void send(uint8_t val)
{
  DATON;//startbit
  _DELAY_;
  CLKFLIP;//rise send data
  _DELAY_;

  for(uint8_t i=0;i<8;i++)
  {
    if(val&0x80)
    {
      DATON;//1
    }
    else
    {
      DATOFF;//0
    }
    val<<=1;
    _DELAY_;
    CLKFLIP;//rise send data
    _DELAY_;
  }
}

volatile uint8_t SendData = 0;
volatile uint8_t Data;


int main(void) {
  TCCR1A = 0;
  TCCR1B = 2;//1/1024 (16000000/1024=15625)tick/s
  TCCR1C = 0;
  TIMSK1 = 0;

  DDRD = _BV(2) | _BV(3);
  DDRB |= _BV(5);
  DATOFF;
  _DELAY_;
  for(int i=0;i<10;i++)
  {
    CLKFLIP;//rise send data
    _DELAY_;
  }
  send(0);

  UCSR0A = 0;
  UCSR0B = _BV(TXEN0) | _BV(RXEN0) | _BV(TXCIE0) | _BV(RXCIE0);
  UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
  UBRR0 = 8;//115200
  
  sei();
  uint8_t oldval = PIND & _BV(4);
  uint16_t sum = 0;
  uint8_t cnt = 0;
  while(true)
  {
    uint8_t val = PIND & _BV(4);
    if(oldval!=val)
    {
      sum += TCNT1;
      TCNT1=0;
      
      cnt++;
      if(cnt==8)
      {
        SendData = 1;
        UDR0 = (uint8_t)sum&0xFF;
        Data = (uint8_t)(sum>>8)&0xFF;
        cnt = 0;
        sum = 0;
      }
      
      
      oldval = val;
    }
  }
}

ISR(USART_RX_vect){
  uint8_t val = UDR0;
  send(val);
}

ISR(USART_TX_vect){
  if(SendData)
  {
    UDR0 = Data;
    SendData = 0;
  }
}
