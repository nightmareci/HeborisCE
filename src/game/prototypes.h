// ars.c
void statAMove(int player, int kickm, int kickr);

// classic.c
void statCMove(int player);
int judgeBigBlockRotate(int player, int bx1, int by1, int kind, int rotate);
int judgeBlockRotate(int player, int bx1, int by1, int kind, int rotate);

// classic_D.c
void statDMove(int player);

// config.c
int SaveConfig(void);
int LoadConfig(void);
void ConfigMenu();
void GameOption();

// cpu.c
int cpu_judgeBlock(int player, int bx1, int by1, int kind, int rotate);
int cpu_setBlock(int player, int bx1, int by1, int kind, int rotate);
int cpu_judgeBigBlock(int player, int bx1, int by1, int kind, int rotate);
int cpu_setBigBlock(int player, int bx1, int by1, int kind, int rotate);
void cpuMove(int player);
void cpuDrop(int player);
int cpuBlockHowManyFilled(int player, int y);
int blockHowManyFilledFromLeft(int player, int y);
int cpuCheckFloating(int player, int x, int y);
int cpuCheckFloatingE(int player, int x, int y);
int cpuCheckLeftRight(int player, int x, int y);
int cpu_blockEraseJudge(int player);
int cpu_HowManyNeedIblock(int player);
void cpuCopyField(int player);
int cpu_checkFieldTop(int player,int x);
int cpu_checkHoleStart(int player,int x);
void cpuCheckBestSpot(int player);

// effect.c
void effect(int player);
void objectCreate(int p, int s, int x, int y, int v, int w, int j, int c);
void objectCreate2(int p, int s, int x, int y, int v, int w, int j, int c);
void objectExecute(void);
void objectStatBlock(int no);
void objectStatBlock2(int no);
void objectStatLine(int no);
void objectAllCrear(int no);
void objectPopBlock(int no);
void objectHanabi(int no);
void objectplatina_erase(int no);
void objectNagareboshiP(int no);
void objectNagareboshiC(int no);
void objectItemWarning(int no);
void objectGetmedal(int no);
void objectitem_erase(int no);
void objectdelfield(int no);
void objectClear(void);
void objectClearPl(int player);
void objectComboClearPl(int player);

// flexdraw.c
void setDrawRate(int size);
int getDrawRate();
void ExBlt(int pno, int dx, int dy);
void ExBltRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy);
void ExBltFast(int pno, int dx, int dy);
void ExBltFastRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy);
void ExBlendBlt(int pno, int dx, int dy, int ar, int ag, int ab, int br, int bg, int bb);
void ExBlendBltRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int ar, int ag, int ab, int br, int bg, int bb);
void ExBltR(int pno, int dx, int dy, int scx, int scy);
void ExBltRectR(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int scx, int scy);
void ExBltFastR(int pno, int dx, int dy, int scx, int scy);
void ExBltFastRectR(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int scx, int scy);
void ExBltTrans(int pno, int dx, int dy);
void BlendExBlt(int pno, int dx, int dy, int ar, int ag, int ab, int br, int bg, int bb);
void BlendExBltRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int ar, int ag, int ab, int br, int bg, int bb);
void BlendExBltR(int pno, int dx, int dy, int ar, int ag, int ab, int br, int bg, int bb, int scx, int scy);
void BlendExBltRectR(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int ar, int ag, int ab, int br, int bg, int bb, int scx, int scy);
void ExTextLayerOn(int n, int x, int y);
void ExTextMove(int n, int x, int y);
void ExTextSize(int n, int size);
void ExTextHeight(int n, int size);
void ExCreateSurface(int pno, int x, int y);

