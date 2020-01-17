#ifndef I2C_GOAL_H
#define I2C_GOAL_H
#include "goal.h"
    
    void i2c_main_loop(void);
    uint16 get_data_from_ble(uint8 index);
    void set_data_from_mcu(uint8 index, uint16 value);
    
#endif
    