#include "smartboy.h"

void main_loop(void) {
    CyBle_ProcessEvents();
    CyBle_EnterLPM(CYBLE_BLESS_DEEPSLEEP);
}

void BleCallBack(uint32 event,void* eventParam) {
    CYBLE_GATTS_WRITE_CMD_REQ_PARAM_T *wrReqParam;
    
    switch(event) {
        case CYBLE_EVT_STACK_ON:
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
            CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
            //set_data_from_ble(0,1);
        break;
        
        case CYBLE_EVT_GATT_CONNECT_IND:
            //set_data_from_ble(0,0);
        break;
            
        case CYBLE_EVT_GATTS_WRITE_REQ:
            wrReqParam = (CYBLE_GATTS_WRITE_CMD_REQ_PARAM_T *) eventParam;
            
            //update_data_write(wrReqParam);
            
        break;
        
        default:
        break;
    }
}

void init_ble(void){
    CyBle_Start(BleCallBack);
}