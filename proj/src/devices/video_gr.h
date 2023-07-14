#pragma once

#include <stdint.h>
#include <stdio.h>
#include <lcom/lcf.h>


int(vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color); 

/**
 * @brief Initializes the video module in graphics mode.
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure
 */
void*(vg_init)(uint16_t mode);

/**
 * @brief Draws a filled rectangle with the specified width, height and color, starting at the specified coordinates.
 * @param x horizontal coordinate of the rectangle's top-left vertex
 * @param y vertical coordinate of the rectangle's top-left vertex
 * @param width rectangle's width in pixels
 * @param height rectangle's height in pixels
 * @param color color to set the pixel
 * @return 0 on success, non-zero otherwise
 *  
 */
int(vg_draw_rectangle)(	uint16_t x, uint16_t y,uint16_t width,uint16_t 	height,uint32_t color);


/**
 * @brief Draws a pixel with the specified color at the specified coordinates.
 * @param x horizontal coordinate of the pixel's top-left vertex
 * @param y vertical coordinate of the pixel's top-left vertex
 * @param color color to set the pixel
 * @return 0 on success, non-zero otherwise
 *  
 */
int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);


/**
 * @brief draws a xpm map into the screen
 * @param xpm_map address of the xpm mapped
 * @param xpm_image image result of the xpm_map
 * @param x horizontal coordinate of the xpm's top-left vertex
 * @param y vertical coordinate of the xpm's top-left vertex
 */
void(outputXPM)(uint8_t* xpm_map, xpm_image_t *xpm_image, int x, int y);

void(vg_erase)(uint8_t* xpm_map, xpm_image_t *xpm_image, int x, int y);

void(vg_buffer)();

void(vg_UIbuffer)();
