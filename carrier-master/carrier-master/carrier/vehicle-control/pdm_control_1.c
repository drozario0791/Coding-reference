/******************************************************************************
 *
 *        Name: pdm_control_1.c
 *
 * Description: The Carrier specific PDM channels are configured and
 *              initialized in this module. The command channel output
 *              functions are named with the Carrier wiring diagram
 *              names.
 *
 *              The pdm_init(ONE, 0, CAN2) function must first be
 *              called from User_Init().
 *
 *              The pdm_control_1_init() must then be called from
 *              User_Init() prior to calling any of the
 *              pdm_n_output_channel_() functions.
 *
 *      Author: Tom
 *        Date: Wednesday, 14 August 2019
 *
 ******************************************************************************
 */

#include "Prototypes_CAN.h"
#include "can_service.h"
#include "pdm_device.h"
#include "pdm_control_1.h"

device_instances_t pdm_device_one = ONE;

//
// pdm_device_one output channel configure functions.
//
static void pdm_1_init_right_spray_solenoid();
static void pdm_1_init_center_spray_solenoid();
static void pdm_1_init_left_spray_solenoid();
static void pdm_1_init_spare_output();
static void pdm_1_init_reverse_lights_output();
static void pdm_1_init_brake_lights_output();
static void pdm_1_init_deck_lights_output();
static void pdm_1_init_brake_release_solenoid();
static void pdm_1_init_reverse_beeper_output();
static void pdm_1_init_reverse_beacon_output();
static void pdm_1_init_channel_11_pwr();
static void pdm_1_init_channel_12_pwr();


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// Internal pdm_device_one channel initialisation functions.
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////



