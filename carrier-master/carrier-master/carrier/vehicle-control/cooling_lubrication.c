/******************************************************************************
 *
 *        Name: cooling_lubrication.c
 *
 * Description:
 *
 *        Date: Wednesday, 21 August 2019
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
#include "Prototypes_CAN.h"
#include "can_service.h"
#include "timer_service.h"
#include "fvt_library.h"
#include "pdm_device.h"
#include "pdm_control_1.h"
#include "pdm_control_2.h"
#include "pdm_control_3.h"
#include "skai2_inverter_vissim.h"
#include "bel_charger_device.h"
#include "bel_charger_control.h"
#include "shinry_dcdc_device.h"
#include "cooling_lubrication.h"
#include "cl712_device.h"
#include "state_machine.h"
#include "traction_inverter_control.h"
#include "hydraulic_inverter_control.h"
#include "cvc_debug_msgs.h"

//=============================================================================
//
// Function Prototypes()
//
// Pre Oil Timer State
//
//=============================================================================
//
static bool_t get_preoil_timer_state(bool_t timer_enable);

bool_t drive_motor_temp_exceeded_status(uint16_t temp_limit);
bool_t hydraulic_motor_temp_exceeded_status(uint16_t temp_limit);
bool_t drive_inverter_temp_exceeded_status(uint16_t temp_limit);
bool_t hydraulic_inverter_temp_exceeded_status(uint16_t temp_limit);
bool_t dcdc_temp_exceeded_status(uint16_t temp_limit);
bool_t charger1_temp_exceeded_status(uint16_t temp_limit);
bool_t charger2_temp_exceeded_status(uint16_t temp_limit);

//=============================================================================
//
// Device Instances
//
//=============================================================================
//
static device_instances_t skai2_device_traction_inverter = ONE;
static device_instances_t skai2_device_hydraulic_inverter = TWO;
static device_instances_t hed_device_screen = ONE;
static device_instances_t bel_charger_device_1 = ONE;
static device_instances_t bel_charger_device_2 = ONE;
static device_instances_t shinry_device_1 = ONE;

//=============================================================================
//
// Static Function Prototypes
//
//=============================================================================
//
static void run_carrier_inverter_motor_pump_control(bool_t pre_oil_timer);
static void run_carrier_charger_pumps_control(bool_t pre_oil_timer);

static bool_t run_charger_1_and_2_fan();
static bool_t run_hydraulic_motor_radiator_cooling_fan();
static bool_t run_hydraulic_fan_solenoid();


//=============================================================================
//
// Pre-Oil Timer: Enables
//
//=============================================================================
//
static bool_t get_preoil_timer_state(bool_t timer_enable)
{

    // drive cooling pump timer.
    static timer_t pre_oil_timer;
    static bool_t first_run = TRUE;
    bool_t pre_oil_state;

    //
    // Initialise timer during first run
    //
    if (first_run)
    {

        //
        // Setup the timers pre-oil/pre-lubrication timers for the
        // pumps and radiator fans.
        //
        pre_oil_timer  =
            timer_setup( get_TEN_SECONDS(), PULSE);

        pre_oil_state = timer_operate(
            &pre_oil_timer,
            FALSE);

        //
        // Set first run to false to ensure this function is part of
        // the code is not called unless a power cycle happened.
        //
        first_run = FALSE;
    }

    pre_oil_state = timer_operate(
        &pre_oil_timer,
        timer_enable);

    return pre_oil_state;
}




/******************************************************************************
 *
 *        Name: cooling_and_lubcrication_system()
 *
 * Description: A pre-oil/pre-lubrication takes place when pre-charge
 *              is complete. This is done for two reasons, the first
 *              one is lubricating the motor before use. The second
 *              reason is to ensure all the pumps and fans function.
 *              During the pre-oil state, the current drawn by the
 *              pumps and fans are measured, the sinking of current by
 *              these devices indicate a pump/fan has not failed. Oil
 *              is pumped once hv is on to make sure the motor is
 *              lubricated before moving.
 *
 ******************************************************************************
 */
