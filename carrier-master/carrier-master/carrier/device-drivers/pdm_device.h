/******************************************************************************
 *
 *        Name: pdm_device.h
 *
 * Description: This is the public facing header file for the
 *              pdm_device, containing enum typedefs and public
 *              functions.
 *
 *      Author: Tom
 *        Date: Tuesday, 25 June 2019
 *
 ******************************************************************************
 */

#ifndef PDM_DEVICE_H_
#define PDM_DEVICE_H_

//
// The PDM has 12 channels, each accessed by one of the following
// names.
//
typedef enum {
    PDM_CHANNEL_1 = 1,
    PDM_CHANNEL_2 = 2,
    PDM_CHANNEL_3 = 3,
    PDM_CHANNEL_4 = 4,
    PDM_CHANNEL_5 = 5,
    PDM_CHANNEL_6 = 6,
    PDM_CHANNEL_7 = 7,
    PDM_CHANNEL_8 = 8,
    PDM_CHANNEL_9 = 9,
    PDM_CHANNEL_10 = 10,
    PDM_CHANNEL_11 = 11,
    PDM_CHANNEL_12 = 12,
    PDM_CHANNEL_CNT = 12,
} pdm_dio_channels_t;

//
// There are 8 PDM analog inputs, each accessed by one of the
// following names.
//
typedef enum {
    // The ADC inputs 1 & 2 are resistive
    PDM_ADC_1_R = 1,
    PDM_ADC_2_R = 2,
    // The ADC inputs 3-8 are 0-5V
    PDM_ADC_3 = 3,
    PDM_ADC_4 = 4,
    PDM_ADC_5 = 5,
    PDM_ADC_6 = 6,
    PDM_ADC_7 = 7,
    PDM_ADC_8 = 8
} pdm_adc_channels_t;

typedef enum {
    PDM_LAMP_MODE = 0,
    PDM_MOTOR_MODE = 1
} pdm_output_motor_lamp_mode_t;

typedef enum {
    PDM_MODULE_TX_RATE_500MS = 0,
    PDM_MODULE_TX_RATE_250MS = 1,
    PDM_MODULE_TX_RATE_50MS  = 2,
    PDM_MODULE_TX_RATE_10MS  = 3,
} pdm_module_tx_rate_t;

typedef enum {
    PDM_CURRENT_LIMIT_0_0A = 0,
    PDM_CURRENT_LIMIT_2_5A = 1,
    PDM_CURRENT_LIMIT_5_0A = 2,
    PDM_CURRENT_LIMIT_7_5A = 3,
    PDM_CURRENT_LIMIT_10_0A = 4,
    PDM_CURRENT_LIMIT_12_5A = 5,
    PDM_CURRENT_LIMIT_15_0A = 6,
} pdm_output_channel_current_limit_t;

typedef enum {
    PDM_CHANNEL_AUTOMATIC_RESET_YES = 0,
    PDM_CHANNEL_AUTOMATIC_RESET_NO = 1,
} pdm_configure_channel_automatic_reset_t;

typedef enum {
    PDM_CHANNEL_HIGH_SIDE = 0,
    PDM_CHANNEL_H_BRIDGE = 1,
} pdm_configure_channel_high_or_bridge_t;

typedef enum {
    PDM_CHANNEL_LOSS_COMM_NO_CHANGE = 0,
    PDM_CHANNEL_LOSS_COMM_NEG_100_PERCENT = 1,
    PDM_CHANNEL_LOSS_COMM_100_PERCENT = 2,
    PDM_CHANNEL_LOSS_COMM_0_PERCENT = 3,
} pdm_configure_channel_loss_of_comm_t;

typedef enum {
    PDM_CONFIGURE_OUTPUT_POR_ENABLE_ON = 0,
    PDM_CONFIGURE_OUTPUT_POR_ENABLE_OFF = 1,
} pdm_configure_output_por_enable_t;

typedef enum {
    PDM_CONFIGURE_OUTPUT_COMMAND_TYPE_ON = 0,
    PDM_CONFIGURE_OUTPUT_COMMAND_TYPE_OFF = 1,
} pdm_configure_output_command_type_t;

