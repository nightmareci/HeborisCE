#include "../script/include.h"

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

	SJoyKey	joykeyAssign[10 * 2];

	SDL_Scancode	holdkey[2];

	int32_t rots[2];
	int32_t lvup[2];

	int32_t		screenMode;
	int32_t		screenIndex;
	int32_t		nextblock;
	uint32_t	cfgversion;
	int32_t		smooth;
	int32_t		nanameallow;
	int32_t		sonicdrop;
	int32_t		blockflash;
	int32_t		fastlrmove;
	int32_t		background;
} SConfig;

static const SConfig DefaultConfig = {
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
	.se = 1,
	.sevolume = 100,
	.bgm = 0,
	.bgmvolume = 100,
	.wavebgm = 0,			//BGMの選択
	.maxPlay = 0,			//プレイヤー人数の選択	0:シングル 1:デュアル

	.breakeffect = 1,	//ラインをそろえたとき、ブロックを弾けさせるか 0:off 1:on
	.showcombo = 0,		//コンボの表示(SINGLEとかHEBORISとか) 0:off 1:on
	.top_frame = 0,		//ブロックの高速消去 0:ブロックを左から右へ消す 1:同時に消す

	.w_reverse = 1,		//ワールドルールで回転方法を逆転させる 0:off 1:on #1.60c7f8

	.downtype = 1,		//下入れタイプ 0:HEBORIS 1:Ti #1.60c7f9

	.lvupbonus = 0,		//レベルアップボーナス 0:TI 1:TGM/TAP #1.60c7g3

	.fontsize = 1,			//フォントサイズ 0:DEFAULT 1:SMALL 宣言し忘れ修正#1.60c6.1a

	.joykeyAssign = {
	    {
	        .device = 0,
	        .guid = 0,
	        .type = JOYKEY_BUTTON,
	        .setting = {
	            .button = 8
	        }
	    },
	    {
	        .device = 0,
	        .guid = 0,
	        .type = JOYKEY_BUTTON,
	        .setting = {
	            .button = 6
	        }
	    },
	    {
	        .device = 0,
	        .guid = 0,
	        .type = JOYKEY_BUTTON,
	        .setting = {
	            .button = 7
	        }
	    },
	    {
	        .device = 0,
	        .guid = 0,
	        .type = JOYKEY_BUTTON,
	        .setting = {
	            .button = 9
	        }
	    },
	    {
	        .device = 0,
	        .guid = 0,
	        .type = JOYKEY_BUTTON,
	        .setting = {
	            .button = 1
	        }
	    },
	    {
	        .device = 0,
	        .guid = 0,
	        .type = JOYKEY_BUTTON,
	        .setting = {
	            .button = 2
	        }
	    },
	    {
	        .device = 0,
	        .guid = 0,
	        .type = JOYKEY_BUTTON,
	        .setting = {
	            .button = 3
	        }
	    },
	    {
	        .device = 0,
	        .guid = 0,
	        .type = JOYKEY_BUTTON,
	        .setting = {
	            .button = 5
	        }
	    },
	    {
	        .device = 0,
	        .guid = 0,
	        .type = JOYKEY_BUTTON,
	        .setting = {
	            .button = 10
	        }
	    },
	    {
	        .device = 0,
	        .guid = 0,
	        .type = JOYKEY_BUTTON,
	        .setting = {
	            .button = 11
	        }
	    },
	    {
	        .device = 0,
	        .guid = 0,
	        .type = JOYKEY_BUTTON,
	        .setting = {
	            .button = 0
	        }
	    },
	    {
	        .device = 0,
	        .guid = 0,
	        .type = JOYKEY_BUTTON,
	        .setting = {
	            .button = 1
	        }
	    },
	    {
	        .device = 0,
	        .guid = 0,
	        .type = JOYKEY_BUTTON,
	        .setting = {
	            .button = 2
	        }
	    },
	    {
	        .device = 0,
	        .guid = 0,
	        .type = JOYKEY_BUTTON,
	        .setting = {
	            .button = 3
	        }
	    },
	    {
	        .device = 0,
	        .guid = 0,
	        .type = JOYKEY_BUTTON,
	        .setting = {
	            .button = 4
	        }
	    },
	    {
	        .device = 0,
	        .guid = 0,
	        .type = JOYKEY_BUTTON,
	        .setting = {
	            .button = 5
	        }
	    },
	    {
	        .device = 0,
	        .guid = 0,
	        .type = JOYKEY_BUTTON,
	        .setting = {
	            .button = 6
	        }
	    },
	    {
	        .device = 0,
	        .guid = 0,
	        .type = JOYKEY_BUTTON,
	        .setting = {
	            .button = 7
	        }
	    },
	    {
	        .device = 0,
	        .guid = 0,
	        .type = JOYKEY_BUTTON,
	        .setting = {
	            .button = 8
	        }
	    },
	    {
	        .device = 0,
	        .guid = 0,
	        .type = JOYKEY_BUTTON,
	        .setting = {
	            .button = 9
	        }
	    },
	},		//ジョイスティックボタン割り当て

	//Holdボタン(キーボード)割り当て
	.holdkey = { SDL_SCANCODE_V, SDL_SCANCODE_KP_0 },	//default 1p側:V 2p側:テンキー0

	.rots = {2, 1},
	.lvup = {1, 1},

	.screenMode = 3 | SCREENMODE_DETAILLEVEL,
	.screenIndex =0,
	.nextblock =8,
	.cfgversion =CFG_VERSION,
	.smooth =0,
	.nanameallow =1,
	.sonicdrop =0,
	.blockflash =3,
	.fastlrmove =1,
	.background =2,
};


