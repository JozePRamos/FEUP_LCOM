#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#include "game/menu.h"
#include "game/wordMenu.h"
#include "devices/device_handler.h"
#include "devices/video_gr.h"
#include "devices/mouse.h"
#include "devices/kbd.h"
#include "devices/utils.h"

#include "xpms_and_images/main_menu.xpm"
#include "xpms_and_images/background.xpm"
#include "xpms_and_images/mouse_icon.xpm"
int main(int argc, char *argv[]) {
  
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

// for testing purposes rn
int (proj_main_loop)(int argc, char *argv[]) {

  //xpm_image_t image2;
  vg_init(0x14C);
  xpm_image_t image;
  xpm_load(main_menu_xpm, XPM_8_8_8_8, &image);
  outputXPM(image.bytes, &image, 0, 0);
  mouse_gesture();
  //sleep(2);
  vg_exit();
  return 0;
}

/*
int (test_wordInsert_menu)(int argc, char *argv[]) {
    
}*/
