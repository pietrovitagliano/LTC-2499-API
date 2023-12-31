/*
 * ltc_2499_adc_test.c
 *
 * Source file for the API testing of the LTC 2499 ADC.
 * It contains the code used to test the API written for the LTC2499 ADC.
 *
 * Author: Pietro Vitagliano
 */

/* Includes ------------------------------------------------------------------*/
#include "ltc_2499_adc_test.h"


/**
 * Perform a comprehensive test of the LTC 2499 API.
 *
 * This function tests the LTC 2499 API, initializing the ADC, reading data from it,
 * decoding the data into a voltage value and, eventually, deinitializing the ADC.
 * For each step of the testing, this function logs the results and asserts successful execution.
 *
 * @param adc_i2c_addr The 7-bit I2C address of the LTC2499 ADC.
 * @param channel_mode The channel mode (differential or single-ended) to be used.
 * @param channel_polarity The polarity of the ADC channel.
 * @param channel_selection The channel(s) to be used for reading.
 *
 * @return 0 if the test completes successfully, otherwise, an error code.
 */
int32_t api_test(uint8_t adc_i2c_addr, Channel_Mode channel_mode, Channel_Polarity channel_polarity, Channel_Selection channel_selection){
    /* ----------- LTC 2499 API TESTING START ----------- */
    APP_LOG(TS_ON, VLEVEL_L, "\n\r\n\rADC API TESTING START\n\r\n\r");

    /* Initialize the LTC 2499 ADC */
    int32_t response_code = ADC_Init(adc_i2c_addr, channel_mode, channel_polarity, channel_selection);
    APP_LOG(TS_ON, VLEVEL_L, "ADC Init Status Code: %d\n\n\r", response_code);
    assert(response_code == 0);

    /* Read from the LTC 2499 ADC */
    uint8_t data_buffer[4] = {0};
    response_code = ADC_Read(data_buffer);
    APP_LOG(TS_ON, VLEVEL_L, "ADC Read Status Code: %d\n\r", response_code);
    APP_LOG(TS_ON, VLEVEL_L, "ADC Read Buffer[0]: %d\n\r", data_buffer[0]);
    APP_LOG(TS_ON, VLEVEL_L, "ADC Read Buffer[1]: %d\n\r", data_buffer[1]);
    APP_LOG(TS_ON, VLEVEL_L, "ADC Read Buffer[2]: %d\n\r", data_buffer[2]);
    APP_LOG(TS_ON, VLEVEL_L, "ADC Read Buffer[3]: %d\n\n\r", data_buffer[3]);
    assert(response_code == 0);

    /* Decode the buffer into a voltage value */
    float voltage = ADC_Decode_Voltage(data_buffer, 4);

    /* This cast is necessary because the APP_LOG function lacks support for float values.
    * The multiplication is employed to convert the voltage to millivolts (mV).
    */
    APP_LOG(TS_ON, VLEVEL_L, "ADC Decoded Voltage: %d mV\n\n\r", (int16_t)(voltage * 1000));

    /* Deinitialize the LTC 2499 ADC */
    response_code = ADC_DeInit();
    APP_LOG(TS_ON, VLEVEL_L, "ADC De Init Status Code: %d\n\n\r", response_code);
    assert(response_code == 0);

    APP_LOG(TS_ON, VLEVEL_L, "ADC API TESTING END\n\r\n\r");
    /* ----------- LTC 2499 API TESTING END ----------- */

    return 0;
}
