#pragma once

#include <stdint.h>

// ars.c
void statAMove(int32_t player, int32_t kickm, int32_t kickr);

// classic.c
void statCMove(int32_t player);
int32_t judgeBigBlockRotate(int32_t player, int32_t bx1, int32_t by1, int32_t kind, int32_t rotate);
int32_t judgeBlockRotate(int32_t player, int32_t bx1, int32_t by1, int32_t kind, int32_t rotate);

// classic_D.c
void statDMove(int32_t player);

// config.c
int32_t SaveConfig(void);
int32_t LoadConfig(void);
void ConfigMenu();
void GameOption();

// cpu.c
int32_t cpu_judgeBlock(int32_t player, int32_t bx1, int32_t by1, int32_t kind, int32_t rotate);
int32_t cpu_setBlock(int32_t player, int32_t bx1, int32_t by1, int32_t kind, int32_t rotate);
int32_t cpu_judgeBigBlock(int32_t player, int32_t bx1, int32_t by1, int32_t kind, int32_t rotate);
int32_t cpu_setBigBlock(int32_t player, int32_t bx1, int32_t by1, int32_t kind, int32_t rotate);
void cpuMove(int32_t player);
void cpuDrop(int32_t player);
int32_t cpuBlockHowManyFilled(int32_t player, int32_t y);
int32_t blockHowManyFilledFromLeft(int32_t player, int32_t y);
int32_t cpuCheckFloating(int32_t player, int32_t x, int32_t y);
int32_t cpuCheckFloatingE(int32_t player, int32_t x, int32_t y);
int32_t cpuCheckLeftRight(int32_t player, int32_t x, int32_t y);
int32_t cpu_blockEraseJudge(int32_t player);
int32_t cpu_HowManyNeedIblock(int32_t player);
void cpuCopyField(int32_t player);
int32_t cpu_checkFieldTop(int32_t player,int32_t x);
int32_t cpu_checkHoleStart(int32_t player,int32_t x);
void cpuCheckBestSpot(int32_t player);

// effect.c
void effect(int32_t player);
void objectCreate(int32_t p, int32_t s, int32_t x, int32_t y, int32_t v, int32_t w, int32_t j, int32_t c);
void objectCreate2(int32_t p, int32_t s, int32_t x, int32_t y, int32_t v, int32_t w, int32_t j, int32_t c);
void objectExecute(void);
void objectStatBlock(int32_t no);
void objectStatBlock2(int32_t no);
void objectStatLine(int32_t no);
void objectAllCrear(int32_t no);
void objectPopBlock(int32_t no);
void objectHanabi(int32_t no);
void objectplatina_erase(int32_t no);
void objectNagareboshiP(int32_t no);
void objectNagareboshiC(int32_t no);
void objectItemWarning(int32_t no);
void objectGetmedal(int32_t no);
void objectitem_erase(int32_t no);
void objectdelfield(int32_t no);
void objectClear(void);
void objectClearPl(int32_t player);
void objectComboClearPl(int32_t player);

