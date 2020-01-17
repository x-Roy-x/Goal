#include "ez_ble.h"

void i2c_init(void){
    I2C_BLE_Start();   
    I2C_BLE_EzI2CSetBuffer1(1,1,i2cbuf);
    I2C_BLE_EzI2CSetBuffer2(2,2,i2cbuf_control);
}