#ifndef KBD_H
#define KBD_H

#include <lcom/lcf.h>

#include "i8042.h"
#include "utils.h"

/**
 * @brief Subscribes and enables keyboard interrupts
 * 
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 * */
int (kbd_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes keyboard interrupts 
 *  *
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbd_unsubscribe_int)();

/**
 * @brief Keyboard interrupt handler
 * 
 */
int (kbd_ih)();

#endif //KBD_H



