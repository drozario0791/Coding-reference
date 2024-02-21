/******************************************************************************
 *
 *        Name: orion_control.c
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Friday, 30 August 2019
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
#include "skai2_inverter.h"
#include "cl712_device.h"
#include "orion_control.h"
#include "orion_device.h"
#include "fvt_library.h"
#include "timer_service.h"
#include "state_machine.h"

#define EEVAR_MAX_BATTERY_PACKS                      3
#define EEVAR_MAX_HIGH_CELL_TEMP_THRESHOLD           40
#define EEVAR_MAX_INTERNAL_BMS_TEMPERATURE_THRESHOLD 40
#define EEVAR_DISPLAY_SOC_SCALER_M_TERM              10/7
#define EEVAR_DISPLAY_SOC_SCALER_B_TERM              -28.5
#define EEVAR_BALANCING_CURRENT_SET_LIMIT            20

static bool_t differences_in_pack_voltages_within_limit();


/******************************************************************************
 *
 *        Name: get_battery_pack_voltage()
 *
 * Description:
 *
 *        Date: Tuesday, 03 September 2019
 *
 ******************************************************************************
 */
uint16_t get_battery_pack_voltage()
{

    //
    // Pack 1 Voltage
    //
    uint16_t pack1_voltage =
        orion_get_instantaneous_pack_voltage(ONE);

    //
    // Pack 2 Voltage
    //
    uint16_t pack2_voltage =
        orion_get_instantaneous_pack_voltage(TWO);

    //
    // Pack 3 Voltage
    //
    uint16_t pack3_voltage =
        orion_get_instantaneous_pack_voltage(THREE);

    //
    // Average Voltage of three packs
    //
    uint16_t average_pack_voltage =
        (uint16_t)((pack1_voltage + pack2_voltage + pack3_voltage) / EEVAR_MAX_BATTERY_PACKS);

    //
    // return the average voltage of the packs
    //
    return average_pack_voltage;

}


/******************************************************************************
 *
 *        Name: get_instantaneous_battery_pack_current()
 *
 * Description: Returns the total current sourced by the battery pack.
 *
 *        Date: Friday, 30 August 2019
 *
 ******************************************************************************
 */
int16_t get_instantaneous_battery_pack_current()
{

    //
    // Instantaneous current of battery pack 1
    //
    int16_t instantaneous_pack_1_current =
        orion_get_instantaneous_pack_current(ONE);

    //
    // Instantaneous current of battery pack 2
    //
    int16_t instantaneous_pack_2_current =
        orion_get_instantaneous_pack_current(TWO);

    //
    // Instantaneous current of battery pack 3
    //
    int16_t instantaneous_pack_3_current =
        orion_get_instantaneous_pack_current(THREE);

    //
    // Total Pack Current
    //
    int16_t instantaneous_battery_pack_current =
        (instantaneous_pack_1_current
         + instantaneous_pack_2_current
         + instantaneous_pack_3_current);

    //
    // Return the total battery current sourced by the battery pack
    //
    return instantaneous_battery_pack_current;

}


/******************************************************************************
 *
 *        Name: get_high_cell_voltage()
 *
 * Description: Returns the highest cell voltage of the three battery
 *              packs.
 *
 *        Date: Friday, 30 August 2019
 *
 ******************************************************************************
 */
uint16_t get_pack_high_cell_voltage()
{

    //
    // Pack1 High Cell Voltage
    //
    uint16_t pack_1_high_cell = orion_get_pack_high_cell_voltage(ONE);

    //
    // Pack2 High Cell Voltage
    //
    uint16_t pack_2_high_cell = orion_get_pack_high_cell_voltage(TWO);

    //
    // Pack3 High Cell Voltage
    //
    uint16_t pack_3_high_cell = orion_get_pack_high_cell_voltage(THREE);

    //
    // Return the min unsigned number
    //
    uint16_t pack_high_cell_voltage = max_of_three_uint16_t_values(
        pack_1_high_cell,
        pack_2_high_cell,
        pack_3_high_cell);

    return pack_high_cell_voltage;

}


/******************************************************************************
 *
 *        Name: get_low_cell_voltage()
 *
 * Description: Returns the lowest cell voltage of the three battery
 *              packs.
 *
 *        Date: Friday, 30 August 2019
 *
 ******************************************************************************
 */
