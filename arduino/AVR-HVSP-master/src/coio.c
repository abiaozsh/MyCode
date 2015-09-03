/*
 * coio.c
 *
 *  Created on: 17 jul 2015
 *      Author: andreas
 */
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "coio.h"
#include "conf.h"

#define NOF_DEVICES 7

#define VCC_LO IO_SET(COIO_VCC_CFG)
#define VCC_HI IO_CLR(COIO_VCC_CFG)
#define RES_LO IO_SET(COIO_RESET_CFG)
#define RES_HI IO_CLR(COIO_RESET_CFG)

#define SCI_LO IO_CLR(COIO_SCI_CFG)
#define SCI_HI IO_SET(COIO_SCI_CFG)
#define SDI_LO IO_CLR(COIO_SDI_CFG)
#define SDI_HI IO_SET(COIO_SDI_CFG)
#define SII_LO IO_CLR(COIO_SII_CFG)
#define SII_HI IO_SET(COIO_SII_CFG)
#define SDO_LO IO_CLR(COIO_SDO_CFG)
#define SDO_HI IO_SET(COIO_SDO_CFG)

typedef struct
{
	tU08 signatureBlock_U08[3];
} tSignature_str;
;

typedef struct
{
	char name_aC[4];
	tU08 signature_aU08[3];
	tU08 fuseLowBits_U08;
	tU08 fuseHighBits_U08;
	tU08 fuseExtendedBits_U08;
} tTargetCpuInfo_str;

static const tTargetCpuInfo_str PROGMEM deviceDefault_astr[NOF_DEVICES] =
{
{	// ATtiny13
		.name_aC = " T13", .signature_aU08 =
		{ SIGNATURE_0, 0x90, 0x07 }, .fuseLowBits_U08 = 0x6A,
				.fuseHighBits_U08 = 0xFF, .fuseExtendedBits_U08 = 0x00 },
		{	// ATtiny24
		.name_aC = "T24 ", .signature_aU08 =
		{ SIGNATURE_0, 0x91, 0x0B }, .fuseLowBits_U08 = 0x62,
				.fuseHighBits_U08 = 0xDF, .fuseExtendedBits_U08 = 0xFF, },
		{	// ATtiny44
		.name_aC = "T44 ", .signature_aU08 =
		{ SIGNATURE_0, 0x92, 0x07 }, .fuseLowBits_U08 = 0x62,
				.fuseHighBits_U08 = 0xDF, .fuseExtendedBits_U08 = 0xFF, },
		{	// ATtiny84
		.name_aC = "T84 ", .signature_aU08 =
		{ SIGNATURE_0, 0x93, 0x0C }, .fuseLowBits_U08 = 0x62,
				.fuseHighBits_U08 = 0xDF, .fuseExtendedBits_U08 = 0xFF, },
		{	// ATtiny25
		.name_aC = "T25 ", .signature_aU08 =
		{ SIGNATURE_0, 0x91, 0x08 }, .fuseLowBits_U08 = 0x62,
				.fuseHighBits_U08 = 0xDF, .fuseExtendedBits_U08 = 0xFF, },
		{	// ATtiny45
		.name_aC = "T45 ", .signature_aU08 =
		{ SIGNATURE_0, 0x92, 0x06 }, .fuseLowBits_U08 = 0x62,
				.fuseHighBits_U08 = 0xDF, .fuseExtendedBits_U08 = 0xFF, },
		{	// ATtiny85
		.name_aC = "T85 ", .signature_aU08 =
		{ SIGNATURE_0, 0x93, 0x0B }, .fuseLowBits_U08 = 0x62,
				.fuseHighBits_U08 = 0xDF, .fuseExtendedBits_U08 = 0xFF, },
// mark end of list
		};

tTargetCpuInfo_str targetDevice_str =
{
{' ', ' ', ' ', ' ' },
{ 0x00, 0x00, 0x00 }, 0x00, 0x00, 0x00 };

static void enterHVSP(void);
static void exitHVSP(void);
static tU08 getSignatureByte_U08(tU08 byte_U08);
static tU08 getLowFuse_U08(void);
static tU08 getHighFuse_U08(void);
static tU08 getExtendedFuse_U08(void);
static tU08 transferByte_U08(tU08 data_U08, tU08 instruction_U08);
static inline void clockit(void);
static void writeFuseLowBits(tU08 code_U08);
static void writeFuseHighBits(tU08 code_U08);
static void writeFuseExtendedBits(tU08 code_U08);
static void pollSDO(void);
static const tTargetCpuInfo_str * findDevice_pstr(tU08 * signature_pU08);
static tB updateSignatureName_B(tU08 * signature_pU08);

void Coio_init(void)
{
}

