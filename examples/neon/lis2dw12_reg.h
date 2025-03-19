#ifndef CH32_LIB_LIS2DW12_REG_H
#define CH32_LIB_LIS2DW12_REG_H

#include "ch32fun.h"

/*
 * OPERATION
 */
#define LIS2DW12_ADDR 0x18
#define LIS2DW12_WHO_AM_I_CHK 0x44

typedef enum
{
	LIS2DW12_STATUS_OK = 0,
	LIS2DW12_STATUS_ERROR
} LIS2DW12StatusTypeDef;

// typedef struct
// {
// 	unsigned int WakeUpStatus : 1;
// 	unsigned int D6DOrientationStatus : 1;
// 	unsigned int SleepStatus : 1;
// } LIS2DW12_Event_Status_t;

// typedef enum
// {
// 	LIS2DW12_HIGH_PERFORMANCE_MODE,
// 	LIS2DW12_LOW_POWER_MODE4,
// 	LIS2DW12_LOW_POWER_MODE3,
// 	LIS2DW12_LOW_POWER_MODE2,
// 	LIS2DW12_LOW_POWER_MODE1
// } LIS2DW12_Operating_Mode_t;

// typedef enum
// {
// 	LIS2DW12_LOW_NOISE_DISABLE,
// 	LIS2DW12_LOW_NOISE_ENABLE
// } LIS2DW12_Low_Noise_t;

/*
 * REGISTERS
 */
#define LIS2DW12_OT_T_L 0x0D
#define LIS2DW12_OT_T_H 0x0E
#define LIS2DW12_WHO_AM_I 0x0F
#define LIS2DW12_CTRL1 0x20
typedef struct
{
	uint8_t lp_mode : 2;
	uint8_t mode : 2;
	uint8_t odr : 4;
} lis2dw12_ctrl1_t;

#define LIS2DW12_CTRL2 0x21
typedef struct
{
	uint8_t sim : 1;
	uint8_t i2c_disable : 1;
	uint8_t if_add_inc : 1;
	uint8_t bdu : 1;
	uint8_t cs_pu_disc : 1;
	uint8_t not_used_01 : 1;
	uint8_t soft_reset : 1;
	uint8_t boot : 1;
} lis2dw12_ctrl2_t;

#define LIS2DW12_CTRL3 0x22
typedef struct
{
	uint8_t slp_mode : 2; /* slp_mode_sel + slp_mode_1 */
	uint8_t not_used_01 : 1;
	uint8_t h_lactive : 1;
	uint8_t lir : 1;
	uint8_t pp_od : 1;
	uint8_t st : 2;
} lis2dw12_ctrl3_t;

#define LIS2DW12_CTRL4_INT1_PAD_CTRL 0x23
typedef struct
{
	uint8_t int1_drdy : 1;
	uint8_t int1_fth : 1;
	uint8_t int1_diff5 : 1;
	uint8_t int1_tap : 1;
	uint8_t int1_ff : 1;
	uint8_t int1_wu : 1;
	uint8_t int1_single_tap : 1;
	uint8_t int1_6d : 1;
} lis2dw12_ctrl4_int1_pad_ctrl_t;

#define LIS2DW12_CTRL5_INT2_PAD_CTRL 0x24
typedef struct
{
	uint8_t int2_drdy : 1;
	uint8_t int2_fth : 1;
	uint8_t int2_diff5 : 1;
	uint8_t int2_ovr : 1;
	uint8_t int2_drdy_t : 1;
	uint8_t int2_boot : 1;
	uint8_t int2_sleep_chg : 1;
	uint8_t int2_sleep_state : 1;
} lis2dw12_ctrl5_int2_pad_ctrl_t;

#define LIS2DW12_CTRL6 0x25
typedef struct
{
	uint8_t not_used_01 : 2;
	uint8_t low_noise : 1;
	uint8_t fds : 1;
	uint8_t fs : 2;
	uint8_t bw_filt : 2;
} lis2dw12_ctrl6_t;

#define LIS2DW12_OT_T 0x26
#define LIS2DW12_STATS 0x27
typedef struct
{
	uint8_t drdy : 1;
	uint8_t ff_ia : 1;
	uint8_t _6d_ia : 1;
	uint8_t single_tap : 1;
	uint8_t double_tap : 1;
	uint8_t sleep_state : 1;
	uint8_t wu_ia : 1;
	uint8_t fifo_ths : 1;
} lis2dw12_status_t;

#define LIS2DW12_OT_X_L 0x28
#define LIS2DW12_OT_X_H 0x29
#define LIS2DW12_OT_Y_L 0x2A
#define LIS2DW12_OT_Y_H 0x2B
#define LIS2DW12_OT_Z_L 0x2C
#define LIS2DW12_OT_Z_H 0x2D
#define LIS2DW12_FIFO_CTRL 0x2E
typedef struct
{
	uint8_t fth : 5;
	uint8_t fmode : 3;
} lis2dw12_fifo_ctrl_t;