uint16_t get_pack_low_cell_voltage()
{

    uint16_t pack_low_cell_voltage = 0;

    //
    // Pack1 Low Cell Voltage
    //
    uint16_t pack_1_low_cell = orion_get_pack_low_cell_voltage(ONE);

    //
    // Pack2 Low Cell Voltage
    //
    uint16_t pack_2_low_cell = orion_get_pack_low_cell_voltage(TWO);

    //
    // Pack3 Low Cell Voltage
    //
    uint16_t pack_3_low_cell = orion_get_pack_low_cell_voltage(THREE);

    //
    // Return the min unsigned number.
    //
    pack_low_cell_voltage = min_of_three_uint16_t_values(
        pack_1_low_cell,
        pack_2_low_cell,
        pack_3_low_cell);

    return pack_low_cell_voltage;

}


/******************************************************************************
 *
 *        Name: get_battery_pack_high_cell_max_temperature()
 *
 * Description: Returns the maximum temperature of the battery pack
 *
 *        Date: Tuesday, 03 September 2019
 *
 ******************************************************************************
 */
int8_t get_battery_pack_high_cell_max_temperature()
{

    //
    // High cell temperature of pack 1
    //
    int8_t high_cell_temp_1 =
        orion_get_pack_high_cell_temperature(ONE);

    //
    // High cell temperature of pack 2
    //
    int8_t high_cell_temp_2 =
        orion_get_pack_high_cell_temperature(TWO);

    //
    // High cell temperature of pack 3
    //
    int8_t high_cell_temp_3 =
        orion_get_pack_high_cell_temperature(THREE);

    //
    // Return the min unsigned number
    //
    int8_t pack_max_temperature = max_of_three_int8_t_values(
        high_cell_temp_1,
        high_cell_temp_2,
        high_cell_temp_3);

    //
    // Return the max cell temprature of the battery pack
    //
    return pack_max_temperature;

}


/******************************************************************************
 *
 *        Name: get_battery_pack_low_cell_max_temperature()
 *
 * Description: Returns the max temperature of the battery pack.
 *
 *        Date: Tuesday, 03 September 2019
 *
 ******************************************************************************
 */
int8_t get_battery_pack_low_cell_max_temperature()
{

    int8_t pack_max_temperature = 0;

    //
    // Low cell temperature of pack 1
    //
    int8_t low_cell_temp_1 =
        orion_get_pack_low_cell_temperature(ONE);

    //
    // Low cell temperature of pack 2
    //
    int8_t low_cell_temp_2 =
        orion_get_pack_low_cell_temperature(TWO);

    //
    // Low cell temperature of pack 3
    //
    int8_t low_cell_temp_3 =
        orion_get_pack_low_cell_temperature(THREE);

    //
    // Get the max temperature of three battery packs
    //
    pack_max_temperature = max_of_three_int8_t_values(low_cell_temp_1,
    												  low_cell_temp_2,
    												  low_cell_temp_3);

    //
    // Return the pack min temperature of the battery pack.
    //
    return pack_max_temperature;

}


/******************************************************************************
 *
 *        Name: get_battery_pack_SOC()
 *
 * Description: returns the average state of charge of the three
 *              battery packs on the carrier.
 *
 *        Date: Tuesday, 03 September 2019
 *
 ******************************************************************************
 */
uint8_t get_battery_pack_SOC()
{
	uint16_t battery_pack_SOC = 0;

    //
    // Pack 1 Voltage
    //
    uint8_t pack_1_SOC =
        orion_get_state_of_charge(ONE);

    //
    // Pack 2 SOC
    //
    uint8_t pack_2_SOC =
        orion_get_state_of_charge(TWO);

    //
    // Pack 3 SOC
    //
    uint8_t pack_3_SOC =
        orion_get_state_of_charge(THREE);

    battery_pack_SOC =
        (pack_1_SOC + pack_2_SOC + pack_3_SOC) / EEVAR_MAX_BATTERY_PACKS;

    //
    // Return the average of pack SOCs
    //
    return (uint8_t)battery_pack_SOC;

}


/******************************************************************************
 *
 *        Name: get_battery_pack_error_status()
 *
 * Description: Checks a set of conditions to determine if a fault has
 *              been incurred by the BMS.
 *
 *        Date: Tuesday, 03 September 2019
 *
 ******************************************************************************
 */
