#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 keyboard.
 */

#define KBDIRQ_LINE 1
#define MOUSEIRQ_LINE 12 

/* Delay for the computer to register an input */
#define DELAY_US 20000

/* Keyboard register addresses */
#define KBC_ST_REG 0x64
#define KBC_INP_REG 0x64
#define KBC_OUT_BUF 0x60

#define KBD_RDCMD 0x20 //sent to 0x64, read from 0x20
#define KBD_WRCMD 0x60 //sent to 0x64, read from 0x60

/* Delay for the computer to register an input */
#define DELAY_US 20000

/* Keyboard register addresses */
#define ST_REG 0x64
#define INP_REG 0x64
#define OUT_BUF 0x60

#define KBD_RDCMD 0x20 //sent to 0x64, read from 0x20
#define KBD_WRCMD 0x60 //sent to 0x64, read from 0x60

/* Mouse register addresses */

#define MOUSE_RDWRPORT 0x60
#define MOUSE_RDREG 0x64
#define MOUSE_WRREG 0x64

/* Keyboard status checks */

#define KBC_CHECK 0xAA
#define KBD_INT_CHECK 0xAB

/* Interface enabler/disabltils.cer */

#define KBD_ENABLE 0xAE
#define KBD_DIS 0xAD

/* KBD Register bits */

#define PARITY_ERR BIT(7)
#define TIMEOUT_ERR BIT(6)

/* Mouse Packet Transfer: FIRST BYTE */

#define Y_OVFL BIT(7)
#define X_OVFL BIT(6)
#define Y_SIGN BIT(5)
#define X_SIGN BIT(4)
#define MID_BTN BIT(2)
#define RGHT_BTN BIT(1)
#define LEFT_BTN BIT(0)

/* Mouse register bits */

#define AUX_BIT BIT(5)
#define IBF_BIT BIT(1)

/* Keyboard first two code bytes */

#define FIRST_BYTE 0xE0

/* Keyboard breakcodes */

#define ESC_BRK 0x81
#define ENTER_BRK 0x9C
#define BACK_BRK 0x8E

#define A_BRK 0x9E
#define B_BRK 0xB0
#define C_BRK 0xAE
#define D_BRK 0xA0
#define E_BRK 0x92
#define F_BRK 0xA1
#define G_BRK 0xA2
#define H_BRK 0xA3
#define I_BRK 0x97
#define J_BRK 0xA4
#define K_BRK 0xA5
#define L_BRK 0xA6
#define M_BRK 0xB2
#define N_BRK 0xB1
#define O_BRK 0x98
#define P_BRK 0x99
#define Q_BRK 0x90
#define R_BRK 0x93
#define S_BRK 0x9F
#define T_BRK 0x94
#define U_BRK 0x96
#define V_BRK 0xAF
#define W_BRK 0x91
#define X_BRK 0xAD
#define Y_BRK 0x95
#define Z_BRK 0xAC


/**@}*/

#endif /* _LCOM_I8042_H_ */
