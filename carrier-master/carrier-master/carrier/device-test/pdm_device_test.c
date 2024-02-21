/******************************************************************************
 *
 *        Name: pdm_device_test.c
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Thursday, 25 July 2019
 *
 ******************************************************************************
 */

#include "Prototypes_CAN.h"
#include "can_service.h"
#include "pdm_device.h"
#include "pdm_device_test.h"


//////////////////////////////////////////////////////////////////////////////
//
// Test the PDM initialization function over several device instances.
//
//////////////////////////////////////////////////////////////////////////////
//
void pdm_test_init(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line)
{
    //
    // Note: The PDM CAN interface supports only 250kbps. Use only on
    // CAN2 which we always configure, in Orchestra, as 250kbps.
    //
    pdm_init(device, module_id, can_line);

    //
    // Set the PDM receive message timeout limits.
    //
    pdm_test_set_timeout(device, module_id, can_line);

    //
    // Configure the output functions and channels.
    //
    pdm_test_configure_output_function_all_channels(device);
    pdm_test_configure_all_output_channels(device);


}


/******************************************************************************
 *
 *        Name:pdm_test_set_timeout()
 *
 * Description: Test the PDM receive CAN message timeouts.
 *
 *      Author: Tom
 *        Date: Saturday, 10 August 2019
 *
 ******************************************************************************
 */
void pdm_test_set_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line)

{
    pdm_set_timeout_analog_in_1_2_digital_in_feedback(
        device, module_id, can_line, EVERY_1000MS);

    /* pdm_set_timeout_analog_in_3_4_digital_out_feedback( */
    /*     device, module_id, can_line, EVERY_1000MS); */

    /* pdm_set_timeout_analog_in_5_6_battery_sensor_supply_feedback( */
    /*     device, module_id, can_line, EVERY_1000MS); */

    /* pdm_set_timeout_analog_in_7_8_power_version_feedback( */
    /*     device, module_id, can_line, EVERY_1000MS); */

    /* pdm_set_timeout_output_current_1_6_feedback( */
    /*     device, module_id, can_line, EVERY_1000MS); */

    /* pdm_set_timeout_output_current_7_12_feedback( */
    /*     device, module_id, can_line, EVERY_1000MS); */

    /* pdm_set_timeout_output_function_handshake( */
    /*     device, module_id, can_line, EVERY_1000MS); */

    /* pdm_set_timeout_output_configuration_1_6_handshake( */
    /*     device, module_id, can_line, EVERY_1000MS); */

    /* pdm_set_timeout_output_configuration_7_12_handshake( */
    /*     device, module_id, can_line, EVERY_1000MS); */

}


/******************************************************************************
 *
 *        Name: pdm_configure_output_function_all_channels()
 *
 * Description: Configure all of the PDM output channels as described.
 *
 *      Author: Tom
 *        Date: Monday, 05 August 2019
 *
 ******************************************************************************
 */
