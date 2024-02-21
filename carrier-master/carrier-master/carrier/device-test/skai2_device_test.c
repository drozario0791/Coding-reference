/******************************************************************************
 *
 *        Name: skai2_device_test.c
 *
 * Description: Functions that would test the SKAI2 device driver.
 *              The following test functions are present in this module:
 *              1) TX timing
 *              2) RX CAN timeout
 *              3) Checking bit parity
 *              4) Getters and Setter functions
 *
 *      Author: Deepak
 *        Date: Monday, 08 July 2019
 *
 ******************************************************************************
 */

/*Include Header Files */
#include <stdio.h>
#include <math.h>
#include "reserved.h"
#include "Prototypes.h"
#include "Constants.h"
#include "User_App.h"
#include "typedefs.h"
#include "can_service.h"
#include "skai2_inverter.h"
#include "skai2_test_device.h"

//=============================================================================
//
// skai2_device_CAN_TX_timing:
//
//=============================================================================
//
void skai2_device_test_CAN_TX_timing(device_instances_t device)
{
	switch (device)
	{
	case ONE:
		// Testing the Timing of TX messages using the SKAI messages
		skai2_set_inverter_control(device, 1, 1, 1, 100);
		tx_skai2_control(device, EVERY_10MS);

		skai2_set_inverter_limits(device, 120, 300, 40000, NORMAL_OPERATION);
		tx_skai2_limits(device, EVERY_20MS);
		break;

	case TWO:
		// Testing the Timing of TX messages using the SKAI messages
		skai2_set_inverter_control(device, 1, 1, 1, 100);
		tx_skai2_control(device, EVERY_30MS);

		skai2_set_inverter_limits(device, 120, 300, 40000, NORMAL_OPERATION);
		tx_skai2_limits(device, EVERY_40MS);
		break;

	case THREE:
		// Testing the Timing of TX messages using the SKAI messages
		skai2_set_inverter_control(device, 1, 1, 1, 100);
		tx_skai2_control(device, EVERY_50MS);

		skai2_set_inverter_limits(device, 120, 300, 40000, NORMAL_OPERATION);
		tx_skai2_limits(device, EVERY_100MS);
		break;

	case FOUR:
		// Testing the Timing of TX messages using the SKAI messages
		skai2_set_inverter_control(device, 1, 1, 1, 100);
		tx_skai2_control(device, EVERY_500MS);

		skai2_set_inverter_limits(device, 120, 300, 40000, NORMAL_OPERATION);
		tx_skai2_limits(device, EVERY_1000MS);
		break;

	case FIVE:
		// Testing the Timing of TX messages using the SKAI messages
		skai2_set_inverter_control(device, 1, 1, 1, 100);
		tx_skai2_control(device, EVERY_2S);

		skai2_set_inverter_limits(device, 120, 300, 40000, NORMAL_OPERATION);
		tx_skai2_limits(device, EVERY_5S);
		break;

	case SIX:
		// Testing the Timing of TX messages using the SKAI messages
		skai2_set_inverter_control(device, 1, 1, 1, 100);
		tx_skai2_control(device, NO_TIME_OUT);

		skai2_set_inverter_limits(device, 120, 300, 40000, NORMAL_OPERATION);
		tx_skai2_limits(device, NO_TIME_OUT);
		break;
			}
}

//=============================================================================
//
// skai2_device_test_CAN_RX_timeout:
//
//=============================================================================
//
void skai2_device_test_CAN_RX_timeout(device_instances_t device)
{
	// The time out for the device is presently set to NONE
	// Configure the time outs for the devices you would like to test

	switch (device)
	{
	case ONE:
		skai2_set_timeout_status(device, 0, CAN3, EVERY_2S);
		skai2_set_timeout_temperature(device, 0, CAN3, EVERY_2S);
		skai2_set_timeout_EA_RPM(device, 0, CAN3, EVERY_2S);
		skai2_set_timeout_torque(device, 0, CAN3, EVERY_2S);
		break;

	case TWO:
		skai2_set_timeout_status(device, 0, CAN3, NO_TIME_OUT);
		skai2_set_timeout_temperature(device, 0, CAN3, NO_TIME_OUT);
		skai2_set_timeout_EA_RPM(device, 0, CAN3, NO_TIME_OUT);
		skai2_set_timeout_torque(device, 0, CAN3, NO_TIME_OUT);
		break;

	case THREE:
		skai2_set_timeout_status(device, 0, CAN3, EVERY_100MS);
		skai2_set_timeout_temperature(device, 0, CAN3, EVERY_500MS);
		skai2_set_timeout_EA_RPM(device, 0, CAN3, EVERY_1000MS);
		skai2_set_timeout_torque(device, 0, CAN3, EVERY_10S);
		break;

	case FOUR:
		skai2_set_timeout_status(device, 0, CAN3, EVERY_100MS);
		skai2_set_timeout_temperature(device, 0, CAN3, EVERY_500MS);
		skai2_set_timeout_EA_RPM(device, 0, CAN3, EVERY_1000MS);
		skai2_set_timeout_torque(device, 0, CAN3, EVERY_10S);
		break;

	case FIVE:
		skai2_set_timeout_status(device, 0, CAN3, EVERY_100MS);
		skai2_set_timeout_temperature(device, 0, CAN3, EVERY_500MS);
		skai2_set_timeout_EA_RPM(device, 0, CAN3, EVERY_1000MS);
		skai2_set_timeout_torque(device, 0, CAN3, EVERY_10S);
		break;

	case SIX:
		skai2_set_timeout_status(device, 0, CAN3, EVERY_100MS);
		skai2_set_timeout_temperature(device, 0, CAN3, EVERY_500MS);
		skai2_set_timeout_EA_RPM(device, 0, CAN3, EVERY_1000MS);
		skai2_set_timeout_torque(device, 0, CAN3, EVERY_10S);
		break;
	}
}


