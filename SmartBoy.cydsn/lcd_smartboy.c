#include "smartboy.h"

CY_ISR(Timer_Update_Int_Handler) {
    lcd_update_dynamic_screen();
}

bool arrayScreens[10];
uint8 numberOfPages[10] = {
 //MAIN_SCREEN  MENU_SCREEN   SENSORS_SCREEN
   ONE_PAGE,    MENU_STRINGS, SENSORS_STRINGS,
 //DATE_SCREEN   HOME_CONTROL_SCREEN  PHONE_SCREEN
   DATE_STRINGS, ONE_PAGE,            ONE_PAGE,
 //SET_TIME_SCREEN   SET_DATE_SCREEN   DEVICES_SCREEN
   SET_TIME_STRINGS, SET_DATE_STRINGS, ONE_PAGE,
 //CONNECTED_DEVICE_SCREEN
   ONE_PAGE
};

// Buffers for Time and Date strings
char timeBuffer[16u];
char dateBuffer[16u];

uint8 lengthArray = sizeof(arrayScreens);

const char * menuStrings[MENU_STRINGS][2] = {
    { "Home control", "" },
    { "Date & Time", "" },
    { "Sensors data", "" },
    { "Phone", "connection" }
};

const char * dateStrings[DATE_STRINGS][2] = {
    { "Set Time", "" },
    { "Set Date", "" },
    { "Alarm", "" },
    { "Stopwatch", "" },
    { "Timer", "" },
    { "Time Settings", "" }
};

const char * sensorsStrings[SENSORS_STRINGS][2] = {
    { "Weather", "T"},
    { "Pulse", "" }
};

void lcd_init(void) {
    LCD_Start();
    LCD_Position(0, 4);
    LCD_PrintString("SmartBoy");
    CyDelay(2000); //5000
}

void lcd_main_screen(void) {
    LCD_ClearDisplay();
    set_active_screen(MAIN_SCREEN);
    
    uint32 time;
    uint32 date;
    
    time = RTC_GetTime();
    date = RTC_GetDate();
    
    sprintf(timeBuffer, "%02lu:%02lu:%02lu", RTC_GetHours(time), 
        RTC_GetMinutes(time), RTC_GetSecond(time));
    sprintf(dateBuffer, "%02lu/%02lu/%02lu", RTC_GetDay(date), 
        RTC_GetMonth(date), RTC_GetYear(date));
    
    LCD_Position(0, 0);
    LCD_PrintString(timeBuffer);
    LCD_Position(0, 11);
    LCD_PrintString("100%");
    LCD_Position(0, 15);
    LCD_PutChar(LCD_CUSTOM_1);
    LCD_Position(1, 0);
    LCD_PrintString(dateBuffer);
}

void lcd_home_control_screen(void) {
    LCD_ClearDisplay();
    set_active_screen(HOME_CONTROL_SCREEN);
    
    
    if(ble_state == BLE_DISCONNECTED) {
        handle_scan();
        if(isConnecting) {
            LCD_PrintString("Connecting");
        } else {
            LCD_PrintString("Scanning");
        }
    } else if(ble_state == BLE_CONNECTED) {
        lcd_init_connected_device_screen();
    }
}

void lcd_init_devices_list_screen(void) {
    set_pages_of_dynamic_screen(DEVICES_LIST_SCREEN, goalDevices + 1);
    set_counter_up_down(0);
    lcd_devices_list_screen(0);
    if(goalDevices > 0) {
        lcd_scroll_brackets();
    }
}

void lcd_devices_list_screen(uint8 stringToDisplay) {
    LCD_ClearDisplay();
    set_active_screen(DEVICES_LIST_SCREEN);
    char buffer[15];
    
    LCD_Position(0, 0);
    if(stringToDisplay == goalDevices) {
        sprintf(buffer, "Rescan");
        LCD_PrintString(buffer);
    } else { 
        char * name = get_device_name(stringToDisplay);
        LCD_PrintString(name);
    }
}

void lcd_init_connected_device_screen(void) {
    uint8 connectedDeviceID = get_connected_device_id();
    uint8 pages = get_uuids_amount_of_device(connectedDeviceID);
    set_pages_of_dynamic_screen(CONNECTED_DEVICE_SCREEN, pages);
    set_counter_up_down(0);
    uint16 uuid = get_current_uuid(connectedDeviceID, 0);
    //char * characteristicName = get_characteristic_name(uuid);
    //uint8 characteristicValue = read_characteristic_value(uuid);
    request_characteristic_value(uuid);
    //lcd_connected_device_screen(characteristicName, characteristicValue);
    if( pages > 1) {
        lcd_scroll_brackets();
    }
}

