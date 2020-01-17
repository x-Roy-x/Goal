#include "goal.h"

void update_data_read(void) {  
    CYBLE_GATTS_HANDLE_VALUE_NTF_T tempHandle;
    
    uint16 LED_Conect_State = get_data_from_mcu(0);
    
    tempHandle.attrHandle = CYBLE_ROOM_LED_1_CHAR_HANDLE;
    tempHandle.value.val = (uint8 *) &LED_Conect_State;
    tempHandle.value.len = 1;
    CyBle_GattsWriteAttributeValue(&tempHandle, 0, &cyBle_connHandle,CYBLE_GATT_DB_LOCALLY_INITIATED);  
}

/***************************************************************************************************************************/

void update_data_write(CYBLE_GATTS_WRITE_CMD_REQ_PARAM_T *wrReqParam) {
    if(wrReqParam->handleValPair.attrHandle == CYBLE_ROOM_LED_1_CHAR_HANDLE) {
        if(CYBLE_GATT_ERR_NONE == CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair, 0, &cyBle_connHandle,CYBLE_GATT_DB_PEER_INITIATED)) {   
            set_data_from_ble(0, wrReqParam->handleValPair.value.val[0]);
            CyBle_GattsWriteRsp(cyBle_connHandle);
        }
    }
}
