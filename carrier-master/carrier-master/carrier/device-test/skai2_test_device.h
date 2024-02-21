#ifndef SKAI2_TEST_DEVICE_H_
#define SKAI2_TEST_DEVICE_H_


//=============================================================================
//
// A function to test the transmission of can data for various devices
//
//=============================================================================
//
void skai2_device_test_CAN_TX_timing(device_instances_t device);

//=============================================================================
//
// A function to set can timeouts. Use this function in User_init.c
//
//=============================================================================
//
void skai2_device_test_CAN_RX_timeout(device_instances_t device);

//=============================================================================
//
// skai2_device_test_getter_functions: A function used to check if all the
//                                     bits are received in the right order.
//
//
//=============================================================================
//
void skai2_device_test_getter_functions(device_instances_t device);

#endif
