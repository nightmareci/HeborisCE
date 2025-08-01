#ifndef gamestart_h_
#define gamestart_h_

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  外部ライブラリのインポート
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
#include "common.h"

#define		STRING_MAX		200
#define		loop			while ( loopFlag )
#define		halt			spriteTime()
#define		ctime			cgametime
#define		stime			sgametime
#define		wait1			gamewait1
#define		wait2			gamewait2
#define		wait3			gamewait3
#define		waitt			gamewaitt

void loadBG(int32_t players,int32_t vsmode);
void LoadBackground(const char *nameStr,int32_t p1,int32_t p2);
void LoadTitle();
void loadBGM(void);
void loadWaves(void);
void loadGraphics(int32_t players);
void LoadGraphic(const char *nameStr,int32_t p1,int32_t p2);
void removeBigBlock(int32_t player,int32_t bx1,int32_t by1,int32_t kind,int32_t rotate);
void setBigBlock(int32_t player,int32_t bx1,int32_t by1,int32_t kind,int32_t rotate);
int32_t judgeBigBlock(int32_t player,int32_t bx1,int32_t by1,int32_t kind,int32_t rotate);
int32_t checkFieldTop2(int32_t player,int32_t x);
int32_t getFieldBlock3(int32_t player,int32_t bx1,int32_t by1);
int32_t getFieldBlock2(int32_t player,int32_t bx1,int32_t by1);
int32_t getModeAdd(int32_t player);
int32_t getFieldBlockS(int32_t player,int32_t bx1,int32_t by1);
int32_t getFieldBlock(int32_t player,int32_t bx1,int32_t by1,int32_t opt);
int32_t checkSquare(int32_t player,int32_t x,int32_t y,int32_t gold);
void grayoutAllBlock(int32_t player);
void eraseItem(int32_t player,int32_t type);
void calcScore(int32_t player,int32_t lines);
int32_t isComboMode(int32_t player);
void grayoutLackedBlock(int32_t player,int32_t no);
void recSectionTime(int32_t player);
void LevelUp(int32_t player);
int32_t fldMirrorProc(int32_t player);
int32_t UpLineShirase(int32_t player);
int32_t makeSquare(int32_t player);
void recStageTime(int32_t player);
void UpLineBlock(int32_t player);
void UpLineBlockJudge(int32_t player);
int32_t blockEraseJudgeFf(int32_t player,int32_t mode);
int32_t blockBigEraseJudge(int32_t player);
void removeBlock(int32_t player,int32_t bx1,int32_t by1,int32_t kind,int32_t rotate);
int32_t blockEraseJudge(int32_t player);
int32_t isTSpin(int32_t player);
int32_t isTLS(int32_t player);
void scanItem(int32_t player);
void doIRS2plus(int32_t player);
void checkSecretGrade(int32_t player);
void setBlock(int32_t player,int32_t bx1,int32_t by1,int32_t kind,int32_t rotate);
int32_t checkGameOver(int32_t player);
void doIRS(int32_t player);
void doHold(int32_t player,int32_t ihs);
int32_t judgeBlock(int32_t player,int32_t bx1,int32_t by1,int32_t kind,int32_t rotate);
void setBlockSpawnPosition(int32_t player);
void doIRS2(int32_t player);
int32_t Admitgradecheck(int32_t player);
void GiziSRand(int32_t player);
void padRepeat(int32_t player);
void checkEnding(int32_t player,int32_t tcbuf);
void setGameOver(int32_t player);
void statusClear(int32_t player);
void increment_time(int32_t player);
void statBanana(int32_t player);
void statReplaySave(int32_t player);
void statSelectStandardSp(int32_t player);
void statDelfromUpper(int32_t player);
void stat180field(int32_t player);
void statMovfield(int32_t player);
void statFreefall(int32_t player);
void statItemRulet(int32_t player);
void statExamination(int32_t player);
void statExchangefield(int32_t player);
void statShotgun(int32_t player);
void statNegafield(int32_t player);
void statLaser(int32_t player);
void statDelField(int32_t player);
void statNothing(int32_t player);
void statGameOver2(int32_t player);
void statVersusSelect(int32_t player);
void statRelayselect(int32_t player);
void statResult(int32_t player);
void statEnding(int32_t player);
void statSelectStartLv(int32_t player);
void statNameEntry(int32_t player);
void statVersusWait(int32_t player);
void statEraseBlock(int32_t player);
void statGameOver(int32_t player);
void statErase(int32_t player);
void statMove(int32_t player);
void statBlock(int32_t player);
void statReady(int32_t player);
void statSelectMode(int32_t player);
void statBlockSutter(int32_t player);
void statJoinwait(int32_t player);
int32_t checkFieldTop(int32_t player);
void recFaultTime(int32_t player);
void winner2();
void winner();
int32_t doGiveup();
int32_t gameRand(int32_t max,int32_t player);
int32_t isWRule(int32_t player);
void tgmNextInit(int32_t player);
void tgm3NextInit(int32_t player);
void SakuraNextInit(int32_t player);
int32_t MostDroughtedPiece(int32_t histogram[7]);
void guidelineNextInit(int32_t player);
int32_t TGMPiece(uint32_t *tgmgseed);
uint32_t LCGRand(uint32_t *lcgseed);
int32_t TGMConvert(int32_t piece);
void playerInitial(int32_t player);
void setNextBlockColors(int32_t player,int32_t first);
void gameAllInit(void);
void versusInit(int32_t player);
void enterMissionMode(int32_t player);
void enterPracticeMode(int32_t player);
void setStartLevel(int32_t pl);
void enterVersusMode(int32_t player);
void enterSoloMode(int32_t player);
void padRepeat2(int32_t player);
void doDemoMode(void);
void testmenu(void);
bool lastProc(void);
bool playerExecute(void);
void restoreSetups();