//=============================================================================
//
// skai2_device_test_getter_functions: A function used to check if all the
//                                     bits are received in the right order.
//
//
//=============================================================================
//
void skai2_device_test_getter_functions(device_instances_t device)
{
	volatile uint16_t phase1_temp, phase2_temp, phase3_temp, pcb_temp = 0;
	volatile uint16_t motor_rpm, motor_temp, motor_torque = 0;
	volatile uint16_t motor_elec_angle_ccw, motor_elec_angle_cw = 0;

	phase1_temp = skai_get_temperature_dcb_phase_L1(device);
	phase2_temp = skai_get_temperature_dcb_phase_L2(device);
	phase3_temp = skai_get_temperature_dcb_phase_L3(device);
	pcb_temp = skai_get_temperature_pcb(device);

	motor_rpm = skai_get_motor_rpm(device);
	motor_temp = skai_get_MotorTemp_C(device);
	motor_torque = skai_get_torque(device);

	motor_elec_angle_ccw = skai_get_electrical_offset_ccw(device);
	motor_elec_angle_cw = skai_get_electrical_offset_cw(device);

	// Pattern Set over PCAN
	// 101X00 -> word1 : 0x1234 :0001 0010 0011 1000

	// Error Feedback 1:

	// Hex_Digit = 0x1
    // Result = 0
	bool_t DSP_UVT_IAP = skai_get_status_DSP_UVT_IAP(device);
    // Result = 0
	bool_t DSP_UVT_AP = skai_get_status_DSP_UVT_AP(device);
    // Result = 0
	bool_t HVIL_DISCHARGE = skai_get_status_HVIL_DISCHAR(device);
    // Result = 1
	bool_t FLT_nINT = skai_get_status_FLT_nINT(device);

	// Hex_Digit = 0x2
    // Result = 0
	bool_t DSP_UVT_TOP3 = skai_get_status_DSP_UVT_TOP3(device);
    // Result = 0
	bool_t DSP_UVT_TOP2 = skai_get_status_DSP_UVT_TOP2(device);
    // Result = 1
	bool_t DSP_UVT_TOP1 = skai_get_status_DSP_UVT_TOP1(device);
    // Result = 0
	bool_t DSP_UVT_BOT3 = skai_get_status_DSP_UVT_BOT3(device);

	// Hex Digit: 0x3
    // Result = 0
	bool_t DSP_UVT_BOT2 = skai_get_status_DSP_UVT_BOT2(device);
    // Result = 0
	bool_t DSP_UVT_BOT1 = skai_get_status_DSP_UVT_BOT1(device);
    // Result = 1
	bool_t DSP_VCE_TOP3 = skai_get_status_DSP_VCE_TOP3(device);
    // Result = 1
	bool_t DSP_VCE_TOP2 = skai_get_status_DSP_VCE_TOP2(device);

	// Hex Digit: 0x4
	// Result = 0
	bool_t DSP_VCE_TOP1 = skai_get_status_DSP_VCE_TOP1(device);
	// Result = 1
	bool_t DSP_VCE_BOT3 = skai_get_status_DSP_VCE_BOT3(device);
	// Result = 0
	bool_t DSP_VCE_BOT2 = skai_get_status_DSP_VCE_BOT2(device);
	// Result = 0
	bool_t DSP_VCE_BOT1 = skai_get_status_DSP_VCE_BOT1(device);


	// 101X00 -> word2 : 0x1234 :0001 0010 0011 1000
	// Hex value: 0x2
	// Result = 0
	bool_t motor_switch = skai_get_status_motor_switch(device);

	// Hex value: 0x1
	// Result = 0
	bool_t DSP_OCT_L3N = skai_get_status_DSP_OCT_L3N(device);
	// Result = 0
	bool_t DSP_OCT_L3p = skai_get_status_DSP_OCT_L3P(device);
	// Result = 0
	bool_t DSP_OCT_L2N = skai_get_status_DSP_OCT_L2N(device);
	// Result = 1
	bool_t DSP_OCT_L2P = skai_get_status_DSP_OCT_L2P(device);

	// Hex value: 0x4
	// Result = 0
	bool_t DSP_OCT_L1N = skai_get_status_DSP_OCT_L1N(device);
	// Result = 1
	bool_t DSP_OCT_L1P = skai_get_status_DSP_OCT_L1P(device);
	// Result = 0
	bool_t OTT_DCB3 = skai_get_status_OTT_DCB3(device);
	// Result = 0
	bool_t OTT_DCB2 = skai_get_status_OTT_DCB2(device);

	// Hex value: 0x3
	// Result = 0
	bool_t OTT_DCB1 = skai_get_status_OTT_DCB1(device);
	// Result = 0
	bool_t OTT_PCB = skai_get_status_OTT_PCB(device);
	// Result = 1
	bool_t DSP_OVT_DCL = skai_get_status_DSP_OVT_DCL(device);
	// Result = 1
	bool_t DSP_nELX = skai_get_status_DSP_nELX(device);

}
