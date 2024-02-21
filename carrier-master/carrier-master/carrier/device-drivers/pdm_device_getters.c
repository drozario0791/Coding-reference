/******************************************************************************
 *
 *        Name: pdm_getters.c
 *
 * Description: Return various data received from the PDM.
 *
 *              Refer to the PDM reference manual.
 *              file:../../../../ref/enovationcontrols/pdm_reference.pdf
 *
 *      Author: Tom
 *        Date: Thursday, 04 July 2019
 *
 ******************************************************************************
 */
#include "pdm_device_private.h"

extern device_data_t *pdm_first_device_data_ptr;


//=============================================================================
//
// uint8_t
// pdm_get_digital_input_feedback(
//     device_instances_t device,
//     pdm_dio_channels_t channel)
//
//=============================================================================
//
uint8_t
pdm_get_digital_input_feedback(
    device_instances_t device,
    pdm_dio_channels_t channel)
{
    uint8_t digital_input = 255;

    // Need channel early in function, verify now.
    if ((channel) > 12 || (channel < 1)) {DEBUG("Invalid channel");}

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    analog_in_1_2_digital_in_feedback_t *source_ptr = &(device_data_ptr->analog_in_1_2_digital_in_feedback);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Read the digital input from the specified channel.
    //
    switch (channel)
    {
    case  PDM_CHANNEL_1:
        digital_input = (uint8_t)(source_ptr->digital_input_1);
        break;

    case  PDM_CHANNEL_2:
        digital_input = (uint8_t)(source_ptr->digital_input_2);
        break;

    case  PDM_CHANNEL_3:
        digital_input = (uint8_t)(source_ptr->digital_input_3);
        break;

    case  PDM_CHANNEL_4:
        digital_input = (uint8_t)(source_ptr->digital_input_4);
        break;

    case  PDM_CHANNEL_5:
        digital_input = (uint8_t)(source_ptr->digital_input_5);
        break;

    case  PDM_CHANNEL_6:
        digital_input = (uint8_t)(source_ptr->digital_input_6);
        break;

    case  PDM_CHANNEL_7:
        digital_input = (uint8_t)(source_ptr->digital_input_7);
        break;

    case  PDM_CHANNEL_8:
        digital_input = (uint8_t)(source_ptr->digital_input_8);
        break;

    case  PDM_CHANNEL_9:
        digital_input = (uint8_t)(source_ptr->digital_input_9);
        break;

    case  PDM_CHANNEL_10:
        digital_input = (uint8_t)(source_ptr->digital_input_10);
        break;

    case  PDM_CHANNEL_11:
        digital_input = (uint8_t)(source_ptr->digital_input_11);
        break;

    case  PDM_CHANNEL_12:
        digital_input = (uint8_t)(source_ptr->digital_input_12);
        break;
    }

    return digital_input;
}


//=============================================================================
//
// uint8_t pdm_get_digital_output_diagnostic(
//     device_instances_t device,
//     pdm_dio_channels_t channel)
//
//=============================================================================
//
uint8_t pdm_get_digital_output_dignostic(
    device_instances_t device,
    pdm_dio_channels_t channel)
{
    uint8_t output_diagnostic = 255;

    // Need channel early in function, verify now.
    if ((channel) > 12 || (channel < 1)) {DEBUG("Invalid channel");}

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    analog_in_3_4_digital_out_feedback_t *source_ptr =
        &(device_data_ptr->analog_in_3_4_digital_out_feedback);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Read the specified channel's output diagnostic.
    //
    switch (channel)
    {
    case  PDM_CHANNEL_1:
        output_diagnostic = (uint8_t)(source_ptr->output_diagnostic_1);
        break;

    case  PDM_CHANNEL_2:
        output_diagnostic = (uint8_t)(source_ptr->output_diagnostic_2);
        break;

    case  PDM_CHANNEL_3:
        output_diagnostic = (uint8_t)(source_ptr->output_diagnostic_3);
        break;

    case  PDM_CHANNEL_4:
        output_diagnostic = (uint8_t)(source_ptr->output_diagnostic_4);
        break;

    case  PDM_CHANNEL_5:
        output_diagnostic = (uint8_t)(source_ptr->output_diagnostic_5);
        break;

    case  PDM_CHANNEL_6:
        output_diagnostic = (uint8_t)(source_ptr->output_diagnostic_6);
        break;

    case  PDM_CHANNEL_7:
        output_diagnostic = (uint8_t)(source_ptr->output_diagnostic_7);
        break;

    case  PDM_CHANNEL_8:
        output_diagnostic = (uint8_t)(source_ptr->output_diagnostic_8);
        break;

    case  PDM_CHANNEL_9:
        output_diagnostic = (uint8_t)(source_ptr->output_diagnostic_9);
        break;

    case  PDM_CHANNEL_10:
        output_diagnostic = (uint8_t)(source_ptr->output_diagnostic_10);
        break;

    case  PDM_CHANNEL_11:
        output_diagnostic = (uint8_t)(source_ptr->output_diagnostic_11);
        break;

    case  PDM_CHANNEL_12:
        output_diagnostic = (uint8_t)(source_ptr->output_diagnostic_12);
        break;
    }

    return output_diagnostic;
}


