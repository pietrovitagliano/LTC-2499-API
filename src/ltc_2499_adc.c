/*
 * ltc_2499_adc.c
 *
 * Source file for the API of the LTC 2499 ADC.
 * It contains the functions for initializing, reading and writing to the LTC 2499 ADC.
 *
 * Author: Pietro Vitagliano
 */


// Includes ------------------------------------------------------------------
#include "ltc_2499_adc.h"
#include "b_l072z_lrwan1_bus.h"

// Global Variables ------------------------------------------------------------------
ADC_Status adc_status = NOT_INITIALIZED;	// Initial ADC status
uint8_t adc_i2c_address = 0;				// Used to store the 7-bit I2C address of the LTC2499 ADC


/**
 * Initialize the ADC with specified parameters.
 *
 * This function initializes the ADC with user-defined parameters, including the I2C address,
 * channel mode (differential or single-ended), channel polarity, and channel selection.
 * It configures the ADC according to the specified settings, enabling it for subsequent read operations.
 *
 * @param adc_i2c_addr The 7-bit I2C address of the LTC2499 ADC.
 * @param channel_mode The channel mode (differential or single-ended) to be used.
 * @param channel_polarity The polarity of the ADC channel.
 * @param channel_selection The channel(s) to be used for reading.
 *
 * @retval BSP status code indicating the success or failure of the initialization:
 *         		BSP_ERROR_INIT if the ADC has already been initialized,
 *         		or other appropriate status codes.
 */
int32_t ADC_Init(uint8_t adc_i2c_addr, Channel_Mode channel_mode, Channel_Polarity channel_polarity, Channel_Selection channel_selection) {
	int32_t response_code;

    // Initialize the I2C interface if the ADC has not already been initialized
	if(adc_status == NOT_INITIALIZED && (response_code = BSP_I2C1_Init()) != BSP_ERROR_NONE){
		return response_code;
	}

    /*
     * Set the input register according to ADC datasheet
     * This register is 16 bits long but only 13 are used (the most significant ones), split in two 8 bit words (8 + 5)
     * For the first words all the 8 bits are needed, while the second just requires the first 5
     * (thus, the last 3 are set to 0):
     *
     * 		- The first word has the first 3 bits set to 101 (the first 1 from the left
     * 		  is defined as INPUT_REGISTER_MSB, while the one from the right is represented
     * 		  as ENABLE_BIT_HIGH and it enables the conversion);
     * 		  the 4th bit from the right is used to set the input as differential or single-ended;
     * 		  the 5th bit from the right is used to set the polarity;
     * 		  the less significant 3 bits are used to choose the channel(s).
     *
     * 		- The second word can be used to configure optional parameters,
     * 		  which aren't necessary in this case.
     */
    uint8_t first_word = INPUT_REGISTER_MSB | ENABLE_BIT_HIGH | channel_mode | channel_polarity | channel_selection;
    uint8_t init_data[] = {first_word, 0};

    // Initialize the address of the ADC
    adc_i2c_address = adc_i2c_addr;

    /*
     * Write init_data buffer into the write register to initialize ADC,
     * in order to make it able to perform read operations with the given mode, polarity and channels.
     */
    response_code = ADC_Write(init_data, 2);

	// If the Write operation has been done correctly, initialize the status variable
	if(response_code == BSP_ERROR_NONE)
	{
		adc_status = INITIALIZED;
	}
	else
	{
        adc_i2c_address = 0;
	}

	return response_code;
}

/**
 * Deinitialize the ADC and reset its configuration.
 *
 * This function deinitializes the ADC, clears its registers, and resets its configuration.
 * It is used to prepare the ADC for a fresh initialization or shutdown.
 *
 * @retval BSP status code indicating the success or failure of the deinitialization:
 *         		BSP_ERROR_INIT if the ADC was not previously initialized,
 *         		or other appropriate status codes.
 */
int32_t ADC_DeInit() {
    if(adc_status == INITIALIZED){
    	// Set the values used to clear the ADC's registers
        uint8_t reset_data[2] = {0};

    	// Clear the ADC's input register
        int32_t input_reg_response_code = ADC_Write(reset_data, 2);

        if(input_reg_response_code != BSP_ERROR_NONE){
        	return input_reg_response_code;
        }
        else
        {
            adc_status = NOT_INITIALIZED;
            adc_i2c_address = 0;

            // De-initialize the I2C interface and return the operation status code
        	return BSP_I2C1_DeInit();
        }
    }

    return BSP_ERROR_NO_INIT;
}

/**
 * Read data from the ADC channel(s).
 *
 * This function reads a buffer of data from the ADC's read register, which is
 * 32 bits long. The data is stored in the provided buffer. The buffer must be
 * at least 4 bytes long to accommodate the 32-bit data.
 *
 * @param data_buffer The buffer used to store the data read from the read register.
 *                    It should be a minimum of 4 bytes in length.
 *
 * @retval BSP status code indicating the success or failure of the read operation:
 *         		BSP_ERROR_WRONG_PARAM if the provided data_buffer is invalid,
 *         		BSP_ERROR_NO_INIT if the ADC has not been initialized,
 *         		or other appropriate status codes.
 */
