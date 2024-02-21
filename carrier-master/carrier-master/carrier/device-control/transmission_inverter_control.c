/******************************************************************************
 *
 *        Name: transmission_inverter_control.c
 *
 * Description: A module that controls the working of the
 *              sevcon inverter.
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
#include "sevcon_hvlp10_device.h"
#include "transmission_inverter_control.h"

#define EEVAR_SEVCON_HVLP10_TORQUE                  18
#define EEVAR_SEVCON_HVLP10_DRIVE_LIMIT             36
#define EEVAR_SEVCON_HVLP10_REGEN_LIMIT             36
#define EEVAR_SEVCON_HVLP10_FWD_RPM_LIMIT           1000
#define EEVAR_SEVCON_HVLP10_RVS_RPM_LIMIT           1800
#define EEVAR_SEVCON_HVLP10_BATTERY_DISCHARGE_LIMIT 18
#define EEVAR_SEVCON_HVLP10_BATTERY_RECHARGE_LIMIT  18
#define EEVAR_SEVCON_HVLP10_CAPACITOR_VOLTAGE_LIMIT 0
#define EEVAR_SEVCON_HVLP10_DIRECTION               255


/******************************************************************************
 *
 *        Name: transmission_inverter_control()
 *
 * Description: A function that is responsible to enable/disable the
 *              transmission inverter based on various conditions.
 *
 *        Date: Friday, 30 August 2019
 *
 ******************************************************************************
 */
void transmission_inverter_control(
    device_instances_t device,
    sevcon_hvlp10_state_t transmission_inverter_enable)
{

    sevcon_hvlp10_state_t inverter_enable = DISABLE_SEVCON;

    uint16_t torque_command = 0;

    //
    // The hydraulic_motor can be enabled or disabled from the screen
    // using the screen over-ride function. This is useful when
    // debugging issues.
    //
    bool_t screen_override_status = FALSE;

    bool_t transmission_motor_override_status = FALSE;

    //
    // Enable the transmission inverter. Check to see of screen
    // over-ride is active. If screen_override is active,
    // enable/disable the inverter based on the input from the
    // screen. Else, use the argument provided to enable or disable
    // the motor. Also, check to see if precharge is successful. Only
    // enable the inverter is precharge is successful and high_voltage
    // is ON.
    //
    if(screen_override_status == TRUE)
    {

        //
        // Use the command from the screen over-ride
        //
        inverter_enable =
            (sevcon_hvlp10_state_t)transmission_motor_override_status;

    }
    else
    {

        //
        // Enable/ disable the motor based on the argument provided to
        // the function.
        //
        inverter_enable = transmission_inverter_enable;

    }


    //
    // Determine the torque based on the direction of rotation
    //
    if(EEVAR_SEVCON_HVLP10_DIRECTION == 1)
    {
        //
        // Send positive torque value
        //
        torque_command =
            EEVAR_SEVCON_HVLP10_TORQUE;
    }
    else
    {
        //
        // Send negative torque value
        //
        torque_command =
            (EEVAR_SEVCON_HVLP10_TORQUE);
    }

    //
    // Use the setter function from the sevcon_hvlp10_ device driver
    // to setup the control message that needs to be transmitted.
    //
    //
    // Sets the operational torque and the maximum drive torque limit
    // of the sevcon. Also sets the enable/disabled bit of the sevcon
    // inverter can protocol.
    //
    sevcon_hvlp10_set_drive_limits_and_inverter_enable(
        device,
        torque_command,
        inverter_enable,
        EEVAR_SEVCON_HVLP10_DRIVE_LIMIT);

    //
    // Sets the rpm limits in forwar and reverse direction. Also sets
    // the regen torque limit of the inverter.
    //
    sevcon_hvlp10_can_operational_limits(
        device,
        EEVAR_SEVCON_HVLP10_REGEN_LIMIT,
        EEVAR_SEVCON_HVLP10_FWD_RPM_LIMIT,
        EEVAR_SEVCON_HVLP10_RVS_RPM_LIMIT);

    //
    // Sets the max current the inverter can draw under motoring
    // operation and the max current it can supply to the battery
    // under regen conditions. Also, sets the max voltage voltage of
    // the dc link capacitor.
    //
    sevcon_hvlp10_set_battery_current_limits_and_voltage_limits(
        device,
        EEVAR_SEVCON_HVLP10_BATTERY_DISCHARGE_LIMIT,
        EEVAR_SEVCON_HVLP10_BATTERY_RECHARGE_LIMIT,
        EEVAR_SEVCON_HVLP10_CAPACITOR_VOLTAGE_LIMIT);

    //
    // Transmits the CAN messages.
    //
    sevcon_hvlp10_tx_can_set_drive_limits_and_inverter_enable(device, EVERY_50MS);

    sevcon_hvlp10_tx_can_set_operational_limits(device, EVERY_50MS);

    sevcon_hvlp10_tx_can_battery_current_limits_and_voltage_limits(device, EVERY_50MS);

    sevcon_hvlp10_tx_can_heartbeat(device, EVERY_50MS);

}
