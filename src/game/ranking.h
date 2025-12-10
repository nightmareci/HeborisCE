#ifndef ranking_h_
#define ranking_h_

#include "app/APP.h"

int32_t RankingLoad(void);
int32_t RankingSave(void);
void printBIGFont(int32_t fontX,int32_t fontY,const char *fontStr,int32_t fontColor);
void getTime(int32_t time);
void RankingProc2_1(void);
int32_t RankingView(void);
void RankingCreate(int32_t cat,int32_t mode);
void RankingProc_1();
void RankingRegist(int32_t rmode,int32_t rtt,int32_t rsc,int32_t rli,int32_t rlv,int32_t rtime,int32_t end,const char *rname);
int32_t RankingCheck(int32_t rmode,int32_t rtt,int32_t rsc,int32_t rtime,int32_t rlv,int32_t end);
void RankingConvert(void);
void RankingInit(void);

#endif
