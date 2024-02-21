/******************************************************************************
 *
 *        Name: skai2_inverter_private.h
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Friday, 09 August 2019
 *
 ******************************************************************************
 */

#ifndef  SKAI2_INVERTER_PRIVATE_VISSIM_H_
#define  SKAI2_INVERTER_PRIVATE_VISSIM_H_

#include <string.h>
#include "can_service_devices.h"


//////////////////////////////////////////////////////////////////////////////
//
// NOTE: There is a required #include file and two function prototypes
// at the end of this head file. This is required for all device
// drivers.
//
//////////////////////////////////////////////////////////////////////////////

//
// FVT Motor Control RTOS RX Messages
//
// Reference: fvt_inverter_control_skai/eeprom.c & processCanRXData
//
// CAN ID: 0x00X01001:
//
typedef struct {
    uint32_t error_feedback_1 : 16;
    uint32_t error_feedback_2 : 16;
	uint32_t MotorTemp_C : 16;
	uint32_t DCLink_Voltage : 16;
} skai2_inverter_rx_msg1_t;

//
// CAN ID: 0x00X01002:
//
typedef struct {
    uint32_t auxilary_input_voltage :16;
    uint32_t throttle :16;
	uint32_t version_number :16;
	uint32_t unused_1 :16;
} skai2_inverter_rx_msg2_t;


//
// CAN ID: 0x00X0103:
//
typedef struct {
    uint32_t torque :16;
    uint32_t motor_rpm :16;
	uint32_t encoder_feedback :16;
	uint32_t unused_1 :16;
} skai2_inverter_rx_msg3_t;


//
// CAN ID: 0x00X01004:
//
typedef struct {
    uint32_t throttle_pedal_adc :16;
    uint32_t IO_status :16;
	uint32_t electrical_angle :16;
	uint32_t unused_1 :16;
} skai2_inverter_rx_msg4_t;


//
// CAN ID: 0x00X01005:
//
typedef struct {
    uint32_t dcb_phase1_temperature :16;
    uint32_t dcb_phase2_temperature :16;
	uint32_t dcb_phase3_temperature :16;
	uint32_t pcb_temperature :16;
} skai2_inverter_rx_msg5_t;


//
// CAN ID: 0x00X01009:
//
typedef struct {
    uint32_t power_consumned :16;
    uint32_t max_temperature :16;
	uint32_t inverter_enable :16;
	uint32_t unused_1 :16;
} skai2_inverter_rx_msg9_t;



//////////////////////////////////////////////////////////////////////////////
//
// Transmit Messages
//
//////////////////////////////////////////////////////////////////////////////

//
// CAN ID: 0x00X11001:
//
typedef struct {
    uint32_t d_proportional_gain :16;
    uint32_t d_integral_gain :16;
	uint32_t d_differential_gain :16;
	uint32_t d_time_constant :16;
} skai2_inverter_tx_msg1_t;


//
// CAN ID: 0x00X11002:
//
typedef struct {
    uint32_t q_proportional_gain :16;
    uint32_t q_integral_gain :16;
	uint32_t q_differential_gain :16;
	uint32_t q_time_constant :16;
} skai2_inverter_tx_msg2_t;

//
// CAN ID: 0x00X11003:
//
typedef struct {
    uint32_t speed_proportional_gain :16;
    uint32_t speed_integral_gain :16;
	uint32_t speed_differential_gain :16;
	uint32_t speed_time_constant :16;
} skai2_inverter_tx_msg3_t;

//
// CAN ID: 0x00X11004:
//
typedef struct {
    uint32_t electrical_angle_forward :16;
    uint32_t electrical_angle_reverse :16;
	uint32_t electrical_angle_forward_regen :16;
	uint32_t electrical_angle_reverse_regen :16;
} skai2_inverter_tx_msg4_t;

//
// CAN ID: 0x00X11005:
//
typedef struct {
    uint32_t inverter_enable :16;
    uint32_t max_battery_current  :16;
	uint32_t pack_state_of_charge :16;
	uint32_t high_cell_voltage :16;
} skai2_inverter_tx_msg5_t;

//
// CAN ID: 0x00X11006:
//
typedef struct {
    uint32_t motor_scale_P1 :16;
    uint32_t motor_scale_P2 :16;
	uint32_t motor_scale_L1 :16;
	uint32_t motor_scale_L2 :16;
} skai2_inverter_tx_msg6_t;

//
// CAN ID: 0x00X11007:
//
typedef struct {
    uint32_t regen_scale_P1 :16;
    uint32_t regen_scale_P2 :16;
	uint32_t regen_scale_L1 :16;
	uint32_t regen_scale_L2 :16;
} skai2_inverter_tx_msg7_t;

//
// CAN ID: 0x00X11008:
//
typedef struct {
    uint32_t throttle_gain_1 :16;
    uint32_t throttle_gain_2 :16;
	uint32_t df_gain :16;
	uint32_t qf_gain :16;
} skai2_inverter_tx_msg8_t;

