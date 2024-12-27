#include "script/include.h"

typedef struct {
	SDL_Scancode keyAssign[10 * 2];
	int32_t	fontc[12];
	int32_t	digitc[12];

	SDL_Scancode	pausekey[2];
	SDL_Scancode	dispnextkey[2];
	int32_t	dtc;
	int32_t	fldtr;
	int32_t	dispnext;
	int32_t	movesound;
	int32_t se;
	int32_t sevolume;
	int32_t bgm;
	int32_t bgmvolume;
	int32_t	wavebgm;
	int32_t	maxPlay;

	int32_t	breakeffect;
	int32_t	showcombo;
	int32_t	top_frame;

	int32_t	w_reverse;

	int32_t	downtype;

	int32_t	lvupbonus;

	int32_t	fontsize;

	#ifdef ENABLE_JOYSTICK
	YGS2kSJoyKey	joyKeyAssign[10 * 2];
	#endif
	
	#ifdef ENABLE_GAME_CONTROLLER
	int32_t playerCons[2];
	YGS2kSConKey conKeyAssign[8 * 2];
	#endif

	int32_t fourwayfilter;
	int32_t fourwaypriorityup;
	
	int32_t rots[2];
	int32_t segacheat;

	YGS2kEScreenModeFlag	screenMode;
	int32_t		screenIndex;
	int32_t		soundbuffer;
	int32_t		nextblock;
	uint32_t	cfgversion;
	int32_t		smooth;
	int32_t		nanameallow;
	int32_t		sonicdrop;
	int32_t		blockflash;
	int32_t		fastlrmove;
	int32_t		background;
	int32_t		spawn_y_type;
	int32_t		hide_tomoyoinf;
	int32_t		tspin_type;
	int32_t		block_spectrum;
	int32_t		next_adjust;
	int32_t		Isbesttime;
	int32_t		b2bcheck;
	int32_t		debug;
	int32_t		enable_grade[2];
	int32_t		ace_irs;
	int32_t		english;
	int32_t		mini_select;
	int32_t		bigtype;
	int32_t		item_interval;
	int32_t		hide_wait;
	int32_t		vs_time;
	int32_t		medaltype;
	int32_t		p_shirase_start_level;
	int32_t		uplinetype;
	int32_t		disable_hold;
	int32_t		disable_irs;
	int32_t		cp_player_1p;
	int32_t		cp_player_2p;
	int32_t		cp_type;
	int32_t		block_rframe;
	int32_t		disable_wallkick;
	int32_t		showctrl;
} SConfig;