//=============================================================================
//
// uint8_t pdm_get_sensor_supply_high(device_instances_t device)
//
//=============================================================================
//
uint8_t pdm_get_sensor_supply_high(device_instances_t device)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    analog_in_5_6_battery_sensor_supply_feedback_t *source_ptr =
        &(device_data_ptr->analog_in_5_6_battery_sensor_supply_feedback);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (uint8_t)(source_ptr->sensor_supply_high);
}

//=============================================================================
//
// uint8_t pdm_get_sensor_supply_low(device_instances_t device)
//
//=============================================================================
//
uint8_t pdm_get_sensor_supply_low(device_instances_t device)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    analog_in_5_6_battery_sensor_supply_feedback_t *source_ptr =
        &(device_data_ptr->analog_in_5_6_battery_sensor_supply_feedback);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (uint8_t)(source_ptr->sensor_supply_low);
}

//=============================================================================
//
// uint16_t pdm_get_battery_voltage(device_instances_t device)
//
//=============================================================================
//
uint16_t pdm_get_battery_voltage_mv(device_instances_t device)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    analog_in_5_6_battery_sensor_supply_feedback_t *source_ptr =
        &(device_data_ptr->analog_in_5_6_battery_sensor_supply_feedback);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return source_ptr->battery_voltage;
}


//=============================================================================
//
// uint8_t pdm_get_power_supply_status(device_instances_t device)
//
//=============================================================================
//
uint8_t pdm_get_power_supply_status(device_instances_t device)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    analog_in_7_8_power_version_feedback_t *source_ptr =
        &(device_data_ptr->analog_in_7_8_power_version_feedback);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (uint8_t)(source_ptr->power_supply_status);
}


//=============================================================================
//
// uint8_t pdm_get_total_current_status(device_instances_t device)
//
//=============================================================================
//
uint8_t pdm_get_total_current_status(device_instances_t device)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    analog_in_7_8_power_version_feedback_t *source_ptr =
        &(device_data_ptr->analog_in_7_8_power_version_feedback);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (uint8_t)(source_ptr->total_current_status);
}

//=============================================================================
//
// uint16_t pdm_get_software_version(device_instances_t device)
//
//=============================================================================
//
uint16_t pdm_get_software_version(device_instances_t device)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    analog_in_7_8_power_version_feedback_t *source_ptr =
        &(device_data_ptr->analog_in_7_8_power_version_feedback);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (uint16_t)(source_ptr->software_version);
}


//=============================================================================
//
// uint8_t pdm_get_output_feedback(
//    device_instances_t device,
//    pdm_dio_channels_t channel)
//
//=============================================================================
//
uint8_t pdm_get_output_current_feedback(
    device_instances_t device,
    pdm_dio_channels_t channel)
{
    uint8_t output_feedback = 255;
    output_current_feedback_t *source_ptr = NULL;
    //
    // Verify channel number
    //
    if ((channel) > 12 || (channel < 1)) {DEBUG("Invalid channel");}

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    if (channel < 7)
    {
        //
        // Have the pointer to this device's data record, now get a
        // pointer to the appropriate receive data structure for channels
        // 1 to 6.
        //
        source_ptr = &(device_data_ptr->output_current_1_6_feedback);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}
    }
    else
    {
        //
        // Have the pointer to this device's data record, now get a
        // pointer to the appropriate receive data structure for channels
        // 7 to 12.
        //
        source_ptr = &(device_data_ptr->output_current_7_12_feedback);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}
    }

    //
    // Get the appropriate data for the specified channel.
    //
    switch (channel)
    {
        case  PDM_CHANNEL_1:
        case  PDM_CHANNEL_7:
            output_feedback = (uint8_t)(source_ptr->output_1_7_feedback);
        break;

        case  PDM_CHANNEL_2:
        case  PDM_CHANNEL_8:
            output_feedback = (uint8_t)(source_ptr->output_2_8_feedback);
        break;

        case  PDM_CHANNEL_3:
        case  PDM_CHANNEL_9:
            output_feedback = (uint8_t)(source_ptr->output_3_9_feedback);
        break;

        case  PDM_CHANNEL_4:
        case  PDM_CHANNEL_10:
            output_feedback = (uint8_t)(source_ptr->output_4_10_feedback);
        break;

        case  PDM_CHANNEL_5:
        case  PDM_CHANNEL_11:
            output_feedback = (uint8_t)(source_ptr->output_5_11_feedback);
        break;

        case  PDM_CHANNEL_6:
        case  PDM_CHANNEL_12:
            output_feedback = (uint8_t)(source_ptr->output_6_12_feedback);
        break;
    }
    return output_feedback;
}



