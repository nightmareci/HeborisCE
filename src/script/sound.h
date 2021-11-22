#pragma once

#include "script/include.h"

void printFont(int32_t fontX,int32_t fontY,const char *fontStr,int32_t fontColor);
void SoundTestProc(void);
void changeBGM(int32_t player);
void checkMasterFadeout(int32_t player);
void checkFadeout(int32_t pl);
void bgmFadeout(void);
int32_t ReplayBgmModeDecide(int32_t pl,int32_t mode,int32_t nv,int32_t dm,int32_t eg);
int32_t ReplaysetstartBGM(int32_t mode,int32_t pl);
int32_t setstartBGM_debug(int32_t mode,int32_t pl);
int32_t BgmModeDecide(int32_t pl,int32_t mode);
int32_t setstartBGM(int32_t mode,int32_t pl);
void PlayAllSE();
void StopSE(int32_t no);
void PlaySE(int32_t no);
void StopAllWaves(void);
void StopAllBGM(void);
void SetVolumeAllWaves(int32_t vol);
void SetVolumeAllBGM(int32_t vol);