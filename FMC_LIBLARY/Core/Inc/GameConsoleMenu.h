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
#include <SSD1306_OLED.h>
#include <GFX_BW.h>


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

	uint8_t NeedsRedraw;
}GameConsole_t;


void Console_Enter(GameConsole_t *Console);
void Console_MoveUp(GameConsole_t *Console);
void Console_MoveDown(GameConsole_t *Console);
void Console_Init(GameConsole_t *Console);
void Console_Draw(GameConsole_t *Console, SSD1306_t *Display);


#endif /* INC_GAMECONSOLEMENU_H_ */
