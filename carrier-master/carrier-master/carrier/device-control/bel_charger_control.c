/******************************************************************************
 *
 *        Name: bel_charger_control.c
 *
 * Description: The function controls the working of the Bel Charger
 *              based on various inputs that are obtained from the
 *              state machine and the BMS.
 *
 *      Author: Deepak
 *        Date: Friday, 06 September 2019
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
#include "orion_control.h"
#include "timer_service.h"
#include "fvt_library.h"
#include "bel_charger_device.h"
#include "bel_charger_control.h"
#include "can_switches.h"
#include "state_machine.h"
#include "cvc_debug_msgs.h"

#define EEVAR_max_allowable_charging_cell_voltage 40400
#define EEVAR_battery_over_voltage_limit 840 * 20
#define EEVAR_battery_under_voltage_limit 450 * 20

#define ZENER_VOLTAGE_LIMIT_MV 4000

//=============================================================================
//
// Static Timers
//
//=============================================================================
//
static timer_t top_of_pack_timer;
static timer_t delay_begin_charging;
static timer_t delay_open_ac_contactor;
static timer_t charge_enable_timer;

static bool_t charging_desired = FALSE;
static bool_t charge_button_state = FALSE;


static bool_t get_charge_button_state();

//=============================================================================
//
// Static Function Prototypes
//
//=============================================================================
//
static float get_charge_current_factor(
    uint16_t max_cell_voltage,
    bool_t charging_desired);


/******************************************************************************
 *
 *        Name: bel_charger_control()
 *
 * Description: A function that enables/disables the bel charger based
 *              on various conditions it receives from the state
 *              machine and BMS.
 *
 *        Date: Friday, 06 September 2019
 *
 ******************************************************************************
 */
void bel_charger_control(
    device_instances_t device)
{

    static bool_t first_pass = TRUE;

    bool_t wake_up_ecu_enable = FALSE;
    bool_t gpo_enable = FALSE;


    if(first_pass == TRUE)
    {

        //
        // Initialisation of timers necessary for the operation of the bel
        // chargers.
        //

        // top_of_pack_timer = timer_setup(45 * get_ONE_MINUTE(), RISING);

        top_of_pack_timer = timer_setup(get_TEN_SECONDS(), RISING);

        delay_begin_charging = timer_setup(get_FIVE_HUNDRED_MSECS(), RISING);

        delay_open_ac_contactor = timer_setup(get_ONE_SECOND(), FALLING);

        charge_enable_timer = timer_setup(get_ONE_SECOND(), RISING);

    //
    // set first pass to false.
    //
    first_pass = FALSE;

    }

    //
    // A boolean that determines if the cell voltage is below the
    // maximum or not.
    //
    static bool_t cell_voltage_ok = FALSE;

    //
    // A boolean that determines if the cell voltage is below the
    // maximum at the time the vehicle is powered up
    //
    static bool_t cell_voltage_ok_on_startup = TRUE;

    //
    // A value that is calculated dynamically based on the highcell
    // voltage / battery pack voltage. This is done, to reduce the
    // current flowing into the battery pack as the cells charge up to
    // their max value.
    //
    float charge_current_factor = 0;

    //
    // A bool to determine if charging is ready to begin.
    //
    bool_t ready_to_charge;

    //
    // A bool that stores the result to enable/disable the charger.
    //
    bool_t enable_charger;

    //
    // Timer temp variables are used to force timers to run (sometimes
    // won't run if they're in AND statements)
    //
    bool_t timer_temp;

    //
    // max_cell_voltage from the BMS
    //
    uint16_t pack_high_cell_voltage =  get_pack_high_cell_voltage();

    bool_t high_voltage_enabled = get_sm_status_enable_hv_systems();
    bool_t charging_mode = (get_sm_current_state() == CHARGING) && high_voltage_enabled;

    //
    // charging desired gets its input from the state machine
    //
    charging_desired = (high_voltage_enabled && charging_mode);

    //
	// This flip flop can never be set. cellVoltageOKonStartup is
	// initially TRUE. Once the maxCellVoltage is above the
	// maxChargingCellVoltage for the first time, it flops to FALSE
	// and will remain FALSE until the program is restarted
    //
	cell_voltage_ok_on_startup =
        reset_set(cell_voltage_ok_on_startup,
                  FALSE,
                  pack_high_cell_voltage >= EEVAR_max_allowable_charging_cell_voltage);

    //
    // If the cell voltage is more than 0.01 V below the maximum, it
	// is OK to charge.  Hysteresis prevents rapid on-off if the
	// voltage is just on the cusp.
    //
    cell_voltage_ok =
        (EEVAR_max_allowable_charging_cell_voltage >= pack_high_cell_voltage);

    //
	// If the cell voltage is OK, wait 45 minutes before enabling.
    //
    timer_temp = timer_operate(&top_of_pack_timer,
                               cell_voltage_ok);

    //
	// ready_to_charge is what enables the charger and AC Contactor.
    //
	ready_to_charge = (cell_voltage_ok
                        && charging_desired
                        && (timer_temp || cell_voltage_ok_on_startup));


    //
    // Once we're ready to charge, wait 500ms before enabling
    // charger for AC contactor to close.
    //
	enable_charger = timer_operate(&delay_begin_charging,
                                   ready_to_charge);

    //
    // AC Contactor closes right away when charger is enabled, and is
    // opened 1 second after the charger is not enabled.
    //
    timer_temp = timer_operate(&delay_open_ac_contactor,
                               enable_charger);

    //
    // Calculate the charge current factor
    //
    charge_current_factor = get_charge_current_factor(
        pack_high_cell_voltage,
        charging_desired);

    //
    // Calculate the current command of the charger
    //
    uint16_t current_command =
        (uint16_t)((EEVAR_CHARGER_total_current_command * charge_current_factor) / EEVAR_CHARGER_no_of_chargers);

    //
    // Will be used only if the IO on the bel charge control
    // controls a pump
    //
    if(charging_desired == TRUE)
    {
        wake_up_ecu_enable = TRUE;
        gpo_enable = TRUE;
    }
    else
    {
        wake_up_ecu_enable = FALSE;
        gpo_enable = FALSE;
    }


    float no_of_cells = 174.0;
    float max_cell_vol = 4.04;

    uint16_t  battery_voltage_limit = (uint16_t)((no_of_cells * max_cell_vol) * 20);


    //=============================================================================
    //
    // SETTERS
    //
    //=============================================================================
    //
    //
    // Set the Bell Charger Set Point Structure
    //
    bel_set_setpoint(
        device,
        (current_command * 20),
         battery_voltage_limit,
         charging_desired);

    //
    // Set the CAN message responsible for the IO control on the bel
    // charger.
    //
    bel_set_gpio(
        device,
        wake_up_ecu_enable,
        gpo_enable);

    //
    // Set the CAN message responsible for setting the upper and lower
    // voltage limits of operation for the charger.
    //
    bel_set_battery_under_over_voltage_limits(
        device,
        EEVAR_battery_under_voltage_limit,
        EEVAR_battery_over_voltage_limit);

    bel_set_led_duty_cycle(
        device,
        100);


    //=============================================================================
    //
    // TRANSMIT MESSAGES
    //
    //=============================================================================
    //
    //
    // Transmit Can Messages
    //
    bel_tx_can_setpoint(device, EVERY_1000MS);

    bel_tx_can_gpio(device, EVERY_1000MS);

    bel_tx_can_battery_under_over_voltage_limits(
        device,
        EVERY_1000MS);

    bel_tx_can_led_duty_cycle(
        device,
        EVERY_100MS);



}


