/*
 * ledc.c
 *
 *  Created on: 18 jul 2015
 *      Author: andreas
 */
#include "conf.h"
#include "ledc.h"

void Ledc_init(void)
{
	CONF_IO(LEDC_DEVICE_CFG, OUTPUT, 0);
	CONF_IO(LEDC_LO_FUSE_CFG, OUTPUT, 0);
	CONF_IO(LEDC_HI_FUSE_CFG, OUTPUT, 0);
	CONF_IO(LEDC_EXT_FUSE_CFG, OUTPUT, 0);
}

void Ledc_set(tLedc_LightState_E lightState_E)
{
	IO_CLR(LEDC_DEVICE_CFG);
	IO_CLR(LEDC_LO_FUSE_CFG);
	IO_CLR(LEDC_HI_FUSE_CFG);
	IO_CLR(LEDC_EXT_FUSE_CFG);
	switch(lightState_E)
	{
	case LEDC_DEVICE_E:
		IO_SET(LEDC_DEVICE_CFG);
		break;
	case LEDC_LO_FUSE_E:
		IO_SET(LEDC_LO_FUSE_CFG);
		break;
	case LEDC_HI_FUSE_E:
		IO_SET(LEDC_HI_FUSE_CFG);
		break;
	case LEDC_EXT_FUSE_E:
		IO_SET(LEDC_EXT_FUSE_CFG);
		break;
	default:
		break;
	}

}
