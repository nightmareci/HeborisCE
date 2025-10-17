#ifndef tomoyo_h_
#define tomoyo_h_

#include "common.h"

void randPlatinum(int32_t player,int32_t tgtnum);
void statTomoyoSelect_FP(int32_t player);
void ReplaySaveCheck(int32_t player,int32_t statnumber);
void statTomoyoResult(int32_t player);
void statTomoyoSelect(int32_t player);
int32_t getBlockPlaneNo(int32_t player,int32_t color);
void saveTomoyoStage(int32_t player,int32_t number);
void loadTomoyoStage(int32_t player,int32_t number);
void statTomoyoEditor(int32_t player);
int32_t getPlatinumBlock(int32_t player);
void tomoyoCheckStageClear(int32_t player);
void getTime(int32_t time);
void printSMALLFont(int32_t fontX,int32_t fontY,const char *fontStr,int32_t fontColor);
void objectCreate2(int32_t p,int32_t s,int32_t x,int32_t y,int32_t v,int32_t w,int32_t j,int32_t c);
int32_t Stage_RankingCheck(int32_t player,int32_t rmode);
void statTomoyoNextStage(int32_t player);
void tomoyoInitial(int32_t player);

#endif
