#include "wordMenu.h"

/* wordMenu vars */

char word[10];
int wordidx = 0;
bool wordFinished = false;

char(getLetter)(uint8_t scancode) {
    switch (scancode) {
        case A_BRK:
            return 'a';
        case B_BRK:
            return 'b';
        case C_BRK:
            return 'c';
        case D_BRK:
            return 'd';
        case E_BRK:
            return 'e';
        case F_BRK:
            return 'f';
        case G_BRK:
            return 'g';
        case H_BRK:
            return 'h';
        case I_BRK:
            return 'i';
        case J_BRK:
            return 'j';
        case K_BRK:
            return 'k';
        case L_BRK:
            return 'l';
        case M_BRK:
            return 'm';
        case N_BRK:
            return 'n';
        case O_BRK:
            return 'o';
        case P_BRK:
            return 'p';
        case Q_BRK:
            return 'q';
        case R_BRK:
            return 'r';
        case S_BRK:
            return 's';
        case T_BRK:
            return 't';
        case U_BRK:
            return 'u';
        case V_BRK:
            return 'v';
        case W_BRK:
            return 'w';
        case X_BRK:
            return 'x';
        case Y_BRK:
            return 'y';
        case Z_BRK:
            return 'z';
        case BACK_BRK:
            return '-';
        case ENTER_BRK:
            return 0;
        default:
            return '1';
            break;
    }
    return '1';
}

void(handleInput)(const char letter) {
    if (letter == '-')  {
        word[wordidx] = ' ';
        wordidx--;
    }
    else if (letter != '1') {
        word[wordidx] = letter;
        wordidx++;
    }
}
