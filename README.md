# LTC-2499-API

## Getting Started
To get started with this API, follow the steps below.

### Prerequisites
Before you begin, ensure you have the following prerequisites:
- B-L072Z-LRWAN1 microcontroller hardware.
- STM32CubeIDE or similar software to configure the microcontroller.
  
### Installation
Clone this repository to your local machine:

	git clone [https://github.com/your-username/your-repo.git](https://github.com/pietrovitagliano/LTC-2499-API.git)

Include the required header files together with their source files in your project:
- stm32l0xx_hal.h
- b_l072z_lrwan1_bus.h
  
Add the API files (ltc_2499_adc.h and ltc_2499_adc.c) to your project.

Now you are ready to use the LTC 2499 API in your project.

# Usage
For a detailed demonstration of how to use the API, refer to the test.c file in this repository. It provides a practical example of how to initialize the LTC 2499, perform  readings, and decode them effectively.

# License
This project is licensed under the MIT License
