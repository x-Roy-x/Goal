#include "goal.h"

uint8 data_set[1];
uint8 data_get[2];

void i2c_init(void) {
    I2C_Start();  
    I2C_EzI2CSetBuffer1(1, 1, data_set);
    I2C_EzI2CSetBuffer2(2, 2, data_get);
}

void set_data_mcu(uint8 index, uint16 value) {
    data_set[index] = value;
}

uint16 get_data_mcu(uint8 index) {
    return data_get[index];
}
