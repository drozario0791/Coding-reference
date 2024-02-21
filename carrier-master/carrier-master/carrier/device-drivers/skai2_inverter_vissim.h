/******************************************************************************
 *
 *        Name: skai2_inverter.h
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Monday, 08 July 2019
 *
 ******************************************************************************
 */

#ifndef SKAI2_INVERTER_VISSIM_H_
#define SKAI2_INVERTER_VISSIM_H_

// Inverter Modes:
typedef enum
{
	NORMAL_OPERATION_VM,
	REGEN_ONLY_MODE_VM,
	THROTTLE_OVERRIDE_MODE_VM,
	LIMP_MODE_VM,
} skai2_inverter_vissim_modes_t;

///////////////////////////////////////////////////////////////////////////////
//
//  Initialize the skai2 vissim device
//
//////////////////////////////////////////////////////////////////////////////
void init_skai2_inverter_vissim(device_instances_t device,
                                uint8_t module_id,
                                CANLINE_ can_line,
                                uint8_t inverter_power_pin);

///////////////////////////////////////////////////////////////////////////////
//
//  SKAI Transmit Messages
//
//////////////////////////////////////////////////////////////////////////////

/******************************************************************************
 * Description: Transmits the gains required for the direct axis
 *              controller.
 *
 ******************************************************************************
 */
void tx_skai2_vissim_d_gains(device_instances_t device,
							 can_rate_t transmit_counter_limit);

/******************************************************************************
 * Description: Transmits the gains required for the quadrature axis
 *              PID controller.
 *
 ******************************************************************************
 */
void tx_skai2_vissim_q_gains(device_instances_t device,
							 can_rate_t transmit_counter_limit);

/******************************************************************************
 * Description: Transmits the gains required for the speed PID
 *              controller.
 *
 ******************************************************************************
 */
void tx_skai2_vissim_speed_gains(device_instances_t device,
								 can_rate_t transmit_counter_limit);

/******************************************************************************
 * Description: Transmits the electrical angle for the motor.
 *
 ******************************************************************************
 */
void tx_skai2_vissim_electrical_angle(device_instances_t device,
									  can_rate_t transmit_counter_limit);

/******************************************************************************
 * Description: Transmits the following parameters
 *              1) Inverter Enable
 *              2) Max Current limit
 *              3) State of Charge(SOC)
 *              4) High Cell Voltage
 *
 ******************************************************************************
 */
void tx_skai2_enable_maxcurrent_soc_highcell(device_instances_t device,
											 can_rate_t transmit_counter_limit);

/******************************************************************************
 * Description: Transmits the parameters that determine the
 *              motorscale. The motor scale limits the motor torque in
 *              motoring operation.
 ******************************************************************************
 */
void tx_skai2_vissim_motor_scale(device_instances_t device,
								 can_rate_t transmit_counter_limit);

/******************************************************************************
 * Description: Transmits the parameters that determine the
 *              regenscale. The regen scale limits the motor torque in
 *              regenration operation.
 *
 ******************************************************************************
 */
void tx_skai2_vissim_regen_scale(device_instances_t device,
								 can_rate_t transmit_counter_limit);

/******************************************************************************
 * Description: The message transmits the following parameters
 *              1) Throttle Filter gain 1
 *              2) Throttle Filter gain2
 *              3) DF gain
 *              4) QF gain
 *
 ******************************************************************************
 */
void tx_skai2_vissim_throttle_filter_dq_df_gains(device_instances_t device,
												 can_rate_t transmit_counter_limit);

/******************************************************************************
 * Description: The message transmits the following parameters
 *              1) Instantaneous Battey Current
 *              2) Mode Switch
 *              3) Motor direction
 *              4) Hydraulic Throttle
 *
 ******************************************************************************
 */
void tx_skai2_vissim_battery_current_mode_direction_hyd_throttle(device_instances_t device,
																 can_rate_t transmit_counter_limit);

