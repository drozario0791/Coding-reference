/******************************************************************************
 *
 *        Name: gears_and_transmission.c
 *
 * Description: A module that controls the transmission on the carrier
 *              based on the gear input.
 *
 *      Author: Deepak
 *        Date: Monday, 26 August 2019
 *
 ******************************************************************************
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "reserved.h"
#include "Prototypes.h"
#include "Constants.h"
#include "User_App.h"
#include "typedefs.h"
#include "can_service.h"
#include "cvc_input_control.h"
#include "pdm_control_1.h"
#include "pdm_control_2.h"
#include "pdm_control_3.h"
#include "gears_and_transmission.h"
#include "cl712_device.h"
#include "fvt_library.h"
#include "state_machine.h"
#include "timer_service.h"
#include "hydraulic_system.h"

//
// main_system_pressure_sensor minimum threshold
//
#define TRANSMISSION_SYSTEM_PRESSURE_SENSOR_MIN_THRESHOLD   200
#define EEVAR_MAXIMUM_ALLOWABLE_GEARS               2

//
// Function Prototypes
//
static shifter_position_t direction_control();
static shifter_position_t gear_control();
//
// static global variables
//
static shifter_position_t direction;
static shifter_position_t gear_position;


/******************************************************************************
 *
 *        Name: gears_and_transmission_control()
 *
 * Description:
 *
 *        Date: Monday, 26 August 2019
 *
 ******************************************************************************
 */
void gears_and_transmission_control()
{
    uint16_t transmission_system_pressure_sensor = 0;

    //
    // Check to see if the shifter has been moved to select a
    // direction of motion. Reads the output of
    //
    direction = direction_control();


    //
    // If the shifter is not in neutral. This also ensures that the
    // transmission pressure is greater than the minimum set limit.
    //
    if (direction != SHIFTER_NEUTRAL)
    {

        //
        // The gear solenoids can only be enabled if the
        // transmission_system_pressure_sensor registers a PSI greater than
        // the minimum pressure value.
        //
        //
        // Reads the output of the shifter and enables the appropriate
        // transmission solenoids.
        //
        gear_position = gear_control();

    }

}


/******************************************************************************
 *
 *        Name:
 *
 * Description: A function that is used to determine the direction of
 *              motion of the carrier based on the position of the
 *              shifter. The shifter has three position's, forward,
 *              reverse and neutral. Enable the forward or reverse
 *              solenoid depending on the position of the shifter.
 *
 *
 *      Author: Deepak
 *        Date: Wednesday, 28 August 2019
 *
 ******************************************************************************
 */
shifter_position_t direction_control()
{
    shifter_position_t shifter_position = SHIFTER_NEUTRAL;
    bool_t shifter_forward = FALSE;
    bool_t shifter_reverse = FALSE;
    bool_t shifter_neutral = FALSE;

    //
    // Get the status of the corresponding inputs.
    //
    shifter_forward =  (bool_t)IN_B15_forward_switch_status();
    shifter_reverse =  (bool_t)IN_C18_reverse_switch_status();

    //
    // In this case, due to a wiring issue we cannot detect the
    // neutral.  The signal at the input of the CVC always stays at
    // 0V. Therefore, check the other two positions to detect if the
    // vehicle is in neutral.
    //
    shifter_neutral =  ((shifter_forward || shifter_reverse) == 0);

    //
    // Enable the respective solenoid based on input signal.
    //
    if (shifter_forward)
    {

        //
        // Enable the forward gear solenoid
        //
        Update_Output(OUT_C01_forward_gear_solenoid, OUTPUT_ON);

        //
        // Done to ensure the reverse gear solenoid is disabled.
        //
        Update_Output(OUT_C02_reverse_gear_solenoid, OUTPUT_OFF);

        shifter_position = SHIFTER_FORWARD;

    }
    else if (shifter_reverse)
    {

        //
        // Enable the reverse gear solenoid
        //
        Update_Output(OUT_C02_reverse_gear_solenoid, OUTPUT_ON);

        //
        // Done to ensure the forward gear solenoid is disabled.
        //
        Update_Output(OUT_C01_forward_gear_solenoid, OUTPUT_OFF);

        shifter_position = SHIFTER_REVERSE;

    }
    else
    {

        //
        // Disable all solenoids
        //
        Update_Output(OUT_C01_forward_gear_solenoid, OUTPUT_OFF);
        Update_Output(OUT_C02_reverse_gear_solenoid, OUTPUT_OFF);
        pdm_2_command_1st_gear_solenoid_pwr(FALSE);
        pdm_2_command_2nd_gear_solenoid_pwr(FALSE);
        pdm_2_command_3rd_gear_solenoid_pwr(FALSE);

        shifter_position = SHIFTER_NEUTRAL;

    }

    return shifter_position;

}


//=============================================================================
//
// gear_control()
//
//=============================================================================
//
shifter_position_t gear_control()
{

    bool_t first_gear = FALSE;
    bool_t second_gear = FALSE;
    bool_t third_gear = FALSE;
    shifter_position_t shifter_position = SHIFTER_NEUTRAL;

    //
    // Read the output of the gear_shifter on the carrier.  The
    // output of the shifter is connected to few inputs on the CVC.
    //

    //
    // In this case, enable only the first gear solenoid and the
    // second gear solenoid. Force all the gear signals from the
    // rest of the shifter to enable the second gear solenoid.
    //
    if ((IN_B17_2nd_gear_switch_status() == FALSE) && (IN_B16_1st_gear_switch_status() == FALSE) && (IN_B18_3rd_gear_switch_status() == FALSE))    	
    {

        //
        // First Gear
        //
        pdm_2_command_1st_gear_solenoid_pwr(FALSE);
        pdm_2_command_2nd_gear_solenoid_pwr(FALSE);
        pdm_2_command_3rd_gear_solenoid_pwr(FALSE);

        shifter_position = SHIFTER_1st_GEAR;

    }
    else if ((IN_B17_2nd_gear_switch_status() == TRUE) && (IN_B16_1st_gear_switch_status() == FALSE) && (IN_B18_3rd_gear_switch_status() == FALSE))    	
    {

        //
        // Second Gear
        //
        pdm_2_command_1st_gear_solenoid_pwr(TRUE);
        pdm_2_command_2nd_gear_solenoid_pwr(FALSE);
        pdm_2_command_3rd_gear_solenoid_pwr(FALSE);

        shifter_position = SHIFTER_2nd_GEAR;

    }
    else if ((IN_B17_2nd_gear_switch_status() == TRUE) && (IN_B18_3rd_gear_switch_status() == TRUE))    	

    {

        //
        // Third Gear
        //
        pdm_2_command_1st_gear_solenoid_pwr(TRUE);
        pdm_2_command_2nd_gear_solenoid_pwr(TRUE);
        pdm_2_command_3rd_gear_solenoid_pwr(FALSE);

        shifter_position = SHIFTER_3rd_GEAR;

    }
    else
    {

        //
        // Force all outputs to zero if in neutral
        //
        pdm_2_command_1st_gear_solenoid_pwr(TRUE);
        pdm_2_command_2nd_gear_solenoid_pwr(FALSE);
        pdm_2_command_3rd_gear_solenoid_pwr(FALSE);

        shifter_position = SHIFTER_2nd_GEAR;

    }


    return shifter_position;

}


//=============================================================================
//
// get_shifter_direction();
//
//=============================================================================
//
shifter_position_t get_shifter_direction()
{

    return direction;

}

//=============================================================================
//
// get_shifter_gear_position();
//
//=============================================================================
//
shifter_position_t get_shifter_gear_position()
{

    return gear_position;

}
