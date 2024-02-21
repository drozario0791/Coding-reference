/******************************************************************************
 *
 *        Name: sevcon_hvlp10_device_getters.c
 *
 * Description: All of the getter functions for the device_name.
 *
 *      Author: Deepak
 *        Date: Friday, 09 August 2019
 *
 ******************************************************************************
 */
#include "sevcon_hvlp10_device_private.h"

extern device_data_t *first_sevcon_device_data_ptr;

//=============================================================================
//
// Getters for sevcon_hvlp10
//
//=============================================================================
//
uint16_t sevcon_hvlp10_get_sevcon_dc_link_voltage(
    device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(
            device,
            first_sevcon_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    //
    // Return the dc voltage present at the input of the sevcon
    // inverter.
    // scaling: 0.0625Nm/bit -> (1/0.0625 = 16)
    //
    return (uint16_t)(device_data_ptr->parameters.sevcon_dc_link_voltage / 16);
}

//=============================================================================
//
// sevcon_hvlp10_get_sevcon_heatsink_temperature()
//
//=============================================================================
//
uint16_t sevcon_hvlp10_get_sevcon_heatsink_temperature(
    device_instances_t device)
{
    uint16_t temperature;

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(
            device,
            first_sevcon_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    sevcon_hvlp10_rx_parameters_t *source_ptr =
        &(device_data_ptr->parameters);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // scaling: 0.0625Nm/bit -> (1/0.0625 = 16)
    //
    temperature =
        (uint16_t)(source_ptr->sevcon_heatsink_temperature / 16);

    return temperature;
}

//=============================================================================
//
// sevcon_hvlp10_get_sevcon_current()
//
//=============================================================================
//
uint16_t sevcon_hvlp10_get_sevcon_current(
    device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_sevcon_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // scaling: 0.0625Nm/bit -> (1/0.0625 = 16)
    return (uint16_t)(device_data_ptr->parameters.sevcon_current / 16);
}

//=============================================================================
//
// sevcon_hvlp10_get_stepper_motor_temperature()
//
//=============================================================================
//
uint16_t sevcon_hvlp10_get_stepper_motor_temperature(
    device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_sevcon_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // scaling: 0.0625Nm/bit -> (1/0.0625 = 16)
    return (uint16_t)(device_data_ptr->parameters.stepper_motor_temperature / 16);
}

//=============================================================================
//
// sevcon_hvlp10_get_stepper_motor_rpm()
//
//=============================================================================
//
sevcon_hvlp10_get_stepper_motor_rpm(
    device_instances_t device)
{

    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_sevcon_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // scaling: 0.0625Nm/bit -> (1/0.0625 = 16)
    return (uint16_t)(device_data_ptr->motor_rpm.motor_rpm);

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
bool_t sevcon_hvlp10_get_can_rx_ok(device_instances_t device)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_sevcon_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // By default, receive CAN message timeouts are disabled.
    //
    if (device_data_ptr->parameters_rx_ok == TRUE)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