//=============================================================================
//
// uint8_t pdm_get_output_function_handshake_soft_start_step_size(
//     device_instances_t device,
//    pdm_channel_numbers_t channel)
//
//=============================================================================
//
uint8_t pdm_get_soft_start_step_size(
    device_instances_t device,
    pdm_dio_channels_t channel)
{
    // Need channel early in function, verify now.
    if ((channel) > 12 || (channel < 1)) {DEBUG("Invalid channel");}

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    output_function_handshake_t *source_ptr =
        &(device_data_ptr->output_function_handshake[channel]);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (uint8_t)(source_ptr->soft_start_step_size);
}

//=============================================================================
//
// uint8_t pdm_get_output_function_handshake_motor_lamp_mode(
//     device_instances_t device,
//     pdm_channel_numbers_t channel)
//
//=============================================================================
//
uint8_t pdm_get_motor_lamp_mode(
    device_instances_t device,
    pdm_dio_channels_t channel)
{
    // Need channel early in function, verify now.
    if ((channel) > 12 || (channel < 1)) {DEBUG("Invalid channel");}

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    output_function_handshake_t *source_ptr =
        &(device_data_ptr->output_function_handshake[channel]);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (uint8_t)(source_ptr->motor_lamp_mode);
}

//=============================================================================
//
// uint8_t pdm_get_output_function_handshake_loss_of_communication(
//     device_instances_t device,
//     pdm_channel_numbers_t channel)
//
//=============================================================================
//
uint8_t pdm_get_loss_of_communication(
    device_instances_t device,
    pdm_dio_channels_t channel)
{
    // Need channel early in function, verify now.
    if ((channel) > 12 || (channel < 1)) {DEBUG("Invalid channel");}

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    output_function_handshake_t *source_ptr =
        &(device_data_ptr->output_function_handshake[channel]);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (uint8_t)(source_ptr->loss_of_communication);
}

//=============================================================================
//
// uint8_t pdm_get_output_function_handshake_power_on_reset_command(
//     device_instances_t device,
//     pdm_channel_numbers_t channel)
//
//=============================================================================
//
uint8_t pdm_get_power_on_reset_command(
    device_instances_t device,
    pdm_dio_channels_t channel)
{
    // Need channel early in function, verify now.
    if ((channel) > 12 || (channel < 1)) {DEBUG("Invalid channel");}

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    output_function_handshake_t *source_ptr =
        &(device_data_ptr->output_function_handshake[channel]);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (uint8_t)(device_data_ptr->output_function_handshake[channel].power_on_reset_command);
}

//=============================================================================
//
// bool_t_t pdm_get_output_function_handshake_power_on_reset_enable(
//     device_instances_t device,
//     pdm_channel_numbers_t channel)
//
//=============================================================================
//
bool_t pdm_get_power_on_reset_enable(
    device_instances_t device,
    pdm_dio_channels_t channel)
{
    // Need channel early in function, verify now.
    if ((channel) > 12 || (channel < 1)) {DEBUG("Invalid channel");}

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    output_function_handshake_t *source_ptr =
        &(device_data_ptr->output_function_handshake[channel]);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)(source_ptr->power_on_reset_enable);
}

//=============================================================================
//
// bool_t pdm_get_output_function_handshake_command_type(
//     device_instances_t device,
//     pdm_channel_numbers_t channel)
//
//=============================================================================
//
bool_t pdm_get_command_type(
    device_instances_t device,
    pdm_dio_channels_t channel)
{
    // Need channel early in function, verify now.
    if ((channel) > 12 || (channel < 1)) {DEBUG("Invalid channel");}

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    output_function_handshake_t *source_ptr =
        &(device_data_ptr->output_function_handshake[channel]);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)(source_ptr->command_type);
}

//=============================================================================
//
// bool_t pdm_get_output_function_handshake_motor_braking(
//     device_instances_t device,
//     pdm_channel_numbers_t channel)
//
//=============================================================================
//
bool_t pdm_get_motor_braking(
    device_instances_t device,
    pdm_dio_channels_t channel)
{
    // Need channel early in function, verify now.
    if ((channel) > 12 || (channel < 1)) {DEBUG("Invalid channel");}

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    output_function_handshake_t *source_ptr =
        &(device_data_ptr->output_function_handshake[channel]);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)(source_ptr->motor_braking);
}

