#pragma once

#include "script/include.h"

#define REPLAY_PLAYER_CHUNK (60 * 60 * 20)
#define REPLAY_CHUNK_SIZE (REPLAY_PLAYER_CHUNK * 2 * sizeof(int32_t))

#define SAVEBUF_2P_CHUNK 22000

// The maximum time that can be supported for replays, given the frame number is int32_t.
#define REPLAY_TIME_MAX (((INT32_MAX / REPLAY_PLAYER_CHUNK - INT32_C(2)) / INT32_C(2)) * REPLAY_PLAYER_CHUNK)

#define REPLAY_SIZE_1P(time) ((300 + ((time) + (time) % 2) / 2) * sizeof(int32_t))
#define REPLAY_SIZE_2P(time) \
((300 + \
        SAVEBUF_2P_CHUNK * 2 * (((time) + (time) % 2) / (SAVEBUF_2P_CHUNK * 2)) + \
        SAVEBUF_2P_CHUNK + ((((time) + (time) % 2) % (SAVEBUF_2P_CHUNK * 2)) / 2) \
) * sizeof(int32_t))

int32_t getModeAddRep2(int32_t *tmpBuf);
int32_t loadReplayData2(int32_t pl,int32_t number);
void ReplayDetail();
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
void ReplaySelectProc(void);
int32_t loadReplay_VS(int32_t number);
int32_t loadReplayData(int32_t pl,int32_t number);
void saveReplay_VS(int32_t number);
void saveReplayData(int32_t pl,int32_t number);
void freeReplayData(); // When done using the current replay data, this must be called.
void ReplaySaveCheck(int32_t player,int32_t statnumber);
