/******************************************************************************
 *
 *        Name: pdm_control_n.h
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


#ifndef PDM_CONTROL_n
#define PDM_CONTROL_n

//
// Rear deck init PDM functions and set output channels to default
// state.
//
void pdm_control_nnn_init();



//
// pdm_device_one output channel command functions.
//
void pdm_2_command_xx1_pwr(
    bool_t switch_on);

void pdm_2_command_xx2_pwr(
    bool_t switch_on);

void pdm_2_command_xx3_pwr(
    bool_t switch_on);

void pdm_2_command_xx4_pwr(
    bool_t switch_on);

void pdm_2_command_xx5_pwr(
    bool_t switch_on);

void pdm_2_command_xx6_pwr(
    bool_t switch_on);

void pdm_2_command_xx7_pwr(
    bool_t switch_on);

void pdm_2_command_xx8_pwr(
    bool_t switch_on);

void pdm_2_command_xx9_pwr(
    bool_t switch_on);

void pdm_2_command_xx10_pwr(
    bool_t switch_on);

void pdm_2_command_xx11_pwr(
    bool_t switch_on);

void pdm_2_command_xx12_pwr(
    bool_t switch_on);


#endif // PDM_DEVICE_ONE