void pdm_test_configure_output_function_all_channels(
    device_instances_t device)
{
    // RED LED, output 1
    pdm_set_configure_output_function(
        device,
        PDM_CHANNEL_1,
        100,
        PDM_MOTOR_MODE,
        PDM_CHANNEL_LOSS_COMM_0_PERCENT,
        0,
        PDM_CONFIGURE_OUTPUT_POR_ENABLE_ON,
        PDM_CONFIGURE_OUTPUT_COMMAND_TYPE_ON,
        PDM_CONFIGURE_OUTPUT_MOTOR_BREAKING_OFF,
        PDM_LSC_DISABLED,
        PDM_CONFIGURE_OUTPUT_CHANNEL_RESPONSE_ACTIVE_HIGH_OR_LOW);

    // YEL LED, output 2
    pdm_set_configure_output_function(
        device,
        PDM_CHANNEL_2,
        100,
        PDM_MOTOR_MODE,
        PDM_CHANNEL_LOSS_COMM_0_PERCENT,
        0,
        PDM_CONFIGURE_OUTPUT_POR_ENABLE_OFF,
        PDM_CONFIGURE_OUTPUT_COMMAND_TYPE_ON,
        PDM_CONFIGURE_OUTPUT_MOTOR_BREAKING_OFF,
        PDM_LSC_DISABLED,
        PDM_CONFIGURE_OUTPUT_CHANNEL_RESPONSE_ACTIVE_LOW);

    // GRN LED, output 3
    pdm_set_configure_output_function(
        device,
        PDM_CHANNEL_3,
        100,
        PDM_MOTOR_MODE,
        PDM_CHANNEL_LOSS_COMM_0_PERCENT,
        0,
        PDM_CONFIGURE_OUTPUT_POR_ENABLE_OFF,
        PDM_CONFIGURE_OUTPUT_COMMAND_TYPE_ON,
        PDM_CONFIGURE_OUTPUT_MOTOR_BREAKING_OFF,
        PDM_LSC_DISABLED,
        PDM_CONFIGURE_OUTPUT_CHANNEL_RESPONSE_ACTIVE_LOW);

    // UNUSED, output 4
    pdm_set_configure_output_function(
        device,
        PDM_CHANNEL_4,
        100,
        PDM_MOTOR_MODE,
        PDM_CHANNEL_LOSS_COMM_0_PERCENT,
        0,
        PDM_CONFIGURE_OUTPUT_POR_ENABLE_OFF,
        PDM_CONFIGURE_OUTPUT_COMMAND_TYPE_ON,
        PDM_CONFIGURE_OUTPUT_MOTOR_BREAKING_OFF,
        PDM_LSC_DISABLED,
        PDM_CONFIGURE_OUTPUT_CHANNEL_RESPONSE_ACTIVE_LOW);

    // UNUSED, output 5
    pdm_set_configure_output_function(
        device,
        PDM_CHANNEL_5,
        100,
        PDM_MOTOR_MODE,
        PDM_CHANNEL_LOSS_COMM_0_PERCENT,
        0,
        PDM_CONFIGURE_OUTPUT_POR_ENABLE_OFF,
        PDM_CONFIGURE_OUTPUT_COMMAND_TYPE_ON,
        PDM_CONFIGURE_OUTPUT_MOTOR_BREAKING_OFF,
        PDM_LSC_DISABLED,
        PDM_CONFIGURE_OUTPUT_CHANNEL_RESPONSE_ACTIVE_LOW);

    // UNUSED, output 6
    pdm_set_configure_output_function(
        device,
        PDM_CHANNEL_6,
        100,
        PDM_MOTOR_MODE,
        PDM_CHANNEL_LOSS_COMM_0_PERCENT,
        0,
        PDM_CONFIGURE_OUTPUT_POR_ENABLE_OFF,
        PDM_CONFIGURE_OUTPUT_COMMAND_TYPE_ON,
        PDM_CONFIGURE_OUTPUT_MOTOR_BREAKING_OFF,
        PDM_LSC_DISABLED,
        PDM_CONFIGURE_OUTPUT_CHANNEL_RESPONSE_ACTIVE_LOW);

    // Ball Valve, GRN wire, H-Bridge outputs 7 & 8
    pdm_set_configure_output_function(
        device,
        PDM_CHANNEL_7,
        100,
        PDM_MOTOR_MODE,
        PDM_CHANNEL_LOSS_COMM_0_PERCENT,
        0,
        PDM_CONFIGURE_OUTPUT_POR_ENABLE_OFF,
        PDM_CONFIGURE_OUTPUT_COMMAND_TYPE_OFF,
        PDM_CONFIGURE_OUTPUT_MOTOR_BREAKING_OFF,
        PDM_LSC_DISABLED,
        PDM_CONFIGURE_OUTPUT_CHANNEL_RESPONSE_ACTIVE_LOW);

    // Ball Valve, RED wire, H-Bridge outputs 7 & 8
    pdm_set_configure_output_function(
        device,
        PDM_CHANNEL_8,
        100,
        PDM_MOTOR_MODE,
        PDM_CHANNEL_LOSS_COMM_0_PERCENT,
        0,
        PDM_CONFIGURE_OUTPUT_POR_ENABLE_OFF,
        PDM_CONFIGURE_OUTPUT_COMMAND_TYPE_OFF,
        PDM_CONFIGURE_OUTPUT_MOTOR_BREAKING_OFF,
        PDM_LSC_DISABLED,
        PDM_CONFIGURE_OUTPUT_CHANNEL_RESPONSE_ACTIVE_LOW);

    // UNUSED, output 9
    pdm_set_configure_output_function(
        device,
        PDM_CHANNEL_9,
        100,
        PDM_MOTOR_MODE,
        PDM_CHANNEL_LOSS_COMM_0_PERCENT,
        0,
        PDM_CONFIGURE_OUTPUT_POR_ENABLE_OFF,
        PDM_CONFIGURE_OUTPUT_COMMAND_TYPE_ON,
        PDM_CONFIGURE_OUTPUT_MOTOR_BREAKING_OFF,
        PDM_LSC_DISABLED,
        PDM_CONFIGURE_OUTPUT_CHANNEL_RESPONSE_ACTIVE_LOW);

    // UNUSED, output 10
    pdm_set_configure_output_function(
        device,
        PDM_CHANNEL_10,
        100,
        PDM_MOTOR_MODE,
        PDM_CHANNEL_LOSS_COMM_0_PERCENT,
        0,
        PDM_CONFIGURE_OUTPUT_POR_ENABLE_OFF,
        PDM_CONFIGURE_OUTPUT_COMMAND_TYPE_ON,
        PDM_CONFIGURE_OUTPUT_MOTOR_BREAKING_OFF,
        PDM_LSC_DISABLED,
        PDM_CONFIGURE_OUTPUT_CHANNEL_RESPONSE_ACTIVE_LOW);

    // UNUSED, output 11
    pdm_set_configure_output_function(
        device,
        PDM_CHANNEL_11,
        100,
        PDM_MOTOR_MODE,
        PDM_CHANNEL_LOSS_COMM_0_PERCENT,
        0,
        PDM_CONFIGURE_OUTPUT_POR_ENABLE_OFF,
        PDM_CONFIGURE_OUTPUT_COMMAND_TYPE_ON,
        PDM_CONFIGURE_OUTPUT_MOTOR_BREAKING_OFF,
        PDM_LSC_DISABLED,
        PDM_CONFIGURE_OUTPUT_CHANNEL_RESPONSE_ACTIVE_LOW);

    // UNUSED, output 12
    pdm_set_configure_output_function(
        device,
        PDM_CHANNEL_12,
        100,
        PDM_MOTOR_MODE,
        PDM_CHANNEL_LOSS_COMM_0_PERCENT,
        0,
        PDM_CONFIGURE_OUTPUT_POR_ENABLE_OFF,
        PDM_CONFIGURE_OUTPUT_COMMAND_TYPE_ON,
        PDM_CONFIGURE_OUTPUT_MOTOR_BREAKING_OFF,
        PDM_LSC_DISABLED,
        PDM_CONFIGURE_OUTPUT_CHANNEL_RESPONSE_ACTIVE_LOW);

    //
    // Send the TX configure output function commands
    //
    pdm_tx_can_configure_output_function(ONE, TX_SEND_EACH_CALL);

}


