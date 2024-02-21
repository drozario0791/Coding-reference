/******************************************************************************
 *
 *        Name: shinry_dcdc_device_getters.c
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Monday, 09 July 2019
 *
 ******************************************************************************
 */

#include "shinry_dcdc_device_private.h"

extern device_data_t *first_shinry_device_data_ptr;


uint8_t shinry_get_software_version(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_shinry_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (uint8_t)(device_data_ptr->status.software_version);
}


uint16_t shinry_get_instantaneous_input_voltage(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_shinry_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (uint16_t)(device_data_ptr->status.instantaneous_input_voltage);
}


uint16_t shinry_get_instantaneous_output_voltage(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_shinry_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    return (uint16_t)(device_data_ptr->status.instantaneous_output_voltage * 0.1);
}


uint16_t shinry_get_instantaneous_temperature(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_shinry_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The received value must be Offseted by -40.
    return (uint16_t)((device_data_ptr->status.instantaneous_temperature - 40));
}


uint16_t shinry_get_instantaneous_current_voltage(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_shinry_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    return (uint16_t)(device_data_ptr->status.instantaneous_current_voltage * 0.1);
}


bool_t shinry_get_over_current_fault(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_shinry_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->status.over_current_fault);
}


bool_t shinry_get_over_temperature_fault(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_shinry_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->status.over_temperature_fault);
}


bool_t shinry_get_derating(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_shinry_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->status.derating);
}


dcdc_mode_t shinry_get_operation_mode(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_shinry_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return STANDBY;
	}

    dcdc_mode_t mode = (dcdc_mode_t)device_data_ptr->status.operation_mode;

    return mode;
}


bool_t shinry_get_can_cmd_lost(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_shinry_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->status.can_cmd_lost);
}


bool_t shinry_get_input_over_voltage(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_shinry_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    return (bool_t)(device_data_ptr->status.input_over_voltage);
}


bool_t shinry_get_input_under_voltage(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_shinry_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    return (bool_t)(device_data_ptr->status.input_under_voltage);
}


bool_t shinry_get_output_over_voltage(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_shinry_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    return (bool_t)(device_data_ptr->status.output_over_voltage);
}


bool_t shinry_get_output_under_voltage(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_shinry_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    return (bool_t)(device_data_ptr->status.output_under_voltage);
}

//////////////////////////////////////////////////////////////////////////////
//
// By default, receive CAN message timeouts are disabled when receive
// messsages are registered. The timeouts may be set with the
// pdm_set_timeout_ setter functions. Also, by default, the _rx_ok
// status is set to FALSE when the receive CAN messages are
// registered. This function will return FALSE until each of the
// devices receive CAN messagers have been received at least one time.
//
// If receive CAN message timeouts are set for one or more messages,
// this function returns TRUE if there are no timeouts and FALSE if
// one or more messages have timed out.
//
// In order for receive CAN message timeouts to be active, a call to
// CAN service's can_rx_check_message_timeouts() must be made from
// User_App().
//
//////////////////////////////////////////////////////////////////////////////
//
bool_t shinry_get_can_rx_ok(device_instances_t device)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_shinry_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // By default, receive CAN message timeouts are disabled.
    //
    if (device_data_ptr->status_rx_ok == TRUE)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
