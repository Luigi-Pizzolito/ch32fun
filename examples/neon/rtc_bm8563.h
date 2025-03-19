#ifndef CH32_LIB_RTC_BM8563_H
#define CH32_LIB_RTC_BM8563_H

#include "ch32fun.h"
#include <stdio.h>

#include "bm8563_reg.h"

#include "lib_i2c.h"

// #define DEBUG

#ifdef DEBUG
    #ifndef BYTE_TO_BINARY_PATTERN
        #define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
        #define BYTE_TO_BINARY(byte)  \
        ((byte) & 0x80 ? '1' : '0'), \
        ((byte) & 0x40 ? '1' : '0'), \
        ((byte) & 0x20 ? '1' : '0'), \
        ((byte) & 0x10 ? '1' : '0'), \
        ((byte) & 0x08 ? '1' : '0'), \
        ((byte) & 0x04 ? '1' : '0'), \
        ((byte) & 0x02 ? '1' : '0'), \
        ((byte) & 0x01 ? '1' : '0') 
    #endif
#endif

BM8563StatusTypeDef BM8563_Init() {
    //! note, must start i2c before calling this function

    // Control register
    bm8563_ctrl2_t ctrl;
    ctrl.timer_int_enable = 0;
    ctrl.alarm_int_enable = 0;
    ctrl.timer_flag = 0;
    ctrl.alarm_flag = 0;
    ctrl.int_or_clkout = 0;

    // Write registers
    i2c_err_t err = i2c_write(BM8563_ADDR, BM8563_CTRL2, (uint8_t *)&ctrl, 1);
    if (err != I2C_OK) {
        printf("I2C write failed with error code: %d at %02X/%02X\n", err, BM8563_ADDR, BM8563_CTRL2);
        return BM8563_STATUS_ERROR;
    }

    return BM8563_STATUS_OK;
}

uint8_t BM8563_read_voltage_low() {
    uint8_t buf;
    i2c_read(BM8563_ADDR, BM8563_SECONDS, &buf, 1);
    bm8563_seconds_t *seconds = (bm8563_seconds_t *)&buf;
    return seconds->vl;
}

BM8563StatusTypeDef BM8563_clear_voltage_low() {
    uint8_t buf;
    i2c_err_t err = i2c_read(BM8563_ADDR, BM8563_SECONDS, &buf, 1);
    if (err != I2C_OK) {
        printf("I2C read failed with error code: %d at %02X/%02X\n", err, BM8563_ADDR, BM8563_SECONDS);
        return BM8563_STATUS_ERROR;
    }
    bm8563_seconds_t *seconds = (bm8563_seconds_t *)&buf;
    seconds->vl = 0;
    buf = *(uint8_t *)seconds;
    err = i2c_write(BM8563_ADDR, BM8563_SECONDS, &buf, 1);
    if (err != I2C_OK) {
        printf("I2C write failed with error code: %d at %02X/%02X\n", err, BM8563_ADDR, BM8563_SECONDS);
        return BM8563_STATUS_ERROR;
    }
    return BM8563_STATUS_OK;
}

bm8563_time_t BM8563_read_time() {
    uint8_t buf[3] = {0x00, 0x00, 0x00};
    i2c_err_t err = i2c_read(BM8563_ADDR, BM8563_SECONDS, buf, 3);
    if (err != I2C_OK) {
        printf("I2C read failed with error code: %d\n", err);
        return (bm8563_time_t){0, 0, 0};
    } else {
        bm8563_time_t time;
        time.seconds = bcd_2digits_to_decimal(buf[0] & 0x7F);
        time.minutes = bcd_2digits_to_decimal(buf[1]);
        time.hours = bcd_2digits_to_decimal(buf[2]);

        #ifdef DEBUG
            printf("Read time: %02d:%02d:%02d\n", time.hours, time.minutes, time.seconds);
            printf("Read S: "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(buf[0]));
            printf("Read M: "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(buf[1]));
            printf("Read H: "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(buf[2]));
        #endif
        return time;
    }
}

