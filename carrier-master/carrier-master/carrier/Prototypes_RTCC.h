/*
** ###################################################################
**     Filename  : Prototypes_RTCC.h
**     Processor : Not applicable
**     FileFormat: V2.00
**     Abstract  :
**         This file contains all the function and structure prototypes 
       for supporting a RTCC Real-time clock calendar - if supported by module hardware
**
**     (c) Copyright HED,Inc. 2006-2016
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

#ifndef PROTOTYPES_RTCC_H_
#define PROTOTYPES_RTCC_H_

#include <time.h>
#include "typedefs.h"

#define RTCC_YEAR_MIN         0   /*equivalent to 1900*/
#define RTCC_YEAR_MAX         255
#define RTCC_MONTH_MIN        0   /*January*/
#define RTCC_MONTH_MAX        11  /*December*/
#define RTCC_DAY_MIN          1   /*day of month is one-based*/
#define RTCC_DAY_MAX          31
#define RTCC_DAY_OF_WEEK_MIN  0   /*Sunday*/
#define RTCC_DAY_OF_WEEK_MAX  6   /*Saturday*/
#define RTCC_HOUR_MIN         0   /*12:00am*/
#define RTCC_HOUR_MAX         23  /*11:00pm, 23:00*/
#define RTCC_MINUTE_MIN       0   
#define RTCC_MINUTE_MAX       59
#define RTCC_SECOND_MIN       0
#define RTCC_SECOND_MAX       60  /* C99 allows for a "leap second"
                                   * If leap second is not supported by the specific RTC, it will be automatically reduced to max value*/

/* the tm structure used in this interface is defined in time.h
 * see compiler documentation for the definition of the structure
 */


/* Gets current time as stored by the Real Time Clock Calendar
 * Return: tm struct in standard ANSI/C99 format  
 */
struct tm Get_DateTime(void);

/* 
 * This function sets current time as stored by the Real Time Clock Calendar 
 * Variant to use standard Time.c structures
 * Parameters:
 * timeDate    time structure filled with values to set RTCC to.
 * Return: TRUE - time set successfully
 *         FALSE - error when setting time-- usually a parameter out of range.
 */
bool_t Set_DateTime (struct tm timeDate);

/* This function gets the current year in ANSI/C99 standard format (years since 1900)*/
int GetRTCCYear(void);

/* This function gets the current year in full, human-readable format (ex: 2010)*/
int GetRTCCFullYear(void);

/* This function gets the current month 0 = January ... 11 = December*/
int GetRTCCMonth(void); 

/* This function gets the current day of the month  1 - 31*/
int GetRTCCDay(void);   

/* This function gets the current day of the week  0 = Sunday 6 = Saturday
 * DOW = Day Of Week
 */
int GetRTCCDOW(void);   

/* This function gets the current hour 0-23*/
int GetRTCCHour(void);   

/* This function gets the current minute 0-59*/
int GetRTCCMinute(void); 

/* This function gets the current second 0-59*/
int GetRTCCSecond(void); 

/* Set the alarm on the Real Time Clock chip with all applicable fields from tm struct
 * return: TRUE - alarm was set successfully
 *         FALSE - error when setting alarm
 * NOTE: not every module clock supports alarms on a specific date/month/year. See module datasheet for more information.
 */
bool_t Set_RTCCAlarm(struct tm alarm_date_time); 

/* Clear the alarm on the Real Time Clock chip 
 * No affect if clearing the alarm when it is not set
 */
void Clear_RTCCAlarm(void);

/* Call this function to see if an alarm is currently active 
 * return: TRUE - alarm is currently active
 *         FALSE - alarm is not active
 * Note: An active alarm will also trigger User_Alarm to be called
 */
bool_t IsRTCCAlarmActive();

/* 
 * This function gets the recorded time when the module shutdown(lost power) last
 * Return: tm struct in standard ANSI/C99 format
 * NOTE: not every module clock supports controlled shutdown. See module datasheet for more information.
 */
struct tm Get_PowerDownDateTime (void);


#endif /* PROTOTYPES_RTCC_H_ */