/******************************************************************************
 * Description: The message transmits the following parameters
 *              1) Throttle depressed
 *              2) Throttle released
 *              3) Hydraulic motor scale settings
 *              4) Battery current integral gain
 *
 ******************************************************************************
 */
void tx_skai2_vissim_pedal_calibration_battery_gain(device_instances_t device,
													can_rate_t transmit_counter_limit);

/******************************************************************************
 * Description: The message transmits the following parameters
 *              1) Unused1
 *              2) Unused2
 *              3) Throttle override
 *              4) Throttle scale
 *
 ******************************************************************************
 */
void tx_skai2_vissim_throttle_override(device_instances_t device,
									   can_rate_t transmit_counter_limit);

/******************************************************************************
 * Description: The message transmits the following parameters
 *              1) Idle Motor rpm
 *              2) Max motor rpm
 *              3) Out gain
 *              4) Speed proportional ramp timer
 *
 ******************************************************************************
 */
void tx_skai2_vissim_rpm_settings_outgain_pgainramptime(device_instances_t device,
														can_rate_t transmit_counter_limit);

///////////////////////////////////////////////////////////////////////////////
//
//  Public Setter Functions
//
//////////////////////////////////////////////////////////////////////////////
void skai2_set_vissim_tx_msg1_data_d_gains(
	device_instances_t device,
	uint16_t d_proportional_gain,
	uint16_t d_integral_gain,
	uint16_t d_differential_gain,
	uint16_t d_time_constant);

void skai2_set_vissim_tx_msg2_data_q_gains(
	device_instances_t device,
	uint16_t q_proportional_gain,
	uint16_t q_integral_gain,
	uint16_t q_differential_gain,
	uint16_t q_time_constant);

void skai2_set_vissim_tx_msg3_data_speed_gains(
	device_instances_t device,
	uint16_t speed_proportional_gain,
	uint16_t speed_integral_gain,
	uint16_t speed_differential_gain,
	uint16_t speed_time_constant);

void skai2_set_vissim_tx_msg4_data_electrical_angle(
	device_instances_t device,
	uint16_t electrical_angle_forward,
	uint16_t electrical_angle_reverse,
	uint16_t electrical_angle_forward_regen,
	uint16_t electrical_angle_reverse_regen);

void skai2_set_vissim_tx_msg5_enable_max_current_soc_high_cell(
	device_instances_t device,
	uint16_t inverter_enable,
	uint16_t max_battery_current,
	uint16_t pack_state_of_charge,
	uint16_t high_cell_voltage);

void skai2_set_vissim_tx_msg6_motor_scale(
	device_instances_t device,
	uint16_t motor_scale_P1,
	uint16_t motor_scale_P2,
	uint16_t motor_scale_L1,
	uint16_t motor_scale_L2);

void skai2_set_vissim_tx_msg7_regen_scale(
	device_instances_t device,
	uint16_t regen_scale_P1,
	uint16_t regen_scale_P2,
	uint16_t regen_scale_L1,
	uint16_t regen_scale_L2);

void skai2_set_vissim_tx_msg8_throttle_filter_dqf_gains(
	device_instances_t device,
	uint16_t throttle_gain_1,
	uint16_t throttle_gain_2,
	uint16_t df_gain,
	uint16_t qf_gain);

void skai2_set_vissim_tx_msg9_battery_current_mode_direction_hyd_throttle(
	device_instances_t device,
	uint16_t battery_current,
	uint16_t mode_switch,
	uint16_t direction,
	uint16_t hydraulic_throttle);

void skai2_set_vissim_tx_msg10_pedal_calibration_motor_scale(
	device_instances_t device,
	uint16_t throttle_released,
	uint16_t throttle_depressed,
	uint16_t hydraulic_motor_scale_settings,
	uint16_t battery_current_integral_gain);

void skai2_set_vissim_tx_msg11_throttle_override(
	device_instances_t device,
	uint16_t throttle_override,
	uint16_t throttle_scale);

