#ifndef ranking3_h_
#define ranking3_h_

#include "common.h"

int32_t viewgrade(int32_t player);
void viewbesttime3(int32_t player,int32_t x,int32_t y,int32_t type);
void printSMALLFont(int32_t fontX,int32_t fontY,const char *fontStr,int32_t fontColor);
void viewbesttimes3(int32_t player,int32_t x,int32_t y,int32_t type);
int32_t RankingLoad3();
void RankingSave3();
void getTime(int32_t time);
void getModeNameEx3(int32_t mode,int32_t number);
void getRuleNameEx3(int32_t rule,int32_t number);
void RankingProc2_3();
void RankingView3();
void RankingProc_3();
void RankingRegist3(int32_t rmode,int32_t rex,int32_t rrots,int32_t rdata,int32_t rtime,int32_t rclear,int32_t rother,char *rname,int32_t rank,int32_t rac,int32_t rst,int32_t rsk,int32_t rco,int32_t rre);
int32_t ARSSRSch(int32_t rots);
int32_t RankingGet3(int32_t rmode,int32_t rrots,int32_t rex);
int32_t RankingCheck3(int32_t rmode,int32_t rex,int32_t rrots,int32_t rdata,int32_t rtime,int32_t rclear);
void RankingInit3();
extern int32_t rkpage;
extern int32_t rkpages2;
extern int32_t modecolor3[15];
extern int32_t rkre3[6 *14 *2];
extern int32_t rkco3[6 *14 *2];
extern int32_t rksk3[6 *14 *2];
extern int32_t rkst3[6 *14 *2];
extern int32_t rkac3[6 *14 *2];
extern int32_t rkrots3[6 *14 *2];
extern int32_t rkother3[6 *14 *2];
extern int32_t rkclear3[6 *14 *2];
extern int32_t rktime3[6 *14 *2];
extern int32_t rkdata3[6 *14 *2];
extern char rkname3[6 *14 *2][4];
extern bool allocked3;

#endif
