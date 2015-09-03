/*
 * conf.h
 *
 *  Created on: 11 jul 2015
 *      Author: andreas
 */
#include <avr/io.h>
#include "type.h"

#ifndef CONF_H_
#define CONF_H_

#define TICK 20

#define DEBUG_MODE 1

#define DISP_INTENSITY 4 /* Display intensity [0-15] */

/*************************************************
 * PIN CONFIGURATION
 *************************************************/
#define BUTT_SELECT_CFG   B,3
#define BUTT_RESET_CFG    D,0

#define BUZZ_CFG		  B,6

#define DISP_SCK_CFG      B,7
#define DISP_DATA_CFG     B,5
#define DISP_CS_CFG       B,4

#define COIO_VCC_CFG      D,5
#define COIO_RESET_CFG    A,0
#define COIO_SCI_CFG      D,4
#define COIO_SDI_CFG	  D,2
#define COIO_SII_CFG	  A,1
#define COIO_SDO_CFG	  D,3

#define LEDC_DEVICE_CFG   B,2
#define LEDC_LO_FUSE_CFG  B,1
#define LEDC_HI_FUSE_CFG  B,0
#define LEDC_EXT_FUSE_CFG D,6

#endif /* CONF_H_ */
