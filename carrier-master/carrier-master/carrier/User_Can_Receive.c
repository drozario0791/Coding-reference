#include "reserved.h"
#include "Prototypes.h"
#include "Constants.h"
#include "User_App.h"
#include "User_Can_Receive.h"
#include "can_service.h"


/******************************************************************************
 *
 *        Name: User_Can_Receive()
 *
 * Description: This function is called from the underlying HED main
 *              loop when CAN messages are present in their receive
 *              FIFO.
 *
 *              Process the received CAN message, which means that we
 *              look for the message in both of our linked lists of
 *              messages and call the function that was registered to
 *              handle the received CAN message. Typically these
 *              functions, within the device drivers, copy the CAN
 *              received data into the messages data structure within
 *              the device's device_data structure. The receive
 *              function also handles little endian to big endian
 *              conversions.
 *
 *  Parameters:
 *
 *  canmessage: The parsed incoming CAN message.
 *   module_id: Module id that sent the message.
 *    can_line: CAN line from whence it came.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void User_Can_Receive(Can_Message_ canmessage, uint8_t module_id, CANLINE_ can_line)
{

    //
    // Process the received CAN message. See comment above.
    //
    fvt_can_process_rx_message(canmessage, module_id, can_line);

}