// flexdraw.c
void setDrawRate(int32_t size);
int32_t getDrawRate();
void ExBlt(int32_t pno, int32_t dx, int32_t dy);
void ExBltRect(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy);
void ExBltFast(int32_t pno, int32_t dx, int32_t dy);
void ExBltFastRect(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy);
void ExBlendBlt(int32_t pno, int32_t dx, int32_t dy, int32_t ar, int32_t ag, int32_t ab, int32_t br, int32_t bg, int32_t bb);
void ExBlendBltRect(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy, int32_t ar, int32_t ag, int32_t ab, int32_t br, int32_t bg, int32_t bb);
void ExBltR(int32_t pno, int32_t dx, int32_t dy, int32_t scx, int32_t scy);
void ExBltRectR(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy, int32_t scx, int32_t scy);
void ExBltFastR(int32_t pno, int32_t dx, int32_t dy, int32_t scx, int32_t scy);
void ExBltFastRectR(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy, int32_t scx, int32_t scy);
void ExBltTrans(int32_t pno, int32_t dx, int32_t dy);
void BlendExBlt(int32_t pno, int32_t dx, int32_t dy, int32_t ar, int32_t ag, int32_t ab, int32_t br, int32_t bg, int32_t bb);
void BlendExBltRect(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy, int32_t ar, int32_t ag, int32_t ab, int32_t br, int32_t bg, int32_t bb);
void BlendExBltR(int32_t pno, int32_t dx, int32_t dy, int32_t ar, int32_t ag, int32_t ab, int32_t br, int32_t bg, int32_t bb, int32_t scx, int32_t scy);
void BlendExBltRectR(int32_t pno, int32_t dx, int32_t dy, int32_t sx, int32_t sy, int32_t hx, int32_t hy, int32_t ar, int32_t ag, int32_t ab, int32_t br, int32_t bg, int32_t bb, int32_t scx, int32_t scy);
void ExTextLayerOn(int32_t n, int32_t x, int32_t y);
void ExTextMove(int32_t n, int32_t x, int32_t y);
void ExTextSize(int32_t n, int32_t size);
void ExTextHeight(int32_t n, int32_t size);
void ExCreateSurface(int32_t pno, int32_t x, int32_t y);

// gamestart.c
void lastProc(void);
void title(void);
void enterSoloMode(int32_t player);
void enterVersusMode(int32_t player);
void enterPracticeMode(int32_t player);
void enterMissionMode(int32_t player);
void doDemoMode(void);
void playerInitial(int32_t player);
void gameAllInit(void);
void versusInit(int32_t player);
void tgmNextInit(int32_t player);
void guidelineNextInit(int32_t player);
void setNextBlockColors(int32_t player, int32_t first);
void playerExecute(void);
int32_t doGiveup();
void increment_time(int32_t player);
int32_t isWRule(int32_t player);
void statJoinwait(int32_t player);
void statBlockSutter(int32_t player);
void statSelectMode(int32_t player);
int32_t Admitgradecheck(int32_t player);
void statSelectStandardSp(int32_t player);
void statSelectStartLv(int32_t player);
void setStartLevel(int32_t pl);
void statReady(int32_t player);
void statBlock(int32_t player);
int32_t checkGameOver(int32_t player);
void setGameOver(int32_t player);
void checkSecretGrade(int32_t player);
void doIRS(int32_t player);
void doIRS2(int32_t player);
void doIRS2plus(int32_t player);
void setBlockSpawnPosition(int32_t player);
void statMove(int32_t player);
void doHold(int32_t player, int32_t ihs);
int32_t isTLS(int32_t player);
int32_t isTSpin(int32_t player);
int32_t blockEraseJudge(int32_t player);
int32_t blockBigEraseJudge(int32_t player);
int32_t blockEraseJudgeFf(int32_t player,int32_t mode);
void statRelayselect(int32_t player);
void UpLineBlockJudge(int32_t player);
void UpLineBlock(int32_t player);
void statErase(int32_t player);
int32_t fldMirrorProc(int32_t player);
int32_t UpLineShirase(int32_t player);
void LevelUp(int32_t player);
void recSectionTime(int32_t player);
void recStageTime(int32_t player);
void recFaultTime(int32_t player);
void statGameOver(int32_t player);
void statEraseBlock(int32_t player);
void grayoutLackedBlock(int32_t player, int32_t no);
void grayoutAllBlock(int32_t player);
int32_t checkSquare(int32_t player, int32_t x, int32_t y, int32_t gold);
int32_t makeSquare(int32_t player);
int32_t getFieldBlockS(int32_t player, int32_t bx1, int32_t by1);
int32_t isComboMode(int32_t player);
void calcScore(int32_t player, int32_t lines);
void checkEnding(int32_t player, int32_t tcbuf);
void statVersusWait(int32_t player);
void statNameEntry(int32_t player);
void statEnding(int32_t player);
void statResult(int32_t player);
void statReplaySave(int32_t player);
void statVersusSelect(int32_t player);
void winner();
void winner2();
void eraseItem(int32_t player, int32_t type);
void statusClear(int32_t player);
void statGameOver2(int32_t player);
void statNothing(int32_t player);
void statDelField(int32_t player);
void statLaser(int32_t player);
void statNegafield(int32_t player);
void statShotgun(int32_t player);
void statExchangefield(int32_t player);
void statExamination(int32_t player);
void statItemRulet(int32_t player);
void statFreefall(int32_t player);
void statMovfield(int32_t player);
void stat180field(int32_t player);
void statDelfromUpper(int32_t player);
void statBanana(int32_t player);
void GiziSRand(int32_t player);
int32_t gameRand(int32_t max,int32_t player);
int32_t abs_YGS2K(int32_t i);
int32_t getFieldBlock(int32_t player, int32_t bx1, int32_t by1, int32_t opt);
int32_t getFieldBlock2(int32_t player, int32_t bx1, int32_t by1);
int32_t getFieldBlock3(int32_t player, int32_t bx1, int32_t by1);
int32_t getModeAdd(int32_t player);
int32_t checkFieldTop(int32_t player);
int32_t checkFieldTop2(int32_t player,int32_t x);
void scanItem(int32_t player);
void padRepeat(int32_t player);
void padRepeat2(int32_t player);
int32_t getPressState(int32_t player, int32_t key);
int32_t getPushState(int32_t player, int32_t key);
int32_t judgeBlock(int32_t player, int32_t bx1, int32_t by1, int32_t kind, int32_t rotate);
void setBlock(int32_t player, int32_t bx1, int32_t by1, int32_t kind, int32_t rotate);
int32_t judgeBigBlock(int32_t player, int32_t bx1, int32_t by1, int32_t kind, int32_t rotate);
void setBigBlock(int32_t player, int32_t bx1, int32_t by1, int32_t kind, int32_t rotate);
void removeBlock(int32_t player, int32_t bx1, int32_t by1, int32_t kind, int32_t rotate);
void removeBigBlock(int32_t player, int32_t bx1, int32_t by1, int32_t kind, int32_t rotate);
void testmenu(void);
void initialize(void);
void LoadGraphics(const char *nameStr, int32_t p1, int32_t p2);
void LoadTitle();
void LoadBackground(const char *nameStr, int32_t p1, int32_t p2);
void loadGraphics(int32_t players);
void loadBG(int32_t players,int32_t vsmode);
void loadWaves(void);
void loadBGM(void);
void backupSetups();
void restoreSetups();
int32_t getMaxKey();
int32_t IsPushDeleteKey();
int32_t IsPushBSKey();
void spriteTime();

