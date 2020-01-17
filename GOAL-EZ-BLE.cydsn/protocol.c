#include "goal.h"

void protocol_init(void) {
    i2c_init();
    CyBle_Start(BleCallBack);
}

void main_loop(void) {
    CyBle_ProcessEvents();
    CyBle_EnterLPM(CYBLE_BLESS_DEEPSLEEP);
}
