/******************************************************************************
 *
 *        Name: skai2_inverter_vissim_getters.c
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Monday, 09 July 2019
 *
 ******************************************************************************
 */

#include "skai2_inverter_private_vissim.h"

extern device_data_t *first_skai2_vissim_device_data_ptr;

//////////////////////////////////////////////////////////////////////
// Rx_Msg1_Data Structure
//////////////////////////////////////////////////////////////////////

bool_t skai_get_vissim_error_status(device_instances_t device)
{
    bool_t error_status = FALSE;

    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    skai2_inverter_rx_msg1_t *source_ptr = &(device_data_ptr->rx_msg1_data);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    uint16_t error_feedback1 =
        (uint16_t)source_ptr->error_feedback_1;

    if(error_feedback1 == 0x0010)
    {

        error_status = FALSE;
    }
    else
    {

        error_status = TRUE;
    }


    return error_status;

}

uint16_t skai_get_vissim_DCLink_Voltage(device_instances_t device)
{

    bool_t error_status = FALSE;

    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    skai2_inverter_rx_msg1_t *source_ptr = &(device_data_ptr->rx_msg1_data);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    uint16_t dc_link_voltage = (uint16_t)(source_ptr->DCLink_Voltage * 0.1);

    return dc_link_voltage;

}

uint16_t skai_get_vissim_motor_temp_C(device_instances_t device)
{

    bool_t error_status = FALSE;

    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    skai2_inverter_rx_msg1_t *source_ptr = &(device_data_ptr->rx_msg1_data);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    uint16_t motor_temp = (uint16_t)(source_ptr->MotorTemp_C * 0.1);

    return motor_temp;

}

//////////////////////////////////////////////////////////////////////
// Rx_Msg2_Data Structure
//////////////////////////////////////////////////////////////////////
uint16_t skai_get_vissim_auxilary_input_voltage(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    return (uint16_t)(device_data_ptr->rx_msg2_data.auxilary_input_voltage * 0.1);
}

uint16_t skai_get_vissim_throttle(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    return (uint16_t)(device_data_ptr->rx_msg2_data.throttle);
}

uint16_t skai_get_vissim_version_number(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    return (uint16_t)(device_data_ptr->rx_msg2_data.version_number);
}

//////////////////////////////////////////////////////////////////////
// Rx_Msg3_Data Structure
//////////////////////////////////////////////////////////////////////

uint16_t skai_get_vissim_torque(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    return (uint16_t)(device_data_ptr->rx_msg3_data.torque);
}

uint16_t skai_get_vissim_motor_rpm(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    return (uint16_t)(device_data_ptr->rx_msg3_data.motor_rpm);
}

uint16_t skai_get_vissim_encoder_feedback(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    return (uint16_t)(device_data_ptr->rx_msg3_data.encoder_feedback);
}

//////////////////////////////////////////////////////////////////////
// Rx_Msg4_Data Structure
//////////////////////////////////////////////////////////////////////
uint16_t skai_get_vissim_throttle_pedal_adc(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    return (uint16_t)(device_data_ptr->rx_msg4_data.throttle_pedal_adc);
}

uint16_t skai_get_vissim_IO_status(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    return (uint16_t)(device_data_ptr->rx_msg4_data.IO_status);
}

uint16_t skai_get_vissim_electrical_angle(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    return (uint16_t)(device_data_ptr->rx_msg4_data.electrical_angle);
}

//////////////////////////////////////////////////////////////////////
// Rx_Msg5_Data Structure
//////////////////////////////////////////////////////////////////////

uint16_t skai_get_vissim_dcb_phase1_temperature(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    return (uint16_t)(device_data_ptr->rx_msg5_data.dcb_phase1_temperature);
}

uint16_t skai_get_vissim_dcb_phase2_temperature(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    return (uint16_t)(device_data_ptr->rx_msg5_data.dcb_phase2_temperature);
}

uint16_t skai_get_vissim_dcb_phase3_temperature(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    return (uint16_t)(device_data_ptr->rx_msg5_data.dcb_phase3_temperature);
}

uint16_t skai_get_vissim_pcb_temperature(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    return (uint16_t)(device_data_ptr->rx_msg5_data.pcb_temperature);
}

//////////////////////////////////////////////////////////////////////
// Rx_Msg9_Data Structure
//////////////////////////////////////////////////////////////////////

uint16_t skai_get_vissim_power_consumned(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    return (uint16_t)(device_data_ptr->rx_msg9_data.power_consumned);
}

uint16_t skai_get_vissim_max_temperature(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    return (uint16_t)(device_data_ptr->rx_msg9_data.max_temperature);
}

uint16_t skai_get_vissim_inverter_enable(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    return (uint16_t)(device_data_ptr->rx_msg9_data.inverter_enable);
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
bool_t skai_get_vissim_inverter_can_rx_ok(device_instances_t device)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(
            device,
            first_skai2_vissim_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // By default, receive CAN message timeouts are disabled.
    //
    if ((device_data_ptr->rx_msg1_data_rx_ok == TRUE) &&
        (device_data_ptr->rx_msg2_data_rx_ok == TRUE) &&
        (device_data_ptr->rx_msg3_data_rx_ok == TRUE) &&
        (device_data_ptr->rx_msg4_data_rx_ok == TRUE) &&
        (device_data_ptr->rx_msg5_data_rx_ok == TRUE) &&
        (device_data_ptr->rx_msg9_data_rx_ok == TRUE))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
