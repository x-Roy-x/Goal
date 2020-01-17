#include "smartboy.h"

uint16 goal_kitchen_uuids[GOAL_KITCHEN_UUIDS] = {
    0xB1B1
};

uint8 get_uuids_amount_of_device(uint8 connectedDeviceID) {
    uint8 uuids = 1;
    
    switch(connectedDeviceID) {
        case GOAL_KITCHEN:
            uuids = GOAL_KITCHEN_UUIDS;
            break;
        default:
		    break;
    }
    
    return uuids;
}

char * get_characteristic_name(uint16 characteristicUUID) {
    char * characteristicName;
    
    switch(characteristicUUID) {
        case GAS_UUID:
            characteristicName = "Gas value";
            break;
        default:
		    break;
    }
    
    return characteristicName;
}

uint16 get_current_uuid(uint8 connectedDeviceID, uint8 position) {
    uint16 uuid;
    
    switch(connectedDeviceID) {
        case GOAL_KITCHEN:
            uuid = goal_kitchen_uuids[position];
            break;
        default:
		    break;
    }
    
    return uuid;
}

void request_characteristic_value(uint16 characteristicUUID) {
	CYBLE_API_RESULT_T 			apiResult;
    CYBLE_GATTC_READ_BY_TYPE_REQ_T readByTypeReqParam;
	CYBLE_GATT_ATTR_HANDLE_RANGE_T range;
    CYBLE_UUID_T uuid;
    CYBLE_GATTC_READ_BY_GRP_RSP_PARAM_T readResponse;
    
    isDataRead = false;
    
    /* This event is generated whenever the discovery procedure is complete*/
    range.startHandle   = 0x0001;
    range.endHandle     = 0xFFFF;
    uuid.uuid16         = characteristicUUID;       //uuid of the characteristic to be read.

    readByTypeReqParam.range = range;
    readByTypeReqParam.uuid = uuid;
    readByTypeReqParam.uuidFormat = 0x01;

    apiResult = CyBle_GattcReadUsingCharacteristicUuid(connHandle, &readByTypeReqParam);
    
    if ( apiResult != CYBLE_ERROR_OK ) {
        // TODO Error handling
    }
    
    CyBle_ProcessEvents();
    
    /*while(!isDataRead) {
        // Wait for data read
    }*/
    
    //uint8 data = get_data();
    //return data;
}
