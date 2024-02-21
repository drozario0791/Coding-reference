/******************************************************************************
 *
 *        Name: state_machine.c
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Saturday, 07 September 2019
 *
 ******************************************************************************
 */

#include "Prototypes.h"
#include "Constants.h"
#include "state_machine.h"
#include "prototypes.h"
#include "User_Low_Power.h"
#include "fvt_library.h"
#include "timer_service.h"
#include <math.h>
#include <stdlib.h>
#include "can_service.h"
#include "skai2_inverter.h"
#include "skai2_inverter_vissim.h"
#include "shinry_dcdc_device.h"
#include "orion_control.h"
#include "state_machine.h"
#include "gears_and_transmission.h"
#include "can_switches.h"
#include "bel_charger_control.h"

#define MAX_CHARGING_CELL_VOLTAGE 40400
extern bool_t low_power_mode;

///////////////////////////////////////////////////////////////////////////
//
// STATE MACHINE
//
///////////////////////////////////////////////////////////////////////////
//
static state_machine_input_data_t sm_input_data;
static state_machine_output_data_t sm_output_data;


//
// Private function prototypes
//
static state_t
do_state_zero_energy(
    const state_machine_input_data_t *input_data,
    state_machine_output_data_t *output_data);

static state_t
do_state_master_closed_no_hv(
    const state_machine_input_data_t *input_data,
    state_machine_output_data_t *output_data);

static state_t
do_state_startup(
    const state_machine_input_data_t *input_data,
    state_machine_output_data_t *output_data);

static state_t
do_state_charging(
    const state_machine_input_data_t *input_data,
    state_machine_output_data_t *output_data);

static state_t
do_state_post_charge_idle(
    const state_machine_input_data_t * input_data,
    state_machine_output_data_t * output_data);

static state_t
do_state_brake_test_mode(
    const state_machine_input_data_t *input_data,
    state_machine_output_data_t *output_data);

static state_t
do_state_diagnostic_mode(
    const state_machine_input_data_t *input_data,
    state_machine_output_data_t *output_data);

static state_t
do_state_ready_to_drive(
    const state_machine_input_data_t *input_data,
    state_machine_output_data_t *output_data);

static state_t
do_state_tow_mode(
    const state_machine_input_data_t *input_data,
    state_machine_output_data_t *output_data);

static state_t
do_state_dead_battery(
    const state_machine_input_data_t *input_data,
    state_machine_output_data_t *output_data);

static state_t
do_state_critical_failure_hv_off(
    const state_machine_input_data_t *input_data,
    state_machine_output_data_t *output_data);

static state_t
do_state_critical_failure_hv_on(
    const state_machine_input_data_t *input_data,
    state_machine_output_data_t *output_data);

static state_t
do_state_e_stop(
    const state_machine_input_data_t *input_data,
    state_machine_output_data_t *output_data);

static state_t
do_state_shutdown(
    const state_machine_input_data_t *input_data,
    state_machine_output_data_t *output_data);

static state_machine_contactor_status_t
contactor_control(
    bool_t power_up,
    bool_t in_neutral,
    bool_t in_motion,
    uint16_t dc_link_voltage,
    uint16_t battery_voltage);

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// It is expected that the programmer will make sure all these values
// are properly filled in and passed into the state machine for proper
// operation.
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
static state_machine_output_data_t
update_output_data(
    const state_machine_input_data_t *input_data,
    const bool_t hv_desired,
    const state_t current_state,
    const bool_t brakes_desired,
    const bool_t charging_desired,
    const float charge_current_factor);


//////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION OF GENERAL FUNCTION OF STATE MACHINE
//
// each state function shares several characteristics:
//
//  1) It assigns the result of update_output_data() to the pointer to
//     output_data passed to it which ensures that the output_data is
//     fully populated.
//
//  2) It declares a destination_state, which is what the function
//     will return.
//
//  3) Several if / else statements determine what destination_state
//     will be.
//
//  4) The destination_state is returned..
//
//////////////////////////////////////////////////////////////////////////////



/******************************************************************************
 *
 *        Name: do_state_zero_energy()
 *
 * Description: The zero energy state corresponds to the vehicle's
 *              rest state or state on startup. It is the initial
 *              state of the central vehicle controller before the
 *              master is closed, and before precharge update
 *              output_data structure with all parameters every time.
 *
 *
 *      Author: Tom
 *        Date: Friday, 16 August 2019
 *
 ******************************************************************************
 */
static state_t
do_state_zero_energy(
    const state_machine_input_data_t *input_data,
    state_machine_output_data_t *output_data)
{
    *output_data =
        update_output_data(
            input_data, FALSE,
            ZERO_ENERGY, TRUE, FALSE, 0);

    state_t destination_state;

    if (input_data->master_closed)
    {
        destination_state = MASTER_CLOSED_NO_HV;
    }
    else
    {
        destination_state = ZERO_ENERGY;
    }


    return destination_state;
}


/******************************************************************************
 *
 *        Name: do_state_master_closed_no_hv()
 *
 * Description: The master_closed_no_hv state corresponds to the state
 *              the vehicle is in right after the master disconnect is
 *              closed and before high voltage is requested. Update
 *              the output_data structure with all parameters every
 *              time.
 *
 *      Author: Tom
 *        Date: Friday, 16 August 2019
 *
 ******************************************************************************
 */