/******************************************************************************
 *
 *        Name: pdm_control_1_init()
 *
 * Description: Initialize the Carrier rear deck PDM channels and set
 *              their default state.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_control_1_init(){

    //
    // Initiailize the PDM outputs associated with the optional spray
    // unit.
    //
    pdm_1_init_right_spray_solenoid();
    pdm_1_init_center_spray_solenoid();
    pdm_1_init_left_spray_solenoid();
    pdm_1_init_spare_output();

    //
    // Initialize all of the rear deck other PDM outputs.
    //
    pdm_1_init_reverse_lights_output();
    pdm_1_init_brake_lights_output();
    pdm_1_init_deck_lights_output();
    pdm_1_init_brake_release_solenoid();
    pdm_1_init_reverse_beeper_output();
    pdm_1_init_reverse_beacon_output();

    //
    // Initialize the unused channels.
    //
    pdm_1_init_channel_11_pwr();
    pdm_1_init_channel_12_pwr();

    //
    // Send the TX configure output function command to send the
    // setter parameters configured above.
    //
    pdm_tx_can_configure_output_function(pdm_device_one, TX_SEND_EACH_CALL);
    pdm_tx_can_configure_output_channels_1_6(pdm_device_one, TX_SEND_EACH_CALL);
    pdm_tx_can_configure_output_channels_7_12(pdm_device_one, TX_SEND_EACH_CALL);

    //
    // Command the default output channel values for the this PDM.
    //
    pdm_1_command_spare_output(FALSE);
    pdm_1_command_left_spray_solenoid(FALSE);
    pdm_1_command_center_spray_solenoid(FALSE);
    pdm_1_command_right_spray_solenoid(FALSE);
    pdm_1_command_reverse_lights_output(FALSE);
    pdm_1_command_brake_lights_output(FALSE);
    pdm_1_command_deck_lights_output(FALSE);
    pdm_1_command_brake_release_solenoid(FALSE);
    pdm_1_command_reverse_beeper_output(FALSE);
    pdm_1_command_reverse_beacon_output(FALSE);

    //
    // Set the default output channel for unused channels.
    //
    pdm_1_command_channel_11_pwr(FALSE);
    pdm_1_command_channel_12_pwr(FALSE);

}




//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// pdm_device_one output channel configure functions. These should be
// tailored to suit the application.
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////



/******************************************************************************
 *
 *        Name: pdm_1_init_right_spray_solenoid()
 *
 * Description: The right spray solenoid is connected to J3 pin-3,
 *              which is PDM_CHANNEL_7.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_1_init_right_spray_solenoid()
{
    //
    // Right spray solenoid power.
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_one,
        PDM_CHANNEL_7,
        100,
        PDM_MOTOR_MODE,
        PDM_CHANNEL_LOSS_COMM_0_PERCENT,
        0,
        PDM_CONFIGURE_OUTPUT_POR_ENABLE_ON,
        PDM_CONFIGURE_OUTPUT_COMMAND_TYPE_ON,
        PDM_CONFIGURE_OUTPUT_MOTOR_BREAKING_OFF,
        PDM_LSC_DISABLED,
        PDM_CONFIGURE_OUTPUT_CHANNEL_RESPONSE_ACTIVE_HIGH_OR_LOW);
    //
    // Configure output channel
    //
    pdm_set_configure_output_channel(
        pdm_device_one,
        PDM_CHANNEL_7,
        PDM_CURRENT_LIMIT_10_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_1_init_center_spray_solenoid()
 *
 * Description: The cneter spray solenoid is connected to J3, pin-4,
 *              which is PDM_CHANNEL_8.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_1_init_center_spray_solenoid()
{
    //
    // Right spray solenoid power.
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_one,
        PDM_CHANNEL_8,
        100,
        PDM_MOTOR_MODE,
        PDM_CHANNEL_LOSS_COMM_0_PERCENT,
        0,
        PDM_CONFIGURE_OUTPUT_POR_ENABLE_ON,
        PDM_CONFIGURE_OUTPUT_COMMAND_TYPE_ON,
        PDM_CONFIGURE_OUTPUT_MOTOR_BREAKING_OFF,
        PDM_LSC_DISABLED,
        PDM_CONFIGURE_OUTPUT_CHANNEL_RESPONSE_ACTIVE_HIGH_OR_LOW);
    //
    // Configure output channel
    //
    pdm_set_configure_output_channel(
        pdm_device_one,
        PDM_CHANNEL_8,
        PDM_CURRENT_LIMIT_10_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_1_init_left_spray_solenoid()
 *
 * Description: The cneter spray solenoid is connected to J3 pin-5,
 *              which is PDM_CHANNEL_9.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_1_init_left_spray_solenoid()
{
    //
    // Left spray solenoid power.
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_one,
        PDM_CHANNEL_9,
        100,
        PDM_MOTOR_MODE,
        PDM_CHANNEL_LOSS_COMM_0_PERCENT,
        0,
        PDM_CONFIGURE_OUTPUT_POR_ENABLE_ON,
        PDM_CONFIGURE_OUTPUT_COMMAND_TYPE_ON,
        PDM_CONFIGURE_OUTPUT_MOTOR_BREAKING_OFF,
        PDM_LSC_DISABLED,
        PDM_CONFIGURE_OUTPUT_CHANNEL_RESPONSE_ACTIVE_HIGH_OR_LOW);
    //
    // Configure output channel
    //
    pdm_set_configure_output_channel(
        pdm_device_one,
        PDM_CHANNEL_9,
        PDM_CURRENT_LIMIT_10_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_1_init_spare_output()
 *
 * Description:
 *
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_1_init_spare_output()
{
    //
    // Left spray solenoid power.
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_one,
        PDM_CHANNEL_10,
        100,
        PDM_MOTOR_MODE,
        PDM_CHANNEL_LOSS_COMM_0_PERCENT,
        0,
        PDM_CONFIGURE_OUTPUT_POR_ENABLE_ON,
        PDM_CONFIGURE_OUTPUT_COMMAND_TYPE_ON,
        PDM_CONFIGURE_OUTPUT_MOTOR_BREAKING_OFF,
        PDM_LSC_DISABLED,
        PDM_CONFIGURE_OUTPUT_CHANNEL_RESPONSE_ACTIVE_HIGH_OR_LOW);
    //
    // Configure output channel
    //
    pdm_set_configure_output_channel(
        pdm_device_one,
        PDM_CHANNEL_10,
        PDM_CURRENT_LIMIT_5_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_1_init_reverse_lights_output()
 *
 * Description: The reverse lights are connected to J4 pin-1,
 *              which is PDM_CHANNEL_1.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_1_init_reverse_lights_output()
{
    //
    // Reverse lights power.
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_one,
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
    //
    // Configure output channel
    //
    pdm_set_configure_output_channel(
        pdm_device_one,
        PDM_CHANNEL_1,
        PDM_CURRENT_LIMIT_5_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_1_init_brake_lights_output()
 *
 * Description: The brake lights are connected to J4 pin 2,
 *              which is PDM_CHANNEL_2.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_1_init_brake_lights_output()
{
    //
    // Brake lights power.
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_one,
        PDM_CHANNEL_2,
        100,
        PDM_MOTOR_MODE,
        PDM_CHANNEL_LOSS_COMM_0_PERCENT,
        0,
        PDM_CONFIGURE_OUTPUT_POR_ENABLE_ON,
        PDM_CONFIGURE_OUTPUT_COMMAND_TYPE_ON,
        PDM_CONFIGURE_OUTPUT_MOTOR_BREAKING_OFF,
        PDM_LSC_DISABLED,
        PDM_CONFIGURE_OUTPUT_CHANNEL_RESPONSE_ACTIVE_HIGH_OR_LOW);
    //
    // Configure output channel
    //
    pdm_set_configure_output_channel(
        pdm_device_one,
        PDM_CHANNEL_2,
        PDM_CURRENT_LIMIT_5_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_1_init_deck_lights_output()
 *
 * Description: The deck lights are connected to J4 pin-3,
 *              which is PDM_CHANNEL_3.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_1_init_deck_lights_output()
{
    //
    // Deck lights power.
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_one,
        PDM_CHANNEL_3,
        100,
        PDM_MOTOR_MODE,
        PDM_CHANNEL_LOSS_COMM_0_PERCENT,
        0,
        PDM_CONFIGURE_OUTPUT_POR_ENABLE_ON,
        PDM_CONFIGURE_OUTPUT_COMMAND_TYPE_ON,
        PDM_CONFIGURE_OUTPUT_MOTOR_BREAKING_OFF,
        PDM_LSC_DISABLED,
        PDM_CONFIGURE_OUTPUT_CHANNEL_RESPONSE_ACTIVE_HIGH_OR_LOW);
    //
    // Configure output channel
    //
    pdm_set_configure_output_channel(
        pdm_device_one,
        PDM_CHANNEL_3,
        PDM_CURRENT_LIMIT_5_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_1_init_brake_release_solenoid()
 *
 * Description: The brake release solenoid is connected to J4 pin-4,
 *              which is PDM_CHANNEL_4.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_1_init_brake_release_solenoid()
{
    //
    // Brake release solenoid power.
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_one,
        PDM_CHANNEL_4,
        100,
        PDM_MOTOR_MODE,
        PDM_CHANNEL_LOSS_COMM_0_PERCENT,
        0,
        PDM_CONFIGURE_OUTPUT_POR_ENABLE_ON,
        PDM_CONFIGURE_OUTPUT_COMMAND_TYPE_ON,
        PDM_CONFIGURE_OUTPUT_MOTOR_BREAKING_OFF,
        PDM_LSC_DISABLED,
        PDM_CONFIGURE_OUTPUT_CHANNEL_RESPONSE_ACTIVE_HIGH_OR_LOW);
    //
    // Configure output channel
    //
    pdm_set_configure_output_channel(
        pdm_device_one,
        PDM_CHANNEL_4,
        PDM_CURRENT_LIMIT_5_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_1_init_reverse_beeper_output()
 *
 * Description: The reverse beeper is connected to J4 pin-5,
 *              which is PDM_CHANNEL_5.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_1_init_reverse_beeper_output()
{
    //
    // Reverse beeper power.
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_one,
        PDM_CHANNEL_5,
        100,
        PDM_MOTOR_MODE,
        PDM_CHANNEL_LOSS_COMM_0_PERCENT,
        0,
        PDM_CONFIGURE_OUTPUT_POR_ENABLE_ON,
        PDM_CONFIGURE_OUTPUT_COMMAND_TYPE_ON,
        PDM_CONFIGURE_OUTPUT_MOTOR_BREAKING_OFF,
        PDM_LSC_DISABLED,
        PDM_CONFIGURE_OUTPUT_CHANNEL_RESPONSE_ACTIVE_HIGH_OR_LOW);
    //
    // Configure output channel
    //
    pdm_set_configure_output_channel(
        pdm_device_one,
        PDM_CHANNEL_5,
        PDM_CURRENT_LIMIT_2_5A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_1_init_reverse_beacon_output()
 *
 * Description: The reverse beacon is connected to J4 pin-6,
 *              which is PDM_CHANNEL_6.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_1_init_reverse_beacon_output()
{
    //
    // Reverse beacon power.
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_one,
        PDM_CHANNEL_6,
        100,
        PDM_MOTOR_MODE,
        PDM_CHANNEL_LOSS_COMM_0_PERCENT,
        0,
        PDM_CONFIGURE_OUTPUT_POR_ENABLE_ON,
        PDM_CONFIGURE_OUTPUT_COMMAND_TYPE_ON,
        PDM_CONFIGURE_OUTPUT_MOTOR_BREAKING_OFF,
        PDM_LSC_DISABLED,
        PDM_CONFIGURE_OUTPUT_CHANNEL_RESPONSE_ACTIVE_HIGH_OR_LOW);
    //
    // Configure output channel
    //
    pdm_set_configure_output_channel(
        pdm_device_one,
        PDM_CHANNEL_6,
        PDM_CURRENT_LIMIT_2_5A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}




/******************************************************************************
 *
 *        Name: pdm_1_init_channel_11_pwr
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_1_init_channel_11_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_one,
        PDM_CHANNEL_11,
        100,
        PDM_MOTOR_MODE,
        PDM_CHANNEL_LOSS_COMM_0_PERCENT,
        0,
        PDM_CONFIGURE_OUTPUT_POR_ENABLE_ON,
        PDM_CONFIGURE_OUTPUT_COMMAND_TYPE_ON,
        PDM_CONFIGURE_OUTPUT_MOTOR_BREAKING_OFF,
        PDM_LSC_DISABLED,
        PDM_CONFIGURE_OUTPUT_CHANNEL_RESPONSE_ACTIVE_HIGH_OR_LOW);

    //
    // Configure output channel
    //
    pdm_set_configure_output_channel(
        pdm_device_one,
        PDM_CHANNEL_11,
        PDM_CURRENT_LIMIT_2_5A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_1_init_channel_12_pwr
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_1_init_channel_12_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_one,
        PDM_CHANNEL_12,
        100,
        PDM_MOTOR_MODE,
        PDM_CHANNEL_LOSS_COMM_0_PERCENT,
        0,
        PDM_CONFIGURE_OUTPUT_POR_ENABLE_ON,
        PDM_CONFIGURE_OUTPUT_COMMAND_TYPE_ON,
        PDM_CONFIGURE_OUTPUT_MOTOR_BREAKING_OFF,
        PDM_LSC_DISABLED,
        PDM_CONFIGURE_OUTPUT_CHANNEL_RESPONSE_ACTIVE_HIGH_OR_LOW);

    //
    // Configure output channel
    //
    pdm_set_configure_output_channel(
        pdm_device_one,
        PDM_CHANNEL_12  ,
        PDM_CURRENT_LIMIT_2_5A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}



//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// pdm_device_one output channel command functions.
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////



/******************************************************************************
 *
 *        Name: pdm_1_command_left_spray_solenoid()
 *
 * Description: Command the PDM output channel state of the left
 *              sprayer.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_1_command_left_spray_solenoid(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_one,
            PDM_CHANNEL_9,
            100,
            switch_on,
            PDM_MODULE_TX_RATE_50MS);
        break;

    case FALSE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_one,
            PDM_CHANNEL_9,
            0,
            switch_on,
            PDM_MODULE_TX_RATE_50MS);
        break;

    default:
        break;

    }

    //
    // Send the TX command output channel commands CAN message.
    //
    pdm_tx_can_command_output_channels_7_12(
        pdm_device_one,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_1_command_center_spray_solenoid()
 *
 * Description: Command the PDM output channel state of the center
 *              sprayer.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_1_command_center_spray_solenoid(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_one,
            PDM_CHANNEL_8,
            100,
            switch_on,
            PDM_MODULE_TX_RATE_50MS);
        break;

    case FALSE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_one,
            PDM_CHANNEL_8,
            0,
            switch_on,
            PDM_MODULE_TX_RATE_50MS);
        break;

    default:
        break;
    }

    //
    // Send the TX command output channel commands CAN message.
    //
    pdm_tx_can_command_output_channels_7_12(
        pdm_device_one,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_1_command_right_spray_solenoid()
 *
 * Description: Command the PDM output channel state of the right
 *              sprayer.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_1_command_right_spray_solenoid(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_one,
            PDM_CHANNEL_7,
            100,
            switch_on,
            PDM_MODULE_TX_RATE_50MS);
        break;

    case FALSE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_one,
            PDM_CHANNEL_7,
            0,
            switch_on,
            PDM_MODULE_TX_RATE_50MS);
        break;

    default:
        break;
    }

    //
    // Send the TX command output channel commands CAN message.
    //
    pdm_tx_can_command_output_channels_7_12(
        pdm_device_one,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_1_command_spare_output()
 *
 * Description: Command the PDM output channel state of the spare
 *              output.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_1_command_spare_output(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_one,
            PDM_CHANNEL_10,
            100,
            switch_on,
            PDM_MODULE_TX_RATE_50MS);
        break;

    case FALSE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_one,
            PDM_CHANNEL_10,
            0,
            switch_on,
            PDM_MODULE_TX_RATE_50MS);
        break;

    default:
        break;
    }

    //
    // Send the TX command output channel commands CAN message.
    //
    pdm_tx_can_command_output_channels_7_12(
        pdm_device_one,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_1_command_reverse_lights_output()
 *
 * Description: Command the PDM output channel state of the reverse
 *              lights.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_1_command_reverse_lights_output(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_one,
            PDM_CHANNEL_1,
            100,
            switch_on,
            PDM_MODULE_TX_RATE_50MS);
        break;

    case FALSE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_one,
            PDM_CHANNEL_1,
            0,
            switch_on,
            PDM_MODULE_TX_RATE_50MS);
        break;

    default:
        break;
    }

    //
    // Send the TX command output channel commands CAN message.
    //
    pdm_tx_can_command_output_channels_1_6(
        pdm_device_one,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_1_command_brake_lights_output()
 *
 * Description: Command the PDM output channel state of the brake
 *              lights.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_1_command_brake_lights_output(bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_one,
            PDM_CHANNEL_2,
            100,
            switch_on,
            PDM_MODULE_TX_RATE_50MS);
        break;

    case FALSE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_one,
            PDM_CHANNEL_2,
            0,
            switch_on,
            PDM_MODULE_TX_RATE_50MS);
        break;

    default:
        break;
    }

    //
    // Send the TX command output channel commands CAN message.
    //
    pdm_tx_can_command_output_channels_1_6(
        pdm_device_one,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_1_command_deck_lights_output()
 *
 * Description: Command the PDM output channel state of the deck
 *              lights.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_1_command_deck_lights_output(bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_one,
            PDM_CHANNEL_3,
            100,
            switch_on,
            PDM_MODULE_TX_RATE_50MS);
        break;

    case FALSE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_one,
            PDM_CHANNEL_3,
            0,
            switch_on,
            PDM_MODULE_TX_RATE_50MS);
        break;

    default:
        break;
    }

    //
    // Send the TX command output channel commands CAN message.
    //
    pdm_tx_can_command_output_channels_1_6(
        pdm_device_one,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_1_command_brake_release_solenoid()
 *
 * Description: Command the PDM output channel state of the rear brake
 *              release.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_1_command_brake_release_solenoid(bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_one,
            PDM_CHANNEL_4,
            100,
            switch_on,
            PDM_MODULE_TX_RATE_50MS);
        break;

    case FALSE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_one,
            PDM_CHANNEL_4,
            0,
            switch_on,
            PDM_MODULE_TX_RATE_50MS);
        break;

    default:
        break;
    }

    //
    // Send the TX command output channel commands CAN message.
    //
    pdm_tx_can_command_output_channels_1_6(
        pdm_device_one,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_1_command_reverse_beeper_output()
 *
 * Description: Command the PDM output channel state of the reverse
 *              beeper.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_1_command_reverse_beeper_output(bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_one,
            PDM_CHANNEL_5,
            100,
            switch_on,
            PDM_MODULE_TX_RATE_50MS);
        break;

    case FALSE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_one,
            PDM_CHANNEL_5,
            0,
            switch_on,
            PDM_MODULE_TX_RATE_50MS);
        break;

    default:
        break;
    }

    //
    // Send the TX command output channel commands CAN message.
    //
    pdm_tx_can_command_output_channels_1_6(
        pdm_device_one,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_1_command_reverse_beacon_output()
 *
 * Description: Command the PDM output channel state of the reverse
 *              beacon.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_1_command_reverse_beacon_output(bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_one,
            PDM_CHANNEL_6,
            100,
            switch_on,
            PDM_MODULE_TX_RATE_50MS);
        break;

    case FALSE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_one,
            PDM_CHANNEL_6,
            0,
            switch_on,
            PDM_MODULE_TX_RATE_50MS);
        break;

    default:
        break;
    }

    //
    // Send the TX command output channel commands CAN message.
    //
    pdm_tx_can_command_output_channels_1_6(
        pdm_device_one,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_1_command_channel_11_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_1_command_channel_11_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_one,
            PDM_CHANNEL_11,
            100,
            switch_on,
            PDM_MODULE_TX_RATE_50MS);
        break;

    case FALSE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_one,
            PDM_CHANNEL_11,
            0,
            switch_on,
            PDM_MODULE_TX_RATE_50MS);
        break;

    default:
        break;
    }

    //
    // Send the TX command output channel commands CAN message.
    //
    pdm_tx_can_command_output_channels_7_12(
        pdm_device_one,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_1_command_channel_12_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_1_command_channel_12_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_one,
            PDM_CHANNEL_12,
            100,
            switch_on,
            PDM_MODULE_TX_RATE_50MS);
        break;

    case FALSE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_one,
            PDM_CHANNEL_12,
            0,
            switch_on,
            PDM_MODULE_TX_RATE_50MS);
        break;

    default:
        break;
    }

    //
    // Send the TX command output channel commands CAN message.
    //
    pdm_tx_can_command_output_channels_7_12(
        pdm_device_one,
        EVERY_1000MS);
}




//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// pdm_device_one current feedback
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


/******************************************************************************
 *
 *        Name: pdm_1_spare_output_current_feedback()
 *
 * Description:
 *
 *        Date: Friday, 23 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_1_spare_output_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_one,
        PDM_CHANNEL_10);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: get_pdm_1_left_spray_solenoid_current_feedback()
 *
 * Description:
 *
 *        Date: Friday, 23 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_1_left_spray_solenoid_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_one,
        PDM_CHANNEL_9);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: get_2_center_spray_solenoid_current_feedback()
 *
 * Description:
 *
 *        Date: Friday, 23 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_1_center_spray_solenoid_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_one,
        PDM_CHANNEL_8);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: get_2_right_spray_solenoid_current_feedback()
 *
 * Description:
 *
 *        Date: Friday, 23 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_1_right_spray_solenoid_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_one,
        PDM_CHANNEL_7);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: get_2_reverse_lights_output_current_feedback()
 *
 * Description:
 *
 *        Date: Friday, 23 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_1_reverse_lights_output_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_one,
        PDM_CHANNEL_1);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: get_2_brake_lights_output_current_feedback()
 *
 * Description:
 *
 *        Date: Friday, 23 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_1_brake_lights_output_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_one,
        PDM_CHANNEL_2);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: get_2_deck_lights_output_current_feedback()
 *
 * Description:
 *
 *        Date: Friday, 23 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_1_deck_lights_output_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_one,
        PDM_CHANNEL_3);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: get_2_brake_release_solenoid_current_feedback()
 *
 * Description:
 *
 *        Date: Friday, 23 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_1_brake_release_solenoid_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_one,
        PDM_CHANNEL_4);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: get_2_reverse_beeper_output_current_feedback()
 *
 * Description:
 *
 *        Date: Friday, 23 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_1_reverse_beeper_output_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_one,
        PDM_CHANNEL_5);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: get_2_reverse_beacon_output_current_feedback()
 *
 * Description:
 *
 *        Date: Friday, 23 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_1_reverse_beacon_output_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_one,
        PDM_CHANNEL_6);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: get_pdm_1_channel_11_current_feedback()
 *
 * Description:
 *
 *        Date: Friday, 23 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_1_channel_11_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_one,
        PDM_CHANNEL_11);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: get_pdm_1_channel_12__current_feedback()
 *
 * Description:
 *
 *        Date: Friday, 23 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_1_channel_12__current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_one,
        PDM_CHANNEL_12);

    return current_feedback;

}

