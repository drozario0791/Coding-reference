/*
** ###################################################################
**     Filename  : User_Safe_Mode.c
**     Processor : Not applicable
**     FileFormat: V2.00
**     Abstract  :
**          This is where the user can add code for when in safe mode
**          This function gets called once a loop when in safe mode
**
**     (c) Copyright HED,Inc. 2006-2016
**     HED,Inc.
**     2120 Constitution Ave.
**     Hartford, WI 53027
**     United States
**     http      : www.hedonline.com
**     mail      : support@hedonline.com
** ###################################################################
*/

/*Include Header Files */
#include "reserved.h"
#include "Prototypes.h"
#include "Constants.h"
#include "User_App.h"
#include "User_Safe_Mode.h"

/*
 * function prototype for local function to control the heartbeat
 */
static void heartbeatControl(void);

/*
 * This function will be called every loop instead of User_App if:
 *
 *    1. The Battery drops below threshold configured in Orchestra if
 *    enabled (not all modules support)
 *
 *    2. When Conductor debug is active and communication is lost or
 *    the Conductor E-Stop is pressed
 *
 * Prior to calling this fucntion, the firmware sets all outputs to
 * Orchestra configured safe mode, this function allows the ability to
 * over ride that functionality
 */
void User_Safe_Mode(void)
{
   heartbeatControl();

	/* Insert your application code here */
#warning "Insert your safe mode code here, remove this warning after you have set it up"
}

/*
 * This is an example function of how to control the heartbeat This is
 * also an example of a static local function, by putting static in
 * front of it no other c file can call it, also allows for the reuse
 * of the function name in other files
 */
static void heartbeatControl(void)
{
	USER_RAM
	static uint16_t  heartbeat_count = 0; /* example of static local variable */
	DEFAULT_RAM

	if(heartbeat_count < get_ONE_HUNDRED_MSECS())
	{
		Heart_Beat_Control(TRUE);
		heartbeat_count++;
	}
	else if(heartbeat_count < get_TWO_HUNDRED_MSECS())
	{
		Heart_Beat_Control(FALSE);
		heartbeat_count++;
	}
	else
	{
		/* Skip*/
	}
	if(heartbeat_count >= get_TWO_HUNDRED_MSECS())
	{
		heartbeat_count = 0;
	}
	else
	{
		/* Skip*/
	}
}
