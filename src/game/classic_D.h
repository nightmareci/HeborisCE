#ifndef classic_D_h_
#define classic_D_h_

#include "app/APP.h"
#include "sound.h"

void effect(int32_t player);
void pinchCheck(int32_t player);
void drawTLSBlock(int32_t player,int32_t bottom);
void drawCBlock(int32_t player,int32_t color,int32_t bright,int32_t offset,int32_t flash,int32_t not3D);
extern int32_t ccol[7];
void PlaySE(WaveNum no);
void hiddenProc(int32_t player);
void statDMove(int32_t player);
extern int32_t blkDDataY[7 *4 *4];
extern int32_t blkDDataX[7 *4 *4];

#endif
