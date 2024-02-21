/******************************************************************************
 *
 *        Name: pdm_control_3.c
 *
 * Description: The Carrier specific PDM channels are configured and
 *              initialized in this module. The command channel output
 *              functions are named with the Carrier wiring diagram
 *              names.
 *
 *              The pdm_init(THREE, 0, CAN2) function must first be
 *              called from User_Init().
 *
 *              The pdm_control_3_init() must then be called from
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
#include "pdm_control_3.h"

static device_instances_t pdm_device_three = THREE;

//
// pdm_control_3 output channel configure functions.
//
static void pdm_3_init_work_lights_pwr();
static void pdm_3_init_beacon_pwr();
static void pdm_3_init_head_light_pwr();
static void pdm_3_init_horn_pwr();
static void pdm_3_init_charger_1_busbar_pwr();
static void pdm_3_init_drive_motor_cooling_pump_pwr();
static void pdm_3_init_imt_cooling_pump_pwr();
static void pdm_3_init_fan_radiator_1_2_pwr();
static void pdm_3_init_pump_charger_1_cooling_pwr();
static void pdm_3_init_pump_charger_2_cooling_pwr();
static void pdm_3_init_main_bussbar_supply_pwr();
static void pdm_3_init_hydraulic_fan_solenoid_pwr();


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// Internal pdm_device_three channel initialisation functions.
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////



/******************************************************************************
 *
 *        Name: pdm_control_3_init()
 *
 * Description: Initialize the Carrier PDM2 function and output
 *              channels.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_control_3_init(){

    //
    // Initialise the pdm_3_control channels.
    //
    pdm_3_init_work_lights_pwr();
    pdm_3_init_beacon_pwr();
    pdm_3_init_head_light_pwr();
    pdm_3_init_horn_pwr();
    pdm_3_init_charger_1_busbar_pwr();
    pdm_3_init_drive_motor_cooling_pump_pwr();
    pdm_3_init_imt_cooling_pump_pwr();
    pdm_3_init_fan_radiator_1_2_pwr();
    pdm_3_init_pump_charger_1_cooling_pwr();
    pdm_3_init_pump_charger_2_cooling_pwr();
    pdm_3_init_main_bussbar_supply_pwr();
    pdm_3_init_hydraulic_fan_solenoid_pwr();

    //
    // Send the TX configure output function commands
    //
    pdm_tx_can_configure_output_function(
        pdm_device_three,
        TX_SEND_EACH_CALL);
    pdm_tx_can_configure_output_channels_1_6(pdm_device_three, TX_SEND_EACH_CALL);
    pdm_tx_can_configure_output_channels_7_12(pdm_device_three, TX_SEND_EACH_CALL);

    //
    // Set the default output channel values for the PDM.
    //
    pdm_3_command_work_lights_pwr(FALSE);
    pdm_3_command_beacon_pwr(FALSE);
    pdm_3_command_head_light_pwr(FALSE);
    pdm_3_command_horn_pwr(FALSE);
    pdm_3_command_charger_1_busbar_pwr(FALSE);
    pdm_3_command_drive_motor_cooling_pump_pwr(FALSE);
    pdm_3_command_imt_cooling_pump_pwr(FALSE);
    pdm_3_command_fan_radiator_1_2_pwr(FALSE);
    pdm_3_command_pump_charger_1_cooling_pwr(FALSE);
    pdm_3_command_pump_charger_2_cooling_pwr(FALSE);
    pdm_3_command_main_bussbar_supply_pwr(FALSE);
    pdm_3_command_hydraulic_fan_solenoid_pwr(FALSE);
}




//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// pdm_device_three output channel configure functions.
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


/******************************************************************************
 *
 *        Name: pdm_3_init_work_lights_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_3_init_work_lights_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_three,
        PDM_CHANNEL_1,
        100,
        PDM_LAMP_MODE,
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
        pdm_device_three,
        PDM_CHANNEL_1,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_3_init_beacon_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_3_init_beacon_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_three,
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
        pdm_device_three,
        PDM_CHANNEL_2,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_3_init_head_light_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_3_init_head_light_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_three,
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
        pdm_device_three,
        PDM_CHANNEL_3,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_3_init_horn_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_3_init_horn_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_three,
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
        pdm_device_three,
        PDM_CHANNEL_4,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_3_init_charger_1_busbar_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_3_init_charger_1_busbar_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_three,
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
        pdm_device_three,
        PDM_CHANNEL_5,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_3_init_drive_motor_cooling_pump_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_3_init_drive_motor_cooling_pump_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_three,
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
        pdm_device_three,
        PDM_CHANNEL_6,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_3_init_imt_cooling_pump_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_3_init_imt_cooling_pump_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_three,
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
        pdm_device_three,
        PDM_CHANNEL_7,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_3_init_fan_radiator_1_2_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_3_init_fan_radiator_1_2_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_three,
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
        pdm_device_three,
        PDM_CHANNEL_8,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_3_init_pump_charger_1_cooling_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_3_init_pump_charger_1_cooling_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_three,
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
        pdm_device_three,
        PDM_CHANNEL_9,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_3_init_pump_charger_2_cooling_pwr
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_3_init_pump_charger_2_cooling_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_three,
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
        pdm_device_three,
        PDM_CHANNEL_10,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}



/******************************************************************************
 *
 *        Name: pdm_3_init_main_bussbar_supply_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_3_init_main_bussbar_supply_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_three,
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
        pdm_device_three,
        PDM_CHANNEL_11,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_3_init_hydraulic_fan_solenoid_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_3_init_hydraulic_fan_solenoid_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_three,
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
        pdm_device_three,
        PDM_CHANNEL_12  ,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}




//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// pdm_device_three output channel command functions.
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////



/******************************************************************************
 *
 *        Name: pdm_3_command_work_lights_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_3_command_work_lights_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_three,
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
            pdm_device_three,
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
        pdm_device_three,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_3_command_beacon_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_3_command_beacon_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_three,
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
            pdm_device_three,
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
        pdm_device_three,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_3_command_head_light_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_3_command_head_light_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_three,
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
            pdm_device_three,
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
        pdm_device_three,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_3_command_horn_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_3_command_horn_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_three,
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
            pdm_device_three,
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
        pdm_device_three,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_3_command_charger_1_busbar_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_3_command_charger_1_busbar_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_three,
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
            pdm_device_three,
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
        pdm_device_three,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_3_command_drive_motor_cooling_pump_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_3_command_drive_motor_cooling_pump_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_three,
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
            pdm_device_three,
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
        pdm_device_three,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_3_command_imt_cooling_pump_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_3_command_imt_cooling_pump_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_three,
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
            pdm_device_three,
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
        pdm_device_three,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_3_command_fan_radiator_1_2_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_3_command_fan_radiator_1_2_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_three,
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
            pdm_device_three,
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
        pdm_device_three,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_3_command_pump_charger_1_cooling_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_3_command_pump_charger_1_cooling_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_three,
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
            pdm_device_three,
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
        pdm_device_three,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_3_command_pump_charger_2_cooling_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
    void pdm_3_command_pump_charger_2_cooling_pwr(
        bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_three,
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
            pdm_device_three,
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
        pdm_device_three,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_3_command_main_bussbar_supply_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_3_command_main_bussbar_supply_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_three,
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
            pdm_device_three,
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
        pdm_device_three,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_3_command_hydraulic_fan_solenoid_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_3_command_hydraulic_fan_solenoid_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_three,
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
            pdm_device_three,
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
        pdm_device_three,
        EVERY_100MS);
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// Current feedback of the outputs
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/******************************************************************************
 *
 *        Name: pdm_3_get_work_lights_current_feedback()
 *
 * Description: Return the current drawn sourced by this pdm output
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_3_get_work_lights_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_three,
        PDM_CHANNEL_1);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: pdm_3_get_beacon_current_feedback()
 *
 * Description: Return the current drawn sourced by this pdm output
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_3_get_beacon_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_three,
        PDM_CHANNEL_2);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: pdm_3_get_head_light_current_feedback()
 *
 * Description: Return the current drawn sourced by this pdm output
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_3_get_head_light_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_three,
        PDM_CHANNEL_3);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: pdm_3_get_horn_current_feedback()
 *
 * Description: Return the current drawn sourced by this pdm output
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_3_get_horn_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_three,
        PDM_CHANNEL_4);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: pdm_3_get_charger_1_busbar_current_feedback()
 *
 * Description: Return the current drawn sourced by this pdm output
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_3_get_charger_1_busbar_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_three,
        PDM_CHANNEL_5);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: pdm_3_get_drive_motor_cooling_pump_current_feedback()
 *
 * Description: Return the current drawn sourced by this pdm output
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_3_get_drive_motor_cooling_pump_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_three,
        PDM_CHANNEL_6);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: pdm_3_get_imt_cooling_pump_current_feedback()
 *
 * Description: Return the current drawn sourced by this pdm output
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_3_get_imt_cooling_pump_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_three,
        PDM_CHANNEL_7);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: pdm_3_get_fan_radiator_1_2_current_feedback()
 *
 * Description: Return the current drawn sourced by this pdm output
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_3_get_fan_radiator_1_2_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_three,
        PDM_CHANNEL_8);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: pdm_3_get_pump_charger_1_cooling_current_feedback()
 *
 * Description: Return the current drawn sourced by this pdm output
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_3_get_pump_charger_1_cooling_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_three,
        PDM_CHANNEL_9);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: pdm_3_get_pump_charger_2_cooling_current_feedback()
 *
 * Description: Return the current drawn sourced by this pdm output
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_3_get_pump_charger_2_cooling_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_three,
        PDM_CHANNEL_10);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: pdm_3_get_main_bussbar_supply_current_feedback()
 *
 * Description: Return the current drawn sourced by this pdm output
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_3_get_main_bussbar_supply_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_three,
        PDM_CHANNEL_11);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: pdm_3_get_hydraulic_fan_solenoid_current_feedback()
 *
 * Description: Return the current drawn sourced by this pdm output
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_3_get_hydraulic_fan_solenoid_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_three,
        PDM_CHANNEL_12);

    return current_feedback;

}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// Input to the Murphy Device
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/******************************************************************************
 *
 *        Name: pdm_3_get_front_traction_temperature_sensor_1_analog_input()
 *
 * Description: Return the analog reading of the murphy, J4-Pin8
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint16_t pdm_3_get_front_traction_temperature_sensor_1_analog_input()
{

    uint16_t SCALING_FACTOR = 1;

    //
    // Read the sensor analog output.
    //
    uint16_t analog_reading =  pdm_get_analog_input(pdm_device_three, PDM_ADC_2_R);

    //
    // Convert the obtained value to usable PSI value.
    //
    uint16_t temperature_reading = analog_reading * SCALING_FACTOR;

    //
    // Return pressure in PSI.
    //
    return temperature_reading;

}

/******************************************************************************
 *
 *        Name: pdm_3_get_front_traction_temperature_sensor_2_analog_input()
 *
 * Description: Return the analog reading of the murphy, J4-Pin9
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint16_t pdm_3_get_front_traction_temperature_sensor_2_analog_input()
{

    uint16_t SCALING_FACTOR = 1;

    //
    // Read the sensor analog output.
    //
    uint16_t analog_reading =  pdm_get_analog_input(pdm_device_three, PDM_ADC_1_R);

    //
    // Convert the obtained value to usable PSI value.
    //
    uint16_t temperature_reading = analog_reading * SCALING_FACTOR;

    //
    // Return pressure in PSI.
    //
    return temperature_reading;

}
