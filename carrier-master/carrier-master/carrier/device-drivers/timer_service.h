/******************************************************************************
 *
 *        Name: timer_service.h
 *
 * Description:
 *
 *      Author: Cyrus
 *        Date: Friday, 05 July 2019
 *
 ******************************************************************************
 */

#include <stdlib.h>

typedef enum
{
    RISING  = 0,
    FALLING = 1,
    PULSE   = 2
} timer_type_t;

//
// Period = number of cycles timer runs for.
//
// Counter = value to keep track of how long the timer has been
// running for.
//
// Type = how the timer behaves: RISING, FALLING, PULSE.
//
typedef struct
{
    uint32_t     period;
    uint32_t     counter;
    timer_type_t type;
} timer_t;

//
// This function returns the initialization for the timer_t
// structure. The period is the number of (User_loop() time period)
// cycles that the timer should delay for. There are three possible
// timer types.
//
//  RISING - The rising timer type delays the turning on of a
//           boolean. The output goes high once input has gone high
//           for the time period specified. If input is low, output is
//           low.
//
// FALLING - The falling timer type is the exact opposite of
//           Rising. If input is high, output is high. Once input goes
//           low, output will go low after the time period specified.
//
//   PULSE - The pulse timer type generates a pulsed output. If the
//           input goes high, the output will go high for the time
//           period specified and then turn off. The input must go low
//           then high again to generate another pulse.
//
timer_t timer_setup(uint32_t period, timer_type_t timer_type);

//
// Timer_Operate should be called each loop. Will return true or
// false, based on inBit and the type of the timer passed.
//
bool_t timer_operate(timer_t *timer, bool_t inBit);
