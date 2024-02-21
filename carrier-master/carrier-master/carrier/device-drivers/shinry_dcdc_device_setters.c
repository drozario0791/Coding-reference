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
#include "shinry_dcdc_device_private.h"

extern device_data_t *first_shinry_device_data_ptr;

/******************************************************************************
 *
 *        Name: shinry_dcdc_enable_signal_to_pin3_on_connector()
 *
 * Description: Controls the enable signal to Pin3 on connector.
 *
 *
 ******************************************************************************
 */
void shinry_dcdc_enable_signal_to_pin3_on_connector(device_instances_t device, uint16_t state)
{
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_shinry_device_data_ptr);

	if (device_data_ptr == NULL)
	{DEBUG("NULL POINTER");}

	if (state == 1)
	{
//        Update_Output(device_data_ptr->inverter_power_pin, OUTPUT_ON);
	}
	else
	{
//        Update_Output(device_data_ptr->inverter_power_pin, OUTPUT_OFF);
	}

}

/******************************************************************************
 *
 *        Name: shinry_set_dcdc_control
 *
 * Description:
 *
 *
 *      Author: Deepak
 *        Date: Thursday, 19 July 2019
 *
 ******************************************************************************
 */
void shinry_set_dcdc_control(
	device_instances_t device,
	float vcu_dcdc_voltage_req,
	bool_t dcdc_enable)
{

	shinry_dcdc_tx_control_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_shinry_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->control);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    // 0.125V/bit
    // Example: desired output voltage = 13.5V
    // CAN message to be sent = 13.5/0.125 = 108
    // dest_ptr->vcu_dcdc_voltage_req = (vcu_dcdc_voltage_req * 8);

    //
    // NOTE: As as per
    //
    dest_ptr->vcu_dcdc_voltage_req = (uint8_t)(vcu_dcdc_voltage_req / 0.125);

    // 0 -> Standby
    // 1 -> Work
    dest_ptr->dcdc_enable = dcdc_enable;

    // The variables below must always be set to zero.
    dest_ptr->unused1 = 0;
    dest_ptr->unused2 = 0;
    dest_ptr->unused3 = 0;

}
