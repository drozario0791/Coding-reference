/******************************************************************************
 *
 *        Name: pdm_device_private.h
 *
 * Description: This is the private header file for the pdm device
 *              driver.
 *
 *      Author: Tom
 *        Date: Tuesday, 25 June 2019
 *
 ******************************************************************************
 */

#ifndef PDM_DEVICE_PRIVATE_H_
#define PDM_DEVICE_PRIVATE_H_

#include <string.h>
#include "can_service_devices.h"
#include "timer_service.h"

//////////////////////////////////////////////////////////////////////////////
//
// NOTE: There is a required #include file and two function prototypes
// at the end of this head file. This is required for all device
// drivers.
//
//////////////////////////////////////////////////////////////////////////////


//
//
//
//
typedef struct configure_output_function_s
{
    // All byte bit fields are msb to lsb
    uint32_t identifier : 8;
    uint32_t output_channel_number : 8;
    uint32_t soft_start_step : 8;
    uint32_t motor_lamp_mode : 8;

    // This reserved_6 field should always be 0b000000
    uint32_t reserved_6 : 6;
    uint32_t loss_of_comm : 2;

    // This reserved_8 field should always be 0xff
    uint32_t reserved_8 : 8;

    uint32_t por_command : 5;
    uint32_t por_enable : 1;
    uint32_t command_type : 1;
    uint32_t motor_breaking : 1;

    uint32_t lsc_digital_input : 4;
    // Unsupported, must always be 0b11
    uint32_t calibration_time : 2;
    uint32_t response_to_input : 2;

} configure_output_function_t;

//
// 4.3.2 Configure Output Channels
//
// The following CAN message also configures the outputs in two
// groups.  Configure Output Channels is a message sent to the PDM to
// set up the high-current output channels as a single high-side
// output or as an H-bridge pair. It also sets the current limit and
// reset behavior. Depending on the output configuration identifier,
// the message applies to either output channels 1-6 or 7-12
// respectively.
//
// Note: When channels are set to H-bridge pair, they are paired
// consecutively (i.e. 1 and 2, 3 and 4, etc.).
//
//     Tx Repetition: On change of state (at least one time, < 1 sec)
//       Data Length: 8 bytes
//         Data Page: 0
//   PDU Format (PF): 239 Proprietary A, PDU1 format
// PDU Specific (PS): 30 (1Eh) or as set DA (Source Address of the PDM)
//          Priority: 5
//  Param Grp Number: 61184 (EF00h)
//
// Byte.bit Parameter Name                                  Reference
//
// 1.1:8    Output Channel Group Identifier                 4.3.2.1
// 2.5:8    Current Limit (Output Channel 1 or 7)           4.3.2.2
// 2.3:4    Feedback Type (Output Channel 1 or 7)           4.3.2.3
// 2.2      Automatic Reset (Output Channel 1 or 7)         4.3.2.3
// 2.1      High-Side or H-Bridge (Output Channel 1 or 7)   4.3.2.5
// 3.5:8    Current Limit (Output Channel 2 or 8)           4.3.2.2
// 3.3:4    Feedback Type (Output Channel 2 or 8)           4.3.2.3
// 3.2      Automatic Reset (Output Channel 2 or 8)         4.3.2.3
// 3.1      High-Side or H-Bridge (Output Channel 2 or 8)   4.3.2.5
// 4.5:8    Current Limit (Output Channel 3 or 9)           4.3.2.2
// 4.3:4    Feedback Type (Output Channel 3 or 9)           4.3.2.3
// 4.2      Automatic Reset (Output Channel 3 or 9)         4.3.2.3
// 4.1      High-Side or H-Bridge (Output Channel 3 or 9)   4.3.2.5
// 5.5:8    Current Limit (Output Channel 4 or 10)          4.3.2.2
// 5.3:4    Feedback Type (Output Channel 4 or 10)          4.3.2.3
// 5.2      Automatic Reset (Output Channel 4 or 10)        4.3.2.3
// 5.1      High-Side or H-Bridge (Output Channel 4 or 10)  4.3.2.5
// 6.5:8    Current Limit (Output Channel 5 or 11)          4.3.2.2
// 6.3:4    Feedback Type (Output Channel 5 or 11)          4.3.2.3
// 6.2      Automatic Reset (Output Channel 5 or 11)        4.3.2.3
// 6.1      High-Side or H-Bridge (Output Channel 5 or 11)  4.3.2.5
// 7.5:8    Current Limit (Output Channel 6 or 12)          4.3.2.2
// 7.3:4    Feedback Type (Output Channel 6 or 12)          4.3.2.3
// 7.2      Automatic Reset (Output Channel 6 or 12)        4.3.2.3
// 7.1      High-Side or H-Bridge (Output Channel 6 or 12)  4.3.2.5
// 8.1:8    Reserved (FFh)
//
typedef struct configure_output_channels_s
{
    uint32_t output_channel_group_id : 8;

    uint32_t current_limit_1_7 : 4;
    uint32_t feedback_type_1_7 : 2;
    uint32_t automatic_reset_1_7 : 1;
    uint32_t highside_or_hbridge_1_7 : 1;

    uint32_t current_limit_2_8 : 4;
    uint32_t feedback_type_2_8 : 2;
    uint32_t automatic_reset_2_8 : 1;
    uint32_t highside_or_hbridge_2_8 : 1;

    uint32_t current_limit_3_9 : 4;
    uint32_t feedback_type_3_9 : 2;
    uint32_t automatic_reset_3_9 : 1;
    uint32_t highside_or_hbridge_3_9 : 1;

    uint32_t current_limit_4_10 : 4;
    uint32_t feedback_type_4_10 : 2;
    uint32_t automatic_reset_4_10 : 1;
    uint32_t highside_or_hbridge_4_10 : 1;

    uint32_t current_limit_5_11 : 4;
    uint32_t feedback_type_5_11 : 2;
    uint32_t automatic_reset_5_11 : 1;
    uint32_t highside_or_hbridge_5_11 : 1;

    uint32_t current_limit_6_12 : 4;
    uint32_t feedback_type_6_12 : 2;
    uint32_t automatic_reset_6_12 : 1;
    uint32_t highside_or_hbridge_6_12 : 1;

    // This reserved field must always be 0xff
    uint32_t reserved : 8;

} configure_output_channels_t;


