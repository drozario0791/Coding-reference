/******************************************************************************
 *
 *        Name: skai2_inverter_setters.c
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Monday, 09 July 2019
 *
 ******************************************************************************
 */

#include "Prototypes.h"
#include "skai2_inverter_private_vissim.h"

extern device_data_t *first_skai2_vissim_device_data_ptr;

void skai2_set_vissim_tx_msg1_data_d_gains(
	device_instances_t device,
	uint16_t d_proportional_gain,
	uint16_t d_integral_gain,
	uint16_t d_differential_gain,
	uint16_t d_time_constant)
{

	skai2_inverter_tx_msg1_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_msg1_data_d_gains);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}


	dest_ptr->d_proportional_gain = d_proportional_gain;
	dest_ptr->d_integral_gain = d_integral_gain;
	dest_ptr->d_differential_gain = d_differential_gain;
	dest_ptr->d_time_constant = d_time_constant;
}


void skai2_set_vissim_tx_msg2_data_q_gains(
	device_instances_t device,
	uint16_t q_proportional_gain,
	uint16_t q_integral_gain,
	uint16_t q_differential_gain,
	uint16_t q_time_constant)
{

	skai2_inverter_tx_msg2_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_msg2_data_q_gains);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}


	dest_ptr->q_proportional_gain = q_proportional_gain;
	dest_ptr->q_integral_gain = q_integral_gain;
	dest_ptr->q_differential_gain = q_differential_gain;
	dest_ptr->q_time_constant = q_time_constant;
}

void skai2_set_vissim_tx_msg3_data_speed_gains(
	device_instances_t device,
	uint16_t speed_proportional_gain,
	uint16_t speed_integral_gain,
	uint16_t speed_differential_gain,
	uint16_t speed_time_constant)
{

	skai2_inverter_tx_msg3_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_msg3_data_speed_gains);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}


	dest_ptr->speed_proportional_gain = speed_proportional_gain;
	dest_ptr->speed_integral_gain = speed_integral_gain;
	dest_ptr->speed_differential_gain = speed_differential_gain;
	dest_ptr->speed_time_constant = speed_time_constant;
}

void skai2_set_vissim_tx_msg4_data_electrical_angle(
	device_instances_t device,
	uint16_t electrical_angle_forward,
	uint16_t electrical_angle_reverse,
	uint16_t electrical_angle_forward_regen,
	uint16_t electrical_angle_reverse_regen)
{

	skai2_inverter_tx_msg4_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_msg4_data_electrical_angle);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}


	dest_ptr->electrical_angle_forward = electrical_angle_forward;
	dest_ptr->electrical_angle_reverse = electrical_angle_reverse;
	dest_ptr->electrical_angle_forward_regen = electrical_angle_forward_regen;
	dest_ptr->electrical_angle_reverse_regen  = electrical_angle_reverse_regen;
}

void skai2_set_vissim_tx_msg5_enable_max_current_soc_high_cell(
	device_instances_t device,
	uint16_t inverter_enable,
	uint16_t max_battery_current,
	uint16_t pack_state_of_charge,
	uint16_t high_cell_voltage)
{

	skai2_inverter_tx_msg5_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_msg5_enable_max_current_soc_high_cell);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}


	dest_ptr->inverter_enable = inverter_enable;
	dest_ptr->max_battery_current = max_battery_current;
	dest_ptr->pack_state_of_charge = pack_state_of_charge;
	dest_ptr->high_cell_voltage = high_cell_voltage;
}

void skai2_set_vissim_tx_msg6_motor_scale(
	device_instances_t device,
	uint16_t motor_scale_P1,
	uint16_t motor_scale_P2,
	uint16_t motor_scale_L1,
	uint16_t motor_scale_L2)
{

	skai2_inverter_tx_msg6_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_msg6_motor_scale);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}


	dest_ptr->motor_scale_P1 = motor_scale_P1;
	dest_ptr->motor_scale_P2 = motor_scale_P2;
	dest_ptr->motor_scale_L1 = motor_scale_L1;
	dest_ptr->motor_scale_L2 = motor_scale_L2;
}

void skai2_set_vissim_tx_msg7_regen_scale(
	device_instances_t device,
	uint16_t regen_scale_P1,
	uint16_t regen_scale_P2,
	uint16_t regen_scale_L1,
	uint16_t regen_scale_L2)
{

	skai2_inverter_tx_msg7_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_msg7_regen_scale);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}


	dest_ptr->regen_scale_P1 = regen_scale_P1;
	dest_ptr->regen_scale_P2 = regen_scale_P2;
	dest_ptr->regen_scale_L1 = regen_scale_L1;
	dest_ptr->regen_scale_L2 = regen_scale_L2;
}

void skai2_set_vissim_tx_msg8_throttle_filter_dqf_gains(
	device_instances_t device,
	uint16_t throttle_gain_1,
	uint16_t throttle_gain_2,
	uint16_t df_gain,
	uint16_t qf_gain)
{

	skai2_inverter_tx_msg8_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_msg8_throttle_filter_dqf_gains);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}


	dest_ptr->throttle_gain_1 = throttle_gain_1;
	dest_ptr->throttle_gain_2 = throttle_gain_2;
	dest_ptr->df_gain = df_gain;
	dest_ptr->qf_gain = qf_gain;
}

void skai2_set_vissim_tx_msg9_battery_current_mode_direction_hyd_throttle(
	device_instances_t device,
	uint16_t battery_current,
	uint16_t mode_switch,
	uint16_t direction,
	uint16_t hydraulic_throttle)
{

	skai2_inverter_tx_msg9_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_msg9_battery_current_mode_direction_hyd_throttle);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}


	dest_ptr->battery_current = battery_current;
	dest_ptr->mode_switch = mode_switch;
	dest_ptr->direction = direction;
	dest_ptr->hydraulic_throttle = hydraulic_throttle;
}


void skai2_set_vissim_tx_msg10_pedal_calibration_motor_scale(
	device_instances_t device,
	uint16_t throttle_released,
	uint16_t throttle_depressed,
	uint16_t hydraulic_motor_scale_settings,
	uint16_t battery_current_integral_gain)
{

	skai2_inverter_tx_msg10_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_msg10_pedal_calibration_motor_scale);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}


	dest_ptr->throttle_released = throttle_released;
	dest_ptr->throttle_depressed = throttle_depressed;
	dest_ptr->hydraulic_motor_scale_settings = hydraulic_motor_scale_settings;
	dest_ptr->battery_current_integral_gain = battery_current_integral_gain;
}

void skai2_set_vissim_tx_msg11_throttle_override(
	device_instances_t device,
	uint16_t throttle_override,
	uint16_t throttle_scale)
{

	skai2_inverter_tx_msg11_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_msg11_throttle_override);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}


	dest_ptr->unused1 = 0;
	dest_ptr->unused2 = 0;
	dest_ptr->throttle_override  = throttle_override ;
	dest_ptr->throttle_scale = throttle_scale;
}

void skai2_set_vissim_tx_msg12_rpm_outgain_ramptime(
	device_instances_t device,
	uint16_t idle_motor_rpm,
	uint16_t max_motor_rpm,
	uint16_t out_gain,
	uint16_t speed_p_gain_ramp_timer)
{

	skai2_inverter_tx_msg12_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_msg12_rpm_outgain_ramptime);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}


	dest_ptr->idle_motor_rpm = idle_motor_rpm;
	dest_ptr->max_motor_rpm = max_motor_rpm;
	dest_ptr->out_gain = out_gain;
	dest_ptr->speed_p_gain_ramp_timer = speed_p_gain_ramp_timer;
}
