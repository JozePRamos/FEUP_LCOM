#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include <machine/int86.h>
#include "video_gr.h"
#include <math.h>

vbe_mode_info_t info;
static unsigned h_res;	        /* Horizontal resolution in pixels */
static unsigned v_res;	        /* Vertical resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static unsigned Bytes_per_pixel;
unsigned int vram_size;

void *video_mem; /* frame-buffer VM address */
void *buffer;
void *UIbuffer;

void*(vg_init)(uint16_t mode){

    vbe_get_mode_info(mode, &info);
    h_res = info.XResolution;
    v_res = info.YResolution;
    bits_per_pixel = info.BitsPerPixel;
    Bytes_per_pixel = (bits_per_pixel+7)/8;
    int t;
    struct minix_mem_range mr; /* physical memory range */
    unsigned int vram_base = info.PhysBasePtr; /* VRAM’s physical addresss */
    vram_size = h_res * v_res * Bytes_per_pixel; /* VRAM’s size, but you can use
    the frame-buffer size, instead */
    /* Allow memory mapping */
    mr.mr_base = (phys_bytes) vram_base;
    mr.mr_limit = mr.mr_base + vram_size;

    if( OK != ( t = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", t);
    /* Map memory */
    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
    if(video_mem == MAP_FAILED)
        panic("couldn’t map video memory");

    buffer = malloc(vram_size);
    UIbuffer = malloc(vram_size); 

    reg86_t r;
    memset(&r, 0, sizeof(reg86_t));
    r.ax = 0x4f02; // VBE call, function 02 -- set VBE mode
    r.bx = 1 << 14 | mode;  // set bit 14: mode
    r.intno = 0x10;
    if( sys_int86(&r) != OK ) {
        printf("set_vbe_mode: sys_int86() failed \n");
           
    }
        
    return video_mem;
}

void(vg_buffer) () {
    memcpy(video_mem, buffer, vram_size);
}

void(vg_UIbuffer)() {
    memcpy(video_mem, UIbuffer, vram_size);
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    if (x + len > info.XResolution || x < 0) {
         printf("Invalid x axis index");
         return 1; 
    }
    if (y > info.YResolution || y < 0) {
         printf("Invalid y axis index");
         return 1;
    }
    for(int i = 0; i < len; i++){
        vg_draw_pixel(x, y, color);
        x++;
    }

    return 0;

}

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color){

    if((x >= h_res) && (y >= v_res)){
        return 1;
    }
    char* dest = (char*) video_mem + Bytes_per_pixel * h_res * y + Bytes_per_pixel * x;
    memcpy(dest, &color, Bytes_per_pixel);
    return 0;

}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
    for(int i = 0; i < height; i++) {
        vg_draw_hline(x, y, width, color);
        y++;
    }
    return 0;
}


void(outputXPM)(uint8_t* xpm_map, xpm_image_t *xpm_image, int x, int y) {
    uint32_t* address = (uint32_t*) xpm_map;
    uint32_t transparent_color = 0xa5a5a5;
    for (int  i = 0; i < xpm_image->height; i++) {
        for (int j = 0; j < xpm_image->width; j++) {
            uint32_t color = *(address + (j+i*xpm_image->width));
            if(color == transparent_color) continue;
            else vg_draw_pixel(x+j, y+i, color);
        }
    }
}

void(vg_erase)(uint8_t* xpm_map, xpm_image_t *xpm_image, int x, int y){
    /*uint32_t* address = (uint32_t*) xpm_map;
    uint32_t transparent_color = 0xa5a5a5;
    for (int  i = 0; i < xpm_image->height; i++) {
        for (int j = 0; j < xpm_image->width; j++) {
            uint32_t color = *(address + (j+i*xpm_image->width));
            color = transparent_color;
            
        }
    }*/
}
