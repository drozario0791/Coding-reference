/*
** ###################################################################
**     Filename  : User_App.h
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
#ifndef _USER_APP_H_
#define _USER_APP_H_
/* 
 * Sample structure shown
 */
typedef struct
{
   uint16_t  temp;
} App_Structure_t;


USER_RAM /* Processor specific # defines to control RAM page the variables stored in -if blank in reserved.h you may exclude*/
   extern App_Structure_t As;
DEFAULT_RAM/* Processor specific # defines to control RAM page the variables stored in -if blank in reserved.h you may exclude*/

/*
 * Function Prototypes
 */
void User_App(void);

#endif /* _USER_APP_H_ */
