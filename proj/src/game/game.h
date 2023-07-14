#ifndef GAME_H
#define GAME_H

#include "menu.h"
#include "wordMenu.h"

#include "../devices/kbd.h"
#include "../devices/mouse.h"
#include "../devices/video_gr.h"
#include "../devices/device_handler.h"

/**
 * @brief the various states in which the game can be
 * 
 */
enum state {menu, wordMenu, game, gameEnd};

/**
 * @brief starts the game
 * 
 */
void(startGame)();

#endif //GAME_H