static state_t
do_state_master_closed_no_hv(
    const state_machine_input_data_t *input_data,
    state_machine_output_data_t *output_data)
{
    *output_data = update_output_data(
        input_data, FALSE,
        MASTER_CLOSED_NO_HV, TRUE, FALSE, 0);

    //
    // If the vehicle is powered on with the key on, require the key
    // to be turned off first.  key must transition from off to on for
    // the vehicle to precharge.
    //
    static bool_t   ignition_off_before_on  = FALSE;

    //
    // This flip flop ensures that the ignition is off before turning
    // it on has an effect.
    //
    ignition_off_before_on =
        set_reset(ignition_off_before_on,
                  input_data->ignition_on, FALSE);

    state_t destination_state;

    //
    // If the master is open, go to the zero energy state.
    //
    if ( !input_data->master_closed)
    {
        destination_state = ZERO_ENERGY;
    }

    //
    // If the diagnostic mode switch is closed, enter diagnostic mode.
    //
    else if (input_data->diagnostic_mode)
    {
        destination_state = DIAGNOSTIC_MODE;
    }

    //
    // If the vehicle is plugged in and the key has gone from off to
    // on, go to startup.
    //
//    else if (input_data->plugged_in ||
//             (input_data->ignition_on && ignition_off_before_on))
//    else if (input_data->plugged_in || input_data->ignition_on)

    else if (input_data->plugged_in ||
             (input_data->ignition_on && ignition_off_before_on))
    {
        destination_state = STARTUP;
    }
    else
    {
        destination_state = MASTER_CLOSED_NO_HV;
    }

    //
    // If we're leaving this state, reset the flip flop so that it
    // works properly the next time we return.
    //
    if (destination_state != MASTER_CLOSED_NO_HV)
    {
        ignition_off_before_on = FALSE;
    }

    return destination_state;
}


/******************************************************************************
 *
 *        Name: do_state_startup()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Friday, 16 August 2019
 *
 ******************************************************************************
 */
static state_t
do_state_startup(
    const state_machine_input_data_t *input_data,
    state_machine_output_data_t *output_data)
{
    *output_data =
        update_output_data(
            input_data, FALSE,
            STARTUP, TRUE, FALSE, 0);

    state_t destination_state;
    // This state is only temporary, so we set a timer for when we exit
    static timer_t startup_timer;
    static bool_t timers_initialized  = FALSE;

    if (!timers_initialized)
    {
        startup_timer = timer_setup(get_FIVE_SECONDS(), RISING);
        timers_initialized = TRUE;
    }

    //
    // Startup will be complete after the timer expires, or we leave
    // this state for another reason.
    //
    bool_t  startup_complete = timer_operate(&startup_timer, TRUE);

    //
    // If the e-stop is pressed go to the e-stop state.
    //
    if (input_data->e_stop)
    {
        destination_state = E_STOP;
    }

    //
    // If the master is open, go to the zero energy state, since
    // precharge has yet to happen.
    //
    if (!input_data->master_closed)
    {
        destination_state = ZERO_ENERGY;
    }

    //
    // If someone turns the key off and the vehicle is unplugged, go
    // back.
    //
    else if (!input_data->ignition_on && !input_data->plugged_in)
    {
        destination_state = MASTER_CLOSED_NO_HV;
    }
    else if (input_data->diagnostic_mode)
    {
        destination_state = DIAGNOSTIC_MODE;
    }

    //
    // Once the timer has expired, see where we're going.
    //
    else if (startup_complete || (input_data->critical_fault_hv_off == 0))
    {
        if (input_data->critical_fault_hv_off != 0)
        {
            destination_state   = CRITICAL_FAILURE_HV_OFF;
        }
        else if (input_data->critical_fault_hv_on != 0)
        {
            destination_state   = CRITICAL_FAILURE_HV_ON;
        }
        else if (input_data->plugged_in)
        {
            destination_state = CHARGING;
        }
        else //normal startup
        {
            destination_state   = READY_TO_DRIVE;
        }
    }
    else
    {
        destination_state   = STARTUP;
    }

    if (destination_state != STARTUP)
    {   // Reset the timer so that the next startup will be the same.
        startup_complete    = timer_operate(&startup_timer, FALSE);
    }

    return destination_state;
}


/******************************************************************************
 *
 *        Name: do_state_charging()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Friday, 16 August 2019
 *
 ******************************************************************************
 */
