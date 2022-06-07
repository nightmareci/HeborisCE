#pragma once

#include "script/include.h"

int32_t getModeAddRep2(int32_t *tmpBuf);
int32_t loadReplayData2(int32_t pl,int32_t number);
void ReplayDetail(int32_t number);
void printSMALLFont(int32_t fontX,int32_t fontY,const char *fontStr,int32_t fontColor);
void getTime(int32_t time);
int32_t getModeAddRep(void);
int32_t ReplayBgmModeDecide(int32_t pl,int32_t mode,int32_t nv,int32_t dm,int32_t eg);
void PlaySE(int32_t no);
void missionSetStatus();
void loadMissionData(int32_t number);
int32_t setstartBGM(int32_t mode,int32_t pl);
void ReplaySelect(void);
void ReplaySelectInitial(void);
int32_t ReplaySelectProc(void);
int32_t loadReplay_VS(int32_t number);
int32_t loadReplayData(int32_t pl,int32_t number);
void saveReplay_VS(int32_t number);
void saveReplayData(int32_t pl,int32_t number);
void ReplaySaveCheck(int32_t player,int32_t statnumber);
