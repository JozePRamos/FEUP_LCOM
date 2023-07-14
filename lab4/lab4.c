// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "mouse.h"

extern struct packet p;
extern uint32_t packet;
extern bool synchro;
int cnt;
int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
  packet = 0;
  int r;
  int ipc_status;
  message msg;

  uint8_t mouse_bit_no = 3;
  uint8_t irq_set = BIT(mouse_bit_no);

  mouse_subscribe_int(&mouse_bit_no);
  mouse_enable_data_reporting();

  while (packet != cnt) { /* You may want to use a different condition */
    /* Get a request message */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */				
                if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
                  mouse_ih();
                  //printf("moving mouse\n");
                  mouse_print_packet(&p);
                }
                break;
            default:
                break; /* no other notifications expected: do nothing */	
        }
    } else { /* received a standard message, not a notification */
        printf("no notification");
    }
  }
  mouse_disable_data_reporting();
  mouse_unsubscribe_int();
  printf("DONE");
  return 0;
  
}

int (mouse_test_async)(uint8_t idle_time) {
    /* To be completed */
    printf("%s(%u): under construction\n", __func__, idle_time);
    return 1;
}


int(mouse_test_gesture)(uint8_t x_len, uint8_t tolerance){
  packet = 0;
  int r;
  int ipc_status;
  message msg;

  uint8_t mouse_bit_no = 3;
  uint8_t irq_set = BIT(mouse_bit_no);

  mouse_subscribe_int(&mouse_bit_no);
  mouse_enable_data_reporting();
  cnt = 0;
  while (cnt != 6) { /* You may want to use a different condition */
    /* Get a request message */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */				
                if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
                  mouse_ih();
                  //printf("moving mouse\n");
                  mouse_print_packet(&p);
                  synchro = false;
                  count_status(&p, x_len, tolerance);
                }
                break;
            default:
                break; /* no other notifications expected: do nothing */	
        }
    } else { /* received a standard message, not a notification */
        printf("no notification");
    }
  }
  mouse_disable_data_reporting();
  mouse_unsubscribe_int();
  printf("DONE");
  return 0;
}
