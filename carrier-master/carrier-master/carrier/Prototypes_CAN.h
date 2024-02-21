/*
** ###################################################################
**     Filename  : Prototypes_CAN.h
**     Processor : Not applicable
**     FileFormat: V2.00
**     Abstract  :
**         This file contains all the function and structure prototypes
**         that are required for CAN Messaging
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
#ifndef PROTOTYPES_CAN_H_
#define PROTOTYPES_CAN_H_

#include "typedefs.h"

/*Types of CAN message identifiers */
typedef enum
{
    STANDARD, /* Identifier is standard 11 bit */
    EXTENDED  /* Identifier is extended 29 bit */
} IDENTIFIER_TYPE_;

/*Variables in a Can Message */
#if defined XGATE_HEADER_FILE && !defined X_GATE_FILE_COMPILE
#pragma align on
#endif

typedef struct
{
    uint32_t identifier;   /* CAN Message identifier - this length is dependent on value in TYPE */
    uint16_t tsr;          /* CAN Message RX timestamp - free running timer with a frequency the same as the CAN baud rate */
    uint8_t  data[8];      /* CAN Message Payload */
    uint8_t length;        /* CAN message Length */
    IDENTIFIER_TYPE_ type; /* CAN message type =Extended or Standard*/
#if CAN_REMOTE_TRANSMISSION_REQUEST_ENABLED == TRUE
    bool_t rtr;                     /* remote transmission request, FALSE = not active, TRUE = active */
#endif
} Can_Message_;


/*These are the supported baud rates for the CAN baud rate functions below*/
typedef enum
{
    CAN_BAUD_5,
    CAN_BAUD_10,
    CAN_BAUD_20,
    CAN_BAUD_50,
    CAN_BAUD_100,
    CAN_BAUD_125,
    CAN_BAUD_250,
    CAN_BAUD_500,
    CAN_BAUD_1000,
    CAN_BAUD_INVALID = -1
} Can_Baud_Rate_;


#if defined XGATE_HEADER_FILE && !defined X_GATE_FILE_COMPILE
#pragma align off
#endif
/*CAN line enumeration */
/* Consult your module's data sheet - the values allowable are dependent on your modules HW configuration*/
typedef enum
{
    CANLINE_NOT_USED = -1,
    CAN1 =0,
    CAN2,
    CAN3,
    CAN4,
    CAN5
} CANLINE_;

/*Updated format*/
typedef CANLINE_ Canline_t;

/* CAN Bus Status */
typedef enum
{
    CAN_OK_STATUS,        /* CAN bus working with no detected error state */
    CAN_WARNING_STATUS,   /* CAN bus detecting errors - error count threshold not exceeded yet */
    CAN_ERROR_STATUS,     /* CAN bus detecting errors - error count threshold exceeded */
    CAN_BUSOFF_STATUS,    /* CAN bus detected BUSS OFF condition*/
    CAN_INVALID           /* Invalid CANLINE_ sent */
} CAN_STATUS_;

typedef enum
{
    CAN_WRITE_OK,           /*Message added to Transmit buffer */
    CAN_WRITE_BUFFER_FULL   /*Transmit buffer is full, could not add */
} CAN_WRITE_STATUS;

typedef enum
{
    CAN_NO_MESSAGES_MISSED, /* No messages missed */
    CAN_MESSAGES_MISSED,    /* Messages are missing, the receive buffer overflow */
    CAN_LINE_INVALID        /* Invalid CANLINE_ sent */
} Can_Missing_Message_Status_t;

typedef enum
{
    RESET_CAN_MISSING_MESSAGE_SUCCESS = 0,  /* Reset the missing messages status of the CAN bus successfully */
    RESET_CAN_MISSING_MESSAGE_INVALID_CAN_LINE  /* Invalid CANLINE_ sent */
} Reset_Can_Missing_Message_Status_t;

typedef enum CAN_BAUD_CHANGE_STATUS_ENUM
{
    CAN_BAUD_CHANGE_SUCCESS = 0,     /* CAN baud rate changed successfully */
    CAN_BAUD_CHANGE_INVALID_MODULE,  /* Bad module ID specified */
    CAN_BAUD_CHANGE_INVALID_CANLINE, /* Bad canline specified */
    CAN_BAUD_CHANGE_INVALID_BAUD,    /* Bad baud rate specified */
    CAN_BAUD_CHANGE_ACCESS_DENIED    /* CAN line configured for CANLink protocol and cannot be changed */
} CAN_Baud_Change_Status_t;

/*
 * This function sends a CAN message
 * Parameters:
 *
 * module_id is the module id of the unit to transmit the CAN message
 * from. see Constants.h for valid IDs
 *
 * can line is the value of the can line you want to send the message
 * on (see CANLINE_ enumeration)
 *
 * can message is a structure that contains all the info about the can
 * message
 *
 * Returns: <see CAN_WRITE_STATUS enumeration for details >
 */
CAN_WRITE_STATUS Send_CAN_Message(uint8_t module_id, CANLINE_ canline, Can_Message_ canmessage);

/*
 * This function gets the status of the CAN bus
 *
 * Parameters:
 *
 * can line is which can line you want to check (see CANLINE_
 * enumeration)
 *
 * Returns:
 *
 * Returns: <see CAN_STATUS_ enumeration for details >
 */
CAN_STATUS_ Check_For_CAN_Status(CANLINE_ canline);

/*
 * This function gets the missing messages status of the CAN bus
 * Parameters:
 * can line is which can line you want to check (see CANLINE_ enumeration)
 * Returns:
 * Returns: <see Can_Missing_Message_Status_t enumeration for details >
 */
Can_Missing_Message_Status_t Get_CAN_Missing_Messages_Status(CANLINE_ canline);

/*
 * This function resets the missing messages status of the CAN bus
 * Parameters:
 * can line is which can line you want to check (see CANLINE_ enumeration)
 * Returns:
 * Returns: <see Reset_Can_Missing_Message_Status_t enumeration for details >
 */
Reset_Can_Missing_Message_Status_t Reset_CAN_Missing_Messages_Status(CANLINE_ canline);

/* This function sets the application CAN baud rate on the given module and CAN line
 * Will not change the baud rate on a client (I/O) module or a CAN line configured
 * to use the CANLink protocol in Orchestra.
 * Will store and use that baud rate if the module goes into boot code
 * Parameters:
 *  module_id - the module number of the master module (from Constants.h)
 *  canline - the CAN line to change the baud rate (CAN1, CAN2, etc.)
 *  baud_rate - the desired baud rate (CAN_BAUD_250, CAN_BAUD_500, etc.)
 * Returns: CAN_Baud_Change_Status_t - indicates success or failure (see enum definition)
 */
CAN_Baud_Change_Status_t SetCanBaudRate(uint8_t module_id, CANLINE_ canline, Can_Baud_Rate_ baud_rate);

/* This function returns the CAN baud rate on the given module and CAN line
 * Parameters:
 *  module_id - the module number of the master module (see Constants.h)
 *  canline - the CAN line to read the baud rate (CAN1, CAN2, etc.)
 * Returns: <see Can_Baud_Rate_ enumeration>
 */
Can_Baud_Rate_ GetCanBaudRate(uint8_t module_id, CANLINE_ canline);

#endif /*PROTOTYPES_CAN_H_*/
