#include "game.h"

extern enum state State;

/* wordMenu vars */
extern char word[10];
extern int wordidx;
extern bool wordFinished;

void(startGame)() {

    enum state gameState = menu; 

    subscribeDevices();

    while (gameState == menu) {

        handleInterrupts();

    }

    unsubscribeDevices();
    
}

