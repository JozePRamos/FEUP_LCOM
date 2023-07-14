#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "keyboard.c"
#include "i8042.h"

extern uint8_t bytes[2];

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {

  int r;
  int ipc_status;
  message msg;
  uint32_t cntr = 0;
  
  //bool make = true;
  //uint8_t size = 1;

  uint8_t kbd_bit_no = 1;
  uint8_t irq_set = BIT(kbd_bit_no);

  kbd_subscribe_int(&kbd_bit_no);

  while (bytes[0] != ESC_BRK) { /* You may want to use a different condition */
    /* Get a request message */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */				
                if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
                  
                  kbc_ih();
                  if (bytes[0] == 0xE0)
                    kbd_print_scancode(true, 2, &bytes[0]);
                  kbd_print_scancode(true, 1, &bytes[0]);
                  cntr++;
                }
                break;
            default:
                break; /* no other notifications expected: do nothing */	
        }
    } else { /* received a standard message, not a notification */
        printf("no notification");
    }
  }
 
  kbd_unsubscribe_int();

  kbd_print_no_sysinb(cntr);
  return 0;
}