void cooling_and_lubcrication_control()
{

    //
    // Check the state of HV_enabled state from the state machine
    //
    bool_t sm_hv_enable = get_sm_status_enable_hv_systems();

    bool_t pre_oil_timer =
    		get_preoil_timer_state(sm_hv_enable);

    //
    // Runs all pumps related to motors and inverters.
    //
    run_carrier_inverter_motor_pump_control(pre_oil_timer);

    //
    // Runs all the pumps related to the charger
    //
    run_carrier_charger_pumps_control(pre_oil_timer);

    //
    // Control the working of the Fans related to the charge
    //
    run_charger_1_and_2_fan();

    //
    // Run Charger 1 and 2 Fans
    //
    bool_t charger_1_and_2_fan_state =
        run_charger_1_and_2_fan();

    //
    // Run Hydraulic motor radiator fan
    //
    bool_t hydraulic_motor_radiator_cooling_fan_state =
        run_hydraulic_motor_radiator_cooling_fan();

    //
    // Run Hydarulic Fan Solenoid
    //
    bool_t hydraulic_fan_solenoid_state =
        run_hydraulic_fan_solenoid();

}


//=============================================================================
//
// carrier_inverter_motor_pump_control()
//
//=============================================================================
//
static void run_carrier_inverter_motor_pump_control(bool_t pre_oil_timer)
{
    //
    // The pumps on the carrier are enabled when one of the two
    // conditions are met.
    //
    // 1) Right after precharge.
    // 2) When the inverters are enabled

    bool_t traction_inverter_enable = traction_inverter_state();
    bool_t hydraulic_inverter_enable = hydraulic_inverter_state();
    state_t state = get_sm_current_state();

    bool_t traction_pump_override_state =
        get_traction_motor_pump_override(ONE);

    bool_t hydraulic_pump_override_state =
        get_hydraulic_motor_pump_override(ONE);

    bool_t pe_pump_override_state =
        pe_pump_override(ONE);

    //
    // Traction motor Enable
    //
    bool_t enable_traction_motor_pump = (pre_oil_timer
                                         || traction_inverter_enable);

    //
    // Hydraulic Motor Enable
    //
    bool_t enable_hydraulic_motor_pump = (pre_oil_timer
                                          || hydraulic_inverter_enable
                                          || hydraulic_motor_temp_exceeded_status(50));

    //
    // PE Pump Enable
    //
    bool_t pe_pump_enable = enable_traction_motor_pump;

    //=============================================================================
    //
    // Enable the fans when one of the two conditions are met
    //
    //=============================================================================
    //
    if((get_sm_current_state() != CHARGING))
    {

        //
        // Enable Pumps if state is not in charging
        //
        pdm_2_command_hydraulic_motor_cooling_pump_pwr(enable_hydraulic_motor_pump ^ hydraulic_pump_override_state);

        pdm_3_command_drive_motor_cooling_pump_pwr(enable_traction_motor_pump ^ traction_pump_override_state);

        pdm_3_command_imt_cooling_pump_pwr(pe_pump_enable ^ pe_pump_override_state);

        pdm_2_command_power_electronics_pump_pwr(pe_pump_enable ^ pe_pump_override_state);

    }
    else
    {

        //
        // Disable motor and inverter pumps when in charing mode
        //
        pdm_2_command_hydraulic_motor_cooling_pump_pwr(FALSE);
        pdm_3_command_drive_motor_cooling_pump_pwr(FALSE);
        pdm_3_command_imt_cooling_pump_pwr(FALSE);
        pdm_2_command_power_electronics_pump_pwr(FALSE);

    }
}