static state_t
do_state_charging(
    const state_machine_input_data_t *input_data,
    state_machine_output_data_t *output_data)
{
    //
    // Set the slowdown voltage within 0.1V of the set maximum cell
    // voltage.
    //
#define SLOW_CHARGING_VOLTAGE_PROX_UV 50000

    float charge_current_factor = 0;
    state_t destination_state;
    static timer_t top_off_timer;
    static bool_t timers_initialized = FALSE;
    static bool_t top_off_cooldown = FALSE;

    if (!timers_initialized)
    {
        top_off_timer =
            timer_setup(6 * get_TEN_MINUTES(), FALLING);

        //
        // Stop timer from being initially true if cell voltage is
        // below max.
        //
        top_off_timer.counter = 0;
        timers_initialized = TRUE;
    }

    //
    // Set the cooldown to true if the cell voltage is higher than the
    // maximum setting.  Cooldown will remain true for one hour after
    // the maximum has droped below the set cell max.
    //
    top_off_cooldown = timer_operate(
        &top_off_timer,
        (input_data->max_cell_voltage_uv >= input_data->setting_max_charging_cell_voltage_uv) ||
        input_data->stop_charging_button);

    //
    // Calculate the current scaling factor. The intention is that the
    // current command sent to the charger gets multiplied by this
    // value, so to reduce the current entering the pack as it
    // approaches the top. It is calculated using an exponential decay
    // function, only once the max cell voltage is within a
    // predetermined threshold.
    //
    if (!output_data->charging_desired)
    {
        charge_current_factor = 0;
    }
    else if ((input_data->setting_max_charging_cell_voltage_uv -
              input_data->max_cell_voltage_uv)
             <= SLOW_CHARGING_VOLTAGE_PROX_UV)
    {
        charge_current_factor =
            (float)(pow(
                        0.1,
                        pow((input_data->max_cell_voltage_uv - (input_data->setting_max_charging_cell_voltage_uv - SLOW_CHARGING_VOLTAGE_PROX_UV)) * (-1.0 / SLOW_CHARGING_VOLTAGE_PROX_UV),
                            2)));

        //
        // Protect against overshoot
        //
        if (charge_current_factor > 1)
        {
            charge_current_factor = 1;
        }
    }
    else
    {
        charge_current_factor = 1;
    }

    *output_data =
        update_output_data(
            input_data,
            TRUE,
            CHARGING,
            TRUE,
            (output_data->enable_hv_systems && !top_off_cooldown),
            charge_current_factor);

    if (input_data->e_stop)
    {
        destination_state   = E_STOP;
    }
    else if ( !input_data->master_closed)
    {
        destination_state   = SHUTDOWN;
    }
    else if (input_data->diagnostic_mode)
    {
        destination_state   = DIAGNOSTIC_MODE;
    }
    else if (input_data->critical_fault_hv_off != 0)
    {
        destination_state   = CRITICAL_FAILURE_HV_OFF;
    }
    else if (input_data->critical_fault_hv_on != 0)
    {
        destination_state   = CRITICAL_FAILURE_HV_ON;
    }
    else if (!input_data->plugged_in)
    {
        destination_state   = POST_CHARGE_IDLE;
    }
    else
    {
        destination_state   = CHARGING;
    }

    if (destination_state != CHARGING)
    {
        top_off_timer.counter   = 0;
    }

    return destination_state;
}


/******************************************************************************
 *
 *        Name: do_state_post_charge_idle()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Friday, 16 August 2019
 *
 ******************************************************************************
 */
static state_t
do_state_post_charge_idle(
    const state_machine_input_data_t * input_data,
    state_machine_output_data_t * output_data)
{
    *output_data =
        update_output_data(
            input_data,
            TRUE,
            POST_CHARGE_IDLE,
            TRUE,
            FALSE,
            0);

    state_t destination_state;
    static bool_t ignition_off_before_on;

    ignition_off_before_on =
        set_reset(ignition_off_before_on,
                  !input_data->ignition_on, FALSE);

    if (input_data->e_stop)
    {
        destination_state = E_STOP;
    }
    else if (!input_data->master_closed)
    {
        destination_state = SHUTDOWN;
    }
    else if (input_data->diagnostic_mode)
    {
        destination_state = DIAGNOSTIC_MODE;
    }
    else if (input_data->critical_fault_hv_off != 0)
    {
        destination_state = CRITICAL_FAILURE_HV_OFF;
    }
    else if (input_data->critical_fault_hv_on != 0)
    {
        destination_state = CRITICAL_FAILURE_HV_ON;
    }
    else if (input_data->plugged_in)
    {
        destination_state = CHARGING;
    }
    else if ((input_data->ignition_on && ignition_off_before_on) || input_data->exit_post_charge_idle)
    {
        destination_state = READY_TO_DRIVE;
    }
    else
    {
        destination_state = POST_CHARGE_IDLE;
    }

    if (destination_state != POST_CHARGE_IDLE)
    {
        ignition_off_before_on = FALSE;
    }

    return destination_state;
}


/******************************************************************************
 *
 *        Name: do_state_brake_test_mode()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Friday, 16 August 2019
 *
 ******************************************************************************
 */
static state_t
do_state_brake_test_mode(
    const state_machine_input_data_t *input_data,
    state_machine_output_data_t *output_data)
{
    *output_data =
        update_output_data(
            input_data, TRUE,
            BRAKE_TEST_MODE, TRUE, FALSE, 0);

    state_t destination_state;

    if (input_data->e_stop)
    {
        destination_state = E_STOP;
    }
    else if ( !input_data->master_closed || !input_data->ignition_on)
    {
        destination_state = SHUTDOWN;
    }
    else if (input_data->diagnostic_mode)
    {
        destination_state = DIAGNOSTIC_MODE;
    }
    else if (input_data->critical_fault_hv_off != 0)
    {
        destination_state = CRITICAL_FAILURE_HV_OFF;
    }
    else if (input_data->critical_fault_hv_on != 0)
    {
        destination_state = CRITICAL_FAILURE_HV_ON;
    }
    else if ( !input_data->brake_test_mode)
    {
        destination_state = READY_TO_DRIVE;
    }
    else
    {
        destination_state   = BRAKE_TEST_MODE;
    }

    return destination_state;
}


/******************************************************************************
 *
 *        Name: do_state_diagnostic_mode()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Friday, 16 August 2019
 *
 ******************************************************************************
 */
static state_t
do_state_diagnostic_mode(
    const state_machine_input_data_t *input_data,
    state_machine_output_data_t *output_data)
{
    *output_data =
        update_output_data(
            input_data, FALSE,
            DIAGNOSTIC_MODE, TRUE, FALSE, 0);

    state_t destination_state;

    if (input_data->e_stop)
    {
        destination_state = E_STOP;
    }
    else if ( !input_data->master_closed || !input_data->diagnostic_mode)
    {
        destination_state = ZERO_ENERGY;
    }
    else
    {
        destination_state = DIAGNOSTIC_MODE;
    }

    return destination_state;
}