static const SConfig DefaultConfig = {
	#ifdef ENABLE_KEYBOARD
	.keyAssign =
	{
		SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT,
		SDL_SCANCODE_Z, SDL_SCANCODE_X, SDL_SCANCODE_C, SDL_SCANCODE_V,
		SDL_SCANCODE_Q, SDL_SCANCODE_W,

		SDL_SCANCODE_KP_8, SDL_SCANCODE_KP_5, SDL_SCANCODE_KP_4, SDL_SCANCODE_KP_6,
		SDL_SCANCODE_KP_1, SDL_SCANCODE_KP_2, SDL_SCANCODE_KP_3, SDL_SCANCODE_KP_0,
		SDL_SCANCODE_PAGEUP, SDL_SCANCODE_PAGEDOWN,
	},
	#endif
	.fontc = {9,1,2,3,8,4,3,6,7},	//題字の色	0:白 1:青 2:赤 3:桃 4:緑 5:黄 6:空 7:橙 8:紫 9:藍
	.digitc = {5,5,7,7,5,5,7,7,5},	//数字の色	それぞれ、TGMRule・TiRule・WorldRule・World2Rule
	.pausekey = { SDL_SCANCODE_F1,SDL_SCANCODE_F2 },	//ポーズキー(デフォルトはF1,F2)		#1.60c7g7
	.dispnextkey = { SDL_SCANCODE_F3,SDL_SCANCODE_F4 },	//NEXT表示キー(デフォルトはF3,F4)	#1.60c7g7
	.dtc = 1,				//tgmlvの表示	0:off  1:on  (lvtype = 1の時は常に表示)
	.fldtr = 96,				//フィールド背景非表示時のフィールド透過度(0-256)
	.dispnext = 3,			//ネクストブロック表示数の選択（０〜３）
	.movesound = 1,			//ブロック移動音の選択	0:OFF 1:ON
	.se = 1,
	.sevolume = 100,
	.bgm = 1,
	.bgmvolume = 100,
	.wavebgm = YGS_WAVE_SIMPLE | YGS_WAVE_OGG,	//BGMの選択
	.maxPlay = 0,			//プレイヤー人数の選択	0:シングル 1:デュアル

	.breakeffect = 1,	//ラインをそろえたとき、ブロックを弾けさせるか 0:off 1:on
	.showcombo = 0,		//コンボの表示(SINGLEとかHEBORISとか) 0:off 1:on
	.top_frame = 0,		//ブロックの高速消去 0:ブロックを左から右へ消す 1:同時に消す

	.w_reverse = 1,		//ワールドルールで回転方法を逆転させる 0:off 1:on #1.60c7f8

	.downtype = 1,		//下入れタイプ 0:HEBORIS 1:Ti #1.60c7f9

	.lvupbonus = 0,		//レベルアップボーナス 0:TI 1:TGM/TAP #1.60c7g3

	.fontsize = 1,			//フォントサイズ 0:DEFAULT 1:SMALL 宣言し忘れ修正#1.60c6.1a

	#ifdef ENABLE_JOYSTICK
	.joyKeyAssign = DEFAULT_JOYKEY_ASSIGN,		//ジョイスティックボタン割り当て
	#endif
	
	#ifdef ENABLE_GAME_CONTROLLER
	.playerCons = { 0, 1 },
	.conKeyAssign = {
		// Player 1
		{
			.type = YGS_CONKEY_BUTTON,
			.index = SDL_CONTROLLER_BUTTON_DPAD_UP
		},
		{
			.type = YGS_CONKEY_BUTTON,
			.index = SDL_CONTROLLER_BUTTON_DPAD_DOWN
		},
		{
			.type = YGS_CONKEY_BUTTON,
			.index = SDL_CONTROLLER_BUTTON_DPAD_LEFT
		},
		{
			.type = YGS_CONKEY_BUTTON,
			.index = SDL_CONTROLLER_BUTTON_DPAD_RIGHT
		},
		{
			.type = YGS_CONKEY_BUTTON,
			.index = SDL_CONTROLLER_BUTTON_A
		},
		{
			.type = YGS_CONKEY_BUTTON,
			.index = SDL_CONTROLLER_BUTTON_B
		},
		{
			.type = YGS_CONKEY_BUTTON,
			.index = SDL_CONTROLLER_BUTTON_X
		},
		{
			.type = YGS_CONKEY_BUTTON,
			.index = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
		},

		// Player 2
		{
			.type = YGS_CONKEY_BUTTON,
			.index = SDL_CONTROLLER_BUTTON_DPAD_UP
		},
		{
			.type = YGS_CONKEY_BUTTON,
			.index = SDL_CONTROLLER_BUTTON_DPAD_DOWN
		},
		{
			.type = YGS_CONKEY_BUTTON,
			.index = SDL_CONTROLLER_BUTTON_DPAD_LEFT
		},
		{
			.type = YGS_CONKEY_BUTTON,
			.index = SDL_CONTROLLER_BUTTON_DPAD_RIGHT
		},
		{
			.type = YGS_CONKEY_BUTTON,
			.index = SDL_CONTROLLER_BUTTON_A
		},
		{
			.type = YGS_CONKEY_BUTTON,
			.index = SDL_CONTROLLER_BUTTON_B
		},
		{
			.type = YGS_CONKEY_BUTTON,
			.index = SDL_CONTROLLER_BUTTON_X
		},
		{
			.type = YGS_CONKEY_BUTTON,
			.index = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
		}
	},
	#endif

	.fourwayfilter = 0,
	.fourwaypriorityup = 0,
	
	.rots = {2, 1},
	.segacheat = 0,

	.screenMode =DEFAULT_SCREEN_MODE,
	.screenIndex =0,
	.soundbuffer =0,
	.nextblock =8,
	.cfgversion =CFG_VERSION,
	.smooth =0,
	.nanameallow =1,
	.sonicdrop =0,
	.blockflash =3,
	.fastlrmove =1,
	.background =2,

	// ブロックの出現位置
	// 0=枠の下 1=枠の上
	.spawn_y_type = 0,

	// TOMOYO詳細情報表示の有無#C7T9.6EX (0=あり 1=なし)
	.hide_tomoyoinf = 1,

	// T-SPINの判定方法
	// 0=一切チェックしない
	// 1=接地した状態で回転させて左右に動かさずにラインを消す(Back to Back は途切れる)
	// 2=四隅のうち、3つ以上がブロックか壁で埋まっている(Back to Back 可能)
	// 3=1と2の両方を判定する(Back to Back は3コーナーのみ)
	.tspin_type = 2,

	//2G以上または即落下使用時にブロックの残像を見せる（0=なし　1=あり）
	.block_spectrum = 1,

	// ゲーム開始直後のブロックに黄色、紫、緑が出現#1.60c7f5
	// 0=する 1=しない
	.next_adjust = 1,

	//それぞれのモードのベストタイムを見せる #C7U2.5EX
	//0=off 1=on
	.Isbesttime = 1,

	// BACK to BACKの有無
	// 0=なし
	// 1=あり　連続でヘボリスまたは3コーナーT-SPIN消しを行うと得点が1.5倍
	.b2bcheck = 1,

	// デバッグフラグ。いろいろ画面に情報を出します。
	// また、SOLO MODEで開始レベルを選べるようになります。
	// 0=無効 1=有効
	.debug = 0,

	// 段位認定の有無#1.60c7i1　#C7T5EX
	// 0=なし 1=あり(1) 2=あり(2) 3=あり(2-20) 4=あり(3)
	.enable_grade = { 4, 1 },

	// IRSの種類
	// 0=クラシック（ヘボリス）　1=ACE　　2=ACE-カスタム
	// 0以外はブロック出現までの間に何度でもブロックを回転出来ます。
	// ACE-カスタムはIHS時に向きをリセットしないのと、
	// HOLD（非IHS）時に通常のIRSが使用可能な点でACEと異なります。
	.ace_irs = 0,

	// English mode (Hide japanese text in game)
	// 0=OFF, 1=ON
	.english = 1,

	// 1にするとSOLO MODEでOTHERS画面が出てこなくなります
	// 0=使用しない 1=使用する
	.mini_select = 1,

	// BIGモードの横移動単位 #1.60c7m6
	// 0=1マス 1=2マス 2=モードによって変える
	.bigtype = 2,

	// VERSUSモードのアイテムの出現間隔デフォルト値
	// ATTACKは2倍、ITEMは半分になる
	.item_interval = 20,

	// WAIT表示の有無#1.60c7e (0=あり 1=なし)
	.hide_wait = 1,

	// VERSUSモードの制限時間
	// 0=無制限
	.vs_time = 180 * 60,

	// メダルの絵の種類(0〜1）2にすると非表示になります #1.60c7m9
	.medaltype = 0,

	// せり上がり開始レベル#1.60c6.2g(デフォルトは500)
	.p_shirase_start_level = 500,

	//せり上がりタイプ(0=COPY(デフォルト),1=PATTERN)
	.uplinetype = 0,

	// HOLDの有無
	// 0=あり 1=なし
	.disable_hold = 0,

	// IRS（先行回転）の有無
	// 0=あり 1=なし
	.disable_irs = 0,

	// 1にすると人間の代わりにコンピュータが操作するようになります。
	.cp_player_1p = 0,	// 1P
	.cp_player_2p = 1,	// 2P

	// コンピュータの性格
	// 0=ひたすらライン消し 1=2ライン以上消えるときだけ消す
	// 0は一人用向き、1は対戦向きです。
	.cp_type = 1,

	// ブロックの周りに白い四角形を表示
	// 0=なし 1=あり
	.block_rframe = 0,

	// 壁蹴りの有無
	// 0=あり 1=なし
	.disable_wallkick = 0,

	// 操作状況の表示#1.60c7f6 (0=リプレイのみ 1=常に表示 2=常に非表示)
	.showctrl = 0
};


