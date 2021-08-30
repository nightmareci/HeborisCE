namespace readdef
{
	//#include "option.h"
	#include "keyconf.h"

	int	fontc[12]  = {9,1,2,3,8,4,3,6,7};	//題字の色	0:白 1:青 2:赤 3:桃 4:緑 5:黄 6:空 7:橙 8:紫 9:藍
	int	digitc[12] = {5,5,7,7,5,5,7,7,5};	//数字の色	それぞれ、TGMRule・TiRule・WorldRule・World2Rule
	int	giveupKey = SDL_SCANCODE_Q;		//捨てゲーキー (デフォルトはQ)
	int	ssKey = SDL_SCANCODE_HOME;		//スナップショットキー (デフォルトはHome)
	int	pausekey[2] = { SDL_SCANCODE_F1,SDL_SCANCODE_F2 };	//ポーズキー(デフォルトはF1,F2)		#1.60c7g7
	int	dispnextkey[2] = { SDL_SCANCODE_F3,SDL_SCANCODE_F4 };	//NEXT表示キー(デフォルトはF3,F4)	#1.60c7g7
	int	dtc = 1;				//tgmlvの表示	0:off  1:on  (lvtype = 1の時は常に表示)
	int	fldtr = 96;				//フィールド背景非表示時のフィールド透過度(0-256)
	int	dispnext = 3;			//ネクストブロック表示数の選択（０〜３）
	int	movesound = 1;			//ブロック移動音の選択	0:OFF 1:ON
	int	wavebgm = 0;			//BGMの選択
	int	maxPlay = 0;			//プレイヤー人数の選択	0:シングル 1:デュアル

	int	breakeffect = 1;	//ラインをそろえたとき、ブロックを弾けさせるか 0:off 1:on
	int	showcombo = 0;		//コンボの表示(SINGLEとかHEBORISとか) 0:off 1:on
	int	top_frame = 0;		//ブロックの高速消去 0:ブロックを左から右へ消す 1:同時に消す

	int	w_reverse = 1;		//ワールドルールで回転方法を逆転させる 0:off 1:on #1.60c7f8

	int	downtype = 1;		//下入れタイプ 0:HEBORIS 1:Ti #1.60c7f9

	int	lvupbonus = 0;		//レベルアップボーナス 0:TI 1:TGM/TAP #1.60c7g3

	int	fontsize = 1;			//フォントサイズ 0:DEFAULT 1:SMALL 宣言し忘れ修正#1.60c6.1a

	JoyKey	joykeyAssign[10 * 2] = { 0 };		//ジョイスティックボタン割り当て

	//Holdボタン(キーボード)割り当て
	int	holdkey[2] = { SDL_SCANCODE_V, SDL_SCANCODE_KP_0 };	//default 1p側:V 2p側:テンキー0

	int rots[2] = {2, 1};
	int lvup[2] = {1, 1};

	int		screenMode = 2;
	int		systemmem =0;
	int		nextblock =8;
	int		smooth =0;
	int		nanameallow =1;
	int		sonicdrop =0;
	int		blockflash =3;
	int		fastlrmove =1;
	int		background =2;


	int readdef()
	{
		int32_t i,j, cfgbuf[CFG_LENGTH];

		keyAssign[7] = holdkey[0];
		keyAssign[17] = holdkey[1];

		FillMemory(&cfgbuf, sizeof(cfgbuf), 0);
		cfgbuf[0] = 0x4F424550;
		cfgbuf[1] = 0x20534953;
		cfgbuf[2] = 0x464E4F44;
		cfgbuf[3] = 0x31764750;

		cfgbuf[4] = screenMode;
		cfgbuf[5] = systemmem;
		cfgbuf[6] = nextblock;
		//cfgbuf[7] = blockkind;
		cfgbuf[8] = smooth;
		cfgbuf[9] = nanameallow;
		cfgbuf[10] = sonicdrop;
		cfgbuf[11] = blockflash;
		cfgbuf[12] = fastlrmove;
		cfgbuf[13] = background;

		for (i = 0; i < 20; i++) {
			cfgbuf[14 + i] = keyAssign[i];
		}
		cfgbuf[38] = giveupKey;
		cfgbuf[39] = ssKey;

		cfgbuf[40] = rots[0];
		cfgbuf[41] = rots[1];
		//cfgbuf[42] = lvup[0];
		//cfgbuf[43] = lvup[1];
		cfgbuf[44] = dtc;
		cfgbuf[45] = dispnext;
		cfgbuf[53] = fldtr;
		cfgbuf[54] = fontsize;
		cfgbuf[55] = maxPlay;
		cfgbuf[60] = movesound;
		cfgbuf[61] = wavebgm;
		cfgbuf[62] = breakeffect;
		cfgbuf[63] = showcombo;
		cfgbuf[64] = top_frame;
		cfgbuf[65] = w_reverse;
		cfgbuf[66] = downtype;
		cfgbuf[67] = lvupbonus;
		cfgbuf[68] = pausekey[0];
		cfgbuf[69] = pausekey[1];
		cfgbuf[70] = dispnextkey[0];
		cfgbuf[71] = dispnextkey[1];


		cfgbuf[74] = fontc[0] + fontc[1] * 0x100 + fontc[2] * 0x10000 + fontc[3] * 0x1000000;
		cfgbuf[75] = digitc[0] + digitc[1] * 0x100 + digitc[2] * 0x10000 + digitc[3] * 0x1000000;
		cfgbuf[76] = fontc[4] + fontc[5] * 0x100 + fontc[6] * 0x10000 + fontc[7] * 0x1000000 ;
		cfgbuf[77] = digitc[4] + digitc[5] * 0x100 + digitc[6] * 0x10000 + digitc[7] * 0x1000000 ;
		cfgbuf[78] = fontc[8] + fontc[9] * 0x100 + fontc[10] * 0x10000 + fontc[11] * 0x1000000 ;
		cfgbuf[79] = digitc[8] + digitc[9] * 0x100 + digitc[10] * 0x10000 + digitc[11] * 0x1000000 ;

		memset(cfgbuf + 80, 0, 8 * 10 * 2 * sizeof(int32_t));

		SaveFile("config/data/CONFIG.SAV", &cfgbuf, sizeof(cfgbuf));

		return (0);
	}
}
