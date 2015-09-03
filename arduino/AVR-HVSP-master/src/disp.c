/*
 * disp.c
 *
 *  Created on: 13 jul 2015
 *      Author: andreas
 */
#include <avr/io.h>

#include "conf.h"
#include "type.h"

#define NOF_DIGITS 4

/*
 ___a__
 |      |
 f|      | b
 ___g__
 e|      | c
 |      |
 ___d__
 */
//_____________________ abc defg
#define LTR_0 0x7e	// 0111 1110
#define LTR_1 0x30	// 0011 0000
#define LTR_2 0x6d	// 0110 1101
#define LTR_3 0x79	// 0111 1001
#define LTR_4 0x33	// 0011 0011
#define LTR_5 0x5b	// 0101 1011
#define LTR_6 0x5f	// 0101 1111
#define LTR_7 0x70	// 0111 0000
#define LTR_8 0x7f	// 0111 1111
#define LTR_9 0x7b	// 0111 1011
#define LTR_A 0x77	// 0111 0111
#define LTR_b 0x1f	// 0001 1111
#define LTR_C 0x4e	// 0100 1110
#define LTR_d 0x3d	// 0011 1101
#define LTR_E 0x4f	// 0100 1111
#define LTR_F 0x47	// 0100 0111

#define LTR_P 0x67	// 0110 0111
#define LTR_G 0x5e	// 0101 1110
#define LTR_n 0x15	// 0001 0101
#define LTR_r 0x05	// 0000 0101
#define LTR_t 0x0f	// 0000 1111
#define LTR__ 0x00	// 0000 0000
#define LTR_dash 0x01	// 0000 0001

#define CLK_HIGH()  IO_SET(DISP_SCK_CFG)
#define CLK_LOW()   IO_CLR(DISP_SCK_CFG)
#define CS_HIGH()   IO_SET(DISP_CS_CFG)
#define CS_LOW()    IO_CLR(DISP_CS_CFG)
#define DATA_HIGH() IO_SET(DISP_DATA_CFG)
#define DATA_LOW()  IO_CLR(DISP_DATA_CFG)

static void initMax7219(void);
static void writeRegister(tU08 address_U08, tU08 data_U08);
static void spiSend(tU08 data_U08);
static void clear(void);

void Disp_init(void)
{
	CONF_IO(DISP_SCK_CFG, OUTPUT, 1);
	CONF_IO(DISP_DATA_CFG, OUTPUT, 0);
	CONF_IO(DISP_CS_CFG, OUTPUT, 1);

	initMax7219();
}

static void initMax7219(void)
{
	// SCK, DO & LOAD

	// Decode mode: none
	writeRegister(0x09, 0x0);
	// Intensity: 3 (0-15)
	writeRegister(0x0A, DISP_INTENSITY);
	// Scan limit: All "digits" (rows) on
	writeRegister(0x0B, NOF_DIGITS - 1);
	// Shutdown register: Display on
	writeRegister(0x0C, 1);
	// Display test: off
	writeRegister(0x0F, 0);
	clear();
}

static void writeRegister(tU08 address_U08, tU08 data_U08)
{
	CS_LOW();
	spiSend(address_U08);
	spiSend(data_U08);
	CS_HIGH();
}

static void spiSend(tU08 data_U08)
{
	tU08 i_U08;

	for (i_U08 = 0; i_U08 < 8; i_U08++, data_U08 <<= 1)
	{
		CLK_LOW();
		if (data_U08 & 0x80)
			DATA_HIGH();
		else
			DATA_LOW();
		CLK_HIGH();
	}

}

void clear(void)
{
	tU08 i_U08;
	for (i_U08 = 0; i_U08 < NOF_DIGITS; i_U08++)
	{
		writeRegister(i_U08 + 1, 1);
	}
}

void Disp_write(const char* text)
{
	tU08 i_U08;
	for (i_U08 = 0; i_U08 < NOF_DIGITS; i_U08++)
	{
		switch (*text)
		{
		case '0':
			writeRegister(i_U08 + 1, LTR_0);
			break;
		case '1':
			writeRegister(i_U08 + 1, LTR_1);
			break;
		case '2':
			writeRegister(i_U08 + 1, LTR_2);
			break;
		case '3':
			writeRegister(i_U08 + 1, LTR_3);
			break;
		case '4':
			writeRegister(i_U08 + 1, LTR_4);
			break;
		case '5':
			writeRegister(i_U08 + 1, LTR_5);
			break;
		case '6':
			writeRegister(i_U08 + 1, LTR_6);
			break;
		case '7':
			writeRegister(i_U08 + 1, LTR_7);
			break;
		case '8':
			writeRegister(i_U08 + 1, LTR_8);
			break;
		case '9':
			writeRegister(i_U08 + 1, LTR_9);
			break;
		case 'A':
			writeRegister(i_U08 + 1, LTR_A);
			break;
		case 'B':
			writeRegister(i_U08 + 1, LTR_b);
			break;
		case 'C':
			writeRegister(i_U08 + 1, LTR_C);
			break;
		case 'D':
			writeRegister(i_U08 + 1, LTR_d);
			break;
		case 'E':
			writeRegister(i_U08 + 1, LTR_E);
			break;
		case 'F':
			writeRegister(i_U08 + 1, LTR_F);
			break;
		case 'T':
			writeRegister(i_U08 + 1, LTR_t);
			break;
		case '-':
			writeRegister(i_U08 + 1, LTR_dash);
			break;
		case ' ':
			writeRegister(i_U08 + 1, LTR__);
			break;
		default:
			writeRegister(i_U08 + 1, LTR__);
			break;
		}
		text++;
	}
}
