/******************************************************************************
 *
 *        Name: cl712_device_getters.c
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Friday, 30 August 2019
 *
 ******************************************************************************
 */

#include "cl712_device_private.h"

extern device_data_t *first_cl712_device_data_ptr;

bool_t get_hydraulic_motor_pump_override(device_instances_t device)
{

    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)((device_data_ptr->rx_message_1.hydraulic_motor_pump_override));

}

bool_t get_traction_motor_pump_override(device_instances_t device)
{

    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)((device_data_ptr->rx_message_1.traction_motor_pump_override));

}

bool_t pe_pump_override(device_instances_t device)
{

    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)((device_data_ptr->rx_message_1.pe_pump_override));

}

bool_t charger_pump_override(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)((device_data_ptr->rx_message_1.charger_pump_override));

}

bool_t charger_fan_override(device_instances_t device)
{

    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)((device_data_ptr->rx_message_1.charger_fan_override));

}

bool_t pe_fan_override(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)((device_data_ptr->rx_message_1.pe_fan_override));
}
