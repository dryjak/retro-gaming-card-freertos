/*
 * GameSnake.h
 *
 *  Created on: Jun 28, 2026
 *      Author: dryla
 */

#ifndef INC_GAMESNAKE_H_
#define INC_GAMESNAKE_H_

#include <stdint.h>
#include <stdlib.h> // dla funkcji rand()
#include "SSD1306_OLED.h"
#include "GFX_BW.h"

// Wymiary planszy (klocki 4x4 piksele)
#define SNAKE_BLOCK_SIZE  8
#define SNAKE_GRID_WIDTH  (128 / SNAKE_BLOCK_SIZE) // 32
#define SNAKE_GRID_HEIGHT (64 / SNAKE_BLOCK_SIZE)  // 16
#define SNAKE_MAX_LENGTH  100 // Oszczędzamy pamięć RAM (wąż max 100 klocków)

typedef enum {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} SnakeDirection_t;

// Struktura pojedynczego segmentu ciała
typedef struct {
    int8_t x;
    int8_t y;
} SnakeSegment_t;

// Główna struktura gry
typedef struct {
    SnakeSegment_t Body[SNAKE_MAX_LENGTH];
    uint8_t Length;
    SnakeDirection_t Direction;
    SnakeSegment_t Apple;

    uint8_t IsDead;
    uint8_t NeedsRedraw; // <--- NOWA FLAGA

    uint32_t LastMoveTime;
    uint32_t SpeedMs;

    uint16_t Score;
} GameSnake_t;

// Zmienna globalna udostępniona dla main.c (aby widział Snake.IsDead)
extern GameSnake_t Snake;

// Funkcje publiczne
void Snake_Init(void);
void Snake_UpdateLogic(void);
void Snake_Draw(SSD1306_t *Display);

void Snake_TurnUp(void);
void Snake_TurnDown(void);
void Snake_TurnLeft(void);
void Snake_TurnRight(void);

#endif
