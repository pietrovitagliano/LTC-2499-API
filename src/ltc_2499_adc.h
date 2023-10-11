/*
 * ltc_2499_adc.h
 *
 * Header file for the LTC2499 ADC driver.
 *
 * Author: Pietro Vitagliano
 */


#ifndef LTC_2499_ADC_H
#define LTC_2499_ADC_H

// Includes ------------------------------------------------------------------
#include "stm32l0xx_hal.h"

// Define the maximum timeout, in milliseconds, for read and write operations.
#define MAX_OPERATION_TIMEOUT 5000

// Define the duration, in milliseconds, of a delay that can be used for waiting inside loops.
#define DELAY_DURATION 100

// Define the ADC's number of levels as 2^24, with 24 being its resolution.
#define RESOLUTION_LEVELS 16777216

// Define the reference voltage supplied to the ADC.
#define REFERENCE_VOLTAGE 5.0

// Define the most significant bit (8th) of the first word that has to be written into the input register
#define INPUT_REGISTER_MSB 0b10000000

// Enable bit enumeration (6th bit from right of the first word used for adc initialization)
typedef enum {
	ENABLE_BIT_LOW = 0,
	ENABLE_BIT_HIGH = 0b100000
} Enable_Bit;

// Enumeration of ADC Channel mode (5th bit from right of the first word used for adc initialization)
typedef enum {
    DIFFERENTIAL_MODE = 0,
    SINGLE_ENDED_MODE = 0b10000
} Channel_Mode;

// Enumeration of ADC Channel Polarity (4th bit from right of the first word used for adc initialization)
typedef enum {
    EVEN_POLARITY = 0,
    ODD_POLARITY = 0b1000
} Channel_Polarity;

/* Enumeration of ADC Channels (first 3 bits from the right of the first word used for ADC initialization)
 *
 * In the case of differential mode, this enumeration corresponds to the channel pairs as they are named.
 * In single-ended mode, the selected channel depends on the polarity:
 * for even polarity, the selected channel is even-numbered (0, 2, etc.),
 * otherwise, it's odd-numbered (1, 3, etc.).
 */
typedef enum {
    CHANNEL_0_1 = 0,
    CHANNEL_2_3 = 0b001,
    CHANNEL_4_5 = 0b010,
    CHANNEL_6_7 = 0b011,
    CHANNEL_8_9 = 0b100,
    CHANNEL_10_11 = 0b101,
    CHANNEL_12_13 = 0b110,
    CHANNEL_14_15 = 0b111
} Channel_Selection;

// Enumeration of ADC Status
typedef enum {
	NOT_INITIALIZED,
	INITIALIZED
} ADC_Status;

/* ----------------------- ADC API DECLARATION START ----------------------- */
int32_t ADC_Init(uint8_t adc_i2c_addr, Channel_Mode channel_mode, Channel_Polarity channel_polarity, Channel_Selection channel_selection);
int32_t ADC_DeInit();
int32_t ADC_Read(uint8_t *dataBuffer);
int32_t ADC_Write(uint8_t* data_buffer, uint16_t buffer_length);
float ADC_Decode_Voltage(uint8_t *data_buffer, uint16_t buffer_length);
/* ----------------------- ADC API DECLARATION END ----------------------- */

#endif //LTC_2499_ADC_H
