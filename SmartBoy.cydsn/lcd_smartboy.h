#ifndef LCD_BIPBOY_H
#define LCD_BIPBOY_H
#include "smartboy.h"
    
    CY_ISR_PROTO(Timer_Update_Int_Handler);
    
    void lcd_init(void);
    void lcd_scroll_brackets(void);
    void lcd_position_brackets(void);
    void lcd_update_dynamic_screen(void);
    
    
    void lcd_main_screen(void);
    void lcd_menu_screen(uint8 stringToDisplay);
    void lcd_sensors_screen(uint8 stringToDisplay);
    void lcd_date_screen(uint8 stringToDisplay);
    void lcd_set_time_screen(void);
    void lcd_set_date_screen(void);
    
    void lcd_home_control_screen(void);
    void lcd_init_devices_list_screen(void);
    void lcd_devices_list_screen(uint8 stringToDisplay);
    void lcd_init_connected_device_screen(void);
    void lcd_connected_device_screen(char * characteristicName, uint8 characteristicValue);
    
    
    uint8 get_pages_number(uint8 activeScreen);
    void set_active_screen(uint8 screen);
    uint8 get_active_screen(void);
    void set_pages_of_dynamic_screen(uint8 screen, uint8 pages);
      
#endif
