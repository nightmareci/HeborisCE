#include "gamedef.h"

typedef struct {
	SDL_Scancode keyAssign[10 * 2];
	int32_t	fontc[12];
	int32_t	digitc[12];
	SDL_Scancode	giveupKey;
	SDL_Scancode	ssKey;
	SDL_Scancode	pausekey[2];
	SDL_Scancode	dispnextkey[2];
	int32_t	dtc;
	int32_t	fldtr;
	int32_t	dispnext;
	int32_t	movesound;
	int32_t	wavebgm;
	int32_t	maxPlay;

	int32_t	breakeffect;
	int32_t	showcombo;
	int32_t	top_frame;

	int32_t	w_reverse;

	int32_t	downtype;

	int32_t	lvupbonus;

	int32_t	fontsize;

	JoyKey	joykeyAssign[10 * 2];

	SDL_Scancode	holdkey[2];

	int32_t rots[2];
	int32_t lvup[2];

	int32_t		screenMode;
	int32_t		displayIndex;
	int32_t		nextblock;
	int32_t		smooth;
	int32_t		nanameallow;
	int32_t		sonicdrop;
	int32_t		blockflash;
	int32_t		fastlrmove;
	int32_t		background;
} settings;

static const settings defsettings = {
	.keyAssign =
	{
		SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT,
		SDL_SCANCODE_Z, SDL_SCANCODE_X, SDL_SCANCODE_C, SDL_SCANCODE_V,
		SDL_SCANCODE_Q, SDL_SCANCODE_W,

		SDL_SCANCODE_KP_8, SDL_SCANCODE_KP_5, SDL_SCANCODE_KP_4, SDL_SCANCODE_KP_6,
		SDL_SCANCODE_KP_1, SDL_SCANCODE_KP_2, SDL_SCANCODE_KP_3, SDL_SCANCODE_KP_0,
		SDL_SCANCODE_PAGEUP, SDL_SCANCODE_PAGEDOWN,
	},
	.fontc = {9,1,2,3,8,4,3,6,7},	//題字の色	0:白 1:青 2:赤 3:桃 4:緑 5:黄 6:空 7:橙 8:紫 9:藍
	.digitc = {5,5,7,7,5,5,7,7,5},	//数字の色	それぞれ、TGMRule・TiRule・WorldRule・World2Rule
	.giveupKey = SDL_SCANCODE_Q,		//捨てゲーキー (デフォルトはQ)
	.ssKey = SDL_SCANCODE_HOME,		//スナップショットキー (デフォルトはHome)
	.pausekey = { SDL_SCANCODE_F1,SDL_SCANCODE_F2 },	//ポーズキー(デフォルトはF1,F2)		#1.60c7g7
	.dispnextkey = { SDL_SCANCODE_F3,SDL_SCANCODE_F4 },	//NEXT表示キー(デフォルトはF3,F4)	#1.60c7g7
	.dtc = 1,				//tgmlvの表示	0:off  1:on  (lvtype = 1の時は常に表示)
	.fldtr = 96,				//フィールド背景非表示時のフィールド透過度(0-256)
	.dispnext = 3,			//ネクストブロック表示数の選択（０〜３）
	.movesound = 1,			//ブロック移動音の選択	0:OFF 1:ON
	.wavebgm = 0,			//BGMの選択
	.maxPlay = 0,			//プレイヤー人数の選択	0:シングル 1:デュアル

	.breakeffect = 1,	//ラインをそろえたとき、ブロックを弾けさせるか 0:off 1:on
	.showcombo = 0,		//コンボの表示(SINGLEとかHEBORISとか) 0:off 1:on
	.top_frame = 0,		//ブロックの高速消去 0:ブロックを左から右へ消す 1:同時に消す

	.w_reverse = 1,		//ワールドルールで回転方法を逆転させる 0:off 1:on #1.60c7f8

	.downtype = 1,		//下入れタイプ 0:HEBORIS 1:Ti #1.60c7f9

	.lvupbonus = 0,		//レベルアップボーナス 0:TI 1:TGM/TAP #1.60c7g3

	.fontsize = 1,			//フォントサイズ 0:DEFAULT 1:SMALL 宣言し忘れ修正#1.60c6.1a

	.joykeyAssign = { 0 },		//ジョイスティックボタン割り当て

	//Holdボタン(キーボード)割り当て
	.holdkey = { SDL_SCANCODE_V, SDL_SCANCODE_KP_0 },	//default 1p側:V 2p側:テンキー0

	.rots = {2, 1},
	.lvup = {1, 1},

	.screenMode =SCREEN_WINDOW | SCREEN_DETAIL_MASK,
	.displayIndex =0,
	.nextblock =8,
	.smooth =0,
	.nanameallow =1,
	.sonicdrop =0,
	.blockflash =3,
	.fastlrmove =1,
	.background =2,
};


