#include <lcom/lcf.h>

#include <stdint.h>

#include "i8042.h"


struct packet p;

int hook_id;
extern int cnt;
bool synchro;
int total_distancex = 0;
uint32_t packet = 0;

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
    while(count != 3) {
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
    tickdelay(micros_to_ticks(DELAY_US));
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