/******************************************************************************
 *
 *        Name: get_charge_current_factor()
 *
 * Description: Calculte the charge current factor if the voltage
 *              difference between the max_cell_voltage from the bms
 *              and the EEPROM_max_allowable_charging_cell_voltage is
 *              less than 0.5V.  The formula would decrease the output
 *              of current to zero as the max cell voltage reaches the
 *              EEVAR_max_allowable_charging_cell_voltage eeprom set
 *              limit.
 *
 *
 *        Date: Friday, 06 September 2019
 *
 ******************************************************************************
 */
static float get_charge_current_factor(
    uint16_t max_cell_voltage,
    bool_t charging_desired)
{

    //
    // set the slowdown voltage within 0.1V of the set maximum cell
    // voltage.
    //
    static uint16_t SLOW_CHARGING_VOLTAGE_PROX_UV_VAR  = 500;

	float charge_current_factor	= 0;

    //
	// calculate the current scaling factor. the intention is that the
	// current command sent to the charger gets multiplied by this
	// value, so to reduce the current entering the pack as it
	// approaches the top.  it is calculated using an exponential
	// decay function, only once the max cell voltage is within a
	// predetermined threshold
    //
    if (!charging_desired)
	{

        //
        // If charging is disabled, set charge_current_factor to
        // zero. Basically setting the current to zero.
        //
		charge_current_factor	= 0;

    }
	else if ((EEVAR_max_allowable_charging_cell_voltage - max_cell_voltage)
			 <= SLOW_CHARGING_VOLTAGE_PROX_UV_VAR)
	{

        //
        // calculte the charge current factor if the voltage
        // difference between the max_cell_voltage from the bms and
        // the EEPROM_max_allowable_charging_cell_voltage is less than
        // 0.5V.  The formula would decrease the output of current to
        // zero as the max cell voltage reaches the
        // EEVAR_max_allowable_charging_cell_voltage eeprom set limit.
        //
		charge_current_factor	= (float)(pow(0.1,pow((max_cell_voltage
                                                       - (EEVAR_max_allowable_charging_cell_voltage - SLOW_CHARGING_VOLTAGE_PROX_UV_VAR))
                                                      * (-1.0 / SLOW_CHARGING_VOLTAGE_PROX_UV_VAR), 2)));
        //
		// protect against overshoot
        //
		if (charge_current_factor > 1)
		{

            //
            // force charge factor value to one if the calculation
            // results to a value greater than 1.
            //
			charge_current_factor	= 1;

        }

	}
	else
	{

        //
        // the difference between max_cell_voltage and
        // EEVAR_max_allowable_charging_cell_voltage is not under
        // 0.5V.  Set limit to 1. This indicates max current (set in
        // the EEPROM) is delivered to the battery pack.
        //
		charge_current_factor	= 1;

	}

    //
    // return the charger current factor
    //
	return charge_current_factor;
}

