/*
 * ltc_2499_adc.h
 *
 * Header file for the API testing of the LTC 2499 ADC.
 *
 * Author: Pietro Vitagliano
 */

#ifndef LTC_2499_ADC_TEST_H
#define LTC_2499_ADC_TEST_H

#include "sys_app.h"
#include "ltc_2499_adc.h"


// Function used to test the API
int32_t api_test(uint8_t adc_i2c_addr, Channel_Mode channel_mode, Channel_Polarity channel_polarity, Channel_Selection channel_selection);

#endif /* LTC_2499_ADC_TEST_H */