void skai2_set_vissim_tx_msg12_rpm_outgain_ramptime(
	device_instances_t device,
	uint16_t idle_motor_rpm,
	uint16_t max_motor_rpm,
	uint16_t out_gain,
	uint16_t speed_p_gain_ramp_timer);

///////////////////////////////////////////////////////////////////////////////
//
//  Set RX Time Out
//
//////////////////////////////////////////////////////////////////////////////
void skai2_vissim_set_timeout_skai2_rxid_msg1(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);

void skai2_vissim_set_timeout_skai2_rxid_msg2(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);

void skai2_vissim_set_timeout_skai2_rxid_msg3(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);

void skai2_vissim_set_timeout_skai2_rxid_msg4(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);

void skai2_vissim_set_timeout_skai2_rxid_msg5(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);

void skai2_vissim_set_timeout_skai2_rxid_msg9(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);

///////////////////////////////////////////////////////////////////////////////
//
//  SKAI Getter Functions
//
//////////////////////////////////////////////////////////////////////////////

/******************************************************************************
 * Description: Returns the error status of the skai2 inverter
 ******************************************************************************
 */
bool_t skai_get_vissim_error_status(device_instances_t device);

/******************************************************************************
 * Description: Returns the temperature of the motor in degree celcius
 ******************************************************************************
 */
uint16_t skai_get_vissim_motor_temp_C(device_instances_t device);

/******************************************************************************
 * Description: DC link voltage of the inverter. A very important
 *              parameter that decides the precharge operation of the
 *              vehicle.
 ******************************************************************************
 */
uint16_t skai_get_vissim_DCLink_Voltage(device_instances_t device);

/******************************************************************************
 * Description: Displays the voltage at the aux input pin
 ******************************************************************************
 */
uint16_t skai_get_vissim_auxilary_input_voltage(device_instances_t device);

/******************************************************************************
 * Description: Version Number
 ******************************************************************************
 */
uint16_t skai_get_vissim_version_number(device_instances_t device);

/******************************************************************************
 * Description: Displays the throttle (a value between 0-100) after
 *              the throttle conditioning block in vissim.
 ******************************************************************************
 */
uint16_t skai_get_vissim_throttle(device_instances_t device);

/******************************************************************************
 * Description: Displays the torque output of the motor.
 ******************************************************************************
 */
uint16_t skai_get_vissim_torque(device_instances_t device);

/******************************************************************************
 * Description: The current rpm of the motor.
 ******************************************************************************
 */
uint16_t skai_get_vissim_motor_rpm(device_instances_t device);

/******************************************************************************
 * Description: Displays the encoder count.
 ******************************************************************************
 */
uint16_t skai_get_vissim_encoder_feedback(device_instances_t device);

/******************************************************************************
 * Description: Displays the raw ADC value of the pedal
 ******************************************************************************
 */
uint16_t skai_get_vissim_throttle_pedal_adc(device_instances_t device);

/******************************************************************************
 * Description: IO Status of the inverter
 ******************************************************************************
 */
uint16_t skai_get_vissim_IO_status(device_instances_t device);

/******************************************************************************
 * Description: The current electrical angle.
 ******************************************************************************
 */
uint16_t skai_get_vissim_electrical_angle(device_instances_t device);

/******************************************************************************
 * Description: Temperature of Phase 1
 ******************************************************************************
 */
uint16_t skai_get_vissim_dcb_phase1_temperature(device_instances_t device);

/******************************************************************************
 * Description: Temperature of Phase 2
 ******************************************************************************
 */
uint16_t skai_get_vissim_dcb_phase2_temperature(device_instances_t device);

/******************************************************************************
 * Description: Temperature of Phase 3
 ******************************************************************************
 */
uint16_t skai_get_vissim_dcb_phase3_temperature(device_instances_t device);

/******************************************************************************
 * Description: Temperature of PCB
 ******************************************************************************
 */
uint16_t skai_get_vissim_pcb_temperature(device_instances_t device);

