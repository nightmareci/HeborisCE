#pragma once

#include "script/include.h"

void drawTLSBlock(int32_t player,int32_t bottom);
void drawCBlock(int32_t player,int32_t color,int32_t bright,int32_t offset,int32_t flash,int32_t not3D);
void drawBigBlockNew(int32_t bx1,int32_t by1,int32_t kind,int32_t rotate,int32_t color,int32_t bright,int32_t offset,int32_t flash,int32_t player,int32_t use3D);
void drawBlock(int32_t bx1,int32_t by1,int32_t kind,int32_t rotate,int32_t color,int32_t bright,int32_t offset,int32_t flash,int32_t player,int32_t use3D);
void drawBigBlockFast(int32_t bx1,int32_t by1,int32_t kind,int32_t rotate,int32_t color,int32_t bright,int32_t offset,int32_t flash,int32_t player,int32_t use3D);
extern int32_t blkDataY[7 *4 *4];
extern int32_t blkDataX[7 *4 *4];
extern int32_t blkDDataY[7 *4 *4];
extern int32_t blkDDataX[7 *4 *4];
void printSMALLFontEX(int32_t fontX,int32_t fontY,const char *fontStr,int32_t fontColor);
void printFontEx(int32_t fontX,int32_t fontY,const char *fontStr,int32_t fontColor);
void viewFldBackground(void);
void drawhBlock(int32_t bx1,int32_t by1,int32_t kind,int32_t color,int32_t r,int32_t player,int32_t rotate,int32_t use3D);
void drawBlockFast(int32_t bx1,int32_t by1,int32_t kind,int32_t rotate,int32_t color,int32_t bright,int32_t offset,int32_t flash,int32_t player,int32_t use3D);
extern int32_t ccol[7];
extern int32_t acol[7];
void viewPreview(void);
extern int32_t cp_fld[10 *22 *2];
int32_t getBlockPlaneNo(int32_t player,int32_t color);
int32_t getBigBlock_add(int32_t player);
void viewEraserLines();
void viewFldFrame(int32_t uponly,int32_t i);
void viewField(void);
void viewMission();
void viewScore(void);
void viewLineInfo(void);
int32_t isScore(int32_t player);
void printBIGFont(int32_t fontX,int32_t fontY,const char *fontStr,int32_t fontColor);
void viewTime(void);
void printTinyFont(int32_t fontX,int32_t fontY,const char *fontStr);
void viewbesttimes3(int32_t player,int32_t x,int32_t y,int32_t type);
void viewbesttimes(int32_t player);
void viewbestSTtimes(int32_t player);
void printPrompt(int32_t fontX, int32_t fontY, EPrompt prompt, int32_t fontColor);
void printGameButton(int32_t fontX, int32_t fontY, EButton button, int32_t player);
#ifdef ENABLE_GAME_CONTROLLER
void printConKey(int32_t fontX, int32_t fontY, int32_t index, YGS2kSConKey* key, int32_t fontColor);
#endif
void printFont(int32_t fontX,int32_t fontY,const char *fontStr,int32_t fontColor);
void printSMALLFont(int32_t fontX,int32_t fontY,const char *fontStr,int32_t fontColor);
void viewScoreSmall(void);
void viewBack(void);
void pinchCheck2(int32_t player);
void pinchCheck(int32_t player);
void getSTime(int32_t time);
void getTime(int32_t time);
void disableShadowTimer(int32_t player);
void hiddenProc(int32_t player);
void showControl(void);
