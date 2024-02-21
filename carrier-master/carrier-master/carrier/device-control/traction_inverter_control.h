/******************************************************************************
 *
 *        Name: traction_inverter_control.h
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Thursday, 05 September 2019
 *
 ******************************************************************************
 */

#ifndef TRACTION_INVERTER_CONTROL_H_
#define TRACTION_INVERTER_CONTROL_H_



/******************************************************************************
 *
 *        Name: traction_inverter_control()
 *
 * Description: The function gathers data from the state machine,
 *              orion BMS and the shifter. Based on the information
 *              obtained, the control function controls the working of
 *              the skai2 traction drive inverter.
 *
 *
 ******************************************************************************
 */
void traction_inverter_control(device_instances_t device);
bool_t traction_inverter_state();


#endif // TRACTION_INVERTER_CONTROL_H_