int32_t ADC_Read(uint8_t* data_buffer) {
	// Check if the dataBuffer is valid
    if (data_buffer == NULL || sizeof(data_buffer) < 4 * sizeof(data_buffer[0])) {
        return BSP_ERROR_WRONG_PARAM;
    }

    // Check if the ADC has already been initialized. If not, return error status code
    if(adc_status == NOT_INITIALIZED)
    {
    	return BSP_ERROR_NO_INIT;
    }
    else
    {
    	// Try to read from the ADC for the max time interval allowed
    	// and return the status code
    	int32_t response_code;
    	int repeat_operation = 1, time_elapsed = 0;

    	while(repeat_operation){
    		response_code = BSP_I2C1_Recv(adc_i2c_address << 1, data_buffer, 4);

    		// Check if the loop has to go on
    		repeat_operation = response_code != BSP_ERROR_NONE && time_elapsed < MAX_OPERATION_TIMEOUT;

    		if(repeat_operation)
    		{
            	// Before repeating the read operation, wait for a little delay
    			HAL_Delay(DELAY_DURATION);
            	time_elapsed += DELAY_DURATION;
    		}
    	}

        return response_code;
    }
}

/**
 * Write data to the ADC channel(s).
 *
 * This function writes a buffer of data into the ADC's write register, which can
 * be either 16-bit or 8-bit wide depending on the buffer length. The buffer should
 * contain the data to be written. The second 8-bit word in the buffer is optional.
 *
 * @param data_buffer The buffer containing the data to be written into the write register.
 * @param buffer_length The number of bytes to write (1 or 2).
 *                     For 16-bit write, provide 2 bytes; for 8-bit write, provide 1 byte.
 *
 * @retval BSP status code indicating the success or failure of the write operation.
 */
int32_t ADC_Write(uint8_t* data_buffer, uint16_t buffer_length) {
    // Check if the data_buffer is valid
    if (data_buffer == NULL || (buffer_length != 1 && buffer_length != 2)) {
        return BSP_ERROR_WRONG_PARAM;
    }

    // Try to write to the ADC for the max time interval allowed
	// and return the status code
	int32_t response_code;
	int repeat_operation = 1, time_elapsed = 0;

	while (repeat_operation) {
		response_code = BSP_I2C1_Send(adc_i2c_address << 1, data_buffer, buffer_length);

		// Check if the loop has to go on
		repeat_operation = response_code != BSP_ERROR_NONE && time_elapsed < MAX_OPERATION_TIMEOUT;

		if (repeat_operation) {
			// Before repeating the write operation, wait for a little delay
			HAL_Delay(DELAY_DURATION);
			time_elapsed += DELAY_DURATION;
		}
	}

	return response_code;
}

/**
 * Decode a buffer of data from the ADC into a voltage
 *
 * This function deciphers a data buffer obtained from the ADC and provides the
 * decoded value as a 32-bit signed floating-point number, that represents a voltage.
 * The data buffer is expected to be in little-endian format.
 *
 * @param data_buffer The buffer containing the data to be decoded.
 * @param buffer_length The number of bytes in the data buffer.
 *
 * @retval The decoded 32-bit signed floating-point value.
 */
float ADC_Decode_Voltage(uint8_t *data_buffer, uint16_t buffer_length) {
    // Declare a variable used to store all the bytes of the buffer
    uint32_t buffer_value = 0;

    // Scroll the buffer from the end, since ARM uses little-endian encoding
    for (int buffer_index = buffer_length - 1, shift_multiplier = 0; buffer_index >= 0; buffer_index--, shift_multiplier++) {
        buffer_value |= data_buffer[buffer_index] << (8 * shift_multiplier);
    }

	/* Decode the voltage, considering the following information:
	 *
	 * 	- 0x7FFFFFFF represents a bitmask where all bits except the Most Significant Bit (MSB) are set to 1.
	 *    Therefore, the bitwise AND operation is used to clear the MSB.
	 *
	 *  - The right shift operation is applied to the word, excluding the first 6 bits (according to the datasheet),
	 *    as these bits do not conform to the two's complement representation and their information is useless.
	 *
	 *	- The voltage is computed as an unsigned integer multiplied by half of the reference voltage used to supply
	 *	  the ADC (since the readable voltage range goes from 0 to half of the reference voltage),
	 *    divided by the maximum resolution level of the ADC (RESOLUTION_LEVELS - 1).
	 *
	 */
    buffer_value = (buffer_value & 0x7FFFFFFF) >> 6;

    return ((float)buffer_value) * REFERENCE_VOLTAGE * 0.5 / (RESOLUTION_LEVELS - 1);
}
