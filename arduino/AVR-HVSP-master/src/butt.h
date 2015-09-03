/*
 * butt.h
 *
 *  Created on: 13 jul 2015
 *      Author: andreas
 */

#ifndef BUTT_H_
#define BUTT_H_

#include "type.h"

typedef enum
{
	BUTT_SELECT_E, BUTT_RESET_E
} tButt_Type_E;

typedef enum
{
    BUTT_RELEASED_E, BUTT_PRESSED_E
}tButt_State_E;

typedef struct
{
    tButt_State_E state_E;
    tU08 tickInState_U08;
} tButt_State_str;

void Butt_init(void);
void Butt_update(void);
tB Butt_pressedFlank_B(tButt_Type_E type_E);
tB Butt_pressed_B(tButt_Type_E type_E);
tU08 Butt_getTickInState_U08(tButt_Type_E type_E);

#endif /* BUTT_H_ */