#define LIS2DW12_FIFO_SAMPLES 0x2F
typedef struct
{
	uint8_t diff : 6;
	uint8_t fifo_ovr : 1;
	uint8_t fifo_fth : 1;
} lis2dw12_fifo_samples_t;

#define LIS2DW12_TAP_THS_X 0x30
typedef struct
{
	uint8_t tap_thsx : 5;
	uint8_t _6d_ths : 2;
	uint8_t _4d_en : 1;
} lis2dw12_tap_ths_x_t;

#define LIS2DW12_TAP_THS_Y 0x31
typedef struct
{
	uint8_t tap_thsy : 5;
	uint8_t tap_prior : 3;
} lis2dw12_tap_ths_y_t;

#define LIS2DW12_TAP_THS_Z 0x32
typedef struct
{
	uint8_t tap_thsz : 5;
	uint8_t tap_z_en : 1;
	uint8_t tap_y_en : 1;
	uint8_t tap_x_en : 1;
} lis2dw12_tap_ths_z_t;

#define LIS2DW12_INT_DR 0x33
typedef struct
{
	uint8_t shock : 2;
	uint8_t quiet : 2;
	uint8_t latency : 4;
} lis2dw12_int_dur_t;

#define LIS2DW12_WAKE_P_THS 0x34
typedef struct
{
	uint8_t wk_ths : 6;
	uint8_t sleep_on : 1;
	uint8_t single_double_tap : 1;
} lis2dw12_wake_up_ths_t;

#define LIS2DW12_WAKE_P_DR 0x35
typedef struct
{
	uint8_t sleep_dur : 4;
	uint8_t stationary : 1;
	uint8_t wake_dur : 2;
	uint8_t ff_dur : 1;
} lis2dw12_wake_up_dur_t;

#define LIS2DW12_FREE_FALL 0x36
typedef struct
{
	uint8_t ff_ths : 3;
	uint8_t ff_dur : 5;
} lis2dw12_free_fall_t;

#define LIS2DW12_STATS_DP 0x37
typedef struct
{
	uint8_t drdy : 1;
	uint8_t ff_ia : 1;
	uint8_t _6d_ia : 1;
	uint8_t single_tap : 1;
	uint8_t double_tap : 1;
	uint8_t sleep_state_ia : 1;
	uint8_t drdy_t : 1;
	uint8_t ovr : 1;
} lis2dw12_status_dup_t;

#define LIS2DW12_WAKE_P_SRC 0x38
typedef struct
{
	uint8_t z_wu : 1;
	uint8_t y_wu : 1;
	uint8_t x_wu : 1;
	uint8_t wu_ia : 1;
	uint8_t sleep_state_ia : 1;
	uint8_t ff_ia : 1;
	uint8_t not_used_01 : 2;
} lis2dw12_wake_up_src_t;

#define LIS2DW12_TAP_SRC 0x39
typedef struct
{
	uint8_t z_tap : 1;
	uint8_t y_tap : 1;
	uint8_t x_tap : 1;
	uint8_t tap_sign : 1;
	uint8_t double_tap : 1;
	uint8_t single_tap : 1;
	uint8_t tap_ia : 1;
	uint8_t not_used_01 : 1;
} lis2dw12_tap_src_t;

#define LIS2DW12_SIXD_SRC 0x3A
typedef struct
{
	uint8_t xl : 1;
	uint8_t xh : 1;
	uint8_t yl : 1;
	uint8_t yh : 1;
	uint8_t zl : 1;
	uint8_t zh : 1;
	uint8_t _6d_ia : 1;
	uint8_t not_used_01 : 1;
} lis2dw12_sixd_src_t;

#define LIS2DW12_ALL_INT_SRC 0x3B
typedef struct
{
	uint8_t ff_ia : 1;
	uint8_t wu_ia : 1;
	uint8_t single_tap : 1;
	uint8_t double_tap : 1;
	uint8_t _6d_ia : 1;
	uint8_t sleep_change_ia : 1;
	uint8_t not_used_01 : 2;
} lis2dw12_all_int_src_t;

#define LIS2DW12_X_OFS_SR 0x3C
#define LIS2DW12_Y_OFS_SR 0x3D
#define LIS2DW12_Z_OFS_SR 0x3E
#define LIS2DW12_CTRL_REG7 0x3F
typedef struct
{
	uint8_t lpass_on6d : 1;
	uint8_t hp_ref_mode : 1;
	uint8_t usr_off_w : 1;
	uint8_t usr_off_on_wu : 1;
	uint8_t usr_off_on_out : 1;
	uint8_t interrupts_enable : 1;
	uint8_t int2_on_int1 : 1;
	uint8_t drdy_pulsed : 1;
} lis2dw12_ctrl_reg7_t;


#endif