// grade.c
void GradeUp(int32_t player);
void GradeUp2(int32_t player, int32_t lines);
void GradeUp3(int32_t player);
void isregret(int32_t player);
void reset_gradeup3(int32_t player);

// mission.c
void statMissionSelect(int32_t player);
void statMissionEditor(int32_t player);
void viewMission();
void getMissionName(int32_t mtype,int32_t number);
void getRoadName(int32_t no);
void missionNormUp(int32_t lines);
void missionSetStatus();
void setEraserLines();
void viewEraserLines();
void loadTargetStage();
void loadMissionData(int32_t number);
void saveMissionData(int32_t number);

// practice.c
void statSelectLevel(int32_t player);
void PracticeStart(void);
void loadWait(int32_t player, int32_t no);
void PracticeDeath();
void PracticeOver(void);

// ranking.c
void RankingInit(void);
void RankingConvert(void);
int32_t RankingCheck(int32_t rmode, int32_t rtt, int32_t rsc, int32_t rtime, int32_t rlv, int32_t end);
void RankingRegist(int32_t rmode, int32_t rtt, int32_t rsc, int32_t rli, int32_t rlv, int32_t rtime, int32_t end, const char *rname);
void RankingProc_1(int32_t cat);
void RankingProc2_1(void);
void RankingCreate(int32_t cat, int32_t mode);
int32_t RankingView(void);
int32_t RankingSave(void);
int32_t RankingLoad(void);

