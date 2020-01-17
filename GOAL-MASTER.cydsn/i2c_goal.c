#include "goal.h"

uint8 data_from_ble[1];
uint8 data_from_mcu[4] = {0, 10, 15, 17};

void i2c_main_loop(void) {
    I2C_Master_I2CMasterClearStatus();
    I2C_Master_I2CMasterWriteBuf(0x09, data_from_mcu, 4, I2C_Master_I2C_MODE_COMPLETE_XFER);
    while (0u == (I2C_Master_I2CMasterStatus() & I2C_Master_I2C_MSTAT_WR_CMPLT))
    {   
    /* Waits until master completes write transfer */
    }         
    I2C_Master_I2CMasterReadBuf(0x08, data_from_ble, 1, I2C_Master_I2C_MODE_COMPLETE_XFER);
}

uint16 get_data_from_ble(uint8 index) {
    return data_from_ble[index];
}

void set_data_from_mcu(uint8 index, uint16 value) {
    data_from_mcu[index] = value;
}
