#ifndef world_h_
#define world_h_

#include "common.h"

extern int32_t blockflash;
extern int32_t sonicdrop;
extern int32_t downtype;
extern int32_t nanameallow;
extern int32_t movesound;
extern int32_t smooth;
int32_t co(int32_t a,int32_t b,int32_t c);
extern int32_t rots[2];
extern int32_t w_reverse;
void statWMove(int32_t player,int32_t kickm,int32_t kickr);
extern int32_t iBlock180KickTable[11 *2 *4];
extern int32_t otherBlock180KickTable[11 *2 *4];
extern int32_t iBlockKickTable[4 *4 *2 *2];
extern int32_t wcol[7];
extern int32_t blkWDataY[7 *4 *4];
extern int32_t blkWDataX[7 *4 *4];

#endif
