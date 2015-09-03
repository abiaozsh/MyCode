/*
 * coio.h
 *
 *  Created on: 17 jul 2015
 *      Author: andreas
 */

#ifndef COIO_H_
#define COIO_H_

#include "type.h"

typedef enum
{
	COIO_OK_E, COIO_UNKNOWN_DEVICE_E, COIO_CONNECTION_FAIL_E
} tCoio_ReadResult_E;

void Coio_init(void);
tCoio_ReadResult_E Coio_readChip_E(void);
void Coio_getDeviceName_pC(char * text_pC);
tU08 Coio_getLowFuse_U08(void);
tU08 Coio_getHighFuse_U08(void);
tU08 Coio_getExtendedFuse_U08(void);
tB Coio_resetChip_B(void);
#endif /* COIO_H_ */