//
// CAN ID: 0x00X11009:
//
typedef struct {
    uint32_t battery_current :16;
    uint32_t mode_switch :16;
	uint32_t direction :16;
	uint32_t hydraulic_throttle :16;
} skai2_inverter_tx_msg9_t;

//
// CAN ID: 0x00X110010:
//
typedef struct {
    uint32_t throttle_released :16;
    uint32_t throttle_depressed :16;
	uint32_t hydraulic_motor_scale_settings :16;
	uint32_t battery_current_integral_gain :16;
} skai2_inverter_tx_msg10_t;

//
// CAN ID: 0x00X110011:
//
typedef struct {
    uint32_t unused1 :16;
    uint32_t unused2 :16;
	uint32_t throttle_override :16;
	uint32_t throttle_scale :16;
} skai2_inverter_tx_msg11_t;

//
// CAN ID: 0x00X110012:
//
typedef struct {
    uint32_t idle_motor_rpm :16;
    uint32_t max_motor_rpm :16;
	uint32_t out_gain :16;
	uint32_t speed_p_gain_ramp_timer :16;
} skai2_inverter_tx_msg12_t;


//
// A compound structure that encompasses all the RX CAN messages
// related to the Murphy VisSim version of the Skai2. One of these is
// allocated for each instantiation of a SKAI2.
//
typedef struct device_data_s {
	struct device_data_s *next_ptr;
	uint8_t device_instance_number;

	// These are rx message data buffer
	skai2_inverter_rx_msg1_t rx_msg1_data;
	skai2_inverter_rx_msg2_t rx_msg2_data;
	skai2_inverter_rx_msg3_t rx_msg3_data;
	skai2_inverter_rx_msg4_t rx_msg4_data;
	skai2_inverter_rx_msg5_t rx_msg5_data;
	skai2_inverter_rx_msg9_t rx_msg9_data;

    // Each of the VisSim SKAI2 CAN receive functions sets bool
    // associated with itself to TRUE when its message is received.
	bool_t rx_msg1_data_rx_ok;
	bool_t rx_msg2_data_rx_ok;
	bool_t rx_msg3_data_rx_ok;
	bool_t rx_msg4_data_rx_ok;
	bool_t rx_msg5_data_rx_ok;
	bool_t rx_msg9_data_rx_ok;

	// These are tx message data buffers
	skai2_inverter_tx_msg1_t tx_msg1_data_d_gains;
	skai2_inverter_tx_msg2_t tx_msg2_data_q_gains;
	skai2_inverter_tx_msg3_t tx_msg3_data_speed_gains;
	skai2_inverter_tx_msg4_t tx_msg4_data_electrical_angle;
	skai2_inverter_tx_msg5_t tx_msg5_enable_max_current_soc_high_cell;
	skai2_inverter_tx_msg6_t tx_msg6_motor_scale;
	skai2_inverter_tx_msg7_t tx_msg7_regen_scale;
	skai2_inverter_tx_msg8_t tx_msg8_throttle_filter_dqf_gains;
	skai2_inverter_tx_msg9_t tx_msg9_battery_current_mode_direction_hyd_throttle;
	skai2_inverter_tx_msg10_t tx_msg10_pedal_calibration_motor_scale;
	skai2_inverter_tx_msg11_t tx_msg11_throttle_override;
	skai2_inverter_tx_msg12_t tx_msg12_rpm_outgain_ramptime;

	// Pointer to the tx registeration records
	can_tx_registration_t *send_inverter_d_gains_ptr;
	can_tx_registration_t *send_inverter_q_gains_ptr;
	can_tx_registration_t *send_inverter_spd_gains_ptr;
	can_tx_registration_t *send_inverter_electrical_angle_ptr;
	can_tx_registration_t *send_inverter_enable_maxCurrent_soc_highCell_ptr;
	can_tx_registration_t *send_inverter_motor_scale_ptr;
	can_tx_registration_t *send_inverter_regen_scale_ptr;
	can_tx_registration_t *send_inverter_throttle_filter_dq_df_gains_ptr;
	can_tx_registration_t *send_inverter_battery_current_mode_direction_hyd_throttle_ptr;
	can_tx_registration_t *send_inverter_pedal_calibration_battery_gain_ptr;
	can_tx_registration_t *send_inverter_throttle_override_ptr;
	can_tx_registration_t *send_inverter_rpm_outgain_ramptime_ptr;

    // Time Out booleans Used to identify if a receive can message has
    // been timed out.
    bool_t skai2_vissim_msg1_rx_ok;
    bool_t skai2_vissim_msg2_rx_ok;
    bool_t skai2_vissim_msg3_rx_ok;
    bool_t skai2_vissim_msg4_rx_ok;
    bool_t skai2_vissim_msg5_rx_ok;
    bool_t skai2_vissim_msg9_rx_ok;

	uint8_t inverter_power_pin;
} device_data_t;

#include "skai2_inverter_vissim.h"

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

#endif // SKAI2_INVERTER_PRIVATE_H_