//=============================================================================
//
// uint8_t pdm_get_output_function_handshake_digital_input(
//     device_instances_t device,
//     pdm_channel_numbers_t channel)
//
//=============================================================================
//
uint8_t pdm_get_digital_input(
    device_instances_t device,
    pdm_dio_channels_t channel)
{
    // Need channel early in function, verify now.
    if ((channel) > 12 || (channel < 1)) {DEBUG("Invalid channel");}

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    output_function_handshake_t *source_ptr =
        &(device_data_ptr->output_function_handshake[channel]);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (uint8_t)(source_ptr->digital_input);
}

//=============================================================================
//
// uint8_t pdm_get_output_function_handshake_calibration_time(
//     device_instances_t device,
//     pdm_channel_numbers_t channel)
//
//=============================================================================
//
uint8_t pdm_get_calibration_time(
    device_instances_t device,
    pdm_dio_channels_t channel)
{
    // Need channel early in function, verify now.
    if ((channel) > 12 || (channel < 1)) {DEBUG("Invalid channel");}

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    output_function_handshake_t *source_ptr =
        &(device_data_ptr->output_function_handshake[channel]);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (uint8_t)(source_ptr->calibration_time);
}

//=============================================================================
//
// uint8_t pdm_get_output_function_handshake_response(
//     device_instances_t device,
//     pdm_channel_numbers_t channel)
//
//=============================================================================
//
uint8_t pdm_get_response(
    device_instances_t device,
    pdm_dio_channels_t channel)
{
    // Need channel early in function, verify now.
    if ((channel) > 12 || (channel < 1)) {DEBUG("Invalid channel");}

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    output_function_handshake_t *source_ptr =
        &(device_data_ptr->output_function_handshake[channel]);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (uint8_t)(source_ptr->response);
}


//=============================================================================
//
// uint16_t pdm_get_analog_input_feedback(
//     device_instances_t device,
//     pdm_adc_channels_t channel)
//
//=============================================================================
//
uint16_t pdm_get_analog_input(
    device_instances_t device,
    pdm_adc_channels_t channel)
{
    // Need channel early in function, verify now.
    if ((channel) > PDM_ADC_8 || (channel < PDM_ADC_1_R)) {DEBUG("Invalid channel");}

    uint16_t analog_input = 0xffff;

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get the required source pointers
    //
    analog_in_1_2_digital_in_feedback_t * source_1_2_ptr =
        &(device_data_ptr->analog_in_1_2_digital_in_feedback);
    if (source_1_2_ptr == NULL) {DEBUG("NULL Pointer");}

    analog_in_3_4_digital_out_feedback_t * source_3_4_ptr =
        &(device_data_ptr->analog_in_3_4_digital_out_feedback);
    if (source_3_4_ptr == NULL) {DEBUG("NULL Pointer");}

    analog_in_5_6_battery_sensor_supply_feedback_t * source_5_6_ptr =
        &(device_data_ptr->analog_in_5_6_battery_sensor_supply_feedback);
    if (source_5_6_ptr == NULL) {DEBUG("NULL Pointer");}

    analog_in_7_8_power_version_feedback_t * source_7_8_ptr =
        &(device_data_ptr->analog_in_7_8_power_version_feedback);
    if (source_7_8_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get the data from the specified channel.
    //
    switch (channel)
    {
        case  PDM_ADC_1_R:
            analog_input = (uint16_t)(source_1_2_ptr->analog_input_1);
        break;

        case  PDM_ADC_2_R:
            analog_input = (uint16_t)(source_1_2_ptr->analog_input_2);
        break;

        case  PDM_ADC_3:
            analog_input = (uint16_t)(source_3_4_ptr->analog_input_3);
        break;

        case  PDM_ADC_4:
            analog_input = (uint16_t)(source_3_4_ptr->analog_input_4);
        break;

        case  PDM_ADC_5:
            analog_input = (uint16_t)(source_5_6_ptr->analog_input_5);
        break;

        case  PDM_ADC_6:
            analog_input = (uint16_t)(source_5_6_ptr->analog_input_6);
        break;

        case  PDM_ADC_7:
            analog_input = (uint16_t)(source_7_8_ptr->analog_input_7);
        break;

        case  PDM_ADC_8:
            analog_input = (uint16_t)(source_7_8_ptr->analog_input_8);
        break;
    }
    return analog_input;
}


//=============================================================================
//
// uint8_t
// pdm_get_output_configuration_handshake_current_limit_output(
//     device_instances_t device,
//     pdm_dio_channels_t channel)
//
//=============================================================================
//
uint8_t pdm_get_current_limit_output(
    device_instances_t device,
    pdm_dio_channels_t channel)
{
    uint8_t current_limit_output = 255;
    output_configuration_handshake_t *source_ptr = NULL;

    // Need channel early in function, verify now.
    if ((channel) > 12 || (channel < 1)) {DEBUG("Invalid channel");}

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    if (channel < 7)
    {
        //
        // Have the pointer to this device's data record, now get a
        // pointer to the appropriate receive data structure for channels
        // 1 to 6.
        //
        source_ptr = &(device_data_ptr->output_configuration_1_6_handshake);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}
    }
    else
    {
        //
        // Have the pointer to this device's data record, now get a
        // pointer to the appropriate receive data structure for channels
        // 7 to 12.
        //
        source_ptr = &(device_data_ptr->output_configuration_7_12_handshake);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}
    }

    //
    // Read the appropriate data from the specified channel.
    //
    switch (channel)
    {
    case  PDM_CHANNEL_1:
    case  PDM_CHANNEL_7:
        current_limit_output = (uint8_t)(source_ptr->current_limit_output_1_7);
        break;

    case  PDM_CHANNEL_2:
    case  PDM_CHANNEL_8:
        current_limit_output = (uint8_t)(source_ptr->current_limit_output_2_8);
        break;

    case  PDM_CHANNEL_3:
    case  PDM_CHANNEL_9:
        current_limit_output = (uint8_t)(source_ptr->current_limit_output_3_9);
        break;

    case  PDM_CHANNEL_4:
    case  PDM_CHANNEL_10:
        current_limit_output = (uint8_t)(source_ptr->current_limit_output_4_10);
        break;

    case  PDM_CHANNEL_5:
    case  PDM_CHANNEL_11:
        current_limit_output = (uint8_t)(source_ptr->current_limit_output_5_11);
        break;

    case  PDM_CHANNEL_6:
    case  PDM_CHANNEL_12:
        current_limit_output = (uint8_t)(source_ptr->current_limit_output_6_12);
        break;
    }

    return current_limit_output;
}


