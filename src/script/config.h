#pragma once

#include "script/include.h"

#define CFG_LENGTH 240

void GameOption();
void ConfigMenu();
int32_t LoadConfig(void);
int32_t SaveConfig(void);
extern int32_t restart;
extern JoyKey joykeyAssign[10 *2];
extern int32_t keyAssign[10 *2];
extern int32_t lvupbonus;
extern int32_t downtype;
extern int32_t w_reverse;
extern int32_t showcombo;
extern int32_t breakeffect;
extern int32_t lasttopframe;
extern int32_t lastmaxPlay;
extern int32_t maxPlay;
extern int32_t fontsize;
extern int32_t movesound;
extern int32_t dispnext;
extern int32_t wavebgm;
extern int32_t fldtr;
extern int32_t dtc;
extern int32_t dispnextkey[2];
extern int32_t pausekey[2];
extern int32_t ssKey;
extern int32_t giveupKey;
extern int32_t digitc[12];
extern int32_t fontc[12];
extern int32_t lvup[2];
extern int32_t rots[2];
extern int32_t background;
extern int32_t blockflash;
extern int32_t fastlrmove;
extern int32_t sonicdrop;
extern int32_t nanameallow;
extern int32_t smooth;
extern int32_t nextblock;
extern int32_t displayIndex;
extern int32_t screenMode;