int32_t getPressState(int32_t player, APP_Button key);
int32_t getPushState(int32_t player, APP_Button key);

int IsPressMenu(int32_t player, APP_Button button, APP_InputType type);
int IsPushMenu(int32_t player, APP_Button button, APP_InputType type);

int IsPressPrompt(APP_Prompt prompt);
int IsPushPrompt(APP_Prompt prompt);

#ifdef APP_ENABLE_GAME_CONTROLLER
int IsPressConTypeKey(APP_InputType type, APP_ConKey* key);
int IsPushConTypeKey(APP_InputType type, APP_ConKey* key);
#endif

#ifdef APP_ENABLE_KEYBOARD
void updateEscapeFrames();
#endif
int quitNow();

void title();
void backupSetups();
void initialize();
void mainUpdate();
void gameExecute();
void setGamePause(int32_t player, bool pauseSetting);

typedef enum EMainLoopState {
	MAIN_INIT,
	MAIN_RESTART,
	MAIN_START,
	MAIN_INIT_TEXT,
	MAIN_INIT_LOAD_1,
	MAIN_INIT_LOAD_2,
	MAIN_INIT_LOAD_3,
	MAIN_INIT_LOAD_4,
	MAIN_INIT_END,
	MAIN_TITLE,
	MAIN_GAME_EXECUTE,
	MAIN_REPLAY_SELECT,
	MAIN_REPLAY_DETAIL,
	MAIN_RANKING_1,
	MAIN_RANKING_2,
	MAIN_RANKING_3,
	MAIN_RANKING2_1,
	MAIN_RANKING2_2,
	MAIN_RANKING2_3,
	MAIN_ST_RANKING,
	MAIN_CONFIG,
	MAIN_SOUND_TEST,
	MAIN_TEST_MENU,
	#ifdef APP_ENABLE_KEYBOARD
	MAIN_RESET_KEYBOARD,
	#endif
	MAIN_QUIT
} EMainLoopState;
extern bool init;
extern EMainLoopState mainLoopState;
extern EMainLoopState lastRankingMainLoopState;
extern int32_t resumeAfterRanking;
extern uint32_t SegaSeed[2];
extern uint32_t BloxeedSeed[2];
extern uint32_t SavedSeed[2];
extern uint32_t PieceSeed;
extern bool inmenu;
extern APP_InputType lastInputType;
extern APP_InputType lastPlayerInputType[2];
extern char *string[STRING_MAX];
extern int32_t fldihardno;
extern int32_t fldigsno;
extern int32_t fldisno;
extern int32_t item_pronum;
extern const char* RankString;
extern const char* version;
extern int32_t se_play[50];
extern int32_t fade_seed;
extern int32_t repdata[20 *2];
extern int32_t misstimer[2];
extern int32_t vs_points[2];
extern int32_t winpoint;
extern int32_t vs_round;
extern int32_t item_inter[2];
extern int32_t vs_style[2];
extern int32_t relayround[2];
extern int32_t relaydata[9 *2];
extern int32_t first_rot[2];
extern int32_t relaymode[2];
extern int32_t dispLinfo;
extern int32_t timelimit_doom_H;
extern int32_t timelimit_doom_N;
extern int32_t timelimit_doom_E;
extern int32_t medal_info;
extern int32_t item_num;
extern int32_t timelimitmw[2];
extern int32_t timelimitmw_devil;
extern int32_t timelimitm[2];
extern int32_t timelimitm_devil;
extern int32_t scrate[2];
extern int32_t grade_pasttime[2];
extern int32_t grade_his[2 *5];
extern int32_t admit_grade[2];
extern int32_t ori_opt[2];
extern int32_t squarecnt[2];
extern int32_t p_nextblock;
extern int32_t item_pro[50];
extern int32_t itemhistory[2 *5];
extern int32_t squaremode[2];
extern int32_t fldsbuf[10 *22 *2];
extern int32_t flds[10 *22 *2];
extern int32_t std_opt[2];
extern int32_t fpbas_mode[2];
extern int32_t fastroll[2];
extern int32_t bo[2];
extern int32_t alpha_tls;
extern int32_t disable_giji3D;
extern int32_t FP_next[4];
extern int32_t FP_bonus_c[2];
extern int32_t FP_bonus[2];
extern int32_t novice_flag[2];
extern int32_t novice_mode[2];
extern int32_t exam_ranking;
extern int32_t devil_nextrise[2];
extern int32_t devil_randrise;
extern int32_t devil_minus[2];
extern int32_t def_vs_uplinetype;
extern int32_t regretcancel[2];
extern int32_t upLines_waiting[2];
extern int32_t fldubuf[10 *22 *2];
extern int32_t fldu[10 *22 *2];
extern int32_t kickc3[2];
extern int32_t m_roll_blockframe;
extern int32_t stp_point[2];
extern int32_t ofs_y2[2];
extern int32_t ofs_x2[2];
extern int32_t ofs_y[2];
extern int32_t ofs_x[2];
extern int32_t do180field[2];
extern int32_t isRmovfield[2];
extern int32_t isLmovfield[2];
extern int32_t Ff_rerise[2];
extern int32_t isFreefall[2];
extern int32_t hanabi_int[2];
extern int32_t def_vs_goal;
extern int32_t are_skipflag[2];
extern int32_t p_item_mode;
extern int32_t bk_D[2];
extern int32_t g_end[2];
extern int32_t g_start[2];
extern int32_t r_end[2];
extern int32_t r_start[2];
extern int32_t ti_s_go_end;
extern int32_t ti_s_go_start;
extern int32_t ti_s_ready_end;
extern int32_t ti_s_ready_start;
extern int32_t ti_go_end;
extern int32_t ti_go_start;
extern int32_t ti_ready_end;
extern int32_t ti_ready_start;
extern int32_t ready_go_style;
extern int32_t ace_bgmchange[2];
extern int32_t ace_bgmlist[3 *4];
extern int32_t ace_bgmfadestart[4 *4];
extern int32_t Isbesttime;
extern int32_t dorulet[2];
extern int32_t tr_20g[2];
extern int32_t versus_rot[2];
extern int32_t harddrop[2];
extern int32_t st_other[30 *2];
extern int32_t lvstop_time[20 *2];
extern int32_t stage_time[30 *2];
extern int32_t hebop_interval[2];
extern int32_t stage_skip_mpc[2];
extern int32_t ace_irs;
extern int32_t exam_range[8];
extern int32_t purupuru[2];
extern int32_t exam_grade[2];
extern int32_t examination[2];
extern int32_t enable_randexam;
extern int32_t banana_c[2];
extern int32_t banana_pos[10 *2];
extern int32_t isbanana[2];
extern int32_t rt_nblk[6 *2];
extern int32_t docopyfld[2];
extern int32_t doexchg[2];
extern int32_t doshotgun[2];
extern int32_t shotgunpos[22 *2];
extern int32_t donega[2];
extern int32_t nega_pos[10 *2];
extern int32_t do16t[2];
extern int32_t dolaser[2];
extern int32_t rapid_c[2];
extern int32_t laserpos[4 *2];
extern int32_t lasernum[2];
extern int32_t disable_ehlast_gimmick;
extern int32_t end_f[2];
extern int32_t medal_st[2];
extern int32_t medal_re[2];
extern int32_t medal_co[2];
extern int32_t medal_sk[2];
extern int32_t medal_ac[2];
extern int32_t ranking_type;
extern int32_t rankingrule;
extern int32_t beginner_rollbgm;
extern int32_t fastmirror;
extern int32_t hebo_plus[2];
extern int32_t item_mode[2];
extern int32_t erasecnt[2];
extern int32_t hide_tomoyoinf;
extern int32_t nanamedown;
extern int32_t hold_snd;
extern int32_t navigation_limitLv;
extern int32_t navigation_limittime;
extern int32_t navigation_time[2];
extern int32_t navigation;
extern int32_t ndelay[2];
extern int32_t tomoyo_ehfinal_c[2];
extern int32_t tomoyo_domirror[2];
extern int32_t tomoyo_waits[2];
extern int32_t hnext_timer[2];
extern int32_t hnext_flag[2];
extern int32_t tomoyo_rise_t[2];
extern int32_t tomoyo_rise[2];
extern int32_t tomoyo_rise_fld[10 *20 *2];
extern int32_t enable_minspeed;
extern int32_t thunder_timer;
extern int32_t thunder;
extern int32_t isthunder[2];
extern int32_t min_speed[2];
extern int32_t textguide;
extern int32_t grade2[2];
extern int32_t border_time48[20];
extern int32_t border_time59[20];
extern int32_t border_time[20];
extern int32_t border_rank[2];
extern int32_t tr2[2];
extern int32_t skillflag[2];
extern int32_t qualityflag[2];
extern int32_t gup3sp_adjust[2];
extern int32_t gup3rank[2];
extern int32_t ave_laptime3[2];
extern int32_t timeN[2];
extern int32_t time99[2];
extern int32_t back_mov_inter[13];
extern int32_t back_mov_f[13];
extern int32_t back_acnt;
extern int32_t death_plus[2];
extern int32_t max_hnext[2];
extern int32_t mov_inter;
extern int32_t title_mov_f;
extern int32_t title_acnt;
extern int32_t first_seed[2];
extern int32_t randseed[2];
extern int32_t start_mission;
extern int32_t gup_down[2];
extern int32_t endlines[2];
extern int32_t skillbai[2];
extern int32_t skillg[2];
extern int32_t skip_fwait;
extern int32_t def_p_sp;
extern int32_t p_setting;
extern int32_t block_spectrum;
extern int32_t p_blind;
extern int32_t blind_flag[2];
extern int32_t isblind[2];
extern int32_t p_heboGBlv;
extern int32_t p_erase_c[11];
extern int32_t use_item[2];
extern int32_t vsbg;
extern int32_t gflash[2];
extern int32_t gtime[2];
extern int32_t gpoint[2];
extern int32_t glimit[33];
extern int32_t isallclear[2];
extern int32_t isreflect[2];
extern int32_t isdeleven[2];
extern int32_t isDWdelfield[2];
extern int32_t isUPdelfield[2];
extern int32_t item_timer[2];
extern int32_t stopmirror_flag[2];
extern int32_t isfakenext[2];
extern int32_t ismiss[2];
extern int32_t isdouble[2];
extern int32_t istrance[2];
extern int32_t isdark[2];
extern int32_t isremote[2];
extern int32_t isfever[2];
extern int32_t isboost[2];
extern int32_t isUDreverse[2];
extern int32_t isLRreverse[2];
extern int32_t ismagnetblock[2];
extern int32_t ishidenext[2];
extern int32_t istimestop[2];
extern int32_t isholdlock[2];
extern int32_t isrotatelock[2];
extern int32_t heboGB[2];
extern int32_t top_frame;
extern int32_t bgnroll_bgmloop;
extern int32_t nmlroll_bgmloop;
extern int32_t tomoyo_opt[2];
extern int32_t staffroll_y_ex[2];
extern int32_t fmirror_cnt[2];
extern int32_t timeextend_snd_cnt[2];
extern int32_t ext_s[2];
extern int32_t platinaerase_cnt[2];
extern int32_t itemappear_c[2];
extern int32_t b2bcheck;
extern int32_t b_to_b_c[2];
extern int32_t b_to_b_flag[2];
extern int32_t last_tspin[2];
extern int32_t tomoyo_hanabi[2];
extern int32_t endingcnt[2];
extern int32_t hanabi_total[2];
extern int32_t hanabi_flag;
extern int32_t hanabi_combo[2];
extern int32_t tspin_type;
extern int32_t upLineT[2];
extern int32_t over1000_start;
extern int32_t over1000_block[2];
extern int32_t ybigtype;
extern int32_t eraser_now_lines;
extern int32_t clear_mission;
extern int32_t tls_view;
extern int32_t spawn_y_type;
extern int32_t eraser_now_pattern;
extern int32_t eraser_cleared[4];
extern int32_t eraser_lines[4];
extern int32_t target_cleared[200];
extern int32_t mission_opt_3[40];
extern int32_t mission_opt_2[40];
extern int32_t mission_opt_1[40];
extern int32_t mission_erase[40];
extern int32_t cycle[4];
extern int32_t mission_bgm[40];
extern int32_t mission_lv[40];
extern int32_t mission_end[40];
extern int32_t mission_time[40];
extern int32_t mission_norm[40];
extern int32_t mission_type[40];
extern int32_t mission_file;
extern int32_t c_norm[2];
extern int32_t c_mission;
extern int32_t world_i_rot;
extern int32_t wintype;
extern int32_t breakti;
extern int32_t vs_time;
extern int32_t vs_goal;
extern int32_t sptmp[2];
extern int32_t disrise;
extern int32_t vsmodesl;
extern int32_t timelimitw[2];
extern int32_t timelimitw_devil;
extern int32_t anothermode[2];
extern int32_t big_rise;
extern int32_t timelimit2[2];
extern int32_t timelimit2_devil;
extern int32_t max_fps_2;
extern int32_t st_new[20 *2];
extern int32_t r_irs;
extern int32_t no_hanabi;
extern int32_t noitem;
extern int32_t hanabi_frame[2];
extern int32_t hanabi_waiting[2];
extern int32_t item_waiting[2];
extern int32_t item_coming[2];
extern int32_t item_nblk[6 *2];
extern int32_t disable_wallkick;
extern int32_t stb[2];
extern int32_t sts[2];
extern int32_t stg[2];
extern int32_t stp[2];
extern int32_t color_bgblack;
extern int32_t xray_drawblockframe;
extern int32_t xray_bgblack;
extern int32_t bg_max;
extern int32_t bgmvolume;
extern int32_t english;
extern int32_t bgfade_cnt;
extern int32_t enable_bgfade;
extern int32_t bgfadelv;
extern int32_t bgfadesw;
extern int32_t p_stage;
extern int32_t up_flag[2];
extern int32_t t_training[2];
extern int32_t endtime[2];
extern int32_t bgm;
extern int32_t se;
extern int32_t sevolume;
extern int32_t block_rframe;
extern int32_t tmp_maxPlay;
extern int32_t cp_rot_c[2];
extern int32_t cp_type;
extern int32_t cp_player_2p;
extern int32_t cp_player_1p;
extern int32_t cp_hold[2];
extern int32_t cpu_flag[2];
extern int32_t cp_input[10 *2];
extern int32_t cp_rt[2];
extern int32_t cp_x[2];
extern int32_t min_sgrade;
extern int32_t sgrade[2];
extern int32_t time2[2];
extern int32_t replay_save[2];
extern int32_t max_fps;
extern int32_t disable_irs;
extern int32_t disable_hold;
extern int32_t tspin_flag[2];
extern int32_t tspin_c[2];
extern int32_t pinch2[2];
extern int32_t REflag[2];
extern int32_t rescue[2];
extern int32_t laststage[2];
extern int32_t hide_tomoyo_totaltime;
extern int32_t p_monochrome_mode;
extern int32_t monochrome_mode[2];
extern int32_t item_monochrome[2];
extern int32_t item_rollroll[2];
extern int32_t item_interval;
extern int32_t item_t[2];
extern int32_t fldibuf[10 *22 *2];
extern int32_t item_g[2];
extern int32_t item_name[2];
extern int32_t item[2];
extern int32_t fldi[10 *22 *2];
extern int32_t vs_win[2];
extern int32_t vs_match;
extern int32_t c_hblk[2];
extern int32_t c_nblk[6 *2];
extern int32_t c_cblk_r[2];
extern int32_t c_cblk[2];
extern int32_t super_breakeffect;
extern int32_t world_drop_snd;
extern int32_t medaltype;
extern int32_t combo2[2];
extern int32_t comboM[2];
extern int32_t bigtype;
extern int32_t BigMove[2];
extern int32_t t_noborder[2];
extern int32_t clearp[2];
extern int32_t clearnum[2];
extern int32_t randommode[2];
extern int32_t stage_nextc[2];
extern int32_t start_nextc[2];
#ifdef APP_ENABLE_KEYBOARD
extern int32_t skipKey;
#endif
extern int32_t start_stage[2];
extern int32_t dummy;
extern int32_t s_go_end;
extern int32_t s_go_start;
extern int32_t s_ready_end;
extern int32_t s_ready_start;
extern int32_t rest_pblock[2];
extern int32_t color_flag[2];
extern int32_t xray_flag[2];
extern int32_t rollroll_flag[2];
extern int32_t isfmirror[2];
extern int32_t ltime[2];
extern int32_t sclear[2];
extern int32_t stage[2];
extern int32_t b_nextc[2];
extern int32_t b_sp[2];
extern int32_t b_waitt[2];
extern int32_t b_wait3[2];
extern int32_t b_wait2[2];
extern int32_t b_wait1[2];
extern int32_t objp[100];
extern int32_t dead_blockframe;
extern int32_t p_goaltypenumlist[10];
extern int32_t p_goaltypenum;
extern int32_t p_goaltype;
extern int32_t p_ending;
extern int32_t allclear[2];
extern int32_t shadow_timer_flag[2];
extern int32_t shadow_timer_type;
extern int32_t st_record_force_viewright;
extern int32_t big_block_type;
extern int32_t hide_st_record;
extern int32_t st_record_interval_heb;
extern int32_t st_record_interval_tgm;
extern int32_t split_time[100 *2];
extern int32_t lap_time[100 *2];
extern int32_t start_flag[2];
extern int32_t skill[2];
extern int32_t go_end;
extern int32_t go_start;
extern int32_t ready_end;
extern int32_t ready_start;
extern int32_t fldtbuf[10 *22 *2];
extern int32_t fldbuf[10 *22 *2];
extern int32_t fldt[10 *22 *2];
extern int32_t p_shadow_timer;
extern int32_t shadow_timer;
extern int32_t iscolor[2];
extern int32_t fcolor;
extern int32_t color_counter[2];
extern int32_t xray_counter[2];
extern int32_t isxray[2];
extern int32_t xray;
extern int32_t isrollroll[2];
extern int32_t p_xray_frame;
extern int32_t p_xray_interval;
extern int32_t p_rollroll_timer;
extern int32_t p_rollroll_interval;
extern int32_t rollroll;
extern int32_t p_fmirror_timer;
extern int32_t p_fmirror_interval;
extern int32_t fmirror;
extern int32_t pass_flg[2];
extern int32_t p_nextpass;
extern int32_t bakhold;
extern int32_t p_hold;
extern int32_t domirror;
extern int32_t mirror;
extern int32_t hide_hidden;
extern int32_t skip_obj;
extern int32_t skip_viewbg;
extern int32_t skip_viewscore;
extern int32_t IsBigStart[2];
extern int32_t mini_select;
extern int32_t giveup_safety;
extern int32_t giveup_func;
extern int32_t semigmflag_r[2];
extern int32_t gmflag_r[2];
extern int32_t tlsrange;
extern int32_t st_bdowncnt[2];
extern int32_t bdowncnt[2];
extern int32_t hold_used[2];
extern int32_t enable_grade[2];
extern int32_t enable_hold_frame;
extern int32_t shadow_heboris;
extern int32_t breaktype;
extern int32_t limit15;
extern int32_t deadtype;
extern int32_t repversw;
extern int32_t mpc4[2];
extern int32_t mpc3[2];
extern int32_t timelimit[2];
extern int32_t timelimit_20G;
extern int32_t timelimit_master;
extern int32_t timelimit_devil;
extern int32_t gmflag3_e[2];
extern int32_t gmflag3[2];
extern int32_t gmflag2_e[2];
extern int32_t gmflag1_e[2];
extern int32_t gmflag2[2];
extern int32_t gmflag1[2];
extern int32_t grade[2];
extern const char* gname2[35];
extern const char* dgname[17];
extern const char* gname[20];
extern int32_t gscore[18];
extern char nextfp_list[4096];
extern char nextdengen_list[4096];
extern char nextb_list[4096];
extern int32_t down_flag[2];
extern int32_t rotate_snd;
extern int32_t showctrl;
extern int32_t bravo;
extern int32_t p_next_adjust;
extern int32_t next_adjust;
extern int32_t raise_shirase_interval;
extern int32_t raise_shirase_lines;
extern int32_t level_shirase_start;
extern int32_t limit[8];
extern int32_t gameMode[2];
extern int32_t pauseGame[2],hnext[2];
extern int32_t setupBak[27];
extern int32_t demotime;
extern int32_t demo;
extern int32_t playback;
extern int32_t saveBuf[50000];
extern int32_t** replayData;
extern int32_t replayChunkCnt;
extern int32_t csr,cnt,enable[80],category,rankingmode;
extern int32_t noredraw,fast;
extern int32_t rksc[40],rkbl[40],rktime[40],rklv[40],rkfl[40];
extern int32_t hiddeny[2],hiddenti[2],hiddenc[2];
extern int32_t vslevel[2],hidden[2],hiddentm[2];
extern int32_t winr,winc,winu,wink;
extern int32_t combo[2],hebocombo[2],pinch[2];
extern int32_t overcount;
extern int32_t flag;
extern int32_t startLvback[2];
extern int32_t IsBig[2];
extern int32_t fldsizew[2],fldsizeh[2];
extern int32_t show_result;
extern int32_t hide_score;
extern int32_t hide_wait;
extern int32_t hide_fps;
extern int32_t p_over1000;
extern int32_t debug;
extern int32_t p_backno;
extern int32_t upFld[256];
extern int32_t upLineNoMax;
extern int32_t upLineNo[2];
extern int32_t uplinetype;
extern int32_t p_shirase_start_level;
extern int32_t p_shirase_interval;
extern int32_t p_shirase_line;
extern int32_t speed[25];
extern int32_t syutugen[25];
extern char waitname[25][128];
extern int32_t tame1,tame2,tame3,tame4;
extern int32_t mp2[2],mpc2[2];
extern int32_t tame1;
extern int32_t color_tbl[40];
extern int32_t color_tbl_max;
extern int32_t ori_bgmfadestart[16];
extern int32_t Replaybgmfadestart[48];
extern int32_t bgmfadestart[50];
extern int32_t fadelv[2];
extern int32_t bgmlv;
extern int32_t slv[2],tlv[2];
extern int32_t startlvTbl[56];
extern int32_t cmbpts[2];
extern int32_t qdrop[2];
extern int32_t sdrop[2];
extern int32_t p_bgmlv;
extern int32_t p_shirase[2];
extern int32_t shirase[2];
extern int32_t caph;
extern int32_t capw;
extern int32_t capy;
extern int32_t capx;
extern int32_t capi;
extern int32_t capKey;
extern int32_t oncap;
extern int32_t capc;
extern int32_t edrec[2];
extern int32_t onRecord[2];
extern int32_t ending[2];
extern int32_t kickc2[2];
extern int32_t kickc[2];
extern int32_t start[2];
extern int32_t tr[2];
extern int32_t tc[2];
extern int32_t ssc;
extern int32_t sw,sh;
extern int32_t dhold2[2];
extern int32_t dhold[2];
extern int32_t hold[2];
extern int32_t nextwork[7 *2];
extern int32_t hole[2];
extern int32_t nextc[2];
extern int32_t nextb[1400 *2];
extern int32_t upLines[2];
extern int32_t lc[2],sp[2];
extern int32_t obj[100],objx[100],objy[100],objv[100],objw[100],objj[100],objc[100],obja[100];
extern int32_t erase[22 *2];
extern int32_t mps[2 *2];
extern int32_t mp[2],mpc[2];
extern int32_t bs[2],bk[2];
extern int32_t bx[2],by[2],rt[2];
extern int32_t blk[2];
extern int32_t fld[10 *22 *2];
extern int32_t next[2 *3];
extern int32_t sc[2],lv[2],li[2];
extern int32_t status[2],statusc[10 *2];
extern int32_t backno;
extern int32_t count;
extern int32_t bgmteisiflg;
extern int32_t wait3[2],waitt[2];
extern int32_t wait1[2],wait2[2];
extern int32_t stime[2];
extern int32_t ctime[2];
extern int32_t gametime[2],timeOn[2];
void shutDown();
void spriteTime();
extern bool loopFlag;
extern bool quitNowFlag;

#endif