//=============================================================================
//
// uint8_t
// pdm_get_output_configuration_handshake_feedback_type_output(
//     device_instances_t device,
//     pdm_dio_channels_t channel)
//
//=============================================================================
//
uint8_t pdm_get_feedback_type_output(
    device_instances_t device,
    pdm_dio_channels_t channel)
{
    uint8_t feedback_type_output = 255;
    output_configuration_handshake_t *source_ptr = NULL;

    // Need channel early in function, verify now.
    if ((channel) > 12 || (channel < 1)) {DEBUG("Invalid channel");}

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get the proper source pointer based on the channel.
    //
    if (channel < 7)
    {
        //
        // Have the pointer to this device's data record, now get a
        // pointer to the appropriate receive data structure for channels
        // 1 to 6.
        //
        source_ptr = &(device_data_ptr->output_configuration_1_6_handshake);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}
    }
    else
    {
        //
        // Have the pointer to this device's data record, now get a
        // pointer to the appropriate receive data structure for channels
        // 7 to 12.
        //
        source_ptr = &(device_data_ptr->output_configuration_7_12_handshake);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}
    }

    //
    // Read the appropriate data from this channel.
    //
    switch (channel)
    {
    case  PDM_CHANNEL_1:
    case  PDM_CHANNEL_7:
        feedback_type_output = (uint8_t)(source_ptr->feedback_type_output_1_7);
        break;

    case  PDM_CHANNEL_2:
    case  PDM_CHANNEL_8:
        feedback_type_output = (uint8_t)(source_ptr->feedback_type_output_2_8);
        break;

    case  PDM_CHANNEL_3:
    case  PDM_CHANNEL_9:
        feedback_type_output = (uint8_t)(source_ptr->feedback_type_output_3_9);
        break;

    case  PDM_CHANNEL_4:
    case  PDM_CHANNEL_10:
        feedback_type_output = (uint8_t)(source_ptr->feedback_type_output_4_10);
        break;

    case  PDM_CHANNEL_5:
    case  PDM_CHANNEL_11:
        feedback_type_output = (uint8_t)(source_ptr->feedback_type_output_5_11);
        break;

    case  PDM_CHANNEL_6:
    case  PDM_CHANNEL_12:
        feedback_type_output = (uint8_t)(source_ptr->feedback_type_output_6_12);
        break;
    }

    return feedback_type_output;
}


