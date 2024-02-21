/******************************************************************************
 *
 *        Name: device_name_device.h
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Tuesday, 25 June 2019
 *
 ******************************************************************************
 */

#ifndef SEVCON_HVLP10_DEVICE_H_
#define SEVCON_HVLP10_DEVICE_H_


typedef enum {
    ENABLE_SEVCON = 5,
    DISABLE_SEVCON = 3,
} sevcon_hvlp10_state_t;

typedef enum
{
    FORWARD,
    REVERSE,
} sevcon_hvlp10_direction_t;


//=============================================================================
//
// Public initialization function for Bel Charger.
//
//=============================================================================
//
void sevcon_hvlp10_init(device_instances_t device, uint8_t module_id, CANLINE_ can_line);

//=============================================================================
//
// These functions transmit the registered device CAN
// messages. Use the setter functions to populate the structure data
// that each one of these refers to.
//
//=============================================================================
//
void sevcon_hvlp10_tx_can_set_drive_limits_and_inverter_enable(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void sevcon_hvlp10_tx_can_set_operational_limits(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void sevcon_hvlp10_tx_can_battery_current_limits_and_voltage_limits(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void sevcon_hvlp10_tx_can_heartbeat(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

//=============================================================================
//
// Public functions to set the timeout counter limit for each of the
// device receive CAN messages.
//
//=============================================================================
//
void sevcon_hvlp10_set_timeout_parameters(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);

//=============================================================================
//
// Setter Functions for Transmit Functions
//
//=============================================================================
//
/******************************************************************************
 * Description: controls the low voltage supply to the sevcon
 ******************************************************************************
 */
void sevcon_hvlp10_set_12V_power(device_instances_t device, uint16_t state);

void sevcon_hvlp10_set_drive_limits_and_inverter_enable(
	device_instances_t device,
	uint16_t set_output_torque,
	sevcon_hvlp10_state_t sevcon_inverter_enable,
	uint16_t set_torque_drive_limit);

void sevcon_hvlp10_can_operational_limits(
	device_instances_t device,
	uint16_t set_torque_regen_limit,
	uint16_t set_forward_speed_limit,
	uint16_t set_reverse_speed_limit);

void sevcon_hvlp10_set_battery_current_limits_and_voltage_limits(
	device_instances_t device,
	uint16_t set_battery_discharge_limit,
	uint16_t set_battery_recharge_limit,
	uint16_t set_voltage_capacitor_limit);


//=============================================================================
//
// Getter Functions
//
//=============================================================================
//
/******************************************************************************
 * Description: returns the DC link voltage of the sevcon inverter
 ******************************************************************************
 */
uint16_t sevcon_hvlp10_get_sevcon_dc_link_voltage(device_instances_t device);

/******************************************************************************
 * Description: returns the heatsink temperature of the sevcon
 ******************************************************************************
 */
uint16_t sevcon_hvlp10_get_sevcon_heatsink_temperature(device_instances_t device);

/******************************************************************************
 * Description: returns the current draw of the stepper motor
 ******************************************************************************
 */
uint16_t sevcon_hvlp10_get_sevcon_current(device_instances_t device);

/******************************************************************************
 * Description: returns the temperature of the stepper motor
 ******************************************************************************
 */
uint16_t sevcon_hvlp10_get_stepper_motor_temperature(device_instances_t device);

/******************************************************************************
 * Description: returns the rpm of the motor
 ******************************************************************************
 */
sevcon_hvlp10_get_stepper_motor_rpm(
    device_instances_t device);


bool_t sevcon_hvlp10_get_can_rx_ok(device_instances_t device);

#endif