/******************************************************************************
 *
 *        Name: do_state_ready_to_drive()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Friday, 16 August 2019
 *
 ******************************************************************************
 */
static state_t
do_state_ready_to_drive(
    const state_machine_input_data_t *input_data,
    state_machine_output_data_t *output_data)
{
    *output_data =
        update_output_data(
            input_data, TRUE,
            READY_TO_DRIVE, FALSE, FALSE, 0);

    state_t destination_state;

    if (input_data->e_stop)
    {
        destination_state = E_STOP;
    }
    else if ( !input_data->master_closed || !input_data->ignition_on)
    {
        destination_state = SHUTDOWN;
    }
    else if (input_data->diagnostic_mode)
    {
        destination_state = DIAGNOSTIC_MODE;
    }
    else if (input_data->critical_fault_hv_off != 0)
    {
        destination_state = CRITICAL_FAILURE_HV_OFF;
    }
    else if (input_data->critical_fault_hv_on != 0)
    {
        destination_state = CRITICAL_FAILURE_HV_ON;
    }
    else if (input_data->plugged_in)
    {
        destination_state = CHARGING;
    }
    else if (input_data->brake_test_mode)
    {
        destination_state = BRAKE_TEST_MODE;
    }
    else
    {
        destination_state = READY_TO_DRIVE;
    }

    return destination_state;
}


/******************************************************************************
 *
 *        Name: do_state_tow_mode()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Friday, 16 August 2019
 *
 ******************************************************************************
 */
static state_t
do_state_tow_mode(
    const state_machine_input_data_t *input_data,
    state_machine_output_data_t *output_data)
{
    *output_data =
        update_output_data(
            input_data, TRUE,
            TOW_MODE, FALSE, FALSE, 0);

    state_t destination_state;

    if (input_data->e_stop)
    {
        destination_state = E_STOP;
    }
    else if (!input_data->master_closed ||
             !input_data->ignition_on)
    {
        destination_state = SHUTDOWN;
    }
    else if (input_data->diagnostic_mode)
    {
        destination_state = DIAGNOSTIC_MODE;
    }
    else if (input_data->critical_fault_hv_off != 0)
    {
        destination_state = CRITICAL_FAILURE_HV_OFF;
    }
    else if (input_data->critical_fault_hv_on != 0)
    {
        destination_state = CRITICAL_FAILURE_HV_ON;
    }
    else if (input_data->plugged_in)
    {
        destination_state = CHARGING;
    }
    else
    {
        destination_state   = TOW_MODE;
    }

    return destination_state;
}


/******************************************************************************
 *
 *        Name: do_state_dead_battery()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Friday, 16 August 2019
 *
 ******************************************************************************
 */
static state_t
do_state_dead_battery(
    const state_machine_input_data_t *input_data,
    state_machine_output_data_t *output_data)
{
    *output_data =
        update_output_data(
            input_data, TRUE,
            DEAD_BATTERY, TRUE, FALSE, 0);

    state_t destination_state;

    if (input_data->e_stop)
    {
        destination_state = E_STOP;
    }
    else if ( !input_data->master_closed || !input_data->ignition_on)
    {
        destination_state = SHUTDOWN;
    }
    else if (input_data->diagnostic_mode)
    {
        destination_state = DIAGNOSTIC_MODE;
    }
    else if (input_data->critical_fault_hv_off != 0)
    {
        destination_state = CRITICAL_FAILURE_HV_OFF;
    }
    else if (input_data->critical_fault_hv_on != 0)
    {
        destination_state = CRITICAL_FAILURE_HV_ON;
    }
    else if (input_data->plugged_in)
    {
        destination_state = CHARGING;
    }
    else
    {
        destination_state = DEAD_BATTERY;
    }

    return destination_state;
}


/******************************************************************************
 *
 *        Name: do_state_critical_failure_hv_off()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Friday, 16 August 2019
 *
 ******************************************************************************
 */
static state_t
do_state_critical_failure_hv_off(
    const state_machine_input_data_t *input_data,
    state_machine_output_data_t *output_data)
{
    *output_data =
        update_output_data(
            input_data, FALSE,
            CRITICAL_FAILURE_HV_OFF, TRUE, FALSE, 0);

    state_t destination_state;
    static timer_t failure_shutdown_timer;
    static bool_t timers_initialized = FALSE;
    static bool_t capture_failure_reason = TRUE;
    static uint32_t reasons_for_failure = 0;
    static uint8_t can_slowdown = 5;

    if ( !timers_initialized)
    {
        failure_shutdown_timer =
            timer_setup(get_TEN_SECONDS(), RISING);
        timers_initialized = TRUE;
    }

    if (capture_failure_reason)
    {
        reasons_for_failure =
            input_data->critical_fault_hv_off;
        capture_failure_reason = FALSE;
    }

    if (can_slowdown >= 5)
    {
        Can_Message_ crit_fail_message;

        crit_fail_message.identifier = 0x11CC33FF;
        crit_fail_message.length = 8;
        crit_fail_message.type = EXTENDED;
        crit_fail_message.data[0] = 0;
        crit_fail_message.data[1] = 0;
        crit_fail_message.data[2] = 0;
        crit_fail_message.data[3] = 0;
        crit_fail_message.data[4] = (uint8_t)((reasons_for_failure >> 24) & 0xFF);
        crit_fail_message.data[5] = (uint8_t)((reasons_for_failure >> 16) & 0xFF);
        crit_fail_message.data[6] = (uint8_t)((reasons_for_failure >> 8) & 0xFF);
        crit_fail_message.data[7] = (uint8_t)((reasons_for_failure >> 0) & 0xFF);

        Send_CAN_Message(0, CAN1, crit_fail_message);
        Send_CAN_Message(0, CAN3, crit_fail_message);
        can_slowdown = 0;
    }
    can_slowdown++;

    if (input_data->e_stop)
    {
        destination_state = E_STOP;
    }
    else if ( !input_data->master_closed || !input_data->ignition_on)
    {
        destination_state = ZERO_ENERGY;
    }
    else if (timer_operate(&failure_shutdown_timer, TRUE))
    {
        destination_state = ZERO_ENERGY;
    }
    else
    {
        destination_state = CRITICAL_FAILURE_HV_OFF;
    }

    if (destination_state != CRITICAL_FAILURE_HV_OFF)
    {
        timer_operate(&failure_shutdown_timer, FALSE);

        capture_failure_reason  = TRUE;
    }

    return destination_state;
}



