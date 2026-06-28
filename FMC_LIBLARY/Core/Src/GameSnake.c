/*
 * GameSnake.c
 *
 *  Created on: Jun 28, 2026
 *      Author: dryla
 */


/*
 * GameSnake.c
 */

#include "GameSnake.h"
#include <stdio.h>

// Globalna instancja gry
GameSnake_t Snake;

// Funkcja pomocnicza: losowanie nowego jabłka
static void SpawnApple(void) {
    Snake.Apple.x = rand() % SNAKE_GRID_WIDTH;
    Snake.Apple.y = rand() % SNAKE_GRID_HEIGHT;
    // W profesjonalnej grze warto tu sprawdzić, czy jabłko nie zrespiło się wewnątrz ciała węża!
}

void Snake_Init(void) {
    // Reset parametrów początkowych
    Snake.Length = 3;
    Snake.Direction = DIR_RIGHT;
    Snake.IsDead = 0;
    Snake.SpeedMs = 150; // Skok węża co 150 milisekund
    Snake.Score = 0;
    Snake.LastMoveTime = HAL_GetTick();

    // Pozycja początkowa węża (środek ekranu)
    Snake.Body[0].x = SNAKE_GRID_WIDTH / 2;
    Snake.Body[0].y = SNAKE_GRID_HEIGHT / 2;

    // Generowanie ogona
    Snake.Body[1].x = Snake.Body[0].x - 1;
    Snake.Body[1].y = Snake.Body[0].y;
    Snake.Body[2].x = Snake.Body[0].x - 2;
    Snake.Body[2].y = Snake.Body[0].y;

    // "Ziarno" dla generatora liczb pseudolosowych
    srand(HAL_GetTick());
    SpawnApple();
    Snake.NeedsRedraw = 1;
}

void Snake_TurnUp(void)    { if(Snake.Direction != DIR_DOWN)  Snake.Direction = DIR_UP; }
void Snake_TurnDown(void)  { if(Snake.Direction != DIR_UP)    Snake.Direction = DIR_DOWN; }
void Snake_TurnLeft(void)  { if(Snake.Direction != DIR_RIGHT) Snake.Direction = DIR_LEFT; }
void Snake_TurnRight(void){ if(Snake.Direction != DIR_LEFT)  Snake.Direction = DIR_RIGHT; }

void Snake_UpdateLogic(void)
{
    if (Snake.IsDead) return;

        if (HAL_GetTick() - Snake.LastMoveTime < Snake.SpeedMs) {
            return;
        }
        Snake.LastMoveTime = HAL_GetTick();

        // Wąż się poruszył, więc musimy zlecić narysowanie nowej klatki!
        Snake.NeedsRedraw = 1;

    // 1. Przesuwanie ciała (od końca do głowy)
    for (int i = Snake.Length - 1; i > 0; i--) {
        Snake.Body[i] = Snake.Body[i - 1];
    }

    // 2. Przesuwanie głowy w wybranym kierunku
    switch (Snake.Direction) {
        case DIR_UP:    Snake.Body[0].y--; break;
        case DIR_DOWN:  Snake.Body[0].y++; break;
        case DIR_LEFT:  Snake.Body[0].x--; break;
        case DIR_RIGHT: Snake.Body[0].x++; break;
    }

    // 3. Kolizja ze ścianą (NAPRAWIONA)
        if (Snake.Body[0].x < 0 || Snake.Body[0].x >= SNAKE_GRID_WIDTH ||
            Snake.Body[0].y < 0 || Snake.Body[0].y >= SNAKE_GRID_HEIGHT) {

            // ZABEZPIECZENIE: Cofamy głowę w miejsce szyi, aby uniknąć wartości ujemnych
            Snake.Body[0] = Snake.Body[1];

            Snake.IsDead = 1;
            Snake.NeedsRedraw = 1; // Zlecamy narysowanie napisu Game Over
            return;
        }

        // 4. Kolizja z własnym ciałem
        for (int i = 1; i < Snake.Length; i++) {
            if (Snake.Body[0].x == Snake.Body[i].x && Snake.Body[0].y == Snake.Body[i].y) {
                Snake.IsDead = 1;
                Snake.NeedsRedraw = 1; // Zlecamy narysowanie napisu Game Over
                return;
            }
        }

    // 5. Zjedzenie jabłka
        if (Snake.Body[0].x == Snake.Apple.x && Snake.Body[0].y == Snake.Apple.y) {
        	Snake.Score++;

            if (Snake.Length < SNAKE_MAX_LENGTH) {

                // NAPRAWA: Skopiuj współrzędne aktualnego ogona do nowego klocka
                Snake.Body[Snake.Length].x = Snake.Body[Snake.Length - 1].x;
                Snake.Body[Snake.Length].y = Snake.Body[Snake.Length - 1].y;

                // Dopiero teraz oficjalnie zwiększamy długość węża
                Snake.Length++;

                // Delikatnie przyspieszamy grę z każdym jabłkiem! (min. 50ms)
                if(Snake.SpeedMs > 50) Snake.SpeedMs -= 5;
            }
            SpawnApple();
        }
}

void Snake_Draw(SSD1306_t *Display) {
    SSD1306_Clear(BLACK);

    // Rysowanie jabłka
    GFX_DrawFillRectangle(
        Snake.Apple.x * SNAKE_BLOCK_SIZE,
        Snake.Apple.y * SNAKE_BLOCK_SIZE,
        SNAKE_BLOCK_SIZE, SNAKE_BLOCK_SIZE, WHITE);

    // Rysowanie węża
    for (int i = 0; i < Snake.Length; i++) {
        GFX_DrawFillRectangle(
            Snake.Body[i].x * SNAKE_BLOCK_SIZE,
            Snake.Body[i].y * SNAKE_BLOCK_SIZE,
            SNAKE_BLOCK_SIZE, SNAKE_BLOCK_SIZE, WHITE);
    }

    // Rysowanie ekranu śmierci
        if (Snake.IsDead) {
            // Tytuł
            GFX_DrawString(20, 15, "GAME OVER", WHITE, BLACK);

            // Magia formatowania: Tworzymy bufor i wklejamy wynik
            char scoreText[20];
            snprintf(scoreText, sizeof(scoreText), "Score: %d", Snake.Score);

            // Rysujemy wynik na środku ekranu
            GFX_DrawString(20, 30, scoreText, WHITE, BLACK);

            // Zachęta do kontynuacji
            GFX_DrawString(10, 45, "Press ENTER", WHITE, BLACK);
        }

        SSD1306_Display(Display);
}
