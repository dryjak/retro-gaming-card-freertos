/*
 * GameFlappy.c
 *
 *  Created on: Aug 2026
 *      Author: dryla
 */

#include "GameFlappy.h"
#include <stdio.h>
#include "../../main.h"

// Globalna instancja gry
GameFlappy_t Flappy;

// Generowanie rury na podanej pozycji startowej X
static void SpawnPipe(uint8_t index, int16_t startX) {
    Flappy.Pipes[index].x = startX;
    Flappy.Pipes[index].Passed = 0;

    // Ograniczenia luki: min 10px od góry, min 10px od dołu
    int min_y = 10;
    int max_y = FLAPPY_SCREEN_HEIGHT - FLAPPY_GAP_SIZE - 10; 
    
    // Losowanie wysokości, na której zaczyna się luka
    Flappy.Pipes[index].gap_y = min_y + (rand() % (max_y - min_y + 1));
}

void Flappy_Init(void) {
    Flappy.IsDead = 0;
    Flappy.Score = 0;
    Flappy.NeedsRedraw = 1;
    Flappy.SpeedMs = 40; // Częstotliwość odświeżania (ok. 25 FPS dla płynności)
    Flappy.LastMoveTime = HAL_GetTick();

    // Startowa pozycja i fizyka ptaka
    Flappy.BirdY = FLAPPY_SCREEN_HEIGHT / 2.0f;
    Flappy.VelocityY = 0.0f;

    srand(HAL_GetTick());

    // Ustawienie pierwszej rury tuż za krawędzią ekranu
    SpawnPipe(0, FLAPPY_SCREEN_WIDTH + 20);
    // Ustawienie drugiej rury z odstępem 70 pikseli od pierwszej
    SpawnPipe(1, FLAPPY_SCREEN_WIDTH + 20 + 70); 
}

void Flappy_Jump(void) {
    // Reaguj na przycisk tylko, jeśli gra nadal trwa
    if (!Flappy.IsDead) {
        Flappy.VelocityY = -4.0f; // Impuls w górę (wartość ujemna, bo oś Y rośnie w dół)
    }
}

void Flappy_UpdateLogic(void) {
    if (Flappy.IsDead) return;

    if (HAL_GetTick() - Flappy.LastMoveTime < Flappy.SpeedMs) {
        return;
    }
    Flappy.LastMoveTime = HAL_GetTick();
    
    // Ptak i rury się przemieściły, wymuszamy odświeżenie ekranu
    Flappy.NeedsRedraw = 1; 

    // 1. Fizyka ptaka
    Flappy.VelocityY += 0.6f; // Siła grawitacji przyciągająca ptaka w dół
    Flappy.BirdY += Flappy.VelocityY;

    // 2. Sprawdzenie kolizji z ziemią lub sufitem
    if (Flappy.BirdY < 0 || (Flappy.BirdY + FLAPPY_BIRD_SIZE) >= FLAPPY_SCREEN_HEIGHT) {
        Flappy.IsDead = 1;
        return;
    }

    // 3. Logika rur i kolizji
    for (int i = 0; i < 2; i++) {
        // Przesuwanie rury w lewo
        Flappy.Pipes[i].x -= 3; 

        // Przydzielanie punktu za minięcie rury
        if (!Flappy.Pipes[i].Passed && (Flappy.Pipes[i].x + FLAPPY_PIPE_WIDTH < FLAPPY_BIRD_X)) {
            Flappy.Pipes[i].Passed = 1;
            Flappy.Score++;
        }

        // Zwijanie rury: jeśli wyleciała za ekran z lewej, przerzucamy ją na prawą stronę
        if (Flappy.Pipes[i].x < -FLAPPY_PIPE_WIDTH) {
            uint8_t other_pipe = (i == 0) ? 1 : 0;
            SpawnPipe(i, Flappy.Pipes[other_pipe].x + 70); // Utrzymanie stałego dystansu
        }

        // 4. Kolizja z przeszkodami
        // Czy ptak znajduje się na szerokości rury (oś X)?
        if ((FLAPPY_BIRD_X + FLAPPY_BIRD_SIZE) > Flappy.Pipes[i].x && 
            FLAPPY_BIRD_X < (Flappy.Pipes[i].x + FLAPPY_PIPE_WIDTH)) {
            
            // Jeśli tak, to czy znajduje się poza luką (oś Y)?
            if (Flappy.BirdY < Flappy.Pipes[i].gap_y || 
               (Flappy.BirdY + FLAPPY_BIRD_SIZE) > (Flappy.Pipes[i].gap_y + FLAPPY_GAP_SIZE)) {
                Flappy.IsDead = 1;
                return;
            }
        }
    }
}

void Flappy_Draw(SSD1306_t *Display) {
    SSD1306_Clear(BLACK);

    // 1. Rysowanie rur
    for (int i = 0; i < 2; i++) {
        // Górna rura
        GFX_DrawFillRectangle(
            Flappy.Pipes[i].x, 
            0, 
            FLAPPY_PIPE_WIDTH, 
            Flappy.Pipes[i].gap_y, 
            WHITE);
            
        // Dolna rura
        GFX_DrawFillRectangle(
            Flappy.Pipes[i].x, 
            Flappy.Pipes[i].gap_y + FLAPPY_GAP_SIZE, 
            FLAPPY_PIPE_WIDTH, 
            FLAPPY_SCREEN_HEIGHT - (Flappy.Pipes[i].gap_y + FLAPPY_GAP_SIZE), 
            WHITE);
    }

    // 2. Rysowanie ptaka (rzutujemy pozycję float na int)
    GFX_DrawFillRectangle(
        FLAPPY_BIRD_X, 
        (int16_t)Flappy.BirdY, 
        FLAPPY_BIRD_SIZE, 
        FLAPPY_BIRD_SIZE, 
        WHITE);

    // 3. Nakładka Game Over (identyczna jak w Snake)
    if (Flappy.IsDead) {
        GFX_DrawString(37, 15, "GAME OVER", WHITE, BLACK);

        char scoreText[20];
        snprintf(scoreText, sizeof(scoreText), "Score: %d", Flappy.Score);
        GFX_DrawString(37, 27 , scoreText, WHITE, BLACK);

        GFX_DrawString(31, 39, "Press ENTER", WHITE, BLACK);
    }

    SSD1306_Display(Display);
}