void SetDefaultKeyboardConfig(SDL_Scancode* keys)
{
	for (int32_t i = 0; i < 20; i++) {
		keys[i] = DefaultConfig.keyAssign[i];
	}
}

void SetDefaultConfig()
{
	int32_t i,j, cfgbuf[CFG_LENGTH];

	YGS2kFillMemory(cfgbuf, sizeof(cfgbuf), 0);
	cfgbuf[0] = 0x4F424550;
	cfgbuf[1] = 0x20534953;
	cfgbuf[2] = 0x464E4F44;
	cfgbuf[3] = 0x31764750;

	cfgbuf[4] = DefaultConfig.screenMode;
	cfgbuf[5] = DefaultConfig.screenIndex;
	cfgbuf[6] = DefaultConfig.nextblock;
	cfgbuf[7] = DefaultConfig.cfgversion;
	cfgbuf[8] = DefaultConfig.smooth;
	cfgbuf[9] = DefaultConfig.nanameallow;
	cfgbuf[10] = DefaultConfig.sonicdrop;
	cfgbuf[11] = DefaultConfig.blockflash;
	cfgbuf[12] = DefaultConfig.fastlrmove;
	cfgbuf[13] = DefaultConfig.background;

	for (i = 0; i < 20; i++) {
		cfgbuf[14 + i] = DefaultConfig.keyAssign[i];
	}

	cfgbuf[35] = DefaultConfig.fourwayfilter;
	cfgbuf[37] = DefaultConfig.fourwaypriorityup;
	cfgbuf[38] = DefaultConfig.soundbuffer;

	cfgbuf[40] = DefaultConfig.rots[0];
	cfgbuf[41] = DefaultConfig.rots[1];
	cfgbuf[42] = DefaultConfig.segacheat;
	cfgbuf[44] = DefaultConfig.dtc;
	cfgbuf[45] = DefaultConfig.dispnext;
	cfgbuf[53] = DefaultConfig.fldtr;
	cfgbuf[54] = DefaultConfig.fontsize;
	cfgbuf[55] = DefaultConfig.maxPlay;
	cfgbuf[60] = DefaultConfig.movesound;
	cfgbuf[61] =
		(( DefaultConfig.se & 0x1) << 23) | (( DefaultConfig.sevolume & 0x7F) << 16) |
		((DefaultConfig.bgm & 0x1) << 15) | ((DefaultConfig.bgmvolume & 0x7F) <<  8) |
		(DefaultConfig.wavebgm & YGS_WAVE_MASK);
	cfgbuf[62] = DefaultConfig.breakeffect;
	cfgbuf[63] = DefaultConfig.showcombo;
	cfgbuf[64] = DefaultConfig.top_frame;
	cfgbuf[65] = DefaultConfig.w_reverse;
	cfgbuf[66] = DefaultConfig.downtype;
	cfgbuf[67] = DefaultConfig.lvupbonus;
	cfgbuf[68] = DefaultConfig.pausekey[0];
	cfgbuf[69] = DefaultConfig.pausekey[1];
	cfgbuf[70] = DefaultConfig.dispnextkey[0];
	cfgbuf[71] = DefaultConfig.dispnextkey[1];


	cfgbuf[74] = DefaultConfig.fontc[0] + DefaultConfig.fontc[1] * 0x100 + DefaultConfig.fontc[2] * 0x10000 + DefaultConfig.fontc[3] * 0x1000000;
	cfgbuf[75] = DefaultConfig.digitc[0] + DefaultConfig.digitc[1] * 0x100 + DefaultConfig.digitc[2] * 0x10000 + DefaultConfig.digitc[3] * 0x1000000;
	cfgbuf[76] = DefaultConfig.fontc[4] + DefaultConfig.fontc[5] * 0x100 + DefaultConfig.fontc[6] * 0x10000 + DefaultConfig.fontc[7] * 0x1000000 ;
	cfgbuf[77] = DefaultConfig.digitc[4] + DefaultConfig.digitc[5] * 0x100 + DefaultConfig.digitc[6] * 0x10000 + DefaultConfig.digitc[7] * 0x1000000 ;
	cfgbuf[78] = DefaultConfig.fontc[8] + DefaultConfig.fontc[9] * 0x100 + DefaultConfig.fontc[10] * 0x10000 + DefaultConfig.fontc[11] * 0x1000000 ;
	cfgbuf[79] = DefaultConfig.digitc[8] + DefaultConfig.digitc[9] * 0x100 + DefaultConfig.digitc[10] * 0x10000 + DefaultConfig.digitc[11] * 0x1000000 ;

	#ifdef ENABLE_JOYSTICK
	int32_t *joykeybuf = &cfgbuf[80];
	for (int32_t pl = 0; pl < 2; pl++) {
		for (int32_t key = 0; key < 10; key++) {
			int32_t *plbuf = &joykeybuf[pl * 10 * 8 + key * 8];
			const YGS2kSJoyKey *pljoy = &DefaultConfig.joyKeyAssign[pl * 10 + key];
			plbuf[0] = pljoy->player;
			for (int32_t i = 0; i < 4; i++) {
				plbuf[1 + i] = pljoy->guid.data[i];
			}
			plbuf[5] = pljoy->type;
			switch (pljoy->type) {
			case YGS_JOYKEY_AXIS:
			case YGS_JOYKEY_HAT:
				plbuf[6] = pljoy->setting.index;
				plbuf[7] = pljoy->setting.value;
				break;

			case YGS_JOYKEY_BUTTON:
				plbuf[6] = pljoy->setting.button;
				break;
			default:
				break;
			}
		}
	}
	#endif
	
	#ifdef ENABLE_GAME_CONTROLLER
	int32_t *conkeybuf = &cfgbuf[240];
	for (int32_t pl = 0; pl < 2; pl++) {
		conkeybuf[pl * (1 + 2 * 8)] = DefaultConfig.playerCons[pl];
		int32_t *plbuf = &conkeybuf[pl * (1 + 2 * 8) + 1];
		for (int32_t key = 0; key < 8; key++) {
			const YGS2kSConKey *plcon = &DefaultConfig.conKeyAssign[pl * 8 + key];
			plbuf[key * 2 + 0] = plcon->type;
			plbuf[key * 2 + 1] = plcon->index;
		}
	}
	#endif

	cfgbuf[274] = DefaultConfig.spawn_y_type;
	cfgbuf[275] = DefaultConfig.hide_tomoyoinf;
	cfgbuf[276] = DefaultConfig.tspin_type;
	cfgbuf[277] = DefaultConfig.block_spectrum;
	cfgbuf[278] = DefaultConfig.next_adjust;
	cfgbuf[279] = DefaultConfig.Isbesttime;
	cfgbuf[280] = DefaultConfig.b2bcheck;
	cfgbuf[281] = DefaultConfig.debug;
	cfgbuf[282] = DefaultConfig.enable_grade[0];
	cfgbuf[283] = DefaultConfig.enable_grade[1];
	cfgbuf[284] = DefaultConfig.ace_irs;
	cfgbuf[285] = DefaultConfig.english;
	cfgbuf[286] = DefaultConfig.mini_select;
	cfgbuf[287] = DefaultConfig.bigtype;
	cfgbuf[288] = DefaultConfig.item_interval;
	cfgbuf[289] = DefaultConfig.hide_wait;
	cfgbuf[290] = DefaultConfig.vs_time;
	cfgbuf[291] = DefaultConfig.medaltype;
	cfgbuf[292] = DefaultConfig.p_shirase_start_level;
	cfgbuf[293] = DefaultConfig.uplinetype;
	cfgbuf[294] = DefaultConfig.disable_hold;
	cfgbuf[295] = DefaultConfig.disable_irs;
	cfgbuf[296] = DefaultConfig.cp_player_1p;
	cfgbuf[297] = DefaultConfig.cp_player_2p;
	cfgbuf[298] = DefaultConfig.cp_type;
	cfgbuf[299] = DefaultConfig.block_rframe;
	cfgbuf[300] = DefaultConfig.disable_wallkick;
	cfgbuf[301] = DefaultConfig.showctrl;

	cfgbuf[34] = ConfigChecksum(cfgbuf);

	YGS2kSaveFile("config/data/CONFIG.SAV", cfgbuf, sizeof(cfgbuf));
}
