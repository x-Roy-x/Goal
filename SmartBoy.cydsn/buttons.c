#include "smartboy.h"

int16 counter_up_down = 0;
uint8 counter_ok_back = 0;

uint8 backStack[3];

CY_ISR(Button_Back_Int_Handler) { 
    button_back_switch_screen();
    
    Button_Back_ClearInterrupt();
}

CY_ISR(Button_Ok_Int_Handler) {
    button_ok_switch_screen();
    
    Button_Ok_ClearInterrupt();
}

CY_ISR(Button_Up_Int_Handler) {
    counter_up_down++;
    scroll_pages();
    
    Button_Up_ClearInterrupt();
}

CY_ISR(Button_Down_Int_Handler) {
    counter_up_down--;
    scroll_pages();
    
    Button_Down_ClearInterrupt();
}

void scroll_pages(void) {
    uint8 activeScreen = get_active_screen();
    uint8 pages = get_pages_number(activeScreen) - 1;

    if(pages == 0) {
        counter_up_down = 0;
    } else if(pages > 0) {
        
        switch(activeScreen) {
            case SET_TIME_SCREEN:
                lcd_set_time_screen();
                return;
            case SET_DATE_SCREEN:
                lcd_set_date_screen();
                return;
        }
        
        if(counter_up_down < 0) {
            counter_up_down = pages;
        } else if(counter_up_down > pages) {
            counter_up_down = 0;
        }
        
        switch(activeScreen) {
            case MENU_SCREEN:
                lcd_menu_screen(counter_up_down);
                break;
            case SENSORS_SCREEN:
                lcd_sensors_screen(counter_up_down);
                break;
            case DATE_SCREEN:
                lcd_date_screen(counter_up_down);
                break;
            case SET_TIME_SCREEN:
                lcd_set_time_screen();
                return;
            case DEVICES_LIST_SCREEN:
                lcd_devices_list_screen(counter_up_down);
                break;
        }
        
        lcd_scroll_brackets();
    }
}

void button_back_switch_screen(void) {
    uint8 activeScreen = get_active_screen();
    switch(activeScreen) {
        case MAIN_SCREEN:
            return;
        case MENU_SCREEN:
            lcd_main_screen();
            break;
        case HOME_CONTROL_SCREEN:
        case DEVICES_LIST_SCREEN:
        case CONNECTED_DEVICE_SCREEN:
            isConnecting = false;
            handle_disconnection();
            // Change peripheral address for dissmiss accidental connection during scanning
            set_peripheral_address_to_connect(0xAA);
            lcd_menu_screen(0);
            //counter_set_back_stack_value(0);
        case SENSORS_SCREEN:
        case DATE_SCREEN:
        case PHONE_SCREEN:
            lcd_menu_screen(0);
            //counter_set_back_stack_value(0);
            break;
        case SET_TIME_SCREEN:
        case SET_DATE_SCREEN:   
            counter_ok_back--;
            counter_up_down = 0;
            if(counter_ok_back == 0) {
                lcd_date_screen(0);
            } else if(counter_ok_back < 5) {
                switch(activeScreen) {
                    case SET_TIME_SCREEN:
                        lcd_set_time_screen();
                        return;
                    case SET_DATE_SCREEN:
                        lcd_set_date_screen();
                        return;
                }
            }
    }
    
    // Change activeScreen value after screen changing
    activeScreen = get_active_screen();
    uint8 pages = get_pages_number(activeScreen) - 1;
    
    if(pages > 0) {
        lcd_scroll_brackets();
    }
    
    counter_up_down = 0;
}

void button_ok_switch_screen(void) {
    uint8 activeScreen = get_active_screen();

    switch(activeScreen) {
        case MAIN_SCREEN:
            lcd_menu_screen(0);
            break;
        case MENU_SCREEN:
            switch(counter_up_down) {
                // HOME_SCREEN
                case 0:
                    backStack[0] = 0;
                    lcd_home_control_screen();
                    //handle_scan();
                    return;
                // DATE_SCREEN
                case 1:
                    backStack[0] = 1;
                    lcd_date_screen(0);
                    break;
                // SENSORS_SCREEN
                case 2:
                    backStack[0] = 2;
                    lcd_sensors_screen(0);                    
                    break;
                // PHONE_SCREEN
                case 3:
                    return;
            }
            break;
        case DATE_SCREEN:
            counter_ok_back = 1;
            switch(counter_up_down) {
                // SET_TIME_SCREEN
                case 0:
                    set_current_time();                    
                    lcd_set_time_screen();
                    break;
                // SET_DATE_SCREEN
                case 1:
                    counter_up_down = 0;
                    set_current_date();
                    lcd_set_date_screen();
                    break;
                // AlARM_SCREEN
                case 2:
                    //counter_up_down = 0;
                    return;
                // STOPWATCH_SCREEN
                case 3:
                    //counter_up_down = 0;
                    return;
                // TIMER_SCREEN
                case 4:
                    //counter_up_down = 0;
                    return;
                // TIME_SETTINGS_SCREEN
                case 5:
                    //counter_up_down = 0;
                    return;
            }
            return;
        case SENSORS_SCREEN:
            return;
        case SET_TIME_SCREEN:
        case SET_DATE_SCREEN:
            counter_ok_back++;
            counter_up_down = 0;
            if(counter_ok_back >= 5) {
                switch(activeScreen) {
                    case SET_TIME_SCREEN:
                        rtc_update_time();
                        break;
                    case SET_DATE_SCREEN:
                        rtc_update_date();
                        break;
                }
                lcd_date_screen(0);
            } else if(counter_ok_back > 1) {
                switch(activeScreen) {
                    case SET_TIME_SCREEN:
                        lcd_set_time_screen();
                        return;
                    case SET_DATE_SCREEN:
                        lcd_set_date_screen();
                        return;
                }
            }
        case DEVICES_LIST_SCREEN:
            if(counter_up_down == goalDevices) {
                handle_scan();
                isConnecting = false;
                set_peripheral_address_to_connect(0xAA);
                lcd_home_control_screen();
            } else if( counter_ok_back != goalDevices) {
                isConnecting = true;
                check_device_availability_and_connect(counter_up_down);
                lcd_home_control_screen();
            }
    }
    
    // Change activeScreen value after screen changing
    activeScreen = get_active_screen();
    uint8 pages = get_pages_number(activeScreen) - 1;
    
    if(pages > 0) {
        lcd_scroll_brackets();
    }
    
    counter_up_down = 0;
}

void counter_set_back_stack_value(uint8 backStackLevel) {
    counter_up_down = backStack[backStackLevel];
}

int16 get_counter_up_down(void) {
    return counter_up_down;
}

uint8 get_counter_ok_back(void) {
    return counter_ok_back;
}

void set_counter_up_down(int16 counter) {
    counter_up_down = counter;
}
