#include <lcom/lcf.h>
#include <minix/syslib.h>

#include <stdint.h>

#include "i8042.h"

uint8_t bytes[2];

int hook_id;

int (kbd_subscribe_int)(uint8_t *bit_no) {

  tickdelay(micros_to_ticks(DELAY_US));
  
  hook_id = *bit_no;
  sys_irqsetpolicy(KBDIRQ_LINE, IRQ_EXCLUSIVE, &hook_id);

  return 0;
}

int (kbd_unsubscribe_int)() {
  
  sys_irqrmpolicy(&hook_id);

  return 0;
}

void (kbc_ih)() {

  uint32_t stat;

  uint32_t data;
    while(1) {
      sys_inb(KBC_ST_REG, &stat); /* assuming it returns OK */
      /* loop while 8042 output buffer is empty */
      if (stat & KBC_OUT_BUF) {
        sys_inb(KBC_OUT_BUF, &data); /* ass. it returns OK */
        if ( (stat &(PARITY_ERR | TIMEOUT_ERR)) == 0 )
          bytes[0] = (uint8_t) data;
        else
          return;
      }
    tickdelay(micros_to_ticks(DELAY_US));
    }
}