tCoio_ReadResult_E Coio_readChip_E(void)
{
	tU08 byte_U08;
	tB deviceFound_B;
	tCoio_ReadResult_E result_E;
	enterHVSP();
	for (byte_U08 = 0; byte_U08 < 3; byte_U08++)
	{
		targetDevice_str.signature_aU08[byte_U08] = getSignatureByte_U08(
				byte_U08);
	}
	deviceFound_B = updateSignatureName_B(targetDevice_str.signature_aU08);
	targetDevice_str.fuseLowBits_U08 = getLowFuse_U08();
	targetDevice_str.fuseHighBits_U08 = getHighFuse_U08();
	targetDevice_str.fuseExtendedBits_U08 = getExtendedFuse_U08();
	exitHVSP();

	if (targetDevice_str.signature_aU08[0] == SIGNATURE_0)
	{
		if (deviceFound_B == TRUE)
		{
			result_E = COIO_OK_E;
		}
		else
		{
			result_E = COIO_UNKNOWN_DEVICE_E;
		}
	}
	else
	{
		result_E = COIO_CONNECTION_FAIL_E;
	}

	return result_E;
}

tB Coio_resetChip_B(void)
{
	const tTargetCpuInfo_str * target_pstr = findDevice_pstr(
			targetDevice_str.signature_aU08);
	tB fuseRestoreSuccessful_B = TRUE;

	/* get default fuse bits for selected target */
	uint8_t fuseLowBits = pgm_read_byte(&target_pstr->fuseLowBits_U08);
	uint8_t fuseHighBits = pgm_read_byte(&target_pstr->fuseHighBits_U08);
	uint8_t fuseExtendedBits = pgm_read_byte(
			&target_pstr->fuseExtendedBits_U08);
	enterHVSP();
	/* write default fuse bits */
	writeFuseLowBits(fuseLowBits);
	writeFuseHighBits(fuseHighBits);
	if (fuseExtendedBits != 0)
	{
		writeFuseExtendedBits(fuseExtendedBits);
	}

	/* verify */
	if ((fuseLowBits == getLowFuse_U08()) && (fuseHighBits == getHighFuse_U08())
			&& ((0 == fuseExtendedBits)
					|| (fuseExtendedBits == getExtendedFuse_U08())))
	{
		fuseRestoreSuccessful_B = TRUE;
	}
	else
	{
		fuseRestoreSuccessful_B = FALSE;
	}
	exitHVSP();
	return fuseRestoreSuccessful_B;
}

/*
 The following algorithm puts the device in High-voltage Serial Programming mode:
 (see Atmel ATtiny data sheet chapter Memory Programming -> High Voltage Serial Programming)

 1. Set Prog_enable pins (SDI, SII, SDO, SCI) 0, RESET pin and VCC to 0V.
 2. Apply 5V between VCC and GND. Ensure that VCC reaches at least 1.8V within the next 20 microseconds.
 3. Wait 20 - 60 us, and apply 11.5 - 12.5V to RESET.
 4. Keep the Prog_enable pins unchanged for at least 10 us after the High-voltage has been
 applied to ensure the Prog_enable Signature has been latched.
 5. Release the Prog_enable[2] (SDO pin) to avoid drive contention on the Prog_enable[2]/SDO pin.
 6. Wait at least 300 us before giving any serial instructions on SDI/SII.
 7. Exit Programming mode by power the device down or by bringing RESET pin to 0V.
 */

static void enterHVSP(void)
{
	// 1
	CONF_IO(COIO_VCC_CFG, OUTPUT, 1);
	CONF_IO(COIO_RESET_CFG, OUTPUT, 1);
	CONF_IO(COIO_SCI_CFG, OUTPUT, 0);
	CONF_IO(COIO_SDI_CFG, OUTPUT, 0);
	CONF_IO(COIO_SII_CFG, OUTPUT, 0);
	CONF_IO(COIO_SDO_CFG, OUTPUT, 0);

	// 2
	VCC_HI;

	// 3
	_delay_us(60);
	RES_HI;

	// 4
	_delay_us(10);

	// 5
	CONF_IO(COIO_SDO_CFG, INPUT, NOPULLUP);

	// 6
	_delay_us(300);
}

static void exitHVSP(void)
{
	// 7
	SCI_LO;
	SDI_LO;
	SII_LO;
	_delay_us(10);
	VCC_LO;
	RES_LO;
	_delay_us(200);
	CONF_IO(COIO_SDO_CFG, OUTPUT, 0);
}

static tU08 getSignatureByte_U08(tU08 byte_U08)
{
	transferByte_U08(0x08, 0x4C);
	transferByte_U08(byte_U08, 0x0C);
	transferByte_U08(0x00, 0x68);
	return transferByte_U08(0x00, 0x6C);
}

static tU08 getLowFuse_U08(void)
{
	transferByte_U08(0x04, 0x4C);
	transferByte_U08(0x00, 0x68);
	return transferByte_U08(0x00, 0x6C);
}

