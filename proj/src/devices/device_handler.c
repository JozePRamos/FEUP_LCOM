#include "device_handler.h"

#include "../game/wordMenu.h"

/* usables */ 
int r;
int ipc_status;
message msg;

/* IRQ lines */
uint8_t kbdirq = 1, mouseirq = 12, timerirq = 0;

/* video_gr utils */

/* mouse utils */
extern struct packet p;
extern uint32_t packet;

/* kbd utils */
extern uint8_t kbd_bytes[2];
uint8_t code;

int subscribeDevices() {

    if ((kbd_subscribe_int(&kbdirq)) != 0) {
        printf("Couldn't subscribe keyboard!\n");
        return 1;
    }

    if ((mouse_subscribe_int(&mouseirq)) != 0) {
        printf("Couldn't subscribe mouse!\n");
        return 1;
    }
    /*
    if ((kbd_subscribe_int(&timerirq)) != 0) {
        printf("Couldn't subscribe timer!\n");
        return 1;
    }
    */
    return 0;
}

char(handleKeyboard)() {
    kbd_ih();
    //if (kbd_bytes[0] == 0xE0)
    code = kbd_bytes[1];
    char letter = getLetter(code);
    return letter;
}

int(handleInterrupts)() {

    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */				
                if (msg.m_notify.interrupts & KBDIRQ_LINE) { /* subscribed interrupt */
                    handleKeyboard();
                }
                if (msg.m_notify.interrupts & MOUSEIRQ_LINE) { /* subscribed interrupt */
                    //handleKeyboard();
                }
                
            default:
                break; /* no other notifications expected: do nothing */	
        } 
        /* received a standard message, not a notification */
    }
    return 0;
}

int(unsubscribeDevices)() {
    if ((kbd_unsubscribe_int(&kbdirq)) != 0) {
        printf("Couldn't unsubscribe keyboard!\n");
        return 1;
    }
    if ((mouse_unsubscribe_int(&mouseirq)) != 0) {
        printf("Couldn't unsubscribe mouse!\n");
        return 1;
    }
    /*
    if ((kbd_unsubscribe_int(&kbdirq)) != 0) {
        printf("Couldn't unsubscribe timer!\n");
        return 1;
    }
    */
   return 0;
}
