#pragma once

#include <stdint.h>

#include "i8042.h"
/**
 * @brief Subscribes and enables mouse interrupts
 * 
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 * 
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes mouse interrupts 
 *  *
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Disables data report, in order to send a command to the mouse
 * 
 */
void (mouse_disable_data_reporting)(void);

/**
 * @brief Enables data report to receive mouse packets
 * 
 */
void (request_mouse)();

/**
 * @brief Mouse interrupt handler
 * 
 */
void (mouse_ih)(void);

void(mouse_position)(struct packet *p);

int(mouse_gesture)();

void(count_status)(struct packet *p, uint8_t x_len, uint8_t tolerance);
