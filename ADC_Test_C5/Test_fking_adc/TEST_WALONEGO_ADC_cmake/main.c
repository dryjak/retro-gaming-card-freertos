/**
  ******************************************************************************
  * file           : main.c
  * brief          : Main program body
  *                  Calls target system initialization then loop in main.
  ******************************************************************************
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "mx_gpio_default.h"
#include "stm32c5xx_hal.h"
#include "stm32c5xx_hal_gpio.h"

/* Private typedef -----------------------------------------------------------*/
#define ADC_SIZE 10
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint16_t AdcData[ADC_SIZE];
uint16_t AdcSum = 0;
uint16_t AdcAverage = 0;

uint32_t TimeNow = 0;
/* Private functions prototype -----------------------------------------------*/

/**
  * brief:  The application entry point.
  * retval: none but we specify int to comply with C99 standard
  */
int main(void)
{
  /** System Init: this code placed in targets folder initializes your system.
    * It calls the initialization (and sets the initial configuration) of the peripherals.
    * You can use STM32CubeMX to generate and call this code or not in this project.
    * It also contains the HAL initialization and the initial clock configuration.
    */
  if (mx_system_init() != SYSTEM_OK)
  {
    return (-1);
  }
  else
  {
    /*
      * You can start your application code here
      */
    HAL_ADC_Start(mx_adc1_gethandle());
    HAL_ADC_REG_StartConv_DMA(mx_adc1_gethandle(), (uint8_t*)AdcData, ADC_SIZE * sizeof(uint16_t));
    TimeNow = HAL_GetTick();

    while (1) 
    {
      if(HAL_GetTick() - TimeNow >= 1000) // Check if 1 second has passed
      {
        TimeNow = HAL_GetTick(); // Update the time
        // Read ADC values into AdcData array
        for (int i = 0; i < ADC_SIZE; i++)
        {
          AdcSum += AdcData[i]; // Sum the ADC values
        }
        AdcAverage = AdcSum / ADC_SIZE;
        AdcSum = 0; // Reset sum for next iteration

        if(AdcAverage > 1000) // Check if average exceeds threshold
        {
          HAL_GPIO_WritePin(LED_PORT, LED_PIN, 1); // Turn on LED
        }
        else
        {
          HAL_GPIO_WritePin(LED_PORT, LED_PIN, 0); // Turn off LED
        }
      }
    }
  }
} /* end main */