bool_t get_battery_pack_error_status(device_instances_t device)
{
    //
    // The conditions that would cause a battery pack failure are as
    // follows:
    // 1) Battery pack is safe to discharge.
    // 2) Battery pack is safe to charge.
    // 3) High voltage isolation fault.
    // 4) Low Cell Voltage Fault.
    // 5) CAN time out.
    // 6) Check to see of the voltage difference between the battery
    //    packs are under 20V.

    //
    // False if BMS discharge current limit hits 0;
    //
    bool_t discharge_relay =
        orion_get_flag0_discharge_relay(device);

    //
    // False if BMS charge current limit hits 0
    //
    bool_t charge_relay =
        orion_get_flag0_charge_relay(device);

    //
    // High Voltage isolation Fault. This code is set when the BMS
    // measures an isolation breakdown between the high voltage
    // battery and the 12 volt system.
    //
    bool_t high_voltage_isolation_fault =
        orion_get_flag2_high_voltage_isolation_fault(device);

    //
    // Check for open cell fault
    //
    bool_t open_cell_fault =
        orion_get_flag1_open_cell_voltage_fault(device);

    //
    // Check to see if an internal comms fault has occured
    //
    bool_t internal_comms_fault =
    		orion_get_flag1_internal_communication_fault(device);

    //
    // Check to see if the difference between the pack voltages are
    // below 20Vs
    //
    bool_t difference_in_pack_voltage_status =
        differences_in_pack_voltages_within_limit();

    //
    // Check to see if the high cell temperatue exceeded 60 degrees
    //
    int8_t high_cell_temp =
    		get_battery_pack_high_cell_max_temperature();

   bool_t high_cell_tenp_fault = (high_cell_temp > 60);

    //
    // Check if an error has occured.
    //
    bool_t error_status = (!discharge_relay
                           || high_voltage_isolation_fault
                           || open_cell_fault
                           || internal_comms_fault
                           || high_cell_tenp_fault
                           || !(difference_in_pack_voltage_status));

    //
    // Return the error_status
    //
    return error_status;

}


/******************************************************************************
 *
 *        Name: differences_in_pack_voltages_status()
 *
 * Description: The three battery packs on the carrier are connected
 *              in parallel. The voltage of these battery packs must
 *              within 20V of each other. If the voltage is greater,
 *              there would be large circulating currents within the
 *              battery pack.
 *
 *        Date: Tuesday, 03 September 2019
 *
 ******************************************************************************
 */
static bool_t differences_in_pack_voltages_within_limit()
{

    bool_t status = FALSE;
    uint16_t allowable_voltage_difference = 20;

    //
    // Pack 1 Voltage
    //
    uint16_t pack1_voltage =
        (uint16_t)orion_get_instantaneous_pack_voltage(ONE);

    //
    // Pack 2 Voltage
    //
    uint16_t pack2_voltage =
        (uint16_t)orion_get_instantaneous_pack_voltage(TWO);

    //
    // Pack 3 Voltage
    //
    uint16_t pack3_voltage =
        (uint16_t)orion_get_instantaneous_pack_voltage(THREE);


    //
    // Get the minimum pack voltage between the three packs
    //
    uint16_t min_of_three_packs = min_of_three_uint16_t_values(pack1_voltage,
                                                             pack2_voltage,
                                                             pack3_voltage);

    //
    // Get the maximum pack voltage between the three packs
    //
    uint16_t max_of_three_packs = max_of_three_uint16_t_values(pack1_voltage,
                                                             pack2_voltage,
                                                             pack3_voltage);
    //
    // Get the difference between the minimum and max pack voltage.
    //
    uint16_t difference_in_voltage =
        ((max_of_three_packs - min_of_three_packs) / 10);

    //
    // Check to see of the difference is greater than 20V. If the
    // voltage is greater than 20V. Return a False to indicate a
    // fault has occurred.
    //
    if (difference_in_voltage > allowable_voltage_difference)
    {

        //
        // The voltage difference exceeded 20V.
        //
        status = FALSE;

    }
    else
    {

        //
        // The voltage difference is within limits.
        //
        status = TRUE;

    }

    //
    // Return the status of the voltage limit check
    //
    return status;
}


/******************************************************************************
 *
 *        Name: bms_fan_controls
 *
 * Description: As of now there is not connections to the fans. Turn
 *              the fans ON when the temperature exceeds a certain set
 *              value.
 *
 *        Date: Wednesday, 04 September 2019
 *
 ******************************************************************************
 */