//=============================================================================
//
// carrier_charger_pumps_control()
//
//=============================================================================
//
static void run_carrier_charger_pumps_control(bool_t pre_oil_timer)
{

    //
    // The charger is enabled only when high voltage is on and the
    // charge button is pressed.
    //
    bool_t charger_enabled = charger_enabled_state();

    bool_t charger_pump_override_state =
        charger_pump_override(ONE);

    //
    // Enable CHarger Pump
    //
    bool_t enable_charger_pump = (bool_t)((charger_enabled || pre_oil_timer) ^ charger_pump_override_state);

    //
    // if the charger is enabled, enable the power for the pumps to
    // the chargers.
    //
    pdm_3_command_pump_charger_1_cooling_pwr(enable_charger_pump);
    pdm_3_command_pump_charger_2_cooling_pwr(enable_charger_pump);

}

//=============================================================================
//
//
//
//=============================================================================
//
static bool_t run_charger_1_and_2_fan()
{

    //
    // The charger is enabled only when high voltage is on and the
    // charge button is pressed.
    //
    bool_t charger_enabled = charger_enabled_state();

    //
    // Check to see if the chargers have exceeded their temperature
    // thresholds.
    //
    bool_t charger1_exceeded_temp = charger1_temp_exceeded_status(45);
    bool_t charger2_exceeded_temp = charger2_temp_exceeded_status(45);


    bool_t charger_fan_override_state =
        charger_fan_override(ONE);

    bool_t enable_fan = (bool_t)((charger_enabled &&
                                  (charger1_exceeded_temp ||
                                   charger2_exceeded_temp)) ^
                                 charger_fan_override_state);



    //
    // If any of the above conditions are true, enable power to the
    // drive motor cooling pump.
    //
    pdm_3_command_fan_radiator_1_2_pwr(enable_fan);

    return enable_fan;

}


//=============================================================================
//
// run_hydraulic_motor_radiator_cooling_fan()
//
//=============================================================================
//
static bool_t run_hydraulic_motor_radiator_cooling_fan()
{

    // Check to see of the inverters are enabled
    bool_t hydraulic_inverter_enable = hydraulic_inverter_state();

    //
    // Check to see if the motor/inverter have exceeded their
    // temperature, threshold.
    //
    bool_t hydrailic_motor_temp_status = hydraulic_motor_temp_exceeded_status(65);

    bool_t hydraulic_fan_override =
        pe_fan_override(ONE);

    bool_t enable_fan = (bool_t)((hydrailic_motor_temp_status
                         && hydraulic_inverter_enable) ^ hydraulic_fan_override);

    //
    // If any of the above conditions are true, enable power to the
    // drive motor cooling pump.
    //
    if((get_sm_current_state() != CHARGING))
    {

        pdm_2_command_hydraulic_motor_radiator_cooling_fan_pwr(enable_fan);

    }
    else
    {

        pdm_2_command_hydraulic_motor_radiator_cooling_fan_pwr(FALSE);

    }

    return enable_fan;
}


