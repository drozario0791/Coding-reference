/******************************************************************************
 *
 *        Name: hydraulic_system.c
 *
 * Description: The carrier consists of two inverters that power the
 *              hydraulic motors. The skai2 powers a Remy 250-90,
 *              which is responsible for the making the main system
 *              pressure, front brake pressure, rear brake pressure,
 *              drive line brake pressure and hydraulic steering.  The
 *              second inverter is the air cooled sevcon, which is
 *              connected to a stepped motor. The stepper motor is
 *              responsible for the pressure on the transmission.
 *              This module controls the working of the hydraulics on
 *              the carrier.
 *
 *      Author: Deepak
 *        Date: Tuesday, 27 August 2019
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
#include "sevcon_hvlp10_device.h"
#include "hydraulic_inverter_control.h"
#include "transmission_inverter_control.h"
#include "gears_and_transmission.h"
#include "skai2_inverter_vissim.h"
#include "hydraulic_system.h"
#include "timer_service.h"
#include "state_machine.h"
#include "cvc_debug_msgs.h"
#include "pdm_device.h"

//
// The thresholds for hydraulic system pressure on the carrier.
//
#define MIN_PRESSURE_THRESHOLD_IN_PSI              1380
#define ABSOLUTE_MIN_PRESSURE_THRESHOLD_IN_PSI     1150
#define MIN_TRANSMISSION_OIL_PRESSURE_THRESHOLD    175
#define SOFT_START_SOLENOID_UPPER_RPM_LIMIT        1000
#define SOFT_START_SOLENOID_LOWER_RPM_LIMIT        100


//=============================================================================
//
// Function Prototypes
//
//=============================================================================
//
static void rear_brake_lights_control();
uint16_t convert_adc_readings_to_psi(uint16_t adc_reading);


/******************************************************************************
 *
 *        Name : hydraulic_system_control()
 *
 * Description: Controls the working of the hydraulic system. There
 *              are two motors on the carrier that control the volume
 *              of fluid through the hydraulic system. The first one
 *              is the remy 250-90. The remy is used to power the
 *              hydraulics responsible for main system pressure,
 *              accumulator pressure, steering pressure and
 *              brakes. The second motor is the stepper motor, which
 *              is controlled by a sevcon. The motor control the
 *              volume of fluid through the transmission system.
 *
 *      Author: Deepak
 *        Date: Thursday, 29 August 2019
 *
 ******************************************************************************
 */
void hydraulic_system_control(device_instances_t device_hydraulic_inverter,
                              device_instances_t device_transmission_inverter)
{
    //
    // shuttle shifter timer
    //
    static timer_t shuttle_shift_timer;
    bool_t shuttle_shift = FALSE;
    bool_t transmission_inverter_enable = FALSE;
    uint16_t hydraulic_inverter_enable = FALSE;

    static bool_t first_pass = TRUE;

    if (first_pass)
    {

        //
        // Setup the shuttle_shift_timer, would change from a 1 to 0,
        // after the specified time.
        //
        shuttle_shift_timer =
            timer_setup(get_THREE_SECONDS(), FALLING);

        first_pass = FALSE;

    }

    //
    // Get the direction of the shifter. Forward, reverse or neutral.
    //
    shifter_position_t shifter_direction =
        get_shifter_direction();

    //
    // Get the RPM of the hydraulic motor. The rpm of the
    // hydraulic_motor is used to enable/disable the soft start solenoid on
    // the carrier.
    //
    uint16_t hydraulic_motor_rpm =
        skai_get_vissim_motor_rpm(device_hydraulic_inverter);

    //
    // Enable the timer when any one of the two conditions have been
    // met
    //
    //
    // Shuttle shift is true if the vehicle is in F or R, and stays
    // true for a time after the vehicle is taken out of gear.
    //
    shuttle_shift = timer_operate(
        &shuttle_shift_timer,
        (shifter_direction == SHIFTER_FORWARD)
        || (shifter_direction == SHIFTER_REVERSE));

    //
    // Check to see if high voltage is enabled on the system.
    //
    bool_t sm_enable_hv_system =
        get_sm_status_enable_hv_systems();

    //
    // When the solenoid is enabled, the motor does not have a load
    // applied. This is done to reduce large start up currents. Once
    // the rpm is above a set limit, the softstart sonlenoid is
    // disabled. When disabled, the motor see the full hydraulic load
    // of the system. The soft-start solenoid is enabled during two
    // conditions. The conditions are given below.
    //

    //
    // Enable the hydraulic motor only when the carrier is not in
    // neutral.
    //
    //
    // 1st Condition: When the motor is enabled, read the rpm of
    // the motor. Keep the softstart solenoid enabled until the
    // rpm exceeds its upper set limit.
    //

//    if((hydraulic_motor_rpm < SOFT_START_SOLENOID_UPPER_RPM_LIMIT) && sm_enable_hv_system)
//    {
//
//    	//
//    	// Enable the softstart solenoid
//    	//
//    	pdm_2_command_start_drain_solenoid_pwr(TRUE);
//
//    }
//    else
//    {
//
//    	//
//    	// Disable the softstart solenoid
//    	//
//    	pdm_2_command_start_drain_solenoid_pwr(FALSE);
//
//    }

    //
    //
    // A function that is responsible to enable/disable the hydraulic
    // inverter based on various conditions. The function also checks
    // to ensure high voltage is ON and if precharger was successful.
    // Also transmits the approprite CAN message after all limits are
    // set.
    //
    //
    // Enable the hydraulic motor if the following conditions are met.
    //
    // 1) HV_system is ON (battery is precharged and ready).  This is
    //    obtained from the state machine.
    //
    // 2) Shuttle_shift_timer is true.
    //
    // 3) The vehicle is not charging.
    //
    // 4) The vehilce is not in post_charge_idle.
    //
    // 5) The vehilce is has no critical_failure_hv_on failure.

    //
    // get the current state of the state machine
    //
    state_t sm_state = get_sm_current_state();

    //
    // Enable the hydraulic_inverter if they meet all these conditions.
    //
    hydraulic_inverter_enable =
        (uint16_t)(sm_enable_hv_system
                  && shuttle_shift
                  && (sm_state != POST_CHARGE_IDLE)
                  && (sm_state != CRITICAL_FAILURE_HV_ON)
                  && (sm_state != CHARGING));

    //
    // Function call that controls the hydraulic inverter.
    //
    hydraulic_inverter_control(device_hydraulic_inverter,
                               hydraulic_inverter_enable);

    //
    //
    // A function that is responsible to enable/disable the
    // transmission inverter based on various conditions. The function
    // also checks to ensure high voltage is ON and if precharger was
    // successful.  Also transmits the approprite CAN message after
    // all limits are set.
    //
    // The function also sets up the limits of operation for the
    // transmission motor. Also transmits the approprite CAN message
    // after all limits are set.
    //

    //
    // Enable the sevcon inverter if it meets the following conditions.
    //

    transmission_inverter_enable =
        (bool_t)(sm_enable_hv_system
                 && shuttle_shift
                 && (sm_state != POST_CHARGE_IDLE)
                 && (sm_state != CRITICAL_FAILURE_HV_ON)
                 && (sm_state != CHARGING));

    //
    // Function call that controls the transmission inverter.
    //
    if (transmission_inverter_enable == TRUE)
    {

        //
        // Enable the Sevcon Device
        //
        transmission_inverter_control(device_transmission_inverter,
                                      ENABLE_SEVCON);

    }
    else
    {

        //
        // Disable the Sevcon Device
        //
        transmission_inverter_control(device_transmission_inverter,
                                      DISABLE_SEVCON);
    }

}



