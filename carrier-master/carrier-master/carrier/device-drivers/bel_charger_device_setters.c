/******************************************************************************
 *
 *        Name: fvt_bel_charger_setters.c
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Thursday, 04 July 2019
 *
 ******************************************************************************
 */

#include "bel_charger_device_private.h"

extern device_data_t *bel_charger_first_device_data_ptr;

//=============================================================================
//
// void bel_set_charger_setpoint(
//    device_instances_t device,
//    uint16_t current_command,
//    uint16_t battery_voltage_limit,
//   uint8_t charger_enable)
//
//=============================================================================
//
void bel_set_setpoint(
    device_instances_t device,
    uint16_t current_command,
    uint16_t battery_voltage_limit,
    uint8_t charger_enable)
{
    bel_charger_tx_setpoint_t *dest_ptr = NULL;

    //
    // Get a pointer to the proper device data structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->setpoints);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_setpoint_j1939_can_ptr;
    if (tx_ptr == NULL) {DEBUG("NULL Pointer");}

    // Cast our pointer to the 8-byte Can_Message_ tx message data to
    // be a pointer to the Bel charger's 8-byte structure that
    // represents the data that this function is sending over CAN.
    bel_charger_tx_setpoint_t *can_data_ptr =
        (bel_charger_tx_setpoint_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Populate the PDM's unique identifier with the j1939_word.
    //
    dest_ptr->j1939_word = tx_ptr->j1939_word;

    //
    // Populate the specified channel's command value and enable with
    // the verified parameters.
    //
    dest_ptr->current_command = BYTE_SWAP16(current_command);
    dest_ptr->battery_voltage_limit = BYTE_SWAP16(battery_voltage_limit);
    dest_ptr->charger_enable = charger_enable;
    dest_ptr->unused_7 = 0xff;
}

//=============================================================================
//
// void bel_set_gpio(
//     device_instances_t device,
//     bool_t wake_up_ecu_enable,
//     bool_t gpo_enable)
//
//=============================================================================
//
void bel_set_gpio(
    device_instances_t device,
    bool_t wake_up_ecu_enable,
    bool_t gpo_enable)
{
    bel_charger_tx_gpio_t *dest_ptr = NULL;

    //
    // Get a pointer to the proper device data structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->gpios);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_gpio_setting_j1939_can_ptr;
    if (tx_ptr == NULL) {DEBUG("NULL Pointer");}

    // Cast our pointer to the 8-byte Can_Message_ tx message data to
    // be a pointer to the Bel charger's 8-byte structure that
    // represents the data that this function is sending over CAN.
    bel_charger_tx_gpio_t *can_data_ptr =
        (bel_charger_tx_gpio_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Populate the PDM's unique identifier with the j1939_byte.
    //
    dest_ptr->j1939_word = tx_ptr->j1939_word;

    //
    // Populate the specified channel's command value and enable with
    // the verified parameters.
    //
    dest_ptr->wake_up_ecu_en = wake_up_ecu_enable;
    dest_ptr->gpo_en = gpo_enable;
    dest_ptr->unused_bits = 0;
    dest_ptr->unused_32bits = 0xffffffff;
}




//=============================================================================
//
// void bel_set_battery_under_over_voltage_limits(
//     device_instances_t device,
//     uint16_t battery_undervoltage_limit,
//     uint16_t battery_overvoltage_limit)
//
//=============================================================================
//
void bel_set_battery_under_over_voltage_limits(
    device_instances_t device,
    uint16_t battery_undervoltage_limit,
    uint16_t battery_overvoltage_limit)
{
    bel_charger_tx_battery_under_over_voltage_limits_t *dest_ptr = NULL;

    //
    // Get a pointer to the proper device data structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->limits);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_battery_voltage_limits_j1939_can_ptr;
    if (tx_ptr == NULL) {DEBUG("NULL Pointer");}

    // Cast our pointer to the 8-byte Can_Message_ tx message data to
    // be a pointer to the Bel charger's 8-byte structure that
    // represents the data that this function is sending over CAN.
    bel_charger_tx_battery_under_over_voltage_limits_t *can_data_ptr =
        (bel_charger_tx_battery_under_over_voltage_limits_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Populate the PDM's unique identifier with the j1939_byte.
    //
    dest_ptr->j1939_word = tx_ptr->j1939_word;

    //
    // Populate the specified channel's command value and enable with
    // the verified parameters.
    //
    dest_ptr->battery_undervoltage_limit = BYTE_SWAP16(battery_undervoltage_limit);
    dest_ptr->battery_overvoltage_limit = BYTE_SWAP16(battery_overvoltage_limit);
    dest_ptr->unused_6 = 0xff;
    dest_ptr->unused_7 = 0xff;

}


void bel_set_led_duty_cycle(
    device_instances_t device,
    uint8_t led_duty_cycle)
{
    bel_charger_tx_led_t *dest_ptr = NULL;

    //
    // Get a pointer to the proper device data structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->leds);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_led_setting_j1939_can_ptr;
    if (tx_ptr == NULL) {DEBUG("NULL Pointer");}

    // Cast our pointer to the 8-byte Can_Message_ tx message data to
    // be a pointer to the Bel charger's 8-byte structure that
    // represents the data that this function is sending over CAN.
    bel_charger_tx_led_t *can_data_ptr =
        (bel_charger_tx_led_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Populate the PDM's unique identifier with the j1939_byte.
    //
    dest_ptr->j1939_word = tx_ptr->j1939_word;

    //
    // Populate the specified channel's command value and enable with
    // the verified parameters.
    //
    dest_ptr->led_duty = BYTE_SWAP16(led_duty_cycle);

}