int32_t SetDefaultConfig()
{
	int32_t i,j, cfgbuf[CFG_LENGTH];

	keyAssign[7] = DefaultConfig.holdkey[0];
	keyAssign[17] = DefaultConfig.holdkey[1];

	FillMemory(&cfgbuf, sizeof(cfgbuf), 0);
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
	cfgbuf[38] = DefaultConfig.giveupKey;
	cfgbuf[39] = DefaultConfig.ssKey;

	cfgbuf[40] = DefaultConfig.rots[0];
	cfgbuf[41] = DefaultConfig.rots[1];
	//cfgbuf[42] = DefaultConfig.lvup[0];
	//cfgbuf[43] = DefaultConfig.lvup[1];
	cfgbuf[44] = DefaultConfig.dtc;
	cfgbuf[45] = DefaultConfig.dispnext;
	cfgbuf[53] = DefaultConfig.fldtr;
	cfgbuf[54] = DefaultConfig.fontsize;
	cfgbuf[55] = DefaultConfig.maxPlay;
	cfgbuf[60] = DefaultConfig.movesound;
	cfgbuf[61] =
		(( DefaultConfig.se & 0x1) << 23) | (( DefaultConfig.sevolume & 0x7F) << 16) |
		((DefaultConfig.bgm & 0x1) << 15) | ((DefaultConfig.bgmvolume & 0x7F) <<  8) |
		(DefaultConfig.wavebgm & 0xFF);
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

     int32_t *joykeybuf = &cfgbuf[80];
        for (int32_t pl = 0; pl < 2; pl++) {
                for (int32_t key = 0; key < 10; key++) {
                        int32_t *plbuf = &joykeybuf[pl * 10 * 8 + key * 8];
                        SJoyKey *pljoy = &DefaultConfig.joykeyAssign[pl * 10 + key];
                        plbuf[0] = pljoy->device;
                        for (int32_t i = 0; i < 4; i++) {
                                plbuf[1 + i] = pljoy->guid.data[i];
                        }
                        plbuf[5] = pljoy->type;
                        switch (pljoy->type) {
                        case JOYKEY_AXIS:
                        case JOYKEY_HAT:
                                plbuf[6] = pljoy->setting.index;
                                plbuf[7] = pljoy->setting.value;
                                break;

                        case JOYKEY_BUTTON:
                                plbuf[6] = pljoy->setting.button;
                                break;
                        }
                }
        }

	cfgbuf[34] = ConfigChecksum(cfgbuf);

	SaveFile("config/data/CONFIG.SAV", &cfgbuf, sizeof(cfgbuf));

	return (0);
}
