#include "common.h"

APP_ScreenModeFlag screenMode;	// スクリーンモード
int32_t screenIndex;		// スクリーンインデックス
#define SOUND_BUFFER_MAX 4
int32_t soundbuffer = 0;	// Size of the sound buffer. It's "1024 << soundbuffer" bytes.
int32_t nextblock;		// ツモ
uint32_t cfgversion;		// 設定ファイルフォーマットのバージョン番号です。
int32_t smooth;			// ブロック落下	0:ノーマル 1:スムーズ
int32_t nanameallow;		// 斜め入力	0:無効 1:有効
int32_t fourwayfilter;		// Filter direction inputs to only allow cardinal directions	0:Both players 1:Only 1P 2:Only 2P 3:Off
int32_t fourwaypriorityup;	// Prioritize up for players using four way filtration	0:Both players 1:Only 1P 2:Only 2P 3:Off
int32_t sonicdrop;		// 上入れ即接地0:有効 1:無効
int32_t fastlrmove;		// 横先行入力	0:有効 1:無効
int32_t blockflash;		// ブロック枠	0:点滅 1:点灯 2:無し
int32_t background;		// フィールド背景0:スクロール 1:スクロール無し 2:ベタ

int32_t rots[2];		// 回転規則
int32_t fontc[12];		// 題字の色	0:白 1:青 2:赤 3:桃 4:緑 5:黄 6:空 7:橙 8:紫 9:藍
int32_t digitc[12];		// 数字の色	それぞれ、TGMRule・TiRule・WorldRule・World2Rule・ARSRule・ARS2Rule・World3Rule
#ifdef APP_ENABLE_KEYBOARD
int32_t dispnextkey[2] = { SDL_SCANCODE_F3, SDL_SCANCODE_F4 };	// NEXT表示キー(デフォルトはF3, F4) 	#1.60c7g7
#endif
int32_t dtc;			// tgmlvの表示	0:off  1:on  (lvtype = 1の時は常に表示)
int32_t fldtr;			// フィールド背景非表示時のフィールド透過度(0-256)
APP_SoundBits wavebgm;		// BGMの選択 || BGM selection
// ver.160c6
int32_t dispnext;		// ネクスト表示個数設定
int32_t movesound;		// ブロック移動音設定	0:OFF　1:ON
int32_t fontsize;		// メイン画面で描画するフォントの大きさ	0:標準　1:小型
int32_t maxPlay;		// プレイする最大人数	0:シングル台　1:ツイン台
int32_t lastmaxPlay;		// 設定変更前の、プレイする最大人数	0:シングル台　1:ツイン台
int32_t lasttopframe;

int32_t breakeffect;		// ラインをそろえたとき、ブロックを弾けさせるか 0:off 1:on
int32_t showcombo;		// コンボの表示(SINGLEとかHEBORISとか) 0:off 1:on
//int32_t quickerase;		// ブロックの高速消去 0:ブロックを左から右へ消す 1:同時に消す

int32_t w_reverse;		// ワールドルールで回転方法を逆転させる 0:off 1:on #1.60c7f8

int32_t downtype;		// 下入れタイプ 0:HEBORIS 1:Ti #1.60c7f9

int32_t lvupbonus;		// レベルアップボーナス 0:TI 1:TGM/TAP 2:ajust#1.60c7g3

#ifdef APP_ENABLE_KEYBOARD
// キーボード設定
SDL_Scancode keyAssign[APP_BUTTON_COUNT * 2] =
{
	SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT,
	SDL_SCANCODE_Z, SDL_SCANCODE_X, SDL_SCANCODE_C, SDL_SCANCODE_V,
	SDL_SCANCODE_Q, SDL_SCANCODE_W,

	SDL_SCANCODE_KP_8, SDL_SCANCODE_KP_5, SDL_SCANCODE_KP_4, SDL_SCANCODE_KP_6,
	SDL_SCANCODE_KP_1, SDL_SCANCODE_KP_2, SDL_SCANCODE_KP_3, SDL_SCANCODE_KP_0,
	SDL_SCANCODE_PAGEUP, SDL_SCANCODE_PAGEDOWN,
};
#endif

int32_t segacheat;		// Allow CW and/or 180 rotation in old style.

#ifdef APP_ENABLE_JOYSTICK
// →pauseとgiveupを追加 1.60c7g7
APP_JoyKey joyKeyAssign[APP_BUTTON_COUNT * 2];	// ジョイスティックボタン割り当て
#endif

#ifdef APP_ENABLE_GAME_CONTROLLER
APP_ConKey conKeyAssign[8 * 2];
#endif

int32_t restart = 0;		// 再起動フラグ
int32_t reinit = 1;		// Indicates if resources should be loaded.

// 設定をバイナリデータに保存 1.60c5
int32_t SaveConfig(void) {
	int32_t i, j, cfgbuf[CFG_LENGTH];

	APP_FillMemory(cfgbuf, sizeof(cfgbuf), 0);
	cfgbuf[0] = 0x4F424550;
	cfgbuf[1] = 0x20534953;
	cfgbuf[2] = 0x464E4F44;
	cfgbuf[3] = 0x31764750;

	cfgbuf[4] = screenMode;
	cfgbuf[5] = screenIndex;
	cfgbuf[6] = nextblock;
	cfgbuf[7] = cfgversion;
	cfgbuf[8] = smooth;
	cfgbuf[9] = nanameallow;
	cfgbuf[10] = sonicdrop;
	cfgbuf[11] = blockflash;
	cfgbuf[12] = fastlrmove;
	cfgbuf[13] = background;

	#ifdef APP_ENABLE_KEYBOARD
	for(i = 0; i < 20; i++) {
		cfgbuf[14 + i] = keyAssign[i];
	}
	#endif

	cfgbuf[35] = fourwayfilter;
	cfgbuf[37] = fourwaypriorityup;

	cfgbuf[38] = soundbuffer;

	cfgbuf[40] = rots[0];
	cfgbuf[41] = rots[1];
	cfgbuf[42] = segacheat;
	cfgbuf[44] = dtc;
	cfgbuf[45] = dispnext;

	cfgbuf[53] = fldtr;
	cfgbuf[54] = fontsize;
	cfgbuf[55] = maxPlay;
	cfgbuf[60] = movesound;
	cfgbuf[61] =
		(( se & 0x1) << 23) | (( sevolume & 0x7F) << 16) |
		((bgm & 0x1) << 15) | ((bgmvolume & 0x7F) <<  8) |
		wavebgm;
	cfgbuf[62] = breakeffect;
	cfgbuf[63] = showcombo;
	cfgbuf[64] = top_frame;
	cfgbuf[65] = w_reverse;
	cfgbuf[66] = downtype;
	cfgbuf[67] = lvupbonus;

	/* 72〜74はジョイスティック用 */
	cfgbuf[74] = fontc[0] + fontc[1] * 0x100 + fontc[2] * 0x10000 + fontc[3] * 0x1000000;
	cfgbuf[75] = digitc[0] + digitc[1] * 0x100 + digitc[2] * 0x10000 + digitc[3] * 0x1000000;
	cfgbuf[76] = fontc[4] + fontc[5] * 0x100 + fontc[6] * 0x10000 + fontc[7] * 0x1000000;
	cfgbuf[77] = digitc[4] + digitc[5] * 0x100 + digitc[6] * 0x10000 + digitc[7] * 0x1000000;
	cfgbuf[78] = fontc[8] + fontc[9] * 0x100 + fontc[10] * 0x10000 + fontc[11] * 0x1000000;
	cfgbuf[79] = digitc[8] + digitc[9] * 0x100 + digitc[10] * 0x10000 + digitc[11] * 0x1000000;

	#ifdef APP_ENABLE_JOYSTICK
	int32_t *joykeybuf = &cfgbuf[80];
	for (int32_t pl = 0; pl < 2; pl++) {
		for (int32_t key = 0; key < 10; key++) {
			int32_t *plbuf = &joykeybuf[pl * 10 * 8 + key * 8];
			APP_JoyKey *pljoy = &joyKeyAssign[pl * 10 + key];
			plbuf[0] = pljoy->player;
			for (int32_t i = 0; i < 4; i++) {
				plbuf[1 + i] = pljoy->guid.data[i];
			}
			plbuf[5] = pljoy->type;
			switch (pljoy->type) {
			case APP_JOYKEY_AXIS:
			case APP_JOYKEY_HAT:
				plbuf[6] = pljoy->setting.index;
				plbuf[7] = pljoy->setting.value;
				break;

			case APP_JOYKEY_BUTTON:
				plbuf[6] = pljoy->setting.button;
				break;
			default:
				break;
			}
		}
	}
	#endif
	
	#ifdef APP_ENABLE_GAME_CONTROLLER
	int32_t* conkeybuf = &cfgbuf[240];
	for (int32_t pl = 0; pl < 2; pl++) {
		conkeybuf[pl * (1 + 2 * 8)] = pl;
		int32_t* plbuf = &conkeybuf[pl * (1 + 2 * 8) + 1];
		for (int32_t key = 0; key < 8; key++) {
			const APP_ConKey* plcon = &conKeyAssign[pl * 8 + key];
			plbuf[key * 2 + 0] = plcon->type;
			plbuf[key * 2 + 1] = plcon->index;
		}
	}
	#endif

	cfgbuf[274] = spawn_y_type;
	cfgbuf[275] = hide_tomoyoinf;
	cfgbuf[276] = tspin_type;
	cfgbuf[277] = block_spectrum;
	cfgbuf[278] = next_adjust;
	cfgbuf[279] = Isbesttime;
	cfgbuf[280] = b2bcheck;

	cfgbuf[281] = debug;
	cfgbuf[282] = enable_grade[0];
	cfgbuf[283] = enable_grade[1];
	cfgbuf[284] = ace_irs;
	cfgbuf[285] = english;
	cfgbuf[286] = mini_select;
	cfgbuf[287] = bigtype;
	cfgbuf[288] = item_interval;
	cfgbuf[289] = hide_wait;
	cfgbuf[290] = vs_time;
	cfgbuf[291] = medaltype;
	cfgbuf[292] = p_shirase_start_level;
	cfgbuf[293] = uplinetype;
	cfgbuf[294] = disable_hold;
	cfgbuf[295] = disable_irs;
	cfgbuf[296] = cp_player_1p;
	cfgbuf[297] = cp_player_2p;
	cfgbuf[298] = cp_type;
	cfgbuf[299] = block_rframe;
	cfgbuf[300] = disable_wallkick;
	cfgbuf[301] = showctrl;

	cfgbuf[34] = ConfigChecksum(cfgbuf);

	APP_SaveFile("config/data/CONFIG.SAV", cfgbuf, sizeof(cfgbuf));

	return (0);
}

// 設定をバイナリデータから読み込み 1.60c5
int32_t LoadConfig(void) {
	int32_t i, j, cfgbuf[CFG_LENGTH];

	APP_FillMemory(cfgbuf, sizeof(cfgbuf), 0);
	APP_LoadFile("config/data/CONFIG.SAV", cfgbuf, sizeof(cfgbuf));
	if(cfgbuf[0] != 0x4F424550) return (1);
	if(cfgbuf[1] != 0x20534953) return (1);
	if(cfgbuf[2] != 0x464E4F44) return (1);
	if(cfgbuf[3] != 0x31764750) return (1);
	if((uint32_t)cfgbuf[7] != CFG_VERSION) return (1);
	if((uint32_t)cfgbuf[34] != ConfigChecksum(cfgbuf)) return (1);

	screenMode = cfgbuf[4];

#ifdef APP_ENABLE_ALL_VIDEO_SETTINGS
	screenIndex = cfgbuf[5];

#else
	screenMode &= ~APP_SCREENMODE_WINDOWTYPE;
	screenMode |=
#ifdef __vita__
		APP_SCREENMODE_FULLSCREEN
#else
		APP_SCREENMODE_WINDOW
#endif
		| APP_SCREENMODE_VSYNC | APP_SCREENMODE_RENDERLEVEL;
	screenIndex = 0;

#endif

	nextblock = cfgbuf[6];
	cfgversion = (uint32_t)cfgbuf[7];
	smooth = cfgbuf[8];
	nanameallow = cfgbuf[9];
	sonicdrop = cfgbuf[10];
	blockflash = cfgbuf[11];
	fastlrmove = cfgbuf[12];
	background = cfgbuf[13];

	#ifdef APP_ENABLE_KEYBOARD
	for(i = 0; i < 20; i++) {
		if (cfgbuf[14 + i] == SDL_GetScancodeFromKey(SDLK_ESCAPE, NULL)) keyAssign[i] = SDL_SCANCODE_UNKNOWN;
		else keyAssign[i] = cfgbuf[14 + i];
	}
	#endif

	fourwayfilter = cfgbuf[35];
	fourwaypriorityup = cfgbuf[37];

	if (cfgbuf[38] < 0 || cfgbuf[38] >= SOUND_BUFFER_MAX) {
		soundbuffer = 0;
	}
	else {
		soundbuffer = cfgbuf[38];
	}

	rots[0] = cfgbuf[40];
	rots[1] = cfgbuf[41];
	segacheat = cfgbuf[42];
	dtc     = cfgbuf[44];
	dispnext = cfgbuf[45];

	fldtr = cfgbuf[53];
	fontsize = cfgbuf[54];
	maxPlay = cfgbuf[55];

	movesound = cfgbuf[60];
	se = (cfgbuf[61] >> 23) & 0x1;
	sevolume = (cfgbuf[61] >> 16) & 0x7F;
	if(sevolume > 100) sevolume = 100;
	bgm = (cfgbuf[61] >> 15) & 0x1;
	bgmvolume = (cfgbuf[61] >> 8) & 0x7F;
	if(bgmvolume > 100) bgmvolume = 100;
	wavebgm = cfgbuf[61] & APP_SOUND_BITS_MASK;
	if ((wavebgm & APP_SOUND_BITS_FORMAT) > APP_SOUND_FORMAT_COUNT || !APP_IsSoundFormatSupported((wavebgm & APP_SOUND_BITS_FORMAT) - 1)) {
		wavebgm = (wavebgm & ~APP_SOUND_BITS_FORMAT) | (APP_SOUND_FORMAT_WAV + 1);
	}

	breakeffect = cfgbuf[62];
	showcombo = cfgbuf[63];
	top_frame = cfgbuf[64];

	w_reverse = cfgbuf[65];

	downtype = cfgbuf[66];
	lvupbonus = cfgbuf[67];

	/* 72〜74はジョイスティック用 */

	for(i = 0; i <= 3; i++) {
		fontc[i] = (cfgbuf[74] >> (i * 8)) & 0xff;
		digitc[i] = (cfgbuf[75] >> (i * 8)) & 0xff;
	}
	for(i = 0; i <= 3; i++) {
		fontc[i + 4] = (cfgbuf[76] >> (i * 8)) & 0xff;
		digitc[i + 4] = (cfgbuf[77] >> (i * 8)) & 0xff;
	}
	for(i = 0; i <= 3; i++) {
		fontc[i + 8] = (cfgbuf[78] >> (i * 8)) & 0xff;
		digitc[i + 8] = (cfgbuf[79] >> (i * 8)) & 0xff;
	}

	#ifdef APP_ENABLE_JOYSTICK
	int32_t *joykeybuf = &cfgbuf[80];
	for (int32_t pl = 0; pl < 2; pl++) {
		for (int32_t key = 0; key < 10; key++) {
			APP_JoyKey *pljoy = &joyKeyAssign[pl * 10 + key];
			int32_t *plbuf = &joykeybuf[pl * 10 * 8 + key * 8];
			pljoy->player = plbuf[0];
			for (int32_t i = 0; i < 4; i++) {
				pljoy->guid.data[i] = plbuf[1 + i];
			}
			pljoy->type = (APP_JoyKeyType)plbuf[5];
			switch (pljoy->type) {
			case APP_JOYKEY_AXIS:
			case APP_JOYKEY_HAT:
				pljoy->setting.index = plbuf[6];
				pljoy->setting.value = plbuf[7];
				break;

			case APP_JOYKEY_BUTTON:
				pljoy->setting.button = plbuf[6];
				break;
			default:
				break;
			}
		}
	}
	#endif
	
	#ifdef APP_ENABLE_GAME_CONTROLLER
	int32_t* conkeybuf = &cfgbuf[240];
	for (int32_t pl = 0; pl < 2; pl++) {
		int32_t* plbuf = &conkeybuf[pl * (1 + 2 * 8) + 1];
		for (int32_t key = 0; key < 8; key++) {
			APP_ConKey* plcon = &conKeyAssign[pl * 8 + key];
			plcon->type = (APP_ConKeyType)plbuf[key * 2 + 0];
			plcon->index = plbuf[key * 2 + 1];
		}
	}
	#endif

	spawn_y_type = cfgbuf[274];
	hide_tomoyoinf = cfgbuf[275];
	tspin_type = cfgbuf[276];
	block_spectrum = cfgbuf[277];
	next_adjust = cfgbuf[278];
	Isbesttime = cfgbuf[279];
	b2bcheck = cfgbuf[280];
	debug = cfgbuf[281];
	enable_grade[0] = cfgbuf[282];
	enable_grade[1] = cfgbuf[283];
	ace_irs = cfgbuf[284];
	english = cfgbuf[285];
	mini_select = cfgbuf[286];
	bigtype = cfgbuf[287];
	item_interval = cfgbuf[288];
	hide_wait = cfgbuf[289];
	vs_time = cfgbuf[290];
	medaltype = cfgbuf[291];
	p_shirase_start_level = cfgbuf[292];
	uplinetype = cfgbuf[293];
	disable_hold = cfgbuf[294];
	disable_irs = cfgbuf[295];
	cp_player_1p = cfgbuf[296];
	cp_player_2p = cfgbuf[297];
	cp_type = cfgbuf[298];
	block_rframe = cfgbuf[299];
	disable_wallkick = cfgbuf[300];
	showctrl = cfgbuf[301];

	return (0);
}

