#ifndef RTC_BIPBOY_H
#define RTC_BIPBOY_H
#include "smartboy.h"
    
    struct Time {
        uint8 hour;
        uint8 minute;
        uint8 second;
    };
    
    struct Date {
        uint8 day;
        uint8 month;
        uint16 year;
    };
    
    /* Time: 00:00:00 */
    #define TIME_HOUR           (0x00u)
    #define TIME_MIN            (0x00u)
    #define TIME_SEC            (0x00u)
    #define TIME_HR_MIN_SEC     ((uint32)(TIME_HOUR << RTC_HOURS_OFFSET) | \
                                (uint32)(TIME_MIN << RTC_MINUTES_OFFSET)    | \
                                 TIME_SEC)
    /* Date: 01/01/2019 */
    #define DATE_MONTH          (RTC_JANUARY)
    #define DATE_DAY            (0x01u)
    #define DATE_YEAR           (0x2019u)
    #define DATE_MONTH_DAY_YEAR ((uint32)(DATE_MONTH << RTC_MONTH_OFFSET)   | \
                                (uint32)(DATE_DAY << RTC_DAY_OFFSET)        | \
                                 DATE_YEAR)

    #define SYSTICK_EACH_1_HZ   (10u)
    #define SYSTICK_RELOAD      (CYDEV_BCLK__SYSCLK__HZ / SYSTICK_EACH_1_HZ)
    
    /* Interrupt prototype */
    CY_ISR_PROTO(SysTickIsrHandler);
    
    void rtc_init(void);
    void rtc_update_time(void);
    void rtc_update_date(void);
    
    void set_current_time(void);
    void set_current_date(void);
    void set_time(void);
    void set_date(void);
    
    int8 get_hour(void);
    int8 get_minute(void);
    int8 get_second(void);
    int8 get_day(void);
    int8 get_month(void);
    int16 get_year(void);
    
#endif