//
// 4.4.1 Command Output Channels
//
// The command output channels message sets the PWM value of each
// output channel. The message bytes refer to outputs 1-6 or 7-12,
// depending on the value of the identifier in the first byte.
//
// CAN message sent to the PDM to drive the outputs.
//
// Byte.bit Parameter Name                                  Reference
//
// 1.1:8    Output Command Identifier                       4.4.1.1
// 2.1:8    Command (Output Channel 1 or 7)                 4.4.1.2
// 3.1:8    Command (Output Channel 2 or 8)                 4.4.1.2
// 4.1:8    Command (Output Channel 3 or 9)                 4.4.1.2
// 5.1:8    Command (Output Channel 4 or 10)                4.4.1.2
// 6.1:8    Command (Output Channel 5 or 11)                4.4.1.2
// 7.1:8    Command (Output Channel 6 or 12)                4.4.1.2
// 8.1      Enable (Output Channel 1 or 7)                  4.4.1.3
// 8.2      Enable (Output Channel 2 or 8)                  4.4.1.3
// 8.3      Enable (Output Channel 3 or 9)                  4.4.1.3
// 8.4      Enable (Output Channel 4 or 10)                 4.4.1.3
// 8.5      Enable (Output Channel 5 or 11)                 4.4.1.3
// 8.6      Enable (Output Channel 6 or 12)                 4.4.1.3
// 8.7:8    Module Transmit Rate / Unused                   4.4.1.4
//
typedef struct command_output_channels_s
{
    uint32_t output_command_identifier : 8;
    uint32_t command_1_7 : 8;
    uint32_t command_2_8 : 8;
    uint32_t command_3_9 : 8;
    uint32_t command_4_10 : 8;
    uint32_t command_5_11 : 8;
    uint32_t command_6_12 : 8;

    uint32_t module_transmit_rate : 2;
    uint32_t enable_6_12 : 1;
    uint32_t enable_5_11 : 1;
    uint32_t enable_4_10 : 1;
    uint32_t enable_3_9 : 1;
    uint32_t enable_2_8 : 1;
    uint32_t enable_1_7 : 1;

} command_output_channels_t;