typedef enum {
    PDM_CONFIGURE_OUTPUT_MOTOR_BREAKING_OFF = 0,
    PDM_CONFIGURE_OUTPUT_MOTOR_BREAKING_ON = 1,
} pdm_configure_output_motor_breaking_t;

typedef enum {
    PDM_LSC_INPUT_CHANNEL_1 = 0,
    PDM_LSC_INPUT_CHANNEL_2 = 1,
    PDM_LSC_INPUT_CHANNEL_3 = 2,
    PDM_LSC_INPUT_CHANNEL_4 = 3,
    PDM_LSC_INPUT_CHANNEL_5 = 4,
    PDM_LSC_INPUT_CHANNEL_6 = 5,
    PDM_LSC_INPUT_CHANNEL_7 = 6,
    PDM_LSC_INPUT_CHANNEL_8 = 7,
    PDM_LSC_INPUT_CHANNEL_9 = 8,
    PDM_LSC_INPUT_CHANNEL_10 = 9,
    PDM_LSC_INPUT_CHANNEL_11 = 10,
    PDM_LSC_INPUT_CHANNEL_12 = 11,
    PDM_LSC_DISABLED = 15,
} pdm_configure_output_lsc_channel_t;

typedef enum {
    PDM_CONFIGURE_OUTPUT_CHANNEL_RESPONSE_ACTIVE_LOW = 1,
    PDM_CONFIGURE_OUTPUT_CHANNEL_RESPONSE_ACTIVE_HIGH = 2,
    PDM_CONFIGURE_OUTPUT_CHANNEL_RESPONSE_ACTIVE_HIGH_OR_LOW = 3,
} pdm_configure_output_channel_response_t;


//
// Public init and CAN transmit functions.
//
void pdm_init(device_instances_t device,
              uint8_t module_id,
              CANLINE_ can_line);

