/******************************************************************************
 *
 *        Name: pdm_test_harness.h
 *
 * Description: Use the top 4 PKU dash switches to set groups of four
 *              PDM 1, 2, 3 outputs that may be toggled with the
 *              bottom four dash switches.
 *
 *      Author: Tom
 *        Date: Friday, 16 August 2019
 *
 ******************************************************************************
 */
#ifndef PDM_TEST_HARNESS_H
#define PDM_TEST_HARNESS_H

void switch_8(bool_t switch_state);
void switch_4(bool_t switch_state);
void switch_2(bool_t switch_state);
void switch_1(bool_t switch_state);

void harness_8(bool_t harness_state);
void harness_4(bool_t harness_state);
void harness_2(bool_t harness_state);
void harness_1(bool_t harness_state);

void harness_control();











#endif // pdm_test_harness.h