//
// 4.5.1 Analog Inputs 1-2, Digital Inputs Feedback
//
// The analog channel feedback is the value of the input signal on the
// respective channel with 10 bit resolution. The digital input
// indicates if the input is open (floating), connected to ground or
// the battery.
//
// NOTE: Regardless of the Source Address of the configuring and
// controlling device, that device must listen for feedback messages
// addressed to Source Address 17 (11h).
//
// CAN message sent by the PDM to communicate the measured values.
//
//    Tx Repetition: 50 ms minimum (5x the base rate)
//      Data Length: 8 bytes
//        Data Page: 0
//  PDU Format (PF): 239, Proprietary A, PDU1 format
//    PDU Spec (PS): 17 (11h), DA (Source Address of the configuring display or controller)
//         Priority: 5
// Param Grp Number: 61184 (EF00h)
//
// Byte.bit Parameter Name                                  Reference
//
// 1.1:8    Feedback and Diagnostics Identifier             4.5.1.1
// 2.7:8    Digital Input 4                                 4.5.1.2
// 2.5:6    Digital Input 3                                 4.5.1.2
// 2.3:4    Digital Input 2                                 4.5.1.2
// 2.1:2    Digital Input 1                                 4.5.1.2
// 3.7:8    Digital Input 8                                 4.5.1.2
// 3.5:6    Digital Input 7                                 4.5.1.2
// 3.3:4    Digital Input 6                                 4.5.1.2
// 3.1:2    Digital Input 5                                 4.5.1.2
// 4.7:8    Digital Input 12                                4.5.1.2
// 4.5:6    Digital Input 11                                4.5.1.2
// 4.3:4    Digital Input 10                                4.5.1.2
// 4.1:2    Digital Input 9                                 4.5.1.2
// 5.1:16   Analog Input 1                                  4.5.1.3
// 7.1:16   Analog Input 2                                  4.5.1.3
//
typedef struct analog_in_1_2_digital_in_feedback_s
{
    uint32_t identifier : 8;

    uint32_t digital_input_4 : 2;
    uint32_t digital_input_3 : 2;
    uint32_t digital_input_2 : 2;
    uint32_t digital_input_1 : 2;

    uint32_t digital_input_8 : 2;
    uint32_t digital_input_7 : 2;
    uint32_t digital_input_6 : 2;
    uint32_t digital_input_5 : 2;

    uint32_t digital_input_12 : 2;
    uint32_t digital_input_11 : 2;
    uint32_t digital_input_10 : 2;
    uint32_t digital_input_9 : 2;

    // These need BYTE_SWAP16() when read
    uint32_t analog_input_1 : 16;
    uint32_t analog_input_2 : 16;

} analog_in_1_2_digital_in_feedback_t;


typedef struct analog_in_3_4_digital_out_feedback_s
{
    uint32_t identifier : 8;

    uint32_t output_diagnostic_1 : 2;
    uint32_t output_diagnostic_2 : 2;
    uint32_t output_diagnostic_3 : 2;
    uint32_t output_diagnostic_4 : 2;
    uint32_t output_diagnostic_5 : 2;
    uint32_t output_diagnostic_6 : 2;
    uint32_t output_diagnostic_7 : 2;
    uint32_t output_diagnostic_8 : 2;
    uint32_t output_diagnostic_9 : 2;
    uint32_t output_diagnostic_10 : 2;
    uint32_t output_diagnostic_11 : 2;
    uint32_t output_diagnostic_12 : 2;

    // These need BYTE_SWAP16() when read
    uint32_t analog_input_3 : 16;
    uint32_t analog_input_4 : 16;

} analog_in_3_4_digital_out_feedback_t;


