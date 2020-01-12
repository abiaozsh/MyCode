
#include <Wire.h>
 #include "Arduino.h"


/*=========================================================================
    I2C ADDRESS/BITS
    -----------------------------------------------------------------------*/
   // #define BMP280_ADDRESS                (0x77)
	 #define BMP280_ADDRESS                (0x76)
/*=========================================================================*/

/*=========================================================================
    REGISTERS
    -----------------------------------------------------------------------*/
    enum
    {
      BMP280_REGISTER_DIG_T1              = 0x88,
      BMP280_REGISTER_DIG_T2              = 0x8A,
      BMP280_REGISTER_DIG_T3              = 0x8C,

      BMP280_REGISTER_DIG_P1              = 0x8E,
      BMP280_REGISTER_DIG_P2              = 0x90,
      BMP280_REGISTER_DIG_P3              = 0x92,
      BMP280_REGISTER_DIG_P4              = 0x94,
      BMP280_REGISTER_DIG_P5              = 0x96,
      BMP280_REGISTER_DIG_P6              = 0x98,
      BMP280_REGISTER_DIG_P7              = 0x9A,
      BMP280_REGISTER_DIG_P8              = 0x9C,
      BMP280_REGISTER_DIG_P9              = 0x9E,

      BMP280_REGISTER_CHIPID             = 0xD0,
      BMP280_REGISTER_VERSION            = 0xD1,
      BMP280_REGISTER_SOFTRESET          = 0xE0,

      BMP280_REGISTER_CAL26              = 0xE1,  // R calibration stored in 0xE1-0xF0

      BMP280_REGISTER_CONTROL            = 0xF4,
      BMP280_REGISTER_CONFIG             = 0xF5,
      BMP280_REGISTER_PRESSUREDATA       = 0xF7,
      BMP280_REGISTER_TEMPDATA           = 0xFA,
    };

/*=========================================================================*/

/*=========================================================================
    CALIBRATION DATA
    -----------------------------------------------------------------------*/
    typedef struct
    {
      uint16_t dig_T1;
      int16_t  dig_T2;
      int16_t  dig_T3;

      uint16_t dig_P1;
      int16_t  dig_P2;
      int16_t  dig_P3;
      int16_t  dig_P4;
      int16_t  dig_P5;
      int16_t  dig_P6;
      int16_t  dig_P7;
      int16_t  dig_P8;
      int16_t  dig_P9;

      uint8_t  dig_H1;
      int16_t  dig_H2;
      uint8_t  dig_H3;
      int16_t  dig_H4;
      int16_t  dig_H5;
      int8_t   dig_H6;
    } bmp280_calib_data;
/*=========================================================================*/

/*
class Adafruit_BMP280_Unified : public Adafruit_Sensor
{
  public:
    Adafruit_BMP280_Unified(int32_t sensorID = -1);
  
    bool  begin(uint8_t addr = BMP280_ADDRESS);
    void  getTemperature(float *temp);
    void  getPressure(float *pressure);
    float pressureToAltitude(float seaLevel, float atmospheric, float temp);
    float seaLevelForAltitude(float altitude, float atmospheric, float temp);
    void  getEvent(sensors_event_t*);
    void  getSensor(sensor_t*);

  private:
    uint8_t   _i2c_addr;
    int32_t   _sensorID;
};

*/

class Adafruit_BMP280
{
  public:
    Adafruit_BMP280(void);
    Adafruit_BMP280(int8_t cspin);
    Adafruit_BMP280(int8_t cspin, int8_t mosipin, int8_t misopin, int8_t sckpin);

    bool  begin(uint8_t addr = BMP280_ADDRESS);
    float readTemperature(void);
    float readPressure(void);

    //  float pressureToAltitude(float seaLevel, float atmospheric, float temp);
    // float seaLevelForAltitude(float altitude, float atmospheric, float temp);

  private:

    void readCoefficients(void);

    void      write8(byte reg, byte value);
    uint8_t   read8(byte reg);
    uint16_t  read16(byte reg);
    int16_t   readS16(byte reg);
    uint16_t  read16_LE(byte reg); // little endian
    int16_t   readS16_LE(byte reg); // little endian

    uint8_t   _i2caddr;
    int32_t   _sensorID;
    int32_t t_fine;


};







static bmp280_calib_data bmp280_calib; 

/***************************************************************************
 PRIVATE FUNCTIONS
 ***************************************************************************/


Adafruit_BMP280::Adafruit_BMP280() 
{ }



bool Adafruit_BMP280::begin(uint8_t a) {
  _i2caddr = a;

    // i2c
    Wire.begin();

  if (read8(BMP280_REGISTER_CHIPID) != 0x58)
    return false;

  readCoefficients();
  write8(BMP280_REGISTER_CONTROL, 0x3F);
  return true;
}

/**************************************************************************/
/*!
    @brief  Writes an 8 bit value over I2C/SPI
*/
/**************************************************************************/
void Adafruit_BMP280::write8(byte reg, byte value)
{
    Wire.beginTransmission((uint8_t)_i2caddr);
    Wire.write((uint8_t)reg);
    Wire.write((uint8_t)value);
    Wire.endTransmission();
}

/**************************************************************************/
/*!
    @brief  Reads an 8 bit value over I2C
*/
/**************************************************************************/
uint8_t Adafruit_BMP280::read8(byte reg)
{
  uint8_t value;
  
    Wire.beginTransmission((uint8_t)_i2caddr);
    Wire.write((uint8_t)reg);
    Wire.endTransmission();
    Wire.requestFrom((uint8_t)_i2caddr, (byte)1);
    value = Wire.read();
    Wire.endTransmission();
  return value;
}

