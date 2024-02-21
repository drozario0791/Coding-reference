/*
 ** ###################################################################
 **     Filename  : User_Low_Power.c
 **     Processor : Not applicable
 **     FileFormat: V2.00
 **     Abstract  :
 **          This is where the user can control when the module enters low power
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

//
// Include header files
//
#include "reserved.h"
#include "Prototypes.h"
#include "Constants.h"
#include "User_App.h"
#ifdef PROTOTYPES_LOW_POWER_HEADER_FILE
#include "User_Low_Power.h"

#include "cvc_power_sequencing.h"

extern bool_t low_power_mode;

/******************************************************************************
 *
 *        Name: User_Low_Power_Init()
 *
 * Description: Called at start up by the firmware to allow the user
 *              to initialize what wakes up the module. The
 *              Set_Wakeup_Modes can be called at any point from any
 *              of the user files to configure the wakeup modes. This
 *              function call is here for convenience; by default all
 *              options are enabled.
 *
 *      Author: Tom
 *        Date: Wednesday, 21 August 2019
 *
 ******************************************************************************
 */
void User_Low_Power_Init(void)
{

//
// Call "Set_Wakeup_Modes" function to set the desired wake-up
// mode(s).
//
// These can be OR'd together for multiple wake-up modes, e.g.
// Set_Wakeup_Modes((wakeup_modes_t)(WAKEUP_PIN|WAKEUP_CAN1|WAKEUP_BUTTON1))
//
    cvc_power_sequencing_init_low_power();
}


/******************************************************************************
 *
 *        Name: User_Low_Power()
 *
 * Description: Called once a loop by the firmware so the user can
 *              determine when the module enters low power.
 *
 *              Return FALSE to keep running, TRUE to enter low power.
 *
 *        NOTE: If one of the wake up options is active when the
 *        ----> function returns TRUE the module will reset instead of
 *              going into low power mode.
 *
 *
 *      Author: Tom
 *        Date: Wednesday, 21 August 2019
 *
 ******************************************************************************
 */
bool_t User_Low_Power(void)
{
    bool_t enter_low_power = FALSE;
    //
    // TODO: Add your code here for choosing when to enter the low power state.
    //
    //       e.g. Monitor the wakeup pin and when it goes to INPUT_OFF,
    //       save your data and return TRUE.
    //
    //       Note: When you return TRUE the module with enter low power
    //             mode without calling any additional user functions
    //             until it is woken up.  Wakeup will will act like a
    //             power cycle (ie will run through all the initilization
    //             code again)
    //
    // enter_low_power = cvc_power_sequencing_low_power_control();
    return low_power_mode;
    //  return FALSE;
}

#endif