//=============================================================================
//
// uint8_t
// pdm_get_output_configuration_handshake_automatic_reset_output(
//     device_instances_t device,
//     pdm_dio_channels_t channel)
//
//=============================================================================
//
uint8_t pdm_get_automatic_reset_output(
    device_instances_t device,
    pdm_dio_channels_t channel)
{
    uint8_t automatic_reset_output = 255;
    output_configuration_handshake_t *source_ptr = NULL;

    // Need channel early in function, verify now.
    if ((channel) > 12 || (channel < 1)) {DEBUG("Invalid channel");}

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    if (channel < 7)
    {
        //
        // Have the pointer to this device's data record, now get a
        // pointer to the appropriate receive data structure for channels
        // 1 to 6.
        //
        source_ptr = &(device_data_ptr->output_configuration_1_6_handshake);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}
    }
    else
    {
        //
        // Have the pointer to this device's data record, now get a
        // pointer to the appropriate receive data structure for channels
        // 7 to 12.
        //
        source_ptr = &(device_data_ptr->output_configuration_7_12_handshake);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}
    }

    //
    // Read the appropriate data from this channel.
    //
    switch (channel)
    {
    case  PDM_CHANNEL_1:
    case  PDM_CHANNEL_7:
        automatic_reset_output = (uint8_t)(source_ptr->automatic_reset_output_1_7);
        break;

    case  PDM_CHANNEL_2:
    case  PDM_CHANNEL_8:
        automatic_reset_output = (uint8_t)(source_ptr->automatic_reset_output_2_8);
        break;

    case  PDM_CHANNEL_3:
    case  PDM_CHANNEL_9:
        automatic_reset_output = (uint8_t)(source_ptr->automatic_reset_output_3_9);
        break;

    case  PDM_CHANNEL_4:
    case  PDM_CHANNEL_10:
        automatic_reset_output = (uint8_t)(source_ptr->automatic_reset_output_4_10);
        break;

    case  PDM_CHANNEL_5:
    case  PDM_CHANNEL_11:
        automatic_reset_output = (uint8_t)(source_ptr->automatic_reset_output_5_11);
        break;

    case  PDM_CHANNEL_6:
    case  PDM_CHANNEL_12:
        automatic_reset_output = (uint8_t)(source_ptr->automatic_reset_output_6_12);
        break;
    }
    return automatic_reset_output;
}


