#include <lcom/lcf.h>

#include <stdint.h>

#include "i8042.h"

#include "video_gr.h"

#include "xpms_and_images/mouse_icon.xpm"
#include "xpms_and_images/main_menu.xpm"
struct packet p;
int x_pos = 1;
int y_pos = 33;
int hook_id;
int cnt;
bool synchro;
int total_distancex = 0;
uint32_t packet = 0;
xpm_image_t image;
xpm_image_t mouse;
int (mouse_subscribe_int)(uint8_t *bit_no) {
  hook_id = *bit_no;
  sys_irqsetpolicy(MOUSEIRQ_LINE, IRQ_EXCLUSIVE | IRQ_REENABLE, &hook_id);

  return 0;
}

int (mouse_unsubscribe_int)() {
  
  sys_irqrmpolicy(&hook_id);

  return 0;
}

void (mouse_disable_data_reporting)(void) {
  uint8_t ctrlWord = 0xD4;

  sys_outb(MOUSE_WRREG, ctrlWord);

  sys_outb(MOUSE_RDWRPORT, 0xF5);
}

void (request_mouse)() {
  uint8_t ctrlWord = 0xD4;

  sys_outb(MOUSE_WRREG, ctrlWord);

  sys_outb(MOUSE_RDWRPORT, 0xF4);
}

void (mouse_ih)(void) {

  
  uint32_t stat;

  uint32_t data;
  int count = 0;
  bool synchro = false;
    while(count != 100) {
      sys_inb(KBC_ST_REG, &stat); /* assuming it returns OK */
      /* loop while 8042 output buffer is empty */
      if (stat & KBC_OUT_BUF) {
        sys_inb(KBC_OUT_BUF, &data); /* ass. it returns OK */
        if ( (stat &(PARITY_ERR | TIMEOUT_ERR)) == 0 ){
          printf("%d\n", synchro);
          if (synchro == false) {

            if ((data & 0x08) == 0) {
              printf("Mouse out of sync please click again!");
              return;
            }
            else
              synchro = true;
          }
          //bytes[0] = (uint8_t) data;
          // if (data && 0x08 == 0x08){
          //   p.lb = (bool) data & LEFT_BTN;
          //   p.rb = (bool) data & RGHT_BTN >> 1;
          //   p.mb = (bool) data & MID_BTN >> 2;
          //   p.delta_x = (int16_t) (data & X_SIGN) >> 4;
          //   p.delta_y = (int16_t) (data & Y_SIGN) >> 5;
          //   p.x_ov = (bool) data & X_OVFL >> 6;
          //   p.y_ov = (bool) data & Y_OVFL >> 7;
          // }
          
          if (count == 0){
            p.bytes[0] = (uint8_t) data;
          }
          else if (count == 1) {
            p.bytes[1] = (uint8_t) data;
          }
          else {
            p.bytes[2] = (uint8_t) data;
            if((p.bytes[0] & 0x80)!= 0) p.y_ov = true;
            else p.y_ov = false;
            if((p.bytes[0] & 0x40)!= 0) p.x_ov = true;
            else p.x_ov = false;
            if ((p.bytes[0] & 0x20) != 0){
              p.delta_y = p.bytes[2] - 256;
            }
            else
              p.delta_y = p.bytes[2];
            if ((p.bytes[0] & 0x10) != 0){
              p.delta_x = p.bytes[1] - 256;
            }
            else
              p.delta_x = p.bytes[1];
            if ((p.bytes[0] & 0x04) != 0)
              p.mb = true;
            else
              p.mb = false;
            if ((p.bytes[0] & 0x02) != 0)
              p.rb = true;
            else
              p.rb = false;
            if ((p.bytes[0] & 0x01) != 0)
              p.lb = true;
            else
              p.lb = false;
            packet++;
          }
          
          count++;
        }
        else
          return;
      }
    //tickdelay(micros_to_ticks(DELAY_US));
    }
  //request_mouse();
  // uint32_t stat;
  // uint32_t data;
  //   while(1) {
  //     sys_inb(MOUSE_RDWRPORT, &stat1); /* assuming it returns OK */
  //     stat = (uint8_t) stat1 & 0xFF;
  //     if (stat != 0xFA){
  //       printf("Stat Error!!!");
  //       break;
  //       }
  //     /* loop while 8042 output buffer is empty */
  //     if ((stat & IBF_BIT) && (stat & AUX_BIT)) {
  //       sys_inb(MOUSE_RDREG, &data); /* ass. it returns OK */
  //       if ( (stat &(PARITY_ERR | TIMEOUT_ERR)) == 0 ) {
  //         p.bytes[0] = (uint8_t) data & 0xFF0000 >> 8;
  //         p.bytes[1] = (uint8_t) data & 0xFF00 >> 4;
  //         p.bytes[2] = (uint8_t) data & 0xFF;
  //         p.lb = (bool) data & LEFT_BTN;
  //         p.rb = (bool) data & RGHT_BTN >> 1;
  //         p.mb = (bool) data & MID_BTN >> 2;
  //         p.delta_x = (int16_t) (data & X_SIGN) | p.bytes[1];
  //         p.delta_y = (int16_t) (data & Y_SIGN) | p.bytes[2];
  //         p.x_ov = (bool) data & X_OVFL >> 6;
  //         p.y_ov = (bool) data & Y_OVFL >> 7;
  //       }
  //       else
  //         return;
  //     }
  //   tickdelay(micros_to_ticks(DELAY_US));
  // }
}

