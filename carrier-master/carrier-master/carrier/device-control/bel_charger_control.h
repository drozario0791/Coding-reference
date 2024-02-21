/******************************************************************************
 *
 *        Name: bel_charger_control.h
 *
 * Description: Controls the working of the Bel Charger based on
 *              various input conditions.
 *
 *      Author: Deepak
 *        Date: Friday, 06 September 2019
 *
 ******************************************************************************
 */

#ifndef BEL_CHARGER_CONTROL_H_
#define BEL_CHARGER_CONTROL_H_


/******************************************************************************
 *
 *        Name: bel_charger_control()
 *
 * Description: A function that enables/disables the bel charger based
 *              on various conditions it receives from the state
 *              machine and BMS.
 *
 *        Date: Friday, 06 September 2019
 *
 ******************************************************************************
 */
void bel_charger_control(
    device_instances_t device);

bool_t charger_enabled_state();
bool_t get_charger_button_enabled_state();
bool_t detect_charge_handle();

#endif // BEL_CHARGER_CONTROL_H_