// gamestart.c
void lastProc(void);
void title(void);
void enterSoloMode(int player);
void enterVersusMode(int player);
void enterPracticeMode(int player);
void enterMissionMode(int player);
void doDemoMode(void);
void playerInitial(int player);
void gameAllInit(void);
void versusInit(int player);
void tgmNextInit(int player);
void guidelineNextInit(int player);
void setNextBlockColors(int player, int first);
void playerExecute(void);
int doGiveup();
void increment_time(int player);
int isWRule(int player);
void statJoinwait(int player);
void statBlockSutter(int player);
void statSelectMode(int player);
int Admitgradecheck(int player);
void statSelectStandardSp(int player);
void statSelectStartLv(int player);
void setStartLevel(int pl);
void statReady(int player);
void statBlock(int player);
int checkGameOver(int player);
void setGameOver(int player);
void checkSecretGrade(int player);
void doIRS(int player);
void doIRS2(int player);
void doIRS2plus(int player);
void setBlockSpawnPosition(int player);
void statMove(int player);
void doHold(int player, int ihs);
int isTLS(int player);
int isTSpin(int player);
int blockEraseJudge(int player);
int blockBigEraseJudge(int player);
int blockEraseJudgeFf(int player,int mode);
void statRelayselect(int player);
void UpLineBlockJudge(int player);
void UpLineBlock(int player);
void statErase(int player);
int fldMirrorProc(int player);
int UpLineShirase(int player);
void LevelUp(int player);
void recSectionTime(int player);
void recStageTime(int player);
void recFaultTime(int player);
void statGameOver(int player);
void statEraseBlock(int player);
void grayoutLackedBlock(int player, int no);
void grayoutAllBlock(int player);
int checkSquare(int player, int x, int y, int gold);
int makeSquare(int player);
int getFieldBlockS(int player, int bx1, int by1);
int isComboMode(int player);
void calcScore(int player, int lines);
void checkEnding(int player, int tcbuf);
void statVersusWait(int player);
void statNameEntry(int player);
void statEnding(int player);
void statResult(int player);
void statReplaySave(int player);
void statVersusSelect(int player);
void winner();
void winner2();
void eraseItem(int player, int type);
void statusClear(int player);
void statGameOver2(int player);
void statNothing(int player);
void statDelField(int player);
void statLaser(int player);
void statNegafield(int player);
void statShotgun(int player);
void statExchangefield(int player);
void statExamination(int player);
void statItemRulet(int player);
void statFreefall(int player);
void statMovfield(int player);
void stat180field(int player);
void statDelfromUpper(int player);
void statBanana(int player);
void GiziSRand(int player);
int rand(int max,int player);
int abs_YGS2K(int i);
int getFieldBlock(int player, int bx1, int by1, int opt);
int getFieldBlock2(int player, int bx1, int by1);
int getFieldBlock3(int player, int bx1, int by1);
int getModeAdd(int player);
int checkFieldTop(int player);
int checkFieldTop2(int player,int x);
void scanItem(int player);
void padRepeat(int player);
void padRepeat2(int player);
int getPressState(int player, int key);
int getPushState(int player, int key);
int judgeBlock(int player, int bx1, int by1, int kind, int rotate);
void setBlock(int player, int bx1, int by1, int kind, int rotate);
int judgeBigBlock(int player, int bx1, int by1, int kind, int rotate);
void setBigBlock(int player, int bx1, int by1, int kind, int rotate);
void removeBlock(int player, int bx1, int by1, int kind, int rotate);
void removeBigBlock(int player, int bx1, int by1, int kind, int rotate);
void testmenu(void);
void initialize(void);
void LoadGraphics(char *nameStr, int p1, int p2);
void LoadTitle();
void LoadBackground(char *nameStr, int p1, int p2);
void loadGraphics(int players);
void loadBG(int players,int vsmode);
void loadWaves(void);
void loadBGM(void);
void backupSetups();
void restoreSetups();
int getMaxKey();
int IsPushDeleteKey();
int IsPushBSKey();
void spriteTime();

// grade.c
void GradeUp(int player);
void GradeUp2(int player, int lines);
void GradeUp3(int player);
void isregret(int player);
void reset_gradeup3(int player);

// mission.c
void statMissionSelect(int player);
void statMissionEditor(int player);
void viewMission();
void getMissionName(int mtype,int number);
void getRoadName(int no);
void missionNormUp(int lines);
void missionSetStatus();
void setEraserLines();
void viewEraserLines();
void loadTargetStage();
void loadMissionData(int number);
void saveMissionData(int number);

// practice.c
void statSelectLevel(int player);
void PracticeStart(void);
void loadWait(int player, int no);
void PracticeDeath();
void PracticeOver(void);

// ranking.c
void RankingInit(void);
void RankingConvert(void);
int RankingCheck(int rmode, int rtt, int rsc, int rtime, int rlv, int end);
void RankingRegist(int rmode, int rtt, int rsc, int rli, int rlv, int rtime, int end, char *rname);
void RankingProc_1(int cat);
void RankingProc2_1(void);
void RankingCreate(int cat, int mode);
int RankingView(void);
int RankingSave(void);
int RankingLoad(void);

// ranking2.c
void RankingInit2();
int RankingCheck2(int rmode, int rrots, int rdata, int rtime, int rclear);
void RankingRegist2(int rmode, int rrots, int rdata, int rtime, int rclear, int rother, char *rname, int rank, int rac, int rst, int rsk, int rco ,int rre);
void RankingProc_2(int cat);
void RankingProc2_2();
void RankingView2();
void RankingSave2();
int RankingLoad2();
int RankingGet(int rmode, int rrots);
int RankingGet2(int rmode,int rtype, int rrots,int player);
void RankingAlloc();
void getModeNameEx( int mode, int number );
void getRuleNameEx( int rule, int number );
void viewbesttime(int player);
void viewbesttimes(int player);
int ARSSRSch(int rots);

