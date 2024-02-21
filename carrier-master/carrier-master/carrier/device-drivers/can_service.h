/******************************************************************************
 *
 *        Name: can_service.h
 *
 * Description: Public interface to some of the can_service emthods
 * and attributes.
 *
 *      Author: Tom
 *        Date: Wednesday, 17 July 2019
 *
 ******************************************************************************
 */
#ifndef CAN_SERVICE_H_
#define CAN_SERVICE_H_

#define NO_RX_TIMEOUT_FOUND 0
#define NO_RX_MESSAGES_REGISTERED 0xffffffff

typedef enum {NONE=0,
              ONE=1,
              TWO=2,
              THREE=3,
              FOUR=4,
              FIVE=5,
              SIX=6,
              SEVEN=7,
              EIGHT=8,
              ALL=255
} device_instances_t;

//
// CAN transmit frequency and expected CAN receive message rates.
//
// NOTE: These enum values assume a User_App() call period of 10ms, as
//       defined in Orchestra.
//
typedef enum {
	TIMED_OUT = -1,
	NO_TIME_OUT = 0,
    TX_SEND_EACH_CALL = 0,
    EVERY_10MS = 1,
    EVERY_20MS = 2,
    EVERY_30MS = 3,
    EVERY_40MS = 4,
    EVERY_50MS = 5,
    EVERY_60MS = 6,
    EVERY_70MS = 7,
    EVERY_80MS = 8,
    EVERY_90MS = 9,
    EVERY_100MS = 10,
    EVERY_200MS = 20,
    EVERY_300MS = 30,
    EVERY_400MS = 40,
    EVERY_500MS = 50,
    EVERY_600MS = 60,
    EVERY_700MS = 70,
    EVERY_800MS = 80,
    EVERY_900MS = 90,
    EVERY_1000MS = 100,
	EVERY_2S = 200,
	EVERY_5S = 500,
	EVERY_10S = 1000,
} can_rate_t;


//=============================================================================
//
// A Function used to set the time out for a particular receive can
// message.
//
//=============================================================================
//
void
fvt_can_set_timeout_receive_id_j1939_byte(
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte,
    can_rate_t timeout_limit);

bool_t
fvt_can_get_timeout_status_for_receive_id_j1939_byte(
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

void
fvt_can_set_timeout_receive_id(
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    can_rate_t timeout_limit);

bool_t
fvt_can_get_timeout_status_for_receive_id(
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id);


//
// A structure that hold the contents of the timeout receive message.
//
typedef struct can_rx_timeout_status_s
{
    // ID of the Module that will receive the message
    uint8_t module_id;
    // One of three CAN interfaces
    CANLINE_ can_line;
    // ID of CAN message. Either extended or standard are acceptable.
    uint32_t can_id;
    // J1939 byte (optional)
    uint8_t j1939_byte;
    // The device_index is zero-indexed. Use the enums ONE, TWO,
    // THREE, ... which equate to 0, 1, 2, ...
    device_instances_t device;
} can_rx_timeout_status_t;


//////////////////////////////////////////////////////////////////////////////
//
// This function must be called from the User_App() each time it executes.
//
//////////////////////////////////////////////////////////////////////////////
//
void can_rx_check_message_timeouts();


//////////////////////////////////////////////////////////////////////////////
//
// This function must be called from within the User_Can_Receive().
//
//////////////////////////////////////////////////////////////////////////////
//
void fvt_can_process_rx_message(
    Can_Message_ received_can_message,
    uint8_t module_id,
    CANLINE_ can_line);


int canPrintf(
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    char *format, ...);

// DEBUG MACROS BUILT USING CANPRINTF

// DEBUG: When used the macro prints the file name, line number and
// the message passed to the function call.
// USAGE:
// 1) DEBUG("This is a Test");
// --> Output - DEBUG: File LineNo, This is a Test
// 2) DEBUG("Answer to the universe is %d", 42);
// --> Output DEBUG: File LineNo, Answer to the universe is 42.
#define DEBUG(M,...)                                                    \
    canPrintf(0, CAN3, 0x1FFFF000, "DEBUG: %s %d:" M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

// Allows diagnostic information to be written to the console using
// canPrintf. In other words, it can be used to add diagnostics in
// your C program.
// USAGE: 1) ASSERT((COUNT > 10) || (COUNT < 90));
#define ASSERT(expr)                                                    \
        if((!(expr)) && ((FVT_ASSERT) > 0))                             \
        canPrintf(0, CAN3, 0x1FFFF000, "ASSERT: File: %s Ln: %d, Assert Failed \n", __FILE__, __LINE__)\

#define CAN_ASSERT(expr)						\
    { \
        if((!(expr)) && ((FVT_ASSERT) > 0)) \
        { \
            canPrintf(0, CAN3, 0x1FFFF000, "ASSERT: File: %s Ln: %d, Assert Failed \n", __FILE__, __LINE__); \                                                 \
        } \
    } \


#if(defined LITTLE_ENDIAN)
#define BYTE_SWAP16(a) 	(a)
#else
#define BYTE_SWAP16(a) (uint16_t)((((uint16_t)(a)&0xFF00)>>8) | \
                                    (((uint16_t)(a)&0x00FF)<<8))


static inline uint32_t BYTE_SWAP32(uint32_t input)
{

    uint32_t output = 0;

    //
    // Disect the 32bit into two 16bit values
    //
    uint16_t MDH = ((uint16_t)((input & 0xFFFF0000) >> 16));
    uint16_t MDL = ((uint16_t)((input & 0x0000FFFF)));

    MDH = BYTE_SWAP16(MDH);
    MDL = BYTE_SWAP16(MDL);

    //
    // Swap MDL and MDH
    //
    output = (uint32_t)(((MDL << 16) | MDH));

    return output;

}


#endif

#endif