/******************************************************************************
 *
 *        Name: do_state_critical_failure_hv_on()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Friday, 16 August 2019
 *
 ******************************************************************************
 */
static state_t
do_state_critical_failure_hv_on(
    const state_machine_input_data_t *input_data,
    state_machine_output_data_t *output_data)
{
    *output_data =
        update_output_data(
            input_data, TRUE,
            CRITICAL_FAILURE_HV_ON, TRUE, FALSE, 0);

    state_t destination_state;
    static bool_t capture_failure_reason = TRUE;
    static uint32_t reasons_for_failure = 0;
    static uint8_t can_slowdown = 5;

    if (capture_failure_reason)
    {
        reasons_for_failure = input_data->critical_fault_hv_on;
        capture_failure_reason  = FALSE;
    }

    if (can_slowdown >= 5)
    {
        Can_Message_ crit_fail_message;

        crit_fail_message.identifier = 0x11CC33FF;
        crit_fail_message.length = 8;
        crit_fail_message.type = EXTENDED;
        crit_fail_message.data[0] = (uint8_t)((reasons_for_failure >> 24) & 0xFF);
        crit_fail_message.data[1] = (uint8_t)((reasons_for_failure >> 16) & 0xFF);
        crit_fail_message.data[2] = (uint8_t)((reasons_for_failure >> 8) & 0xFF);
        crit_fail_message.data[3] = (uint8_t)((reasons_for_failure >> 0) & 0xFF);
        crit_fail_message.data[4] = 0;
        crit_fail_message.data[5] = 0;
        crit_fail_message.data[6] = 0;
        crit_fail_message.data[7] = 0;

        Send_CAN_Message(0, CAN1, crit_fail_message);
        Send_CAN_Message(0, CAN3, crit_fail_message);
        can_slowdown = 0;
    }
    can_slowdown++;

    if (input_data->e_stop)
    {
        destination_state = E_STOP;
    }
    else if ( !input_data->master_closed || !input_data->ignition_on)
    {
        destination_state = SHUTDOWN;
    }
    else if (input_data->diagnostic_mode)
    {
        destination_state = DIAGNOSTIC_MODE;
    }
    else if (input_data->critical_fault_hv_off != 0)
    {
        destination_state = CRITICAL_FAILURE_HV_OFF;
    }
    else if (input_data->critical_fault_hv_on == 0)
    {
        destination_state = READY_TO_DRIVE;
    }
    else
    {
        destination_state = CRITICAL_FAILURE_HV_ON;
    }

    return destination_state;
}


/******************************************************************************
 *
 *        Name: do_state_e_stop()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Friday, 16 August 2019
 *
 ******************************************************************************
 */
static state_t
do_state_e_stop(
    const state_machine_input_data_t *input_data,
    state_machine_output_data_t *output_data)
{
    *output_data =
        update_output_data(
            input_data, FALSE,
            E_STOP, TRUE, FALSE, 0);

    state_t destination_state;

    if (!input_data->e_stop)
    {
        destination_state = ZERO_ENERGY;
    }
    else if (!input_data->master_closed || !input_data->ignition_on)
    {
        destination_state = ZERO_ENERGY;
    }
    else
    {
        destination_state = E_STOP;
    }

    return destination_state;
}


/******************************************************************************
 *
 *        Name: do_state_shutdown()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Friday, 16 August 2019
 *
 ******************************************************************************
 */
static state_t
do_state_shutdown(
    const state_machine_input_data_t *input_data,
    state_machine_output_data_t *output_data)
{
    state_t destination_state;
    static timer_t shutdown_timer;
    static bool_t timers_initialized  = FALSE;

    if ( !timers_initialized)
    {
        shutdown_timer =
            timer_setup(get_ONE_AND_A_HALF_SECONDS(), RISING);
        timers_initialized  = TRUE;
    }

    *output_data =
        update_output_data(
            input_data, TRUE,
            SHUTDOWN, TRUE, FALSE, 0);

    if (input_data->e_stop)
    {
        destination_state = ZERO_ENERGY;
    }
    else if (timer_operate(&shutdown_timer, TRUE))
    {
        destination_state = ZERO_ENERGY;
    }
    else
    {
        destination_state = SHUTDOWN;
    }

    //
    // If we are exiting the shutdown state, reset the timer to the
    // full period and reset shutdown_complete to false (we have
    // already set the flag to leave, so next time we enter shutdown
    // from another state, the timer responds properly.
    //
    if (destination_state != SHUTDOWN)
    {
        timer_operate(&shutdown_timer, FALSE);
    }

    return destination_state;
}



