/******************************************************************************
 *
 *        Name: pdm_setters.c
 *
 * Description:
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
// void pdm_set_configure_output_function(device_instances_t device,
//                                        pdm_dio_channels_t channel,
//                                        uint8_t soft_start_step,
//                                        uint8_t motor_lamp_mode,
//                                        uint8_t loss_of_comm,
//                                        uint8_t por_command,
//                                        uint8_t por_enable,
//                                        uint8_t command_type,
//                                        uint8_t motor_braking,
//                                        uint8_t lsc_digital_input,
//                                        uint8_t response_to_input)
//
//=============================================================================
//
void pdm_set_configure_output_function(
    device_instances_t device,
    pdm_dio_channels_t channel,
    uint8_t soft_start_step_percent,
    pdm_output_motor_lamp_mode_t motor_lamp_mode,
    pdm_configure_channel_loss_of_comm_t loss_of_comm,
    int8_t por_command_percent,
    pdm_configure_output_por_enable_t por_enable,
    pdm_configure_output_command_type_t command_type,
    pdm_configure_output_motor_breaking_t motor_breaking,
    pdm_configure_output_lsc_channel_t lsc_digital_input,
    pdm_configure_output_channel_response_t response_to_input)
{
    // Need channel early in function, verify now.
    if ((channel < 1) || (channel > 12))
        DEBUG("Invalid channel");

    configure_output_function_t *dest_ptr = NULL;

    //
    // Limit the soft start step percent.
    //
    if (soft_start_step_percent > 100)
    {
        soft_start_step_percent = 100;
    }

    //
    // Calculate the command byte from the percent parameter.
    //
    uint8_t soft_start_step_command = soft_start_step_percent;

    //
    // Limit the por_command_percent to between -100% to 100%.
    //
    if (por_command_percent < -100)
    {
        por_command_percent = -100;
    }
    else if (por_command_percent > 100)
    {
        por_command_percent = 100;
    }

    //
    // Calculate the por_command_byte to indicate the PWM value from
    // the -100% to 100% command_percent parameter. This is applied on
    // power up reset of PDM.
    //
    int8_t por_command_byte = (int8_t)(por_command_percent * 0.16);
    if (por_command_percent < 0)
        por_command_byte = por_command_byte & 0x1f;

    if (por_command_percent == 100)
        por_command_byte -= 1;

    //
    // Get a pointer to the proper device data structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->configure_output_function[channel - 1]);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_configure_output_function_ptr;
    if (tx_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Cast our pointer to the 8-byte Can_Message_ tx message data to
    // be a pointer to be of the type that's associated with this
    // function's transmit obkect.
    //
    configure_output_function_t *can_data_ptr =
        (configure_output_function_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Populate the PDM's unique identifier.
    //
    dest_ptr->identifier = tx_ptr->j1939_byte;

    //
    // Populate with provided and verified parameters.
    //
    dest_ptr->output_channel_number = channel;
    dest_ptr->soft_start_step = soft_start_step_command;
    dest_ptr->motor_lamp_mode = motor_lamp_mode;
    dest_ptr->loss_of_comm = loss_of_comm;
    dest_ptr->por_command = por_command_byte;
    dest_ptr->por_enable = por_enable;
    dest_ptr->command_type = command_type;
    dest_ptr->motor_breaking = motor_breaking;
    dest_ptr->lsc_digital_input = lsc_digital_input;
    dest_ptr->response_to_input = response_to_input;

    //
    // Populate fixed fields according to reference manual.
    //
    // Well, almost. The reference manual says to do this, but
    // instead, this 6-bit field should be cleared to zeros.
    //
    dest_ptr->reserved_6 = 0x3f;
    //dest_ptr->reserved_6 = 0x00;

    dest_ptr->reserved_8 = 0xff;
    dest_ptr->calibration_time = 0x3;

}


//=============================================================================
//
// void pdm_set_configure_output_channel(
//     device_instances_t device,
//     pdm_dio_channels_t channel,
//     pdm_output_channel_current_limit_t current_limit,
//     uint8_t automatic_reset,
//     uint8_t highside_or_hbridge)
//
//=============================================================================
//
void pdm_set_configure_output_channel(
    device_instances_t device,
    pdm_dio_channels_t channel,
    pdm_output_channel_current_limit_t current_limit,
    pdm_configure_channel_automatic_reset_t automatic_reset,
    pdm_configure_channel_high_or_bridge_t highside_or_hbridge)
{
    // Need channel early in function, verify now.
    if ((channel < 1) || (channel > 12))
        DEBUG("Invalid channel");

	can_tx_registration_t *tx_ptr = NULL;
    configure_output_channels_t *dest_ptr = NULL;

    //
    // Get a pointer to the proper device data structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the proper output configure structure. There are
    // two structures, one for channels 1-6, and one for channels
    // 7-12.
    //
    if (channel <= 6)
    {
        dest_ptr = &(device_data_ptr->configure_output_channels_1_6);
        if (dest_ptr == NULL) {DEBUG("NULL Pointer");}
    }
    else
    {
        dest_ptr = &(device_data_ptr->configure_output_channels_7_12);
        if (dest_ptr == NULL) {DEBUG("NULL Pointer");}
    }

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    // Is the channel between 1 and 6?
    if (channel <= 6)
    {
        // Yes, use the _1_6 structure pointer.
        tx_ptr = device_data_ptr->send_configure_output_channels_1_6_ptr;
        if (tx_ptr == NULL) {DEBUG("NULL Pointer");}
    }
    else
    {
        // No, use the _7_12 structure pointer.
        tx_ptr = device_data_ptr->send_configure_output_channels_7_12_ptr;
        if (tx_ptr == NULL) {DEBUG("NULL Pointer");}
    }

    //
    // Cast our pointer to the 8-byte Can_Message_ tx message data to
    // be a pointer to be of the type that's associated with this
    // function's transmit object.
    //
        configure_output_channels_t *can_data_ptr =
            (configure_output_channels_t *)tx_ptr->tx_message.data;
        if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Populate the PDM's unique identifier with the j1939_byte.
    //
    dest_ptr->output_channel_group_id = tx_ptr->j1939_byte;

    //
    // Populate with the verified parameters. The lvalue to populate
    // depends on the channel number. See the
    // configure_output_channels_t structure for details.
    //
    if (channel == 1 || channel == 7)
    {
        // Channel is either 1 or 7.
        dest_ptr->current_limit_1_7 = current_limit;
        // Only current feedback supported (3)
        dest_ptr->feedback_type_1_7 = 0x3;
        dest_ptr->automatic_reset_1_7 = automatic_reset;
        dest_ptr->highside_or_hbridge_1_7 = highside_or_hbridge;
    }
    else if (channel == 2 || channel == 8)
    {
        // Channel is either 2 or 8.
        dest_ptr->current_limit_2_8 = current_limit;
        // Only current feedback supported (3)
        dest_ptr->feedback_type_2_8 = 0x3;
        dest_ptr->automatic_reset_2_8 = automatic_reset;
        dest_ptr->highside_or_hbridge_2_8 = highside_or_hbridge;
    }
    else if (channel == 3 || channel == 9)
    {
        // Channel is either 3 or 9.
        dest_ptr->current_limit_3_9 = current_limit;
        // Only current feedback supported (3)
        dest_ptr->feedback_type_3_9 = 0x3;
        dest_ptr->automatic_reset_3_9 = automatic_reset;
        dest_ptr->highside_or_hbridge_3_9 = highside_or_hbridge;
    }
    else if (channel == 4 || channel == 10)
    {
        // Channel is either 4 or 10.
        dest_ptr->current_limit_4_10 = current_limit;
        // Only current feedback supported (3)
        dest_ptr->feedback_type_4_10 = 0x3;
        dest_ptr->automatic_reset_4_10 = automatic_reset;
        dest_ptr->highside_or_hbridge_4_10 = highside_or_hbridge;
    }
    else if (channel == 5 || channel == 11)
    {
        // Channel is either 5 or 11.
        dest_ptr->current_limit_5_11 = current_limit;
        // Only current feedback supported (3)
        dest_ptr->feedback_type_5_11 = 0x3;
        dest_ptr->automatic_reset_5_11 = automatic_reset;
        dest_ptr->highside_or_hbridge_5_11 = highside_or_hbridge;
    }
    else
    {
        // Channel is either 6 or 12.
        dest_ptr->current_limit_6_12 = current_limit;
        // Only current feedback supported (3)
        dest_ptr->feedback_type_6_12 = 0x3;
        dest_ptr->automatic_reset_6_12 = automatic_reset;
        dest_ptr->highside_or_hbridge_6_12 = highside_or_hbridge;
    }

    //
    // Populate fixed fields according to reference manual.
    //
    dest_ptr->reserved = 0xff;

}


//=============================================================================
//
// void pdm_set_command_output_channel(device_instances_t device,
//                                     pdm_dio_channels_t channel,
//                                     int8_t command,
//                                     bool_t enable,
//                                     uint8_t module_tranmit_rate)
//
//=============================================================================
//
void pdm_set_command_output_channel(
    device_instances_t device,
    pdm_dio_channels_t channel,
    int8_t command_percent,
    bool_t enable,
    pdm_module_tx_rate_t module_transmit_rate)
{
    // Need channel early in function, verify now.
    if ((channel < 1) || (channel > 12))
        DEBUG("Invalid channel");

    command_output_channels_t *dest_ptr = NULL;
    can_tx_registration_t *tx_ptr = NULL;

    //
    // Limit the command_percent to between -100% to 100%.
    //
    if (command_percent < -100)
    {
        command_percent = -100;
    }
    if (command_percent > 100)
    {
        command_percent = 100;
    }

    //
    // Calculate the command_byte to indicate the PWM value from the
    // -100% to 100% command_percent parameter.
    //
    //      0% PWM = 0x00
    //   +100% PWM = 0x7f
    //   -100% PWM = 0x80
    //     -1% PWM = 0xff
    //
    int8_t command_byte = (int8_t)(command_percent * 1.27);

    //
    // Get a pointer to the proper device data structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the proper output command structure. There are
    // two structures, one for channels 1-6, and one for channels
    // 7-12.
    //
    if (channel <= 6)
    {
        dest_ptr = &(device_data_ptr->command_output_channels_1_6);
        if (dest_ptr == NULL) {DEBUG("NULL Pointer");}
    }
    else
    {
        dest_ptr = &(device_data_ptr->command_output_channels_7_12);
        if (dest_ptr == NULL) {DEBUG("NULL Pointer");}
    }

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    // Is the channel between 1 and 6?
    if (channel <= 6)
    {
        // Yes, use the _1_6 structure pointer.
        tx_ptr = device_data_ptr->send_command_output_channels_1_6_ptr;
        if (tx_ptr == NULL) {DEBUG("NULL Pointer");}
    }
    else
    {
        // No, use the _7_12 structure pointer.
        tx_ptr = device_data_ptr->send_command_output_channels_7_12_ptr;
        if (tx_ptr == NULL) {DEBUG("NULL Pointer");}
    }

    //
    // Cast our pointer to the 8-byte Can_Message_ tx message data to
    // be a pointer to be of the type that's associated with this
    // function's transmit object.
    //
    command_output_channels_t *can_data_ptr =
        (command_output_channels_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Populate the PDM's unique identifier with the j1939_byte.
    //
    dest_ptr->output_command_identifier = tx_ptr->j1939_byte;

    //
    // Populate the specified channel's command value and enable with
    // the verified parameters. The command is a PWM value of -100% to
    // 100%.
    //
    switch (channel)
    {
    case PDM_CHANNEL_1:
    case PDM_CHANNEL_7:
        dest_ptr->command_1_7 = command_byte;
        dest_ptr->enable_1_7 = enable;
        break;

    case PDM_CHANNEL_2:
    case PDM_CHANNEL_8:
        dest_ptr->command_2_8 = command_byte;
        dest_ptr->enable_2_8 = enable;
        break;

    case PDM_CHANNEL_3:
    case PDM_CHANNEL_9:
        dest_ptr->command_3_9 = command_byte;
        dest_ptr->enable_3_9 = enable;
        break;

    case PDM_CHANNEL_4:
    case PDM_CHANNEL_10:
        dest_ptr->command_4_10 = command_byte;
        dest_ptr->enable_4_10 = enable;
        break;

    case PDM_CHANNEL_5:
    case PDM_CHANNEL_11:
        dest_ptr->command_5_11 = command_byte;
        dest_ptr->enable_5_11 = enable;
        break;

    case PDM_CHANNEL_6:
    case PDM_CHANNEL_12:
        dest_ptr->command_6_12 = command_byte;
        dest_ptr->enable_6_12 = enable;
        break;
    }

    //
    // Populate the module_transmit_rate.  A decision was made to
    // create this setter function with the ability to set one channel
    // at a time. Although this reduces the number of arguments
    // required, when calling this function, from twelve channels to
    // one channel, it does present an issue in that there is a PDM
    // parameter that is device wide in this structure. This means
    // that the module_transmit_rate parameter must be set the same to
    // each call to this setter.
    //
    dest_ptr->module_transmit_rate = module_transmit_rate;

}


/******************************************************************************
 *
 *        Name: pdm_zero_analog_in_1_2_digital_in_feedback_rx_cnt()
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
void pdm_zero_analog_in_1_2_digital_in_feedback_rx_cnt(
    device_instances_t device)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    device_data_ptr->analog_in_1_2_digital_in_feedback_rx_cnt = 0;
}

/******************************************************************************
 *
 *        Name: pdm_zero_analog_in_3_4_digital_out_feedback_rx_cnt()
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
void pdm_zero_analog_in_3_4_digital_out_feedback_rx_cnt(
    device_instances_t device)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    device_data_ptr->analog_in_3_4_digital_out_feedback_rx_cnt = 0;
}


/******************************************************************************
 *
 *        Name: pdm_zero_analog_in_5_6_battery_sensor_supply_feedback_rx_cnt()
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
void pdm_zero_analog_in_5_6_battery_sensor_supply_feedback_rx_cnt(
    device_instances_t device)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    device_data_ptr->analog_in_5_6_battery_sensor_supply_feedback_rx_cnt = 0;
}


/******************************************************************************
 *
 *        Name: pdm_zero_analog_in_7_8_power_version_feedback_rx_cnt()
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
void pdm_zero_analog_in_7_8_power_version_feedback_rx_cnt(
    device_instances_t device)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    device_data_ptr->analog_in_7_8_power_version_feedback_rx_cnt = 0;
}


/******************************************************************************
 *
 *        Name: pdm_zero_output_current_1_6_feedback_rx_cnt()
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
void pdm_zero_output_current_1_6_feedback_rx_cnt(
    device_instances_t device)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    device_data_ptr->output_current_1_6_feedback_rx_cnt = 0;
}


/******************************************************************************
 *
 *        Name: pdm_zero_output_current_7_12_feedback_rx_cnt()
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
void pdm_zero_output_current_7_12_feedback_rx_cnt(
    device_instances_t device)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    device_data_ptr->output_current_7_12_feedback_rx_cnt = 0;
}


/******************************************************************************
 *
 *        Name: pdm_zero_output_function_handshake_rx_cnt()
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
void pdm_zero_output_function_handshake_rx_cnt(
    device_instances_t device)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    device_data_ptr->output_function_handshake_rx_cnt = 0;
}


/******************************************************************************
 *
 *        Name: pdm_zero_output_configuration_1_6_handshake_rx_cnt()
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
void pdm_zero_output_configuration_1_6_handshake_rx_cnt(
    device_instances_t device)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    device_data_ptr->output_configuration_1_6_handshake_rx_cnt = 0;
}


/******************************************************************************
 *
 *        Name: pdm_zero_output_configuration_7_12_handshake_rx_cnt()
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
void pdm_zero_output_configuration_7_12_handshake_rx_cnt(
    device_instances_t device)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    device_data_ptr->output_configuration_7_12_handshake_rx_cnt = 0;
}

//=============================================================================
//
//          Name: pdm_get_output_channel_command_state()
//
//=============================================================================
//
bool_t pdm_get_output_channel_command_state(
    device_instances_t device,
    pdm_dio_channels_t channel)
{

    bool_t enable_state = 0;

    // Need channel early in function, verify now.
    if ((channel < 1) || (channel > 12))
        DEBUG("Invalid channel");

    command_output_channels_t *dest_ptr = NULL;
    can_tx_registration_t *tx_ptr = NULL;

    //
    // Get a pointer to the proper device data structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the proper output command structure. There are
    // two structures, one for channels 1-6, and one for channels
    // 7-12.
    //
    if (channel <= 6)
    {
        dest_ptr = &(device_data_ptr->command_output_channels_1_6);
        if (dest_ptr == NULL) {DEBUG("NULL Pointer");}
    }
    else
    {
        dest_ptr = &(device_data_ptr->command_output_channels_7_12);
        if (dest_ptr == NULL) {DEBUG("NULL Pointer");}
    }

    //
    // Populate the specified channel's command value and enable with
    // the verified parameters. The command is a PWM value of -100% to
    // 100%.
    //
    switch (channel)
    {
    case PDM_CHANNEL_1:
    case PDM_CHANNEL_7:
        enable_state = dest_ptr->enable_1_7;
        break;

    case PDM_CHANNEL_2:
    case PDM_CHANNEL_8:
        enable_state = dest_ptr->enable_2_8;
        break;

    case PDM_CHANNEL_3:
    case PDM_CHANNEL_9:
        enable_state = dest_ptr->enable_3_9;
        break;

    case PDM_CHANNEL_4:
    case PDM_CHANNEL_10:
        enable_state = dest_ptr->enable_4_10;
        break;

    case PDM_CHANNEL_5:
    case PDM_CHANNEL_11:
        enable_state = dest_ptr->enable_5_11;
        break;

    case PDM_CHANNEL_6:
    case PDM_CHANNEL_12:
        enable_state = dest_ptr->enable_6_12;
        break;
    }

    return enable_state;

}
