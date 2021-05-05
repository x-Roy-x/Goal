#include "goal.h"

void read(void) {  
    CYBLE_GATTS_HANDLE_VALUE_NTF_T tempHandle;
    
    uint16 LED_Conect_State = get_data_mcu(0);
    
    tempHandle.attrHandle = CYBLE_SWITCH_FAUCET_CHAR_HANDLE;
    tempHandle.value.val = (uint8 *) &LED_Conect_State;
    tempHandle.value.len = 1;
    CyBle_GattsWriteAttributeValue(&tempHandle, 0, &cyBle_connHandle,CYBLE_GATT_DB_LOCALLY_INITIATED);  
}

/***************************************************************************************************************************/

void write(CYBLE_GATTS_WRITE_CMD_REQ_PARAM_T *wrReqParam) {
    if(wrReqParam->handleValPair.attrHandle == CYBLE_SWITCH_FAUCET_CHAR_HANDLE) {
        if(CYBLE_GATT_ERR_NONE == CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair, 0, &cyBle_connHandle,CYBLE_GATT_DB_PEER_INITIATED)) {   
            set_data_mcu(0, wrReqParam->handleValPair.value.val[0]);
            CyBle_GattsWriteRsp(cyBle_connHandle);
        }
    }
}