/******************************************************************************
 *
 *        Name: run_state_machine()
 *
 * Description: This is the main function for the state machine,
 *              called by the external loop. It must receive a
 *              populated input_data structure.
 *
 *      Author: Tom
 *        Date: Friday, 16 August 2019
 *
 ******************************************************************************
 */
void
run_state_machine()
{
    //
    // Declare all state functions as an array of functions, in the
    // same order as the state_t enumeration must line up with state_t
    // enumeration.
    //
    static const state_function_t state_table[NUM_STATES] =
    {
        do_state_zero_energy,
        do_state_master_closed_no_hv,
        do_state_startup,
        do_state_charging,
        do_state_post_charge_idle,
        do_state_brake_test_mode,
        do_state_diagnostic_mode,
        do_state_ready_to_drive,
        do_state_tow_mode,
        do_state_dead_battery,
        do_state_critical_failure_hv_off,
        do_state_critical_failure_hv_on,
        do_state_e_stop,
        do_state_shutdown
    };

//    static state_machine_output_data_t output_data;
    static state_t current_state = ZERO_ENERGY;

    static bool_t first_run = TRUE;

    static timer_t crit_fail_hv_off_timer;
    static timer_t crit_fail_hv_on_timer;

    static bool_t crit_fail_hv_off_debounce = FALSE;
    static bool_t crit_fail_hv_on_debounce = FALSE;

    //
    // Initialize timers.
    //
    if (first_run)
    {
        first_run = FALSE;

        crit_fail_hv_off_timer =
            timer_setup(get_FIVE_SECONDS(), RISING);

        crit_fail_hv_on_timer =
            timer_setup(get_FIVE_SECONDS(), RISING);
    }

    //
    // Some code to make sure that the incoming value has a fault for
    // five seconds before a fault is triggered.
    //
    crit_fail_hv_off_debounce =
        timer_operate(&crit_fail_hv_off_timer,
                      sm_input_data.critical_fault_hv_off != 0);

    crit_fail_hv_on_debounce =
        timer_operate(&crit_fail_hv_on_timer,
                      sm_input_data.critical_fault_hv_on != 0);

    //
    // If the timer hasn't gone true yet, set the values to 0 to
    // ignore the fault. Otherwise, the timer has run and the value
    // shall remain.
    //
    if (!crit_fail_hv_off_debounce)
    {
        sm_input_data.critical_fault_hv_off = 0;
    }
    if (!crit_fail_hv_on_debounce)
    {
        sm_input_data.critical_fault_hv_on = 0;
    }

    //
    // The desired order of operations for this state machine is:
    //
    // 1) Run the state function for the current state, and set
    // sm_output_data accordingly.
    //
    // 2) Capture and store the next state, which will be used on the
    // next state machine call.
    //
    // 3) Allow the rest of the vehicle to respond to the setting of
    // sm_output_data.
    //
    // 4) Run the state machine again with the new state (step 1
    // again).
    //
    // Decide here if high voltage is desired.
    //
    // Run the function for the current state and capture the result,
    // which is the next state. The new state will give the state
    // function called on the next iteration of this function.
    //
    state_t new_state =
        state_table[current_state](&sm_input_data, &sm_output_data);

    // Capture the new state for the next iteration of the state
    // machine.
    current_state = new_state;

    //
    // Allow vehicle to respond to the setting of sm_output_data
    // (including the state that was just run).
    //
    // return sm_output_data;
}


/*
 * this function contains the precharging algorithm.
 * power_up is used to indicate the desire for high voltage
 * once the dc_link_voltage is close to the battery voltage, the precharge is done.
 */


/******************************************************************************
 *
 *        Name: update_output_data()
 *
 * Description: This function is to ensure that all output data is
 *              updated completely in every single state function. It
 *              should be called from every state function, and each
 *              parameter must be correctly filled out.
 *
 *      Author: Tom
 *        Date: Friday, 16 August 2019
 *
 ******************************************************************************
 */
static state_machine_output_data_t
update_output_data(
    const state_machine_input_data_t *input_data,
    const bool_t hv_desired,
    const state_t current_state,
    const bool_t brakes_desired,
    const bool_t charging_desired,
    const float charge_current_factor)
{
    state_machine_output_data_t data;

    // Slight delay on turning off high voltage when shutting down.
    static timer_t hv_turning_off_timer;
    // Boolean to capture the delay on shutting down.
    static bool_t hv_desired_delay = FALSE;
    // Boolean for initializing timers frst run.
    static bool_t first_run_init = TRUE;

    if (first_run_init)
    {
        hv_turning_off_timer =
            timer_setup(get_FIVE_HUNDRED_MSECS(), FALLING);
        first_run_init = FALSE;
    }

    //
    // Get the result of the high voltage delay timer.
    //
    hv_desired_delay = timer_operate(&hv_turning_off_timer, hv_desired);

    //
    // Populate all the data in the output_data structure.
    //
    data.current_state = current_state;
    data.brakes_desired = brakes_desired;
    data.charging_desired = charging_desired;
    data.charge_current_factor = charge_current_factor;

    data.contactors =
        contactor_control(
            hv_desired_delay,
            input_data->in_neutral,
            (abs(input_data->traction_rpm) > 5),
            input_data->dc_bus_voltage_v,
            input_data->battery_voltage_v);

    data.enable_hv_systems =
        (data.contactors.precharge_success &&
         hv_desired && current_state != SHUTDOWN);

    return data;
}