typedef struct analog_in_5_6_battery_sensor_supply_feedback_s
{
    uint32_t identifier : 8;

    uint32_t unused : 6;
    uint32_t sensor_supply_high : 1;
    uint32_t sensor_supply_low : 1;

    // These need BYTE_SWAP16() when read
    uint32_t battery_voltage : 16;
    uint32_t analog_input_5 : 16;
    uint32_t analog_input_6 : 16;

} analog_in_5_6_battery_sensor_supply_feedback_t;


typedef struct analog_in_7_8_power_version_feedback_s
{
    uint32_t identifier : 8;

    uint32_t power_supply_status : 1;
    uint32_t total_current_status : 1;
    uint32_t unused : 6;

    // These need BYTE_SWAP16() when read
    uint32_t software_version : 16;
    uint32_t analog_input_7 : 16;
    uint32_t analog_input_8 : 16;

} analog_in_7_8_power_version_feedback_t;


typedef struct output_current_feedback_s
{
    uint32_t identifier : 8;
    uint32_t output_1_7_feedback : 8;
    uint32_t output_2_8_feedback : 8;
    uint32_t output_3_9_feedback : 8;
    uint32_t output_4_10_feedback : 8;
    uint32_t output_5_11_feedback : 8;
    uint32_t output_6_12_feedback : 8;
    uint32_t unused : 8;

} output_current_feedback_t;


typedef struct output_function_handshake_s
{
    uint32_t identifier : 8;
    uint32_t channel_number : 8;
    uint32_t soft_start_step_size : 8;
    uint32_t motor_lamp_mode : 8;
    uint32_t loss_of_communication : 8;
    uint32_t reserved : 8;

    uint32_t power_on_reset_command : 5;
    uint32_t power_on_reset_enable : 1;
    uint32_t command_type : 1;
    uint32_t motor_braking : 1;

    uint32_t digital_input : 4;
    uint32_t calibration_time : 2;
    uint32_t response : 2;

} output_function_handshake_t;


typedef struct output_configuration_handshake_s
{
    uint32_t identifier : 8;

    uint32_t current_limit_output_1_7 : 4;
    uint32_t feedback_type_output_1_7 : 2;
    uint32_t automatic_reset_output_1_7 : 1;
    uint32_t highside_or_hbridge_ouput_1_7 : 1;

    uint32_t current_limit_output_2_8 : 4;
    uint32_t feedback_type_output_2_8 : 2;
    uint32_t automatic_reset_output_2_8 : 1;
    uint32_t highside_or_hbridge_ouput_2_8 : 1;

    uint32_t current_limit_output_3_9 : 4;
    uint32_t feedback_type_output_3_9 : 2;
    uint32_t automatic_reset_output_3_9 : 1;
    uint32_t highside_or_hbridge_ouput_3_9 : 1;

    uint32_t current_limit_output_4_10 : 4;
    uint32_t feedback_type_output_4_10 : 2;
    uint32_t automatic_reset_output_4_10 : 1;
    uint32_t highside_or_hbridge_ouput_4_10 : 1;

    uint32_t current_limit_output_5_11 : 4;
    uint32_t feedback_type_output_5_11 : 2;
    uint32_t automatic_reset_output_5_11 : 1;
    uint32_t highside_or_hbridge_ouput_5_11 : 1;

    uint32_t current_limit_output_6_12 : 4;
    uint32_t feedback_type_output_6_12 : 2;
    uint32_t automatic_reset_output_6_12 : 1;
    uint32_t highside_or_hbridge_ouput_6_12 : 1;

    uint32_t reserved : 8;

} output_configuration_handshake_t;



