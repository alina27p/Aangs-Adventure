#include "gba.h"
#include "game.h"
#include "level3bg.h"
#include "level3spritesheet.h"
#include "firebending.h"
#include "sound.h"

void initLevel3() {
    hOff = 0;
    vOff = 0;
    //background
    DMANow(3, level3bgTiles, &CHARBLOCK[0], level3bgTilesLen/2);
    DMANow(3, level3bgMap, &SCREENBLOCK[31], level3bgMapLen/2);
    //sprites
    DMANow(3, level3spritesheetPal, SPRITEPALETTE, level3spritesheetPalLen/2);
    DMANow(3, level3spritesheetTiles, &CHARBLOCK[4], level3spritesheetTilesLen/2);
    //aang2
    aangSSCol = 0;
    aangSSRow = 0;
    aang2.width = 30;
    aang2.height = 40;
    aang2.rdel = 1;
    aang2.cdel = 1;
    aang2.worldCol = 20;
    aang2.worldRow = 129 - aang2.height;
    aang2.aniCounter = 0;

    //soldiers
    numOfSoldiers = 1;
    for (int i = 0; i < SOLDIERCOUNT; i++) {
        soldiers[i].width = 29;
        soldiers[i].height = 48;
        soldiers[i].rdel = 1;
        soldiers[i].cdel = 1;
        soldiers[i].worldCol = 170 + 20*i;
        soldiers[i].worldRow = 129 - soldiers[i].height;
        soldiers[i].aniCounter = 0;
        soldiers[i].curFrame = SOLDIERSTAND; 
        soldiers[i].numFrames = 4;
        soldiers[i].hide = 0;
        soldiers[i].damage = 0;
        soldiers[i].attacked = 0;
    }

    //fireballs
    for (int i = 0; i < FIREBALLCOUNT; i++) {
        fireball[i].width = 14;
        fireball[i].height = 8;
        fireball[i].cdel = 3;
        fireball[i].hide = 1;
    }

    //airballs
    for (int i = 0; i < AIRBALLCOUNT; i++) {
        airball[i].width = 14;
        airball[i].height = 8;
        airball[i].cdel = 3;
        airball[i].hide = 1;
    }
}
void updateLevel3() {
    hideSprites();

    if (BUTTON_PRESSED(BUTTON_L) && !avatarState) {
        avatarState = 1;
    }
    if (BUTTON_PRESSED(BUTTON_R) && avatarState) {
        avatarState = 0;
        DMANow(3, level3spritesheetPal, SPRITEPALETTE, level3spritesheetPalLen/2);
        DMANow(3, level3bgPal, PALETTE, level3bgPalLen/2);
    }

    //aang2
    if (time%50 == 0) {
        aangSSCol = 0;
        aangSSRow = 0;
        aang2.height = 40;
        aang2.worldRow = 129 - aang2.height;
    }
    if (BUTTON_PRESSED(BUTTON_A) && !((BUTTON_PRESSED(BUTTON_DOWN) || BUTTON_HELD(BUTTON_DOWN)))) {
        aangSSCol = 24;
        aangSSRow = 0;
        aang2.height = 36;
        aang2.worldRow = 129 - aang2.height;
        for (int i = 0; i < AIRBALLCOUNT; i++) {
            if (airball[i].hide) {
                airball[i].hide = 0;
                airball[i].worldCol = aang2.worldCol + aang2.width + 20 + airball[i].width;
                airball[i].worldRow = aang2.worldRow + 4;
                break;
            }
        }
    }
    if (BUTTON_PRESSED(BUTTON_DOWN) || BUTTON_HELD(BUTTON_DOWN)) {
        aangSSCol = 0;
        aangSSRow = 8;
        aang2.height = 26;
        aang2.worldRow = 129 - aang2.height;
    }
    shadowOAM[0].attr0 = (aang2.worldRow & ROWMASK) | ATTR0_REGULAR | ATTR0_4BPP | ATTR0_SQUARE;
    shadowOAM[0].attr1 = (aang2.worldCol & COLMASK) | ATTR1_LARGE;
    shadowOAM[0].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(aangSSCol,aangSSRow);

    //soldiers
    for (int i = 0; i < SOLDIERCOUNT; i++) {
        if (soldiers[i].hide == 1) {
            shadowOAM[1+i].attr0 = ATTR0_HIDE;
        }
        if (!soldiers[i].hide) {
            if (!soldiers[i].attacked) {
                if (soldiers[i].aniCounter % 20 == 0) {
                    soldiers[i].curFrame = (soldiers[i].curFrame + 1) % soldiers[i].numFrames;
                }
                soldiers[i].aniCounter++;
                //fireball
                if (soldiers[i].curFrame == SOLDIERFIRE) {
                    //mgba_printf("state fire");
                    playSoundB(firebending_data, firebending_length - 30, 0);
                    for (int i = 0; i < FIREBALLCOUNT; i++) {
                        if (fireball[i].hide) {
                            fireball[i].hide = 0;
                            fireball[i].worldCol = soldiers[i].worldCol - fireball[i].width;
                            fireball[i].worldRow = soldiers[i].worldRow + 8;
                            break;
                        }
                    }
                }
            } else {
                soldiers[i].attacked++;
                if (soldiers[i].attacked > 50) {
                    soldiers[i].attacked = 0;
                }
            }
            //airball-soldier collision
            for (int i = 0; i < AIRBALLCOUNT; i++) {
                if (!airball[i].hide) {
                    if (time >= collisionTime3 + 20) {
                        if (collision(soldiers[i].worldCol, soldiers[i].worldRow, soldiers[i].width, soldiers[i].height,
                        airball[i].worldCol, airball[i].worldRow, airball[i].width, airball[i].height)) {
                            collisionTime3 = time;
                            if (!avatarState) {
                                soldiers[i].damage++;
                            } else {
                                soldiers[i].damage += 2;
                            }
                            soldiers[i].attacked = 1;
                        }
                    }
                }
            }

            if (soldiers[i].damage >= 4) {
                soldiers[i].hide = 1;
                numOfSoldiers--;
            }
            shadowOAM[1+i].attr0 = (soldiers[i].worldRow & ROWMASK) | ATTR0_REGULAR | ATTR0_4BPP | ATTR0_SQUARE;
            shadowOAM[1+i].attr1 = (soldiers[i].worldCol & COLMASK) | ATTR1_LARGE;
            shadowOAM[1+i].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(8*soldiers[i].curFrame, 16);
        }
    }

    //firebll
    for (int i = 0; i < FIREBALLCOUNT; i++) {
        if (!fireball[i].hide) {
            fireball[i].worldCol = fireball[i].worldCol - fireball[i].cdel;
            if (fireball[i].worldCol <= (0 - fireball[i].width)) {
                fireball[i].hide = 1;
            }
            //fireball-aang2 collision
            if (time >= collisionTime4 + 60) {
                if (collision(aang2.worldCol, aang2.worldRow, aang2.width, aang2.height,
                fireball[i].worldCol, fireball[i].worldRow, fireball[i].width, fireball[i].height)) {
                    collisionTime4 = time;
                    aangSSCol = 16;
                    aangSSRow = 8;
                    aang2.height = 24;
                    aang2.worldRow = 129 - aang2.height;
                    lives--;
                }
            }

            shadowOAM[4+i].attr0 = (fireball[i].worldRow & ROWMASK) | ATTR0_REGULAR | ATTR0_4BPP | ATTR0_WIDE;
            shadowOAM[4+i].attr1 = (fireball[i].worldCol & COLMASK) | ATTR1_TINY;
            shadowOAM[4+i].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(9,24);
        }
    }

    //airball
    for (int i = 0; i < AIRBALLCOUNT; i++) {
        if (!airball[i].hide) {
            airball[i].worldCol = airball[i].worldCol + airball[i].cdel;
            if (airball[i].worldCol >= 240) {
                airball[i].hide = 1;
            }
            shadowOAM[7+i].attr0 = (airball[i].worldRow & ROWMASK) | ATTR0_REGULAR | ATTR0_4BPP | ATTR0_WIDE;
            shadowOAM[7+i].attr1 = (airball[i].worldCol & COLMASK) | ATTR1_TINY;
            shadowOAM[7+i].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(25,8);
        }
    }

    //lives
    if (lives == 3) {
        shadowOAM[10].attr0 = 150 | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[10].attr1 = 29 | ATTR1_TINY;
        shadowOAM[10].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(0,31);
        shadowOAM[11].attr0 = 150 | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[11].attr1 = 38 | ATTR1_TINY;
        shadowOAM[11].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(0,31);
        shadowOAM[12].attr0 = 150 | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[12].attr1 = 47 | ATTR1_TINY;
        shadowOAM[12].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(0,31);
    }
    if (lives == 2) {
        shadowOAM[10].attr0 = 150 | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[10].attr1 = 29 | ATTR1_TINY;
        shadowOAM[10].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(0,31);
        shadowOAM[11].attr0 = 150 | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[11].attr1 = 38 | ATTR1_TINY;
        shadowOAM[11].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(0,31);
    }
    if (lives == 1) {
        shadowOAM[10].attr0 = 150 | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[10].attr1 = 29 | ATTR1_TINY;
        shadowOAM[10].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(0,31);
    }

    //cheat
    if (avatarState) {
        SPRITEPALETTE[9] = COLOR(26, 31, 31);
        SPRITEPALETTE[3] = COLOR(26, 31, 31);
        SPRITEPALETTE[4] = COLOR(26, 31, 31);
        PALETTE[1] = COLOR(7, 18 , 31);
        PALETTE[5] = COLOR(13, 22, 31);
        PALETTE[2] = COLOR(16, 24, 31);
        PALETTE[4] = COLOR(18, 26, 31);
    }

    //Copy shadowOAM into OAM
    waitForVBlank();
    DMANow(3, shadowOAM, OAM, (128*8)/2);
}