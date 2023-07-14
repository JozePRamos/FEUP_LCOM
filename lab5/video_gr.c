// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include "video_gr.h"

void* video_mem;
vbe_mode_info_t vbeinfo;

static unsigned h_res;	        /* Horizontal resolution in pixels */
static unsigned v_res;	        /* Vertical resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

void*(vg_init)(uint16_t mode) {

    
    vbe_get_mode_info(mode, &vbeinfo); 
    h_res = vbeinfo.XResolution;
    v_res = vbeinfo.YResolution;

    bits_per_pixel = vbeinfo.BitsPerPixel;
 
    int t;
    struct minix_mem_range mr; /* physical memory range */
    unsigned int vram_base = vbeinfo.PhysBasePtr; /* VRAM’s physical addresss */
    unsigned int vram_size = h_res*v_res*((bits_per_pixel+7)/8); /* VRAM’s size, but you can use
    the frame-buffer size, instead */

    /* Allow memory mapping */
    mr.mr_base = (phys_bytes) vram_base;
    mr.mr_limit = mr.mr_base + vram_size;

    if( OK != (t = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", t);
    
    /* Map memory */
    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
    if(video_mem == MAP_FAILED)
        panic("couldn’t map video memory");

    reg86_t r;
    memset(&r, 0, sizeof(reg86_t));
    
    r.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
    r.bx = 1<<14|mode; // set bit 14: linear framebuffer
    r.intno = 0x10;

    if( sys_int86(&r) != OK) {
        printf("set_vbe_mode: sys_int86() failed \n");
    }

        
    return video_mem;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
     if (x + len > vbeinfo.XResolution || x < 0) {
         printf("Invalid x axis index");
         return 1; 
     }
     if (y > vbeinfo.YResolution || y < 0) {
         printf("Invalid y axis index");
         return 1;
     }
    for (int i = x; i < len; i++) {
        vg_draw_pixel(i, y, color);
    }
    return 0;
}

int(vg_draw_rectangle)(	uint16_t x, uint16_t y,uint16_t width,uint16_t 	height,uint32_t color){
    for(int i = y; i < height; i++){
        vg_draw_hline(x, i, width, color);
    }
    for(int i = x; i < width; i++){
        vg_draw_vline(y, i, height, color);
    }
    return 0;
}



int(vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
     if (x + len > vbeinfo.XResolution || x < 0) {
         printf("Invalid x axis index");
         return 1; 
     }
     if (y > vbeinfo.YResolution || y < 0) {
         printf("Invalid y axis index");
         return 1;
     }
    for (int i = y; i < len; i++) {
        vg_draw_pixel(x, i, color);
    }
    return 0;
}

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
    char* address = (char*) video_mem +(y*v_res+x);
    *address = color;
    return 0;
}

void(outputXPM)(uint8_t* xpm_map, xpm_image_t *xpm_image, int x, int y) {
    for (int  i = 0; i < xpm_image->height; i++)
        for (int j = 0; j < xpm_image->width; j++)
            vg_draw_pixel(x+j, y+i, *(xpm_map + (i*xpm_image->width+j)*((bits_per_pixel+7)/8)));
}

