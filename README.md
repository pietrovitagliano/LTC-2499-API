# LTC-2499-API

## Getting Started
To get started with this API, follow the steps below.

### Prerequisites
Before you begin, ensure you have the following prerequisites:
- LTC 2499 analog-to-digital converter
- B-L072Z-LRWAN1 microcontroller hardware.
- STM32CubeIDE or similar software to configure the microcontroller.
  
### Installation
Clone this repository to your local machine:

	git clone https://github.com/pietrovitagliano/LTC-2499-API.git

Include the required header files together with their source files in your project:
- stm32l0xx_hal.h
- b_l072z_lrwan1_bus.h
  
Add the API files "ltc_2499_adc.h" and "ltc_2499_adc.c" to your project and include the header one where you need to use its functions.

Now you are ready to use the LTC 2499 API in your project.

# Usage
For a detailed demonstration of how to use the API, refer to "ltc_2499_adc_test.h" and "ltc_2499_adc_test.c" files in this repository. They provide a practical example of how to initialize the LTC 2499 and use it to perform readings effectively.

The test function can also be used to verify the proper operation of the LTC 2499 after it has been connected to the microcontroller.

# License
This project is licensed under the MIT License.