void(count_status)(struct packet *p, uint8_t x_len, uint8_t tolerance){
  if(p->lb && cnt == 0 && !p->rb && !p->mb){
    cnt = 1;
  }

  else if((cnt == 1 || cnt == 2) && p->delta_x + tolerance >= 0 && p->delta_y + tolerance >= 0 && p->lb){
    total_distancex += p->delta_x;
    if(total_distancex >= x_len){
      cnt=2;
      
    } 
  }

  else if(cnt == 2 && p->lb == 0){
    total_distancex = 0;
    cnt = 3;
  }

  else if((cnt == 1 || cnt == 2)){
    total_distancex = 0;
    cnt = 0;
  }

  else if(p->rb == 1 && cnt == 3 && !p->lb && !p->mb){
    cnt = 4;
  }

  else if((cnt == 4 || cnt == 5) && p->delta_x + tolerance >= 0 && p->delta_y - tolerance <= 0 && p->rb){
    total_distancex += p->delta_x;
    if(total_distancex >= x_len){
      cnt=5;
    } 
  }

  else if(p->rb == 0 && cnt == 5){
    cnt = 6;
  }

  else if((cnt == 4 || cnt == 5)){
    total_distancex = 0;
    cnt = 0;
  }

  

  

  printf("count = %d \n", cnt);
}

void(mouse_position)(struct packet *p){
  //1152 864
  if(x_pos + p->delta_x > 1128){
    x_pos = 1128;
  }

  else if(x_pos + p->delta_x < 1){
    x_pos = 1;
  }

  else{
    x_pos += p->delta_x;
  }

  if(y_pos + p->delta_y > 830){
    y_pos = 830;
  }

  else if(y_pos + p->delta_y < 33){
    y_pos = 33;
  }

  else{
    y_pos += p->delta_y;
  }
  vg_buffer();
  
  outputXPM(image.bytes, &image, 0, 0);
  outputXPM(mouse.bytes, &mouse, x_pos, y_pos-32);
  
}

int(mouse_gesture)(){
  xpm_load(main_menu_xpm, XPM_8_8_8_8, &image);
  xpm_load(mouse_icon_xpm, XPM_8_8_8_8, &mouse);
  packet = 0;
  int r;
  int ipc_status;
  message msg;

  uint8_t mouse_bit_no = 3;
  uint8_t irq_set = BIT(mouse_bit_no);
  cnt = 0;
  mouse_subscribe_int(&mouse_bit_no);
  mouse_enable_data_reporting();
  while (cnt != 10) { /* You may want to use a different condition */
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
                  synchro = false;
                  mouse_position(&p);
                  cnt++;
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
