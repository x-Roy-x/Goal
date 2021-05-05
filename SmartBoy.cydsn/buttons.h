#ifndef BUTTONS_H
#define BUTTONS_H
#include "smartboy.h"

    bool get_ble_mode(void);

    CY_ISR_PROTO(Button_Scan_Int_Handler);
    CY_ISR_PROTO(Button_Reset_Int_Handler);
 
#endif
    