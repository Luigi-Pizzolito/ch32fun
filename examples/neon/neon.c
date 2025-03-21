// #define SYSTEM_CORE_CLOCK 48000000
#include "ch32fun.h"
#include <stdio.h>

#include "lib_i2c.h"

#define HVCTRL	PD2

#define DIG1 	PD5
#define DIG2 	PD7
#define DIG3	PA1
#define DIG4	PA2
#define DIG5	PD0
#define DIG6	PC0
#define DIG7	PC3
#define DIG8	PC4
#define DIG9	PC5
#define DIG10	PC6
#define DIG11	PC7
#define DIG12	PD3

#define P_SDA	PC1
#define P_SCL	PC2
#define P_INT	PD6

#include "lis2dw12.h"
#include "rtc_bm8563.h"

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

// PWM HVCTRL Init
void t1pwm_init(void) {
	// Enable TIM1 and output pin PD2
	RCC->APB2PCENR |= RCC_APB2Periph_GPIOD | RCC_APB2Periph_TIM1;
	// GPIOD-> CFGLR &= ~(0xf << (4 * 2));
	GPIOD-> CFGLR |= (GPIO_Speed_50MHz | GPIO_CNF_OUT_PP_AF) << (4 * 2);
	// Reset TIM1 to initialise all registers
	RCC->APB2PRSTR |= RCC_APB2Periph_TIM1;
	RCC->APB2PRSTR &= ~RCC_APB2Periph_TIM1;
	// Set prescaler to 0
	TIM1->PSC = 0x0000;
	// Set auto reload to 255, to define PWM period
	TIM1->ATRLR = 140;
	// Reload the registers immediately
	TIM1->SWEVGR |= TIM_UG;
	// Enable TIM1_CH1 channel output, positive polarity
	TIM1->CCER |= TIM_CC1E | TIM_CC1NP;
	// COnfigure the TIM1_CH1 channel as PWM1 mode
	TIM1->CHCTLR1 |= TIM_OC1M_2 | TIM_OC1M_1;
	// Set the duty cycle (48%)
	TIM1->CH1CVR = 48;
	// Enable Outputs
	TIM1->BDTR |= TIM_MOE;
	// Enable TIM1
	TIM1->CTLR1 |= TIM_CEN;
}
// PWM HVCTRL Set
void t1pwm_set_dc(uint16_t dc) {
	// Ensure range is within 0-255
	if (dc > TIM1->ATRLR) {
		dc = TIM1->ATRLR;
	}
	// Set the duty cycle
	TIM1->CH1CVR = dc;
}

void print_i2c_address(uint8_t address) {
	printf("I2C device found at address: 0x%02X\n", address);
}

// void i2c_single_read(const uint8_t addr, const uint8_t reg) {
// 	uint8_t buf[1] = {0x00};
// 	i2c_err_t err = i2c_read(addr, reg, buf, 1);
// 	if (err != I2C_OK) {
// 		printf("I2C read failed with error code: %d\n", err);
// 	} else {
// 		printf("Read data: 0x%02X/0x%02X: 0x%02X\n", addr, reg, buf[0]);
// 	}
// }

void i2c_single_read_bin(const uint8_t addr, const uint8_t reg) {
	uint8_t buf[1] = {0x00};
	i2c_err_t err = i2c_read(addr, reg, buf, 1);
	if (err != I2C_OK) {
		printf("I2C read failed with error code: %d\n", err);
	} else {
		printf("Read data: 0x%02X/0x%02X: 0b"BYTE_TO_BINARY_PATTERN"\n", addr, reg, BYTE_TO_BINARY(buf[0]));
	}
}

