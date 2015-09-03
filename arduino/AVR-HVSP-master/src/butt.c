/*
 * butt.c
 *
 *  Created on: 13 jul 2015
 *      Author: andreas
 */
#include <avr/interrupt.h>
#include <avr/io.h>

#include "butt.h"
#include "conf.h"
#include "ledc.h"
#include "type.h"

#define NOF_BUTTONS 2

#define FILTER_TIME 60 //[ms]

static tButt_State_str buttState_astr[NOF_BUTTONS];

void Butt_init(void)
{
	/* Activate internal pull-up */
	CONF_IO(BUTT_SELECT_CFG, INPUT, PULLUP);
	CONF_IO(BUTT_RESET_CFG, INPUT, PULLUP);

}

void Butt_update(void)
{

	static tButt_State_str buttonRawOld_astr[NOF_BUTTONS];

	tButt_State_E buttonRaw_aE[NOF_BUTTONS] =
	{ (GET_STATUS(BUTT_SELECT_CFG) ? BUTT_RELEASED_E : BUTT_PRESSED_E), (
	GET_STATUS(BUTT_RESET_CFG) ? BUTT_RELEASED_E : BUTT_PRESSED_E) };
	tU08 i_U08;
	for (i_U08 = 0; i_U08 < NOF_BUTTONS; i_U08++)
	{
		/* Is raw button glitching */
		if (buttonRawOld_astr[i_U08].state_E == buttonRaw_aE[i_U08])
		{
			INC_U08(buttonRawOld_astr[i_U08].tickInState_U08);
		}
		else
		{
			buttonRawOld_astr[i_U08].tickInState_U08 = 0;
		}

		/* Check if raw value is stable and different. */
		if ((buttonRawOld_astr[i_U08].tickInState_U08 >= FILTER_TIME / TICK)
				&& (buttState_astr[i_U08].state_E != buttonRaw_aE[i_U08]))
		{
			buttState_astr[i_U08].state_E = buttonRaw_aE[i_U08];
			buttState_astr[i_U08].tickInState_U08 = 0;
		}
		else
		{
			INC_U08(buttState_astr[i_U08].tickInState_U08);
		}
		buttonRawOld_astr[i_U08].state_E = buttonRaw_aE[i_U08];
	}
}

tB Butt_pressedFlank_B(tButt_Type_E type_E)
{
	tB ret_B = FALSE;
	if ((buttState_astr[type_E].state_E == BUTT_PRESSED_E)
			&& (buttState_astr[type_E].tickInState_U08 == 0))
	{
		ret_B = TRUE;
	}

	return ret_B;
}

tB Butt_pressed_B(tButt_Type_E type_E)
{
	tB ret_B = FALSE;
	if (buttState_astr[type_E].state_E == BUTT_PRESSED_E)
	{
		ret_B = TRUE;
	}
	return ret_B;
}

tU08 Butt_getTickInState_U08(tButt_Type_E type_E)
{
	return buttState_astr[type_E].tickInState_U08;
}
