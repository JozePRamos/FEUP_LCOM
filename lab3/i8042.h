#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 keyboard.
 */

#define KBDIRQ_LINE 1 

/* Delay for the computer to register an input */
#define DELAY_US 20000

/* Keyboard register addresses */
#define KBC_ST_REG 0x64
#define KBC_INP_REG 0x64
#define KBC_OUT_BUF 0x60

#define KBD_RDCMD 0x20 //sent to 0x64, read from 0x20
#define KBD_WRCMD 0x60 //sent to 0x64, read from 0x60

/* Keyboard status checks */

#define KBC_CHECK 0xAA
#define KBD_INT_CHECK 0xAB

/* Interface enabler/disabltils.cer */

#define KBD_ENABLE 0xAE
#define KBD_DIS 0xAD

/* Register bits */

#define PARITY_ERR BIT(7)
#define TIMEOUT_ERR BIT(6)

/* Keyboard first two code bytes */

#define FIRST_BYTE 0xE0

/* Keyboard breakcodes */

#define ESC_BRK 0x81

/**@}*/

#endif /* _LCOM_I8042_H_ */