int32_t readdef()
{
	int32_t i,j, cfgbuf[CFG_LENGTH];

	keyAssign[7] = defsettings.holdkey[0];
	keyAssign[17] = defsettings.holdkey[1];

	FillMemory(&cfgbuf, sizeof(cfgbuf), 0);
	cfgbuf[0] = 0x4F424550;
	cfgbuf[1] = 0x20534953;
	cfgbuf[2] = 0x464E4F44;
	cfgbuf[3] = 0x31764750;

	cfgbuf[4] = defsettings.screenMode;
	cfgbuf[5] = defsettings.displayIndex;
	cfgbuf[6] = defsettings.nextblock;
	//cfgbuf[7] = defsettings.blockkind;
	cfgbuf[8] = defsettings.smooth;
	cfgbuf[9] = defsettings.nanameallow;
	cfgbuf[10] = defsettings.sonicdrop;
	cfgbuf[11] = defsettings.blockflash;
	cfgbuf[12] = defsettings.fastlrmove;
	cfgbuf[13] = defsettings.background;

	for (i = 0; i < 20; i++) {
		cfgbuf[14 + i] = defsettings.keyAssign[i];
	}
	cfgbuf[38] = defsettings.giveupKey;
	cfgbuf[39] = defsettings.ssKey;

	cfgbuf[40] = defsettings.rots[0];
	cfgbuf[41] = defsettings.rots[1];
	//cfgbuf[42] = defsettings.lvup[0];
	//cfgbuf[43] = defsettings.lvup[1];
	cfgbuf[44] = defsettings.dtc;
	cfgbuf[45] = defsettings.dispnext;
	cfgbuf[53] = defsettings.fldtr;
	cfgbuf[54] = defsettings.fontsize;
	cfgbuf[55] = defsettings.maxPlay;
	cfgbuf[60] = defsettings.movesound;
	cfgbuf[61] = defsettings.wavebgm;
	cfgbuf[62] = defsettings.breakeffect;
	cfgbuf[63] = defsettings.showcombo;
	cfgbuf[64] = defsettings.top_frame;
	cfgbuf[65] = defsettings.w_reverse;
	cfgbuf[66] = defsettings.downtype;
	cfgbuf[67] = defsettings.lvupbonus;
	cfgbuf[68] = defsettings.pausekey[0];
	cfgbuf[69] = defsettings.pausekey[1];
	cfgbuf[70] = defsettings.dispnextkey[0];
	cfgbuf[71] = defsettings.dispnextkey[1];


	cfgbuf[74] = defsettings.fontc[0] + defsettings.fontc[1] * 0x100 + defsettings.fontc[2] * 0x10000 + defsettings.fontc[3] * 0x1000000;
	cfgbuf[75] = defsettings.digitc[0] + defsettings.digitc[1] * 0x100 + defsettings.digitc[2] * 0x10000 + defsettings.digitc[3] * 0x1000000;
	cfgbuf[76] = defsettings.fontc[4] + defsettings.fontc[5] * 0x100 + defsettings.fontc[6] * 0x10000 + defsettings.fontc[7] * 0x1000000 ;
	cfgbuf[77] = defsettings.digitc[4] + defsettings.digitc[5] * 0x100 + defsettings.digitc[6] * 0x10000 + defsettings.digitc[7] * 0x1000000 ;
	cfgbuf[78] = defsettings.fontc[8] + defsettings.fontc[9] * 0x100 + defsettings.fontc[10] * 0x10000 + defsettings.fontc[11] * 0x1000000 ;
	cfgbuf[79] = defsettings.digitc[8] + defsettings.digitc[9] * 0x100 + defsettings.digitc[10] * 0x10000 + defsettings.digitc[11] * 0x1000000 ;

	memset(cfgbuf + 80, 0, 8 * 10 * 2 * sizeof(int32_t));

	SaveFile("config/data/CONFIG.SAV", &cfgbuf, sizeof(cfgbuf));

	return (0);
}