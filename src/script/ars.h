#pragma once

#include "script/include.h"

void effect(int32_t player);
void pinchCheck(int32_t player);
void drawTLSBlock(int32_t player,int32_t bottom);
void drawCBlock(int32_t player,int32_t color,int32_t bright,int32_t offset,int32_t flash,int32_t not3D);
int32_t judgeBlockRotate(int32_t player,int32_t bx1,int32_t by1,int32_t kind,int32_t rotate);
void PlaySE(int32_t no);
void hiddenProc(int32_t player);
void statAMove(int32_t player,int32_t kickm,int32_t kickr);
extern int32_t acol[7];
