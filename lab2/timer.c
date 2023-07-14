#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

unsigned int counter = 0;

union timer_status_field_val statval;

enum timer_status_field statfield;

int hook_id;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  
  hook_id = *bit_no;
  sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);

  return 0;
}

int (timer_unsubscribe_int)() {
  
  sys_irqrmpolicy(&hook_id);

  return 0;
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {

  uint8_t ctrlWord0 = TIMER_RB_CMD | TIMER_RB_STATUS_ | TIMER_RB_SEL(timer);

  sys_outb(TIMER_CTRL, ctrlWord0);

  util_sys_inb(TIMER_0 + timer, st);
  //printf(*st);

  return 0;
}


//to be implemented;
int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

  switch(field)
    case 0:
      timer_print_config(timer, st, );
    case 1:

    case 2:

    case 3:

  return 1;
}
