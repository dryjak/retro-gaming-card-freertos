/*
 * GameMenu.h
 *
 *  Created on: Jun 21, 2026
 *      Author: dryla
 */

#ifndef INC_GAMECONSOLEMENU_H_
#define INC_GAMECONSOLEMENU_H_

#include <stdint.h>
#include <stddef.h>

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
extern const MenuItem_t GamesMenuItems[];

// Main Menu
extern const MenuItem_t MainMenuItems[];

typedef struct
{
	GameConsoleState_t CurrentSystemState;
	uint8_t MenuCursorIndex;

}GameConsole_t;

#endif /* INC_GAMECONSOLEMENU_H_ */
