#include "gba.h"
#include "game.h"
#include "level2bg.h"
#include "level2spritesheet.h"
#include "collisionmap.h"
#include "sound.h"
#include "keyCollected.h"
#include "doorOpen.h"

unsigned char * collisionMap = (unsigned char *) collisionmapBitmap;
int collisionCheck(unsigned char *collisionMap, int col, int row, int mapWidth) {
    return collisionMap[(OFFSET(col, row, mapWidth))];
}
void initLevel2() {
    if (!avatarState) {
        hOff = 0;
        vOff = 0;
    } else {
        hOff = 0;
        vOff = 351;
    }

    //background
    DMANow(3, level2bgTiles, &CHARBLOCK[0], level2bgTilesLen/2);
    DMANow(3, level2bgMap, &SCREENBLOCK[28], level2bgMapLen/2);
    //sprites
    DMANow(3, level2spritesheetPal, SPRITEPALETTE, level2spritesheetPalLen/2);
    DMANow(3, level2spritesheetTiles, &CHARBLOCK[4], level2spritesheetTilesLen/2);
    //aang
    aang.width = 16;
    aang.height = 20;
    aang.rdel = 1;
    aang.cdel = 1;
    if (!avatarState) {
        aang.worldCol = 100;
        aang.worldRow = 80;
    } else {
        aang.worldCol = 32;
        aang.worldRow = 472;
    }
    aang.aniCounter = 0;
    aang.curFrame = 0;
    aang.numFrames = 4;
    aang.aniState = SPRITEFRONT;
    //scrolls
    scrollsCollected = 0;
    for (int i = 0; i < SCROLLCOUNT; i++) {
        scrolls[i].width = 16;
        scrolls[i].height = 8;
        scrolls[i].hide = 0;
    }
    if (!avatarState) {
        scrolls[0].worldCol = 24;
        scrolls[0].worldRow = 16;
        scrolls[1].worldCol = 104;
        scrolls[1].worldRow = 414;
        scrolls[2].worldCol = 412;
        scrolls[2].worldRow = 91;
        scrolls[3].worldCol = 278;
        scrolls[3].worldRow = 164;
        scrolls[4].worldCol = 248;
        scrolls[4].worldRow = 266;
        scrolls[5].worldCol = 283;
        scrolls[5].worldRow = 70;
        scrolls[6].worldCol = 280;
        scrolls[6].worldRow = 130;
        scrolls[7].worldCol = 146;
        scrolls[7].worldRow = 347;
        scrolls[8].worldCol = 263;
        scrolls[8].worldRow = 486;
        scrolls[9].worldCol = 472;
        scrolls[9].worldRow = 84;
    } else {
        for (int i = 0; i < SCROLLCOUNT; i++) {
            scrolls[i].worldCol = aang.worldCol + 40*i;
            scrolls[i].worldRow = aang.worldRow;
        }
    }
    //key
    hasKey = 0;
    key.worldCol = 489;
    key.worldRow = 480;
    key.width = 8;
    key.height = 8;
    //interrupts
    REG_IME = 0;
    REG_IE |= INT_TM2;
    REG_INTERRUPT = interruptHandler;
    REG_IME = 1;
    //timer
    second = 0;
    REG_TM2CNT = TIMER_OFF;
    REG_TM2D = (65536-16384);
    REG_TM2CNT = TM_FREQ_1024 | TIMER_ON | TM_IRQ;
}
void updateLevel2() {
    hideSprites();

    if (BUTTON_PRESSED(BUTTON_L) && !avatarState) {
        avatarState = 1;
        hOff = 0;
        vOff = 351;
        aang.worldCol = 32;
        aang.worldRow = 472;
        for (int i = 0; i < SCROLLCOUNT; i++) {
            if (!scrolls[i].hide) {
                scrolls[i].prevCol = scrolls[i].worldCol;
                scrolls[i].prevRow = scrolls[i].worldRow;
                scrolls[i].worldCol = aang.worldCol + 40*i;
                scrolls[i].worldRow = aang.worldRow;
            }
        }

    }
    if (BUTTON_PRESSED(BUTTON_R) && avatarState) {
        avatarState = 0;
        DMANow(3, level2spritesheetPal, SPRITEPALETTE, level2spritesheetPalLen/2);
        DMANow(3, level2bgPal, PALETTE, level2bgPalLen/2);
        for (int i = 0; i < SCROLLCOUNT; i++) {
            if (!scrolls[i].hide) {
                scrolls[i].worldCol = scrolls[i].prevCol;
                scrolls[i].worldRow = scrolls[i].prevRow;
            }
        }
    }

    //cheat
    if (avatarState) {
        //change color palette
        SPRITEPALETTE[1] = COLOR(26, 31, 31);
        SPRITEPALETTE[11] = COLOR(26, 31, 31);
        SPRITEPALETTE[15] = COLOR(26, 31, 31);
        SPRITEPALETTE[8] = COLOR(26, 31, 31);
        PALETTE[0] = COLOR(15, 21, 13);
        PALETTE[1] = COLOR(27, 24, 18);
        PALETTE[2] = COLOR(16, 10, 6);
    }

    //aang
    if (aang.aniState != SPRITEIDLE) {
        aang.prevAniState = aang.aniState;
        aang.aniState = SPRITEIDLE;
    }
    if(aang.aniCounter % 10 == 0) {
        aang.curFrame = (aang.curFrame + 1) % aang.numFrames;

    }

    if(BUTTON_HELD(BUTTON_UP)) {
        aang.aniState = SPRITEBACK;
        if (aang.worldRow > 0) {
            if (hasKey) {
                if (collisionCheck(collisionMap, aang.worldCol, aang.worldRow - aang.rdel, MAPWIDTH) != 0 && collisionCheck(collisionMap, aang.worldCol + aang.width - 1, aang.worldRow - aang.rdel, MAPWIDTH) != 0 
                && collisionCheck(collisionMap, aang.worldCol + ((aang.width - 1) / 2), aang.worldRow - aang.rdel, MAPWIDTH) != 0) {
                    aang.worldRow -= aang.rdel;
                    if (aang.worldRow - vOff <= (SCREENHEIGHT/2) && vOff > 0) {
                        vOff--;
                    }
                }
            } else {
                if (collisionCheck(collisionMap, aang.worldCol, aang.worldRow - aang.rdel, MAPWIDTH) == 1 && collisionCheck(collisionMap, aang.worldCol + aang.width - 1, aang.worldRow - aang.rdel, MAPWIDTH) == 1 
                && collisionCheck(collisionMap, aang.worldCol + ((aang.width - 1) / 2), aang.worldRow - aang.rdel, MAPWIDTH) == 1) {
                    aang.worldRow -= aang.rdel;
                    if (aang.worldRow - vOff <= (SCREENHEIGHT/2) && vOff > 0) {
                        vOff--;
                    }
                }
            }
        }
    }
    if(BUTTON_HELD(BUTTON_DOWN)) {
        aang.aniState = SPRITEFRONT;
        if (aang.worldRow + aang.height < MAPHEIGHT) {
            if (collisionCheck(collisionMap, aang.worldCol, aang.worldRow + aang.height - 1 + aang.rdel, MAPWIDTH) != 0 && collisionCheck(collisionMap, aang.worldCol + aang.width - 1, aang.worldRow + aang.height - 1 + aang.rdel, MAPWIDTH) != 0 
            && collisionCheck(collisionMap, aang.worldCol + ((aang.width - 1)/2), aang.worldRow + aang.height - 1 + aang.rdel, MAPWIDTH) != 0) {
                aang.worldRow += aang.rdel;
                if (aang.worldRow - vOff > (SCREENHEIGHT/2) && (vOff + 1) < (MAPHEIGHT - SCREENHEIGHT)) {
                    vOff++;
                }
            }
        }
    }
    if(BUTTON_HELD(BUTTON_LEFT)) {
        aang.aniState = SPRITELEFT;
        if (aang.worldCol > 0) {
            if (collisionCheck(collisionMap, aang.worldCol - aang.cdel, aang.worldRow, MAPWIDTH) != 0 && collisionCheck(collisionMap, aang.worldCol - aang.cdel, aang.worldRow + aang.height - 1, MAPWIDTH) != 0 
            && collisionCheck(collisionMap, aang.worldCol - aang.cdel, aang.worldRow + ((aang.height - 1)/4), MAPWIDTH) != 0 && collisionCheck(collisionMap, aang.worldCol - aang.cdel, aang.worldRow + aang.height - 1 - ((aang.height - 1)/4), MAPWIDTH) != 0
            && collisionCheck(collisionMap, aang.worldCol - aang.cdel, aang.worldRow + ((aang.height - 1)/2), MAPWIDTH) != 0) {
                aang.worldCol -= aang.cdel;
                if (aang.worldCol - hOff <= (SCREENWIDTH/2) && hOff > 0) {
                    hOff--;
                }
            }
        }
    }
    if(BUTTON_HELD(BUTTON_RIGHT)) {
        aang.aniState = SPRITERIGHT;
        if (aang.worldCol + aang.width < MAPWIDTH) {
            if (collisionCheck(collisionMap, aang.worldCol + aang.width - 1 + aang.cdel, aang.worldRow, MAPWIDTH) != 0 && collisionCheck(collisionMap, aang.worldCol + aang.width - 1 + aang.cdel, aang.worldRow + aang.height - 1, MAPWIDTH) != 0 
            && collisionCheck(collisionMap, aang.worldCol + aang.width - 1 + aang.cdel, aang.worldRow + ((aang.height - 1)/4), MAPWIDTH) != 0 && collisionCheck(collisionMap, aang.worldCol + aang.width - 1 + aang.cdel, aang.worldRow + aang.height - 1 - ((aang.height - 1)/4), MAPWIDTH) != 0
            && collisionCheck(collisionMap, aang.worldCol + aang.width - 1 + aang.cdel, aang.worldRow + ((aang.height - 1)/2), MAPWIDTH) != 0) {
                aang.worldCol += aang.cdel;
                if (aang.worldCol - hOff > (SCREENWIDTH/2) && (hOff + 1) < (MAPWIDTH - SCREENWIDTH)) {
                    hOff++;
                }
            }
        }
    }
    if (aang.aniState == SPRITEIDLE) {
        aang.curFrame = 0;
        aang.aniState = aang.prevAniState;
    } else {
        aang.aniCounter++;
    }

    shadowOAM[0].attr0 = ((aang.worldRow - vOff) & ROWMASK) | ATTR0_REGULAR | ATTR0_4BPP | ATTR0_TALL;
    shadowOAM[0].attr1 = ((aang.worldCol - hOff) & COLMASK) | ATTR1_MEDIUM;
    shadowOAM[0].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(aang.curFrame * 3, aang.aniState * 4);

    //scrolls
    for (int i = 0; i < SCROLLCOUNT; i++) {
        if (scrolls[i].hide) {
            shadowOAM[1+i].attr0 = ATTR0_HIDE;
        }
        if (!scrolls[i].hide) {
            //aang-scrolls collisions
            if (time >= collisionTime2 + 30) {
                if (collision(aang.worldCol, aang.worldRow, aang.width, aang.height, scrolls[i].worldCol, scrolls[i].worldRow, scrolls[i].width, scrolls[i].height)) {
                    collisionTime2 = time;
                    scrolls[i].hide = 1;
                    scrollsCollected++;
                    playSoundB(keyCollected_data, keyCollected_length - 90, 0);
                }
            }
            if ((scrolls[i].worldRow + scrolls[i].height) - vOff >= 0 && (scrolls[i].worldRow - scrolls[i].height) - vOff < SCREENHEIGHT 
            && (scrolls[i].worldCol + scrolls[i].width) - hOff >= 0 && (scrolls[i].worldCol - scrolls[i].width) - hOff < SCREENWIDTH) {
                shadowOAM[1+i].attr0 = ((scrolls[i].worldRow - vOff) & ROWMASK) | ATTR0_REGULAR | ATTR0_4BPP | ATTR0_SQUARE;
                shadowOAM[1+i].attr1 = ((scrolls[i].worldCol - hOff) & COLMASK) | ATTR1_MEDIUM;
                shadowOAM[1+i].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(1,16);
            } else {
                shadowOAM[1+i].attr0 = ATTR0_HIDE;
            }
            
        }
    }

    //timer
    shadowOAM[12].attr0 = 0 | ATTR0_REGULAR | ATTR0_SQUARE;
    shadowOAM[12].attr1 = 3 | ATTR1_TINY;
    shadowOAM[12].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID((second/100), 23);
    shadowOAM[13].attr0 = 0 | ATTR0_REGULAR | ATTR0_SQUARE;
    shadowOAM[13].attr1 = 10 | ATTR1_TINY;
    shadowOAM[13].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID((second/10)%10, 23);
    shadowOAM[14].attr0 = 0 | ATTR0_REGULAR | ATTR0_SQUARE;
    shadowOAM[14].attr1 = 17 | ATTR1_TINY;
    shadowOAM[14].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID((second%10), 23);

    //lives
    if (lives == 3) {
        shadowOAM[15].attr0 = 150 | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[15].attr1 = 29 | ATTR1_TINY;
        shadowOAM[15].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(0,31);
        shadowOAM[16].attr0 = 150 | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[16].attr1 = 38 | ATTR1_TINY;
        shadowOAM[16].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(0,31);
        shadowOAM[17].attr0 = 150 | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[17].attr1 = 47 | ATTR1_TINY;
        shadowOAM[17].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(0,31);
    }
    if (lives == 2) {
        shadowOAM[15].attr0 = 150 | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[15].attr1 = 29 | ATTR1_TINY;
        shadowOAM[15].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(0,31);
        shadowOAM[16].attr0 = 150 | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[16].attr1 = 38 | ATTR1_TINY;
        shadowOAM[16].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(0,31);
    }
    if (lives == 1) {
        shadowOAM[15].attr0 = 150 | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[15].attr1 = 29 | ATTR1_TINY;
        shadowOAM[15].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(0,31);
    }

    //number of scrolls collected displayed
    //   "scrolls:"
    shadowOAM[18].attr0 = 150 | ATTR0_REGULAR | ATTR0_WIDE;
    shadowOAM[18].attr1 = 190 | ATTR1_MEDIUM;
    shadowOAM[18].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(0,27);
    //    number
    shadowOAM[19].attr0 = 150 | ATTR0_REGULAR | ATTR0_SQUARE;
    shadowOAM[19].attr1 = 220 | ATTR1_TINY;
    shadowOAM[19].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(scrollsCollected,23);
    //    "/10"
    shadowOAM[20].attr0 = 149 | ATTR0_REGULAR | ATTR0_WIDE;
    shadowOAM[20].attr1 = 225 | ATTR1_TINY;
    shadowOAM[20].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(11,23);

    //key
    if (collision(aang.worldCol, aang.worldRow, aang.width, aang.height, key.worldCol, key.worldRow, key.width, key.height)) {
        hasKey = 1;
    }

    if (!hasKey) {
        if ((key.worldRow + key.height) - vOff >= 0 && (key.worldRow - key.height) - vOff < SCREENHEIGHT 
        && (key.worldCol + key.width) - hOff >= 0 && (key.worldCol - key.width) - hOff < SCREENWIDTH) {
            shadowOAM[21].attr0 = ((key.worldRow - vOff) & ROWMASK) | ATTR0_REGULAR | ATTR0_SQUARE;
            shadowOAM[21].attr1 = ((key.worldCol - hOff) & COLMASK) | ATTR1_TINY;
            shadowOAM[21].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(6,16);
        } else {
            shadowOAM[21].attr0 = ATTR0_HIDE;
        }
    } else {
        shadowOAM[21].attr0 = ((aang.worldRow + 8 - vOff) & ROWMASK) | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[21].attr1 = ((aang.worldCol + 10 - hOff) & COLMASK) | ATTR1_TINY;
        shadowOAM[21].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(6,16) | ATTR2_PRIORITY(1);
    }
    
    //tilemap modification
    if (hasKey) {
        if (BUTTON_PRESSED(BUTTON_A)) {
            mgba_printf("Col:%d", aang.worldCol);
            mgba_printf("Row:%d", aang.worldRow);
        }
        if (SCREENBLOCK[29].tilemap[OFFSET((aang.worldCol/8), (aang.worldRow/8), 32)] == 8) {
            SCREENBLOCK[29].tilemap[OFFSET((aang.worldCol/8), (aang.worldRow/8), 32)] = 2;
            SCREENBLOCK[29].tilemap[OFFSET(296/8, 198/8, 32)] = 2;
            SCREENBLOCK[29].tilemap[OFFSET(288/8, 198/8, 32)] = 2;
            SCREENBLOCK[29].tilemap[OFFSET(287/8, 198/8, 32)] = 2;
            SCREENBLOCK[29].tilemap[OFFSET(279/8, 198/8, 32)] = 2;
            SCREENBLOCK[29].tilemap[OFFSET(271/8, 198/8, 32)] = 2;

            // SCREENBLOCK[29].tilemap[OFFSET(264/8, 190/8, 32)] = 8;
            // SCREENBLOCK[29].tilemap[OFFSET(264/8, 182/8, 32)] = 8;
            // SCREENBLOCK[29].tilemap[OFFSET(264/8, 196/8, 32)] = 8;
            // SCREENBLOCK[29].tilemap[OFFSET(264/8, 182/8, 32)] = 8;
            // SCREENBLOCK[29].tilemap[OFFSET(296/8, 190/8, 32)] = 8;
            // SCREENBLOCK[29].tilemap[OFFSET(296/8, 182/8, 32)] = 8;
            // SCREENBLOCK[29].tilemap[OFFSET(296/8, 196/8, 32)] = 8;
            playSoundB(doorOpen_data, doorOpen_length, 0);
        }
    }


    //Copy shadowOAM into OAM
    waitForVBlank();
    DMANow(3, shadowOAM, OAM, (128*8)/2);
}