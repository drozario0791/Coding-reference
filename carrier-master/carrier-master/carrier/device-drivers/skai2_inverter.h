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


#ifndef SKAI2_INVERTER_H_
#define SKAI2_INVERTER_H_

// Inverter Modes:
typedef enum
{
	NORMAL_OPERATION,
	REGEN_ONLY_MODE,
	THROTTLE_OVERRIDE_MODE,
	LIMP_MODE,
} skai2_inverter_modes_t;

/******************************************************************************
 *
 * Description: SKAI Initialisation Functions
 *
 ******************************************************************************
 */
void init_skai2_inverter(device_instances_t device, uint8_t module_id, CANLINE_ can_line, uint8_t inverter_power_pin);

/******************************************************************************
 *
 * Description: SKAI Transmit Messages
 *
 *
 ******************************************************************************
 */
void tx_skai2_control(device_instances_t device, can_rate_t transmit_counter_limit);
void tx_skai2_limits(device_instances_t device, can_rate_t transmit_counter_limit);

/******************************************************************************
 *
 * Description: Public Setter Function
 *
 *
 ******************************************************************************
 */
void skai2_set_12V_power(device_instances_t device, uint16_t state);

void skai2_set_inverter_control(
	device_instances_t device,
	uint8_t motor_switch,
	uint8_t direction,
	uint8_t diagnostic_mode,
	uint16_t commanded_rpm);

void skai2_set_inverter_limits(
	device_instances_t device,
	uint16_t max_battery_current,
	uint16_t instantaneous_battery_current,
	uint16_t high_cell_voltage,
	skai2_inverter_modes_t mode);

//=============================================================================
//
// SET RX TIME OUT COUNT
//
//=============================================================================
//
void skai2_set_timeout_status(device_instances_t device,
                              uint8_t module_id,
                              CANLINE_ can_line,
                              can_rate_t receive_timeout_counter_limit);

void skai2_set_timeout_temperature(device_instances_t device,
                                   uint8_t module_id,
                                   CANLINE_ can_line,
                                   can_rate_t receive_timeout_counter_limit);

void skai2_set_timeout_EA_RPM(device_instances_t device,
                              uint8_t module_id,
                              CANLINE_ can_line,
                              can_rate_t receive_timeout_counter_limit);

void skai2_set_timeout_torque(device_instances_t device,
                                  uint8_t module_id,
                                  CANLINE_ can_line,
                                  can_rate_t receive_timeout_counter_limit);

/******************************************************************************
 *
 * Description: SKAI Getter Functions
 *
 ******************************************************************************
 */

/******************************************************************************
 * Description: returns motor temperature in degree celcius.
 ******************************************************************************
 */
uint16_t skai_get_MotorTemp_C(device_instances_t device);

/******************************************************************************
 * Description: returns DC link voltage (Important for precharge operation)
 ******************************************************************************
 */
uint16_t skai_get_DCLink_Voltage(device_instances_t device);

/******************************************************************************
 * Description: returns skai phase and PCB Temperratures
 ******************************************************************************
 */
uint16_t skai_get_temperature_dcb_phase_L1(device_instances_t device);
uint16_t skai_get_temperature_dcb_phase_L2(device_instances_t device);
uint16_t skai_get_temperature_dcb_phase_L3(device_instances_t device);
uint16_t skai_get_temperature_pcb(device_instances_t device);

/******************************************************************************
 * Description: returns the set electrical angles
 ******************************************************************************
 */
uint16_t skai_get_electrical_offset_ccw(device_instances_t device);
uint16_t skai_get_electrical_offset_cw(device_instances_t device);

/******************************************************************************
 * Description: returns the rpm of the motor
 ******************************************************************************
 */
uint16_t skai_get_motor_rpm(device_instances_t device);

/******************************************************************************
 * Description: returns the maximum temperature of the three phases
 ******************************************************************************
 */
uint16_t skai_get_max_temperature(device_instances_t device);

/******************************************************************************
 * Description: returns the instantaneous torque output of the motor
 ******************************************************************************
 */
uint16_t skai_get_torque(device_instances_t device);

/******************************************************************************
 *
 * Description: Returns the status of the inverter. Look into the
 *              datasheet of the skai inverter for the abrivation of
 *              the words.
 *
 ******************************************************************************
 */
bool_t skai_get_inverter_error_status(device_instances_t device);

//=============================================================================
//
// Individual Getter Functions
//
//=============================================================================
//
bool_t skai_get_status_DSP_UVT_BOT1(device_instances_t device);
bool_t skai_get_status_DSP_UVT_BOT2(device_instances_t device);
bool_t skai_get_status_DSP_UVT_BOT3(device_instances_t device);
bool_t skai_get_status_DSP_UVT_TOP1(device_instances_t device);
bool_t skai_get_status_DSP_UVT_TOP2(device_instances_t device);
bool_t skai_get_status_DSP_UVT_TOP3(device_instances_t device);
bool_t skai_get_status_FLT_nINT(device_instances_t device);
bool_t skai_get_status_HVIL_DISCHAR(device_instances_t device);
bool_t skai_get_status_DSP_UVT_AP(device_instances_t device);
bool_t skai_get_status_DSP_UVT_IAP(device_instances_t device);
bool_t skai_get_status_DSP_VCE_BOT1(device_instances_t device);
bool_t skai_get_status_DSP_VCE_BOT2(device_instances_t device);
bool_t skai_get_status_DSP_VCE_BOT3(device_instances_t device);
bool_t skai_get_status_DSP_VCE_TOP1(device_instances_t device);
bool_t skai_get_status_DSP_VCE_TOP2(device_instances_t device);
bool_t skai_get_status_DSP_VCE_TOP3(device_instances_t device);
bool_t skai_get_status_DSP_OCT_L2P(device_instances_t device);
bool_t skai_get_status_DSP_OCT_L2N(device_instances_t device);
bool_t skai_get_status_DSP_OCT_L3P(device_instances_t device);
bool_t skai_get_status_DSP_OCT_L3N(device_instances_t device);
bool_t skai_get_status_motor_switch(device_instances_t device);
bool_t skai_get_status_DSP_nELX(device_instances_t device);
bool_t skai_get_status_DSP_OVT_DCL(device_instances_t device);
bool_t skai_get_status_OTT_PCB(device_instances_t device);
bool_t skai_get_status_OTT_DCB1(device_instances_t device);
bool_t skai_get_status_OTT_DCB2(device_instances_t device);
bool_t skai_get_status_OTT_DCB3(device_instances_t device);
bool_t skai_get_status_DSP_OCT_L1P(device_instances_t device);
bool_t skai_get_status_DSP_OCT_L1N(device_instances_t device);
bool_t skai2_get_can_rx_ok(device_instances_t device);

#endif // SKAI2_INVERTER_H_
