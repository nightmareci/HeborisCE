#pragma once

#include "../script/include.h"

int32_t PlayerdataLoad(void);
void PlayerdataSave(void);
void PlaySE(int32_t no);
void getSTime(int32_t time);
void ST_RankingView();
void printSMALLFont(int32_t fontX,int32_t fontY,const char *fontStr,int32_t fontColor);
void viewbestSTtimes(int32_t player);
void getTime(int32_t time);
void printFont(int32_t fontX,int32_t fontY,const char *fontStr,int32_t fontColor);
void viewbestSTtime(int32_t player);
int32_t ST_RankingLoad(void);
void ST_RankingSave(void);
void ST_RankingUpdate(int32_t player,int32_t rmode,int32_t end,int32_t enable_grade);
int32_t Stage_RankingCheck(int32_t player,int32_t rmode);
int32_t ST_RankingCheckAll(int32_t player,int32_t rmode,int32_t enable_grade);
int32_t ST_rankingGet(int32_t player,int32_t rmode,int32_t enable_grade);
int32_t ST_RankingCheck(int32_t player,int32_t rmode,int32_t section,int32_t enable_grade);
void ST_RankingInit(void);
extern int32_t st_others[90];
extern int32_t st_lvstop[90];
extern int32_t st_end[90];
extern int32_t st_time[90];
extern int32_t st_version;
