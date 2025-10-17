#ifndef practice_h_
#define practice_h_

#include "common.h"

void PracticeOver(void);
void viewLineInfo(void);
void StopAllBGM(void);
void PracticeDeath();
void hiddenProc(int32_t player);
void PracticeStart(void);
void loadWait(int32_t player,int32_t no);
void loadTomoyoStage(int32_t player,int32_t number);
extern int32_t ccol[7];
extern int32_t acol[7];
void PlaySE(WaveNum no);
void statSelectLevel(int32_t player);

#endif
