/*
 * utils.c
 *
 *  Created on: 31.05.2012
 *      Author: Peter Schuster
 */

#include "utils.h"
#include "xparameters.h"
#include "xbasic_types.h"
#include "xtmrctr.h"

XTmrCtr DelayTimer;

void init_utils() {

	// Initialize the timer
	XTmrCtr_Initialize(&DelayTimer, XPAR_XPS_TIMER_0_DEVICE_ID);
	XTmrCtr_SetOptions(&DelayTimer, 1, XTC_DOWN_COUNT_OPTION);
}

// Delay function (microseconds)
void delay_us(Xuint32 time)
{
  XTmrCtr_SetResetValue(&DelayTimer, 1, time * 125);
  XTmrCtr_Start(&DelayTimer, 1);
  while(!(XTmrCtr_IsExpired(&DelayTimer, 1))){}
  XTmrCtr_Stop(&DelayTimer, 1);
}

// Delay function (milliseconds)
void delay_ms(Xuint32 time)
{
  XTmrCtr_SetResetValue(&DelayTimer, 1, time * 125000);
  XTmrCtr_Start(&DelayTimer, 1);
  while(!(XTmrCtr_IsExpired(&DelayTimer, 1))){}
  XTmrCtr_Stop(&DelayTimer, 1);
}
