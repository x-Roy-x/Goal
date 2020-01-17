#ifndef GOAL_DEVICES_H
#define GOAL_DEVICES_H
#include "smartboy.h"    

    uint8 get_uuids_amount_of_device(uint8 connectedDeviceID);
    char * get_characteristic_name(uint16 characteristicUUID);
    uint16 get_current_uuid(uint8 connectedDeviceID, uint8 position);
    void request_characteristic_value(uint16 characteristicUUID);
       
#endif