// ranking3.c
void RankingInit3();
int RankingCheck3(int rmode, int rex,int rrots, int rdata, int rtime, int rclear);
void RankingRegist3(int rmode, int rex,int rrots, int rdata, int rtime, int rclear, int rother, char *rname, int rank, int rac, int rst, int rsk, int rco ,int rre);
void RankingProc_3(int cat,int pages2);
void RankingProc2_3();
void RankingView3();
void RankingSave3();
int RankingLoad3();
int RankingGet3(int rmode, int rrots,int rex);
void RankingAlloc3();
void getModeNameEx3( int mode, int number );
void getRuleNameEx3( int rule, int number );
void viewbesttimes3(int player,int x,int y,int type);
void viewbesttime3(int player,int x,int y,int type);
int viewgrade(int player);

// readdef.c
namespace readdef
{
	int readdef();
};

// replay.c
void ReplaySaveCheck(int player, int statnumber);
void saveReplayData(int pl, int number);
void saveReplay_VS(int number);
int loadReplayData(int pl, int number);
int loadReplay_VS(int number);
int ReplaySelectProc(void);
int ReplayBgmModeDecide(int pl,int mode,int nv,int dm,int eg);
int getModeAddRep(void);
void ReplaySelectInitial(void);
void ReplaySelect(void);
void ReplayDetail(int number);
int loadReplayData2(int pl, int number);
int getModeAddRep2(int *tmpBuf);

// sectime.c
void ST_RankingInit(void);
int ST_RankingCheck(int player, int rmode, int section,int enable_grade);
int ST_RankingCheckAll(int player, int rmode, int enable_grade);
int Stage_RankingCheck(int player, int rmode);
void ST_RankingUpdate(int player, int rmode, int end,int enable_grade);
void ST_RankingSave(void);
int ST_RankingLoad(void);
void viewbestSTtime(int player);
void viewbestSTtimes(int player);
int ST_rankingGet(int player,int rmode,int enable_grade);
void ST_RankingView();
void PlayerdataSave(void);
int PlayerdataLoad(void);

// sound.c
void StopAllWaves(void);
void StopAllBGM(void);
void PlaySE(int no);
void StopSE(int no);
void PlayAllSE();
int setstartBGM(int mode, int pl);
int setstartBGM_debug(int mode, int pl);
int BgmModeDecide(int pl,int mode);
int ReplaysetstartBGM(int mode, int pl);
void bgmFadeout(void);
void checkFadeout(int pl);
void checkMasterFadeout(int player);
void changeBGM(int player);
void SoundTestProc(void);

// staffroll.c
void staffInitPl(int player);
void staffExecute(int player);
void staffRoll(int player);

// tomoyo.c
void tomoyoInitial(int player);
void statTomoyoNextStage(int player);
void tomoyoCheckStageClear(int player);
int getPlatinumBlock(int player);
void statTomoyoEditor(int player);
void statTomoyoSelect(int player);
void statTomoyoResult(int player);
void statTomoyoSelect_FP(int player);
void randPlatinum(int player, int tgtnum);
void loadTomoyoStage(int player, int number);
void saveTomoyoStage(int player, int number);

// view.c
void showControl(void);
void hiddenProc(int player);
void disableShadowTimer(int player);
void getTime(int time);
void getSTime(int time);
void pinchCheck(int player);
void pinchCheck2(int player);
void viewBack(void);
void viewScoreSmall(void);
void viewTime(void);
void viewLineInfo(void);
int isScore(int player);
void viewScore(void);
void viewField(void);
void viewPreview(void);
void viewFldBackground(void);
void viewFldFrame(int uponly,int i);
void printFont(int fontX, int fontY, char *fontStr, int fontColor);
void printFontEx(int fontX, int fontY, char *fontStr, int fontColor);
void printSMALLFont(int fontX, int fontY, char *fontStr, int fontColor);
void printSMALLFontEX(int fontX, int fontY, char *fontStr, int fontColor);
void printBIGFont(int fontX, int fontY, char *fontStr, int fontColor);
void printTinyFont(int fontX, int fontY, char *fontStr);
int getBlockPlaneNo(int player, int color);
int getBigBlock_add(int player);
void drawBlockFast(int bx1, int by1, int kind, int rotate, int color, int bright, int offset, int flash, int player, int use3D);
void drawBigBlockFast(int bx1, int by1, int kind, int rotate, int color, int bright, int offset, int flash, int player, int use3D);
void drawBlock(int bx1, int by1, int kind, int rotate, int color, int bright, int offset, int flash, int player, int use3D);
void drawBigBlockNew(int bx1, int by1, int kind, int rotate, int color, int bright, int offset, int flash, int player, int use3D);
void drawCBlock(int player, int color, int bright, int offset, int flash, int not3D);
void drawTLSBlock(int player, int bottom);
void drawhBlock(int bx1, int by1, int kind, int color, int r, int player, int rotate, int use3D);

// world.c
void statWMove(int player, int kickm, int kickr);
int	co(int a, int b, int c);

// game/readini.c
void ReadIniSection(char *section, char *value);
void ReadIniSectionString(char *section, char *value);