void lcd_connected_device_screen(char * characteristicName, uint8 characteristicValue) {
    LCD_ClearDisplay();
    set_active_screen(CONNECTED_DEVICE_SCREEN);
    
    char buffer[6];
    sprintf(buffer, "%u", characteristicValue);
    
    LCD_Position(0, 0);
    LCD_PrintString(characteristicName);
    LCD_Position(1, 0);
    LCD_PrintString(buffer);
}

void lcd_set_date_screen(void) {
    LCD_ClearDisplay();
    set_active_screen(SET_DATE_SCREEN);
    
    set_date();
    
    int8 day = get_day();
    int8 month = get_month();
    int16 year = get_year();
    
    sprintf(dateBuffer, "%02u/%02u/%02u", day, month, year);
    LCD_Position(0, 0);
    LCD_PrintString(dateBuffer);
    LCD_Position(0, 14);
    LCD_PrintString("OK");
    lcd_position_brackets();
}

void lcd_set_time_screen(void) {
    LCD_ClearDisplay();
    set_active_screen(SET_TIME_SCREEN);
    
    set_time();
    
    int8 hour = get_hour();
    int8 minute = get_minute();
    int8 second = get_second();    
    
    sprintf(timeBuffer, "%02u:%02u:%02u", hour, minute, second);
    LCD_Position(0, 0);
    LCD_PrintString(timeBuffer);
    LCD_Position(0, 14);
    LCD_PrintString("OK");
    lcd_position_brackets();
}

void lcd_position_brackets(void) {
    uint8 position = get_counter_ok_back();
    if(position == 1){
        LCD_Position(1, 0);
        LCD_PrintString("^^");
    } else if(position == 2){
        LCD_Position(1, 3);
        LCD_PrintString("^^");
    } else if(position == 3){
        LCD_Position(1, 6);
        LCD_PrintString("^^");
    }else if(position == 4){
        LCD_Position(1, 14);
        LCD_PrintString("^^");
    }
}

void lcd_sensors_screen(uint8 stringToDisplay) {
    LCD_ClearDisplay();
    set_active_screen(SENSORS_SCREEN);
    LCD_Position(0, 0);
    LCD_PrintString(sensorsStrings[stringToDisplay][0]);
    LCD_Position(1, 0);
    LCD_PrintString(sensorsStrings[stringToDisplay][1]);
}

void lcd_menu_screen(uint8 stringToDisplay) {
    LCD_ClearDisplay();
    set_active_screen(MENU_SCREEN);
    LCD_Position(0, 0);
    LCD_PrintString(menuStrings[stringToDisplay][0]);
    LCD_Position(1, 0);
    LCD_PrintString(menuStrings[stringToDisplay][1]);
}

void lcd_date_screen(uint8 stringToDisplay) {
    LCD_ClearDisplay();
    set_active_screen(DATE_SCREEN);
    LCD_Position(0, 0);
    LCD_PrintString(dateStrings[stringToDisplay][0]);
    LCD_Position(1, 0);
    LCD_PrintString(dateStrings[stringToDisplay][1]);
}

void lcd_update_dynamic_screen(void) {
    uint8 activeScreen = get_active_screen();
    
    switch(activeScreen) {
        case MAIN_SCREEN:
            lcd_main_screen();
            break;
    }
   
    if(ble_state == BLE_CONNECTED) {
        uint8 connectedDeviceID;
        
        switch(activeScreen) {
            case HOME_CONTROL_SCREEN:
                lcd_home_control_screen();
                break;
            case CONNECTED_DEVICE_SCREEN:
                if(isDataRead) {
                    connectedDeviceID = get_connected_device_id();
                    uint8 position = get_counter_up_down();
                    uint16 uuid = get_current_uuid(connectedDeviceID, position);
                    request_characteristic_value(uuid);
                }
                break;
            default:
		        break;
        }
    }
    
}

void set_active_screen(uint8 screen) {
    for(uint8 currentScreen = 0; currentScreen < lengthArray; currentScreen++) {
        if(currentScreen == screen) {
            arrayScreens[currentScreen] = true;
        } else {
            arrayScreens[currentScreen] = false;
        }
    }
}

uint8 get_active_screen(void) {
    uint8 activeScreen = 0;
    
    for(uint8 currentScreen = 0; currentScreen < lengthArray; currentScreen++) {
        if(arrayScreens[currentScreen] == true) {
            activeScreen = currentScreen;
            break;
        }
    }
    
    return activeScreen;
}

void set_pages_of_dynamic_screen(uint8 screen, uint8 pages) {
    numberOfPages[screen] = pages;   
}

void lcd_scroll_brackets(void) {
    LCD_Position(0, 15);
    LCD_PutChar('^');
    LCD_Position(1, 15);
    LCD_PutChar(LCD_CUSTOM_0);
}

uint8 get_pages_number(uint8 activeScreen) {
    return numberOfPages[activeScreen];
}
