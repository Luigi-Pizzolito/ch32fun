#ifndef CH32_LIB_LIS2DW12_H
#define CH32_LIB_LIS2DW12_H

#include "ch32fun.h"
#include <stdio.h>

#include "lis2dw12_reg.h"

#include "lib_i2c.h"



/*
 * IMPLEMENTATION
 */
LIS2DW12StatusTypeDef LIS2DW12_Init(void)
{
    //! note, must start i2c before calling this function

    uint8_t who_am_i = 0;
    LIS2DW12StatusTypeDef err = i2c_read(LIS2DW12_ADDR, LIS2DW12_WHO_AM_I, &who_am_i, 1);

    // Read WHO_AM_I register
    if (err != LIS2DW12_STATUS_OK)
    {
        printf("I2C read failed with error code: %d\n", err);
    }
    else
    {
        if (who_am_i != LIS2DW12_WHO_AM_I_CHK)
        {
            printf("LIS2DW12 not found, WHOAMI check failed.\n");
            return LIS2DW12_STATUS_ERROR;
        }
    }

    // Set INT1 to input
    funPinMode(P_INT, GPIO_CFGLR_IN_FLOAT);

    // Control register 1
    lis2dw12_ctrl1_t ctrl1;
    ctrl1.lp_mode   = 0;  // Low power mode 1 (12-bit)
    ctrl1.mode      = 0;  // Low power mode
    ctrl1.odr       = 7;  // 400/200Hz HP/LP

    // Control register 2
    lis2dw12_ctrl2_t ctrl2;
    ctrl2.sim           = 0;
    ctrl2.i2c_disable   = 0;
    ctrl2.if_add_inc    = 1; // Auto-increment register address on read
    ctrl2.bdu           = 1; // Block data update: don't update until both LSB and MSB are read
    ctrl2.cs_pu_disc    = 1; // Disconnect CS pull-up to save pwr
    ctrl2.not_used_01   = 0;
    ctrl2.soft_reset    = 0;
    ctrl2.boot          = 1; // Retrieve trimming params fron non-volatile memory

    // Control register 3
    lis2dw12_ctrl3_t ctrl3;
    ctrl3.slp_mode      = 2; // Single data conversion demand by writing SLP_MODE_1 to 1, disabled
    ctrl3.not_used_01   = 0;
    ctrl3.h_lactive     = 0; // Interrupt active high
    ctrl3.lir           = 1; // Latched interrupts
    ctrl3.pp_od         = 0; // Push-pull output interrupts
    ctrl3.st            = 0; // Self-test disabled

    // Control register 4 (INT1 CTRL)
    lis2dw12_ctrl4_int1_pad_ctrl_t ctrl4;
    ctrl4.int1_drdy         = 0; // Int. on data ready
    ctrl4.int1_fth          = 0; // Int. on FIFO threshold
    ctrl4.int1_diff5        = 0; // Int. on FIFO full
    ctrl4.int1_tap          = 1; // Int. on double-tap
    ctrl4.int1_ff           = 0; // Int. on free-fall
    ctrl4.int1_wu           = 0; // Int. on wake-up recognition
    ctrl4.int1_single_tap   = 1; // Int. on single-tap
    ctrl4.int1_6d           = 1; // Int. on 6D recognition (orientation change)

    // Control register 5 (INT2 CTRL)
    lis2dw12_ctrl5_int2_pad_ctrl_t ctrl5;
    ctrl5.int2_drdy         = 0; // Int. on data ready
    ctrl5.int2_fth          = 0; // Int. on FIFO threshold
    ctrl5.int2_diff5        = 0; // Int. on FIFO full
    ctrl5.int2_ovr          = 0; // Int. on FIFO overrun
    ctrl5.int2_drdy_t       = 0; // Int. on temperature ready
    ctrl5.int2_boot         = 0; // Int. synced to boot state
    ctrl5.int2_sleep_chg    = 0; // Int. on sleep status change
    ctrl5.int2_sleep_state  = 0; // Int. synced sleep status

    // Control register 6
    lis2dw12_ctrl6_t ctrl6;
    ctrl6.not_used_01   = 0;
    ctrl6.low_noise     = 1; // Low noise mode enabled
    ctrl6.fds           = 0; // Low-pass filter enabled
    ctrl6.fs            = 0; // Full-scale selection: +/-2g
    ctrl6.bw_filt       = 0; // Bandwidth selection: ODR/2

    // FIFO Control register
    lis2dw12_fifo_ctrl_t fifo_ctrl;
    fifo_ctrl.fmode     = 0; // FIFO mode: Bypass (off)
    fifo_ctrl.fth       = 0; // FIFO threshold: 0

    // Control register 7
    lis2dw12_ctrl_reg7_t ctrl7;
    ctrl7.drdy_pulsed       = 1; // Data-ready int. pulsed
    ctrl7.int2_on_int1      = 1; // INT2 routed to INT1
    ctrl7.interrupts_enable = 1; // Interrupts enabled
    ctrl7.usr_off_on_out    = 0; // User offset on output disabled
    ctrl7.usr_off_on_wu     = 0; // User offset on wake-up disabled
    ctrl7.usr_off_w         = 0; // User offset weight 0:977ug/LSB 1:15.6mg/LSB
    ctrl7.hp_ref_mode       = 0; // High-pass filter reference mode (off)
    ctrl7.lpass_on6d        = 0; // Low-pass filter on 6D 0:ODR/2 1:LPF2

    // Tap configuration
    // Tap X register
    lis2dw12_tap_ths_x_t tap_x;
    tap_x.tap_thsx  = 15; // threshold for x-axis tap detection (unused)
    tap_x._6d_ths   = 2; // 60deg threshold for 4D/6D detection
    tap_x._4d_en    = 0; // 4D detection enabled (portrait/landscape, face up/down) 0:6D 1:4D
    // Tap Y register
    lis2dw12_tap_ths_y_t tap_y;
    tap_y.tap_thsy  = 15; // threshold for y-axis tap detection (unused)
    tap_y.tap_prior = 7;  // priority for tap detection Z>Y>X
    // Tap Z register
    lis2dw12_tap_ths_z_t tap_z;
    tap_z.tap_thsz  = 12; // threshold for z-axis tap detection
    tap_z.tap_z_en  = 1;  // enable z-axis tap detection
    tap_z.tap_y_en  = 0;  // disable y-axis tap detection
    tap_z.tap_x_en  = 0;  // disable x-axis tap detection
    // Wake-up configuration
    lis2dw12_wake_up_ths_t wake_up0;
    wake_up0.wk_ths             = 31; // threshold for wake-up detection (LSB=1/64FS)
    wake_up0.sleep_on           = 0; // enable auto sleep mode
    wake_up0.single_double_tap  = 1; // enable single and double tap recognition
    lis2dw12_wake_up_dur_t wake_up1;
    wake_up1.sleep_dur      = 2; // duration to go in sleep mode (LSB=512*1/ODR + 16*1/ODR)
    wake_up1.stationary     = 1; // enable stationary detection
    wake_up1.wake_dur       = 0; // duration to wake up (LSB=1/ODR)
    wake_up1.ff_dur         = 0;
    lis2dw12_int_dur_t int_dur;
    int_dur.shock   = 0x0;
    int_dur.quiet   = 0x0;
    int_dur.latency = 0x0;

    // Write registers
    i2c_write(LIS2DW12_ADDR, LIS2DW12_CTRL1, (uint8_t *)&ctrl1, 1);
    i2c_write(LIS2DW12_ADDR, LIS2DW12_CTRL2, (uint8_t *)&ctrl2, 1);
    i2c_write(LIS2DW12_ADDR, LIS2DW12_CTRL3, (uint8_t *)&ctrl3, 1);
    i2c_write(LIS2DW12_ADDR, LIS2DW12_CTRL4_INT1_PAD_CTRL, (uint8_t *)&ctrl4, 1);
    i2c_write(LIS2DW12_ADDR, LIS2DW12_CTRL5_INT2_PAD_CTRL, (uint8_t *)&ctrl5, 1);
    i2c_write(LIS2DW12_ADDR, LIS2DW12_CTRL6, (uint8_t *)&ctrl6, 1);
    i2c_write(LIS2DW12_ADDR, LIS2DW12_FIFO_CTRL, (uint8_t *)&fifo_ctrl, 1);
    i2c_write(LIS2DW12_ADDR, LIS2DW12_TAP_THS_X, (uint8_t *)&tap_x, 1);
    i2c_write(LIS2DW12_ADDR, LIS2DW12_TAP_THS_Y, (uint8_t *)&tap_y, 1);
    i2c_write(LIS2DW12_ADDR, LIS2DW12_TAP_THS_Z, (uint8_t *)&tap_z, 1);
    i2c_write(LIS2DW12_ADDR, LIS2DW12_INT_DR, (uint8_t *)&int_dur, 1);
    i2c_write(LIS2DW12_ADDR, LIS2DW12_WAKE_P_THS, (uint8_t *)&wake_up0, 1);
    i2c_write(LIS2DW12_ADDR, LIS2DW12_WAKE_P_DR, (uint8_t *)&wake_up1, 1);
    i2c_write(LIS2DW12_ADDR, LIS2DW12_CTRL_REG7, (uint8_t *)&ctrl7, 1);

    return LIS2DW12_STATUS_OK;
}

#endif