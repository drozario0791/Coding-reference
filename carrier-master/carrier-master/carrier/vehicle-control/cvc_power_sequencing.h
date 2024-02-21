/******************************************************************************
 *
 *        Name: cvc_power_sequencing.h
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Thursday, 22 August 2019
 *
 ******************************************************************************
 */

#ifndef CVC_POWER_SEQUENCING_H
#define CVC_POWER_SEQUENCING_H

#define KEYSWITCH_ON_TRHESHOLD_MV 4000
#define TEN_COUNTS_100MS 10

void cvc_power_sequencing_init_low_power();
bool_t cvc_power_sequencing_low_power_control();


#endif // CVC_POWER_SEQUENCING_H
