/******************************************************************************
 *
 *        Name: timer_service.c
 *
 * Description: A module with function calls to initialize timers and
 *              operate timers.
 *
 *      Author: Cyrus
 *        Date: Friday, 05 July 2019
 *
 ******************************************************************************
 */

#include <stdlib.h>
#include "Prototypes.h"
#include "Constants.h"
#include "User_Can_Receive.h"
#include "timer_service.h"
#include "Prototypes_Time.h"

/******************************************************************************
 *
 *        Name: timer_setup()
 *
 * Description: This function initializes a timer with a type and a
 *              period that can be used elsewhere in the program It
 *              creates a timer, gives it a period, initializes the
 *              counter, and returns it.
 *
 *      Author: Cyrus
 *        Date: Friday, 05 July 2019
 *
 ******************************************************************************
 */
timer_t timer_setup (uint32_t period, timer_type_t timer_type)
{
    timer_t tp;

    tp.period = period;
    tp.type   = timer_type;

    // tp.preVal = FALSE;
    if (timer_type == PULSE)
    {
		// pulse timers work by checking if the value is between
		// extremes (0 or period).
        tp.period = period + 1;

        // since 0 and period will return false, and between them will
        // return true, one cycle should be added to the pulse time
    }

    if (timer_type == RISING)
    {
        tp.counter = period;
    }
    else
    {
        tp.counter = 0;
    }

    return tp;
}

/******************************************************************************
 *
 *        Name: timer_operate()
 *
 * Description: There are three timer types: Rising, Falling, and
 *              Pulse.  Here are explanations on how they function.
 *              Each timer takes a boolean input, and then based on
 *              the duration and type, generates a boolean output. The
 *              relationship between input and output is dependant on
 *              the timer type.
 *
 *              RISING : A rising timer delays the response of an
 *                       input going true.  If the input goes true,
 *                       the output will go true after the timer runs
 *                       out.  If the input goes false, the output
 *                       will go false right away and the timer will
 *                       be reset.
 *
 *              FALLING: A falling timer delays the response of an
 *                       input going false.  If the input goes false,
 *                       the output will go false after the timer runs
 *                       out.  If the input goes true, the output will
 *                       go true right away and the timer will be
 *                       reset.
 *
 *              PULSE: A pulse timer returns true for a brief period
 *                     if the input goes true.  When the input goes
 *                     true, the output will stay true until the timer
 *                     runs out.  If the input goes false while the
 *                     output is true, it will stay true until the
 *                     timer runs out.  The output will stay true no
 *                     matter what the input is doing if the timer has
 *                     not expired.  Once the timer expires, the
 *                     output will stay false until the input goes
 *                     from false to true again.
 *
 *
 *      Author: Cyrus
 *        Date: Friday, 05 July 2019
 *
 ******************************************************************************
 */
bool_t timer_operate(timer_t *	timer,
					 bool_t			input)
{
	// begin control for timers of rising type
	if (timer->type == RISING)
	{
		if (input)
		{
			// if timer hasn't run out yet, decrement
			if (timer->counter != 0)
			{
				timer->counter--;
			}
		}
		else											// input is false, reset timer
		{
			timer->counter	= timer->period;			// reset the timer
		}
	}

	// begin control for timers of falling type
	else if (timer->type == FALLING)
	{
		if ( !input)
		{
			// if timer hasn't run out yet, decrement
			if (timer->counter != 0)
			{
				timer->counter--;
			}
		}

		// input is true, reset the timer
		else
		{
			timer->counter	= timer->period;			// reset the timer
		}
	}

	// begin control for timers of pulse type
	else if (timer->type == PULSE)
	{
		if (input && (timer->counter == timer->period))
		{												// if input is true and the timer is set
			timer->counter--;							// decrease the value by one to signal that the timer should run
		}
		else if ((timer->counter != timer->period) && (timer->counter != 0))
		{												// if the timer is running
			timer->counter--;							// continue to run it
		}
		else if ((timer->counter == 0) && !input)		// if the timer is done and the input is false
		{
			timer->counter	= timer->period;			// reset the timer
		}
		else
		{
			// do nothing
		}
	}
	else												// timer is of unknown type
	{
		// do nothing
	}

	return (((timer->type == RISING) && (input && (timer->counter == 0)))
			||		// rising types are true if the input is true and the timer has reached zero
			((timer->type == FALLING) && (input || (timer->counter != 0)))
			||		// falling types are true if the input is true or if the counter has not reached zero
			((timer->type == PULSE) && (timer->counter != 0) && (timer->counter != timer->period)));
	                // pulse types are true if the timer is between extremes (running)
}
