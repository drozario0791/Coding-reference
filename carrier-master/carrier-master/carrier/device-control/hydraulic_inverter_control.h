/******************************************************************************
 *
 *        Name: hydraulic_inverter_control.h
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Thursday, 29 August 2019
 *
 ******************************************************************************
 */

#ifndef HYDRAULIC_INVERTER_CONTROL_H_
#define HYDRAULIC_INVERTER_CONTROL_H_

//=============================================================================
//
// hydraulic_inverter_control()
//
// A function that is responsible to enable/disable the hydraulic
// inverter based on various conditions.
//
//=============================================================================
//
void hydraulic_inverter_control(
    device_instances_t device,
    uint16_t inverter_enable);

bool_t hydraulic_inverter_state();



#endif
