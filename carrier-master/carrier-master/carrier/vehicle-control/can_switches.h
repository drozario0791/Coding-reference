/******************************************************************************
 *
 *        Name: can_switches.h
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Friday, 27 September 2019
 *
 ******************************************************************************
 */

#ifndef CAN_SWITCHES_H_
#define CAN_SWITCHES_H_

void top_switch_1(bool_t switch_state);
void top_switch_2(bool_t switch_state);
void top_switch_3(bool_t switch_state);
void top_switch_4(bool_t switch_state);

void bottom_switch_1(bool_t switch_state);
void bottom_switch_2(bool_t switch_state);
void bottom_switch_3(bool_t switch_state);
void bottom_switch_4(bool_t switch_state);

bool_t ignition();
bool_t enable_charger_can_switch();
bool_t sprayer_solenoids_status();
bool_t brake_test_mode_status();


#endif
