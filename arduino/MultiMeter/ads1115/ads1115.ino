#ifndef ADS1115_H
#define ADS1115_H

#define ADS1115_COMP_QUEUE_SHIFT 0
#define ADS1115_COMP_LATCH_SHIFT 2
#define ADS1115_COMP_POLARITY_SHIFT 3
#define ADS1115_COMP_MODE_SHIFT 4
#define ADS1115_DATA_RATE_SHIFT 5
#define ADS1115_MODE_SHIFT 8
#define ADS1115_PGA_SHIFT 9
#define ADS1115_MUX_SHIFT 12

enum ads1115_comp_queue {
	ADS1115_COMP_QUEUE_AFTER_ONE = 0,
	ADS1115_COMP_QUEUE_AFTER_TWO = 0x1 << ADS1115_COMP_QUEUE_SHIFT,
	ADS1115_COMP_QUEUE_AFTER_FOUR = 0x2 << ADS1115_COMP_QUEUE_SHIFT,
	ADS1115_COMP_QUEUE_DISABLE = 0x3 << ADS1115_COMP_QUEUE_SHIFT,
	ADS1115_COMP_QUEUE_MASK = 0x3 << ADS1115_COMP_QUEUE_SHIFT,
};

enum ads1115_comp_latch {
	ADS1115_COMP_LATCH_NO = 0,
	ADS1115_COMP_LATCH_YES = 1 << ADS1115_COMP_LATCH_SHIFT,
	ADS1115_COMP_LATCH_MASK = 1 << ADS1115_COMP_LATCH_SHIFT,
};

enum ads1115_comp_polarity {
	ADS1115_COMP_POLARITY_ACTIVE_LOW = 0,
	ADS1115_COMP_POLARITY_ACTIVE_HIGH = 1 << ADS1115_COMP_POLARITY_SHIFT,
	ADS1115_COMP_POLARITY_MASK = 1 << ADS1115_COMP_POLARITY_SHIFT,
};

enum ads1115_comp_mode {
	ADS1115_COMP_MODE_WINDOW = 0,
	ADS1115_COMP_MODE_HYSTERESIS = 1 << ADS1115_COMP_MODE_SHIFT,
	ADS1115_COMP_MODE_MASK = 1 << ADS1115_COMP_MODE_SHIFT,
};

enum ads1115_data_rate {
	ADS1115_DATA_RATE_8_SPS = 0,
	ADS1115_DATA_RATE_16_SPS = 0x1 << ADS1115_DATA_RATE_SHIFT,
	ADS1115_DATA_RATE_32_SPS = 0x2 << ADS1115_DATA_RATE_SHIFT,
	ADS1115_DATA_RATE_64_SPS = 0x3 << ADS1115_DATA_RATE_SHIFT,
	ADS1115_DATA_RATE_128_SPS = 0x4 << ADS1115_DATA_RATE_SHIFT,
	ADS1115_DATA_RATE_250_SPS = 0x5 << ADS1115_DATA_RATE_SHIFT,
	ADS1115_DATA_RATE_475_SPS = 0x6 << ADS1115_DATA_RATE_SHIFT,
	ADS1115_DATA_RATE_860_SPS = 0x7 << ADS1115_DATA_RATE_SHIFT,
	ADS1115_DATA_RATE_MASK = 0x7 << ADS1115_DATA_RATE_SHIFT,
};

enum ads1115_mode {
	ADS1115_MODE_CONTINUOUS = 0,
	ADS1115_MODE_SINGLE_SHOT = 1 << ADS1115_MODE_SHIFT,
	ADS1115_MODE_MASK = 1 << ADS1115_MODE_SHIFT,
};

enum ads1115_pga {
	ADS1115_PGA_TWO_THIRDS = 0,
	ADS1115_PGA_ONE = 0x1 << ADS1115_PGA_SHIFT,
	ADS1115_PGA_TWO = 0x2 << ADS1115_PGA_SHIFT,
	ADS1115_PGA_FOUR = 0x3 << ADS1115_PGA_SHIFT,
	ADS1115_PGA_EIGHT = 0x4 << ADS1115_PGA_SHIFT,
	ADS1115_PGA_SIXTEEN = 0x5 << ADS1115_PGA_SHIFT,
	ADS1115_PGA_MASK = 0x7 << ADS1115_PGA_SHIFT,
};

enum ads1115_mux {
	ADS1115_MUX_DIFF_AIN0_AIN1 = 0,
	ADS1115_MUX_DIFF_AIN0_AIN3 = 0x1 << ADS1115_MUX_SHIFT,
	ADS1115_MUX_DIFF_AIN1_AIN3 = 0x2 << ADS1115_MUX_SHIFT,
	ADS1115_MUX_DIFF_AIN2_AIN3 = 0x3 << ADS1115_MUX_SHIFT,
	ADS1115_MUX_GND_AIN0 = 0x4 << ADS1115_MUX_SHIFT,
	ADS1115_MUX_GND_AIN1 = 0x5 << ADS1115_MUX_SHIFT,
	ADS1115_MUX_GND_AIN2 = 0x6 << ADS1115_MUX_SHIFT,
	ADS1115_MUX_GND_AIN3 = 0x7 << ADS1115_MUX_SHIFT,
	ADS1115_MUX_MASK = 0x7 << ADS1115_MUX_SHIFT,
};