//=============================================================================
//
// charger_enabled_state()
//
//=============================================================================
//
bool_t charger_enabled_state()
{
    return charging_desired;
}

typedef enum {OFF = 0, ON = 1, ALMOST_ON, ALMOST_OFF} charge_state_t;

/******************************************************************************
 *
 *        Name: detect_charge_handle()
 *
 * Description:
 *
 *        Date: Tuesday, 15 October 2019
 *
 ******************************************************************************
 */
bool_t detect_charge_handle()
{

    bool_t charge_handle_detected = FALSE;

    bool_t charger_button_enabled =
        get_charger_button_enabled_state();

    //
    // Controls the debounce for detecting if the charge handle is
    // plugged in.
    //
    static timer_t plugged_in_timer;
    static bool_t first_run = TRUE;

    //
    // Processes to be done during first run.
    //
    if(first_run)
    {

        //
        // Initialse Timers used by the state machine
        //
        plugged_in_timer = timer_setup(get_THREE_SECONDS(), RISING);

        //
        // set first run to false
        //
        first_run = FALSE;

    }

    //
    // Check to see of the charge_button if pressed
    //
    if (charger_button_enabled == TRUE)
    {

        //
        // Charger button the charge_box has been pressed
        //

        //
        // Close the contactor that complete the circuit to the zener
        // diode.
        //
        Update_Output(OUT_A03_charge_box_relay_signal_from_cvc, OUTPUT_ON);

    }
    else
    {

        //
        // Return False, to indicate, charge handle has not been plugged in
        //
        Update_Output(OUT_A03_charge_box_relay_signal_from_cvc, OUTPUT_OFF);

    }

    //
    // Measure to check if the zener voltage is present. Look for
    // a voltage greater than 4V.
    //
    bool_t charge_plugged_in_timer = timer_operate(&plugged_in_timer,
                                           IN_E01_cvc_zener_input > 4000);

    //
    // Set variable to TRUE only if both the charge_button variable is
    // set to true and the charge_plugged_in_timer is true.
    //
    charge_handle_detected = (charge_plugged_in_timer && charger_button_enabled);

    return charge_handle_detected;

}



/******************************************************************************
 *
 *        Name: get_charger_enabled_state()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Wednesday, 09 October 2019
 *
 ******************************************************************************
 */
bool_t get_charger_button_enabled_state()
{
    bool_t ret_val = FALSE;
    uint16_t screen_ma = 0;
    uint16_t telematics_ma = 0;
    static bool_t first_pass = TRUE;
    static timer_t shutdown_timer;
    static charge_state_t charge_enable_state = ALMOST_ON;
    Input_State_t charge_input_switch;
    bool_t state;

    charge_input_switch = IN_A15_stop_charging_button_signal;

    switch (charge_enable_state)
    {

    case OFF:

        if (charge_input_switch == INPUT_OFF)
        {
            charge_enable_state = ALMOST_ON;
        }

        Update_Output(OUT_B03_stop_charge_button_led, OUTPUT_OFF);
        Update_Output(OUT_B03_stop_charge_button_led, OUTPUT_OFF);
        state = FALSE;

        break;

    case ALMOST_ON:

        if (charge_input_switch == INPUT_ON)
        {
            charge_enable_state = ON;
        }

        Update_Output(OUT_B03_stop_charge_button_led, OUTPUT_OFF);
        Update_Output(OUT_B03_stop_charge_button_led, OUTPUT_OFF);
        state = FALSE;

        break;

    case ON:

        if (charge_input_switch == INPUT_OFF)
        {
            charge_enable_state = ALMOST_OFF;
        }

        Update_Output(OUT_B03_stop_charge_button_led, OUTPUT_ON);
        Update_Output(OUT_B03_stop_charge_button_led, OUTPUT_ON);
        state = TRUE;

        break;

    case ALMOST_OFF:

        if (charge_input_switch == INPUT_ON)
        {
            charge_enable_state = OFF;
        }

        Update_Output(OUT_B03_stop_charge_button_led, OUTPUT_ON);
        Update_Output(OUT_B03_stop_charge_button_led, OUTPUT_ON);
        state = TRUE;

        break;
    }

    return state;
}
