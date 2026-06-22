/*
 * GameConsoleMenu.c
 *
 *  Created on: Jun 21, 2026
 *      Author: dryla
 */

#include "GameConsoleMenu.h"

// --- 1. MAKRA POMOCNICZE ---
// Kompilator obliczy rozmiar tablicy. Np. dla MainMenuItems: 3 * (wielkość MenuItem_t) / (wielkość MenuItem_t) = 3
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))


const MenuItem_t GamesMenuItems[] = {
    {"1. Snake", STATE_GAME_SNAKE,    NULL},
    {"2. Pong",  STATE_MAIN_MENU,     NULL}, // we do not have game pong
    {"3. Back",  STATE_MAIN_MENU,     NULL}  //go to main menu
};

const MenuItem_t MainMenuItems[] = {
    {"Games",    STATE_GAMES_MENU,    NULL},
    {"Settings", STATE_SETTINGS_MENU, NULL},
    {"About",    STATE_INFO_MENU,	  NULL}
    //{"Sleep",    STATE_MAIN_MENU,     null} //Enter_Deep_Sleep_Hardware Wywoła funkcję sprzętową!
};

/**
 * @brief Initialize console to basic state
 */
void Console_Init(GameConsole_t *Console) {
    Console->CurrentSystemState = STATE_MAIN_MENU;
    Console->MenuCursorIndex = 0;
    Console->NeedsRedraw = 1;
}

// --- 3. LOGIKA SILNIKA MENU ---

/**
 * @brief Funkcja wewnętrzna (Bridge). Zwraca tablicę i jej rozmiar dla aktualnego stanu.
 */
static void Get_Active_Menu_Data(GameConsoleState_t State, const MenuItem_t** OutMenuArray, uint8_t* OutSize) {
    switch(State) {
        case STATE_MAIN_MENU:
            *OutMenuArray = MainMenuItems;
            *OutSize = ARRAY_SIZE(MainMenuItems);
            break;
        case STATE_GAMES_MENU:
            *OutMenuArray = GamesMenuItems;
            *OutSize = ARRAY_SIZE(GamesMenuItems);
            break;
        default:
            // Zabezpieczenie przed błędem - domyślnie wracamy do głównego menu
            *OutMenuArray = MainMenuItems;
            *OutSize = ARRAY_SIZE(MainMenuItems);
            break;
    }
}

/**
 * @brief Reakcja na przycisk "W DÓŁ".
 */


void Console_MoveDown(GameConsole_t *Console) {
    const MenuItem_t *CurrentMenu;
    uint8_t MenuSize;

    // Pobieramy informacje o aktualnym pokoju, w którym jesteśmy
    Get_Active_Menu_Data(Console->CurrentSystemState, &CurrentMenu, &MenuSize);

    // Zabezpieczenie i zapętlenie kursora
    if (Console->MenuCursorIndex < (MenuSize - 1)) {
    	Console->MenuCursorIndex++; // Zjedź niżej
    } else {
    	Console->MenuCursorIndex = 0; // Jesteśmy na dole, wróć na samą górę!
    }

    Console->NeedsRedraw = 1;
}

/**
 * @brief Reakcja na przycisk "ENTER". Wykonuje akcję i zmienia stan.
 */
void Console_Enter(GameConsole_t *Console) {
    const MenuItem_t *CurrentMenu;
    uint8_t MenuSize;

    Get_Active_Menu_Data(Console->CurrentSystemState, &CurrentMenu, &MenuSize);

    // 1. Wykonanie akcji sprzętowej/programowej (Callback), jeśli istnieje
    if (CurrentMenu[Console->MenuCursorIndex].Action != NULL) {
    	CurrentMenu[Console->MenuCursorIndex].Action();
    }

    // 2. Pobranie nowego stanu (pokoju) z danych wybranej pozycji
    GameConsoleState_t NextState = CurrentMenu[Console->MenuCursorIndex].NextState;

    // 3. Zmiana stanu systemu, jeśli faktycznie przechodzimy do innego menu
    if (Console->CurrentSystemState != NextState) {
    	Console->CurrentSystemState = NextState;
    	Console->MenuCursorIndex = 0; // Reset kursora na samą górę nowego menu
    }

    Console->NeedsRedraw = 1;
}


void Console_Draw(GameConsole_t *Console, SSD1306_t *Display)
{
	if(Console->NeedsRedraw != 1)
	{
		return;
	}
	Console->NeedsRedraw = 0;

	const MenuItem_t *CurrentMenu; 		//current menu to show
	uint8_t CurrentMenuSize;

	Get_Active_Menu_Data(Console->CurrentSystemState, &CurrentMenu, &CurrentMenuSize);

	//clear the display
	SSD1306_Clear(BLACK);

	//switch case to display titles of pages
	uint8_t y_pos = 0;
	switch (Console->CurrentSystemState)
	{
		case STATE_MAIN_MENU:		//printf title main menu
			GFX_DrawString(0, y_pos, "--- MAIN MENU ---",  WHITE, BLACK);
		break;
		case STATE_GAMES_MENU:		//printf games title
			GFX_DrawString(0, y_pos, "--- GAMES ---",  WHITE, BLACK);
		break;
		case STATE_SETTINGS_MENU:	//printf settings title
			GFX_DrawString(0, y_pos, "--- SETTINGS ---",  WHITE, BLACK);

		break;
		case STATE_INFO_MENU:		//printf info title
			GFX_DrawString(0, y_pos, "--- ABOUT ---",  WHITE, BLACK);
		break;
	}

	//Display page all
	for (int8_t i = 0; i < CurrentMenuSize; i++)
	{
		y_pos = 15 + (i * 12);
		//draw arrow
		if (i == Console->MenuCursorIndex)
		{
			GFX_DrawChar(2, y_pos, '>', WHITE, BLACK);
		}

		GFX_DrawString(15, y_pos, (char*) CurrentMenu[i].Text,  WHITE, BLACK);
	}

	SSD1306_Display(Display);
}
