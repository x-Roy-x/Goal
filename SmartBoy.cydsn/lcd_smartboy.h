#ifndef LCD_BIPBOY_H
#define LCD_BIPBOY_H
#include "smartboy.h"
    
    CY_ISR_PROTO(Timer_Update_Int_Handler);
    
    void lcd_init(void);
    void lcd_data_screen(void);
    void lcd_init_devices_list_screen(void);
    
#endif
