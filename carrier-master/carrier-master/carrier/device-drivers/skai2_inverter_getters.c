/******************************************************************************
 *
 *        Name: skai2_inverter_getters.c
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Monday, 09 July 2019
 *
 ******************************************************************************
 */

#include "skai2_inverter_private.h"

extern device_data_t *first_skai2_device_data_ptr;

//=============================================================================
//
// A set of Functions that return the temperatures of phase A, phase
// B, phase C and pcb temperature of the inverter.
//
//=============================================================================
//
//=============================================================================
//
// skai_get_temperature_dcb_phase_L1()
//
//=============================================================================
//
uint16_t skai_get_temperature_dcb_phase_L1(device_instances_t device)
{

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    skai2_inverter_temperature_t *source_ptr = &(device_data_ptr->temperature);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}


    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    uint16_t temperature_dcb_phase_L1 = (uint16_t)(source_ptr->temperature_dcb_phase_L1 * 0.1);

    return temperature_dcb_phase_L1;
}

//=============================================================================
//
// skai_get_temperature_dcb_phase_L2()
//
//=============================================================================
//
uint16_t skai_get_temperature_dcb_phase_L2(device_instances_t device)
{

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    skai2_inverter_temperature_t *source_ptr = &(device_data_ptr->temperature);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}


    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    uint16_t temperature_dcb_phase_L2 = (uint16_t)(source_ptr->temperature_dcb_phase_L2 * 0.1);

    return temperature_dcb_phase_L2;
}

//=============================================================================
//
// skai_get_temperature_dcb_phase_L3()
//
//=============================================================================
//
uint16_t skai_get_temperature_dcb_phase_L3(device_instances_t device)
{

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    skai2_inverter_temperature_t *source_ptr = &(device_data_ptr->temperature);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}


    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    uint16_t temperature_dcb_phase_L3 = (uint16_t)(source_ptr->temperature_dcb_phase_L3 * 0.1);

    return temperature_dcb_phase_L3;
}

//=============================================================================
//
// skai_get_temperature_pcb()
//
//=============================================================================
//
uint16_t skai_get_temperature_pcb(device_instances_t device)
{

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    skai2_inverter_temperature_t *source_ptr = &(device_data_ptr->temperature);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    uint16_t temperature_pcb = (uint16_t)(source_ptr->temperature_pcb * 0.1);

    return temperature_pcb;
}

//=============================================================================
//
// skai_get_electrical_offset_ccw(device_instances_t device)
//
//=============================================================================
//
uint16_t skai_get_electrical_offset_ccw(device_instances_t device)
{

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    skai2_inverter_eleAngle_RPM_t *source_ptr = &(device_data_ptr->ElectricalAngle_RPM);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    uint16_t electrical_offset_ccw = (uint16_t)(source_ptr->electrical_offset_ccw * 0.1);

    return electrical_offset_ccw;
}

//=============================================================================
//
// skai_get_electrical_offset_cw(device_instances_t device)
//
//=============================================================================
//
uint16_t skai_get_electrical_offset_cw(device_instances_t device)
{

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    skai2_inverter_eleAngle_RPM_t *source_ptr = &(device_data_ptr->ElectricalAngle_RPM);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    uint16_t electrical_offset_cw = (uint16_t)(source_ptr->electrical_offset_cw * 0.1);

    return electrical_offset_cw;
}

//=============================================================================
//
// skai_get_motor_rpm(device_instances_t device)
//
//=============================================================================
//
uint16_t skai_get_motor_rpm(device_instances_t device)
{

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    skai2_inverter_eleAngle_RPM_t *source_ptr = &(device_data_ptr->ElectricalAngle_RPM);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    uint16_t motor_rpm = (uint16_t)(source_ptr->motor_rpm);

    return motor_rpm;
}

//=============================================================================
//
// skai_get_max_temperature(device_instances_t device)
//
//=============================================================================
//
uint16_t skai_get_max_temperature(device_instances_t device)
{

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    skai2_inverter_eleAngle_RPM_t *source_ptr = &(device_data_ptr->ElectricalAngle_RPM);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    uint16_t max_temperature = (uint16_t)(source_ptr->max_temperature * 0.1);

    return max_temperature;
}

//=============================================================================
//
// skai_get_torque(device_instances_t device)
//
//=============================================================================
//
uint16_t skai_get_torque(device_instances_t device)
{

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    skai2_inverter_torque_t *source_ptr = &(device_data_ptr->motor_torque);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    uint16_t motor_torque = (uint16_t)(source_ptr->motor_torque * 0.1);

    return motor_torque;

}

//=============================================================================
//
// skai_get_MotorTemp_C(device_instances_t device)
//
//=============================================================================
//
uint16_t skai_get_MotorTemp_C(device_instances_t device)
{

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    skai2_inverter_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    uint16_t motor_temp = (uint16_t)(source_ptr->MotorTemp_C * 0.1);

    return motor_temp;
}

//=============================================================================
//
// skai_get_MotorTemp_C(device_instances_t device)
//
//=============================================================================
//
uint16_t skai_get_DCLink_Voltage(device_instances_t device)
{

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    skai2_inverter_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    uint16_t DCLink_Voltage = (uint16_t)(source_ptr->DCLink_Voltage * 0.1);

    return DCLink_Voltage;
}


//=============================================================================
//
// A set of functions to check the status bits of the inverter
//
//=============================================================================
//
bool_t skai_get_inverter_error_status(device_instances_t device)
{

    bool_t error_status = FALSE;

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    skai2_inverter_status_t *source_ptr = &(device_data_ptr->status);
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

    //
    // Return the error status of the skai2 inverter
    //
    return error_status;
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
bool_t skai2_get_can_rx_ok(device_instances_t device)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // By default, receive CAN message timeouts are disabled.
    //
    if ((device_data_ptr->status_rx_ok == TRUE) &&
        (device_data_ptr->temperature_rx_ok == TRUE) &&
        (device_data_ptr->ElectricalAngle_RPM_rx_ok == TRUE) &&
        (device_data_ptr->motor_torque_rx_ok == TRUE))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
