/******************************************************************************
 *
 *        Name: transmission_inverter_control.h
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Friday, 30 August 2019
 *
 ******************************************************************************
 */

#ifndef TRANSMISSION_INVERTER_CONTROL_H_
#define TRANSMISSION_INVERTER_CONTROL_H_

//=============================================================================
//
//  Controls the transmission inverter (SEVCON HVLP10) on the carrier.
//
//=============================================================================
//
void transmission_inverter_control(
    device_instances_t device,
    sevcon_hvlp10_state_t transmission_inverter_enable);


#endif