// ranking2.c
void RankingInit2();
int32_t RankingCheck2(int32_t rmode, int32_t rrots, int32_t rdata, int32_t rtime, int32_t rclear);
void RankingRegist2(int32_t rmode, int32_t rrots, int32_t rdata, int32_t rtime, int32_t rclear, int32_t rother, const char *rname, int32_t rank, int32_t rac, int32_t rst, int32_t rsk, int32_t rco ,int32_t rre);
void RankingProc_2(int32_t cat);
void RankingProc2_2();
void RankingView2();
void RankingSave2();
int32_t RankingLoad2();
int32_t RankingGet(int32_t rmode, int32_t rrots);
int32_t RankingGet2(int32_t rmode,int32_t rtype, int32_t rrots,int32_t player);
void RankingAlloc();
void getModeNameEx( int32_t mode, int32_t number );
void getRuleNameEx( int32_t rule, int32_t number );
void viewbesttime(int32_t player);
void viewbesttimes(int32_t player);
int32_t ARSSRSch(int32_t rots);

// ranking3.c
void RankingInit3();
int32_t RankingCheck3(int32_t rmode, int32_t rex,int32_t rrots, int32_t rdata, int32_t rtime, int32_t rclear);
void RankingRegist3(int32_t rmode, int32_t rex,int32_t rrots, int32_t rdata, int32_t rtime, int32_t rclear,
	int32_t rother, char *rname, int32_t rank, int32_t rac, int32_t rst, int32_t rsk, int32_t rco ,int32_t rre);
void RankingProc_3(int32_t cat,int32_t pages2);
void RankingProc2_3();
void RankingView3();
void RankingSave3();
int32_t RankingLoad3();
int32_t RankingGet3(int32_t rmode, int32_t rrots,int32_t rex);
void RankingAlloc3();
void getModeNameEx3( int32_t mode, int32_t number );
void getRuleNameEx3( int32_t rule, int32_t number );
void viewbesttimes3(int32_t player,int32_t x,int32_t y,int32_t type);
void viewbesttime3(int32_t player,int32_t x,int32_t y,int32_t type);
int32_t viewgrade(int32_t player);

// readdef.c
int32_t readdef();

// replay.c
void ReplaySaveCheck(int32_t player, int32_t statnumber);
void saveReplayData(int32_t pl, int32_t number);
void saveReplay_VS(int32_t number);
int32_t loadReplayData(int32_t pl, int32_t number);
int32_t loadReplay_VS(int32_t number);
int32_t ReplaySelectProc(void);
int32_t ReplayBgmModeDecide(int32_t pl,int32_t mode,int32_t nv,int32_t dm,int32_t eg);
int32_t getModeAddRep(void);
void ReplaySelectInitial(void);
void ReplaySelect(void);
void ReplayDetail(int32_t number);
int32_t loadReplayData2(int32_t pl, int32_t number);
int32_t getModeAddRep2(int32_t *tmpBuf);

// sectime.c
void ST_RankingInit(void);
int32_t ST_RankingCheck(int32_t player, int32_t rmode, int32_t section,int32_t enable_grade);
int32_t ST_RankingCheckAll(int32_t player, int32_t rmode, int32_t enable_grade);
int32_t Stage_RankingCheck(int32_t player, int32_t rmode);
void ST_RankingUpdate(int32_t player, int32_t rmode, int32_t end,int32_t enable_grade);
void ST_RankingSave(void);
int32_t ST_RankingLoad(void);
void viewbestSTtime(int32_t player);
void viewbestSTtimes(int32_t player);
int32_t ST_rankingGet(int32_t player,int32_t rmode,int32_t enable_grade);
void ST_RankingView();
void PlayerdataSave(void);
int32_t PlayerdataLoad(void);

