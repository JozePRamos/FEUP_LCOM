#pragma once

#include <stdint.h>

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

int(vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color); 

void*(vg_init)(uint16_t mode);

int(vg_draw_rectangle)(	uint16_t x, uint16_t y,uint16_t width,uint16_t 	height,uint32_t color);

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

void(outputXPM)(uint8_t* xpm_map, xpm_image_t *xpm_image, int x, int y);
