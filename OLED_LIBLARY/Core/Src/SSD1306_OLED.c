/*
 * SSD1306_OLED.c
 *
 *  Created on: Jun 16, 2026
 *      Author: dryla
 */
#include "main.h"
#include "SSD1306_OLED.h"

void SSD1306_Command(SSD1306_t *OLED, uint8_t Command)
{
	HAL_I2C_Mem_Write(OLED->I2c, (OLED->Address<<1), 0x00, 1, &Command, 1, SSD1306_TIMEOUT);
}
void SSD1306_Data(SSD1306_t *OLED, uint8_t Data)
{
	HAL_I2C_Mem_Write(OLED->I2c, (OLED->Address<<1), 0x40, 1, &Data, 1, SSD1306_TIMEOUT);
}

void SSD1306_Init(SSD1306_t *OLED, uint8_t Address, I2C_HandleTypeDef *I2C)
{
	OLED->Address = Address;
	OLED->I2c 	  = I2C;
}