// sound.c
void StopAllWaves(void);
void StopAllBGM(void);
void PlaySE(int32_t no);
void StopSE(int32_t no);
void PlayAllSE();
int32_t setstartBGM(int32_t mode, int32_t pl);
int32_t setstartBGM_debug(int32_t mode, int32_t pl);
int32_t BgmModeDecide(int32_t pl,int32_t mode);
int32_t ReplaysetstartBGM(int32_t mode, int32_t pl);
void bgmFadeout(void);
void checkFadeout(int32_t pl);
void checkMasterFadeout(int32_t player);
void changeBGM(int32_t player);
void SoundTestProc(void);

// staffroll.c
void staffInitPl(int32_t player);
void staffExecute(int32_t player);
void staffRoll(int32_t player);

// tomoyo.c
void tomoyoInitial(int32_t player);
void statTomoyoNextStage(int32_t player);
void tomoyoCheckStageClear(int32_t player);
int32_t getPlatinumBlock(int32_t player);
void statTomoyoEditor(int32_t player);
void statTomoyoSelect(int32_t player);
void statTomoyoResult(int32_t player);
void statTomoyoSelect_FP(int32_t player);
void randPlatinum(int32_t player, int32_t tgtnum);
void loadTomoyoStage(int32_t player, int32_t number);
void saveTomoyoStage(int32_t player, int32_t number);

// view.c
void showControl(void);
void hiddenProc(int32_t player);
void disableShadowTimer(int32_t player);
void getTime(int32_t time);
void getSTime(int32_t time);
void pinchCheck(int32_t player);
void pinchCheck2(int32_t player);
void viewBack(void);
void viewScoreSmall(void);
void viewTime(void);
void viewLineInfo(void);
int32_t isScore(int32_t player);
void viewScore(void);
void viewField(void);
void viewPreview(void);
void viewFldBackground(void);
void viewFldFrame(int32_t uponly,int32_t i);
void printFont(int32_t fontX, int32_t fontY, const char *fontStr, int32_t fontColor);
void printFontEx(int32_t fontX, int32_t fontY, const char *fontStr, int32_t fontColor);
void printSMALLFont(int32_t fontX, int32_t fontY, const char *fontStr, int32_t fontColor);
void printSMALLFontEX(int32_t fontX, int32_t fontY, const char *fontStr, int32_t fontColor);
void printBIGFont(int32_t fontX, int32_t fontY, const char *fontStr, int32_t fontColor);
void printTinyFont(int32_t fontX, int32_t fontY, const char *fontStr);
int32_t getBlockPlaneNo(int32_t player, int32_t color);
int32_t getBigBlock_add(int32_t player);
void drawBlockFast(int32_t bx1, int32_t by1, int32_t kind, int32_t rotate, int32_t color, int32_t bright, int32_t offset, int32_t flash, int32_t player, int32_t use3D);
void drawBigBlockFast(int32_t bx1, int32_t by1, int32_t kind, int32_t rotate, int32_t color, int32_t bright, int32_t offset, int32_t flash, int32_t player, int32_t use3D);
void drawBlock(int32_t bx1, int32_t by1, int32_t kind, int32_t rotate, int32_t color, int32_t bright, int32_t offset, int32_t flash, int32_t player, int32_t use3D);
void drawBigBlockNew(int32_t bx1, int32_t by1, int32_t kind, int32_t rotate, int32_t color, int32_t bright, int32_t offset, int32_t flash, int32_t player, int32_t use3D);
void drawCBlock(int32_t player, int32_t color, int32_t bright, int32_t offset, int32_t flash, int32_t not3D);
void drawTLSBlock(int32_t player, int32_t bottom);
void drawhBlock(int32_t bx1, int32_t by1, int32_t kind, int32_t color, int32_t r, int32_t player, int32_t rotate, int32_t use3D);

// world.c
void statWMove(int32_t player, int32_t kickm, int32_t kickr);
int32_t	co(int32_t a, int32_t b, int32_t c);

// game/readini.c
void ReadIniSection(const char *section, const char *value);
void ReadIniSectionString(const char *section, const char *value);