class ADS1115 {
public:
	ADS1115(uint8_t address = 0x48);

	void begin();
	uint8_t trigger_sample();
	uint8_t reset();
	bool is_sample_in_progress();
	int16_t read_sample();
	float sample_to_float(int16_t val);
	float read_sample_float();

	void set_comp_queue(enum ads1115_comp_queue val) { set_config(val, ADS1115_COMP_QUEUE_MASK); }
	void set_comp_latching(enum ads1115_comp_latch val) { set_config(val, ADS1115_COMP_LATCH_MASK); }
	void set_comp_polarity(enum ads1115_comp_polarity val) { set_config(val, ADS1115_COMP_POLARITY_MASK); }
	void set_comp_mode(enum ads1115_comp_mode val) { set_config(val, ADS1115_COMP_MODE_MASK); }
	void set_data_rate(enum ads1115_data_rate val) { set_config(val, ADS1115_DATA_RATE_MASK); }
	void set_mode(enum ads1115_mode val) { set_config(val, ADS1115_MODE_MASK); }
	void set_pga(enum ads1115_pga val) { set_config(val, ADS1115_PGA_MASK); m_voltage_range = val >> ADS1115_PGA_SHIFT; }
	void set_mux(enum ads1115_mux val) { set_config(val, ADS1115_MUX_MASK); }

private:
	void set_config(uint16_t val, uint16_t mask) {
		m_config = (m_config & ~mask) | val;
	}

	uint8_t write_register(uint8_t reg, uint16_t val);
	uint16_t read_register(uint8_t reg);

	uint8_t m_address;
	uint16_t m_config;
	int m_voltage_range;
};

#endif







#include <Arduino.h>
#include <Wire.h>

#define SAMPLE_BIT (0x8000)

enum ads1115_register {
	ADS1115_REGISTER_CONVERSION = 0,
	ADS1115_REGISTER_CONFIG = 1,
	ADS1115_REGISTER_LOW_THRESH = 2,
	ADS1115_REGISTER_HIGH_THRESH = 3,
};

#define FACTOR 32768.0
static float ranges[] = { 6.144 / FACTOR, 4.096 / FACTOR, 2.048 / FACTOR, 1.024 / FACTOR, 0.512 / FACTOR, 0.256 / FACTOR};

ADS1115::ADS1115(uint8_t address)
{
        m_address = address;
        m_config = ADS1115_COMP_QUEUE_AFTER_ONE |
                   ADS1115_COMP_LATCH_NO |
                   ADS1115_COMP_POLARITY_ACTIVE_LOW |
                   ADS1115_COMP_MODE_WINDOW |
                   ADS1115_DATA_RATE_128_SPS |
                   ADS1115_MODE_SINGLE_SHOT |
                   ADS1115_MUX_GND_AIN0;
        set_pga(ADS1115_PGA_ONE);
}

uint8_t ADS1115::write_register(uint8_t reg, uint16_t val)
{
        Wire.beginTransmission(m_address);
        Wire.write(reg);
        Wire.write(val>>8);
        Wire.write(val & 0xFF);
        return Wire.endTransmission();
}

uint16_t ADS1115::read_register(uint8_t reg)
{
        Wire.beginTransmission(m_address);
        Wire.write(reg);
        Wire.endTransmission();

        uint8_t result = Wire.requestFrom((int)m_address, 2, 1);
        if (result != 2) {
                Wire.flush();
                return 0;
        }

        uint16_t val;

        val = Wire.read() << 8;
        val |= Wire.read();
        return val;
}

void ADS1115::begin()
{
        Wire.begin();
}

uint8_t ADS1115::trigger_sample()
{
        return write_register(ADS1115_REGISTER_CONFIG, m_config | SAMPLE_BIT);
}

uint8_t ADS1115::reset()
{
	Wire.beginTransmission(0);
	Wire.write(0x6);
	return Wire.endTransmission();
}

bool ADS1115::is_sample_in_progress()
{
	uint16_t val = read_register(ADS1115_REGISTER_CONFIG);
	return (val & SAMPLE_BIT) == 0;
}

int16_t ADS1115::read_sample()
{
        return read_register(ADS1115_REGISTER_CONVERSION);
}

float ADS1115::sample_to_float(int16_t val)
{
	return val * ranges[m_voltage_range];
}

float ADS1115::read_sample_float()
{
	return sample_to_float(read_sample());
}









ADS1115 adc;

void setup() {
    adc.begin();
    adc.set_data_rate(ADS1115_DATA_RATE_8_SPS);
    adc.set_mode(ADS1115_MODE_CONTINUOUS);
    adc.set_mux(ADS1115_MUX_DIFF_AIN0_AIN1);
    adc.set_pga(ADS1115_PGA_ONE);
Serial.begin(19200);// read at 9600
    if (adc.trigger_sample() != 0){
            Serial.println("adc read trigger failed (ads1115 not connected?)");
    }
}

void loop() {
        /* You will be oversampling if the loop takes too short a time */
        int32_t val = 0;
        uint16_t i = 0;
        for(i=0;i<4;i++){
         val += adc.read_sample();
        Serial.print(adc.is_sample_in_progress());
        }
        Serial.println(val);
}
