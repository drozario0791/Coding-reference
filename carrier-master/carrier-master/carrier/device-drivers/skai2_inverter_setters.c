/******************************************************************************
 *
 *        Name: skai2_inverter_setters.c
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Monday, 09 July 2019
 *
 ******************************************************************************
 */

#include "Prototypes.h"
#include "skai2_inverter_private.h"

extern device_data_t *first_skai2_device_data_ptr;

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
void skai2_set_12V_power(device_instances_t device, uint16_t state)
{
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_skai2_device_data_ptr);

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
 *        Name: skai2_set_inverter_control
 *
 * Description:
 *
 *
 *      Author: Deepak
 *        Date: Thursday, 19 July 2019
 *
 ******************************************************************************
 */
void skai2_set_inverter_control(
	device_instances_t device,
	uint8_t motor_switch,
	uint8_t direction,
	uint8_t diagnostic_mode,
	uint16_t commanded_rpm)
{

	skai2_inverter_tx_control_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_skai2_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->control);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

	dest_ptr->motor_switch_enable = motor_switch;
	dest_ptr->commanded_motor_direction = direction;
	dest_ptr->diagnostic_mode = diagnostic_mode;
	dest_ptr->commanded_rpm = commanded_rpm;

	//
	// The variables below must always be set to zero.
	// They are used by the dyno.
    //
	dest_ptr->backdriving_mode = 0x00;
	dest_ptr->commanded_torque = 0x00;
}

/******************************************************************************
 *
 *        Name: skai2_set_inverter_limits
 *
 * Description:
 *
 *
 *      Author: Deepak
 *        Date: Thursday, 19 July 2019
 *
 ******************************************************************************
 */
void skai2_set_inverter_limits(
	device_instances_t device,
	uint16_t max_battery_current,
	uint16_t instantaneous_battery_current,
	uint16_t high_cell_voltage,
	skai2_inverter_modes_t mode)
{

	skai2_inverter_tx_limits_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_skai2_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->limits);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}


	dest_ptr->max_battery_current = max_battery_current;
	dest_ptr->instantaneous_battery_current = instantaneous_battery_current;
	dest_ptr->high_cell_voltage = high_cell_voltage;
	dest_ptr->mode = mode;
}