void pdm_test_configure_all_output_channels(
    device_instances_t device)
{
    // RED LED, output 1
    pdm_set_configure_output_channel(
        device,
        PDM_CHANNEL_1,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);

    // YEL LED, output 2
    pdm_set_configure_output_channel(
        device,
        PDM_CHANNEL_2,
        PDM_CURRENT_LIMIT_2_5A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);

    // GRN LED, output 3
    pdm_set_configure_output_channel(
        device,
        PDM_CHANNEL_3,
        PDM_CURRENT_LIMIT_2_5A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);

    // UNUSED, output 4
    pdm_set_configure_output_channel(
        device,
        PDM_CHANNEL_4,
        PDM_CURRENT_LIMIT_2_5A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);

    // UNUSED, output 5
    pdm_set_configure_output_channel(
        device,
        PDM_CHANNEL_5,
        PDM_CURRENT_LIMIT_2_5A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);

    // UNUSED, output 6
    pdm_set_configure_output_channel(
        device,
        PDM_CHANNEL_6,
        PDM_CURRENT_LIMIT_2_5A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);

    //
    // Ball Valve, GRN, H-Bridge outputs 7 & 8
    //
    pdm_set_configure_output_channel(
        device,
        PDM_CHANNEL_7,
        PDM_CURRENT_LIMIT_2_5A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_H_BRIDGE);

    //
    // Ball Valve, RED, H-Bridge outputs 7 & 8
    //
    pdm_set_configure_output_channel(
        device,
        PDM_CHANNEL_8,
        PDM_CURRENT_LIMIT_2_5A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_H_BRIDGE);

    // UNUSED, output 9
    pdm_set_configure_output_channel(
        device,
        PDM_CHANNEL_9,
        PDM_CURRENT_LIMIT_2_5A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);

    // UNUSED, output 10
    pdm_set_configure_output_channel(
        device,
        PDM_CHANNEL_10,
        PDM_CURRENT_LIMIT_2_5A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);

    // UNUSED, output 11
    pdm_set_configure_output_channel(
        device,
        PDM_CHANNEL_11,
        PDM_CURRENT_LIMIT_2_5A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);

    // UNUSED, output 12
    pdm_set_configure_output_channel(
        device,
        PDM_CHANNEL_12,
        PDM_CURRENT_LIMIT_2_5A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);

    //
    // Send the TX configure output channel commands
    //
    pdm_tx_can_configure_output_channels_1_6(
        device,
        TX_SEND_EACH_CALL);

    pdm_tx_can_configure_output_channels_7_12(
        device,
        TX_SEND_EACH_CALL);

}