bool_t bms_fan_controls()
{
    bool_t exceeded_max_high_cell_temp = FALSE;
    bool_t exceeded_max_internal_temperature = FALSE;
    bool_t exceeded_temperature_thresholds = FALSE;

    //
    // Check the max temp of the high cells in the battery pack.
    //
    int8_t max_high_cell_temp =
        get_battery_pack_high_cell_max_temperature();

    //
    // Check the max internal temperature of the battery pack.
    //
    int8_t pack1_internal_temp = (int8_t)orion_get_internal_bms_temperature(ONE);
    int8_t pack2_internal_temp = (int8_t)orion_get_internal_bms_temperature(TWO);
    int8_t pack3_internal_temp = (int8_t)orion_get_internal_bms_temperature(THREE);

    //
    // Obtain the max internal tenperatures between the three battery
    // packs.
    //
    int8_t max_internal_temperature = max_of_three_int8_t_values(
        pack1_internal_temp,
        pack2_internal_temp,
        pack3_internal_temp);

    //
    // Check to see if the the max high cell temperature and the max
    // internal temperature has exceeded the set thresholds
    //
    //
    exceeded_max_high_cell_temp =
        hysteresis(exceeded_max_high_cell_temp,
                   max_high_cell_temp,
                   EEVAR_MAX_HIGH_CELL_TEMP_THRESHOLD, 
                   (EEVAR_MAX_HIGH_CELL_TEMP_THRESHOLD - 10));

    //
    // Check to see if the internal temperature of the BMS has
    // exceeded the set threshold.
    //
    exceeded_max_internal_temperature =
        hysteresis(exceeded_max_internal_temperature,
                   max_internal_temperature,
                   EEVAR_MAX_INTERNAL_BMS_TEMPERATURE_THRESHOLD,
                   (EEVAR_MAX_INTERNAL_BMS_TEMPERATURE_THRESHOLD - 10));

    //
    // As of now there is no power supplied to the fans on the BMS.
    // Therefore, just return a bool of TRUE or FALSE if a temperature
    // limit has been exceeded.
    //
    exceeded_temperature_thresholds =
        (exceeded_max_high_cell_temp ||
         exceeded_max_internal_temperature);

    //
    // True if a temperature threshold has been exceeded.
    //
    return exceeded_temperature_thresholds;


}


/******************************************************************************
 *
 *        Name: enable_cell_balancing()
 *
 * Description: Enables BMS cell balancing
 *
 *        Date: Wednesday, 04 September 2019
 *
 ******************************************************************************
 */
void enable_cell_balancing()
{

    //
    // The enabling/disabling of cell voltages are controlled by the
    // CVC. The CVC enables cell balancing only when the the totally
    // current draw from the battery pack is less than the set limit
    //
    int16_t instantaneous_pack_current =
        get_instantaneous_battery_pack_current();

    //
    // Check to see if the charger is plugged in. The balancing is
    // active when the charger has been plugged in.
    //
    bool_t charger_plugged_in = (get_sm_current_state() == CHARGING);

    //
    // Check to see if the current draw is less than a set limit or if
    // the charger is pluggedin.
    //
    if ((instantaneous_pack_current < EEVAR_BALANCING_CURRENT_SET_LIMIT) || charger_plugged_in)
    {

        //
        // One of the above conditions have been met. Enable
        // Balancing.
        //
        Update_Output(OUT_A13_turn_on_balancing_signal, OUTPUT_ON);

    }
    else
    {

        //
        // No conditions have been met. Disable balancing.
        //
        Update_Output(OUT_A13_turn_on_balancing_signal, OUTPUT_OFF);

    }

}


/******************************************************************************
 *
 *        Name: get_screen_SOC()
 *
 * Description: Calculate the state of charge that would need to be
 *              displayed on the screen.
 *
 *        Date: Wednesday, 04 September 2019
 *
 ******************************************************************************
 */
uint8_t get_screen_SOC()
{

    uint8_t screen_soc = 0;

    //
    // BMS Reported SOC
    //
    uint16_t bms_reported_soc = (uint16_t)get_battery_pack_SOC();

    //
    // The pack SOC displayed on the screen is different from the real
    // SOC of the pack. A linear fit is implemented where 0%(disaply
    // SOC) is 20%(bms_reported_soc) and 100%(display SOC) is
    // 90%(bms_reported_soc). The linear curver fit equaltion is given
    // by y =mx + b.
    //
    // Calculate the SOC that would be displayed on the screen.
    //
    screen_soc =
        (uint8_t)((EEVAR_DISPLAY_SOC_SCALER_M_TERM * bms_reported_soc) +
                   EEVAR_DISPLAY_SOC_SCALER_B_TERM);

    //
    // Return the calculated SOC that would be displayed on the
    // Screen.
    //
    return screen_soc;
}


