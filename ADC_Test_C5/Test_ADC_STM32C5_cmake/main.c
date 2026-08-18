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
#include "mx_adc1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint16_t ADC_Values[10] = {0}; /* ADC values buffer */
uint16_t Mean = 0; /* Variable to store the mean of ADC values */
uint16_t Sum = 0; /* Variable to store the sum of ADC values */
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
    HAL_ADC_REG_StartConv_DMA(mx_adc1_gethandle(), (uint8_t *)ADC_Values, sizeof(ADC_Values)/sizeof(ADC_Values[0])); /* Start ADC conversion in DMA mode */
    while (1) {
  
      Sum = 0; /* Reset sum before calculation */
      for (uint8_t i = 0; i < sizeof(ADC_Values)/sizeof(ADC_Values[0]); i++)
      {
      Sum += ADC_Values[i];
      }
      Mean = Sum / sizeof(ADC_Values)/sizeof(ADC_Values[0]);     
    }
  }
} /* end main */

