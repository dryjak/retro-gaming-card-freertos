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
#include "stm32c5xx_hal_gpio.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
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
      HAL_GPIO_WritePin(LED_PORT, LED_PIN, HAL_GPIO_PIN_SET);
      HAL_Delay(2000);
      HAL_GPIO_WritePin(LED_PORT, LED_PIN, HAL_GPIO_PIN_RESET);
      HAL_Delay(1000);
    while (1) 
    {
      if(HAL_GPIO_ReadPin(BUTTON_UP_PORT, BUTTON_UP_PIN) == HAL_GPIO_PIN_RESET || 
      HAL_GPIO_ReadPin(BUTTON_DOWN_PORT, BUTTON_DOWN_PIN) == HAL_GPIO_PIN_RESET ||
      HAL_GPIO_ReadPin(BUTTON_LEFT_PORT, BUTTON_LEFT_PIN) == HAL_GPIO_PIN_RESET ||
      HAL_GPIO_ReadPin(BUTTON_RIGHT_PORT, BUTTON_RIGHT_PIN) == HAL_GPIO_PIN_RESET ||
      HAL_GPIO_ReadPin(BUTTON_ENTER_PORT, BUTTON_ENTER_PIN) == HAL_GPIO_PIN_RESET)
      {
        HAL_GPIO_WritePin(LED_PORT, LED_PIN, HAL_GPIO_PIN_SET);
      }
      else
      {
        HAL_GPIO_WritePin(LED_PORT, LED_PIN, HAL_GPIO_PIN_RESET);
      }
      HAL_Delay(10);
    }
  }
} /* end main */

