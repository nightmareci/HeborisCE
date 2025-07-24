#ifndef effect_h_
#define effect_h_

#include "common.h"

void objectComboClearPl(int32_t player);
void objectClearPl(int32_t player);
void objectClear(void);
int32_t getBigBlock_add(int32_t player);
int32_t getBlockPlaneNo(int32_t player,int32_t color);
void objectplatina_erase(int32_t no);
void objectdelfield(int32_t no);
void objectitem_erase(int32_t no);
void objectGetmedal(int32_t no);
void objectItemWarning(int32_t no);
void objectNagareboshiC(int32_t no);
void objectNagareboshiP(int32_t no);
void objectHanabi(int32_t no);
void objectPopBlock(int32_t no);
void objectStatBlock2(int32_t no);
void objectAllCrear(int32_t no);
void objectStatLine(int32_t no);
void objectStatBlock(int32_t no);
void objectExecute(void);
void objectCreate2(int32_t p,int32_t s,int32_t x,int32_t y,int32_t v,int32_t w,int32_t j,int32_t c);
void objectCreate(int32_t p,int32_t s,int32_t x,int32_t y,int32_t v,int32_t w,int32_t j,int32_t c);
void effect(int32_t player);

#endif
