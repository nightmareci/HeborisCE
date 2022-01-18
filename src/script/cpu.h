#pragma once

#include "../script/include.h"

void cpuCopyField(int32_t player);
int32_t cpu_checkFieldTop(int32_t player,int32_t x);
int32_t cpu_checkHoleStart(int32_t player,int32_t x);
int32_t cpu_HowManyNeedIblock(int32_t player);
int32_t cpu_blockEraseJudge(int32_t player);
int32_t cpuCheckLeftRight(int32_t player,int32_t x,int32_t y);
int32_t cpuCheckFloatingE(int32_t player,int32_t x,int32_t y);
int32_t cpuCheckFloating(int32_t player,int32_t x,int32_t y);
int32_t blockHowManyFilledFromLeft(int32_t player,int32_t y);
int32_t cpuBlockHowManyFilled(int32_t player,int32_t y);
void cpuCheckBestSpot(int32_t player);
void cpuDrop(int32_t player);
void cpuMove(int32_t player);
int32_t cpu_setBigBlock(int32_t player,int32_t bx1,int32_t by1,int32_t kind,int32_t rotate);
int32_t cpu_setBlock(int32_t player,int32_t bx1,int32_t by1,int32_t kind,int32_t rotate);
extern int32_t blkDataY[7 *4 *4];
extern int32_t blkDataX[7 *4 *4];
extern int32_t blkDDataY[7 *4 *4];
extern int32_t blkDDataX[7 *4 *4];
int32_t cpu_judgeBigBlock(int32_t player,int32_t bx1,int32_t by1,int32_t kind,int32_t rotate);
int32_t cpu_judgeBlock(int32_t player,int32_t bx1,int32_t by1,int32_t kind,int32_t rotate);
extern int32_t cp_erase[22 *2];
extern int32_t cp_fld[10 *22 *2];
