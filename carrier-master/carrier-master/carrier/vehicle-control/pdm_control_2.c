/******************************************************************************
 *
 *        Name: pdm_control_2.c
 *
 * Description: The Carrier specific PDM channels are configured and
 *              initialized in this module. The command channel output
 *              functions are named with the Carrier wiring diagram
 *              names.
 *
 *              The pdm_init(TWO, 0, CAN2) function must first be
 *              called from User_Init().
 *
 *              The pdm_control_2_init() must then be called from
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
#include "pdm_control_2.h"

#define SCALING_FACTOR 1

static device_instances_t pdm_device_two = TWO;


//
// pdm_control_2 output channel configure functions.
//
static void pdm_2_init_start_drain_solenoid_pwr();
static void pdm_2_init_aux_hydraulicss_12v_bussbar_pwr();
static void pdm_2_init_1st_gear_solenoid_pwr();
static void pdm_2_init_2nd_gear_solenoid_pwr();
static void pdm_2_init_3rd_gear_solenoid_pwr();
static void pdm_2_init_4th_gear_solenoid_pwr();
static void pdm_2_init_power_electronics_pump_pwr();
static void pdm_2_init_selector_solenoid_pwr();
static void pdm_2_init_brake_release_solenoid_pwr();
static void pdm_2_init_service_brake_solenoid_pwr();
static void pdm_2_init_hydraulic_motor_cooling_pump_pwr();
static void pdm_2_init_hydraulic_motor_radiator_cooling_fan_pwr();


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// Internal pdm_device_two channel initialisation functions.
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////



/******************************************************************************
 *
 *        Name: pdm_control_2_init()
 *
 * Description: Initialize the Carrier PDM2 function and output
 *              channels.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_control_2_init(){

    //
    // Initialise the pdm_2_control channels.
    //
    pdm_2_init_start_drain_solenoid_pwr();
    pdm_2_init_aux_hydraulicss_12v_bussbar_pwr();
    pdm_2_init_1st_gear_solenoid_pwr();
    pdm_2_init_2nd_gear_solenoid_pwr();
    pdm_2_init_3rd_gear_solenoid_pwr();
    pdm_2_init_4th_gear_solenoid_pwr();
    pdm_2_init_power_electronics_pump_pwr();
    pdm_2_init_selector_solenoid_pwr();
    pdm_2_init_brake_release_solenoid_pwr();
    pdm_2_init_service_brake_solenoid_pwr();
    pdm_2_init_hydraulic_motor_cooling_pump_pwr();
    pdm_2_init_hydraulic_motor_radiator_cooling_fan_pwr();

    //
    // Send the TX configure output function commands
    //
    pdm_tx_can_configure_output_function(
        pdm_device_two,
        TX_SEND_EACH_CALL);
    pdm_tx_can_configure_output_channels_1_6(pdm_device_two, TX_SEND_EACH_CALL);
    pdm_tx_can_configure_output_channels_7_12(pdm_device_two, TX_SEND_EACH_CALL);

    //
    // Set the default output channel values for the PDM.
    //
    pdm_2_command_start_drain_solenoid_pwr(FALSE);
    pdm_2_command_aux_hydraulicss_12v_bussbar_pwr(FALSE);
    pdm_2_command_1st_gear_solenoid_pwr(FALSE);
    pdm_2_command_2nd_gear_solenoid_pwr(FALSE);
    pdm_2_command_3rd_gear_solenoid_pwr(FALSE);
    pdm_2_command_4th_gear_solenoid_pwr(FALSE);
    pdm_2_command_power_electronics_pump_pwr(FALSE);
    pdm_2_command_selector_solenoid_pwr(FALSE);
    pdm_2_command_brake_release_solenoid_pwr(FALSE);
    pdm_2_command_service_brake_solenoid_pwr(FALSE);
    pdm_2_command_hydraulic_motor_cooling_pump_pwr(FALSE);
    pdm_2_command_hydraulic_motor_radiator_cooling_fan_pwr(FALSE);
}




//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// pdm_device_two output channel configure functions.
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////



/******************************************************************************
 *
 *        Name: pdm_2_init_start_drain_solenoid_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_2_init_start_drain_solenoid_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_two,
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
        pdm_device_two,
        PDM_CHANNEL_1,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_2_init_aux_hydraulicss_12v_bussbar_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_2_init_aux_hydraulicss_12v_bussbar_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_two,
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
        pdm_device_two,
        PDM_CHANNEL_2,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_2_init_1st_gear_solenoid_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_2_init_1st_gear_solenoid_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_two,
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
        pdm_device_two,
        PDM_CHANNEL_3,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_2_init_2nd_gear_solenoid_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_2_init_2nd_gear_solenoid_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_two,
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
        pdm_device_two,
        PDM_CHANNEL_4,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_2_init_3rd_gear_solenoid_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_2_init_3rd_gear_solenoid_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_two,
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
        pdm_device_two,
        PDM_CHANNEL_5,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_2_init_4th_gear_solenoid_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_2_init_4th_gear_solenoid_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_two,
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
        pdm_device_two,
        PDM_CHANNEL_6,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_2_init_power_electronics_pump_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_2_init_power_electronics_pump_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_two,
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
        pdm_device_two,
        PDM_CHANNEL_7,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_2_init_selector_solenoid_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_2_init_selector_solenoid_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_two,
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
        pdm_device_two,
        PDM_CHANNEL_8,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_2_init_brake_release_solenoid_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_2_init_brake_release_solenoid_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_two,
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
        pdm_device_two,
        PDM_CHANNEL_9,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_2_init_service_brake_solenoid_pwr
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_2_init_service_brake_solenoid_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_two,
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
        pdm_device_two,
        PDM_CHANNEL_10,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_2_init_hydraulic_motor_cooling_pump_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_2_init_hydraulic_motor_cooling_pump_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_two,
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
        pdm_device_two,
        PDM_CHANNEL_11,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_2_init_hydraulic_motor_radiator_cooling_fan_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_2_init_hydraulic_motor_radiator_cooling_fan_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_two,
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
        pdm_device_two,
        PDM_CHANNEL_12  ,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}





//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// pdm_device_two output channel command functions.
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////




/******************************************************************************
 *
 *        Name: pdm_2_command_start_drain_solenoid_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_2_command_start_drain_solenoid_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_two,
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
            pdm_device_two,
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
        pdm_device_two,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_2_command_aux_hydraulicss_12v_bussbar_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_2_command_aux_hydraulicss_12v_bussbar_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_two,
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
            pdm_device_two,
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
        pdm_device_two,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_2_command_1st_gear_solenoid_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_2_command_1st_gear_solenoid_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_two,
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
            pdm_device_two,
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
        pdm_device_two,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_2_command_2nd_gear_solenoid_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_2_command_2nd_gear_solenoid_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_two,
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
            pdm_device_two,
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
        pdm_device_two,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_2_command_3rd_gear_solenoid_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_2_command_3rd_gear_solenoid_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_two,
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
            pdm_device_two,
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
        pdm_device_two,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_2_command_4th_gear_solenoid_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_2_command_4th_gear_solenoid_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_two,
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
            pdm_device_two,
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
        pdm_device_two,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_2_command_power_electronics_pump_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_2_command_power_electronics_pump_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_two,
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
            pdm_device_two,
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
        pdm_device_two,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_2_command_selector_solenoid_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_2_command_selector_solenoid_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_two,
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
            pdm_device_two,
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
        pdm_device_two,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_2_command_brake_release_solenoid_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_2_command_brake_release_solenoid_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_two,
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
            pdm_device_two,
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
        pdm_device_two,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_2_command_service_brake_solenoid_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_2_command_service_brake_solenoid_pwr(
        bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_two,
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
            pdm_device_two,
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
        pdm_device_two,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_2_command_hydraulic_motor_cooling_pump_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_2_command_hydraulic_motor_cooling_pump_pwr(
        bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_two,
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
            pdm_device_two,
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
        pdm_device_two,
        EVERY_100MS);
}


/******************************************************************************
 *
 *        Name: pdm_2_command_hydraulic_motor_radiator_cooling_fan_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_2_command_hydraulic_motor_radiator_cooling_fan_pwr(
        bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_two,
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
            pdm_device_two,
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
        pdm_device_two,
        EVERY_1000MS);
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// pdm_device_two get current feedback functions.
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/******************************************************************************
 *
 *        Name: pdm_2_get_start_drain_solenoid_current_feedback()
 *
 * Description: Return the current drawn sourced by this pdm output
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_2_get_start_drain_solenoid_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_two,
        PDM_CHANNEL_1);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: pdm_2_get_aux_hydraulicss_12v_bussbar_current_feedback()
 *
 * Description: Return the current drawn sourced by this pdm output
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_2_get_aux_hydraulicss_12v_bussbar_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_two,
        PDM_CHANNEL_2);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: pdm_2_get_1st_gear_solenoid_current_feedback()
 *
 * Description: Return the current drawn sourced by this pdm output
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_2_get_1st_gear_solenoid_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_two,
        PDM_CHANNEL_3);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: pdm_2_get_2nd_gear_solenoid_current_feedback()
 *
 * Description: Return the current drawn sourced by this pdm output
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_2_get_2nd_gear_solenoid_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_two,
        PDM_CHANNEL_4);

    return current_feedback;

}


/******************************************************************************
 *
 *        Name: pdm_2_get_3rd_gear_solenoid_current_feedback()
 *
 * Description: Return the current drawn sourced by this pdm output
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_2_get_3rd_gear_solenoid_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_two,
        PDM_CHANNEL_5);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: pdm_2_get_4th_gear_solenoid_current_feedback()
 *
 * Description: Return the current drawn sourced by this pdm output
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_2_get_4th_gear_solenoid_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_two,
        PDM_CHANNEL_6);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: pdm_2_get_command_power_electronics_pump_current_feedback()
 *
 * Description: Return the current drawn sourced by this pdm output
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_2_get_power_electronics_pump_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_two,
        PDM_CHANNEL_7);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: pdm_2_get_command_selector_solenoid_current_feedback()
 *
 * Description: Return the current drawn sourced by this pdm output
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_2_get_selector_solenoid_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_two,
        PDM_CHANNEL_8);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: pdm_2_get_command_brake_release_solenoid_current_feedback()
 *
 * Description: Return the current drawn sourced by this pdm output
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_2_get_brake_release_solenoid_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_two,
        PDM_CHANNEL_9);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: pdm_2_get_command_service_brake_solenoid_current_feedback()
 *
 * Description: Return the current drawn sourced by this pdm output
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_2_get_service_brake_solenoid_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_two,
        PDM_CHANNEL_10);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: pdm_2_get_command_hydraulic_motor_cooling_pump_current_feedback()
 *
 * Description: Return the current drawn sourced by this pdm output
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_2_get_hydraulic_motor_cooling_pump_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_two,
        PDM_CHANNEL_11);

    return current_feedback;

}

/******************************************************************************
 *
 *        Name: pdm_2_get_command_hydraulic_motor_radiator_cooling_fan_current_feedback()
 *
 * Description: Return the current drawn sourced by this pdm output
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint8_t pdm_2_get_hydraulic_motor_radiator_cooling_fan_current_feedback()
{
    uint8_t current_feedback = 0;

    //
    // Obtain the current feedback reading using the getter function.
    //
    current_feedback = pdm_get_output_current_feedback(
        pdm_device_two,
        PDM_CHANNEL_12);

    return current_feedback;

}


/******************************************************************************
 *
 *        Name: pdm_2_get_rear_accum_pressure_analog_input()
 *
 * Description: Return the analog reading of the murphy
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint16_t pdm_2_get_rear_accum_pressure_analog_input()
{

    //
    // Read the sensor analog output.
    //
    uint16_t analog_reading =  pdm_get_analog_input(pdm_device_two, PDM_ADC_7);


//     uint16_t analog_reading =  pdm_get_analog_input(pdm_device_two, PDM_ADC_3);


    return analog_reading;

}


/******************************************************************************
 *
 *        Name: pdm_2_get_front_brake_pressure_analog_input()
 *
 * Description: Return the analog reading of the murphy
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint16_t pdm_2_get_front_brake_pressure_analog_input()
{

    //
    // Read the sensor analog output.
    //
    uint16_t analog_reading =  pdm_get_analog_input(pdm_device_two, PDM_ADC_4);

    return analog_reading;

}


/******************************************************************************
 *
 *        Name: pdm_2_get_front_axil_parking_pressure_analog_input()
 *
 * Description: Return the analog reading of the murphy
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint16_t pdm_2_get_front_axil_parking_pressure_analog_input()
{

    //
    // Read the sensor analog output.
    //
    uint16_t analog_reading =  pdm_get_analog_input(pdm_device_two, PDM_ADC_5);

    return analog_reading;

}


/******************************************************************************
 *
 *        Name: pdm_2_get_drive_shaft_brake_pressure_analog_input()
 *
 * Description: Return the analog reading of the murphy
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint16_t pdm_2_get_drive_shaft_brake_pressure_analog_input()
{

    //
    // Read the sensor analog output.
    //
    uint16_t analog_reading =  pdm_get_analog_input(pdm_device_two, PDM_ADC_6);

    return analog_reading;

}


/******************************************************************************
 *
 *        Name: pdm_2_get_transmission_oil_pressure_analog_input()
 *
 * Description: Return the analog reading of the murphy
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint16_t pdm_2_get_transmission_oil_pressure_analog_input()
{

    //
    // Read the sensor analog output.
    //
     uint16_t analog_reading =  pdm_get_analog_input(pdm_device_two, PDM_ADC_3);


//    uint16_t analog_reading =  pdm_get_analog_input(pdm_device_two, PDM_ADC_7);

    return analog_reading;

}


/******************************************************************************
 *
 *        Name: pdm_2_get_drive_shaft_brake_pressure_analog_input()
 *
 * Description: Return the analog reading of the murphy
 *
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */
uint16_t pdm_2_get_front_accum_pressure_analog_input()
{

    //
    // Read the sensor analog output.
    //
    uint16_t analog_reading =  pdm_get_analog_input(pdm_device_two, PDM_ADC_8);

    return analog_reading;

}


/******************************************************************************
 *
 *        Name: uint8_t pdm_2_get_seat_belt_switch_signal()
 *
 * Description: returns the value of the seat belt signal
 *
 *
 ******************************************************************************
 */
uint8_t pdm_2_get_seat_belt_switch_signal()
{

    //
    // Read the digital input
    //
    uint8_t seat_belt_signal =
        pdm_get_digital_input_feedback(pdm_device_two, PDM_CHANNEL_11);

    return seat_belt_signal;

}
