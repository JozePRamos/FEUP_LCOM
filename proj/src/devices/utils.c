#include <lcom/lcf.h>

#include <stdint.h>

int (util_sys_inb)(int port, uint8_t *value) {
    uint32_t tmp = (uint32_t) value;

    if (sys_inb(port, &tmp) != 0) 
      return 1;

    *value = tmp;

    return 0;
}