uint32_t ConfigChecksum(int32_t *cfgbuf) {
	uint32_t checksum = 0u;
	for (int32_t i = 0; i < CFG_LENGTH; i++) {
		if (i == 34) continue;
		checksum += (uint32_t)cfgbuf[i];
	}
	return checksum;
}


void ConfigMenu() {
	static int32_t i, j, m, pl, conPlayer, count, pages;
	static int32_t ncfg[CFG_LENGTH];
	static int32_t need_reset;	// 設定保存時にリセットするか
	static int32_t need_setScreen;
	static int32_t need_reloadBG;
	static int32_t last_BG;

	if (init) {
		pages = 5;

		ncfg[0]  = screenMode;
		ncfg[1]  = screenIndex;
		ncfg[2]  = nextblock;
		ncfg[3]  = cfgversion;
		ncfg[4]  = smooth;
		ncfg[5]  = nanameallow;
		ncfg[6]  = sonicdrop;
		ncfg[7]  = fastlrmove;
		ncfg[8]  = blockflash;
		ncfg[9]  = background;
		last_BG = background;

		#ifdef APP_ENABLE_KEYBOARD
		for(i = 0; i < 20; i++) ncfg[10 + i] = keyAssign[i];
		#endif

		ncfg[35] = fourwayfilter;
		ncfg[37] = fourwaypriorityup;

		ncfg[38] = soundbuffer;

		ncfg[36] = dtc;
		ncfg[42] = segacheat;
		ncfg[43] = fldtr;
		ncfg[44] =
			(( se & 0x1) << 23) | (( sevolume & 0x7F) << 16) |
			((bgm & 0x1) << 15) | ((bgmvolume & 0x7F) <<  8) |
			wavebgm;
		ncfg[45] = dispnext;
		ncfg[46] = movesound;
		ncfg[47] = fontsize;
		ncfg[48] = maxPlay;
		lastmaxPlay = maxPlay;

		ncfg[49] = breakeffect;
		ncfg[50] = showcombo;
		ncfg[51] = top_frame;
		lasttopframe = top_frame;

		ncfg[52] = w_reverse;

		ncfg[53] = downtype;
		ncfg[54] = lvupbonus;

		/* 59〜61はジョイスティック用 */
		ncfg[62] = fontc[0];
		ncfg[63] = digitc[0];
		ncfg[64] = fontc[1];
		ncfg[65] = digitc[1];
		ncfg[66] = fontc[2];
		ncfg[67] = digitc[2];
		ncfg[68] = fontc[3];
		ncfg[69] = digitc[3];
		ncfg[70] = fontc[4];
		ncfg[71] = digitc[4];
		ncfg[72] = fontc[5];
		ncfg[73] = digitc[5];
		ncfg[74] = fontc[6];
		ncfg[75] = digitc[6];
		ncfg[76] = fontc[7];
		ncfg[77] = digitc[7];
		ncfg[78] = fontc[8];
		ncfg[79] = digitc[8];

		#ifdef APP_ENABLE_JOYSTICK
		int32_t *joykeybuf = &ncfg[80];
		for (int32_t pl = 0; pl < 2; pl++) {
			int32_t *plbuf = &joykeybuf[pl * 10 * 8];
			APP_JoyKey *pljoy = &joyKeyAssign[pl * 10];
			plbuf[0] = pljoy->player;
			for (int32_t i = 0; i < 4; i++) {
				plbuf[1 + i] = pljoy->guid.data[i];
			}
			plbuf[5] = pljoy->type;
			switch (pljoy->type) {
			case APP_JOYKEY_AXIS:
			case APP_JOYKEY_HAT:
				plbuf[6] = pljoy->setting.index;
				plbuf[7] = pljoy->setting.value;
				break;

			case APP_JOYKEY_BUTTON:
				plbuf[6] = pljoy->setting.button;
				break;
			default:
				break;
			}
		}
		#endif
		
		#ifdef APP_ENABLE_GAME_CONTROLLER
		int32_t* conkeybuf = &ncfg[240];
		for (int32_t pl = 0; pl < 2; pl++) {
			conkeybuf[pl * (1 + 2 * 8)] = pl;
			int32_t* plbuf = &conkeybuf[pl * (1 + 2 * 8) + 1];
			for (int32_t key = 0; key < 8; key++) {
				const APP_ConKey* plcon = &conKeyAssign[pl * 8 + key];
				plbuf[key * 2 + 0] = plcon->type;
				plbuf[key * 2 + 1] = plcon->index;
			}
		}
		#endif

		ncfg[274] = spawn_y_type;
		ncfg[275] = hide_tomoyoinf;
		ncfg[276] = tspin_type;
		ncfg[277] = block_spectrum;
		ncfg[278] = next_adjust;
		ncfg[279] = Isbesttime;
		ncfg[280] = b2bcheck;
		ncfg[281] = debug;
		ncfg[282] = enable_grade[0];
		ncfg[283] = enable_grade[1];
		ncfg[284] = ace_irs;
		ncfg[285] = english;
		ncfg[286] = mini_select;
		ncfg[287] = bigtype;
		ncfg[288] = item_interval;
		ncfg[289] = hide_wait;
		ncfg[290] = vs_time;
		ncfg[291] = medaltype;
		ncfg[292] = p_shirase_start_level;
		ncfg[293] = uplinetype;
		ncfg[294] = disable_hold;
		ncfg[295] = disable_irs;
		ncfg[296] = cp_player_1p;
		ncfg[297] = cp_player_2p;
		ncfg[298] = cp_type;
		ncfg[299] = block_rframe;
		ncfg[300] = disable_wallkick;
		ncfg[301] = showctrl;

		for(i = 0; i < 10; i++) statusc[i] = 0;

		status[0] = 0;
		statusc[0] = 1;
		statusc[1] = 1;
		statusc[2] = 0;

		count = 0;

		need_reset = 0;
		need_reloadBG = 1;
		
		init = false;
	}

	count++;

	// 背景描画
	if(background == 0) {
		for(i = 0; i <= 4; i++) {
			if(getDrawRate() == 1)
				APP_BltFastRect(4, 96 * i - (count % 96) / 3, 0, 0, 0, 96, 240);
			else
				APP_BltFastRect(4, 192 * i - (count % 32), 0, 0, 0, 192, 480);
		}
	} else if(background == 1) {
		for(i = 0; i <= 4; i++) {
			ExBltFastRect(4, 96 * i, 0, 0, 0, 96, 240);
		}
	} else {
		ExBltFast(30, 0, 0);
	}
	ExBltRect(77, 0, 212,  count % 320, 20, 320 - (count % 320), 8);
	ExBltRect(77, 320 - (count % 320), 212,  0, 20, count % 320, 8);

	ExBltRect(77, count % 320, 36,  0, 28, 320 - (count % 320), 8);
	ExBltRect(77, 0, 36, 320 - (count % 320), 28, count % 320, 8);

	printFont(1, 1, "HEBORIS SETTING MENU", fontc[rots[0]]);

	if(status[0] == 0) {
		// game page 1
		printFont(23, 1, "- GAME P.1", fontc[rots[0]]);
		printFont(2,  3, "<< A/V <<	     >> GAME P.2 >>", digitc[rots[0]] * (statusc[0] == 0) * (count % 2));
		printFont(2,  6, "NEXT PATTERN     :", (statusc[0] == 1) * fontc[rots[0]]);
		printFont(2,  7, "NEXT DISPLAY     :", (statusc[0] == 2) * fontc[rots[0]]);
		printFont(2,  8, "8WAY INPUT       :", (statusc[0] == 3) * fontc[rots[0]]);
		printFont(2,  9, "SONIC DROP       :", (statusc[0] == 4) * fontc[rots[0]]);
		printFont(2, 10, "INIT LR MOVE     :", (statusc[0] == 5) * fontc[rots[0]]);
		printFont(2, 11, "BLOCK FALL       :", (statusc[0] == 6) * fontc[rots[0]]);
		printFont(2, 12, "SHOW LEVEL       :", (statusc[0] == 7) * fontc[rots[0]]);	// "TGM LEVEL"を"SHOW LEVEL"に変更 #1.60c7i2
		printFont(2, 13, "WORLDREVERSE     :", (statusc[0] == 8) * fontc[rots[0]]);
		printFont(2, 14, "DOWN TYPE        :", (statusc[0] == 9) * fontc[rots[0]]);
		printFont(2, 15, "LVUP BONUS       :", (statusc[0] == 10) * fontc[rots[0]]);
		printFont(2, 16, "OLDSTYLE ARS     :", (statusc[0] == 11) * fontc[rots[0]]); // allow CW and 180 i oldschool ARS
		printFont(2, 17, "SPAWN Y TYPE     :", (statusc[0] == 12) * fontc[rots[0]]);
		printFont(2, 18, "HIDE INFO(TOMOYO):", (statusc[0] == 13) * fontc[rots[0]]);
		printFont(2, 19, "T-SPIN TYPE      :", (statusc[0] == 14) * fontc[rots[0]]);
		printFont(2, 20, "BLOCK SPECTRUM   :", (statusc[0] == 15) * fontc[rots[0]]);
		printFont(2, 21, "NEXT ADJUST      :", (statusc[0] == 16) * fontc[rots[0]]);
		printFont(2, 22, "VIEW BEST TIME   :", (statusc[0] == 17) * fontc[rots[0]]);
		printFont(2, 23, "BACK TO BACK     :", (statusc[0] == 18) * fontc[rots[0]]);
		printFont(2, 24, "4WAY FILTER      :", (statusc[0] == 19) * fontc[rots[0]]);
		printFont(2, 25, "4WAY PRIORITY UP :", (statusc[0] == 20) * fontc[rots[0]]);

		printMenuButton(2, 28, APP_BUTTON_A, 0);
		printMenuButton(17, 28, APP_BUTTON_B, 0);
		printFont(2, 28, " :SAVE&RETURN   :CANCEL", 9);

		i = statusc[0];
		switch(i)
		{
		case 0:
			printFont(1, 3, "b", fontc[rots[0]]); break;
		default:
			printFont(1, 5 + i, "b", fontc[rots[0]]); break;
		}

		// next pattern
		if((ncfg[2] > 1)&&(ncfg[2] < 8))
		sprintf(string[0], "HEBO%" PRId32, ncfg[2]);
		else if(ncfg[2] == 0)
		sprintf(string[0], "RANDOM");
		else if(ncfg[2] == 1)
		sprintf(string[0], "MEMORY1");
		else if(ncfg[2] == 8)
		sprintf(string[0], "MEMORY4");
		else if(ncfg[2] == 9)
		sprintf(string[0], "GUIDELINE");
		else if(ncfg[2] == 10)
		sprintf(string[0], "DENGEN");
		else if(ncfg[2] == 11)
		sprintf(string[0], "TOMOYO");
		else if(ncfg[2] == 12)
		sprintf(string[0], "FP");
		else if(ncfg[2] == 13)
		sprintf(string[0], "SEGA");
		else if(ncfg[2] == 14)
		sprintf(string[0], "BLOXEED");
		else if (ncfg[2] == 15)
			sprintf(string[0], "EH-MEMORY6");
		else if (ncfg[2] == 16)
			sprintf(string[0], "MEMORY4-DP");
		printFont(20, 6, string[0], (statusc[0] == 1) * (count % 2) * digitc[rots[0]]);

		// next display
		sprintf(string[0], "%" PRId32, ncfg[45]);
		printFont(20, 7, string[0], (statusc[0] == 2) * (count % 2) * digitc[rots[0]]);

		// 8way input
		if(ncfg[5]) sprintf(string[0], "e"); // × 斜め入力
		else sprintf(string[0], "c");		// ○
		printFont(20, 8, string[0], (statusc[0] == 3) * (count % 2) * digitc[rots[0]]);

		// sonic drop
		if(ncfg[6]) sprintf(string[0], "e");	// × 高速落下
		else sprintf(string[0], "c");		// ○
		printFont(20, 9, string[0], (statusc[0] == 4) * (count % 2) * digitc[rots[0]]);

		// init lr move
		if(ncfg[7]) sprintf(string[0], "e");	// × 横先行入力
		else sprintf(string[0], "c");		// ○
		printFont(20, 10, string[0], (statusc[0] == 5) * (count % 2) * digitc[rots[0]]);

		// block fall
		if(ncfg[4]) sprintf(string[0], "SMOOTH");
		else sprintf(string[0], "NORMAL");
		printFont(20, 11, string[0], (statusc[0] == 6) * (count % 2) * digitc[rots[0]]);

		// show level
		if(ncfg[36]) sprintf(string[0], "ON");
		else sprintf(string[0], "OFF");
		printFont(20, 12, string[0], (statusc[0] == 7) * (count % 2) * digitc[rots[0]]);

		// WORLDREVERSE
		if(ncfg[52]) sprintf(string[0], "ON");
		else sprintf(string[0], "OFF");
		printFont(20, 13, string[0], (statusc[0] == 8) * (count % 2) * digitc[rots[0]]);

		// downtype
		if(ncfg[53]) sprintf(string[0], "DOWN RESET");
		else sprintf(string[0], "NO RESET(HEBORIS)");
		printFont(20, 14, string[0], (statusc[0] == 9) * (count % 2) * digitc[rots[0]]);

		// lvupbonus
		if(ncfg[54]==0) sprintf(string[0], "ON");
		else if(ncfg[54] ==1)sprintf(string[0], "OFF");
		else if(ncfg[54] ==2)sprintf(string[0], "ADJUST");
		printFont(20, 15, string[0], (statusc[0] == 10) * (count % 2) * digitc[rots[0]]);

		// segacheat
		if (ncfg[42]==0) sprintf(string[0], "CCW ONLY");
		else if (ncfg[42] == 1)sprintf(string[0], "CW AND 180 ALLOW");
		else if (ncfg[42] == 2)sprintf(string[0], "D.R.S. 3STATE+CW+180");
		printFont(20, 16, string[0], (statusc[0] == 11) * (count % 2) * digitc[rots[0]]);

		// spawn y type
		if(ncfg[274] == 0) sprintf(string[0], "FIELD IN");
		else sprintf(string[0], "FIELD OUT");
		printFont(20, 17, string[0], (statusc[0] == 12) * (count % 2) * digitc[rots[0]]);

		// hide info (tomoyo)
		if(ncfg[275] == 0) sprintf(string[0], "NOT HIDE");
		else sprintf(string[0], "HIDE");
		printFont(20, 18, string[0], (statusc[0] == 13) * (count % 2) * digitc[rots[0]]);

		// t-spin type
		     if(ncfg[276] == 0) sprintf(string[0], "NO CHECK");
		else if(ncfg[276] == 1) sprintf(string[0], "ERASE(NO BONUS)");
		else if(ncfg[276] == 2) sprintf(string[0], "3-C(BONUS)");
		else if(ncfg[276] == 3) sprintf(string[0], "BOTH(BONUS:3-C)");
		printFont(20, 19, string[0], (statusc[0] == 14) * (count % 2) * digitc[rots[0]]);

		// block spectrum
		if(ncfg[277] == 0) sprintf(string[0], "OFF");
		else sprintf(string[0], "ON");
		printFont(20, 20, string[0], (statusc[0] == 15) * (count % 2) * digitc[rots[0]]);

		// next adjust
		if(ncfg[278] == 0) sprintf(string[0], "OFF");
		else sprintf(string[0], "ON(NOT FIRST OSZ)");
		printFont(20, 21, string[0], (statusc[0] == 16) * (count % 2) * digitc[rots[0]]);

		// view best time
		if(ncfg[279] == 0) sprintf(string[0], "OFF");
		else sprintf(string[0], "ON");
		printFont(20, 22, string[0], (statusc[0] == 17) * (count % 2) * digitc[rots[0]]);

		// back to back
		if(ncfg[280] == 0) sprintf(string[0], "OFF");
		else sprintf(string[0], "ON");
		printFont(20, 23, string[0], (statusc[0] == 18) * (count % 2) * digitc[rots[0]]);

		     if(ncfg[35] == 0) sprintf(string[0], "1P&2P");
		else if(ncfg[35] == 1) sprintf(string[0], "ONLY 1P");
		else if(ncfg[35] == 2) sprintf(string[0], "ONLY 2P");
		else if(ncfg[35] == 3) sprintf(string[0], "OFF");
		printFont(20, 24, string[0], (statusc[0] == 19) * (count % 2) * digitc[rots[0]]);

		     if(ncfg[37] == 0) sprintf(string[0], "1P&2P");
		else if(ncfg[37] == 1) sprintf(string[0], "ONLY 1P");
		else if(ncfg[37] == 2) sprintf(string[0], "ONLY 2P");
		else if(ncfg[37] == 3) sprintf(string[0], "OFF");
		printFont(20, 25, string[0], (statusc[0] == 20) * (count % 2) * digitc[rots[0]]);

		statusc[1] = 0;

		for(pl = 0; pl < 2; pl++) {
			// ↑↓カーソルリピード #1.60c7k8
			m=0;
			padRepeat2(pl);
			// ↑
			if((mpc2[0] == 1) || ((mpc2[0] > tame3) && (mpc2[0] % tame4 == 0)))
				if(getPressState(pl, APP_BUTTON_UP)) m--;

			// ↓
			if((mpc2[0] == 1) || ((mpc2[0] > tame3) && (mpc2[0] % tame4 == 0)))
				if(getPressState(pl, APP_BUTTON_DOWN)) m++;
			padRepeat(pl);
			if(m) {
				PlaySE(5);
				statusc[0] = (statusc[0] + m + 21) % 21;
			}
			// HOLDボタンでページ切り替え #1.60c7k8
			if(getPushState(pl, APP_BUTTON_D)) {
				assert(pages > 0);
				status[0] = (status[0] + 1 + pages)%pages;
				statusc[0] = 0;
				statusc[1] = 1;
			} else {
				m = getPushState(pl, APP_BUTTON_RIGHT) - getPushState(pl, APP_BUTTON_LEFT);

				if(m) {
					     if(statusc[0] ==  1) ncfg[2] = (ncfg[2] + 17 + m) % 17;	// nextbloc 8を追加#1.60c7h4
					else if(statusc[0] ==  2) ncfg[45] = (ncfg[45] + 7 + m) % 7;	// dispnext
					else if(statusc[0] ==  3) ncfg[5] = !ncfg[5];			// nanameallow
					else if(statusc[0] ==  4) ncfg[6] = !ncfg[6];			// sonicdrop
					else if(statusc[0] ==  5) ncfg[7] = !ncfg[7];			// fastlrmove
					else if(statusc[0] ==  6) ncfg[4] = !ncfg[4];			// smooth
					else if(statusc[0] ==  7) ncfg[36] = !ncfg[36];			// tgmlv
					else if(statusc[0] ==  8) ncfg[52] = !ncfg[52];			// w_reverse
					else if(statusc[0] ==  9) ncfg[53] = !ncfg[53];			// downtype
					else if(statusc[0] == 10) ncfg[54] = (ncfg[54] + 3 + m)%3;	// lvupbonus
					else if(statusc[0] == 11) ncfg[42] = (ncfg[42] + 3 + m)%3;	// segacheat
					else if(statusc[0] == 12) ncfg[274] = !ncfg[274];		// spawn y type
					else if(statusc[0] == 13) ncfg[275] = !ncfg[275];		// hide info (tomoyo)
					else if(statusc[0] == 14) ncfg[276] = (ncfg[276] + 4 + m) % 4;	// t-spin type
					else if(statusc[0] == 15) ncfg[277] = !ncfg[277];		// block spectrum
					else if(statusc[0] == 16) ncfg[278] = !ncfg[278];		// next adjust
					else if(statusc[0] == 17) ncfg[279] = !ncfg[279];		// view best time
					else if(statusc[0] == 18) ncfg[280] = !ncfg[280];		// back to back
					else if(statusc[0] == 19) ncfg[35] = (ncfg[35] + 4 + m)%4;	// four way filter
					else if(statusc[0] == 20) ncfg[37] = (ncfg[37] + 4 + m)%4;	// four way priority up

					else if(statusc[0] == 0) {	// page
						PlaySE(3);
						status[0] = (status[0] + m + pages) % pages;
						statusc[0] = 0;
						statusc[1] = 0;
					}
					statusc[1] = 1;
				}
			}
			if(getPushState(pl, APP_BUTTON_A)) {	// A:保存&再起動
				APP_PlayWave(10);
				screenMode = ncfg[0];
				screenIndex = ncfg[1];
				nextblock = ncfg[2];
				cfgversion = (uint32_t)ncfg[3];
				smooth = ncfg[4];
				nanameallow = ncfg[5];
				sonicdrop = ncfg[6];
				fastlrmove = ncfg[7];
				blockflash = ncfg[8];
				background = ncfg[9];

				fourwayfilter = ncfg[35];
				fourwaypriorityup = ncfg[37];
				soundbuffer = ncfg[38];
				dtc = ncfg[36];
				segacheat = ncfg[42];
				fldtr = ncfg[43];
				se = (ncfg[44] >> 23) & 0x1;
				sevolume = (ncfg[44] >> 16) & 0x7F;
				bgm = (ncfg[44] >> 15) & 0x1;
				bgmvolume = (ncfg[44] >> 8) & 0x7F;
				wavebgm = ncfg[44] & APP_SOUND_BITS_MASK;
				dispnext = ncfg[45];
				movesound = ncfg[46];
				fontsize = ncfg[47];
				maxPlay = ncfg[48];
				breakeffect = ncfg[49];
				showcombo = ncfg[50];
				top_frame = ncfg[51];
				w_reverse = ncfg[52];
				downtype = ncfg[53];
				lvupbonus = ncfg[54];

				fontc[0] = ncfg[62];
				digitc[0] = ncfg[63];
				fontc[1] = ncfg[64];
				digitc[1] = ncfg[65];
				fontc[2] = ncfg[66];
				digitc[2] = ncfg[67];
				fontc[3] = ncfg[68];
				digitc[3] = ncfg[69];
				fontc[4] = ncfg[70];
				digitc[4] = ncfg[71];
				fontc[5] = ncfg[72];
				digitc[5] = ncfg[73];
				fontc[6] = ncfg[74];
				digitc[6] = ncfg[75];
				fontc[7] = ncfg[76];
				digitc[7] = ncfg[77];
				fontc[8] = ncfg[78];
				digitc[8] = ncfg[79];

				spawn_y_type = ncfg[274];
				hide_tomoyoinf = ncfg[275];
				tspin_type = ncfg[276];
				block_spectrum = ncfg[277];
				next_adjust = ncfg[278];
				Isbesttime = ncfg[279];
				b2bcheck = ncfg[280];
				debug = ncfg[281];
				enable_grade[0] = ncfg[282];
				enable_grade[1] = ncfg[283];
				ace_irs = ncfg[284];
				english = ncfg[285];
				mini_select = ncfg[286];
				bigtype = ncfg[287];
				item_interval = ncfg[288];
				hide_wait = ncfg[289];
				vs_time = ncfg[290];
				medaltype = ncfg[291];
				p_shirase_start_level = ncfg[292];
				uplinetype = ncfg[293];
				disable_hold = ncfg[294];
				disable_irs = ncfg[295];
				cp_player_1p = ncfg[296];
				cp_player_2p = ncfg[297];
				cp_type = ncfg[298];
				block_rframe = ncfg[299];
				disable_wallkick = ncfg[300];
				showctrl = ncfg[301];
				SaveConfig();
				if((maxPlay == lastmaxPlay) && (top_frame == lasttopframe) && (((last_BG <= 1) && (background <= 1)) || ((last_BG == 2) && (background == 2))))
					need_reloadBG = 0;

				tmp_maxPlay = maxPlay;

				if(need_reset) {
					restart = 1;	// 再起動フラグ
				} else if(need_reloadBG==1){	//プレイする最大人数の変更があったら…
					loadBG(maxPlay,0);			// 背景だけ再読み込み C7T2.5EX
				}
				status[0] = -1;
			}

			if(getPushState(pl, APP_BUTTON_B)) {	// B:設定破棄&タイトル画面に戻る
				SetVolumeAllWaves(sevolume);
				SetVolumeAllBGM(bgmvolume);
				APP_SetMusicVolume(bgmvolume);
				status[0] = -1;
			}
		}
	} else if(status[0] == 1) {
		// game page 2
		printFont(23, 1, "- GAME P.2", fontc[rots[0]]);
		printFont(2,  3, "<< GAME P.1 <<	  >> DESIGN >>", digitc[rots[0]] * (statusc[0] == 0) * (count % 2));
		printFont(2,  6, "DEBUG MODE        :", (statusc[0] == 1) * fontc[rots[0]]);
		printFont(2,  7, "GRADE TYPE        :", (statusc[0] == 2) * fontc[rots[0]]);
		printFont(2,  8, "IRS TYPE          :", (statusc[0] == 3) * fontc[rots[0]]);
		printFont(2,  9, "LANGUAGE          :", (statusc[0] == 4) * fontc[rots[0]]);
		printFont(2, 10, "MINI SELECT       :", (statusc[0] == 5) * fontc[rots[0]]);
		printFont(2, 11, "BIG MOVE TYPE     :", (statusc[0] == 6) * fontc[rots[0]]);
		printFont(2, 12, "ITEM INTERVAL     :", (statusc[0] == 7) * fontc[rots[0]]);
		printFont(2, 13, "HIDE WAITS        :", (statusc[0] == 8) * fontc[rots[0]]);
		printFont(2, 14, "VS LIMIT TIME(SEC):", (statusc[0] == 9) * fontc[rots[0]]);
		printFont(2, 15, "MEDAL GRAPHICS    :", (statusc[0] == 10) * fontc[rots[0]]);
		printFont(2, 16, "DEVIL RISE STARTLV:", (statusc[0] == 11) * fontc[rots[0]]);
		printFont(2, 17, "RISE TYPE         :", (statusc[0] == 12) * fontc[rots[0]]);
		printFont(2, 18, "HOLD              :", (statusc[0] == 13) * fontc[rots[0]]);
		printFont(2, 19, "IRS               :", (statusc[0] == 14) * fontc[rots[0]]);
		printFont(2, 20, "USE CPU(1P)       :", (statusc[0] == 15) * fontc[rots[0]]);
		printFont(2, 21, "USE CPU(2P)       :", (statusc[0] == 16) * fontc[rots[0]]);
		printFont(2, 22, "CPU TYPE          :", (statusc[0] == 17) * fontc[rots[0]]);
		printFont(2, 23, "BLOCK ROTATE FRAME:", (statusc[0] == 18) * fontc[rots[0]]);
		printFont(2, 24, "WALL KICK         :", (statusc[0] == 19) * fontc[rots[0]]);
		printFont(2, 25, "SHOW 1P CONTROL   :", (statusc[0] == 20) * fontc[rots[0]]);

		printMenuButton(2, 28, APP_BUTTON_A, 0);
		printMenuButton(4, 28, APP_BUTTON_B, 0);
		printFont(2, 28, " / :RETURN", 9);

		i = statusc[0];
		switch(i)
		{
		case 0:
			printFont(1, 3, "b", fontc[rots[0]]); break;
		default:
			printFont(1, 5 + i, "b", fontc[rots[0]]); break;
		}

		// debug mode
		if(ncfg[281] == 0) sprintf(string[0], "OFF");
		else sprintf(string[0], "ON");
		printFont(21, 6, string[0], (statusc[0] == 1) * (count % 2) * digitc[rots[0]]);

		// grade type
		sprintf(string[0], "%" PRId32, ncfg[282]);
		printFont(21, 7, string[0], (statusc[0] == 2) * (count % 2) * digitc[rots[0]]);

		// irs type
		if(ncfg[284] == 0) sprintf(string[0], "CLASSIC");
		else if(ncfg[284] == 1)sprintf(string[0], "ACE");
		else sprintf(string[0], "ACE+");
		printFont(21, 8, string[0], (statusc[0] == 3) * (count % 2) * digitc[rots[0]]);

		// language
		if(ncfg[285] == 0) sprintf(string[0], "JAPANESE");
		else sprintf(string[0], "ENGLISH");
		printFont(21, 9, string[0], (statusc[0] == 4) * (count % 2) * digitc[rots[0]]);
		
		// mini select
		if(ncfg[286] == 0) sprintf(string[0], "OFF");
		else sprintf(string[0], "ON");
		printFont(21, 10, string[0], (statusc[0] == 5) * (count % 2) * digitc[rots[0]]);
		
		// big move type
		if(ncfg[287] == 0) sprintf(string[0], "1CELL");
		else if(ncfg[287] == 1)sprintf(string[0], "2CELL");
		else sprintf(string[0], "TOMOYO:1/OTHER:2");
		printFont(21, 11, string[0], (statusc[0] == 6) * (count % 2) * digitc[rots[0]]);
		
		// item interval
		sprintf(string[0], "%" PRId32, ncfg[288]);
		printFont(21, 12, string[0], (statusc[0] == 7) * (count % 2) * digitc[rots[0]]);
		
		// hide waits
		if(ncfg[289] == 0) sprintf(string[0], "NOT HIDE");
		else sprintf(string[0], "HIDE");
		printFont(21, 13, string[0], (statusc[0] == 8) * (count % 2) * digitc[rots[0]]);

		// versus limit time (seconds)
		sprintf(string[0], "%" PRId32, ncfg[290] / 60);
		printFont(21, 14, string[0], (statusc[0] == 9) * (count % 2) * digitc[rots[0]]);
		
		// medal graphics
		if(ncfg[291] == 0) sprintf(string[0], "f");	//□
		else if(ncfg[291] == 1) sprintf(string[0], "c");//○
		else sprintf(string[0], "OFF");
		printFont(21, 15, string[0], (statusc[0] == 10) * (count % 2) * digitc[rots[0]]);
		
		// devil rise start level
		sprintf(string[0], "%" PRId32, ncfg[292]);
		printFont(21, 16, string[0], (statusc[0] == 11) * (count % 2) * digitc[rots[0]]);
		
		// rise type
		if(ncfg[293] == 0) sprintf(string[0], "COPY");
		else if(ncfg[293] == 1)sprintf(string[0], "PATTERN");
		else if(ncfg[293] == 2)sprintf(string[0], "RANDOM");
		else sprintf(string[0], "REVERSE ERASE FIELD");
		printFont(21, 17, string[0], (statusc[0] == 12) * (count % 2) * digitc[rots[0]]);
		
		// hold
		if(ncfg[294] == 0) sprintf(string[0], "ENABLE");
		else sprintf(string[0], "DISABLE");
		printFont(21, 18, string[0], (statusc[0] == 13) * (count % 2) * digitc[rots[0]]);
		
		// irs
		if(ncfg[295] == 0) sprintf(string[0], "ENABLE");
		else sprintf(string[0], "DISABLE");
		printFont(21, 19, string[0], (statusc[0] == 14) * (count % 2) * digitc[rots[0]]);
		
		// use cpu (1p)
		if(ncfg[296] == 0) sprintf(string[0], "OFF");
		else sprintf(string[0], "ON");
		printFont(21, 20, string[0], (statusc[0] == 15) * (count % 2) * digitc[rots[0]]);
		
		// use cpu (2p)
		if(ncfg[297] == 0) sprintf(string[0], "OFF");
		else sprintf(string[0], "ON");
		printFont(21, 21, string[0], (statusc[0] == 16) * (count % 2) * digitc[rots[0]]);
		
		// cpu type
		if(ncfg[298] == 0) sprintf(string[0], "ERASE SOON");
		else sprintf(string[0], "ERASE STORE");
		printFont(21, 22, string[0], (statusc[0] == 17) * (count % 2) * digitc[rots[0]]);
		
		// block rotate frame
		if(ncfg[299] == 0) sprintf(string[0], "OFF");
		else sprintf(string[0], "ON");
		printFont(21, 23, string[0], (statusc[0] == 18) * (count % 2) * digitc[rots[0]]);
		
		// wall kick
		if(ncfg[300] == 0) sprintf(string[0], "ENABLE");
		else sprintf(string[0], "DISABLE");
		printFont(21, 24, string[0], (statusc[0] == 19) * (count % 2) * digitc[rots[0]]);
		
		// show 1p control
		if(ncfg[301] == 0) sprintf(string[0], "REPLAY ONLY");
		else if(ncfg[301] == 1) sprintf(string[0], "ALWAYS ON");
		else sprintf(string[0], "ALWAYS OFF");
		printFont(21, 25, string[0], (statusc[0] == 20) * (count % 2) * digitc[rots[0]]);

		for(pl = 0; pl < 2; pl++) {
			padRepeat(pl);
			m=0;
			padRepeat2(pl);
			// ↑
			if((mpc2[0] == 1) || ((mpc2[0] > tame3) && (mpc2[0] % tame4 == 0)))
				if(getPressState(pl, APP_BUTTON_UP)) m--;

			// ↓
			if((mpc2[0] == 1) || ((mpc2[0] > tame3) && (mpc2[0] % tame4 == 0)))
				if(getPressState(pl, APP_BUTTON_DOWN)) m++;
			if(m) {
				PlaySE(5);
				statusc[0] = (statusc[0] + m + 21) % 21;
			}

			// HOLDボタンでページ切り替え #1.60c7k8
			if(getPushState(pl, APP_BUTTON_D)) {
				PlaySE(3);
				status[0] = (status[0] + 1 + pages)%pages;
				statusc[0] = 0;
				statusc[1] = 1;
			} else {
				m = getPushState(pl, APP_BUTTON_RIGHT) - getPushState(pl, APP_BUTTON_LEFT);
				if(m) {
					     if(statusc[0] ==   1) ncfg[281] = !ncfg[281];
					else if(statusc[0] ==   2) ncfg[282] = (ncfg[282] + 5 + m) % 5;
					else if(statusc[0] ==   3) ncfg[284] = (ncfg[284] + 3 + m) % 3;
					else if(statusc[0] ==   4) ncfg[285] = !ncfg[285];
					else if(statusc[0] ==   5) ncfg[286] = !ncfg[286];
					else if(statusc[0] ==   6) ncfg[287] = (ncfg[287] + 3 + m) % 3;
					else if(statusc[0] ==   7) ncfg[288] = (ncfg[288] + 51 + m) % 51;
					else if(statusc[0] ==   8) ncfg[289] = !ncfg[289];
					else if(statusc[0] ==   9) ncfg[290] = (ncfg[290] + 37800 + m * 1800) % 37800;
					else if(statusc[0] ==  10) ncfg[291] = (ncfg[291] + 3 + m) % 3;
					else if(statusc[0] ==  11) ncfg[292] = (ncfg[292] + 1050 + m * 50) % 1050;
					else if(statusc[0] ==  12) ncfg[293] = (ncfg[293] + 3 + m) % 3;
					else if(statusc[0] ==  13) ncfg[294] = !ncfg[294];
					else if(statusc[0] ==  14) ncfg[295] = !ncfg[295];
					else if(statusc[0] ==  15) ncfg[296] = !ncfg[296];
					else if(statusc[0] ==  16) ncfg[297] = !ncfg[297];
					else if(statusc[0] ==  17) ncfg[298] = !ncfg[298];
					else if(statusc[0] ==  18) ncfg[299] = !ncfg[299];
					else if(statusc[0] ==  19) ncfg[300] = !ncfg[300];
					else if(statusc[0] ==  20) ncfg[301] = (ncfg[301] + 3 + m) % 3;

					else if(statusc[0] == 0) {
						PlaySE(3);
						status[0] = (status[0] + m + pages)%pages;
						statusc[0] = 0;
					}
					statusc[1] = 1;
				}
			}

			if(getPushState(pl, APP_BUTTON_A) || getPushState(pl, APP_BUTTON_B)) {	// A&B:mainに戻る
				PlaySE(3);
				status[0] = 0;
				statusc[0] = 0;
				statusc[1] = 1;
			}
		}
	} else if(status[0] == 2) {
		// design setting
		printFont(23, 1, "- DESIGN SETTING", fontc[rots[0]]);
		printFont(2, 3, "<< GAME P.2 <<	   >> INPUT >>", digitc[rots[0]] * (statusc[0] == 0) * (count % 2));

		printFont( 2,  6, "BLOCK FRAME    :", fontc[rots[0]] * (statusc[0] == 1));
		printFont( 2,  7, "FIELD BG       :", fontc[rots[0]] * (statusc[0] == 2));
		printFont( 2,  8, "FIELD BG TR    :", fontc[rots[0]] * (statusc[0] == 3));

		printFont( 2,  9, "FONT [HEBORIS] :", fontc[rots[0]] * (statusc[0] == 4));
		printFont( 2, 10, "FONT [TI-ARS]  :", fontc[rots[0]] * (statusc[0] == 5));
		printFont( 2, 11, "FONT [TI-WORLD]:", fontc[rots[0]] * (statusc[0] == 6));
		printFont( 2, 12, "FONT [ACE-SRS] :", fontc[rots[0]] * (statusc[0] == 7));
		printFont( 2, 13, "FONT [ACE-ARS] :", fontc[rots[0]] * (statusc[0] == 8));
		printFont( 2, 14, "FONT [ACE-ARS2]:", fontc[rots[0]] * (statusc[0] == 9));
		printFont( 2, 15, "FONT [DS-WORLD]:", fontc[rots[0]] * (statusc[0] == 10));
		printFont( 2, 16, "FONT [SRS-X]   :", fontc[rots[0]] * (statusc[0] == 11));
		printFont( 2, 17, "FONT [DRS]     :", fontc[rots[0]] * (statusc[0] == 12));

		printFont(22,  9, "DIGIT[HEBORIS] :", fontc[rots[0]] * (statusc[0] == 13));
		printFont(22, 10, "DIGIT[TI-ARS]  :", fontc[rots[0]] * (statusc[0] == 14));
		printFont(22, 11, "DIGIT[TI-WORLD]:", fontc[rots[0]] * (statusc[0] == 15));
		printFont(22, 12, "DIGIT[ACE-SRS] :", fontc[rots[0]] * (statusc[0] == 16));
		printFont(22, 13, "DIGIT[ACE-ARS] :", fontc[rots[0]] * (statusc[0] == 17));
		printFont(22, 14, "DIGIT[ACE-ARS2]:", fontc[rots[0]] * (statusc[0] == 18));
		printFont(22, 15, "DIGIT[DS-WORLD]:", fontc[rots[0]] * (statusc[0] == 19));
		printFont(22, 16, "DIGIT[SRS-X]   :", fontc[rots[0]] * (statusc[0] == 20));
		printFont(22, 17, "DIGIT[DRS]     :", fontc[rots[0]] * (statusc[0] == 21));

		printFont( 2, 19, "NUMBER FONT    :", fontc[rots[0]] * (statusc[0] == 22));
		printFont( 2, 20, "MAX PLAYER     :", fontc[rots[0]] * (statusc[0] == 23));
		printFont( 2, 21, "BREAK EFFECT   :", fontc[rots[0]] * (statusc[0] == 24));
		printFont( 2, 22, "SHOW COMBOS    :", fontc[rots[0]] * (statusc[0] == 25));
		printFont( 2, 23, "TOP FRAME      :", fontc[rots[0]] * (statusc[0] == 26));

		printMenuButton(2, 28, APP_BUTTON_A, 0);
		printMenuButton(4, 28, APP_BUTTON_B, 0);
		printFont(2, 28, " / :RETURN", 9);

		i = statusc[0];
		if((i >= 13) && (i <= 21)) {
			printFont(21, i - 4, "b", fontc[rots[0]]);
		} else if(i > 21) {
			printFont(1, i - 3, "b", fontc[rots[0]]);
		} else if(i == 0) {
			printFont(1, 3, "b", fontc[rots[0]]);
		} else {
			printFont(1, 5 + i, "b", fontc[rots[0]]);
		}

		if(ncfg[8] == 0) sprintf(string[0], "BLINK");
		else if(ncfg[8] == 1) sprintf(string[0], "ON");
		else if(ncfg[8] == 2) sprintf(string[0], "OFF");
		else if(ncfg[8] == 3) sprintf(string[0], "DRAW ONLY FIELD BLOCKS");
		printFont(18, 6, string[0], (statusc[0] == 1) * (count % 2) * digitc[rots[0]]);

		if(ncfg[9] == 0) sprintf(string[0], "GRID (SCROLLING)");
		else if(ncfg[9] == 1) sprintf(string[0], "GRID");
		else sprintf(string[0], "BLACK");
		printFont(18, 7, string[0], (statusc[0] == 2) * (count % 2) * digitc[rots[0]]);

		sprintf(string[0], "%" PRId32, ncfg[43]);
		printFont(18, 8, string[0], (statusc[0] == 3) * (count % 2) * digitc[rots[0]]);

		for(i = 0; i < 9; i++) { //FONT
			sprintf(string[0], "%" PRId32, ncfg[62+(2*i)]);
			printFont(18, 9+i, string[0], ncfg[62+(2*i)]);
		}
		for(i = 0; i < 9; i++) {//DIGIT
			sprintf(string[0], "%" PRId32, ncfg[63+(2*i)]);
			printFont(38, 9+i, string[0], ncfg[63+(2*i)]);
		}

		if(ncfg[47] == 0) sprintf(string[0], "DEFAULT");
		else sprintf(string[0], "SHARP");
		printFont(18, 19, string[0], (statusc[0] == 22) * (count % 2) * digitc[rots[0]]);

		if(ncfg[48] == 0) sprintf(string[0], "SINGLE");
		else sprintf(string[0], "DUAL");
		printFont(18, 20, string[0], (statusc[0] == 23) * (count % 2) * digitc[rots[0]]);

		//breakefect
		if(ncfg[49] == 0) sprintf(string[0], "OFF");
		else sprintf(string[0], "ON");
		printFont(18, 21, string[0], (statusc[0] == 24) * (count % 2) * digitc[rots[0]]);

		//showcombo
		if(ncfg[50] == 0) sprintf(string[0], "OFF");
		else sprintf(string[0], "ON");
		printFont(18, 22, string[0], (statusc[0] == 25) * (count % 2) * digitc[rots[0]]);

		//top frame
		if(ncfg[51] == 0) sprintf(string[0], "TI");
		else sprintf(string[0], "ACE");
		printFont(18, 23, string[0], (statusc[0] == 26) * (count % 2) * digitc[rots[0]]);

		statusc[1] = 0;

		for(pl = 0; pl < 2; pl++) {
			padRepeat(pl);
			m=0;
			padRepeat2(pl);
			// ↑
			if((mpc2[0] == 1) || ((mpc2[0] > tame3) && (mpc2[0] % tame4 == 0)))
				if(getPressState(pl, APP_BUTTON_UP)) m--;

			// ↓
			if((mpc2[0] == 1) || ((mpc2[0] > tame3) && (mpc2[0] % tame4 == 0)))
				if(getPressState(pl, APP_BUTTON_DOWN)) m++;
			if(m) {
				PlaySE(5);
				statusc[0] = (statusc[0] + m + 27) % 27;
			}

			// HOLDボタンでページ切り替え #1.60c7k8
			if(getPushState(pl, APP_BUTTON_D)) {
				PlaySE(3);
				status[0] = (status[0] + 1 + pages)%pages;
				statusc[0] = 0;
				statusc[1] = 1;
			} else {
				m = getPushState(pl, APP_BUTTON_RIGHT) - getPushState(pl, APP_BUTTON_LEFT);
				if(m) {
					if(statusc[0] == 1) ncfg[8] = (ncfg[8] + 4 + m) % 4;	// blockflash
					if(statusc[0] == 2) ncfg[9] = (ncfg[9] + 3 + m) % 3;	// background

					if(statusc[0] == 4) ncfg[62] = (ncfg[62] + m + 10) % 10;
					if(statusc[0] == 5) ncfg[64] = (ncfg[64] + m + 10) % 10;
					if(statusc[0] == 6) ncfg[66] = (ncfg[66] + m + 10) % 10;
					if(statusc[0] == 7) ncfg[68] = (ncfg[68] + m + 10) % 10; // WORLD2 FONT
					if(statusc[0] == 8) ncfg[70] = (ncfg[70] + m + 10) % 10; // ARS FONT
					if(statusc[0] == 9) ncfg[72] = (ncfg[72] + m + 10) % 10; // ARS2 FONT
					if(statusc[0] == 10) ncfg[74] = (ncfg[74] + m + 10) % 10; // WORLD3 FONT
					if(statusc[0] == 11) ncfg[76] = (ncfg[76] + m + 10) % 10;
					if(statusc[0] == 12) ncfg[78] = (ncfg[78] + m + 10) % 10;

					if(statusc[0] == 13) ncfg[63] = (ncfg[63] + m + 10) % 10;
					if(statusc[0] == 14) ncfg[65] = (ncfg[65] + m + 10) % 10;
					if(statusc[0] == 15) ncfg[67] = (ncfg[67] + m + 10) % 10;
					if(statusc[0] == 16) ncfg[69] = (ncfg[69] + m + 10) % 10; // WORLD2 DIGIT
					if(statusc[0] == 17) ncfg[71] = (ncfg[71] + m + 10) % 10; // ARS DIGIT
					if(statusc[0] == 18) ncfg[73] = (ncfg[73] + m + 10) % 10; // ARS2 DIGIT
					if(statusc[0] == 19) ncfg[75] = (ncfg[75] + m + 10) % 10; // WORLD3 DIGIT
					if(statusc[0] == 20) ncfg[77] = (ncfg[77] + m + 10) % 10;
					if(statusc[0] == 21) ncfg[79] = (ncfg[79] + m + 10) % 10;

					if(statusc[0] == 22) ncfg[47] = !ncfg[47];
					if(statusc[0] == 23) ncfg[48] = !ncfg[48];
					if(statusc[0] == 24) ncfg[49] = !ncfg[49];	// breakeffect
					if(statusc[0] == 25) ncfg[50] = !ncfg[50];	// showcombo
					if(statusc[0] == 26) ncfg[51] = !ncfg[51];	// top_frame

					if(statusc[0] == 0){
						PlaySE(3);
						status[0] = (status[0] + m + pages)%pages;
						statusc[0] = 0;
					}
					statusc[1] = 1;
				}
			}
			if(mp[pl] || m) {
				if(statusc[0] == 3){	// fldtr
					if(m) ncfg[43] = (ncfg[43] + 257 + m) % 257;
					else if(mpc[pl] > 30) ncfg[43] = (ncfg[43] + 252 + mp[pl] * 2) % 257;
				}
				statusc[1] = 1;
			}

			if(getPushState(pl, APP_BUTTON_A) || getPushState(pl, APP_BUTTON_B)) {	// A&B:mainに戻る
				PlaySE(3);
				status[0] = 0;
				statusc[0] = 0;
				statusc[1] = 1;
			}
		}
	} else if(status[0] == 3) {
		// input setting
		// menu
		int32_t optionIndex = 0;
		if(statusc[2] == optionIndex++) {
			printFont(23, 1, "- INPUT SETTING", fontc[rots[0]]);
			printFont(2,  3, "<< DESIGN <<	       >> A/V >>", digitc[rots[0]] * (statusc[0] == 0) * (count % 2));
			int32_t numOptions = 0;
			#ifdef APP_ENABLE_KEYBOARD
			numOptions++; printFont(2, 4 + numOptions * 2, "[KEYBOARD 1P]", fontc[rots[0]] * (statusc[0] == numOptions));
			numOptions++; printFont(2, 4 + numOptions * 2, "[KEYBOARD 2P]", fontc[rots[0]] * (statusc[0] == numOptions));
			#endif
			#ifdef APP_ENABLE_JOYSTICK
			numOptions++; printFont(2, 4 + numOptions * 2, "[JOYSTICK 1P]", fontc[rots[0]] * (statusc[0] == numOptions));
			numOptions++; printFont(2, 4 + numOptions * 2, "[JOYSTICK 2P]", fontc[rots[0]] * (statusc[0] == numOptions));
			#endif
			#ifdef APP_ENABLE_GAME_CONTROLLER
			numOptions++; printFont(2, 4 + numOptions * 2, "[GAME CONTROLLER 1P]", fontc[rots[0]] * (statusc[0] == numOptions));
			numOptions++; printFont(2, 4 + numOptions * 2, "[GAME CONTROLLER 2P]", fontc[rots[0]] * (statusc[0] == numOptions));
			#endif
			numOptions++; printFont(2, 4 + numOptions * 2, "[INPUT TEST]", fontc[rots[0]] * (statusc[0] == numOptions));
			numOptions++; printFont(2, 4 + numOptions * 2, "[DISP ASSIGN]", fontc[rots[0]] * (statusc[0] == numOptions));
			printMenuButton(2, 28, APP_BUTTON_A, 0);
			printMenuButton(12, 28, APP_BUTTON_B, 0);
			printFont(2, 28, " :SELECT   :RETURN", 9);

			if(statusc[0] != 0) printFont(1, 4+statusc[0]*2, "b", fontc[rots[0]]);
			else printFont(1, 3, "b", fontc[rots[0]]);

			for(pl = 0; pl < 2; pl++) {
				// ↑↓カーソルリピード #1.60c7k8
				m=0;
				padRepeat2(pl);
				// ↑
				if((mpc2[0] == 1) || ((mpc2[0] > tame3) && (mpc2[0] % tame4 == 0)))
					if(getPressState(pl, APP_BUTTON_UP)) m--;

				// ↓
				if((mpc2[0] == 1) || ((mpc2[0] > tame3) && (mpc2[0] % tame4 == 0)))
					if(getPressState(pl, APP_BUTTON_DOWN)) m++;
				if(m) {
					PlaySE(5);
					statusc[0] = (statusc[0] + m + numOptions + 1) % (numOptions + 1);
				}
				// HOLDボタンでページ切り替え #1.60c7k8
				if(getPushState(pl, APP_BUTTON_D)) {
					PlaySE(3);
					status[0] = (status[0] + 1 + pages)%pages;
					statusc[0] = 0;
					statusc[1] = 1;
				} else {
					m = getPushState(pl, APP_BUTTON_RIGHT) - getPushState(pl, APP_BUTTON_LEFT);
					if(m) {
						if(statusc[0] == 0){
							PlaySE(3);
							status[0] = (status[0] + m + pages)%pages;
							statusc[0] = 0;
							statusc[1] = 1;
						}
					}
				}

				if(getPushState(pl, APP_BUTTON_A) && (statusc[0] != 0)) {
					PlaySE(19);
					statusc[2] = statusc[0];
					statusc[0] = 0;
					conPlayer = -1;
				}

				if(getPushState(pl, APP_BUTTON_B)) {
					PlaySE(3);
					status[0] = 0;
					statusc[0] = 0;
					statusc[1] = 1;
				}
			}
		}
		#ifdef APP_ENABLE_KEYBOARD
		else if(optionIndex += 2, (statusc[2] == optionIndex - 2) || (statusc[2] == optionIndex - 1)) {
			// keyboard 1p&2p
			bool cancel = false;
			#if defined(APP_ENABLE_JOYSTICK) || defined(APP_ENABLE_GAME_CONTROLLER) || defined(APP_ENABLE_LINUX_GPIO)
			switch (APP_GetLastInputType()) {
			#ifdef APP_ENABLE_JOYSTICK
			case APP_INPUT_JOYSTICK:
				if (statusc[0] < 10 && APP_IsPushJoyKey(&joyKeyAssign[APP_BUTTON_B])) {
					PlaySE(5);
					statusc[0] = 0;
					statusc[2] = 0;
					cancel = true;
				}
				break;
			#endif
			#ifdef APP_ENABLE_LINUX_GPIO
			case APP_INPUT_LINUXGPIO:
				if (statusc[0] < 10 && APP_IsPushGPIO(APP_BUTTON_B)) {
					PlaySE(5);
					statusc[0] = 0;
					statusc[2] = 0;
					cancel = true;
				}
				break;
			#endif
			#ifdef APP_ENABLE_GAME_CONTROLLER
			case APP_INPUT_XBOX:
			case APP_INPUT_PLAYSTATION:
			case APP_INPUT_NINTENDO:
				if (statusc[0] < 10 && IsPushMenu(0, APP_BUTTON_B, APP_GetLastInputType())) {
					PlaySE(5);
					statusc[0] = 0;
					statusc[2] = 0;
					cancel = true;
				}
				break;
			#endif

			default:
				break;
			}
			#endif
			if (!cancel) {
				pl = statusc[2] - optionIndex + 2;
				sprintf(string[0], "KEYBOARD %" PRId32 "P SETTING", pl + 1);
				printFont(2, 3, string[0], digitc[rots[0]]);

				printMenuButton(3, 6, APP_BUTTON_UP, pl); printFont(9, 6, ":", fontc[rots[0]] * (statusc[0] == 0));
				printMenuButton(3, 7, APP_BUTTON_DOWN, pl); printFont(9, 7, ":", fontc[rots[0]] * (statusc[0] == 1));
				printMenuButton(3, 8, APP_BUTTON_LEFT, pl); printFont(9, 8, ":", fontc[rots[0]] * (statusc[0] == 2));
				printMenuButton(3, 9, APP_BUTTON_RIGHT, pl); printFont(9, 9, ":", fontc[rots[0]] * (statusc[0] == 3));
				printFont(3, 10, "A     :", fontc[rots[0]] * (statusc[0] == 4));
				printFont(3, 11, "B     :", fontc[rots[0]] * (statusc[0] == 5));
				printFont(3, 12, "C     :", fontc[rots[0]] * (statusc[0] == 6));
				printFont(3, 13, "D     :", fontc[rots[0]] * (statusc[0] == 7));
				printFont(3, 14, "GIVEUP:", fontc[rots[0]] * (statusc[0] == 8));
				printFont(3, 15, "PAUSE :", fontc[rots[0]] * (statusc[0] == 9));

				for(i = 0; i < statusc[0]; i++) {
					sprintf(string[0], "%03X", (unsigned)ncfg[10 + i + pl * 10]);
					printFont(10, 6 + i, string[0], digitc[rots[0]]);
				}

				if(statusc[0] < 10) {
					printFont(10, 6 + statusc[0], "_", digitc[rots[0]] * (count % 2));
					for(i = 0; i < APP_GetMaxKey(); i++) {
						if(i != SDL_GetScancodeFromKey(SDLK_ESCAPE, NULL) && APP_IsPushKey(i)) {
							bool unmapped = true;
							for (APP_Button button = 0; button < statusc[0]; button++) {
								if (ncfg[10 + button + pl * 10] == i) {
									unmapped = false;
									break;
								}
							}
							if (unmapped) {
								PlaySE(5);
								ncfg[10 + statusc[0] + pl * 10] = i;
								statusc[0]++;
							}
							break;
						}
					}
				} else {
					printFont(3, 17, "OK[     ] / RETRY[   ] / CANCEL[  ]", digitc[rots[0]] * (count % 2));
					printPrompt(3 + 3, 17, APP_PROMPT_OK, digitc[rots[0]] * (count % 2));
					printPrompt(3 + 18, 17, APP_PROMPT_RETRY, digitc[rots[0]] * (count % 2));
					printPrompt(3 + 32, 17, APP_PROMPT_CANCEL, digitc[rots[0]] * (count % 2));

					if(IsPushPrompt(APP_PROMPT_OK)) {
						PlaySE(10);
						for(i = 0; i < APP_BUTTON_COUNT * 2; i++) keyAssign[i] = ncfg[10 + i];
						statusc[0] = 0;
						statusc[2] = 0;
					}
					if(IsPushPrompt(APP_PROMPT_RETRY)) {
						PlaySE(5);
						for(i = 0; i < APP_BUTTON_COUNT * 2; i++) ncfg[10 + i] = keyAssign[i];
						statusc[0] = 0;
					}
					if(IsPushPrompt(APP_PROMPT_CANCEL)) {
						PlaySE(5);
						for(i = 0; i < APP_BUTTON_COUNT * 2; i++) ncfg[10 + i] = keyAssign[i];
						statusc[0] = 0;
						statusc[2] = 0;
					}
				}
			}
		}
		#endif
		#ifdef APP_ENABLE_JOYSTICK
		else if(optionIndex += 2, (statusc[2] == optionIndex - 2) || (statusc[2] == optionIndex - 1)) {
			// joystick setting
			if (APP_GetNumJoys() <= 0) {
				statusc[0] = 0;
				statusc[2] = 0;
			}
			else {
				bool cancel = false;
				#if defined(APP_ENABLE_GAME_CONTROLLER) || defined(APP_ENABLE_LINUX_GPIO) || defined(APP_ENABLE_KEYBOARD)
				switch (APP_GetLastInputType()) {
				#ifdef APP_ENABLE_GAME_CONTROLLER
				case APP_INPUT_XBOX:
				case APP_INPUT_PLAYSTATION:
				case APP_INPUT_NINTENDO:
					if (statusc[0] < 10 && IsPushMenu(0, APP_BUTTON_B, APP_GetLastInputType())) {
						PlaySE(5);
						statusc[0] = 0;
						statusc[2] = 0;
						cancel = true;
					}
					break;
				#endif
				#ifdef APP_ENABLE_LINUX_GPIO
				case APP_INPUT_LINUXGPIO:
				#endif
				#ifdef APP_ENABLE_KEYBOARD
				case APP_INPUT_KEYBOARD:
					if (statusc[0] < 10 && APP_IsPushKey(keyAssign[APP_BUTTON_B])) {
						PlaySE(5);
						statusc[0] = 0;
						statusc[2] = 0;
						cancel = true;
					}
					break;
				#endif

				default:
					break;
				}
				#endif
				if (!cancel) {
					pl = statusc[2] - optionIndex + 2;
					sprintf(string[0], "JOYSTICK %" PRId32 "P SETTING", pl + 1);
					printFont(2, 3, string[0], digitc[rots[0]]);

					printMenuButton(3, 6, APP_BUTTON_UP, pl); printFont(9, 6, ":", fontc[rots[0]] * (statusc[0] == 0));
					printMenuButton(3, 7, APP_BUTTON_DOWN, pl); printFont(9, 7, ":", fontc[rots[0]] * (statusc[0] == 1));
					printMenuButton(3, 8, APP_BUTTON_LEFT, pl); printFont(9, 8, ":", fontc[rots[0]] * (statusc[0] == 2));
					printMenuButton(3, 9, APP_BUTTON_RIGHT, pl); printFont(9, 9, ":", fontc[rots[0]] * (statusc[0] == 3));
					printFont(3, 10, "A     :", fontc[rots[0]] * (statusc[0] == 4));
					printFont(3, 11, "B     :", fontc[rots[0]] * (statusc[0] == 5));
					printFont(3, 12, "C     :", fontc[rots[0]] * (statusc[0] == 6));
					printFont(3, 13, "D     :", fontc[rots[0]] * (statusc[0] == 7));
					printFont(3, 14, "GIVEUP:", fontc[rots[0]] * (statusc[0] == 8));
					printFont(3, 15, "PAUSE :", fontc[rots[0]] * (statusc[0] == 9));

					j = 80 + pl * 10 * 8;
					for(i = 0; i < statusc[0]; i++) {
						switch((APP_JoyKeyType)ncfg[j+5+i*8]) {
						case APP_JOYKEY_AXIS:
							sprintf(string[0], "JOY%dP:A%d%c", (int)ncfg[j+0+i*8] + 1, (int)ncfg[j+6+i*8], ncfg[j+7+i*8] >= 0 ? '+' : '-');
							break;
						case APP_JOYKEY_HAT:
							sprintf(string[0], "JOY%dP:H%d,%d", (int)ncfg[j+0+i*8] + 1, (int)ncfg[j+6+i*8], (int)ncfg[j+7+i*8]);
							break;
						case APP_JOYKEY_BUTTON:
							sprintf(string[0], "JOY%dP:B%d", (int)ncfg[j+0+i*8] + 1, (int)ncfg[j+6+i*8]);
							break;
						default:
							sprintf(string[0], "???");
							break;
						}
						printFont(10, 6 + i, string[0], digitc[rots[0]]);
					}

					if(statusc[0] < 10) {
						printFont(10, 6 + statusc[0], "_", digitc[rots[0]] * (count % 2));
						APP_JoyKey pushKey;
						int pushPlayer = -1;
						do {
							if (APP_GetPlayerSlotType(pl) != APP_PLAYERSLOT_JOY) {
								break;
							}

							pushKey.player = pl;
							pushKey.guid = APP_GetJoyGUID(pl);

							pushKey.type = APP_JOYKEY_AXIS;
							for (int32_t j = 0; j < APP_GetMaxJoyAxis(pl); j++)
							{
								pushKey.setting.index = j;

								pushKey.setting.value = -APP_DEADZONE_MAX;
								if (APP_IsPushJoyKey(&pushKey))
								{
									pushPlayer = pl;
									break;
								}
								pushKey.setting.value = +APP_DEADZONE_MAX;
								if (APP_IsPushJoyKey(&pushKey))
								{
									pushPlayer = pl;
									break;
								}
							}
							if (pushPlayer >= 0) break;

							pushKey.type = APP_JOYKEY_HAT;
							for (int32_t j = 0; j < APP_GetMaxJoyHat(pl); j++)
							{
								pushKey.setting.index = j;

								pushKey.setting.value = SDL_HAT_LEFT;
								if (APP_IsPushJoyKey(&pushKey))
								{
									pushPlayer = pl;
									break;
								}
								pushKey.setting.value = SDL_HAT_RIGHT;
								if (APP_IsPushJoyKey(&pushKey))
								{
									pushPlayer = pl;
									break;
								}
								pushKey.setting.value = SDL_HAT_UP;
								if (APP_IsPushJoyKey(&pushKey))
								{
									pushPlayer = pl;
									break;
								}
								pushKey.setting.value = SDL_HAT_DOWN;
								if (APP_IsPushJoyKey(&pushKey))
								{
									pushPlayer = pl;
									break;
								}
							}
							if (pushPlayer >= 0) break;

							pushKey.type = APP_JOYKEY_BUTTON;
							for (int32_t j = 0; j < APP_GetMaxJoyButton(pl); j++)
							{
								pushKey.setting.button = j;
								if (APP_IsPushJoyKey(&pushKey))
								{
									pushPlayer = pl;
									break;
								}
							}
						} while (false);
						if(pushPlayer == pl)
						{
							bool unmapped = true;
							for (APP_Button button = 0; button < statusc[0]; button++) {
								if (
									ncfg[j+0+button*8] == pushKey.player &&
									memcmp(&ncfg[j+1+button*8], pushKey.guid.data, sizeof(int32_t) * 4) == 0 &&
									ncfg[j+5+button*8] == pushKey.type
								) {
									switch (pushKey.type) {
									case APP_JOYKEY_AXIS:
									case APP_JOYKEY_HAT:
										unmapped = (ncfg[j+6+button*8] != pushKey.setting.index || ncfg[j+7+button*8] != pushKey.setting.value);
										break;
									case APP_JOYKEY_BUTTON:
										unmapped = (ncfg[j+6+button*8] != pushKey.setting.button);
										break;
									default:
										break;
									}
								}
								if (!unmapped) break;
							}
							if (unmapped) {
								PlaySE(5);
								ncfg[j+0+statusc[0]*8] = pushKey.player;
								for (int32_t i = 0; i < 4; i++) {
									ncfg[j+1+i+statusc[0]*8] = pushKey.guid.data[i];
								}
								ncfg[j+5+statusc[0]*8] = pushKey.type;
								switch(pushKey.type) {
								case APP_JOYKEY_AXIS:
								case APP_JOYKEY_HAT:
									ncfg[j+6+statusc[0]*8] = pushKey.setting.index;
									ncfg[j+7+statusc[0]*8] = pushKey.setting.value;
									break;
								case APP_JOYKEY_BUTTON:
									ncfg[j+6+statusc[0]*8] = pushKey.setting.button;
									break;
								default:
									break;
								}
								statusc[0]++;
							}
						}
					} else {
						printFont(3, 17, "OK[     ] / RETRY[   ] / CANCEL[  ]", digitc[rots[0]] * (count % 2));
						printPrompt(3 + 3, 17, APP_PROMPT_OK, digitc[rots[0]] * (count % 2));
						printPrompt(3 + 18, 17, APP_PROMPT_RETRY, digitc[rots[0]] * (count % 2));
						printPrompt(3 + 32, 17, APP_PROMPT_CANCEL, digitc[rots[0]] * (count % 2));
						if(IsPushPrompt(APP_PROMPT_OK)) {
							PlaySE(10);
							for (APP_Button button = 0; button < APP_BUTTON_COUNT; button++) {
								APP_JoyKey *pljoy = &joyKeyAssign[pl * 10 + button];
								int32_t *plcfg = &ncfg[80 + pl * 10 * 8 + button * 8];
								pljoy->player = plcfg[0];
								for (int32_t i = 0; i < 4; i++) {
									pljoy->guid.data[i] = plcfg[1 + i];
								}
								pljoy->type = (APP_JoyKeyType)plcfg[5];
								switch (pljoy->type) {
								case APP_JOYKEY_AXIS:
								case APP_JOYKEY_HAT:
									pljoy->setting.index = plcfg[6];
									pljoy->setting.value = plcfg[7];
									break;

								case APP_JOYKEY_BUTTON:
									pljoy->setting.button = plcfg[6];
									break;
								default:
									break;
								}
							}

							statusc[0] = 0;
							statusc[2] = 0;
						}
						else if(IsPushPrompt(APP_PROMPT_RETRY)) {
							PlaySE(5);
							for (APP_Button button = 0; button < APP_BUTTON_COUNT; button++) {
								int32_t *plcfg = &ncfg[80 + pl * 10 * 8 + button * 8];
								APP_JoyKey *pljoy = &joyKeyAssign[pl * 10 + button];
								plcfg[0] = pljoy->player;
								for (int32_t i = 0; i < 4; i++) {
									plcfg[1 + i] = pljoy->guid.data[i];
								}
								plcfg[5] = pljoy->type;
								switch (pljoy->type) {
								case APP_JOYKEY_AXIS:
								case APP_JOYKEY_HAT:
									plcfg[6] = pljoy->setting.index;
									plcfg[7] = pljoy->setting.value;
									break;

								case APP_JOYKEY_BUTTON:
									plcfg[6] = pljoy->setting.button;
									break;
								default:
									break;
								}
							}
							statusc[0] = 0;
						}
						else if(IsPushPrompt(APP_PROMPT_CANCEL)) {
							PlaySE(5);
							for (APP_Button button = 0; button < 10; button++) {
								int32_t *plcfg = &ncfg[80 + pl * 10 * 8 + button * 8];
								APP_JoyKey *pljoy = &joyKeyAssign[pl * 10 + button];
								plcfg[0] = pljoy->player;
								for (int32_t i = 0; i < 4; i++) {
									plcfg[1 + i] = pljoy->guid.data[i];
								}
								plcfg[5] = pljoy->type;
								switch (pljoy->type) {
								case APP_JOYKEY_AXIS:
								case APP_JOYKEY_HAT:
									plcfg[6] = pljoy->setting.index;
									plcfg[7] = pljoy->setting.value;
									break;

								case APP_JOYKEY_BUTTON:
									plcfg[6] = pljoy->setting.button;
									break;
								default:
									break;
								}
							}
							statusc[0] = 0;
							statusc[2] = 0;
						}
					}
				}
			}
		}
		#endif
		#ifdef APP_ENABLE_GAME_CONTROLLER
		else if(optionIndex += 2, (statusc[2] == optionIndex - 2) || (statusc[2] == optionIndex - 1)) {
			// game controller setting
			if (APP_GetNumCons() <= 0) {
				statusc[0] = 0;
				statusc[2] = 0;
			}
			else {
				APP_ConKey cancelKey = {
					.type = APP_CONKEY_BUTTON,
					.index = 4
				};
				bool cancel = false;
				switch (APP_GetLastInputType()) {
				case APP_INPUT_XBOX:
				case APP_INPUT_PLAYSTATION:
				case APP_INPUT_NINTENDO:
					if (APP_IsPushConKey(-1, &cancelKey)) {
						PlaySE(5);
						statusc[0] = 0;
						statusc[2] = 0;
						cancel = true;
					}
					break;
				#ifdef APP_ENABLE_JOYSTICK
				case APP_INPUT_JOYSTICK:
					if (statusc[0] < 8 && APP_IsPushJoyKey(&joyKeyAssign[APP_BUTTON_B])) {
						PlaySE(5);
						statusc[0] = 0;
						statusc[2] = 0;
						cancel = true;
					}
					break;
				#endif
				#ifdef APP_ENABLE_LINUX_GPIO
				case APP_INPUT_LINUXGPIO:
					if (statusc[0] < 8 && APP_IsPushGPIO(APP_BUTTON_B)) {
						PlaySE(5);
						statusc[0] = 0;
						statusc[2] = 0;
						cancel = true;
					}
					break;
				#endif
				#ifdef APP_ENABLE_KEYBOARD
				case APP_INPUT_KEYBOARD:
					if (statusc[0] < 8 && APP_IsPushKey(keyAssign[APP_BUTTON_B])) {
						PlaySE(5);
						statusc[0] = 0;
						statusc[2] = 0;
						cancel = true;
					}
					break;
				#endif

				default:
					break;
				}

				if (!cancel) {
					pl = statusc[2] - optionIndex + 2;
					sprintf(string[0], "CONTROLLER %" PRId32 "P SETTING", pl + 1);
					printFont(2, 3, string[0], digitc[rots[0]]);

					printMenuButton(3, 6, APP_BUTTON_UP, pl); printFont(4, 6, ":", fontc[rots[0]] * (statusc[0] == 0));
					printMenuButton(3, 7, APP_BUTTON_DOWN, pl); printFont(4, 7, ":", fontc[rots[0]] * (statusc[0] == 1));
					printMenuButton(3, 8, APP_BUTTON_LEFT, pl); printFont(4, 8, ":", fontc[rots[0]] * (statusc[0] == 2));
					printMenuButton(3, 9, APP_BUTTON_RIGHT, pl); printFont(4, 9, ":", fontc[rots[0]] * (statusc[0] == 3));
					printFont(3, 10, "A:", fontc[rots[0]] * (statusc[0] == 4));
					printFont(3, 11, "B:", fontc[rots[0]] * (statusc[0] == 5));
					printFont(3, 12, "C:", fontc[rots[0]] * (statusc[0] == 6));
					printFont(3, 13, "D:", fontc[rots[0]] * (statusc[0] == 7));

					APP_ConKey key;
					key.type = APP_CONKEY_ANY;
					if (conPlayer == -1 && !APP_IsPressConKey(-1, &key)) {
						APP_ResetLastActiveCon();
						conPlayer = -2;
					}
					else if (conPlayer == -2 && APP_GetLastActiveCon() == pl) {
						conPlayer = pl;
						ncfg[240 + pl * (1 + 2 * 8)] = conPlayer;
					}

					if (conPlayer == pl) {
						for(i = 0; i < statusc[0]; i++) {
							APP_ConKey key = {
								.type  = ncfg[240 + pl * (1 + 2 * 8) + 1 + i * 2 + 0],
								.index = ncfg[240 + pl * (1 + 2 * 8) + 1 + i * 2 + 1]
							};
							sprintf(string[0], "%" PRId32 "P:", conPlayer + 1);
							printFont(5, 6 + i, string[0], digitc[rots[0]]);
							printConKey(5 + strlen(string[0]), 6 + i, conPlayer, &key, digitc[rots[0]]);
						}
					}
					if(statusc[0] < APP_BUTTON_GAME_COUNT) {
						printFont(5, 6 + statusc[0], "_", digitc[rots[0]] * (count % 2));
						bool pushed = false;
						APP_ConKey pushKey;

						pushKey.type = APP_CONKEY_BUTTON;
						for (pushKey.index = 0; pushKey.index < APP_CONBUTTON_MAX; pushKey.index++) {
							if (APP_IsPushConKey(conPlayer, &pushKey)) {
								pushed = true;
								break;
							}
						}

						if (!pushed) {
							pushKey.type = APP_CONKEY_AXIS;
							for (pushKey.index = 0; pushKey.index < APP_CONAXIS_MAX; pushKey.index++) {
								if (APP_IsPushConKey(conPlayer, &pushKey)) {
									pushed = true;
									break;
								}
							}
						}

						if (pushed) {
							PlaySE(5);
							ncfg[240 + pl * (1 + 2 * 8) + 1 + statusc[0] * 2 + 0] = pushKey.type;
							ncfg[240 + pl * (1 + 2 * 8) + 1 + statusc[0] * 2 + 1] = pushKey.index;
							statusc[0]++;
						}
					} else {
						printFont(3, 17, "OK[     ] / RETRY[   ] / CANCEL[  ]", digitc[rots[0]] * (count % 2));
						printPrompt(3 + 3, 17, APP_PROMPT_OK, digitc[rots[0]] * (count % 2));
						printPrompt(3 + 18, 17, APP_PROMPT_RETRY, digitc[rots[0]] * (count % 2));
						printPrompt(3 + 32, 17, APP_PROMPT_CANCEL, digitc[rots[0]] * (count % 2));
						if(IsPushPrompt(APP_PROMPT_OK)) {
							PlaySE(10);
							for(i = 0; i < 8; i++) {
								APP_ConKey* key = &conKeyAssign[pl * 8 + i];
								key->type = ncfg[240 + pl * (1 + 2 * 8) + 1 + i * 2 + 0];
								key->index = ncfg[240 + pl * (1 + 2 * 8) + 1 + i * 2 + 1];
							}
							statusc[0] = 0;
							statusc[2] = 0;
						}
						else if(IsPushPrompt(APP_PROMPT_RETRY)) {
							PlaySE(5);
							ncfg[240 + pl * (1 + 2 * 8)] = pl;
							for (i = 0; i < 8; i++) {
								APP_ConKey* key = &conKeyAssign[pl * 8 + i];
								ncfg[240 + pl * (1 + 2 * 8) + 1 + i * 2 + 0] = key->type;
								ncfg[240 + pl * (1 + 2 * 8) + 1 + i * 2 + 1] = key->index;
							}
							statusc[0] = 0;
						}
						else if(IsPushPrompt(APP_PROMPT_CANCEL)) {
							PlaySE(5);
							ncfg[240 + pl * (1 + 2 * 8)] = pl;
							for (i = 0; i < 8; i++) {
								APP_ConKey* key = &conKeyAssign[pl * 8 + i];
								ncfg[240 + pl * (1 + 2 * 8) + 1 + i * 2 + 0] = key->type;
								ncfg[240 + pl * (1 + 2 * 8) + 1 + i * 2 + 1] = key->index;
							}
							statusc[0] = 0;
							statusc[2] = 0;
						}
					}
				}
			}
		}
		#endif
		else if(statusc[2] == optionIndex++) {
			// INPUT TEST #1.60c7n3
			printFont(2,  3, "INPUT TEST", digitc[rots[0]]);
			printFont(2, 28, "EXIT[ & ]", 9);
			printMenuButton(7, 28, APP_BUTTON_RIGHT, 0);
			printMenuButton(9, 28, APP_BUTTON_A, 0);

			for(pl=0; pl<2; pl++) {
				sprintf(string[0],"%" PRId32 "P", pl + 1);
				printFont(3, 6 + pl * 10, string[0], 2 - pl);

				for(i=0; i<=7; i++) {
					if(i == 0)      { printMenuButton(3, 7 + i + pl * 10, APP_BUTTON_UP, pl); printFont(4, 7 + i + pl * 10, ":", 0); }
					else if(i == 1) { printMenuButton(3, 7 + i + pl * 10, APP_BUTTON_DOWN, pl); printFont(4, 7 + i + pl * 10, ":", 0); }
					else if(i == 2) { printMenuButton(3, 7 + i + pl * 10, APP_BUTTON_LEFT, pl); printFont(4, 7 + i + pl * 10, ":", 0); }
					else if(i == 3) { printMenuButton(3, 7 + i + pl * 10, APP_BUTTON_RIGHT, pl); printFont(4, 7 + i + pl * 10, ":", 0); }
					else if(i == 4) { printFont(3, 7 + i + pl * 10, "A:", 0); }
					else if(i == 5) { printFont(3, 7 + i + pl * 10, "B:", 0); }
					else if(i == 6) { printFont(3, 7 + i + pl * 10, "C:", 0); }
					else if(i == 7) { printFont(3, 7 + i + pl * 10, "D:", 0); }
					j =
						0
						#ifdef APP_ENABLE_KEYBOARD
						|| APP_IsPressKey(keyAssign[i + 10 * pl])
						#endif
						#ifdef APP_ENABLE_JOYSTICK
						|| APP_IsPressJoyKey(&joyKeyAssign[i + 10 * pl])	// キー入力状態取得
						#endif
						#ifdef APP_ENABLE_GAME_CONTROLLER
						|| APP_IsPressConKey(pl, &conKeyAssign[i + 8 * pl])
						#endif
						;
					if(j) sprintf(string[0],"d");
					else  sprintf(string[0],"c");
					printFont(5, 7 + i + pl * 10, string[0], j+1);
				}
			}

			if( getPressState(0, APP_BUTTON_RIGHT) && getPressState(0, APP_BUTTON_A) ) {
				PlaySE(5);
				statusc[0] = 0;
				statusc[2] = 0;
			}
		} else if(statusc[2] == optionIndex++) {
			// DISP ASSIGN #1.60c7n4
			printFont(2,  3, "DISP ASSIGN", digitc[rots[0]]);
			printMenuButton(2, 28, APP_BUTTON_A, 0);
			printMenuButton(4, 28, APP_BUTTON_B, 0);
			printFont(2, 28, " / : EXIT", 9);

			for(pl=0; pl<2; pl++) {
				string[0][0] = '\0';
				sprintf(string[0],"%" PRId32 "P", pl + 1);
				printFont(3, 6 + pl * 10, string[0], 2 - pl);

				for(i=0; i<=7; i++) {
					if(i == 0)      { printMenuButton(3, 7 + i + pl * 10, APP_BUTTON_UP, pl); printFont(4, 7 + i + pl * 10, ":", 0); }
					else if(i == 1) { printMenuButton(3, 7 + i + pl * 10, APP_BUTTON_DOWN, pl); printFont(4, 7 + i + pl * 10, ":", 0); }
					else if(i == 2) { printMenuButton(3, 7 + i + pl * 10, APP_BUTTON_LEFT, pl); printFont(4, 7 + i + pl * 10, ":", 0); }
					else if(i == 3) { printMenuButton(3, 7 + i + pl * 10, APP_BUTTON_RIGHT, pl); printFont(4, 7 + i + pl * 10, ":", 0); }
					else if(i == 4) printFont(3, 7 + i + pl * 10, "A:", 0);
					else if(i == 5) printFont(3, 7 + i + pl * 10, "B:", 0);
					else if(i == 6) printFont(3, 7 + i + pl * 10, "C:", 0);
					else if(i == 7) printFont(3, 7 + i + pl * 10, "D:", 0);

					string[0][0] = '\0';

					size_t stringPos = 0u;

					#ifdef APP_ENABLE_KEYBOARD
					if (keyAssign[i + pl * 10] != SDL_SCANCODE_UNKNOWN)
					{
						sprintf(string[1]," KB:%03X", (unsigned)keyAssign[i + pl * 10]);
						strcat(string[0], string[1]);
					}
					#endif

					#ifdef APP_ENABLE_JOYSTICK
					APP_JoyKey* const key = &joyKeyAssign[i + pl * 10];
					APP_JoyGUID getGUID = APP_GetJoyGUID(key->player);
					APP_JoyGUID zeroGUID = { 0 };
					if (
						APP_GetPlayerSlotType(key->player) == APP_PLAYERSLOT_JOY &&
						memcmp(key->guid.data, zeroGUID.data, sizeof(key->guid.data)) != 0 &&
						memcmp(getGUID.data, zeroGUID.data, sizeof(getGUID.data)) != 0 &&
						memcmp(key->guid.data, getGUID.data, sizeof(key->guid.data)) == 0
					) {
						switch(key->type) {
						case APP_JOYKEY_AXIS:
							sprintf(string[1], " JOY%dP:A%d%c",
								key->player + 1,
								key->setting.index,
								key->setting.value >= 0 ? '+' : '-'
							);
							break;
						case APP_JOYKEY_HAT:
							sprintf(string[1], " JOY%dP:H%d,%d",
								key->player + 1,
								key->setting.index,
								key->setting.value
							);
							break;
						case APP_JOYKEY_BUTTON:
							sprintf(string[1], " JOY%dP:B%d",
								key->player + 1,
								key->setting.button
							);
							break;
						default:
							string[1][0] = '\0';
							break;
						}
						if (string[1][0] != '\0') {
							strcat(string[0], string[1]);
						}

					}
					#endif
					
					#ifdef APP_ENABLE_GAME_CONTROLLER
					APP_ConKey conKey = conKeyAssign[i + pl * 8];
					if (APP_GetPlayerSlotType(pl) == APP_PLAYERSLOT_CON && (conKey.type == APP_CONKEY_AXIS || conKey.type == APP_CONKEY_BUTTON)) {
						if (string[0][0] != '\0') printFont(5, 7 + i + pl * 10, string[0], 0);
						sprintf(string[1], " CON%" PRId32 "P:", pl + 1);
						int32_t x = 5 + strlen(string[0]);
						printFont(x, 7 + i + pl * 10, string[1], 0);
						printConKey(x + strlen(string[1]), 7 + i + pl * 10, pl, &conKey, 0);
					}
					else {
						if (string[0][0] == '\0') sprintf(string[0], " NO ASSIGN");
						printFont(5, 7 + i + pl * 10, string[0], 0);
					}
					#else
					if (string[0][0] == '\0') sprintf(string[0], " NO ASSIGN");
					printFont(5, 7 + i + pl * 10, string[0], 0);
					#endif
				}
			}

			// A/B:戻る
			if(getPushState(0, APP_BUTTON_A) || getPushState(0, APP_BUTTON_B) || getPushState(1, APP_BUTTON_A) || getPushState(1, APP_BUTTON_B)) {
				PlaySE(5);
				statusc[0] = 0;
				statusc[2] = 0;
			}
		}
	} else if(status[0] == 4) {
		// video setting
		bool showScreenModeSetting = (ncfg[0] & APP_SCREENMODE_WINDOWTYPE) == APP_SCREENMODE_FULLSCREEN || (ncfg[0] & APP_SCREENMODE_WINDOWTYPE) == APP_SCREENMODE_WINDOW;
		printFont(23, 1, "- A/V SETTING", fontc[rots[0]]);
		enum {
			MENU_AV_CHANGE_MENU,
#ifdef APP_ENABLE_ALL_VIDEO_SETTINGS
			MENU_AV_WINDOW_TYPE,
			MENU_AV_SCREEN_INDEX,
#endif
			MENU_AV_DETAIL_LEVEL,
#ifdef APP_ENABLE_ALL_VIDEO_SETTINGS
			MENU_AV_VSYNC,
#endif
			MENU_AV_SCALE_MODE,
#ifdef APP_ENABLE_ALL_VIDEO_SETTINGS
			MENU_AV_RENDER_LEVEL,
			MENU_AV_SCREEN_MODE,
#endif
			MENU_AV_MOVE_SOUND,
			MENU_AV_SOUND_BUFFER,
			MENU_AV_PLAY_SOUND,
			MENU_AV_SOUND_VOLUME,
			MENU_AV_PLAY_BGM,
			MENU_AV_BGM_VOLUME,
			MENU_AV_BGM_TYPE,
			MENU_AV_BGM_FORMAT,
			MENU_AV_MAX
		};
		printFont(2, 3, "<< INPUT <<	   >> GAME P.1 >>", digitc[rots[0]] * (statusc[0] == MENU_AV_CHANGE_MENU) * (count % 2));
#ifdef APP_ENABLE_ALL_VIDEO_SETTINGS
		printFont(2, 5 + MENU_AV_WINDOW_TYPE, "WINDOW TYPE :", (statusc[0] == MENU_AV_WINDOW_TYPE) * fontc[rots[0]]);
		printFont(2, 5 + MENU_AV_SCREEN_INDEX, "SCREEN INDEX:", (statusc[0] == MENU_AV_SCREEN_INDEX) * fontc[rots[0]]);
#endif
		printFont(2, 5 + MENU_AV_DETAIL_LEVEL, "DETAIL LEVEL:", (statusc[0] == MENU_AV_DETAIL_LEVEL) * fontc[rots[0]]);
#ifdef APP_ENABLE_ALL_VIDEO_SETTINGS
		printFont(2, 5 + MENU_AV_VSYNC, "VSYNC       :", (statusc[0] == MENU_AV_VSYNC) * fontc[rots[0]]);
#endif
		printFont(2, 5 + MENU_AV_SCALE_MODE, "SCALE MODE  :", (statusc[0] == MENU_AV_SCALE_MODE) * fontc[rots[0]]);
#ifdef APP_ENABLE_ALL_VIDEO_SETTINGS
		printFont(2, 5 + MENU_AV_RENDER_LEVEL, "RENDER LEVEL:", (statusc[0] == MENU_AV_RENDER_LEVEL) * fontc[rots[0]]);
		if(showScreenModeSetting) printFont(2, 5 + MENU_AV_SCREEN_MODE, "SCREEN MODE :", (statusc[0] == MENU_AV_SCREEN_MODE) * fontc[rots[0]]);
#endif
		printFont(2, 5 + MENU_AV_MOVE_SOUND, "MOVE SOUND  :", (statusc[0] == MENU_AV_MOVE_SOUND) * fontc[rots[0]]);
		printFont(2, 5 + MENU_AV_SOUND_BUFFER, "SOUND BUFFER:", (statusc[0] == MENU_AV_SOUND_BUFFER)* fontc[rots[0]]);
		printFont(2, 5 + MENU_AV_PLAY_SOUND, "PLAY SOUND  :", (statusc[0] == MENU_AV_PLAY_SOUND) * fontc[rots[0]]);
		if ((ncfg[44] >> 23) & 0x1) printFont(2, 5 + MENU_AV_SOUND_VOLUME, "SOUND VOLUME:", (statusc[0] == MENU_AV_SOUND_VOLUME) * fontc[rots[0]]);
		printFont(2, 5 + MENU_AV_PLAY_BGM, "PLAY BGM    :", (statusc[0] == MENU_AV_PLAY_BGM) * fontc[rots[0]]);
		if((ncfg[44] >> 15) & 0x1) printFont(2, 5 + MENU_AV_BGM_VOLUME, "BGM VOLUME  :", (statusc[0] == MENU_AV_BGM_VOLUME) * fontc[rots[0]]);
		if((ncfg[44] >> 15) & 0x1) printFont(2, 5 + MENU_AV_BGM_TYPE, "BGM TYPE    :", (statusc[0] == MENU_AV_BGM_TYPE) * fontc[rots[0]]);
		if((ncfg[44] >> 15) & 0x1) printFont(2, 5 + MENU_AV_BGM_FORMAT, "BGM FORMAT  :", (statusc[0] == MENU_AV_BGM_FORMAT) * fontc[rots[0]]);

		printMenuButton(2, 28, APP_BUTTON_A, 0);
		printMenuButton(4, 28, APP_BUTTON_B, 0);
		printFont(2, 28, " / :RETURN", 9);

		switch(statusc[0]) {
		case 0:
			printFont(1, 3, "b", fontc[rots[0]]); break;
		default:
			printFont(1, 5 + statusc[0], "b", fontc[rots[0]]); break;
		}

#ifdef APP_ENABLE_ALL_VIDEO_SETTINGS
		/* 画面モード */
		switch(ncfg[0] & APP_SCREENMODE_WINDOWTYPE) {
		case APP_SCREENMODE_WINDOW: sprintf(string[0], "WINDOW"); break;
		case APP_SCREENMODE_WINDOW_MAXIMIZED: sprintf(string[0], "WINDOW MAXIMIZED"); break;
		case APP_SCREENMODE_FULLSCREEN_DESKTOP: sprintf(string[0], "FULL SCREEN DESKTOP"); break;
		case APP_SCREENMODE_FULLSCREEN: sprintf(string[0], "FULL SCREEN"); break;
		default: sprintf(string[0], "???"); break;
		}
		printFont(15, 5 + MENU_AV_WINDOW_TYPE, string[0], (statusc[0] == MENU_AV_WINDOW_TYPE) * (count % 2) * digitc[rots[0]]);
		sprintf(string[0], "%" PRId32, APP_SCREENINDEX_DISPLAY_TOVALUE(ncfg[1]));
		printFont(15, 5 + MENU_AV_SCREEN_INDEX, string[0], (statusc[0] == MENU_AV_SCREEN_INDEX) * (count % 2) * digitc[rots[0]]);
		if(ncfg[0] & APP_SCREENMODE_VSYNC) sprintf(string[0], "ON");
		else sprintf(string[0], "OFF");
		printFont(15, 5 + MENU_AV_VSYNC, string[0], (statusc[0] == MENU_AV_VSYNC) * (count % 2) * digitc[rots[0]]);
#endif
		sprintf(string[0], "%s", ncfg[0] & APP_SCREENMODE_DETAILLEVEL ? "HIGH (640X480)" : "LOW (320X240)");
		printFont(15, 5 + MENU_AV_DETAIL_LEVEL, string[0], (statusc[0] == MENU_AV_DETAIL_LEVEL) * (count % 2) * digitc[rots[0]]);

		if (ncfg[0] & APP_SCREENMODE_SCALEMODE) {
			sprintf(string[0], "INTEGER");
		}
		else {
			sprintf(string[0], "FILL SCREEN");
		}
		printFont(15, 5 + MENU_AV_SCALE_MODE, string[0], (statusc[0] == MENU_AV_SCALE_MODE) * (count % 2) * digitc[rots[0]]);

#ifdef APP_ENABLE_ALL_VIDEO_SETTINGS
		if (ncfg[0] & APP_SCREENMODE_RENDERLEVEL) {
			sprintf(string[0], "HIGH");
		}
		else {
			sprintf(string[0], "LOW (%s)", ncfg[0] & APP_SCREENMODE_DETAILLEVEL ? "640X480" : "320X240");
		}
		printFont(15, 5 + MENU_AV_RENDER_LEVEL, string[0], (statusc[0] == MENU_AV_RENDER_LEVEL) * (count % 2) * digitc[rots[0]]);

		/* 画面モード */
		switch(ncfg[0] & APP_SCREENMODE_WINDOWTYPE) {
		case APP_SCREENMODE_WINDOW:
			{
			        sprintf(string[0], "%" PRId32 "X%" PRId32,
					(!!(ncfg[0] & APP_SCREENMODE_DETAILLEVEL) + 1) * 320 * (APP_SCREENINDEX_MODE_TOVALUE(ncfg[1]) + 1),
					(!!(ncfg[0] & APP_SCREENMODE_DETAILLEVEL) + 1) * 240 * (APP_SCREENINDEX_MODE_TOVALUE(ncfg[1]) + 1)
				);
				printFont(15, 5 + MENU_AV_SCREEN_MODE, string[0], (statusc[0] == MENU_AV_SCREEN_MODE) * (count % 2) * digitc[rots[0]]);
			}
			break;
		case APP_SCREENMODE_FULLSCREEN:
			{
				SDL_DisplayMode displayMode;
				if (!APP_GetDisplayMode(APP_SCREENINDEX_DISPLAY_TOVALUE(ncfg[1]), APP_SCREENINDEX_MODE_TOVALUE(ncfg[1]), &displayMode)) {
					APP_Exit(EXIT_FAILURE);
				}
				int bpp;
				Uint32 Rmask, Gmask, Bmask, Amask;
				if(SDL_GetMasksForPixelFormat(displayMode.format, &bpp, &Rmask, &Bmask, &Gmask, &Amask))
				{
					sprintf(string[0], "%" PRId32 "X%" PRId32 " %.2fHZ %" PRId32 "BPP", displayMode.w, displayMode.h, displayMode.refresh_rate, bpp);
				}
				else
				{
					sprintf(string[0], "%" PRId32 "X%" PRId32 " %.2fHZ", displayMode.w, displayMode.h, displayMode.refresh_rate);
				}
				printFont(15, 5 + MENU_AV_SCREEN_MODE, string[0], (statusc[0] == MENU_AV_SCREEN_MODE) * (count % 2) * digitc[rots[0]]);
			}
			break;
		default:
			break;
		}
#endif

		if(ncfg[46]) sprintf(string[0], "ON");
		else sprintf(string[0], "OFF");
		printFont(15, 5 + MENU_AV_MOVE_SOUND, string[0], (statusc[0] == MENU_AV_MOVE_SOUND) * (count % 2) * digitc[rots[0]]);

		sprintf(string[0], "%d BYTES", 1024 << (int)ncfg[38]);
		printFont(15, 5 + MENU_AV_SOUND_BUFFER, string[0], (statusc[0] == MENU_AV_SOUND_BUFFER) * (count % 2) * digitc[rots[0]]);

		if((ncfg[44] >> 23) & 0x1) sprintf(string[0], "ON");
		else sprintf(string[0], "OFF");
		printFont(15, 5 + MENU_AV_PLAY_SOUND, string[0], (statusc[0] == MENU_AV_PLAY_SOUND) * (count % 2) * digitc[rots[0]]);

		if((ncfg[44] >> 23) & 0x1) {
			sprintf(string[0], "%" PRId32, (int)((ncfg[44] >> 16) & 0x7F));
			printFont(15, 5 + MENU_AV_SOUND_VOLUME, string[0], (statusc[0] == MENU_AV_SOUND_VOLUME) * (count % 2) * digitc[rots[0]]);
		}

		if((ncfg[44] >> 15) & 0x1) sprintf(string[0], "ON");
		else sprintf(string[0], "OFF");
		printFont(15, 5 + MENU_AV_PLAY_BGM, string[0], (statusc[0] == MENU_AV_PLAY_BGM) * (count % 2) * digitc[rots[0]]);

		if((ncfg[44] >> 15) & 0x1) {
			sprintf(string[0], "%" PRId32, (int)((ncfg[44] >> 8) & 0x7F));
			printFont(15, 5 + MENU_AV_BGM_VOLUME, string[0], (statusc[0] == MENU_AV_BGM_VOLUME) * (count % 2) * digitc[rots[0]]);

			int32_t wavebgm_temp = ncfg[44] & APP_SOUND_BITS_MASK;
			if(wavebgm_temp & APP_SOUND_BITS_SIMPLE) sprintf(string[0], "SIMPLE");
			else sprintf(string[0], "MULTITRACK");
			printFont(15, 5 + MENU_AV_BGM_TYPE, string[0], (statusc[0] == MENU_AV_BGM_TYPE) * (count % 2) * digitc[rots[0]]);

			switch((wavebgm_temp & APP_SOUND_BITS_FORMAT) - 1) {
				case APP_SOUND_FORMAT_MID: sprintf(string[0], "MIDI"); break;
				case APP_SOUND_FORMAT_WAV: sprintf(string[0], "WAVE"); break;
				case APP_SOUND_FORMAT_OGG: sprintf(string[0], "OGG"); break;
				case APP_SOUND_FORMAT_MP3: sprintf(string[0], "MP3"); break;
				case APP_SOUND_FORMAT_FLAC: sprintf(string[0], "FLAC"); break;
				case APP_SOUND_FORMAT_OPUS: sprintf(string[0], "OPUS"); break;
				case APP_SOUND_FORMAT_MOD: sprintf(string[0], "MOD"); break;
				case APP_SOUND_FORMAT_IT: sprintf(string[0], "IT"); break;
				case APP_SOUND_FORMAT_XM: sprintf(string[0], "XM"); break;
				case APP_SOUND_FORMAT_S3M: sprintf(string[0], "S3M"); break;
				default: sprintf(string[0], "???"); break;
			}
			printFont(15, 5 + MENU_AV_BGM_FORMAT, string[0], (statusc[0] == MENU_AV_BGM_FORMAT) * (count % 2) * digitc[rots[0]]);
		}

		for(pl = 0; pl < 2; pl++) {
			if(getPushState(pl, APP_BUTTON_A) || getPushState(pl, APP_BUTTON_B)) {
				PlaySE(3);
				status[0] = 0;
				statusc[0] = 0;
				statusc[1] = 1;
			} else if(padRepeat2(pl), ((mpc2[0] == 1) || ((mpc2[0] > tame3) && (mpc2[0] % tame4 == 0))) && (m = getPressState(pl, APP_BUTTON_DOWN) - getPressState(pl, APP_BUTTON_UP))) {
				PlaySE(5);
				int32_t nextChoice = (statusc[0] + m + MENU_AV_MAX) % MENU_AV_MAX;
				if(m > 0) {
#ifdef APP_ENABLE_ALL_VIDEO_SETTINGS
					nextChoice +=  nextChoice == MENU_AV_SCREEN_MODE && !showScreenModeSetting;
#endif
					nextChoice +=  nextChoice == MENU_AV_SOUND_VOLUME && !((ncfg[44] >> 23) & 0x1);
					nextChoice += (nextChoice == MENU_AV_BGM_VOLUME && !((ncfg[44] >> 15) & 0x1)) * 3;
				}
				else if(m < 0) {
					nextChoice -= (nextChoice == MENU_AV_BGM_FORMAT && !((ncfg[44] >> 15) & 0x1)) * 3;
					nextChoice -=  nextChoice == MENU_AV_SOUND_VOLUME && !((ncfg[44] >> 23) & 0x1);
#ifdef APP_ENABLE_ALL_VIDEO_SETTINGS
					nextChoice -=  nextChoice == MENU_AV_SCREEN_MODE && !showScreenModeSetting;
#endif
				}
				statusc[0] = (nextChoice + MENU_AV_MAX) % MENU_AV_MAX;
			} else {
				padRepeat(pl);
				m = getPushState(pl, APP_BUTTON_RIGHT) - getPushState(pl, APP_BUTTON_LEFT);
				if(m && ((statusc[0] >= MENU_AV_CHANGE_MENU && statusc[0] <= MENU_AV_PLAY_SOUND) || statusc[0] == MENU_AV_PLAY_BGM || statusc[0] == MENU_AV_BGM_TYPE || statusc[0] == MENU_AV_BGM_FORMAT)) {
					if(statusc[0] == MENU_AV_CHANGE_MENU) {
						PlaySE(3);
						status[0] = (status[0] + m + pages) % pages;
						statusc[0] = MENU_AV_CHANGE_MENU;
						statusc[1] = 1;
					}
#ifdef APP_ENABLE_ALL_VIDEO_SETTINGS
					else if(statusc[0] == MENU_AV_WINDOW_TYPE) {
						ncfg[1] &= ~APP_SCREENINDEX_MODE;
						ncfg[0] = (ncfg[0] & ~APP_SCREENMODE_WINDOWTYPE) | ((((ncfg[0] & APP_SCREENMODE_WINDOWTYPE) + APP_SCREENMODE_NUMWINDOWTYPES + m)) % APP_SCREENMODE_NUMWINDOWTYPES);
						need_reset = 1;
						if (screenMode != ncfg[0]) {
							need_setScreen = 1;
						}
					}
					else if(statusc[0] == MENU_AV_SCREEN_INDEX) {
						ncfg[1] &= ~APP_SCREENINDEX_MODE;
						ncfg[1] = (ncfg[1] & ~APP_SCREENINDEX_DISPLAY) | APP_SCREENINDEX_DISPLAY_TOSETTING((APP_SCREENINDEX_DISPLAY_TOVALUE(ncfg[1]) + APP_GetMaxDisplayIndex() + m) % APP_GetMaxDisplayIndex());	// displayIndex
						need_reset = 1;
						need_setScreen = 1;
					}
#endif
					else if(statusc[0] == MENU_AV_DETAIL_LEVEL) {
						if((ncfg[0] & APP_SCREENMODE_WINDOWTYPE) == APP_SCREENMODE_WINDOW) ncfg[1] &= ~APP_SCREENINDEX_MODE;
						ncfg[0] ^= APP_SCREENMODE_DETAILLEVEL;
						reinit = 1;
						need_reset = 1;
					}
#ifdef APP_ENABLE_ALL_VIDEO_SETTINGS
					else if(statusc[0] == MENU_AV_VSYNC) {
						ncfg[0] ^= APP_SCREENMODE_VSYNC;
						need_reset = 1;
					}
#endif
					else if(statusc[0] == MENU_AV_SCALE_MODE) {
						ncfg[0] ^= APP_SCREENMODE_SCALEMODE;
						need_reset = 1;
					}
#ifdef APP_ENABLE_ALL_VIDEO_SETTINGS
					else if(statusc[0] == MENU_AV_RENDER_LEVEL) {
						ncfg[0] ^= APP_SCREENMODE_RENDERLEVEL;
						need_reset = 1;
					}
					else if(statusc[0] == MENU_AV_SCREEN_MODE) {
						switch(ncfg[0] & APP_SCREENMODE_WINDOWTYPE) {
						case APP_SCREENMODE_WINDOW: {
							const SDL_DisplayMode* displayMode = APP_GetDesktopDisplayMode(APP_SCREENINDEX_DISPLAY_TOVALUE(ncfg[1]));
							int baseW = (!!(ncfg[0] & APP_SCREENMODE_DETAILLEVEL) + 1) * 320;
							int baseH = (!!(ncfg[0] & APP_SCREENMODE_DETAILLEVEL) + 1) * 240;
							int maxMode;
							if(displayMode->w <= baseW || displayMode->h <= baseH) {
								maxMode = 1;
							}
							else if(displayMode->w > displayMode->h) {
								maxMode = (displayMode->h / baseH) - (displayMode->h % baseH == 0);
							}
							else {
								maxMode = (displayMode->w / baseW) - (displayMode->w % baseW == 0);
							}
							int modeIndex = APP_SCREENINDEX_MODE_TOVALUE(ncfg[1]);
							modeIndex = (modeIndex + maxMode + m) % maxMode;
							ncfg[1] = (ncfg[1] & ~APP_SCREENINDEX_MODE) | APP_SCREENINDEX_MODE_TOSETTING(modeIndex);
							break;
						}
						case APP_SCREENMODE_FULLSCREEN: {
							int modeIndex = APP_SCREENINDEX_MODE_TOVALUE(ncfg[1]);
							int maxDisplayMode = APP_GetMaxDisplayMode(APP_SCREENINDEX_DISPLAY_TOVALUE(ncfg[1]));
							modeIndex = (modeIndex + maxDisplayMode + m) % maxDisplayMode;
							ncfg[1] = (ncfg[1] & ~APP_SCREENINDEX_MODE) | APP_SCREENINDEX_MODE_TOSETTING(modeIndex);
							break;
						}
						default: break;
						}
						need_reset = 1;
						need_setScreen = 1;
					}
#endif
					else if(statusc[0] == MENU_AV_MOVE_SOUND) ncfg[46] = !ncfg[46];
					else if (statusc[0] == MENU_AV_SOUND_BUFFER) {
						// soundbuffer
						if (m < 0 && ncfg[38] == 0) {
							ncfg[38] = SOUND_BUFFER_MAX - 1;
						}
						else {
							ncfg[38] = (ncfg[38] + m) % SOUND_BUFFER_MAX;
						}
						reinit = 1;
						need_reset = 1;
					}
					else if(statusc[0] == MENU_AV_PLAY_SOUND) {
						// se
						ncfg[44] ^= 0x1 << 23;
						reinit = 1;
						need_reset = 1;
					}
					else if(statusc[0] == MENU_AV_PLAY_BGM) {
						// bgm
						ncfg[44] ^= 0x1 << 15;
						reinit = 1;
						need_reset = 1;
					}
					else if(statusc[0] == MENU_AV_BGM_TYPE) {
						// wavebgm type
						ncfg[44] ^= APP_SOUND_BITS_SIMPLE;
						reinit = 1;
						need_reset = 1;
					}
					else if(statusc[0] == MENU_AV_BGM_FORMAT) {
						// wavebgm format
						APP_SoundFormat wavebgm_format = (ncfg[44] & APP_SOUND_BITS_FORMAT) - 1;
						do {
							wavebgm_format += m;
							if (m < 0 && wavebgm_format < 0) {
								wavebgm_format = APP_SOUND_FORMAT_COUNT - 1;
							}
							else if (m > 0 && wavebgm_format > APP_SOUND_FORMAT_COUNT - 1) {
								wavebgm_format = 0;
							}
						} while (!APP_IsSoundFormatSupported(wavebgm_format));

						ncfg[44] = (ncfg[44] & ~APP_SOUND_BITS_FORMAT) | (wavebgm_format + 1);
						reinit = 1;
						need_reset = 1;
					}
				}
				else if((m || (mp[pl] && mpc[pl] > 30)) && (statusc[0] == MENU_AV_SOUND_VOLUME || statusc[0] == MENU_AV_BGM_VOLUME)) {
					if(statusc[0] == MENU_AV_SOUND_VOLUME) {
						// sevolume
						int32_t sevolume_temp = (ncfg[44] >> 16) & 0x7F;
						if(m) sevolume_temp += m;
						else if(mp[pl] && mpc[pl] > 30) sevolume_temp += mp[pl] * 2 - 5;
						
						if(sevolume_temp < 0) sevolume_temp = 0;
						if(sevolume_temp > 100) sevolume_temp = 100;
						
						ncfg[44] = (ncfg[44] & ~(0x7F << 16)) | ((sevolume_temp & 0x7F) << 16);
						SetVolumeAllWaves(sevolume_temp);
					}
					else if(statusc[0] == MENU_AV_BGM_VOLUME) {
						// bgmvolume
						int32_t bgmvolume_temp = (ncfg[44] >> 8) & 0x7F;
						if(m) bgmvolume_temp += m;
						else if(mp[pl] && mpc[pl] > 30) bgmvolume_temp += mp[pl] * 2 - 5;

						if(bgmvolume_temp < 0) bgmvolume_temp = 0;
						if(bgmvolume_temp > 100) bgmvolume_temp = 100;

						ncfg[44] = (ncfg[44] & ~(0x7F << 8)) | ((bgmvolume_temp & 0x7F) << 8);
						SetVolumeAllBGM(bgmvolume_temp);
						APP_SetMusicVolume(bgmvolume_temp);
					}
				}
			}
		}
	}

	if(restart) {
		mainLoopState = MAIN_RESTART;
		return;
	}
	// exit setting menu
	else if(status[0] == -1) {
		mainLoopState = MAIN_TITLE;
		init = true;
	}
}
