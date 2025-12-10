#ifndef mission_h_
#define mission_h_

#include "app/APP.h"

void randPlatinum(int32_t player,int32_t tgtnum);
void loadTomoyoStage(int32_t player,int32_t number);
void loadTargetStage();
void viewEraserLines();
void changeBGM(int32_t player);
void objectCreate2(int32_t p,int32_t s,int32_t x,int32_t y,int32_t v,int32_t w,int32_t j,int32_t c);
void setEraserLines();
extern int32_t blkDataY[7 *4 *4];
extern int32_t blkDDataY[7 *4 *4];
int32_t getPlatinumBlock(int32_t player);
void missionNormUp(int32_t lines);
void drawBlockFast(int32_t bx1,int32_t by1,int32_t kind,int32_t rotate,int32_t color,int32_t bright,int32_t offset,int32_t flash,int32_t player,int32_t use3D);
extern int32_t ccol[7];
extern int32_t acol[7];
void getMissionName(int32_t mtype,int32_t number);
void viewMission();
void getTime(int32_t time);
void statMissionEditor(int32_t player);
void saveMissionData(int32_t number);
void missionSetStatus();
void StopAllBGM(void);
void loadMissionData(int32_t number);
void getRoadName(int32_t no);
void statMissionSelect(int32_t player);

#endif