//=============================================================================
//
// uint8_t
// pdm_get_output_configuration_handshake_highside_or_hbridge_ouput(
//     device_instances_t device,
//     pdm_dio_channels_t channel)
//
//=============================================================================
//
uint8_t pdm_get_highside_or_hbridge_ouput(
    device_instances_t device,
    pdm_dio_channels_t channel)
{
    uint8_t highside_or_hbridge_ouput = 255;
    output_configuration_handshake_t *source_ptr = NULL;

    // Need channel early in function, verify now.
    if ((channel) > 12 || (channel < 1)) {DEBUG("Invalid channel");}

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    if (channel < 7)
    {
        //
        // Have the pointer to this device's data record, now get a
        // pointer to the appropriate receive data structure for channels
        // 1 to 6.
        //
        source_ptr = &(device_data_ptr->output_configuration_1_6_handshake);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}
    }
    else
    {
        //
        // Have the pointer to this device's data record, now get a
        // pointer to the appropriate receive data structure for channels
        // 7 to 12.
        //
        source_ptr = &(device_data_ptr->output_configuration_7_12_handshake);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}
    }
    //
    // Read the appropriate data from this channel.
    //
    switch (channel)
    {
    case  PDM_CHANNEL_1:
    case  PDM_CHANNEL_7:
        highside_or_hbridge_ouput = (uint8_t)(source_ptr->highside_or_hbridge_ouput_1_7);
        break;

    case  PDM_CHANNEL_2:
    case  PDM_CHANNEL_8:
        highside_or_hbridge_ouput = (uint8_t)(source_ptr->highside_or_hbridge_ouput_2_8);
        break;

    case  PDM_CHANNEL_3:
    case  PDM_CHANNEL_9:
        highside_or_hbridge_ouput = (uint8_t)(source_ptr->highside_or_hbridge_ouput_3_9);
        break;

    case  PDM_CHANNEL_4:
    case  PDM_CHANNEL_10:
        highside_or_hbridge_ouput = (uint8_t)(source_ptr->highside_or_hbridge_ouput_4_10);
        break;

    case  PDM_CHANNEL_5:
    case  PDM_CHANNEL_11:
        highside_or_hbridge_ouput = (uint8_t)(source_ptr->highside_or_hbridge_ouput_5_11);
        break;

    case  PDM_CHANNEL_6:
    case  PDM_CHANNEL_12:
        highside_or_hbridge_ouput = (uint8_t)(source_ptr->highside_or_hbridge_ouput_6_12);
        break;
    }

    return highside_or_hbridge_ouput;
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
bool_t pdm_get_can_rx_ok(device_instances_t device)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // By default, receive CAN message timeouts are disabled.
    //
    if ((device_data_ptr->analog_in_1_2_digital_in_feedback_rx_ok == TRUE) &&
        (device_data_ptr->analog_in_3_4_digital_out_feedback_rx_ok == TRUE) &&
        (device_data_ptr->analog_in_5_6_battery_sensor_supply_feedback_rx_ok == TRUE) &&
        (device_data_ptr->analog_in_7_8_power_version_feedback_rx_ok == TRUE) &&
        (device_data_ptr->output_current_1_6_feedback_rx_ok == TRUE) &&
        (device_data_ptr->output_current_7_12_feedback_rx_ok == TRUE) &&
        (device_data_ptr->output_function_handshake_rx_ok == TRUE) &&
        (device_data_ptr->output_configuration_1_6_handshake_rx_ok == TRUE) &&
        (device_data_ptr->output_configuration_7_12_handshake_rx_ok == TRUE))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/******************************************************************************
 *
 *        Name: pdm_compare_analog_in_1_2_digital_in_feedback_rx_cnt()
 *
 * Description: Each time a registered receive CAN messsage is
 *              received, its receive function increments the count
 *              assiciated with its message. This function compares
 *              this count with the compare_cnt parameter. If the
 *              receive message count is >= the compare count this
 *              function returns TRUE, otherwise it returns FALSE.
 *
 *              Note that the message's receive counter may be set to
 *              zero by a call to pdm_zero_ ___ rx_cnt(), where ___ is
 *              the name of the message. These pdm_zero_ ___ rx_cnt()
 *              are located in the pdm_device_setters.c module.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
bool_t pdm_compare_analog_in_1_2_digital_in_feedback_rx_cnt(
    device_instances_t device, uint32_t compare_cnt)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    if (device_data_ptr->analog_in_1_2_digital_in_feedback_rx_cnt % compare_cnt == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/******************************************************************************
 *
 *        Name: pdm_compare_analog_in_3_4_digital_out_feedback_rx_cnt()
 *
 * Description: Each time a registered receive CAN messsage is
 *              received, its receive function increments the count
 *              assiciated with its message. This function compares
 *              this count with the compare_cnt parameter. If the
 *              receive message count is >= the compare count this
 *              function returns TRUE, otherwise it returns FALSE.
 *
 *              Note that the message's receive counter may be set to
 *              zero by a call to pdm_zero_ ___ rx_cnt(), where ___ is
 *              the name of the message. These pdm_zero_ ___ rx_cnt()
 *              are located in the pdm_device_setters.c module.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
bool_t pdm_compare_analog_in_3_4_digital_out_feedback_rx_cnt(
    device_instances_t device, uint32_t compare_cnt)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    if (device_data_ptr->analog_in_3_4_digital_out_feedback_rx_cnt % compare_cnt == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/******************************************************************************
 *
 *        Name: pdm_compare_analog_in_5_6_battery_sensor_supply_feedback_rx_cnt()
 *
 * Description: Each time a registered receive CAN messsage is
 *              received, its receive function increments the count
 *              assiciated with its message. This function compares
 *              this count with the compare_cnt parameter. If the
 *              receive message count is >= the compare count this
 *              function returns TRUE, otherwise it returns FALSE.
 *
 *              Note that the message's receive counter may be set to
 *              zero by a call to pdm_zero_ ___ rx_cnt(), where ___ is
 *              the name of the message. These pdm_zero_ ___ rx_cnt()
 *              are located in the pdm_device_setters.c module.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
bool_t pdm_compare_analog_in_5_6_battery_sensor_supply_feedback_rx_cnt(
    device_instances_t device, uint32_t compare_cnt)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    if (device_data_ptr->analog_in_5_6_battery_sensor_supply_feedback_rx_cnt % compare_cnt == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/******************************************************************************
 *
 *        Name: pdm_compare_analog_in_7_8_power_version_feedback_rx_cnt()
 *
 * Description: Each time a registered receive CAN messsage is
 *              received, its receive function increments the count
 *              assiciated with its message. This function compares
 *              this count with the compare_cnt parameter. If the
 *              receive message count is >= the compare count this
 *              function returns TRUE, otherwise it returns FALSE.
 *
 *              Note that the message's receive counter may be set to
 *              zero by a call to pdm_zero_ ___ rx_cnt(), where ___ is
 *              the name of the message. These pdm_zero_ ___ rx_cnt()
 *              are located in the pdm_device_setters.c module.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
bool_t pdm_compare_analog_in_7_8_power_version_feedback_rx_cnt(
    device_instances_t device, uint32_t compare_cnt)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    if (device_data_ptr->analog_in_7_8_power_version_feedback_rx_cnt % compare_cnt == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/******************************************************************************
 *
 *        Name: pdm_compare_output_current_1_6_feedback_rx_cnt()
 *
 * Description: Each time a registered receive CAN messsage is
 *              received, its receive function increments the count
 *              assiciated with its message. This function compares
 *              this count with the compare_cnt parameter. If the
 *              receive message count is >= the compare count this
 *              function returns TRUE, otherwise it returns FALSE.
 *
 *              Note that the message's receive counter may be set to
 *              zero by a call to pdm_zero_ ___ rx_cnt(), where ___ is
 *              the name of the message. These pdm_zero_ ___ rx_cnt()
 *              are located in the pdm_device_setters.c module.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
bool_t pdm_compare_output_current_1_6_feedback_rx_cnt(
    device_instances_t device, uint32_t compare_cnt)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    if (device_data_ptr->output_current_1_6_feedback_rx_cnt % compare_cnt == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/******************************************************************************
 *
 *        Name: pdm_compare_output_current_7_12_feedback_rx_cnt()
 *
 * Description: Each time a registered receive CAN messsage is
 *              received, its receive function increments the count
 *              assiciated with its message. This function compares
 *              this count with the compare_cnt parameter. If the
 *              receive message count is >= the compare count this
 *              function returns TRUE, otherwise it returns FALSE.
 *
 *              Note that the message's receive counter may be set to
 *              zero by a call to pdm_zero_ ___ rx_cnt(), where ___ is
 *              the name of the message. These pdm_zero_ ___ rx_cnt()
 *              are located in the pdm_device_setters.c module.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
bool_t pdm_compare_output_current_7_12_feedback_rx_cnt(
    device_instances_t device, uint32_t compare_cnt)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    if (device_data_ptr->output_current_7_12_feedback_rx_cnt % compare_cnt == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/******************************************************************************
 *
 *        Name: pdm_compare_output_function_handshake_rx_cnt()
 *
 * Description: Each time a registered receive CAN messsage is
 *              received, its receive function increments the count
 *              assiciated with its message. This function compares
 *              this count with the compare_cnt parameter. If the
 *              receive message count is >= the compare count this
 *              function returns TRUE, otherwise it returns FALSE.
 *
 *              Note that the message's receive counter may be set to
 *              zero by a call to pdm_zero_ ___ rx_cnt(), where ___ is
 *              the name of the message. These pdm_zero_ ___ rx_cnt()
 *              are located in the pdm_device_setters.c module.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
bool_t pdm_compare_output_function_handshake_rx_cnt(
    device_instances_t device, uint32_t compare_cnt)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    if (device_data_ptr->output_function_handshake_rx_cnt % compare_cnt == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/******************************************************************************
 *
 *        Name: pdm_compare_output_configuration_1_6_handshake_rx_cnt()
 *
 * Description: Each time a registered receive CAN messsage is
 *              received, its receive function increments the count
 *              assiciated with its message. This function compares
 *              this count with the compare_cnt parameter. If the
 *              receive message count is >= the compare count this
 *              function returns TRUE, otherwise it returns FALSE.
 *
 *              Note that the message's receive counter may be set to
 *              zero by a call to pdm_zero_ ___ rx_cnt(), where ___ is
 *              the name of the message. These pdm_zero_ ___ rx_cnt()
 *              are located in the pdm_device_setters.c module.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
bool_t pdm_compare_output_configuration_1_6_handshake_rx_cnt(
    device_instances_t device, uint32_t compare_cnt)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    if (device_data_ptr->output_configuration_1_6_handshake_rx_cnt % compare_cnt == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/******************************************************************************
 *
 *        Name: pdm_compare_output_configuration_7_12_handshake_rx_cnt()
 *
 * Description: Each time a registered receive CAN messsage is
 *              received, its receive function increments the count
 *              assiciated with its message. This function compares
 *              this count with the compare_cnt parameter. If the
 *              receive message count is >= the compare count this
 *              function returns TRUE, otherwise it returns FALSE.
 *
 *              Note that the message's receive counter may be set to
 *              zero by a call to pdm_zero_ ___ rx_cnt(), where ___ is
 *              the name of the message. These pdm_zero_ ___ rx_cnt()
 *              are located in the pdm_device_setters.c module.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
bool_t pdm_compare_output_configuration_7_12_handshake_rx_cnt(
    device_instances_t device, uint32_t compare_cnt)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    if (device_data_ptr->output_configuration_7_12_handshake_rx_cnt % compare_cnt == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
