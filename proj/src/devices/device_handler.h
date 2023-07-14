#ifndef DEVICE_HANDLER_H
#define DEVICE_HANDLER_H

#include <lcom/lcf.h>

#include "utils.h"
#include "kbd.h"
#include "mouse.h"

/**
 * @brief Will handle the characters written using the keyboard
 * 
 * @return The corresponding character to the code
 */
char(handleKeyboard)();

/**
 * @brief checks ifit's possible to subscribe devices
 *
 * @return Returns 1 if there is at least an error, 0 otherwise
 * 
 */
int(subscribeDevices)();

/**
 * @brief Interrupt handler used for keyboard and mouse
 * @return Returns 0 upon success
 */
int(handleInterrupts)();

/**
 * @brief checks ifit's possible to unsubscribe devices
 *
 * @return Returns 1 if there is at least an error, 0 otherwise
 */
int(unsubscribeDevices)();
//int(insertWordMenu_handler)();

#endif //DEVICE_HANDLER_H