/******************************************************************************
 *
 *        Name: run_hydraulic_fan_solenoid()
 *
 * Description:
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
static bool_t run_hydraulic_fan_solenoid()
{

    // Check to see of the inverters are enabled
    bool_t hydraulic_inverter_enable = hydraulic_inverter_state();

    //
    // Check to see if the motor/inverter have exceeded their
    // temperature, threshold.
    //
    bool_t dc_dc_temp_status = dcdc_temp_exceeded_status(60);
    bool_t hyd_inv_temp_status = hydraulic_inverter_temp_exceeded_status(65);
    bool_t drive_inv_temp_status = drive_inverter_temp_exceeded_status(65);
    bool_t hydrailic_motor_temp_status = hydraulic_motor_temp_exceeded_status(65);
    bool_t drive_motor_temp_status = drive_motor_temp_exceeded_status(65);

    bool_t enable_fan = ((dc_dc_temp_status
                              || hyd_inv_temp_status
                              || drive_inv_temp_status
                              || hydrailic_motor_temp_status
                              || drive_motor_temp_status)
                              && (hydraulic_inverter_enable));

    //
    // If any of the above conditions are true, enable power to the
    // drive motor cooling pump.
    //
    if(enable_fan == TRUE)
    {
    	pdm_3_command_hydraulic_fan_solenoid_pwr(FALSE);
    }
    else
    {
    	pdm_3_command_hydraulic_fan_solenoid_pwr(TRUE);
    }

    return enable_fan;
}

//=============================================================================
//
// get_cooling_device_state_of_health()
//
//=============================================================================
//
bool_t get_cooling_device_state_of_health()
{

    //
    // Return the state of health of all the cooling pumps and fans on
    // the vehicle.
    //
    return FALSE;

}


//=============================================================================
//
//
//
//=============================================================================
//
bool_t drive_motor_temp_exceeded_status(uint16_t temp_limit)
{
    bool_t status = FALSE;

    unsigned short traction_motor_temperature =
        skai_get_vissim_motor_temp_C(skai2_device_traction_inverter);

    if(traction_motor_temperature > temp_limit)
    {
        status = TRUE;
    }
    else
    {
        status = FALSE;
    }
    return status;

}

//=============================================================================
//
//
//
//=============================================================================
//
bool_t hydraulic_motor_temp_exceeded_status(uint16_t temp_limit)
{

    bool_t status = FALSE;

    unsigned short hydraulic_motor_temperature =
        skai_get_vissim_motor_temp_C(skai2_device_hydraulic_inverter);

    if(hydraulic_motor_temperature > temp_limit)
    {
        status = TRUE;
    }
    else
    {
        status = FALSE;
    }
    return status;

}

//=============================================================================
//
//
//
//=============================================================================
//
bool_t drive_inverter_temp_exceeded_status(uint16_t temp_limit)
{
    bool_t status = FALSE;

    uint16_t traction_inverter_temperature =
        skai_get_vissim_max_temperature(skai2_device_traction_inverter);

    if(traction_inverter_temperature > temp_limit)
    {
        status = TRUE;
    }
    else
    {
        status = FALSE;
    }
    return status;

}

//=============================================================================
//
//
//
//=============================================================================
//
bool_t hydraulic_inverter_temp_exceeded_status(uint16_t temp_limit)
{

    bool_t status = FALSE;

    uint16_t hydraulic_inverter_temperature =
        skai_get_vissim_max_temperature(skai2_device_hydraulic_inverter);

    if(hydraulic_inverter_temperature > temp_limit)
    {
        status = TRUE;
    }
    else
    {
        status = FALSE;
    }
    return status;

}

//=============================================================================
//
//
//
//=============================================================================
//
bool_t charger1_temp_exceeded_status(uint16_t temp_limit)
{

    bool_t status = FALSE;

    int16_t charger1_temperature =
        bel_get_chassis_temperature_c(bel_charger_device_1);

    if(charger1_temperature > temp_limit)
    {
        status = TRUE;
    }
    else
    {
        status = FALSE;
    }

    return status;

}

//=============================================================================
//
//
//
//=============================================================================
//
bool_t charger2_temp_exceeded_status(uint16_t temp_limit)
{

    bool_t status = FALSE;

    int16_t charger2_temperature =
        bel_get_chassis_temperature_c(bel_charger_device_2);

    if(charger2_temperature > temp_limit)
    {
        status = TRUE;
    }
    else
    {
        status = FALSE;
    }

    return status;

}

//=============================================================================
//
//
//
//=============================================================================
//
bool_t dcdc_temp_exceeded_status(uint16_t temp_limit)
{

    bool_t status = FALSE;

    uint16_t dc_dc_temperature =
        shinry_get_instantaneous_temperature(shinry_device_1);

    if(dc_dc_temperature > temp_limit)
    {
        status = TRUE;
    }
    else
    {
        status = FALSE;
    }

    return status;

}
