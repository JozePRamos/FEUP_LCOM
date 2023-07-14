#pragma once

#include <stdint.h>

#include "i8042.h"

int (mouse_subscribe_int)(uint8_t *bit_no);

int (mouse_unsubscribe_int)();

void (mouse_disable_data_reporting)(void);

void (request_mouse)();

void (mouse_ih)(void);

void(count_status)(struct packet *p, uint8_t x_len, uint8_t tolerance);