/******************************************************************************
 *
 *        Name: contactor_control()
 *
 * Description: This function controls the power up sequence of the
 *              vehicle and closes contactors are required. The
 *              boolean power_up informs the function if high voltage
 *              is desired by the system. If it is, precharge will
 *              begin and contactors will close. If it is false,
 *              contactors will open right away.
 *
 *      Author: Cyrus
 *        Date: Friday, 16 August 2019
 *
 ******************************************************************************
 */
static state_machine_contactor_status_t contactor_control(bool_t	power_up,
		bool_t														in_neutral,
		bool_t														in_motion,
		uint16_t													dc_link_voltage,
		uint16_t													battery_voltage)
{
#define PRECHARGE_COMPLETE_THRESHOLD_V 25
#define MINIMUM_DC_LINK_V 10
	static bool_t							safe_startup		=
		FALSE;		// boolean to make sure vehicle is in neutral before closing contactors
	static bool_t							prechargeComplete	=
		FALSE;		// boolean that determines that the precharge has completed successfully
	static timer_t						delay_open_precharge;
	static timer_t						delay_close_precharge;
	static timer_t						precharge_timer;
	static timer_t						precharge_fail_timer;
	static timer_t						system_wakeup;
	static bool_t							timers_initialized	= FALSE;
	static state_machine_contactor_status_t	contactor_command;

	// timer temp variables to force the execution of the timer_operate function inside a boolean evaluation
	bool_t	timerTemp1;
	bool_t	timerTemp2;
	bool_t	timerTemp3;

	if ( !timers_initialized)
	{
		delay_open_precharge	= timer_setup(get_ONE_HUNDRED_MSECS(), RISING);
		delay_close_precharge	= timer_setup(get_TWO_HUNDRED_FIFTY_MSECS(), RISING);
		precharge_timer			= timer_setup(get_FIFTEEN_SECONDS(), PULSE);
		precharge_fail_timer	= timer_setup(precharge_timer.period, RISING);
		system_wakeup			= timer_setup(get_TWO_HUNDRED_FIFTY_MSECS(), RISING);
		timers_initialized		= TRUE;
	}

	// If vehicle is in neutral and the contactors are open, this will be true. It allows the vehicle to transition from off to on state with shifter in neutral
	safe_startup	= set_reset(
						  safe_startup,
						  in_neutral && !in_motion,
						  ( !contactor_command.neg_contactor || !contactor_command.pos_contactor));

	// determine state of negative contactors                                        //close the negative contactor if:
	contactor_command.neg_contactor	= power_up										// if high voltage is desired
									  && (dc_link_voltage >= MINIMUM_DC_LINK_V)		// and there is no load on the DC bus
									  && !contactor_command.precharge_failure		// and the precharge hasn't failed
									  && safe_startup;								// and the shifter is in neutral before precharge is initiated

	// precharge is complete if the DC link voltage has made it to within 25V of the battery voltage
	prechargeComplete	= set_reset(
							  prechargeComplete,
							  (dc_link_voltage > (battery_voltage - PRECHARGE_COMPLETE_THRESHOLD_V)),
							  !power_up);
	timerTemp1	= timer_operate(
					  &delay_open_precharge,
					  prechargeComplete);		// open precharge slightly after precharge is complete (to allow + contactor to have closed)
	timerTemp2	= timer_operate(
					  &precharge_timer,
					  contactor_command.
						  neg_contactor);		// close precharge contactor as a pulse when negative contactor is closed
	timerTemp3	= timer_operate(
					  &delay_close_precharge,
					  contactor_command.
						  neg_contactor);		// close precharge contactor slightly after positive contactor is closed
	contactor_command.
		pre_contactor = (timerTemp2 && timerTemp3
						 && contactor_command.neg_contactor		// only close precharge if negative contactor is closed
									   && safe_startup				// and vehicle is in neutral on startup
									   && power_up					// and high voltage is desired
									   && in_neutral				// and vehicle is currently in neutral
									   && !timerTemp1
									   && !contactor_command.
										   precharge_failure);		// and there hasn't been a precharge failure
	timerTemp1	= timer_operate(
					  &precharge_fail_timer,
					  contactor_command.neg_contactor);				// timer to see if precharge doesn't complete successfully
	
	contactor_command.precharge_failure	= set_reset(
			contactor_command.precharge_failure,
			timerTemp1 && !prechargeComplete,
			!power_up);		// failure if timers expires and vehicle has not completed precharge

	// logic for closing positive contactor
	contactor_command.pos_contactor	= (!in_neutral
			|| prechargeComplete
			|| contactor_command.pre_contactor)
	        && power_up
	        && safe_startup
	        && contactor_command.neg_contactor
	        && prechargeComplete;

	// ignition is complete if positive and negative contactors are closed
	contactor_command.precharge_success	= timer_operate(
			&system_wakeup,
			(power_up && contactor_command.neg_contactor && contactor_command.pos_contactor));

	return contactor_command;
}


/******************************************************************************
 *
 *        Name: populate_state_machine_member_elements()
 *
 * Description: A function that populates all the state machine input
 *              member elements. This function is called from userApp
 *              just before
 *
 *      Author: Deepak
 *        Date: Saturday, 07 September 2019
 *
 ******************************************************************************
 */