//
// A compound structure that encompasses all the RX CAN messages
// related to the Murphy PDM. One of these is allocated for each
// instantiation of a PDM.
//
typedef struct device_data_s {
    struct device_data_s *next_ptr;
    uint8_t device_instance_number;

    // These are the receive CAN message data structures.
    analog_in_1_2_digital_in_feedback_t analog_in_1_2_digital_in_feedback;
    analog_in_3_4_digital_out_feedback_t analog_in_3_4_digital_out_feedback;
    analog_in_5_6_battery_sensor_supply_feedback_t analog_in_5_6_battery_sensor_supply_feedback;
    analog_in_7_8_power_version_feedback_t analog_in_7_8_power_version_feedback;
    output_current_feedback_t output_current_1_6_feedback;
    output_current_feedback_t output_current_7_12_feedback;
    output_function_handshake_t output_function_handshake[12];
    output_configuration_handshake_t output_configuration_1_6_handshake;
    output_configuration_handshake_t output_configuration_7_12_handshake;

    // Each of the PDM CAN receive functions sets bool associated with
    // itself to TRUE when its message is received.
    bool_t analog_in_1_2_digital_in_feedback_rx_ok;
    bool_t analog_in_3_4_digital_out_feedback_rx_ok;
    bool_t analog_in_5_6_battery_sensor_supply_feedback_rx_ok;
    bool_t analog_in_7_8_power_version_feedback_rx_ok;
    bool_t output_current_1_6_feedback_rx_ok;
    bool_t output_current_7_12_feedback_rx_ok;
    bool_t output_function_handshake_rx_ok;
    bool_t output_configuration_1_6_handshake_rx_ok;
    bool_t output_configuration_7_12_handshake_rx_ok;

    // Each of the PDM CAN receive functions sets bool associated with
    // itself to TRUE when its message is received.
    uint32_t analog_in_1_2_digital_in_feedback_rx_cnt;
    uint32_t analog_in_3_4_digital_out_feedback_rx_cnt;
    uint32_t analog_in_5_6_battery_sensor_supply_feedback_rx_cnt;
    uint32_t analog_in_7_8_power_version_feedback_rx_cnt;
    uint32_t output_current_1_6_feedback_rx_cnt;
    uint32_t output_current_7_12_feedback_rx_cnt;
    uint32_t output_function_handshake_rx_cnt;
    uint32_t output_configuration_1_6_handshake_rx_cnt;
    uint32_t output_configuration_7_12_handshake_rx_cnt;

    // These are the transmit CAN message data structures.
    configure_output_function_t configure_output_function[12];
    configure_output_channels_t configure_output_channels_1_6;
    configure_output_channels_t configure_output_channels_7_12;
    command_output_channels_t command_output_channels_1_6;
    command_output_channels_t command_output_channels_7_12;

    // These are pointers to CAN tx registration records.
    can_tx_registration_t *send_configure_output_function_ptr;
    can_tx_registration_t *send_configure_output_channels_1_6_ptr;
    can_tx_registration_t *send_configure_output_channels_7_12_ptr;
    can_tx_registration_t *send_command_output_channels_1_6_ptr;
    can_tx_registration_t *send_command_output_channels_7_12_ptr;
} device_data_t;


//////////////////////////////////////////////////////////////////////////////
//
// Private function prototypes
//
//
//////////////////////////////////////////////////////////////////////////////
//
void rx_message_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);



//////////////////////////////////////////////////////////////////////////////
//
// Note: These two include files must be located here. This is the
// structure we are following with the idea that all of these device
// drivers will be built into a libray.
//
//////////////////////////////////////////////////////////////////////////////
//
#include "pdm_device.h"
#include "Prototypes_Time.h"


//////////////////////////////////////////////////////////////////////////////
//
// Note: These two function prototypes of functions defined in
// can_service.c are required here at this point in each device's
// private header file.
//
//////////////////////////////////////////////////////////////////////////////
//
device_data_t *
create_device_linked_data_record(
    device_instances_t device_instance,
    device_data_t **first_device_data_ptr,
    uint8_t *device_instance_counter,
    uint32_t sizeof_device_data_t);


device_data_t *
get_device_linked_data_record_instance_ptr(
    device_instances_t device,
    device_data_t *first_device_data_ptr);


#endif


