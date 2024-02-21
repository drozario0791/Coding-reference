/******************************************************************************
 *
 *        Name: pdm_control_n.c
 *
 * Description: The Carrier specific PDM channels are configured and
 *              initialized in this module. The command channel output
 *              functions are named with the Carrier wiring diagram
 *              names.
 *
 *              The pdm_init(nnn, 0, CAN2) function must first be
 *              called from User_Init().
 *
 *              The pdm_control_n_init() must then be called from
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
#include "pdm_control_n.h"

static device_instances_t pdm_device_nnn = ABC;

//
// pdm_control_2 output channel configure functions.
//
static void pdm_nnn_init_xx1_pwr();
static void pdm_nnn_init_xx2_pwr();
static void pdm_nnn_init_xx3_pwr();
static void pdm_nnn_init_xx4_pwr();
static void pdm_nnn_init_xx5_pwr();
static void pdm_nnn_init_xx6_pwr();
static void pdm_nnn_init_xx7_pwr();
static void pdm_nnn_init_xx8_pwr();
static void pdm_nnn_init_xx9_pwr();
static void pdm_nnn_init_xx10_pwr();
static void pdm_nnn_init_xx11_pwr();
static void pdm_nnn_init_xx12_pwr();


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
void pdm_control_nnn_init(){

    //
    // Initialise the pdm_nnn_control channels.
    //
    pdm_nnn_init_xx1_pwr();
    pdm_nnn_init_xx2_pwr();
    pdm_nnn_init_xx3_pwr();
    pdm_nnn_init_xx4_pwr();
    pdm_nnn_init_xx5_pwr();
    pdm_nnn_init_xx6_pwr();
    pdm_nnn_init_xx7_pwr();
    pdm_nnn_init_xx8_pwr();
    pdm_nnn_init_xx9_pwr();
    pdm_nnn_init_xx10_pwr();
    pdm_nnn_init_xx11_pwr();
    pdm_nnn_init_xx12_pwr();

    //
    // Send the TX configure output function commands
    //
    pdm_tx_can_configure_output_function(
        pdm_device_nnn,
        TX_SEND_EACH_CALL);

    //
    // Set the default output channel values for the PDM.
    //
    pdm_nnn_command_xx1_pwr(FALSE);
    pdm_nnn_command_xx2_pwr(FALSE);
    pdm_nnn_command_xx3_pwr(FALSE);
    pdm_nnn_command_xx4_pwr(FALSE);
    pdm_nnn_command_xx5_pwr(FALSE);
    pdm_nnn_command_xx6_pwr(FALSE);
    pdm_nnn_command_xx7_pwr(FALSE);
    pdm_nnn_command_xx8_pwr(FALSE);
    pdm_nnn_command_xx9_pwr(FALSE);
    pdm_nnn_command_xx10_pwr(FALSE);
    pdm_nnn_command_xx11_pwr(FALSE);
    pdm_nnn_command_xx12_pwr(FALSE);
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
 *        Name: pdm_nnn_init_xx1_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_nnn_init_xx1_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_nnn,
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
        pdm_device_nnn,
        PDM_CHANNEL_1,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_nnn_init_xx2_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_nnn_init_xx2_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_nnn,
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
        pdm_device_nnn,
        PDM_CHANNEL_2,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_nnn_init_xx3_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_nnn_init_xx3_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_nnn,
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
        pdm_device_nnn,
        PDM_CHANNEL_3,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_nnn_init_xx4_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_nnn_init_xx4_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_nnn,
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
        pdm_device_nnn,
        PDM_CHANNEL_4,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_nnn_init_xx5_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_nnn_init_xx5_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_nnn,
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
        pdm_device_nnn,
        PDM_CHANNEL_5,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_nnn_init_xx5_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_nnn_init_xx5_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_nnn,
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
        pdm_device_nnn,
        PDM_CHANNEL_6,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_nnn_init_xx7_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_nnn_init_imt_xx7_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_nnn,
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
        pdm_device_nnn,
        PDM_CHANNEL_7,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_nnn_init_xx8_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_nnn_init_xx8_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_nnn,
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
        pdm_device_nnn,
        PDM_CHANNEL_8,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_nnn_init_xx9_pwr()
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_nnn_init_xx9_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_nnn,
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
        pdm_device_nnn,
        PDM_CHANNEL_9,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_nnn_init_xx10_pwr
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_nnn_init_xx10_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_nnn,
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
        pdm_device_nnn,
        PDM_CHANNEL_10,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_nnn_init_xx11_pwr
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_nnn_init_xx11_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_nnn,
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
        pdm_device_nnn,
        PDM_CHANNEL_11,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}


/******************************************************************************
 *
 *        Name: pdm_nnn_init_xx12_pwr
 *
 * Description: Configure the function and output for this channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void pdm_nnn_init_xx12_pwr()
{
    //
    // Configure output function.
    //
    pdm_set_configure_output_function(
        pdm_device_nnn,
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
        pdm_device_nnn,
        PDM_CHANNEL_12  ,
        PDM_CURRENT_LIMIT_15_0A,
        PDM_CHANNEL_AUTOMATIC_RESET_NO,
        PDM_CHANNEL_HIGH_SIDE);
}





//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// pdm_device_nnn output channel command functions.
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


/******************************************************************************
 *
 *        Name: pdm_nnn_command_xx1_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_nnn_command_xx1_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_nnn,
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
            pdm_device_nnn,
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
        pdm_device_nnn,
        TX_SEND_EACH_CALL);
}


/******************************************************************************
 *
 *        Name: pdm_nnn_command_xx2_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_nnn_command_xx2_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_nnn,
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
            pdm_device_nnn,
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
        pdm_device_nnn,
        TX_SEND_EACH_CALL);
}


/******************************************************************************
 *
 *        Name: pdm_nnn_command_xx3_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_nnn_command_xx3_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_nnn,
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
            pdm_device_nnn,
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
        pdm_device_nnn,
        TX_SEND_EACH_CALL);
}


/******************************************************************************
 *
 *        Name: pdm_nnn_command_xx4_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_nnn_command_xx4_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_nnn,
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
            pdm_device_nnn,
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
        pdm_device_nnn,
        TX_SEND_EACH_CALL);
}


/******************************************************************************
 *
 *        Name: pdm_nnn_command_xx5_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_nnn_command_xx5_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_nnn,
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
            pdm_device_nnn,
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
        pdm_device_nnn,
        TX_SEND_EACH_CALL);
}


/******************************************************************************
 *
 *        Name: pdm_nnn_command_xx6_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_nnn_command_xx6_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_nnn,
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
            pdm_device_nnn,
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
        pdm_device_nnn,
        TX_SEND_EACH_CALL);
}


/******************************************************************************
 *
 *        Name: pdm_nnn_command_xx7_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_nnn_command_xx7_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_nnn,
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
            pdm_device_nnn,
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
        pdm_device_nnn,
        TX_SEND_EACH_CALL);
}


/******************************************************************************
 *
 *        Name: pdm_nnn_command_xx8_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_nnn_command_xx8_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_nnn,
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
            pdm_device_nnn,
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
        pdm_device_nnn,
        TX_SEND_EACH_CALL);
}


/******************************************************************************
 *
 *        Name: pdm_nnn_command_xx9_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_nnn_command_xx9_pwr(
    bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_nnn,
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
            pdm_device_nnn,
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
        pdm_device_nnn,
        TX_SEND_EACH_CALL);
}


/******************************************************************************
 *
 *        Name: pdm_nnn_command_xx10_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_nnn_command_xx10_pwr(
        bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_nnn,
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
            pdm_device_nnn,
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
        pdm_device_nnn,
        TX_SEND_EACH_CALL);
}


/******************************************************************************
 *
 *        Name: pdm_nnn_command_xx11_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_nnn_command_xx11_pwr(
        bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_nnn,
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
            pdm_device_nnn,
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
        pdm_device_nnn,
        TX_SEND_EACH_CALL);
}


/******************************************************************************
 *
 *        Name: pdm_nnn_command_xx12_pwr()
 *
 * Description: Command this output channel.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pdm_nnn_command_xx12_pwr(
        bool_t switch_on)
{
    switch (switch_on)
    {
    case TRUE:
        //
        // Switch on the PDM output channel.
        //
        pdm_set_command_output_channel(
            pdm_device_nnn,
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
            pdm_device_nnn,
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
        pdm_device_nnn,
        TX_SEND_EACH_CALL);
}
