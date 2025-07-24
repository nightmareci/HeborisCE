#ifndef classic_h_
#define classic_h_

#include "common.h"

int32_t judgeBigBlockRotate(int32_t player,int32_t bx1,int32_t by1,int32_t kind,int32_t rotate);
void effect(int32_t player);
void pinchCheck(int32_t player);
void drawTLSBlock(int32_t player,int32_t bottom);
void drawCBlock(int32_t player,int32_t color,int32_t bright,int32_t offset,int32_t flash,int32_t not3D);
int32_t judgeBlockRotate(int32_t player,int32_t bx1,int32_t by1,int32_t kind,int32_t rotate);
void PlaySE(int32_t no);
void hiddenProc(int32_t player);
void statCMove(int32_t player);
extern int32_t ccol[7];
extern int32_t blkDataY[7 *4 *4];
extern int32_t blkDataX[7 *4 *4];

#endif
