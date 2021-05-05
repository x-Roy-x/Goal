#include "smartboy.h"

bool ble_mode = false;

bool get_ble_mode(void){
    return ble_mode;
}

CY_ISR(Button_Reset_Int_Handler) {
    ble_mode = false;
    change_mode();
    Button_Reset_ClearInterrupt();
}

CY_ISR(Button_Scan_Int_Handler) {
    ble_mode = !ble_mode;
    change_mode();
    Button_Scan_ClearInterrupt();
}