void pdm_test_command_all_output_channel_leds_on(
    device_instances_t device)
{
    // RED LED, output 1
    pdm_set_command_output_channel(
        device,
        PDM_CHANNEL_1,
        100,
        TRUE,
        PDM_MODULE_TX_RATE_50MS);

    // YEL LED, output 2
    pdm_set_command_output_channel(
        device,
        PDM_CHANNEL_2,
        100,
        TRUE,
        PDM_MODULE_TX_RATE_50MS);

    // GRN LED, output 3
    pdm_set_command_output_channel(
        device,
        PDM_CHANNEL_3,
        100,
        TRUE,
        PDM_MODULE_TX_RATE_50MS);

    // UNUSED, output 4
    pdm_set_command_output_channel(
        device,
        PDM_CHANNEL_4,
        100,
        TRUE,
        PDM_MODULE_TX_RATE_50MS);

    // UNUSED, output 5
    pdm_set_command_output_channel(
        device,
        PDM_CHANNEL_5,
        100,
        TRUE,
        PDM_MODULE_TX_RATE_50MS);

    // UNUSED, output 6
    pdm_set_command_output_channel(
        device,
        PDM_CHANNEL_6,
        100,
        TRUE,
        PDM_MODULE_TX_RATE_50MS);

    pdm_tx_can_command_output_channels_1_6(
        device,
        EVERY_500MS);
}


void pdm_test_command_valve_off(
    device_instances_t device)
{
    pdm_set_command_output_channel(
        device,
        PDM_CHANNEL_7,
        100,
        TRUE,
        PDM_MODULE_TX_RATE_50MS);

    pdm_set_command_output_channel(
        device,
        PDM_CHANNEL_8,
        0,
        TRUE,
        PDM_MODULE_TX_RATE_50MS);

    // UNUSED, output 9
    pdm_set_command_output_channel(
        device,
        PDM_CHANNEL_9,
        100,
        TRUE,
        PDM_MODULE_TX_RATE_50MS);

    // UNUSED, output 10
    pdm_set_command_output_channel(
        device,
        PDM_CHANNEL_10,
        100,
        TRUE,
        PDM_MODULE_TX_RATE_50MS);

    // UNUSED, output 11
    pdm_set_command_output_channel(
        device,
        PDM_CHANNEL_11,
        100,
        TRUE,
        PDM_MODULE_TX_RATE_50MS);

    // UNUSED, output 12
    pdm_set_command_output_channel(
        device,
        PDM_CHANNEL_12,
        100,
        TRUE,
        PDM_MODULE_TX_RATE_50MS);

    pdm_tx_can_command_output_channels_7_12(
        device,
        EVERY_500MS);
}

void pdm_test_command_valve_on(
    device_instances_t device)
{
    pdm_set_command_output_channel(
        device,
        PDM_CHANNEL_7,
        100,
        TRUE,
        PDM_MODULE_TX_RATE_50MS);

    pdm_set_command_output_channel(
        device,
        PDM_CHANNEL_8,
        0,
        TRUE,
        PDM_MODULE_TX_RATE_50MS);

    pdm_tx_can_command_output_channels_7_12(
        device,
        EVERY_500MS);

}