void
populate_state_machine_member_elements()
{

    static bool_t first_run = TRUE;

    //
    // All the member elements of the input data struct has to be
    // populated before running the state machine.
    //
    //
    // master_closed: the master on the vehilce is a red knife switch.
    //                It is an input to the CVC
    sm_input_data.master_closed = (IN_E11_master_switch_signal == INPUT_ON);

    //
    // estop: estops are palced at various places on vehicle. If an
    //        Estop is presed, it signifies an emergency. The estop
    //        output signals are connected as inputs to the CVC.
    //
    sm_input_data.e_stop =
        !(IN_A16_dash_e_stop_signal == INPUT_ON) ||
        !(IN_A17_charge_box_e_stop_signal == INPUT_ON);

    //
    // Ignition_on: Set ignition, which is like they key switch being
    // on and signals the desire for high voltage to be active.
    //
    sm_input_data.ignition_on = (IN_A11_keyswitch > 8000) || (low_power_mode != TRUE);

    //
    // plugged_in: identifies of the charger is plugged_in
    //
    sm_input_data.plugged_in = detect_charge_handle();

    //
    // in_neutral: check to shifter is in neutal
    //
    shifter_position_t shifter_pos =
        get_shifter_direction();

    sm_input_data.in_neutral =
        (shifter_pos == SHIFTER_NEUTRAL);

    //
    // critical_fault_hv_off: shuts off the high voltage if a critical
    // fault has occured.
    //
    sm_input_data.critical_fault_hv_off =
    		check_critical_fault_high_voltage_off();

    //
    // critical_fault_hv_on: a critical fault has occured. But leave
    // the high voltage ON.
    //
    sm_input_data.critical_fault_hv_on =
    		check_critical_fault_high_voltage_on();

    //
    // stop_charging_button: An input to the CVC from the charging box.
    //
    sm_input_data.stop_charging_button =
        (bool_t)IN_A15_stop_charging_button_signal;

    //
    // brake_test_mode: does not exist in this machine, therfore kept
    // it FALSE.
    //
    sm_input_data.brake_test_mode = FALSE;

    //
    // diagnostic_mode:
    //
    sm_input_data.diagnostic_mode =
        (bool_t)IN_A18_diagnositic_switch_signal;

    //
    // exit_post_charge_idle:
    //
    // Exit post charge idle state and go to Ready to drive only of the iginition
    // switch is ON and the sifter is in neutral
    // When vehilce is in neutral and switch1 is ON
    sm_input_data.exit_post_charge_idle = 
        ((shifter_pos == SHIFTER_NEUTRAL) && (IN_A11_keyswitch > 8000));

    //
    // max_cell_voltage_in_micro_volts: Orion BMS
    //
    sm_input_data.max_cell_voltage_uv =
        get_pack_high_cell_voltage();

    //
    // min_cell_voltage_in_micro_volts: Orion BMS
    //
    sm_input_data.min_cell_voltage_uv =
        get_pack_low_cell_voltage();

    //
    // battery_soc: Orion BMS
    //
    sm_input_data.battery_soc =
        get_battery_pack_SOC();

    //
    // dc_bus_voltage: Skai2 Traction Inverter
    //
    sm_input_data.dc_bus_voltage_v =
    		skai_get_vissim_DCLink_Voltage(ONE);

    //
    // battery_voltage: Orion BMS
    //
    sm_input_data.battery_voltage_v =
        (uint16_t)(get_battery_pack_voltage() / 10);

    //
    // traction_rpm: rpm from the traction drive.
    //
    sm_input_data.traction_rpm =
        (int16_t)skai_get_vissim_motor_rpm(ONE);

    //
    // setting_max_charging_cell_voltage_uv: EEPROM VALUE
    //
    sm_input_data.setting_max_charging_cell_voltage_uv =
        MAX_CHARGING_CELL_VOLTAGE;

}

//=============================================================================
//
// STATE MACHINE OUTPUT GETTERS
//
//=============================================================================
//
state_t get_sm_current_state()
{

    //
    // Return the current state of the state machine.
    //
    return sm_output_data.current_state;

}

bool_t get_sm_status_brakes_desired()
{

    //
    // Returns the state of brakes desired
    //
    return sm_output_data.brakes_desired;

}

bool_t get_sm_status_charging_desired()
{

    //
    // Returns the state of charging desired
    //
    return sm_output_data.charging_desired;

}

bool_t get_sm_status_enable_hv_systems()
{

    //
    // Returns the state of high voltage on the system
    //
    return sm_output_data.enable_hv_systems;

}

float get_sm_charge_current_factor()
{

    //
    // return charge current factor
    //
    return sm_output_data.charge_current_factor;

}

bool_t get_sm_pos_contactor_status()
{

    //
    // return the contactor status
    //
    return sm_output_data.contactors.pos_contactor;

}

bool_t get_sm_neg_contactor_status()
{

    //
    // return the contactor status
    //
    return sm_output_data.contactors.neg_contactor;

}

bool_t get_sm_pre_contactor_status()
{

    //
    // return the contactor status
    //
    return sm_output_data.contactors.pre_contactor;

}

bool_t get_sm_precharge_failure_status()
{

    //
    // return the contactor status
    //
    return sm_output_data.contactors.precharge_failure;

}

bool_t get_sm_precharge_success_status()
{

    //
    // return the contactor status
    //
    return sm_output_data.contactors.precharge_success;

}


//=============================================================================
//
// State Machine setters
//
//=============================================================================
//
void set_state_machine_state(state_t state)
{

    sm_output_data.current_state = state;
}

void set_positive_contactor(bool_t state)
{
    sm_output_data.contactors.pos_contactor = state;
}

void set_negative_contactor(bool_t state)
{
    sm_output_data.contactors.neg_contactor = state;
}

void set_pre_contactor(bool_t state)
{
    sm_output_data.contactors.pre_contactor = state;
}