static tU08 getHighFuse_U08(void)
{
	transferByte_U08(0x04, 0x4C);
	transferByte_U08(0x00, 0x7A);
	return transferByte_U08(0x00, 0x7E);
}

static tU08 getExtendedFuse_U08(void)
{
	transferByte_U08(0x04, 0x4C);
	transferByte_U08(0x00, 0x6A);
	return transferByte_U08(0x00, 0x6E);
}

static void writeFuseLowBits(tU08 code_U08)
{
	transferByte_U08(0x40, 0x4C);
	transferByte_U08(code_U08, 0x2C);
	transferByte_U08(0x00, 0x64);
	transferByte_U08(0x00, 0x6C);
	pollSDO();
	transferByte_U08(0x00, 0x4C);
	pollSDO();
}

static void writeFuseHighBits(tU08 code_U08)
{
	transferByte_U08(0x40, 0x4C);
	transferByte_U08(code_U08, 0x2C);
	transferByte_U08(0x00, 0x74);
	transferByte_U08(0x00, 0x7C);
	pollSDO();
	transferByte_U08(0x00, 0x4C);
	pollSDO();
}

static void writeFuseExtendedBits(tU08 code_U08)
{
	transferByte_U08(0x40, 0x4C);
	transferByte_U08(code_U08, 0x2C);
	transferByte_U08(0x00, 0x66);
	transferByte_U08(0x00, 0x6E);
	pollSDO();
	transferByte_U08(0x00, 0x4C);
	pollSDO();
}

static tU08 transferByte_U08(tU08 data_U08, tU08 instruction_U08)
{
	tU08 i_U08;
	tU08 byteRead_U08 = 0;

	/* first bit is zero */
	SDI_LO;
	SII_LO;
	clockit();

	for (i_U08 = 0; i_U08 < 8; i_U08++)
	{
		/* read one bit form SDO */
		byteRead_U08 <<= 1;   // shift one bit, this clears LSB
		if (GET_STATUS(COIO_SDO_CFG) != 0)
		{
			byteRead_U08 |= 1;
		}

		/* output next data bit on SDI */
		if (data_U08 & 0x80)
		{
			SDI_HI;
		}
		else
		{
			SDI_LO;
		}

		/* output next instruction bit on SII */
		if (instruction_U08 & 0x80)
		{
			SII_HI;
		}
		else
		{
			SII_LO;
		}

		clockit();

		/* prepare for processing next bit */
		data_U08 <<= 1;
		instruction_U08 <<= 1;
	}

	/* Last two bits are zero */
	SDI_LO;
	SII_LO;
	clockit();
	clockit();

	return byteRead_U08;

}

/*
 * The minimum period for the Serial Clock Input (SCI) during High-voltage Serial Programming is 220 ns.
 */
static inline void clockit(void)
{
	SCI_HI;
	_delay_us(1);
	SCI_LO;
}

static void pollSDO(void)
{
	// wait until SDO goes high
	while (0 == GET_STATUS(COIO_SDO_CFG))
	{
	}
}

static tB updateSignatureName_B(tU08 * signature_pU08)
{
	tB ret_B = FALSE;
	const tTargetCpuInfo_str * target_pstr = findDevice_pstr(signature_pU08);

	if (target_pstr != 0)
	{
		memcpy_P(&targetDevice_str.name_aC, &target_pstr->name_aC,
				sizeof(targetDevice_str.name_aC));

		ret_B = TRUE;
	}
	return ret_B;
}

void Coio_getDeviceName_pC(char text_pC[])
{
	tU08 i_U08;
	for (i_U08 = 0; i_U08 < 4; i_U08++)
	{
		text_pC[i_U08] = targetDevice_str.name_aC[i_U08];
	}
}

tU08 Coio_getLowFuse_U08(void)
{
	return targetDevice_str.fuseLowBits_U08;
}

tU08 Coio_getHighFuse_U08(void)
{
	return targetDevice_str.fuseHighBits_U08;
}

tU08 Coio_getExtendedFuse_U08(void)
{
	return targetDevice_str.fuseExtendedBits_U08;
}

static const tTargetCpuInfo_str * findDevice_pstr(tU08 * signature_pU08)
{
	const tTargetCpuInfo_str * device_pstr = 0;

	tU08 device_U08;

	for (device_U08 = 0; device_U08 < NOF_DEVICES; device_U08++)
	{
		if ((signature_pU08[0]
				== pgm_read_byte(
						&deviceDefault_astr[device_U08].signature_aU08[0]))
				&& (signature_pU08[1]
						== pgm_read_byte(
								&deviceDefault_astr[device_U08].signature_aU08[1]))
				&& (signature_pU08[2]
						== pgm_read_byte(
								&deviceDefault_astr[device_U08].signature_aU08[2])))
		{
			device_pstr = &deviceDefault_astr[device_U08];
		}
	}
	return device_pstr;
}