/**************************************************************************/
/*!
    @brief  Reads a 16 bit value over I2C
*/
/**************************************************************************/
uint16_t Adafruit_BMP280::read16(byte reg)
{
  uint16_t value;

    Wire.beginTransmission((uint8_t)_i2caddr);
    Wire.write((uint8_t)reg);
    Wire.endTransmission();
    Wire.requestFrom((uint8_t)_i2caddr, (byte)2);
    value = (Wire.read() << 8) | Wire.read();
    Wire.endTransmission();

  return value;
}

uint16_t Adafruit_BMP280::read16_LE(byte reg) {
  uint16_t temp = read16(reg);
  return (temp >> 8) | (temp << 8);
  
}

/**************************************************************************/
/*!
    @brief  Reads a signed 16 bit value over I2C
*/
/**************************************************************************/
int16_t Adafruit_BMP280::readS16(byte reg)
{
  return (int16_t)read16(reg);

}

int16_t Adafruit_BMP280::readS16_LE(byte reg)
{
  return (int16_t)read16_LE(reg);

}

/**************************************************************************/
/*!
    @brief  Reads the factory-set coefficients
*/
/**************************************************************************/
void Adafruit_BMP280::readCoefficients(void)
{
    bmp280_calib.dig_T1 = read16_LE(BMP280_REGISTER_DIG_T1);
    bmp280_calib.dig_T2 = readS16_LE(BMP280_REGISTER_DIG_T2);
    bmp280_calib.dig_T3 = readS16_LE(BMP280_REGISTER_DIG_T3);

    bmp280_calib.dig_P1 = read16_LE(BMP280_REGISTER_DIG_P1);
    bmp280_calib.dig_P2 = readS16_LE(BMP280_REGISTER_DIG_P2);
    bmp280_calib.dig_P3 = readS16_LE(BMP280_REGISTER_DIG_P3);
    bmp280_calib.dig_P4 = readS16_LE(BMP280_REGISTER_DIG_P4);
    bmp280_calib.dig_P5 = readS16_LE(BMP280_REGISTER_DIG_P5);
    bmp280_calib.dig_P6 = readS16_LE(BMP280_REGISTER_DIG_P6);
    bmp280_calib.dig_P7 = readS16_LE(BMP280_REGISTER_DIG_P7);
    bmp280_calib.dig_P8 = readS16_LE(BMP280_REGISTER_DIG_P8);
    bmp280_calib.dig_P9 = readS16_LE(BMP280_REGISTER_DIG_P9);
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
float Adafruit_BMP280::readTemperature(void)
{
  int32_t var1, var2;
  
  int32_t adc_T = read16(BMP280_REGISTER_TEMPDATA);
  adc_T <<= 8;
  adc_T |= read8(BMP280_REGISTER_TEMPDATA+2);
  adc_T >>= 4;

  var1  = ((((adc_T>>3) - ((int32_t)bmp280_calib.dig_T1 <<1))) * 
	   ((int32_t)bmp280_calib.dig_T2)) >> 11;

  var2  = (((((adc_T>>4) - ((int32_t)bmp280_calib.dig_T1)) * 
	     ((adc_T>>4) - ((int32_t)bmp280_calib.dig_T1))) >> 12) * 
	   ((int32_t)bmp280_calib.dig_T3)) >> 14;

  t_fine = var1 + var2;

  float T  = (t_fine * 5 + 128) >> 8;
  return T/100;
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
float Adafruit_BMP280::readPressure(void) {
  int64_t var1, var2, p;
  
  int32_t adc_P = read16(BMP280_REGISTER_PRESSUREDATA);
  adc_P <<= 8;
  adc_P |= read8(BMP280_REGISTER_PRESSUREDATA+2);
  adc_P >>= 4;

  var1 = ((int64_t)t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)bmp280_calib.dig_P6;
  var2 = var2 + ((var1*(int64_t)bmp280_calib.dig_P5)<<17);
  var2 = var2 + (((int64_t)bmp280_calib.dig_P4)<<35);
  var1 = ((var1 * var1 * (int64_t)bmp280_calib.dig_P3)>>8) +
    ((var1 * (int64_t)bmp280_calib.dig_P2)<<12);
  var1 = (((((int64_t)1)<<47)+var1))*((int64_t)bmp280_calib.dig_P1)>>33;
  
  if (var1 == 0) {
    return 0;  // avoid exception caused by division by zero
  }
  p = 1048576 - adc_P;
  p = (((p<<31) - var2)*3125) / var1;
  var1 = (((int64_t)bmp280_calib.dig_P9) * (p>>13) * (p>>13)) >> 25;
  var2 = (((int64_t)bmp280_calib.dig_P8) * p) >> 19;

  p = ((p + var1 + var2) >> 8) + (((int64_t)bmp280_calib.dig_P7)<<4);
  return (float)p/256;
}


























Adafruit_BMP280 bme; // I2C
  
void setup() {
  Serial.begin(9600);
  Serial.println(F("BMP280 test"));
  
  if (!bme.begin()) {  
    Serial.println("Could not find!");
    while (1);
  }
}
  
void loop() {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");
    
    Serial.print("Pressure = ");
    Serial.print(bme.readPressure());
    Serial.println(" Pa");

    Serial.println();
    delay(500);
}
