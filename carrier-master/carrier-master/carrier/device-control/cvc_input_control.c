/******************************************************************************
 *
 *        Name: cvc_input_control.c
 *
 * Description: There are inputs and outputs present on a CVC. This
 *              module is dedicated to read the input values of the
 *              CVC and return the status of the input pins. These
 *              values are used by various vehicle control functions
 *              to perform the desired actions.
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
#include "pdm_control_2.h"
#include "orion_control.h"

//=============================================================================
//
// IN_A15_stop_charging_button_signal_status()
//
//=============================================================================
//
Input_State_t IN_A15_stop_charging_button_signal_status()
{
    Input_State_t status;

    //
    // Check the state of pin E01 and return the status of the PIN.
    // True indicates the presence of 12V at the PIN. False indicates
    // 0V at the PIN.
    //
    if (IN_A15_stop_charging_button_signal == INPUT_ON)
    {

        // The digital input is on
        status = INPUT_ON;

    }
    else if (IN_A15_stop_charging_button_signal == INPUT_OFF)
    {

        // The digital input is off
        status = INPUT_OFF;

    }
    else if (IN_A15_stop_charging_button_signal == INPUT_DISABLED)
    {

        // The digital input is either not configured, its debounce
        // time not expired on boot up, or the i/o module it is
        // assigned to is missing
        status = INPUT_DISABLED;

    }

    return status;

}


//=============================================================================
//
// IN_A16_dash_e_stop_signal_status()
//
//=============================================================================
//
Input_State_t IN_A16_dash_e_stop_signal_status()
{
    Input_State_t status;

    //
    // Check the state of pin E01 and return the status of the PIN.
    // True indicates the presence of 12V at the PIN. False indicates
    // 0V at the PIN.
    //
    if (IN_A16_dash_e_stop_signal == INPUT_ON)
    {

        // The digital input is on
        status = INPUT_ON;

    }
    else if (IN_A16_dash_e_stop_signal == INPUT_OFF)
    {

        // The digital input is off
        status = INPUT_OFF;

    }
    else if (IN_A16_dash_e_stop_signal == INPUT_DISABLED)
    {

        // The digital input is either not configured, its debounce
        // time not expired on boot up, or the i/o module it is
        // assigned to is missing
        status = INPUT_DISABLED;

    }

    return status;

}


//=============================================================================
//
// IN_A17_charge_box_e_stop_signal_status()
//
//=============================================================================
//
Input_State_t IN_A17_charge_box_e_stop_signal_status()
{
    Input_State_t status;

    //
    // Check the state of pin E01 and return the status of the PIN.
    // True indicates the presence of 12V at the PIN. False indicates
    // 0V at the PIN.
    //
    if (IN_A17_charge_box_e_stop_signal == INPUT_ON)
    {

        // The digital input is on
        status = INPUT_ON;

    }
    else if (IN_A17_charge_box_e_stop_signal == INPUT_OFF)
    {

        // The digital input is off
        status = INPUT_OFF;

    }
    else if (IN_A17_charge_box_e_stop_signal == INPUT_DISABLED)
    {

        // The digital input is either not configured, its debounce
        // time not expired on boot up, or the i/o module it is
        // assigned to is missing
        status = INPUT_DISABLED;

    }

    return status;

}


//=============================================================================
//
// IN_A18_diagnositic_switch_signal_status()
//
//=============================================================================
//
Input_State_t IN_A18_diagnositic_switch_signal_status()
{
    Input_State_t status;

    //
    // Check the state of pin E01 and return the status of the PIN.
    // True indicates the presence of 12V at the PIN. False indicates
    // 0V at the PIN.
    //
    if (IN_A18_diagnositic_switch_signal == INPUT_ON)
    {

        // The digital input is on
        status = INPUT_ON;

    }
    else if (IN_A18_diagnositic_switch_signal == INPUT_OFF)
    {

        // The digital input is off
        status = INPUT_OFF;

    }
    else if (IN_A18_diagnositic_switch_signal == INPUT_DISABLED)
    {

        // The digital input is either not configured, its debounce
        // time not expired on boot up, or the i/o module it is
        // assigned to is missing
        status = INPUT_DISABLED;

    }

    return status;

}


//=============================================================================
//
// IN_B15_forward_switch_status()
//
//=============================================================================
//
Input_State_t IN_B15_forward_switch_status()
{
    Input_State_t status;

    //
    // Check the state of pin E01 and return the status of the PIN.
    // True indicates the presence of 12V at the PIN. False indicates
    // 0V at the PIN.
    //
    if (IN_B15_forward_switch == INPUT_ON)
    {

        // The digital input is on
        status = INPUT_ON;

    }
    else if (IN_B15_forward_switch == INPUT_OFF)
    {

        // The digital input is offxd
        status = INPUT_OFF;

    }
    else if (IN_B15_forward_switch == INPUT_DISABLED)
    {

        // The digital input is either not configured, its debounce
        // time not expired on boot up, or the i/o module it is
        // assigned to is missing
        status = INPUT_DISABLED;

    }

    return status;

}


//=============================================================================
//
// IN_B16_1st_gear_switch_status()
//
//=============================================================================
//
Input_State_t IN_B16_1st_gear_switch_status()
{
    Input_State_t status;

    //
    // Check the state of pin E01 and return the status of the PIN.
    // True indicates the presence of 12V at the PIN. False indicates
    // 0V at the PIN.
    //
    if (IN_B16_1st_gear_switch == INPUT_ON)
    {

        // The digital input is on
        status = INPUT_ON;

    }
    else if (IN_B16_1st_gear_switch == INPUT_OFF)
    {

        // The digital input is off
        status = INPUT_OFF;

    }
    else if (IN_B16_1st_gear_switch == INPUT_DISABLED)
    {

        // The digital input is either not configured, its debounce
        // time not expired on boot up, or the i/o module it is
        // assigned to is missing
        status = INPUT_DISABLED;

    }

    return status;

}


//=============================================================================
//
// IN_B17_2nd_gear_switch_status()
//
//=============================================================================
//
Input_State_t IN_B17_2nd_gear_switch_status()
{
    Input_State_t status;

    //
    // Check the state of pin E01 and return the status of the PIN.
    // True indicates the presence of 12V at the PIN. False indicates
    // 0V at the PIN.
    //
    if (IN_B17_2nd_gear_switch == INPUT_ON)
    {

        // The digital input is on
        status = INPUT_ON;

    }
    else if (IN_B17_2nd_gear_switch == INPUT_OFF)
    {

        // The digital input is off
        status = INPUT_OFF;

    }
    else if (IN_B17_2nd_gear_switch == INPUT_DISABLED)
    {

        // The digital input is either not configured, its debounce
        // time not expired on boot up, or the i/o module it is
        // assigned to is missing
        status = INPUT_DISABLED;

    }

    return status;

}


//=============================================================================
//
// IN_B18_3rd_gear_switch_status()
//
//=============================================================================
//
Input_State_t IN_B18_3rd_gear_switch_status()
{
    Input_State_t status;

    //
    // Check the state of pin E01 and return the status of the PIN.
    // True indicates the presence of 12V at the PIN. False indicates
    // 0V at the PIN.
    //
    if (IN_B18_3rd_gear_switch == INPUT_ON)
    {

        // The digital input is on
        status = INPUT_ON;

    }
    else if (IN_B18_3rd_gear_switch == INPUT_OFF)
    {

        // The digital input is off
        status = INPUT_OFF;

    }
    else if (IN_B18_3rd_gear_switch == INPUT_DISABLED)
    {

        // The digital input is either not configured, its debounce
        // time not expired on boot up, or the i/o module it is
        // assigned to is missing
        status = INPUT_DISABLED;

    }

    return status;

}


//=============================================================================
//
// IN_C11_neutral_switch_status()
//
//=============================================================================
//
Input_State_t IN_C11_neutral_switch_status()
{
    Input_State_t status;

    //
    // Check the state of pin E01 and return the status of the PIN.
    // True indicates the presence of 12V at the PIN. False indicates
    // 0V at the PIN.
    //
    if (IN_C11_neutral_switch == INPUT_ON)
    {

        // The digital input is on
        status = INPUT_ON;

    }
    else if (IN_C11_neutral_switch == INPUT_OFF)
    {

        // The digital input is off
        status = INPUT_OFF;

    }
    else if (IN_C11_neutral_switch == INPUT_DISABLED)
    {

        // The digital input is either not configured, its debounce
        // time not expired on boot up, or the i/o module it is
        // assigned to is missing
        status = INPUT_DISABLED;

    }

    return status;

}


//=============================================================================
//
// IN_C15_aba_brake_button_signal_status()
//
//=============================================================================
//
Input_State_t IN_C15_aba_brake_button_signal_status()
{
    Input_State_t status;

    //
    // Check the state of pin E01 and return the status of the PIN.
    // True indicates the presence of 12V at the PIN. False indicates
    // 0V at the PIN.
    //
    if (IN_C15_aba_brake_button_signal == INPUT_ON)
    {

        // The digital input is on
        status = INPUT_ON;

    }
    else if (IN_C15_aba_brake_button_signal == INPUT_OFF)
    {

        // The digital input is off
        status = INPUT_OFF;

    }
    else if (IN_C15_aba_brake_button_signal == INPUT_DISABLED)
    {

        // The digital input is either not configured, its debounce
        // time not expired on boot up, or the i/o module it is
        // assigned to is missing
        status = INPUT_DISABLED;

    }

    return status;

}


//=============================================================================
//
// IN_C16_aba_brake_momentary_signal_status()
//
//=============================================================================
//
Input_State_t IN_C16_aba_brake_momentary_signal_status()
{
    Input_State_t status;

    //
    // Check the state of pin E01 and return the status of the PIN.
    // True indicates the presence of 12V at the PIN. False indicates
    // 0V at the PIN.
    //
    if (IN_C16_aba_brake_momentary_signal == INPUT_ON)
    {

        // The digital input is on
        status = INPUT_ON;

    }
    else if (IN_C16_aba_brake_momentary_signal == INPUT_OFF)
    {

        // The digital input is off
        status = INPUT_OFF;

    }
    else if (IN_C16_aba_brake_momentary_signal == INPUT_DISABLED)
    {

        // The digital input is either not configured, its debounce
        // time not expired on boot up, or the i/o module it is
        // assigned to is missing
        status = INPUT_DISABLED;

    }

    return status;

}


//=============================================================================
//
// IN_C18_reverse_switch_status()
//
//=============================================================================
//
Input_State_t IN_C18_reverse_switch_status()
{
    Input_State_t status;

    //
    // Check the state of pin E01 and return the status of the PIN.
    // True indicates the presence of 12V at the PIN. False indicates
    // 0V at the PIN.
    //
    if (IN_C18_reverse_switch == INPUT_ON)
    {

        // The digital input is on
        status = INPUT_ON;

    }
    else if (IN_C18_reverse_switch == INPUT_OFF)
    {

        // The digital input is off
        status = INPUT_OFF;

    }
    else if (IN_C18_reverse_switch == INPUT_DISABLED)
    {

        // The digital input is either not configured, its debounce
        // time not expired on boot up, or the i/o module it is
        // assigned to is missing.
        status = INPUT_DISABLED;

    }

    return status;

}


//=============================================================================
//
// The E port on the CVC has special inputs which are connected to
// sensors. These sensors return a uint16_t if a value is avaliable.
//
//=============================================================================
//

//=============================================================================
//
// IN_E01_cvc_zener_input_status()
//
//=============================================================================
//
uint16_t IN_E01_cvc_zener_input_status()
{
    Analog_Values_ analog_values;
    uint16_t analog_reading = 0;

    /*
     * Call this function to read analog value of input
     * software_input_number is the input software number of the input
     * you want to read (since it is on master module it lines up with
     * the IOMap address) frequency sets the amount of time the input
     * is sampled over.  For example 50 Hz -> 20 msec, 100 Hz, 10
     * msec, 200 Hz 5 msec return the Analog_Values_ structure NOTE:
     * if the frequency is set such that it takes longer to sample the
     * input than the loop time, the previous value will be reported
     * until the sampling is finished.  In some cases the inputs on
     * the module are run through a multiplexer, in these cases having
     * one long sample time will affect all the inputs since the
     * multiplexer cannot be flipped until all the inputs have
     * finished sampling
     */
    int16_t frequency = (int16_t)FREQ_100HZ;

    analog_values =  Read_Analog_Value_Inputs(
         IN_E01_cvc_zener_input, 100);

     // If flag false, the analog value return invalid; if flag true,
     // the analog value return valid
     if (analog_values.available_analog_value_flag == TRUE)
     {

         analog_reading = analog_values.ui16_analog_value;

     }
     else
     {

         analog_reading = 0;

     }


     return analog_reading;

}


