/*
** ###################################################################
**     Filename  : Prototypes_BAM.h
**     Processor : Not applicable
**     FileFormat: V2.00
**     Abstract  :
**     This file contains all the function and structure prototypes 
**     that are required for BAM messaging
**
**     NOTE** not all modules support BAM messaging.  Look in reserved.h to verify
**           how many are supported
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
#ifndef PROTOTYPES_DM1_DM2_BAM_H_
#define PROTOTYPES_DM1_DM2_BAM_H_

#include "typedefs.h"

/**************************************/
/*       BAM Messaging Structures   */
/**************************************/
/* Currently Composer does not support BAM messages, so in order to use them
 * the user needs to setup the properties in c code 
 */
typedef struct
{
   uint16_t     ui16_pgn;              /* the error's pgn number */
   uint8_t      ui8_source_address;    /* source address of the error */
   uint8_t      ui8_module_id;         /* the module the message is received on, see constant.h for enum */
   uint8_t      ui8_canline;           /* the can line on the module the message is received on, see Prototype_Can for enum */
} BAM_Properties_;

/* SPN item states */
typedef enum 
{
   SPN_INACTIVE,
   SPN_ACTIVE
}SPN_STATE;


/* each received spn has the following properties.  Each of these values is parsed from the incoming can message(s) */
typedef struct
{
   uint32_t     ui32_spn;              /* the spn read from the CAN message */
   SPN_STATE    ui8_spn_state;         /* HED provided with the state of the spn (0 -inactive, 1, active) */
   uint8_t      ui8_fmi;               /* the fmi read from the message */
   uint8_t      ui8_occurance_count;   /* the occurrence count read from the CAN message */ 
   uint8_t      ui8_conversion_method; /* the conversion method read from the CAN message */ 
   bool_t       ui8_new_error;         /* HED provided - if # occurrences  increased since last message then new error is set to TRUE, otherwise it is FALSE */ 
} BAM_Spn_Values_;
#if NUMBER_OF_BAM_MESSAGES > 0
/* Structure for each BAM message */
typedef struct
{
   BAM_Spn_Values_   s_spn_values[NUMBER_SPN_PER_BAM];   /* array of read in spn/fmi info */
   uint8_t             ui8_lamp_status;                  /* lamp status read from CAN message */
   uint8_t             ui8_lamp_flash;                   /* lamp flash read from CAN message */
} BAM_Values_;

#ifdef USE_PAGED_RAM
   #pragma DATA_SEG __RPAGE_SEG PAGED_RAM_J1939_BAM
#endif


extern BAM_Properties_ s_BAM_Properties[NUMBER_OF_BAM_MESSAGES];  /* array of properties to be set by user and read by firmware*/
extern BAM_Values_     s_BAM_Values[NUMBER_OF_BAM_MESSAGES];      /* array of values set by firmware to be read by user */
extern uint8_t         ui8_Number_Of_J1939_BAM_Messages;        /* to be set by the user and read by fimrware to know how many active BAM messages there are */
#endif

#ifdef USE_PAGED_RAM 
#pragma DATA_SEG DEFAULT
#endif

#endif /* PROTOTYPES_BAM_H_ */
