#include "kbd.h"

int kbd_hook_id = 1;
uint8_t kbd_bytes[2];

int (kbd_subscribe_int)(uint8_t *bit_no) {

  //tickdelay(micros_to_ticks(DELAY_US));
  
  kbd_hook_id = *bit_no;
  sys_irqsetpolicy(KBDIRQ_LINE, IRQ_EXCLUSIVE | IRQ_REENABLE, &kbd_hook_id);

  return 0;
}

int(kbd_unsubscribe_int)() {
  sys_irqrmpolicy(&kbd_hook_id);
  return 0;
}

int (kbd_ih)() {

  uint8_t stat;
  uint8_t data;

    while(1) {

      if (util_sys_inb(KBC_ST_REG, &stat)) { 
        printf("Invalid sys_inb!");
        return 1;
      }

      /* loop while 8042 output buffer is empty */
      if (stat & KBC_OUT_BUF) {

        if (util_sys_inb(KBC_OUT_BUF, &data)) { 
            printf("Invalid sys_inb!");
            return 1;
        } 

        if (((stat &(PARITY_ERR | TIMEOUT_ERR)) == 0) && ((stat & AUX_BIT) == AUX_BIT)) {
          kbd_bytes[0] = (uint8_t) data;
        }
        else {
          printf("Keyboard status says still not usable");
          return 1;
        }
      }
    tickdelay(micros_to_ticks(DELAY_US));
    }
    return -1;
}