//=============================================================================
//
// IN_E03_throttle_position_sensor_signal_status()
//
//=============================================================================
//
uint16_t IN_E03_throttle_position_sensor_signal_status()
{
    Analog_Values_ analog_values;
    static uint16_t analog_reading = 0;

    /*
     * Call this function to read analog value of input
     * software_input_number is the input software number of the input
     * you want to read (since it is on master module it lines up with
     * the IOMap address) frequency sets the amount of time the input
     * is sampled over.  For example 50 Hz -> 20 msec, 100 Hz, 10
     * msec, 200 Hz 5 msec return the Analog_Values_ structure NOTE:
     * if the frequency is set such that it takes longer to sample the
     * input than the loop time, the previous value will be reported
     * until the sampling is finished.  In some cases the inputs on
     * the module are run through a multiplexer, in these cases having
     * one long sample time will affect all the inputs since the
     * multiplexer cannot be flipped until all the inputs have
     * finished sampling
     */
     analog_values =  Read_Analog_Value_Inputs(
    		 IOMapIndex_IN_E03_throttle_position_sensor_signal,
    		 10);

     analog_reading = analog_values.ui16_analog_value;

     uint16_t throttle = (uint16_t)(analog_reading * 0.0473260) - 23.663038;

     return throttle;

}