bm8563_date_t BM8563_read_date() {
    uint8_t buf[4] = {0x00, 0x00, 0x00, 0x00};
    i2c_err_t err = i2c_read(BM8563_ADDR, BM8563_DAYS, buf, 4);
    if (err != I2C_OK) {
        printf("I2C read failed with error code: %d\n", err);
        return (bm8563_date_t){0, 0, 0, 0};
    } else {
        bm8563_days_t *days = (bm8563_days_t *)buf;
        bm8563_weekdays_t *weekdays = (bm8563_weekdays_t *)(buf + 1);
        bm8563_months_t *months = (bm8563_months_t *)(buf + 2);

        bm8563_date_t date;
        date.days = bcd_2digits_to_decimal(days->days_bcd);
        date.weekday = bcd_2digits_to_decimal(weekdays->weekdays);
        date.months = bcd_2digits_to_decimal(months->months_bcd);
        date.years = ( (months->century)? 2100:2000) + bcd_2digits_to_decimal(buf[3]);
        
        #ifdef DEBUG
        printf("Read date: %s %02d/%02d/%04d\n", get_weekday_name(date.weekday), date.days, date.months, date.years);
            printf("Read D: "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(buf[0]));
            printf("Read W: "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(buf[1]));
            printf("Read M: "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(buf[2]));
            printf("Read Y: "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(buf[3]));
        #endif
        return date;
    }
}

BM8563StatusTypeDef BM8563_set_time(uint8_t hours, uint8_t minutes, uint8_t seconds) {
    uint8_t buf[3] = {0x00, 0x00, 0x00};
    uint8_t vl = BM8563_read_voltage_low();
    buf[0] = ( (vl)? 0x80:0x00 ) | decimal_to_bcd_2digits(seconds);
    buf[1] = decimal_to_bcd_2digits(minutes);
    buf[2] = decimal_to_bcd_2digits(hours);
    i2c_err_t err = i2c_write(BM8563_ADDR, BM8563_SECONDS, buf, 3);
    if (err != I2C_OK) {
        printf("I2C write failed with error code: %d at %02X/%02X\n", err, BM8563_ADDR, BM8563_SECONDS);
        return BM8563_STATUS_ERROR;
    }
    printf("Set time: %02d:%02d:%02d\n", hours, minutes, seconds);
    #ifdef DEBUG
        printf("Wrote S: "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(buf[0]));
        printf("Wrote M: "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(buf[1]));
        printf("Wrote H: "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(buf[2]));
    #endif
    return BM8563_STATUS_OK;
}

BM8563StatusTypeDef BM8563_set_date(uint8_t days, uint8_t weekday, uint8_t months, uint16_t years) {
    uint8_t buf[4] = {0x00, 0x00, 0x00, 0x00};
    buf[0] = decimal_to_bcd_2digits(days);
    buf[1] = weekday;
    uint8_t century = (years >= 2100)? 1:0;
    buf[2] = decimal_to_bcd_2digits(months) | (century << 7);
    buf[3] = decimal_to_bcd_2digits(years - ( (century)? 2100:2000 ));

    i2c_err_t err = i2c_write(BM8563_ADDR, BM8563_DAYS, buf, 4);
    if (err != I2C_OK) {
        printf("I2C write failed with error code: %d at %02X/%02X\n", err, BM8563_ADDR, BM8563_DAYS);
        return BM8563_STATUS_ERROR;
    }

    printf("Set date: %s %02d/%02d/%04d\n", get_weekday_name(weekday), days, months, years);
    #ifdef DEBUG
        printf("Wrote D: "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(buf[0]));
        printf("Wrote W: "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(buf[1]));
        printf("Wrote M: "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(buf[2]));
        printf("Wrote Y: "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(buf[3]));
    #endif
    return BM8563_STATUS_OK;
}

void BM8563_print_time() {
    bm8563_time_t time = BM8563_read_time();
    printf("%02d:%02d:%02d\n", time.hours, time.minutes, time.seconds);
}

void BM8563_print_date() {
    bm8563_date_t date = BM8563_read_date();
    printf("%s %02d/%02d/%04d\n", get_weekday_name(date.weekday), date.days, date.months, date.years);
}

#endif