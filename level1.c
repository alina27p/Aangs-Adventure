#include "gba.h"
#include "game.h"
#include "level1spritesheet.h"
#include "level1bg.h"
#include "balloonCollisionSound.h"
#include "sound.h"

int sbb;
void initLevel1() {
    hOff = 0;
    vOff = 0;
    //sprites
    DMANow(3, level1spritesheetPal, SPRITEPALETTE, level1spritesheetPalLen/2);
    DMANow(3, level1spritesheetTiles, &CHARBLOCK[4], level1spritesheetTilesLen/2);
    //background
    DMANow(3, level1bgTiles, &CHARBLOCK[0], level1bgTilesLen/2);
    DMANow(3, level1bgMap, &SCREENBLOCK[27], level1bgMapLen/2);
    sbb = 27;
    //appa
    appa.width = 32;
    appa.height = 13;
    appa.rdel = 1;
    appa.worldCol = 100;
    appa.worldRow = 80;
    appa.aniCounter = 0;
    //war balloons
    for (int i = 0; i < WARBALLOONCOUNT; i++) {
        warBalloon[i].width = 29;
        warBalloon[i].height = 19;
        warBalloon[i].cdel = 1;
        warBalloon[i].worldCol = 240;
        warBalloon[i].worldRow = rand() % (160 - warBalloon[i].height);
        warBalloon[i].hide = 1;
    }
    //air balls/attacks
    for (int i = 0; i < ATTACKSCOUNT; i++) {
        attacks[i].width = 7;
        attacks[i].height = 5;
        attacks[i].hide = 1;
        attacks[i].cdel = 2;
    }
}
void updateLevel1() {
    hideSprites();
   
    if (BUTTON_PRESSED(BUTTON_L) && !avatarState) {
        avatarState = 1;
    }
    if (BUTTON_PRESSED(BUTTON_R) && avatarState) {
        avatarState = 0;
        DMANow(3, level1spritesheetPal, SPRITEPALETTE, level1spritesheetPalLen/2);
        DMANow(3, level1bgPal, PALETTE, level1bgPalLen/2);
    }
    
    //background
    if (hOff >= 255) {
        if (sbb < 30) {
            hOff -= 255;
            sbb++;
        } 
        REG_BG0CNT = BG_SIZE_WIDE | BG_SCREENBLOCK(sbb) | BG_CHARBLOCK(0) | BG_4BPP;
    }
    if (time%2 == 0) {
        if ((hOff < 255) && (sbb < 30)) {
            hOff++;
        }
    }
    REG_BG0HOFF = hOff;

    //appa
    if (BUTTON_PRESSED(BUTTON_UP) || BUTTON_HELD(BUTTON_UP)) {
        if (appa.worldRow > 0) {
            appa.worldRow--;
        }
    }
    if (BUTTON_PRESSED(BUTTON_DOWN) || BUTTON_HELD(BUTTON_DOWN)) {
        if (appa.worldRow < 160 - appa.height) {
            appa.worldRow++;
        }
    }
    if (sbb == 30) {
        if (appa.worldCol - hOff < 240 - appa.width) {
            appa.worldCol++;
        }
    }
    shadowOAM[0].attr0 = (appa.worldRow & ROWMASK) | ATTR0_REGULAR | ATTR0_4BPP | ATTR0_WIDE;
    shadowOAM[0].attr1 = (appa.worldCol & COLMASK) | ATTR1_MEDIUM;
    shadowOAM[0].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(0,0);
    //war balloons
    for(int i = 0; i < WARBALLOONCOUNT; i++) {
        if (time%40 == 0) {
            if (warBalloon[i].hide) {
                warBalloon[i].hide = 0;
                warBalloon[i].worldCol = 240;
                warBalloon[i].worldRow = rand() % (160 - warBalloon[i].height); 
                break;
            }
        }
        if (!warBalloon[i].hide) {
            warBalloon[i].worldCol -= warBalloon[i].cdel;
            if (warBalloon[i].worldCol <= 0 - warBalloon[i].width) {
                warBalloon[i].worldCol = 240;
                warBalloon[i].worldRow = rand() % (160 - warBalloon[i].height);
            }
            //appa-warballoon collisions
            if (time >= collisionTime + 70) {
                if (collision(appa.worldCol, appa.worldRow, appa.width, appa.height, warBalloon[i].worldCol, warBalloon[i].worldRow, warBalloon[i].width, warBalloon[i].height)) {
                    collisionTime = time;
                    lives--;
                    playSoundB(balloonCollisionSound_data, balloonCollisionSound_length - 100, 0);
                }
            }
            //warballoon-attack collisions
            if (avatarState) {
                for (int j = 0; j < ATTACKSCOUNT; j++) {
                    if (!attacks[j].hide) {
                        if (collision(warBalloon[i].worldCol, warBalloon[i].worldRow, warBalloon[i].width, warBalloon[i].height,
                        attacks[j].worldCol, attacks[j].worldRow, attacks[j].width, attacks[j].height)) {
                            shadowOAM[1+i].attr0 = ATTR0_HIDE;
                            warBalloon[i].hide = 1;
                        }
                    }
                }
            }
            shadowOAM[1+i].attr0 = (warBalloon[i].worldRow & ROWMASK) | ATTR0_REGULAR | ATTR0_SQUARE;
            shadowOAM[1+i].attr1 = (warBalloon[i].worldCol & COLMASK) | ATTR1_MEDIUM;
            shadowOAM[1+i].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(0,5);
        }
    }

    //lives
    if (lives == 3) {
        shadowOAM[11].attr0 = 150 | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[11].attr1 = 29 | ATTR1_TINY;
        shadowOAM[11].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(0,31);
        shadowOAM[12].attr0 = 150 | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[12].attr1 = 38 | ATTR1_TINY;
        shadowOAM[12].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(0,31);
        shadowOAM[13].attr0 = 150 | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[13].attr1 = 47 | ATTR1_TINY;
        shadowOAM[13].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(0,31);
    }
    if (lives == 2) {
        shadowOAM[11].attr0 = 150 | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[11].attr1 = 29 | ATTR1_TINY;
        shadowOAM[11].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(0,31);
        shadowOAM[12].attr0 = 150 | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[12].attr1 = 38 | ATTR1_TINY;
        shadowOAM[12].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(0,31);
    }
    if (lives == 1) {
        shadowOAM[11].attr0 = 150 | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[11].attr1 = 29 | ATTR1_TINY;
        shadowOAM[11].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(0,31);
    }

    //cheat
    if (avatarState) {
        //change palette
        SPRITEPALETTE[7] = COLOR(26, 31, 31);
        SPRITEPALETTE[15] = COLOR(31, 31, 31);
        SPRITEPALETTE[14] = COLOR(31, 31, 31);
        PALETTE[0] = COLOR(10, 20, 31);

        //attacking
        if (BUTTON_PRESSED(BUTTON_A)) {
            for (int i = 0; i < ATTACKSCOUNT; i++) {
                if (attacks[i].hide) {
                    attacks[i].hide = 0;
                    attacks[i].worldCol = appa.worldCol + appa.width;
                    attacks[i].worldRow = appa.worldRow + 4;
                    break;
                } 
            }
        }
        for (int i = 0; i < ATTACKSCOUNT; i++) {
             if (!attacks[i].hide) {
                attacks[i].worldCol += attacks[i].cdel;
                if (attacks[i].worldCol - hOff >= 240) {
                    attacks[i].hide = 1;
                }
                shadowOAM[14 + i].attr0 = (attacks[i].worldRow & ROWMASK) | ATTR0_REGULAR | ATTR0_SQUARE;
                shadowOAM[14 + i].attr1 = (attacks[i].worldCol & COLMASK) | ATTR1_TINY;
                shadowOAM[14 + i].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(0, 10);
                }
            }
        }

    //Copy shadowOAM into OAM
    waitForVBlank();
    DMANow(3, shadowOAM, OAM, (128*8)/2);
}