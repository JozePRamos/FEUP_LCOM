#ifndef WORDMENU_H
#define WORDMENU_H

#include <lcom/lcf.h>

#include "../devices/i8042.h"

/**
 * @brief gets the letter coorrespondent to the scancode
 * @param scancode the code of a given getLetter
 * @return char of the given code
 * 
 */
char(getLetter)(uint8_t scancode);

#endif //WORDMENU_H
