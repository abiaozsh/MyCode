/*
 * ledc.h
 *
 *  Created on: 18 jul 2015
 *      Author: andreas
 */

#ifndef LEDC_H_
#define LEDC_H_

typedef enum
{
	LEDC_NONE_E, LEDC_DEVICE_E, LEDC_LO_FUSE_E, LEDC_HI_FUSE_E, LEDC_EXT_FUSE_E
}tLedc_LightState_E;

void Ledc_init(void);
void Ledc_set(tLedc_LightState_E lightState_E);

#endif /* LEDC_H_ */
