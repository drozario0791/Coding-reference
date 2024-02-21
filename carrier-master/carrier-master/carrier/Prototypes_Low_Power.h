/*
** ###################################################################
**     Filename  : Prototypes_Low_Power.h
**     Processor : Not applicable
**     FileFormat: V2.00
**     Abstract  :
**     This file contains functions used to support Low Power Mode
**
**     (c) Copyright HED,Inc. 2015 - 2016
**     HED,Inc.
**     2120 Constitution Ave.
**     Hartford, WI 53027
**     United States
**     http      : www.hedonline.com
**     mail      : support@hedonline.com
** #########################################################################
**                            !!!  WARNING !!!
**     ALTERING THIS FILE IN ANY WAY MAY CAUSE PERSONAL INJURY AND/OR DEATH.
** #########################################################################
*/

#ifndef PROTOTYPES_LOW_POWER_H_
#define PROTOTYPES_LOW_POWER_H_

/*defines used to tell the wakeup module which devices allowed to wake up the chip from low power mode*/
/* OR these together to enable multiple wakeup modes*/
#define WAKEUP_CAN1     (wakeup_modes_t)0x00000001      /*Wake up when activity received on CAN1*/
#define WAKEUP_CAN2     (wakeup_modes_t)0x00000002      /*Wake up when activity received on CAN2*/
#define WAKEUP_CAN3     (wakeup_modes_t)0x00000004      /*Wake up when activity received on CAN3*/
#define WAKEUP_CAN4     (wakeup_modes_t)0x00000008      /*Wake up when activity received on CAN4*/
#define WAKEUP_PIN      (wakeup_modes_t)0x00000010      /*Wake up when the external wakeup input is active*/
/* Reserved             (wakeup_modes_t)0x00000020*/
#define WAKEUP_RTC      (wakeup_modes_t)0x00000040      /*Wake up when the RTC alarm goes off*/
#define WAKEUP_TSC      (wakeup_modes_t)0x00000080      /*Wake up when touch screen is pressed*/
#define WAKEUP_BUTTON1  (wakeup_modes_t)0x00000100      /*Wake up when button 1 is pressed*/
#define WAKEUP_BUTTON2  (wakeup_modes_t)0x00000200      /*Wake up when button 2 is pressed*/
#define WAKEUP_BUTTON3  (wakeup_modes_t)0x00000400      /*Wake up when button 3 is pressed*/
#define WAKEUP_BUTTON4  (wakeup_modes_t)0x00000800      /*Wake up when button 4 is pressed*/

typedef uint32_t wakeup_modes_t;

/* Call this function to set the desired wake-up mode(s).
   These can be OR'd together for multiple wake-up modes
   (e.g. Set_Wakeup_Modes((wakeup_modes_t)(WAKEUP_PIN|WAKEUP_CAN1|WAKEUP_BUTTON1) */
void Set_Wakeup_Modes(wakeup_modes_t WakeupModes);
#endif /* PROTOTYPES_LOW_POWER_H_ */
