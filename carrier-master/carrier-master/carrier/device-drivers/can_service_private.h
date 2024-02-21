/******************************************************************************
 *
 *        Name: can_service_private.h
 *
 * Description:
 *
 *      Author: Tom & Deepak
 *        Date: Wednesday, 19 June 2019
 *
 ******************************************************************************
 */

#ifndef CAN_SERVICE_PRIVATE_H
#define CAN_SERVICE_PRIVATE_H

//
//
typedef struct device_data_s {
    struct device_data_s *next_ptr;
    uint8_t device_instance_number;
} device_data_t;

#endif // CAN_SERVICE_PRIVATE_H