//=============================================================================
//
// GETTERS
//
//=============================================================================
//



//=============================================================================
//
// get_rear_accum_pressure()
//
//=============================================================================
//
uint16_t get_rear_accum_pressure()
{

    uint16_t analog_reading =
        pdm_get_analog_input(TWO, PDM_ADC_7);


    return convert_adc_to_psi(analog_reading);

}

//=============================================================================
//
// get_transmission_pressure()
//
//=============================================================================
//
uint16_t get_transmission_pressure()
{

    uint16_t analog_reading =
        pdm_get_analog_input(TWO, PDM_ADC_3);


    return convert_adc_to_psi(analog_reading);

}

//=============================================================================
//
// get_front_axil_parking_brake_pressure()
//
//=============================================================================
//
uint16_t get_front_axil_parking_brake_pressure()
{

    uint16_t analog_reading =
        pdm_get_analog_input(TWO, PDM_ADC_5);


    return convert_adc_to_psi(analog_reading);

}

//=============================================================================
//
// get_drive_line_brake_pressure()
//
//=============================================================================
//
uint16_t get_drive_line_brake_pressure()
{

    uint16_t analog_reading =
        pdm_get_analog_input(ONE, PDM_ADC_8);

    return convert_adc_to_psi(analog_reading);

}


//=============================================================================
//
// get_front_accum_pressure()
//
//=============================================================================
//
uint16_t get_front_accum_pressure()
{

    uint16_t analog_reading =
        pdm_get_analog_input(TWO, PDM_ADC_8);

    return convert_adc_to_psi(analog_reading);
}

//=============================================================================
//
// get_front_service_brake_pressure()
//
//=============================================================================
//
uint16_t get_front_service_brake_pressure()
{

    uint16_t analog_reading =
        pdm_get_analog_input(TWO, PDM_ADC_4);

    return convert_adc_to_psi(analog_reading);
}


uint16_t get_rear_service_brake_pressure()
{

    uint16_t analog_reading =
        pdm_get_analog_input(TWO, PDM_ADC_6);


    return convert_adc_to_psi(analog_reading);


}

//=============================================================================
//
// main_sys_pressure()
//
//=============================================================================
//
uint16_t main_sys_pressure()
{

    uint16_t main_system_pressure =
        (uint16_t)(1.25 * IN_E10_main_sys_pressure_sensor_signal - 624.99);

    return main_system_pressure;

}





//=============================================================================
//
// convert_adc_to_psi(): Convert murphy adc to pressure.
//
//=============================================================================
//
uint16_t convert_adc_to_psi(uint16_t raw_adc_value)
{

    float raw_adc_to_milli_volts =  (float)(raw_adc_value * 4.88e-3) * 1000;


    //
    // 0.5V corresponds to 0 PSI
    // 4.5V corresponds to 5000 PSI
    //
    uint16_t pressure_in_psi =
        (uint16_t)(1.25 * raw_adc_to_milli_volts - 624.99);

    if(pressure_in_psi < 0)
    {
        //
        // If PSI reading is lower than 0, force the PSI to 0.
        //
        pressure_in_psi = 0;
    }

    //
    // return the pressure in PSI.
    //
    return pressure_in_psi;
}
