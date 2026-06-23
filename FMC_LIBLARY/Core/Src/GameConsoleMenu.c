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
    {"1. Snake", ITEM_ACTION,	STATE_GAME_SNAKE,    NULL},
    {"2. Pong",  ITEM_ACTION,	STATE_MAIN_MENU,     NULL}, // we do not have game pong
    {"Back",  ITEM_FOLDER,		STATE_MAIN_MENU,     NULL}  //go to main menu
};

const MenuItem_t MainMenuItems[] = {
    {"Games",    ITEM_FOLDER, 	STATE_GAMES_MENU,    NULL},
    {"Settings", ITEM_FOLDER,   STATE_SETTINGS_MENU, NULL},
    {"About",    ITEM_FOLDER,   STATE_INFO_MENU,	 NULL}
    //{"Sleep",    STATE_MAIN_MENU,     null} //Enter_Deep_Sleep_Hardware Wywoła funkcję sprzętową!
};

const MenuItem_t SettingsMenuItems[] = {
    // Text       // Type         // Stan docelowy (dla folderu) // Action
    {"Brightness", ITEM_VALUE,    STATE_SETTINGS_MENU,           NULL},
    {"Mode",       ITEM_VALUE,    STATE_SETTINGS_MENU,           NULL},
    {"Back",       ITEM_FOLDER,   STATE_MAIN_MENU,               NULL}
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
        case STATE_SETTINGS_MENU:
			*OutMenuArray = SettingsMenuItems;
			*OutSize = ARRAY_SIZE(SettingsMenuItems);
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

void Console_MoveUp(GameConsole_t *Console) {
    const MenuItem_t *CurrentMenu;
    uint8_t MenuSize;

    Get_Active_Menu_Data(Console->CurrentSystemState, &CurrentMenu, &MenuSize);

    // Zabezpieczenie i zapętlenie kursora
    if (Console->MenuCursorIndex > 0) {
    	Console->MenuCursorIndex--; // go up
    }
    else
    {
    	Console->MenuCursorIndex = MenuSize - 1;
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

    // Pobieramy aktualny element
        MenuItem_t SelectedItem = CurrentMenu[Console->MenuCursorIndex];

        // Jeśli jesteśmy w trybie edycji, Enter z niego wychodzi (zatwierdza)
        if (Console->IsEditMode == 1) {
            Console->IsEditMode = 0;
            Console->NeedsRedraw = 1;
            return;
        }

        // Sprawdzamy typ podświetlonego elementu
        if (SelectedItem.Type == ITEM_VALUE) {
            // Wchodzimy w tryb edycji!
            Console->IsEditMode = 1;
            Console->NeedsRedraw = 1;
        }
        else if (SelectedItem.Type == ITEM_FOLDER) {
            // Przechodzimy do innego menu
            Console->CurrentSystemState = SelectedItem.NextState;
            Console->MenuCursorIndex = 0;
            Console->NeedsRedraw = 1;
        }
        else if (SelectedItem.Type == ITEM_ACTION) {
            // Wykonujemy akcję (np. start gry)
            if (SelectedItem.Action != NULL) SelectedItem.Action();
        }
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
	uint8_t y_pos = 1;
	switch (Console->CurrentSystemState)
	{
		case STATE_MAIN_MENU:		//printf title main menu
			GFX_DrawString(0, y_pos, "--- MAIN MENU ---",  WHITE, 0);
		break;
		case STATE_GAMES_MENU:		//printf games title
			GFX_DrawString(0, y_pos, "--- GAMES ---",  WHITE, 0);
		break;
		case STATE_SETTINGS_MENU:	//printf settings title
			GFX_DrawString(0, y_pos, "--- SETTINGS ---",  WHITE, 0);

		break;
		case STATE_INFO_MENU:		//printf info title
			GFX_DrawString(0, y_pos, "--- ABOUT ---",  WHITE, 0);
		break;
	}

	//Display page all
	for (int8_t i = 0; i < CurrentMenuSize; i++)
	{
		y_pos = 15 + (i * 12);
		//draw arrow
		if (i == Console->MenuCursorIndex)
		{
			GFX_DrawChar(2, y_pos, '>', WHITE, 0);
		}

		GFX_DrawString(15, y_pos, (char*) CurrentMenu[i].Text,  WHITE, 0);
	}

	SSD1306_Display(Display);
}
