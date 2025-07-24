#ifndef ranking2_h_
#define ranking2_h_

#include "common.h"

void printSMALLFont(int32_t fontX,int32_t fontY,const char *fontStr,int32_t fontColor);
void viewbesttimes(int32_t player);
void viewbesttime(int32_t player);
int32_t RankingGet2(int32_t rmode,int32_t rtype,int32_t rrots,int32_t player);
int32_t RankingLoad2();
void RankingSave2();
void getTime(int32_t time);
void getModeNameEx(int32_t mode,int32_t number);
void getRuleNameEx(int32_t rule,int32_t number);
void RankingProc2_2();
void RankingView2();
void RankingProc_2();
void RankingRegist2(int32_t rmode,int32_t rrots,int32_t rdata,int32_t rtime,int32_t rclear,int32_t rother,const char *rname,int32_t rank,int32_t rac,int32_t rst,int32_t rsk,int32_t rco,int32_t rre);
int32_t RankingGet(int32_t rmode,int32_t rrots);
int32_t ARSSRSch(int32_t rots);
int32_t RankingCheck2(int32_t rmode,int32_t rrots,int32_t rdata,int32_t rtime,int32_t rclear);
void RankingInit2();
extern int32_t modecolor[15];
extern int32_t rkre[5 *15 *2];
extern int32_t rkco[5 *15 *2];
extern int32_t rksk[5 *15 *2];
extern int32_t rkst[5 *15 *2];
extern int32_t rkac[5 *15 *2];
extern int32_t rkrots[5 *15 *2];
extern int32_t rkother[5 *15 *2];
extern int32_t rkclear[5 *15 *2];
extern int32_t rktime2[5 *15 *2];
extern int32_t rkdata[5 *15 *2];
extern char rkname[5 *15 *2][4];

#endif
