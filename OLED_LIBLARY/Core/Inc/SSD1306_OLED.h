/*
 * SSD1306_OLED.h
 *
 *  Created on: Jun 16, 2026
 *      Author: dryla
 */

#ifndef INC_SSD1306_OLED_H_
#define INC_SSD1306_OLED_H_

#define SSD1306_TIMEOUT 1000

typedef struct
{
	uint8_t Address;
	I2C_HandleTypeDef *I2c;

}SSD1306_t;

#endif /* INC_SSD1306_OLED_H_ */
