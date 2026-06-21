/*
 * GameConsoleMenu.c
 *
 *  Created on: Jun 21, 2026
 *      Author: dryla
 */

#include <GameConsoleMenu.h>

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

