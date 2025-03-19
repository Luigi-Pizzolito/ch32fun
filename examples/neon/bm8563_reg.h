#ifndef CH32_LIB_BM8563_REG_H
#define CH32_LIB_BM8563_REG_H

#include "ch32fun.h"

/*
 * OPERATION
*/
#define BM8563_ADDR 0x51
typedef enum {
    BM8563_STATUS_OK = 0,
    BM8563_STATUS_ERROR
} BM8563StatusTypeDef;

typedef struct {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
} bm8563_time_t;
typedef struct {
    uint8_t days;
    uint8_t weekday;
    uint8_t months;
    uint16_t years;
} bm8563_date_t;


/*
 * HELPER FUNCS
*/
uint8_t bcd_2digits_to_decimal(uint8_t bcd) {
	// bcd &= 0x7F;
	uint8_t tens = (bcd >> 4) & 0x0F;
	uint8_t units = bcd & 0x0F;
	return (tens << 3) + (tens << 1) + units; // equivalent to tens * 10 + units
}

uint8_t decimal_to_bcd_2digits(uint8_t decimal) {
    uint8_t tens = decimal / 10;
    uint8_t units = decimal % 10;
    return (tens << 4) | units;
}

const char* get_weekday_name(uint8_t weekdays) {
    const char *weekday;
    switch (weekdays) {
        case 1:
            weekday = "MON";
            break;
        case 2:
            weekday = "TUE";
            break;
        case 3:
            weekday = "WED";
            break;
        case 4:
            weekday = "THU";
            break;
        case 5:
            weekday = "FRI";
            break;
        case 6:
            weekday = "SAT";
            break;
        case 7:
            weekday = "SUN";
            break;
        default:
            weekday = "UNK";
            break;
    }
    return weekday;
}

/*
 * REGISTERS
*/

// #define BM8563_CTRL1 0x00 // Test mode for manufacturer
// typedef struct {
//     uint8_t not_used_05 : 1;
//     uint8_t not_used_04 : 1;
//     uint8_t not_used_03 : 1;
//     uint8_t testc : 1;
//     uint8_t not_used_02 : 1;
//     uint8_t stop : 1;
//     uint8_t not_used_01 : 1;
//     uint8_t test : 1;
// } bm8563_ctrl1_t;
#define BM8563_CTRL2 0x01
typedef struct {
    uint8_t timer_int_enable : 1;
    uint8_t alarm_int_enable : 1;
    uint8_t timer_flag : 1;
    uint8_t alarm_flag : 1;
    uint8_t int_or_clkout : 1;
    uint8_t not_used_03 : 1;
    uint8_t not_used_02 : 1;
    uint8_t not_used_01 : 1;
} bm8563_ctrl2_t;
#define BM8563_SECONDS 0x02
typedef struct {
    uint8_t seconds_bcd : 7;
    uint8_t vl : 1;
} bm8563_seconds_t;
#define BM8563_MINUTES 0x03
typedef struct {
    uint8_t minutes_bcd : 7;
    uint8_t not_used_01 : 1;
} bm8563_minutes_t;
#define BM8563_HOURS 0x04
typedef struct {
    uint8_t hours_bcd : 6;
    uint8_t not_used_01 : 2;
} bm8563_hours_t;
#define BM8563_DAYS 0x05
typedef struct {
    uint8_t days_bcd : 6;
    uint8_t not_used_01 : 2;
} bm8563_days_t;
#define BM8563_WEEKDAYS 0x06
typedef struct {
    uint8_t weekdays : 3;
    uint8_t not_used_01 : 5;
} bm8563_weekdays_t;
#define BM8563_MONTHS 0x07
typedef struct {
    uint8_t months_bcd : 5;
    uint8_t not_used_01 : 2;
    uint8_t century : 1;
} bm8563_months_t;
#define BM8563_YEARS 0x08
// 0x09-0x0C are alarm registers, not used for this application
// #define BM8563_CLKOUT_CTRL 0x0D
// typedef struct {
//     uint8_t frequency_divider : 2; // 00:32.768kHz 01:1024Hz 10:32Hz 11:1Hz
//     uint8_t not_used_01 : 5;
//     uint8_t clkout_enable : 1;
// } bm8563_clkout_ctrl_t;
// 0x0E-0x0F are timer registers, not used for this application

#endif