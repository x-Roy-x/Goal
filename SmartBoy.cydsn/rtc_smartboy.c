#include "smartboy.h"

struct Time Time;
struct Date Date;

int8 hour;
int8 minute;
int8 second;

int8 day;
int8 month = 1;
int16 year;

uint32 timeHrMinSec = TIME_HR_MIN_SEC;
uint32 dateMonthDayYear = DATE_MONTH_DAY_YEAR;

void rtc_init(void) {
    /* Starts SysTick component */
    CySysTickStart();

    /* Configure SysTick timer to generate interrupt every 100 ms */
    CySysTickSetReload(SYSTICK_RELOAD);
    
    /* Find unused callback slot. */
    for (uint32 i = 0u; i < CY_SYS_SYST_NUM_OF_CALLBACKS; ++i) {
        if (CySysTickGetCallback(i) == NULL) {
            /* Set callback */
            CySysTickSetCallback(i, SysTickIsrHandler);
            break;
        }
    }
    
    /* Starts RTC component */
    RTC_Start();

    /* Set Date and Time */
    RTC_SetDateAndTime(timeHrMinSec, dateMonthDayYear);
    
    /* Set RTC time update period */
    RTC_SetPeriod(1u, SYSTICK_EACH_1_HZ);
}

uint8 counter;

CY_ISR(SysTickIsrHandler) {
    RTC_Update(); 
}

void set_time(void) {
    uint8 position = get_counter_ok_back();
    uint8 offset = get_counter_up_down();
    
    if(position == 1 && offset == 0) {
        hour = Time.hour;
        Time.minute = minute;
        minute = Time.minute;
        second = Time.second;
        set_counter_up_down(hour + 2);
    } else if(position == 2 && offset == 0) {
        Time.hour = hour;
        minute = Time.minute;
        Time.second = second;
        set_counter_up_down(minute + 2);
    } else if(position == 3 && offset == 0) {
        Time.minute = minute;
        second = Time.second;
        set_counter_up_down(second + 2);
    } else if(position == 4 && offset == 0) {
        Time.second = second;

    }
    
    offset = get_counter_up_down();
    
    if(position == 1) {
        hour = offset - 2;
        if(hour > 23) {
            set_counter_up_down(0 + 2);
            hour = 0;
        } else if(hour < 0) {
            set_counter_up_down(23 + 2);
            hour = 23;
        }
    } else if(position == 2) {
        minute = offset - 2;
        if(minute > 59) {
            set_counter_up_down(0 + 2);
            minute = 0;
        } else if(minute < 0) {
            set_counter_up_down(59 + 2);
            minute = 59;
        }
    } else if(position == 3) {
        second = offset - 2;
        if(second > 59) {
            set_counter_up_down(0 + 2);
            second = 0;
        } else if(second < 0) {
            set_counter_up_down(59 + 2);
            second = 59;
        }
    }
}

void set_date(void) {
    uint8 position = get_counter_ok_back();
    uint16 offset = get_counter_up_down();
    
    if(position == 1 && offset == 0) {
        day = Date.day;
        Date.month = month;
        month = Date.month;
        year = Date.year;
        set_counter_up_down(day + 2);
    } else if(position == 2 && offset == 0) {
        Date.day = day;
        month = Date.month;
        Date.year = year;
        set_counter_up_down(month + 2);
    } else if(position == 3 && offset == 0) {
        Date.month = month;
        year = Date.year;
        set_counter_up_down(year + 2);
    } else if(position == 4 && offset == 0) {
        Date.year = year;

    }
    
    offset = get_counter_up_down();
    
    if(position == 1) {
        day = offset - 2;
        if(day > 31) {
            set_counter_up_down(1 + 2);
            day = 1;
        } else if(day < 1) {
            set_counter_up_down(31 + 2);
            day = 31;
        }
    } else if(position == 2) {
        month = offset - 2;
        if(month > 12) {
            set_counter_up_down(1 + 2);
            month = 1;
        } else if(month < 1) {
            set_counter_up_down(12 + 2);
            month = 12;
        }
    } else if(position == 3) {
        year = offset - 2;
        if(year > 2050) {
            set_counter_up_down(2019 + 2);
            year = 2019;
        } else if(year < 2019) {
            set_counter_up_down(2050 + 2);
            year = 2050;
        }
    }
}

int8 get_hour(void) {
    return hour;
}

int8 get_minute(void) {
    return minute;
}

int8 get_second(void) {
    return second;
}

int8 get_day(void) {
    return day;
}

int8 get_month(void) {
    return month;
}

int16 get_year(void) {
    return year;
}

void rtc_update_time(void) {
    timeHrMinSec = ((uint32)(Time.hour << RTC_HOURS_OFFSET) | 
                    (uint32)(Time.minute << RTC_MINUTES_OFFSET) |
                             Time.second);
    RTC_SetDateAndTime(timeHrMinSec, dateMonthDayYear);    
}

void rtc_update_date(void) {
    dateMonthDayYear = ((uint32)(Date.month << RTC_MONTH_OFFSET)   |
                        (uint32)(Date.day << RTC_DAY_OFFSET)       |
                        Date.year);
    RTC_SetDateAndTime(timeHrMinSec, dateMonthDayYear);
}

void set_current_time(void) {
    Time.hour = 0;
    Time.minute = 0;
    Time.second = 0; 
}

void set_current_date(void) {
    Date.day = 1;
    Date.month = 1;
    Date.year = 2019;
}