int main()
{
	SystemInit();

	// Enable GPIOs
	// RCC->APB2PCENR |= RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA;
	funGpioInitAll();

	funPinMode(HVCTRL, GPIO_CFGLR_OUT_10Mhz_PP);
	funPinMode(DIG1, GPIO_CFGLR_OUT_10Mhz_PP);
	funPinMode(DIG2, GPIO_CFGLR_OUT_10Mhz_PP);
	funPinMode(DIG3, GPIO_CFGLR_OUT_10Mhz_PP);
	funPinMode(DIG4, GPIO_CFGLR_OUT_10Mhz_PP);
	funPinMode(DIG5, GPIO_CFGLR_OUT_10Mhz_PP);
	funPinMode(DIG6, GPIO_CFGLR_OUT_10Mhz_PP);
	funPinMode(DIG7, GPIO_CFGLR_OUT_10Mhz_PP);
	funPinMode(DIG8, GPIO_CFGLR_OUT_10Mhz_PP);
	funPinMode(DIG9, GPIO_CFGLR_OUT_10Mhz_PP);
	funPinMode(DIG10, GPIO_CFGLR_OUT_10Mhz_PP);
	funPinMode(DIG11, GPIO_CFGLR_OUT_10Mhz_PP);
	funPinMode(DIG12, GPIO_CFGLR_OUT_10Mhz_PP);

	printf("Hi!\n");
	funDigitalWrite(DIG1, FUN_LOW);
	funDigitalWrite(DIG2, FUN_LOW);
	funDigitalWrite(DIG3, FUN_LOW);
	funDigitalWrite(DIG4, FUN_LOW);
	funDigitalWrite(DIG5, FUN_LOW);
	funDigitalWrite(DIG6, FUN_LOW);
	funDigitalWrite(DIG7, FUN_LOW);
	funDigitalWrite(DIG8, FUN_LOW);
	funDigitalWrite(DIG9, FUN_LOW);
	funDigitalWrite(DIG10, FUN_LOW);
	funDigitalWrite(DIG11, FUN_LOW);
	funDigitalWrite(DIG12, FUN_LOW);


	printf("I2C Init\n");
	i2c_err_t err = i2c_init(I2C_CLK_100KHZ);
	if (err != I2C_OK) {
		printf("I2C init failed with error code: %d\n", err);
	}
	printf("Scanning for I2C devices...\n");
	i2c_scan(&print_i2c_address);

	printf("Initialising BM8563 RTC...\n");
	BM8563_Init();
	BM8563_set_date(10, 1, 3, 2025);
	BM8563_set_time(20, 30, 0);
	BM8563_print_time();
	BM8563_print_date();

	printf("Initialising LIS2DW12 accelerometer...\n");
	LIS2DW12_Init();

	// uint8_t buf[2] = {0x00, 0x00};
	// i2c_err_t err2 = i2c_read(0x18, 0x0D, buf, 2);
	// if (err2 != I2C_OK) {
	// 	printf("I2C read failed with error code: %d\n", err2);
	// } else {
	// 	printf("Read data: 0x%02X 0x%02X\n", buf[0], buf[1]);
	// 	uint16_t temp = buf[1] << 8 | buf[0];
	// 	int16_t temperature = (int16_t)temp;
	// 	// temperature >>= 4;
	// 	printf("Temperature: %d\n", temperature);
	// }

	// i2c_single_read(0x18, 0x0F);
	// i2c_single_read(0x18, 0x26);
	// i2c_single_read_bin(0x18, 0x27);

	printf("Starting flyback converter...\n");
	t1pwm_init();
	t1pwm_set_dc(48);

	uint8_t pins[] = {DIG1, DIG2, DIG3, DIG4, DIG5, DIG6, DIG7, DIG8, DIG9, DIG10, DIG11, DIG12};
    int numPins = sizeof(pins) / sizeof(pins[0]);
	printf("Starting animation...\n");
	while (1) {

		if (funDigitalRead(P_INT) == FUN_HIGH) {
			
			printf("Interrupt stat:\n");
			// i2c_single_read_bin(0x18, 0x27);
			printf("TAP: ");
			// i2c_single_read_bin(0x18, 0x39);
			uint8_t tap = 0x00;
			i2c_read(0x18, 0x39, &tap, 1);
			printf("0b"BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(tap));
			printf("WAKE: ");
			i2c_single_read_bin(0x18, 0x38);
			printf("6D: ");
			uint8_t orin = 0x00;
			i2c_read(0x18, 0x3A, &orin, 1);
			printf("0b"BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(orin));
			printf("ALL_SRC: ");
			i2c_single_read_bin(0x18, 0x3B);
			
			printf("\n");

			BM8563_print_time();
			// BM8563_print_date();

			// if ( (orin && 0b01000000) < 64 ) {
				// continue;
			// } else {
				switch (orin) {
					case 0b01100000:
						printf("Z DOWN\n");
						break;
					case 0b01010000:
						printf("Z UP\n");
						break;
					default:
						continue;
						break;
				}
			// }

			tap &= 0b00010000;
			if (tap != 0x00) {
				// Double
				printf("DOUBLE TAP\n");
				for (int i = 11; i > 0; i--) {
					funDigitalWrite(pins[i], FUN_HIGH);
					Delay_Ms(50);
					funDigitalWrite(pins[i], FUN_LOW);
				}
			} else {
				// Single
				printf("SINGLE TAP\n");
				for (int i = 0; i < numPins; i++) {
					funDigitalWrite(pins[i], FUN_HIGH);
					Delay_Ms(50);
					funDigitalWrite(pins[i], FUN_LOW);
				}
			}

			
		}

		// rtc_read_time();

		// uint8_t temp = 0x00;
		// i2c_err_t err = i2c_read(0x18, 0x26, &temp, 1);
		// if (err != I2C_OK) {
		// 	printf("I2C read failed with error code: %d\n", err);
		// } else {
		// 	int8_t temperature = (int8_t)temp + 25;
		// 	printf("Temperature: %dC\n", temperature);
		// }
		// // i2c_single_read_bin(0x18, 0x26);
		// i2c_single_read_bin(0x18, 0x27);
	}
}