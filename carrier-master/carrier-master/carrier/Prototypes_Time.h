/*
** ###################################################################
**     Filename  : ProtoTypes_Time.h
**     Processor : Not applicable
**     FileFormat: V2.00
**     Abstract  :
**     This file contains functions used to get number of loops required to
**     measure a time period
**
**     (c) Copyright HED,Inc. 2016
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
#ifndef _PROTOTYPES_TIME_H_
#define _PROTOTYPES_TIME_H_

/* Returns number of loops (passes through User_App/ User_Safe_Mode) that equal the amount of time*/
/* You can calculate any time desired by using multiples/division of the time provided
 * or through the equation: Time Desired / LoopTime(set in Orchestra) = number of User_App loops
 */
uint16_t get_TEN_MSECS(void);
uint16_t get_TWENTY_MSECS(void);
uint16_t get_THIRTY_MSECS(void);
uint16_t get_FOURTY_MSECS(void);
uint16_t get_FIFTY_MSECS(void);
uint16_t get_ONE_HUNDRED_MSECS(void);
uint16_t get_ONE_HUNDRED_FIFTY_MSECS(void);
uint16_t get_TWO_HUNDRED_MSECS(void);
uint16_t get_TWO_HUNDRED_FIFTY_MSECS(void);
uint16_t get_THREE_HUNDRED_MSECS(void);
uint16_t get_THREE_HUNDRED_FIFTY_MSECS(void);
uint16_t get_FIVE_HUNDRED_MSECS(void);
uint16_t get_SEVEN_HUNDRED_MSECS(void);
uint16_t get_EIGHT_HUNDRED_MSECS(void);
uint16_t get_ONE_SECOND(void);
uint16_t get_ONE_AND_A_HALF_SECONDS(void);
uint16_t get_TWO_SECONDS(void);
uint16_t get_TWO_AND_A_HALF_SECONDS(void);
uint32_t get_THREE_SECONDS(void);
uint32_t get_THREE_AND_A_HALF_SECONDS(void);
uint32_t get_FOUR_SECONDS(void);
uint32_t get_FIVE_SECONDS(void);
uint32_t get_SEVEN_SECONDS(void);
uint32_t get_TEN_SECONDS(void);
uint32_t get_TWELVE_SECONDS(void);
uint32_t get_FIFTEEN_SECONDS(void);
uint32_t get_THIRTY_SECONDS(void);
uint32_t get_ONE_MINUTE(void);
uint32_t get_TEN_MINUTES(void);

uint32_t ConvertMsecToLoops(uint32_t msec);

#endif /* _PROTOTYPES_TIME_H_*/
