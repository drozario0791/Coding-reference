/******************************************************************************
 *
 *        Name: device_name_setters.c
 *
 * Description:
 *
 *        Date: Wednesday, 31 July 2019
 *
 ******************************************************************************
 */
#include "Prototypes.h"
#include "sevcon_hvlp10_device_private.h"

extern device_data_t *first_sevcon_device_data_ptr;

/******************************************************************************
 *
 *        Name: skai2_power_on/skai2_power_off
 *
 * Description: A set of functions that control's the 12V supply to the
 *              SKAI inverters.
 *
 *      Author: Deepak
 *        Date: Thursday, 11 July 2019
 *
 ******************************************************************************
 */
void sevcon_hvlp10_set_12V_power(device_instances_t device, uint16_t state)
{
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_sevcon_device_data_ptr);

	if (device_data_ptr == NULL)
	{DEBUG("NULL POINTER");}

	if (state == 1)
	{
	// Update_Output(device_data_ptr->inverter_power_pin, OUTPUT_ON);
	}
	else
	{
	// Update_Output(device_data_ptr->inverter_power_pin, OUTPUT_OFF);
	}


}

/******************************************************************************
 *
 *        Name: sevcon_hvlp10_set_drive_limits_and_inverter_enable
 *
 * Description:
 *
 *
 *      Author: Deepak
 *        Date: Thursday, 19 July 2019
 *
 ******************************************************************************
 */
void sevcon_hvlp10_set_drive_limits_and_inverter_enable(
	device_instances_t device,
	uint16_t set_output_torque,
	sevcon_hvlp10_state_t sevcon_inverter_enable,
	uint16_t set_torque_drive_limit)
{

	sevcon_hvlp10_tx_data1_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_sevcon_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->data1);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    // The output torque that inverter will attempt to produce when
    // enabled is denoted by the variable set_output_torque.
    // scaling: 0.0625Nm/bit -> (1/0.0625 = 16)
    dest_ptr->set_output_torque = (~(set_output_torque * 16) + 1);

    // Controls the state of the switches in the inverter
    // value of 5 is enable, value of 3 is disable.
    dest_ptr->sevcon_inverter_enable = sevcon_inverter_enable;

    // Maximum allowed drive torque.
    // scaling: 0.0625Nm/bit -> (1/0.0625 = 16)
    dest_ptr->set_torque_drive_limit = (set_torque_drive_limit * 16);

	// The variables below must always be set to zero.
	// They are used by the dyno.
 	dest_ptr->sequence_check = 0;
    dest_ptr->checksum = 0;
}


/******************************************************************************
 *
 *        Name: sevcon_hvlp10_can_operational_limits()
 *
 * Description:
 *
 *        Date: Wednesday, 31 July 2019
 *
 ******************************************************************************
 */
void sevcon_hvlp10_can_operational_limits(
	device_instances_t device,
	uint16_t set_torque_regen_limit,
	uint16_t set_forward_speed_limit,
	uint16_t set_reverse_speed_limit)
{

	sevcon_hvlp10_tx_data2_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_sevcon_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->data2);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    // For negative value, use 2's compliment to transmit the
    // message.
    // value = (~value_2_send + 1)
    // Maximum allowed regen torque
    // scaling: 0.0625Nm/bit -> (1/0.0625 = 16)
    // set_torque_regen_limit = ~(set_torque_regen_limit * 16) + 1
    dest_ptr->set_torque_regen_limit = set_torque_regen_limit * 16;

    // Set the maximum speed of the motor under motoring operation.
    // scaling: 1RPM/bit
    dest_ptr->set_forward_speed_limit = set_forward_speed_limit;

    // Set the maximum spped of the motor under reverse motoring operation.
    // Take 2's compliment as its a negative number
    // scaling: 1RPM/bit
    dest_ptr->set_reverse_speed_limit = (~set_reverse_speed_limit + 1);

	// The variables below must always be set to zero.
	// They are used by the dyno.
 	dest_ptr->sequence_check = 0;
    dest_ptr->checksum = 0;
}

/******************************************************************************
 *
 *        Name: sevcon_hvlp10_set_battery_current_limits_and_voltage_limits()
 *
 * Description:
 *
 *
 *        Date: Wednesday, 19 July 2019
 *
 ******************************************************************************
 */
void sevcon_hvlp10_set_battery_current_limits_and_voltage_limits(
	device_instances_t device,
	uint16_t set_battery_discharge_limit,
	uint16_t set_battery_recharge_limit,
	uint16_t set_voltage_capacitor_limit)
{

	sevcon_hvlp10_tx_data3_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_sevcon_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->data3);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    // Maximum allowed battery discharge
    // scaling: 1A/bit
    dest_ptr->set_battery_discharge_limit = set_battery_discharge_limit;

    // Maximum allowed battery current during discharge
    // scaling: 1A/bit
    // As the regen current is negative in nature, use 2's compliment.
    dest_ptr->set_battery_recharge_limit = set_battery_recharge_limit;

    // Set the maximum DC link voltage at the input of the inverter.
    // scaling: 0.0625Nm/bit -> (1/0.0625 = 16)
    dest_ptr->set_voltage_capacitor_limit = set_voltage_capacitor_limit * 16;

	// The variables below must always be set to zero.
	// They are used by the dyno.
 	dest_ptr->sequence_check = 0;
    dest_ptr->checksum = 0;
}
