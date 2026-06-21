/*
 * GameMenu.h
 *
 *  Created on: Jun 21, 2026
 *      Author: dryla
 */

#ifndef INC_GAMECONSOLEMENU_H_
#define INC_GAMECONSOLEMENU_H_

#include "main.h"

typedef enum{
	STATE_MAIN_MENU = 0,
	STATE_GAMES_MENU,
	STATE_SETTINGS_MENU,
	STATE_INFO_MENU,
	STATE_GAME_SNAKE
}GameConsoleState_t;

typedef void (*MenuAction_t) (void);

typedef struct
{
	const char* Text;
	GameConsoleState_t NextState;
	MenuAction_t Action;
}MenuItem_t;

// Pick game menu
const MenuItem_t GamesMenuItems[] = {
    {"1. Snake", STATE_GAME_SNAKE,    NULL},
    {"2. Pong",  STATE_MAIN_MENU,     NULL}, // we do not have game pong
    {"3. Back",  STATE_MAIN_MENU,     NULL}  //go to main menu
};

// Main Menu
const MenuItem_t MainMenuItems[] = {
    {"Games",    STATE_GAMES_MENU,    NULL},
    {"Settings", STATE_SETTINGS_MENU, NULL},
    {"About",    STATE_INFO_MENU,	  NULL}
    //{"Sleep",    STATE_MAIN_MENU,     null} //Enter_Deep_Sleep_Hardware Wywoła funkcję sprzętową!
};

typedef struct
{
	GameConsoleState_t CurrentSystemState;
	uint8_t MenuCursorIndex;

}GameConsole_t;

#endif /* INC_GAMECONSOLEMENU_H_ */
