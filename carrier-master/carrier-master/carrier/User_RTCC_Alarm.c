/*
** ###################################################################
**     Filename  : User_RTCC_Alarm.c
**     Processor : Not applicable
**     FileFormat: V2.00
**     Abstract  :
**
**     (c) Copyright HED,Inc. 2006-2016
**     HED,Inc.
**     2120 Constitution Ave.
**     Hartford, WI 53027
**     United States
**     http      : www.hedonline.com
**     mail      : support@hedonline.com
** ###################################################################
*/
/*Include Header Files */
#include "reserved.h"
#include "Prototypes.h"
#include "Constants.h"
#include "User_App.h"
#ifdef PROTOTYPES_RTCC_HEADER_FILE
#include "User_RTCC_Alarm.h"

/*
 * This function gets called once a loop when the RTCC alarm has been triggered until it has been cleared by the application
 * The alarm will only trigger when it has been configured by the user
 * The alarm will last through power cycles
 * If the alarm has been configured as a wake up source, it will wake up when the alarm occurs and if the application
 * attemps to go into low power mode with the alarm active it will cause the module to wake up immediately
 */
void User_RTCC_Alarm(void)
{
#warning "Insert your code to manage the RTCC alarm and clear it here the code below is already setup to clear it but you can customize it, remove this warning after you have set it up"
	/* call to clear the alarm */
	Clear_RTCCAlarm();
}
#endif
