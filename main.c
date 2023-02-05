#include "gba.h" // Mode 0 Scaffold
#include "print.h"
#include "game.h"
#include "startscreen.h"
#include "instructionsbg1.h"
#include "instructionsbg2.h"
#include "instructionsbg3.h"
#include "pausescreen.h"
#include "losescreen.h"
#include "winscreen.h"
#include "level1bg.h"
#include "level2bg.h"
#include "level3bg.h"
#include "topBg.h"
#include "sound.h"
#include "backgroundMusic.h"
#include "level2Fail.h"

int main() {
    initialize();
    while(1) {
        // Update button variables
        oldButtons = buttons;
        buttons = BUTTONS;

        //update registers
        REG_BG0HOFF = hOff;
		REG_BG0VOFF = vOff;
                
        // State Machine
        switch(state) {
            case SPLASH:
                splash();
                break;
            case INSTRUCTIONS1:
                instructions1();
                break;
            case INSTRUCTIONS2:
                instructions2();
                break;
            case INSTRUCTIONS3:
                instructions3();
                break; 
            case LEVEL1:
                level1();
                break;
            case LEVEL2:
                level2();
                break;
            case LEVEL3:
                level3();
                break;
            case PAUSE:
                pause();
                break;
            case WIN:
                win();
                break;
            case LOSE:
                lose();
                break;
        }
    }
}
void initialize() {
    mgba_open();
    REG_DISPCTL = MODE0 | BG0_ENABLE | BG2_ENABLE | SPRITE_ENABLE;
    REG_BG2CNT = BG_SIZE_SMALL | BG_SCREENBLOCK(25) | BG_CHARBLOCK(2) | BG_4BPP;
    REG_BG0CNT = BG_SIZE_SMALL | BG_SCREENBLOCK(31) | BG_CHARBLOCK(0) | BG_4BPP;
    oldButtons = 0;
    buttons = BUTTONS;
    time = 0;
    level = 0;
    lives = 3;
    avatarState = 0;
    goToSplash();
    mgba_open();
    setupSounds();
    setupInterrupts();
    playSoundA(backgroundMusic_data, backgroundMusic_length - 1000, 1);
}
void goToSplash() {
    vOff = 0;
    hOff = 0;
    REG_DISPCTL = MODE0 | BG0_ENABLE;
    REG_BG0CNT = BG_SIZE_SMALL | BG_SCREENBLOCK(31) | BG_CHARBLOCK(0) | BG_8BPP;
    DMANow(3, startscreenPal, PALETTE, startscreenPalLen/2);
    DMANow(3, startscreenTiles, &CHARBLOCK[0], startscreenTilesLen/2);
    DMANow(3, startscreenMap, &SCREENBLOCK[31], startscreenMapLen/2);
    state = SPLASH;
}
void splash() {
    seed++;
    if (BUTTON_PRESSED(BUTTON_START)) {
        srand(seed);
        initLevel1();
        goToLevel1();
        lives = 3;
    }
    if (BUTTON_PRESSED(BUTTON_B)) {
        goToInstructions1();
    }
}
void goToInstructions1() {
    hideSprites();
    vOff = 0;
    hOff = 0;
    REG_DISPCTL = MODE0 | BG0_ENABLE;
    REG_BG0CNT = BG_SIZE_SMALL | BG_SCREENBLOCK(31) | BG_CHARBLOCK(0) | BG_4BPP;
    DMANow(3, instructionsbg1Pal, PALETTE, instructionsbg1PalLen/2);
    DMANow(3, instructionsbg1Tiles, &CHARBLOCK[0], instructionsbg1TilesLen/2);
    DMANow(3, instructionsbg1Map, &SCREENBLOCK[31], instructionsbg1MapLen/2);
    state = INSTRUCTIONS1;
}
void instructions1() {
    if (BUTTON_PRESSED(BUTTON_B)) {
        goToSplash();
    }
    if (BUTTON_PRESSED(BUTTON_START)) {
        initLevel1();
        goToLevel1();
        lives = 3;
    }
    if (BUTTON_PRESSED(BUTTON_RIGHT)) {
        goToInstructions2();
    }
}
void goToInstructions2() {
    hideSprites();
    vOff = 0;
    hOff = 0;
    REG_DISPCTL = MODE0 | BG0_ENABLE;
    REG_BG0CNT = BG_SIZE_SMALL | BG_SCREENBLOCK(31) | BG_CHARBLOCK(0) | BG_4BPP;
    DMANow(3, instructionsbg2Pal, PALETTE, instructionsbg2PalLen/2);
    DMANow(3, instructionsbg2Tiles, &CHARBLOCK[0], instructionsbg2TilesLen/2);
    DMANow(3, instructionsbg2Map, &SCREENBLOCK[31], instructionsbg2MapLen/2);
    state = INSTRUCTIONS2;
}
void instructions2() {
    if (BUTTON_PRESSED(BUTTON_B)) {
        goToSplash();
    }
    if (BUTTON_PRESSED(BUTTON_START)) {
        initLevel1();
        goToLevel1();
        lives = 3;
    }
    if (BUTTON_PRESSED(BUTTON_RIGHT)) {
        goToInstructions3();
    }
    if (BUTTON_PRESSED(BUTTON_LEFT)) {
        goToInstructions1();
    }
}
void goToInstructions3() {
    hideSprites();
    vOff = 0;
    hOff = 0;
    REG_DISPCTL = MODE0 | BG0_ENABLE;
    REG_BG0CNT = BG_SIZE_SMALL | BG_SCREENBLOCK(31) | BG_CHARBLOCK(0) | BG_4BPP;
    DMANow(3, instructionsbg3Pal, PALETTE, instructionsbg3PalLen/2);
    DMANow(3, instructionsbg3Tiles, &CHARBLOCK[0], instructionsbg3TilesLen/2);
    DMANow(3, instructionsbg3Map, &SCREENBLOCK[31], instructionsbg3MapLen/2);
    state = INSTRUCTIONS3;
}
void instructions3() {
    if (BUTTON_PRESSED(BUTTON_B)) {
        goToSplash();
    }
    if (BUTTON_PRESSED(BUTTON_START)) {
        initLevel1();
        goToLevel1();
        lives = 3;
    }
    if (BUTTON_PRESSED(BUTTON_LEFT)) {
        goToInstructions2();
    }
}
void goToLevel1() {
    REG_DISPCTL = MODE0 | BG0_ENABLE | BG2_ENABLE | SPRITE_ENABLE;
    REG_BG2CNT = BG_SIZE_SMALL | BG_SCREENBLOCK(25) | BG_CHARBLOCK(2) | BG_4BPP;
    REG_BG0CNT = BG_SIZE_WIDE | BG_SCREENBLOCK(27) | BG_CHARBLOCK(0) | BG_4BPP | 1;
    DMANow(3, level1bgPal, PALETTE, level1bgPalLen/2);
    DMANow(3, topBgTiles, &CHARBLOCK[2], topBgTilesLen/2);
    DMANow(3, topBgMap, &SCREENBLOCK[25], topBgMapLen/2);
    state = LEVEL1;
    level = 1;
}
void level1() {
    updateLevel1();
    time++;
    if (BUTTON_PRESSED(BUTTON_SELECT)) {
        goToPause();
    }
    if (appa.worldCol - hOff >= 240 - appa.width - 1) {
        goToLevel2();
        initLevel2();
    }
    if (lives == 0) {
        goToLose();
    }
}
void goToLevel2() {
    REG_DISPCTL = MODE0 | BG0_ENABLE | BG2_ENABLE | SPRITE_ENABLE;
    REG_BG2CNT = BG_SIZE_SMALL | BG_SCREENBLOCK(25) | BG_CHARBLOCK(2) | BG_4BPP;
    REG_BG0CNT = BG_SIZE_LARGE | BG_SCREENBLOCK(28) | BG_CHARBLOCK(0) | BG_4BPP | 1;
    DMANow(3, level2bgPal, PALETTE, level2bgPalLen/2);
    state = LEVEL2;
    level = 2;
}
void level2() {
    updateLevel2();
    time++;
    if (BUTTON_PRESSED(BUTTON_SELECT)) {
        REG_TM2CNT = TIMER_OFF;
        goToPause();
    }
    if (scrollsCollected == SCROLLCOUNT) {
        initLevel3();
        goToLevel3();
    }
    if (second > 100) {
        lives--;
        initLevel2();
        goToLevel2();
        playSoundB(level2Fail_data, level2Fail_length, 0);
    }
    if (lives == 0) {
        goToLose();
    }
}
void goToLevel3() {
    REG_DISPCTL = MODE0 | BG0_ENABLE | BG2_ENABLE |SPRITE_ENABLE;
    REG_BG2CNT = BG_SIZE_SMALL | BG_SCREENBLOCK(25) | BG_CHARBLOCK(2) | BG_4BPP;
    REG_BG0CNT = BG_SIZE_SMALL | BG_SCREENBLOCK(31) | BG_CHARBLOCK(0) | BG_4BPP | 1;
    DMANow(3, level3bgPal, PALETTE, level3bgPalLen/2);
    state = LEVEL3;
    level = 3;
}
void level3() {
    updateLevel3();
    time++;
    if (BUTTON_PRESSED(BUTTON_SELECT)) {
        goToPause();
    }
    if (numOfSoldiers == 0) {
        goToWin();
    }
    if (lives == 0) {
        goToLose();
    }
}
void goToPause() {
    prevVoff = vOff;
    prevHoff = hOff;
    vOff = 0;
    hOff = 0;
    REG_DISPCTL = MODE0 | BG1_ENABLE;
    REG_BG1CNT = BG_SIZE_SMALL | BG_SCREENBLOCK(23) | BG_CHARBLOCK(1) | BG_4BPP;
    DMANow(3, pausescreenPal, PALETTE, pausescreenPalLen/2);
    DMANow(3, pausescreenTiles, &CHARBLOCK[1], pausescreenTilesLen/2);
    DMANow(3, pausescreenMap, &SCREENBLOCK[23], pausescreenMapLen/2);
    state = PAUSE;
}
void pause() {
    if (BUTTON_PRESSED(BUTTON_SELECT)) {
        if (level == 1) {
            goToLevel1();
        }
        if (level == 2) {
            goToLevel2();
            REG_TM2CNT = TM_FREQ_1024 | TM_IRQ | TIMER_ON;
            vOff = prevVoff;
            hOff = prevHoff;
        }
        if (level == 3) {
            goToLevel3();
        }
    }
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToSplash();
    }
}
void goToLose() {
    vOff = 0;
    hOff = 0;
    REG_DISPCTL = MODE0 | BG0_ENABLE;
    REG_BG0CNT = BG_SIZE_SMALL | BG_SCREENBLOCK(31) | BG_CHARBLOCK(0) | BG_4BPP;
    DMANow(3, losescreenPal, PALETTE, losescreenPalLen/2);
    DMANow(3, losescreenTiles, &CHARBLOCK[0], losescreenTilesLen/2);
    DMANow(3, losescreenMap, &SCREENBLOCK[31], losescreenMapLen/2);
    state = LOSE;
}
void lose() {
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToSplash();
    }
}
void goToWin() {
    vOff = 0;
    hOff = 0;
    REG_DISPCTL = MODE0 | BG0_ENABLE;
    REG_BG0CNT = BG_SIZE_SMALL | BG_SCREENBLOCK(31) | BG_CHARBLOCK(0) | BG_8BPP;
    state = WIN;
    DMANow(3, winscreenPal, PALETTE, winscreenPalLen/2);
    DMANow(3, winscreenTiles, &CHARBLOCK[0], winscreenTilesLen/2);
    DMANow(3, winscreenMap, &SCREENBLOCK[31], winscreenMapLen/2);
}
void win() {
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToSplash();
    }
}