void pdm_tx_can_configure_output_function(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void pdm_tx_can_configure_output_channels_1_6(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void pdm_tx_can_configure_output_channels_7_12(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void pdm_tx_can_command_output_channels_1_6(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void pdm_tx_can_command_output_channels_7_12(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

//
// Public functions to set the timeout counter limit for each of the
// PDM receive CAN messages.
//
void pdm_set_timeout_analog_in_1_2_digital_in_feedback(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);

void pdm_set_timeout_analog_in_3_4_digital_out_feedback(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);

void pdm_set_timeout_analog_in_5_6_battery_sensor_supply_feedback(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);

void pdm_set_timeout_analog_in_7_8_power_version_feedback(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);

void pdm_set_timeout_output_current_1_6_feedback(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);

void pdm_set_timeout_output_current_7_12_feedback(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);

void pdm_set_timeout_output_function_handshake(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);

void pdm_set_timeout_output_configuration_1_6_handshake(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);

void pdm_set_timeout_output_configuration_7_12_handshake(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);

//
// Public Getter functions
//
uint8_t pdm_get_digital_input_feedback(device_instances_t device, pdm_dio_channels_t channel);
uint8_t pdm_get_digital_output_dignostic(device_instances_t device, pdm_dio_channels_t channel);
uint8_t pdm_get_sensor_supply_high(device_instances_t device);
uint8_t pdm_get_sensor_supply_low(device_instances_t device);
uint16_t pdm_get_battery_voltage_mv(device_instances_t device);
uint8_t pdm_get_power_supply_status(device_instances_t device);
uint8_t pdm_get_total_current_status(device_instances_t device);
uint16_t pdm_get_software_version(device_instances_t device);
uint8_t pdm_get_output_current_feedback(device_instances_t device, pdm_dio_channels_t channel);
uint8_t pdm_get_soft_start_step_size(device_instances_t device, pdm_dio_channels_t channel);
uint8_t pdm_get_motor_lamp_mode(device_instances_t device, pdm_dio_channels_t channel);
uint8_t pdm_get_loss_of_communication(device_instances_t device, pdm_dio_channels_t channel);
uint8_t pdm_get_power_on_reset_command(device_instances_t device, pdm_dio_channels_t channel);
bool_t pdm_get_power_on_reset_enable(device_instances_t device, pdm_dio_channels_t channel);
bool_t pdm_get_command_type(device_instances_t device, pdm_dio_channels_t channel);
bool_t pdm_get_motor_braking(device_instances_t device, pdm_dio_channels_t channel);
uint8_t pdm_get_digital_input(device_instances_t device, pdm_dio_channels_t channel);
uint8_t pdm_get_calibration_time(device_instances_t device, pdm_dio_channels_t channel);
uint8_t pdm_get_response(device_instances_t device, pdm_dio_channels_t channel);
uint16_t pdm_get_analog_input(device_instances_t device, pdm_adc_channels_t channel);
uint8_t pdm_get_current_limit_output(device_instances_t device, pdm_dio_channels_t channel);
uint8_t pdm_get_feedback_type_output(device_instances_t device, pdm_dio_channels_t channel);
uint8_t pdm_get_automatic_reset_output(device_instances_t device, pdm_dio_channels_t channel);
uint8_t pdm_get_highside_or_hbridge_ouput(device_instances_t device, pdm_dio_channels_t channel);
bool_t pdm_get_can_rx_ok(device_instances_t device);
bool_t pdm_get_output_channel_command_state(device_instances_t device, pdm_dio_channels_t channel);

//
//
//
bool_t pdm_compare_analog_in_1_2_digital_in_feedback_rx_cnt(
    device_instances_t device, uint32_t compare_cnt);

bool_t pdm_compare_analog_in_3_4_digital_out_feedback_rx_cnt(
    device_instances_t device, uint32_t compare_cnt);

bool_t pdm_compare_analog_in_5_6_battery_sensor_supply_feedback_rx_cnt(
    device_instances_t device, uint32_t compare_cnt);

bool_t pdm_compare_analog_in_7_8_power_version_feedback_rx_cnt(
    device_instances_t device, uint32_t compare_cnt);

bool_t pdm_compare_output_current_1_6_feedback_rx_cnt(
    device_instances_t device, uint32_t compare_cnt);

bool_t pdm_compare_output_current_7_12_feedback_rx_cnt(
    device_instances_t device, uint32_t compare_cnt);

bool_t pdm_compare_output_function_handshake_rx_cnt(
    device_instances_t device, uint32_t compare_cnt);

bool_t pdm_compare_output_configuration_1_6_handshake_rx_cnt(
    device_instances_t device, uint32_t compare_cnt);

bool_t pdm_compare_output_configuration_7_12_handshake_rx_cnt(
    device_instances_t device, uint32_t compare_cnt);

//
//
//
void pdm_zero_analog_in_1_2_digital_in_feedback_rx_cnt(
    device_instances_t device);

void pdm_zero_analog_in_3_4_digital_out_feedback_rx_cnt(
    device_instances_t device);

void pdm_zero_analog_in_5_6_battery_sensor_supply_feedback_rx_cnt(
    device_instances_t device);

void pdm_zero_analog_in_7_8_power_version_feedback_rx_cnt(
    device_instances_t device);

void pdm_zero_output_current_1_6_feedback_rx_cnt(
    device_instances_t device);

void pdm_zero_output_current_7_12_feedback_rx_cnt(
    device_instances_t device);

void pdm_zero_output_function_handshake_rx_cnt(
    device_instances_t device);

void pdm_zero_output_configuration_1_6_handshake_rx_cnt(
    device_instances_t device);

void pdm_zero_output_configuration_7_12_handshake_rx_cnt(
    device_instances_t device);




//
// Public setter functions.
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
    pdm_configure_output_motor_breaking_t motor_braking,
    pdm_configure_output_lsc_channel_t lsc_digital_input,
    pdm_configure_output_channel_response_t response_to_input);

void pdm_set_configure_output_channel(
    device_instances_t device,
    pdm_dio_channels_t channel,
    pdm_output_channel_current_limit_t current_limit,
    pdm_configure_channel_automatic_reset_t automatic_reset,
    pdm_configure_channel_high_or_bridge_t highside_or_hbridge);

void pdm_set_command_output_channel(
    device_instances_t device,
    pdm_dio_channels_t channel,
    int8_t command_percent,
    bool_t enable,
    pdm_module_tx_rate_t module_transmit_rate);


#endif // PDM_DEVICE_H_