/******************************************************************************
 * Description:
 ******************************************************************************
 */
uint16_t skai_get_vissim_power_consumned(device_instances_t device);

/******************************************************************************
 * Description: Max temperature of the inverter
 ******************************************************************************
 */
uint16_t skai_get_vissim_max_temperature(device_instances_t device);

/******************************************************************************
 * Description: An indication to know the state of operation of the
 *              motor.
 ******************************************************************************
 */
uint16_t skai_get_vissim_inverter_enable(device_instances_t device);

/******************************************************************************
 *
 * Description: Returns the status of the inverter. Look into the
 *              datasheet of the skai inverter for the abrivation of
 *              the words.
 *
 ******************************************************************************
 */
bool_t skai_get_vissim_rx_msg1_data_DSP_UVT_BOT1(device_instances_t device);
bool_t skai_get_vissim_rx_msg1_data_DSP_UVT_BOT2(device_instances_t device);
bool_t skai_get_vissim_rx_msg1_data_DSP_UVT_BOT3(device_instances_t device);
bool_t skai_get_vissim_rx_msg1_data_DSP_UVT_TOP1(device_instances_t device);
bool_t skai_get_vissim_rx_msg1_data_DSP_UVT_TOP2(device_instances_t device);
bool_t skai_get_vissim_rx_msg1_data_DSP_UVT_TOP3(device_instances_t device);
bool_t skai_get_vissim_rx_msg1_data_FLT_nINT(device_instances_t device);
bool_t skai_get_vissim_rx_msg1_data_HVIL_DISCHAR(device_instances_t device);
bool_t skai_get_vissim_rx_msg1_data_DSP_UVT_AP(device_instances_t device);
bool_t skai_get_vissim_rx_msg1_data_DSP_UVT_IAP(device_instances_t device);
bool_t skai_get_vissim_rx_msg1_data_DSP_VCE_BOT1(device_instances_t device);
bool_t skai_get_vissim_rx_msg1_data_DSP_VCE_BOT2(device_instances_t device);
bool_t skai_get_vissim_rx_msg1_data_DSP_VCE_BOT3(device_instances_t device);
bool_t skai_get_vissim_rx_msg1_data_DSP_VCE_TOP1(device_instances_t device);
bool_t skai_get_vissim_rx_msg1_data_DSP_VCE_TOP2(device_instances_t device);
bool_t skai_get_vissim_rx_msg1_data_DSP_VCE_TOP3(device_instances_t device);
bool_t skai_get_vissim_rx_msg1_data_DSP_OCT_L2P(device_instances_t device);
bool_t skai_get_vissim_rx_msg1_data_DSP_OCT_L2N(device_instances_t device);
bool_t skai_get_vissim_rx_msg1_data_DSP_OCT_L3P(device_instances_t device);
bool_t skai_get_vissim_rx_msg1_data_DSP_OCT_L3N(device_instances_t device);
bool_t skai_get_vissim_rx_msg1_data_motor_switch(device_instances_t device);
bool_t skai_get_vissim_rx_msg1_data_DSP_nELX(device_instances_t device);
bool_t skai_get_vissim_rx_msg1_data_DSP_OVT_DCL(device_instances_t device);
bool_t skai_get_vissim_rx_msg1_data_OTT_PCB(device_instances_t device);
bool_t skai_get_vissim_rx_msg1_data_OTT_DCB1(device_instances_t device);
bool_t skai_get_vissim_rx_msg1_data_OTT_DCB2(device_instances_t device);
bool_t skai_get_vissim_rx_msg1_data_OTT_DCB3(device_instances_t device);
bool_t skai_get_vissim_rx_msg1_data_DSP_OCT_L1P(device_instances_t device);
bool_t skai_get_vissim_rx_msg1_data_DSP_OCT_L1N(device_instances_t device);
bool_t skai_get_vissim_inverter_can_rx_ok(device_instances_t device);

#endif // SKAI2_INVERTER_H_
