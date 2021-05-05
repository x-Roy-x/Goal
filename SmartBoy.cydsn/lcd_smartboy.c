#include "smartboy.h"

CY_ISR(Timer_Update_Int_Handler) {
    lcd_data_screen();
}

char first_data_buffer[16u];
char second_data_buffer[16u];

void lcd_init(void) {
    LCD_Start();
}

void lcd_data_screen(void) {
    LCD_ClearDisplay();
    
    uint16 *adc_data_list = get_adc_list();
    sprintf(first_data_buffer, "%u", get_adc_by_channel(0));
    sprintf(second_data_buffer, "%u:%u", adc_data_list[0], adc_data_list[1]);
    
    LCD_Position(0, 0);
    LCD_PrintString(first_data_buffer);
    LCD_Position(1, 0);
    LCD_PrintString(second_data_buffer);
    free(adc_data_list);
}
