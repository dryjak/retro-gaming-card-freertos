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
#include "stm32c5xx_drivers/hal/stm32c5xx_hal_i2c.h"
#include "stm32c5xx_hal_gpio.h"
#include "stm32c5xx_hal_i2c.h"
#include "mx_i2c1.h"

#include "SSD1306_OLED.h"
#include "GFX_BW.h"
#include "GameConsoleMenu.h"
#include "button.h"
#include "font_8x5.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//OLED
SSD1306_t OLED; 
uint8_t OledAddress = 0x3C; // OLED I2C address

//Buttons
Button_t Enter, Up, Down, Left, Right;
uint8_t ButtonPressedFlag;

//Game console FSM
GameConsole_t Console;

uint8_t value = 0xAE;
/* Private functions prototype -----------------------------------------------*/
//Wrapper
void Action_MenuUp(void);
void Action_MenuDown(void);
void Action_MenuEnter(void);
void Action_MenuLeft(void);
void Action_MenuRight(void);
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
      //Initialize OLED
      SSD1306_Init(&OLED, OledAddress, mx_i2c1_i2c_gethandle());
      GFX_SetFont(font_8x5);
      SSD1306_Clear(SSD1306_BLACK);
      HAL_Delay(50);

      //Default screen
      GFX_DrawString(10, 4, "RETRO GAME CONSOLE", WHITE, BLACK);
      GFX_DrawLine(0, 14, 127, 14, WHITE);

      GFX_DrawRectangle(44, 29, 30, 30, WHITE);
      GFX_DrawRectangle(54, 19, 30, 30, WHITE);

      GFX_DrawLine(44, 29, 54, 19, WHITE);
      GFX_DrawLine(73, 29, 83, 19, WHITE);
      GFX_DrawLine(44, 58, 54, 48, WHITE);
      GFX_DrawLine(73, 58, 83, 48, WHITE);

      GFX_DrawFillRectangle(56, 31, 6, 6, WHITE);
      SSD1306_Display(&OLED);
      HAL_Delay(4000);


      //HAL_I2C_MASTER_Transmit(&hi2c1, (0x3C << 1), 0x00, 1, &value, 1, 100);
      //HAL_I2C_MASTER_Transmit_DMA(&hi2c1, (0x3C << 1), 0x00, 1, &value, 1, 100);
      //HAL_I2C_Memory_Write(&hi2c1, 0x3C << 1, 0x00, 1, &value ,1, 100);
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

