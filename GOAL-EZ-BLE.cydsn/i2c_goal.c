#include "goal.h"

uint8 data_from_ble[1];
uint8 data_from_mcu[2];

void i2c_init(void) {
    I2C_Start();  
    I2C_EzI2CSetBuffer1(1, 1, data_from_ble);
    I2C_EzI2CSetBuffer2(2, 2, data_from_mcu);
}

void set_data_from_ble(uint8 index, uint16 value) {
    data_from_ble[index] = value;
}

uint16 get_data_from_mcu(uint8 index) {
    return data_from_mcu[index];
}