/******************************************************************************
 *
 *        Name: get_range_in_minutes_flats()
 *
 * Description: Calculate the range the vehicle can drive in minutes.
 *
 *        Date: Wednesday, 04 September 2019
 *
 ******************************************************************************
 */
uint16_t get_range_in_minutes_flats()
{
    float range_in_minutes = 0;

    //
    // Based on Empirical Data
    //
    float scaling_factor = 2.1;

    //
    // Obtain the average SOC of the three battery packs
    //
    uint16_t bms_reported_soc = (uint16_t)get_battery_pack_SOC();

    //
    // Calculate the amount of time left to drive in minutes.
    //
    range_in_minutes =
        bms_reported_soc * scaling_factor * EEVAR_MAX_BATTERY_PACKS;

    //
    // Return the driving range of the vehicle in minutes.
    //
    return (uint16_t)range_in_minutes;

}


/******************************************************************************
 *
 *        Name: get_range_in_minutes_slopes()
 *
 * Description: Calculate the range the vehicle can drive in minutes.
 *
 *        Date: Wednesday, 04 September 2019
 *
 ******************************************************************************
 */
uint16_t get_range_in_minutes_slopes()
{
    float range_in_minutes = 0;

    //
    // Based on Empirical Data
    //
    float scaling_factor = 0.6;

    //
    // Obtain the average SOC of the three battery packs
    //
    uint16_t bms_reported_soc = (uint16_t)get_battery_pack_SOC();

    //
    // Calculate the amount of time left to drive in minutes.
    //
    range_in_minutes =
        bms_reported_soc * scaling_factor * EEVAR_MAX_BATTERY_PACKS;

    //
    // Return the driving range of the vehicle in minutes.
    //
    return (uint16_t)range_in_minutes;

}


//=============================================================================
//
// bms_fresh_data()
//
//=============================================================================
//
bool_t bms_fresh_data_1()
{

    static timer_t fresh_data;
    static bool_t first_pass = TRUE;
    static uint8_t rolling_counter_prev = 0;
    bool_t fresh_data_status = TRUE;


    if(first_pass == TRUE)
    {
        //
        // five_seconds
        //
        fresh_data = timer_setup(get_FIVE_SECONDS(), RISING);
        first_pass = FALSE;

    }

    uint8_t rolling_counter = orion_get_rolling_counter(ONE);

    fresh_data_status = !timer_operate(&fresh_data,
                                       (rolling_counter_prev == rolling_counter));

    rolling_counter_prev = rolling_counter;

    return fresh_data_status;

}

//=============================================================================
//
// bms_fresh_data()
//
//=============================================================================
//
bool_t bms_fresh_data_2()
{

    static timer_t fresh_data;
    static bool_t first_pass = TRUE;
    static uint8_t rolling_counter_prev = 0;
    bool_t fresh_data_status = TRUE;


    if(first_pass == TRUE)
    {
        //
        // five_seconds
        //
        fresh_data = timer_setup(get_FIVE_SECONDS(), RISING);
        first_pass = FALSE;

    }

    uint8_t rolling_counter = orion_get_rolling_counter(TWO);

    fresh_data_status = !timer_operate(&fresh_data,
                                       (rolling_counter_prev == rolling_counter));

    rolling_counter_prev = rolling_counter;

    return fresh_data_status;

}

//=============================================================================
//
// bms_fresh_data()
//
//=============================================================================
//
bool_t bms_fresh_data_3()
{

    static timer_t fresh_data;
    static bool_t first_pass = TRUE;
    static uint8_t rolling_counter_prev = 0;
    bool_t fresh_data_status = TRUE;


    if(first_pass == TRUE)
    {
        //
        // five_seconds
        //
        fresh_data = timer_setup(get_FIVE_SECONDS(), RISING);
        first_pass = FALSE;

    }

    uint8_t rolling_counter = orion_get_rolling_counter(THREE);

    fresh_data_status = !timer_operate(&fresh_data,
                                       (rolling_counter_prev == rolling_counter));

    rolling_counter_prev = rolling_counter;

    return fresh_data_status;

}
