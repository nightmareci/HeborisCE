int	screenMode;		// ウィンドウモード	0:全画面 1:ウィンドウ(320x240) 2:ウィンドウ(640x480)
int colorMode;		// カラーモード		0:16bit (65536色) 1:32bit (1677万色)
int	systemmem;		// サーフェス格納	0:VRAM(高速) 1:システムメモリ(低速)
int	nextblock;		// ツモ
//int	blockkind;		// ブロックグラフィック	0:aa 1:ab 2:ba 3:bb
int	smooth;			// ブロック落下	0:ノーマル 1:スムーズ
int	nanameallow;	// 斜め入力		0:無効 1:有効
int	sonicdrop;		// 上入れ即接地	0:有効 1:無効
int	fastlrmove;		// 横先行入力	0:有効 1:無効
int	blockflash;		// ブロック枠	0:点滅 1:点灯 2:無し
int	background;		// フィールド背景0:スクロール 1:スクロール無し 2:ベタ

int	rots[2];		// 回転規則
int	lvup[2];		// レベルアップ方式
int	fontc[12];		// 題字の色	0:白 1:青 2:赤 3:桃 4:緑 5:黄 6:空 7:橙 8:紫 9:藍
int	digitc[12];		// 数字の色	それぞれ、TGMRule・TiRule・WorldRule・World2Rule・ARSRule・ARS2Rule・World3Rule
int	giveupKey = 0x10;	// 捨てゲーキー (デフォルトはQ)
int	ssKey = 0xC7;		// スナップショットキー (デフォルトはHome)
int pausekey[2] = { 0x3B, 0x3C };	// ポーズキー(デフォルトはF1, F2)		#1.60c7g7
int dispnextkey[2] = { 0x3D, 0x3E };	// NEXT表示キー(デフォルトはF3, F4) 	#1.60c7g7
int	dtc;			// tgmlvの表示	0:off  1:on  (lvtype = 1の時は常に表示)
int	fldtr;			// フィールド背景非表示時のフィールド透過度(0-256)
int	wavebgm;		// BGMの選択	0:標準midi 1:Wave 2:mp3 
// ver.160c6
int	dispnext;		// ネクスト表示個数設定
int	movesound;		// ブロック移動音設定	0:OFF　1:ON
int	fontsize;		// メイン画面で描画するフォントの大きさ	0:標準　1:小型
int	maxPlay;		// プレイする最大人数	0:シングル台　1:ツイン台
int lastmaxPlay;	// 設定変更前の、プレイする最大人数	0:シングル台　1:ツイン台
int lasttopframe;

int	breakeffect;	// ラインをそろえたとき、ブロックを弾けさせるか 0:off 1:on
int	showcombo;		// コンボの表示(SINGLEとかHEBORISとか) 0:off 1:on
//int	quickerase;		// ブロックの高速消去 0:ブロックを左から右へ消す 1:同時に消す

int	w_reverse;		// ワールドルールで回転方法を逆転させる 0:off 1:on #1.60c7f8

int	downtype;		// 下入れタイプ 0:HEBORIS 1:Ti #1.60c7f9

int	lvupbonus;		// レベルアップボーナス 0:TI 1:TGM/TAP 2:ajust#1.60c7g3

int	keyAssign[10 * 2] =		// キーボード設定 (↑↓←→ABCD)
{
31, 45, 44, 46, 48, 49, 57, 50, 0, 0, 		// sxzc bn[space]m
200, 208, 203, 205, 79, 80, 81, 82, 0, 0	// ↑↓←→ num1num2num3num0
};

int	joykeyAssign[10 * 2] = {		// ジョイスティックボタン割り当て
0, 1, 2, 3,   4, 5, 6, 7, -1, -1,		//default={0,1,2,3,10,7,4,8,-1,-1} →pauseとgiveupを追加 1.60c7g7
0, 1, 2, 3,   4, 5, 6, 7, -1, -1
};

int	restart;				// 再起動フラグ

// 設定をバイナリデータに保存 1.60c5
int SaveConfig(void) {
	int i, j, cfgbuf[100];

	FillMemory(&cfgbuf, 400, 0);
	cfgbuf[0] = 0x4F424550;
	cfgbuf[1] = 0x20534953;
	cfgbuf[2] = 0x464E4F44;
	cfgbuf[3] = 0x31764750;

	cfgbuf[4] = screenMode + colorMode * 0x100;
	cfgbuf[5] = systemmem;
	cfgbuf[6] = nextblock;
	//cfgbuf[7] = blockkind;
	cfgbuf[8] = smooth;
	cfgbuf[9] = nanameallow;
	cfgbuf[10] = sonicdrop;
	cfgbuf[11] = blockflash;
	cfgbuf[12] = fastlrmove;
	cfgbuf[13] = background;

	for(i = 0; i < 20; i++) {
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
	
	/* 72～74はジョイスティック用 */
	cfgbuf[74] = fontc[0] + fontc[1] * 0x100 + fontc[2] * 0x10000 + fontc[3] * 0x1000000;
	cfgbuf[75] = digitc[0] + digitc[1] * 0x100 + digitc[2] * 0x10000 + digitc[3] * 0x1000000;
	cfgbuf[76] = fontc[4] + fontc[5] * 0x100 + fontc[6] * 0x10000 + fontc[7] * 0x1000000;
	cfgbuf[77] = digitc[4] + digitc[5] * 0x100 + digitc[6] * 0x10000 + digitc[7] * 0x1000000;
	cfgbuf[78] = fontc[8] + fontc[9] * 0x100 + fontc[10] * 0x10000 + fontc[11] * 0x1000000;
	cfgbuf[79] = digitc[8] + digitc[9] * 0x100 + digitc[10] * 0x10000 + digitc[11] * 0x1000000;
	
	for(j=0;j<=1;j++)
	for(i=0;i<6;i++){
		if(!cfgbuf[i + 80 + (j * 6)]) cfgbuf[i + 80 + (j * 6)] = -1;
		cfgbuf[i + 80 + (j * 6)] = joykeyAssign[i + 4 + (4 * j) + (j * 6)];
	}
	
	SaveFile("CONFIG.SAV", &cfgbuf, 400);

	return (0);
}

// 設定をバイナリデータから読み込み 1.60c5
int LoadConfig(void) {
	int i, j, cfgbuf[100];


	FillMemory(&cfgbuf, 100 * 4, 0);
	LoadFile("CONFIG.SAV", &cfgbuf, 16);
	if(cfgbuf[0] != 0x4F424550) return (1);
	if(cfgbuf[1] != 0x20534953) return (1);
	if(cfgbuf[2] != 0x464E4F44) return (1);
	if(cfgbuf[3] != 0x31764750) return (1);

	LoadFile("CONFIG.SAV", &cfgbuf, 400);

	screenMode = cfgbuf[4] & 0xff;
	colorMode = cfgbuf[4] >> 8;
	systemmem = cfgbuf[5];
	nextblock = cfgbuf[6];
	//blockkind = cfgbuf[7];
	smooth = cfgbuf[8];
	nanameallow = cfgbuf[9];
	sonicdrop = cfgbuf[10];
	blockflash = cfgbuf[11];
	fastlrmove = cfgbuf[12];
	background = cfgbuf[13];

	for(i = 0; i < 20; i++) {
		keyAssign[i] = cfgbuf[14 + i];
	}
	giveupKey = cfgbuf[38];
	ssKey = cfgbuf[39];

	rots[0] = cfgbuf[40];
	rots[1] = cfgbuf[41];
	//lvup[0] = cfgbuf[42];
	//lvup[1] = cfgbuf[43];
	dtc     = cfgbuf[44];
	dispnext = cfgbuf[45];

	fldtr = cfgbuf[53];
	fontsize = cfgbuf[54];
	maxPlay = cfgbuf[55];
	movesound = cfgbuf[60];
	wavebgm = cfgbuf[61];

	breakeffect = cfgbuf[62];
	showcombo = cfgbuf[63];
	top_frame = cfgbuf[64];

	w_reverse = cfgbuf[65];

	downtype = cfgbuf[66];
	lvupbonus = cfgbuf[67];
	pausekey[0] = cfgbuf[68];
	pausekey[1] = cfgbuf[69];
	dispnextkey[0] = cfgbuf[70];
	dispnextkey[1] = cfgbuf[71];
	
	/* 72～74はジョイスティック用 */
	
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
	for(j=0;j<=1;j++)
	for(i=0;i<6;i++){
		if(!cfgbuf[i + 80 + (j * 6)]) cfgbuf[i + 80 + (j * 6)] = -1;
		joykeyAssign[i + 4 + (4 * j)+ (j * 6)] = cfgbuf[i + 80 + (j * 6)];
	}
		
	return (0);
}



void ConfigMenu() {
	int i, j, m, pl, count, pages;
	int ncfg[100];
	int need_reset;	// 設定保存時にリセットするか
	int need_reloadBG;
	int last_BG;
	
	pages = 3;

	ncfg[0]  = screenMode + colorMode * 0x100;
	ncfg[1]  = systemmem;
	ncfg[2]  = nextblock;
	//ncfg[3]  = blockkind;
	ncfg[4]  = smooth;
	ncfg[5]  = nanameallow;
	ncfg[6]  = sonicdrop;
	ncfg[7]  = fastlrmove;
	ncfg[8]  = blockflash;
	ncfg[9]  = background;
	last_BG = background;

	for(i = 0; i < 20; i++) ncfg[10 + i] = keyAssign[i];

	ncfg[34] = giveupKey;
	ncfg[35] = ssKey;
	ncfg[36] = dtc;

	ncfg[43] = fldtr;
	ncfg[44] = wavebgm;
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
	ncfg[55] = pausekey[0];
	ncfg[56] = pausekey[1];
	ncfg[57] = dispnextkey[0];
	ncfg[58] = dispnextkey[1];
	
	/* 59～61はジョイスティック用 */
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
	
	for(j=0;j<=1;j++)
	for(i=0;i<6;i++){
		ncfg[i + 80 + (j * 6)] = joykeyAssign[i + 4 + (4 * j) + (j * 6)];
		if(!ncfg[i + 80 + (j * 6)]) ncfg[i + 80 + (j * 6)] = -1;
	}
	
	for(i = 0; i < 10; i++) statc[i] = 0;

	stat[0] = 0;
	statc[0] = 1;
	statc[1] = 1;
	statc[2] = 0;

	count = 0;

	need_reset = 0;
	need_reloadBG = 1;
	
	loop {
		count++;
		KeyInput();
		
		// 背景描画
		if(background == 0) {
			for(i = 0; i <= 4; i++) {
				if(getDrawRate() == 1)
					BltFastRect(4, 96 * i - (count % 96) / 3, 0, 0, 0, 96, 240);
				else
					BltFastRect(4, 192 * i - (count % 32), 0, 0, 0, 192, 480);
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

		// main setting
		if(stat[0] == 0){
			printFont(23, 1, "- MAIN", fontc[rots[0]]);
			printFont(2,  3, "<< INPUT <<             >> DESIGN >>", digitc[rots[0]] * (statc[0] == 0) * (count % 2));
			printFont(2,  6, "SCREEN MODE :", (statc[0] == 1) * fontc[rots[0]]);
			// printFont(2,  7, "COLOR MODE  :", (statc[0] == 2) * fontc[rots[0]]);
			// printFont(2,  8, "GRAPHIC LOAD:", (statc[0] == 3) * fontc[rots[0]]);
			printFont(2, 10, "NEXT PATTERN:", (statc[0] == 4) * fontc[rots[0]]);
			printFont(2, 11, "NEXT DISPLAY:", (statc[0] == 5) * fontc[rots[0]]);
			printFont(2, 13, "8WAY INPUT  :", (statc[0] == 6) * fontc[rots[0]]);
			printFont(2, 14, "SONIC DROP  :", (statc[0] == 7) * fontc[rots[0]]);
			printFont(2, 15, "INIT LR MOVE:", (statc[0] == 8) * fontc[rots[0]]);
			printFont(2, 17, "BLOCK FALL  :", (statc[0] == 9) * fontc[rots[0]]);
			printFont(2, 19, "SHOW LEVEL  :", (statc[0] == 10) * fontc[rots[0]]);	// "TGM LEVEL"を"SHOW LEVEL"に変更 #1.60c7i2
			printFont(2, 21, "MOVE SOUND  :", (statc[0] == 11) * fontc[rots[0]]);
			printFont(2, 22, "BGM         :", (statc[0] == 12) * fontc[rots[0]]);
			printFont(2, 23, "WORLDREVERSE:", (statc[0] == 13) * fontc[rots[0]]);
			printFont(2, 24, "DOWN TYPE   :", (statc[0] == 14) * fontc[rots[0]]);
			printFont(2, 25, "LVUP BONUS  :", (statc[0] == 15) * fontc[rots[0]]);
			printFont(2, 28, "A:SAVE&RETURN  B:CANCEL", 9);

			i = statc[0];
			if(statc[0] != 0) printFont(1, 5 + i + (i > 3) + (i > 5) + (i > 8) + (i > 9) + (i > 10), "b", fontc[rots[0]]);
			else printFont(1, 3, "b", fontc[rots[0]]);

//			if(statc[1]) {	// 表示更新
				/* 050825 画面モード拡張改造部分 -- ここから */
				if((ncfg[0] & 0xff) == 0) sprintf(string[0], "FULL SCREEN 320X240");
				else if((ncfg[0] & 0xff) == 1) sprintf(string[0], "WINDOW 320X240");
				else if((ncfg[0] & 0xff) == 2) sprintf(string[0], "WINDOW 640X480");
				else if((ncfg[0] & 0xff) == 3) sprintf(string[0], "FULL SCREEN 640X480");
				else if((ncfg[0] & 0xff) == 4) sprintf(string[0], "WINDOW 800X600");
				else if((ncfg[0] & 0xff) == 5) sprintf(string[0], "WINDOW 1024X768");
				else if((ncfg[0] & 0xff) == 6) sprintf(string[0], "WINDOW 1280X960");
				else if((ncfg[0] & 0xff) == 7) sprintf(string[0], "FULL SCREEN 640X480(LITE)");
				else if((ncfg[0] & 0xff) == 8) sprintf(string[0], "WINDOW 480X360 (LITE)");
				else if((ncfg[0] & 0xff) == 9) sprintf(string[0], "WINDOW 640X480 (LITE)");
				else if((ncfg[0] & 0xff) == 10) sprintf(string[0], "WINDOW 800X600 (LITE)");
				else if((ncfg[0] & 0xff) == 11) sprintf(string[0], "WINDOW 1024X768 (LITE)");
				else if((ncfg[0] & 0xff) == 12) sprintf(string[0], "WINDOW 1280X960 (LITE)");
				/* ここまで */
				printFont(15, 6, string[0], (statc[0] == 1) * (count % 2) * digitc[rots[0]]);

#if		0
				if(ncfg[0] >> 8 == 0) sprintf(string[0], "HIGH COLOR (16BIT)");
				else if(ncfg[0] >> 8 == 1) sprintf(string[0], "TRUE COLOR (32BIT)");
				printFont(15, 7, string[0], (statc[0] == 2) * (count % 2) * digitc[rots[0]]);

				if(ncfg[1]) sprintf(string[0], "SYSTEM MEMORY");
				else sprintf(string[0], "VIDEO MEMORY");
				printFont(15, 8, string[0], (statc[0] == 3) * (count % 2) * digitc[rots[0]]);
#endif

				if((ncfg[2] > 1)&&(ncfg[2] < 8))
				sprintf(string[0], "HEBO%d", ncfg[2]);
				else if(ncfg[2] == 0)
				sprintf(string[0], "RANDOM", ncfg[2]);
				else if(ncfg[2] == 1)
				sprintf(string[0], "MEMORY1", ncfg[2]);
				else if(ncfg[2] == 8)
				sprintf(string[0], "MEMORY4", ncfg[2]);
				else if(ncfg[2] == 9)
				sprintf(string[0], "GUIDELINE", ncfg[2]);
				else if(ncfg[2] == 10)
				sprintf(string[0], "DENGEN", ncfg[2]);
				else if(ncfg[2] == 11)
				sprintf(string[0], "TOMOYO", ncfg[2]);
				else if(ncfg[2] == 12)
				sprintf(string[0], "FP", ncfg[2]);
				printFont(15, 10, string[0], (statc[0] == 4) * (count % 2) * digitc[rots[0]]);

				sprintf(string[0], "%d", ncfg[45]);
				printFont(15, 11, string[0], (statc[0] == 5) * (count % 2) * digitc[rots[0]]);

				if(ncfg[5]) sprintf(string[0], "e"); // × 斜め入力
				else sprintf(string[0], "c");		// ○
				printFont(15, 13, string[0], (statc[0] == 6) * (count % 2) * digitc[rots[0]]);

				if(ncfg[6]) sprintf(string[0], "e");	// × 高速落下
				else sprintf(string[0], "c");		// ○
				printFont(15, 14, string[0], (statc[0] == 7) * (count % 2) * digitc[rots[0]]);

				if(ncfg[7]) sprintf(string[0], "e");	// × 横先行入力
				else sprintf(string[0], "c");		// ○
				printFont(15, 15, string[0], (statc[0] == 8) * (count % 2) * digitc[rots[0]]);

				if(ncfg[4]) sprintf(string[0], "SMOOTH");
				else sprintf(string[0], "NORMAL");
				printFont(15, 17, string[0], (statc[0] == 9) * (count % 2) * digitc[rots[0]]);

				if(ncfg[36]) sprintf(string[0], "ON");
				else sprintf(string[0], "OFF");
				printFont(15, 19, string[0], (statc[0] == 10) * (count % 2) * digitc[rots[0]]);

				if(ncfg[46]) sprintf(string[0], "ON");
				else sprintf(string[0], "OFF");
				printFont(15, 21, string[0], (statc[0] == 11) * (count % 2) * digitc[rots[0]]);

				if(ncfg[44] == 0) sprintf(string[0], "MIDI (SIMPLE)");
				else if(ncfg[44] == 1) sprintf(string[0], "MIDI");
				else if(ncfg[44] == 2) sprintf(string[0], "WAVE");
				else if(ncfg[44] == 3) sprintf(string[0], "OGG");
				else if(ncfg[44] == 4) sprintf(string[0], "MP3");
				else if(ncfg[44] == 5) sprintf(string[0], "MOD (.MOD)");
				else if(ncfg[44] == 6) sprintf(string[0], "MOD (.IT)");
				else if(ncfg[44] == 7) sprintf(string[0], "MOD (.XM)");
				printFont(15, 22, string[0], (statc[0] == 12) * (count % 2) * digitc[rots[0]]);

				// WORLDREVERSE
				if(ncfg[52]) sprintf(string[0], "ON");
				else sprintf(string[0], "OFF");
				printFont(15, 23, string[0], (statc[0] == 13) * (count % 2) * digitc[rots[0]]);

				// downtype
				if(ncfg[53]) sprintf(string[0], "DOWN RESET");
				else sprintf(string[0], "NO RESET(HEBORIS)");
				printFont(15, 24, string[0], (statc[0] == 14) * (count % 2) * digitc[rots[0]]);

				// lvupbonus
				if(ncfg[54]==0) sprintf(string[0], "ON");
				else if(ncfg[54] ==1)sprintf(string[0], "OFF");
				else if(ncfg[54] ==2)sprintf(string[0], "ADJUST");
				printFont(15, 25, string[0], (statc[0] == 15) * (count % 2) * digitc[rots[0]]);

				statc[1] = 0;
//			}

//			for(i = 0; i < 10; i++) printFont(15, 6 + 2 * i, string[i], (statc[0] == i + 1) * (count % 2) * digitc[rots[0]]);

			for(pl = 0; pl < 2; pl++) {
//				m = getPushState(pl, 1) - getPushState(pl, 0);
				// ↑↓カーソルリピード #1.60c7k8
				m=0;
				padRepeat2(pl);
				// ↑
				if((mpc2[0] == 1) || ((mpc2[0] > tame3) && (mpc2[0] % tame4 == 0)))
					if(getPressState(pl, 0)) m--;

				// ↓
				if((mpc2[0] == 1) || ((mpc2[0] > tame3) && (mpc2[0] % tame4 == 0)))
					if(getPressState(pl, 1)) m++;
				padRepeat(pl);
				if(m) {
					PlaySE(5);
					statc[0] = (statc[0] + m + 16) % 16;
				}
				// HOLDボタンでページ切り替え #1.60c7k8
				if(getPushState(pl, 7)) {
					PlaySE(3);
					stat[0] = (stat[0] + 1 + pages)%pages;
					statc[0] = 0;
					statc[1] = 1;
				} else {
					m = getPushState(pl, 3) - getPushState(pl, 2);

					// C++版で無効な項目
					if ( statc[0] == 2 ) { statc[0] = 4; }
					if ( statc[0] == 3 ) { statc[0] = 1; }

					if(m) {
						PlaySE(5);
						
						if(statc[0] == 1){
#if		HEBORIS_DIRECTX9 || SDL_USE_OPENGL
							ncfg[0] = (ncfg[0] & 0x100) + ((ncfg[0] % 0x100) + 13 + m) % 13;	// screenMode
#else
							ncfg[0] = (ncfg[0] & 0x100) + ((ncfg[0] % 0x100) + 4 + m) % 4;	// screenMode
#endif
							need_reset = 1;
						}
#if		0
						else if(statc[0] == 2) {
							ncfg[0] = ncfg[0] ^ 0x100;			// colorMode
							need_reset = 1;
						}
						else if(statc[0] == 3) {
							ncfg[1] = !ncfg[1];					// systemmem
							need_reset = 1;
						}
#endif
						else if(statc[0] == 4) ncfg[2] = (ncfg[2] + 13 + m) % 13;		// nextbloc 8を追加#1.60c7h4
						else if(statc[0] == 5) ncfg[45] = (ncfg[45] + 7 + m) % 7;		// dispnext
						else if(statc[0] == 6) ncfg[5] = !ncfg[5];			// nanameallow
						else if(statc[0] == 7) ncfg[6] = !ncfg[6];			// sonicdrop
						else if(statc[0] == 8) ncfg[7] = !ncfg[7];			// fastlrmove
						else if(statc[0] == 9) ncfg[4] = !ncfg[4];			// smooth
						else if(statc[0] == 10) ncfg[36] = !ncfg[36];		// tgmlv
						else if(statc[0] == 11) ncfg[46] = !ncfg[46];		// movesound
						else if(statc[0] == 12) {
							// bgmwave
							ncfg[44] = ncfg[44] + m;
							
							if(ncfg[44] < 0) ncfg[44] = 7;
							if(ncfg[44] > 7) ncfg[44] = 0;
							
							if(bgm) need_reset = 1;
						}
						else if(statc[0] == 13) ncfg[52] = !ncfg[52];		// w_reverse
						else if(statc[0] == 14) ncfg[53] = !ncfg[53];		// downtype
						else if(statc[0] == 15) ncfg[54] = (ncfg[54] + 3 + m)%3;		// lvupbonus

						else if(statc[0] == 0) {	// page
							PlaySE(3);
							stat[0] = (stat[0] + m + pages) % pages;
							statc[0] = 0;
							statc[1] = 0;
						}
						statc[1] = 1;
					}
				}
				if(getPushState(pl, 4)) {	// A:保存&再起動
					PlayWave(10);
					screenMode = ncfg[0] & 0xff;
					colorMode = ncfg[0] >> 8;
					systemmem = ncfg[1];
					nextblock = ncfg[2];
					//blockkind = ncfg[3];
					smooth = ncfg[4];
					nanameallow = ncfg[5];
					sonicdrop = ncfg[6];
					fastlrmove = ncfg[7];
					blockflash = ncfg[8];
					background = ncfg[9];

//					for(i = 0; i < 20; i++) keyAssign[i] = ncfg[10 + i];	// keyAssignは変更後すぐに反映
//					for(i = 0; i < 4; i++) joykeyAssign[i + 4] = ncfg[30 + i];
//					giveupKey = ncfg[34];
//					ssKey = ncfg[35];
					dtc = ncfg[36];
					
					fldtr = ncfg[43];
					wavebgm = ncfg[44];
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
					SaveConfig();
					if((maxPlay == lastmaxPlay) && (top_frame == lasttopframe) && (((last_BG <= 1) && (background <= 1)) || (last_BG == 2) && (background == 2)))
						need_reloadBG = 0;
					
					tmp_maxPlay = maxPlay;
					
					if(need_reset) {
						restart = 1;	// 再起動フラグ
					} else if(need_reloadBG==1){	//プレイする最大人数の変更があったら…
						loadBG(maxPlay,0);			// 背景だけ再読み込み C7T2.5EX
					}
					stat[0] = -1;
				}

				if(getPushState(pl, 5)) stat[0] = -1;	// B:設定破棄&タイトル画面に戻る
			}
		}


		// design setting
		else if(stat[0] == 1) {
			printFont(23, 1, "- DESIGN SETTING", fontc[rots[0]]);
			printFont(2, 3, "<< MAIN <<               >> INPUT >>", digitc[rots[0]] * (statc[0] == 0) * (count % 2));
			
			printFont( 2,  6, "BLOCK FRAME    :", fontc[rots[0]] * (statc[0] == 1));
			printFont( 2,  7, "FIELD BG       :", fontc[rots[0]] * (statc[0] == 2));
			printFont( 2,  8, "FIELD BG TR    :", fontc[rots[0]] * (statc[0] == 3));
			
			printFont( 2,  9, "FONT [HEBORIS] :", fontc[rots[0]] * (statc[0] == 4));
			printFont( 2, 10, "FONT [TI-ARS]  :", fontc[rots[0]] * (statc[0] == 5));
			printFont( 2, 11, "FONT [TI-WORLD]:", fontc[rots[0]] * (statc[0] == 6));
			printFont( 2, 12, "FONT [ACE-SRS] :", fontc[rots[0]] * (statc[0] == 7));
			printFont( 2, 13, "FONT [ACE-ARS] :", fontc[rots[0]] * (statc[0] == 8));
			printFont( 2, 14, "FONT [ACE-ARS2]:", fontc[rots[0]] * (statc[0] == 9));
			printFont( 2, 15, "FONT [DS-WORLD]:", fontc[rots[0]] * (statc[0] == 10));
			printFont( 2, 16, "FONT [SRS-X]   :", fontc[rots[0]] * (statc[0] == 11));
			printFont( 2, 17, "FONT [DRS]     :", fontc[rots[0]] * (statc[0] == 12));
			
			printFont(22,  9, "DIGIT[HEBORIS] :", fontc[rots[0]] * (statc[0] == 13));
			printFont(22, 10, "DIGIT[TI-ARS]  :", fontc[rots[0]] * (statc[0] == 14));
			printFont(22, 11, "DIGIT[TI-WORLD]:", fontc[rots[0]] * (statc[0] == 15));
			printFont(22, 12, "DIGIT[ACE-SRS] :", fontc[rots[0]] * (statc[0] == 16));
			printFont(22, 13, "DIGIT[ACE-ARS] :", fontc[rots[0]] * (statc[0] == 17));
			printFont(22, 14, "DIGIT[ACE-ARS2]:", fontc[rots[0]] * (statc[0] == 18));
			printFont(22, 15, "DIGIT[DS-WORLD]:", fontc[rots[0]] * (statc[0] == 19));
			printFont(22, 16, "DIGIT[SRS-X]   :", fontc[rots[0]] * (statc[0] == 20));
			printFont(22, 17, "DIGIT[DRS]     :", fontc[rots[0]] * (statc[0] == 21));
			
			printFont( 2, 19, "NUMBER FONT    :", fontc[rots[0]] * (statc[0] == 22));
			printFont( 2, 20, "MAX PLAYER     :", fontc[rots[0]] * (statc[0] == 23));
			printFont( 2, 21, "BREAK EFFECT   :", fontc[rots[0]] * (statc[0] == 24));
			printFont( 2, 22, "SHOW COMBOS    :", fontc[rots[0]] * (statc[0] == 25));
			printFont( 2, 23, "TOP FRAME      :", fontc[rots[0]] * (statc[0] == 26));
			
			printFont(2, 28, "A/B:RETURN MAIN", 9);

			i = statc[0];
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
			printFont(18, 6, string[0], (statc[0] == 1) * (count % 2) * digitc[rots[0]]);

			if(ncfg[9] == 0) sprintf(string[0], "GRID (SCROLLING)");
			else if(ncfg[9] == 1) sprintf(string[0], "GRID");
			else sprintf(string[0], "BLACK");
			printFont(18, 7, string[0], (statc[0] == 2) * (count % 2) * digitc[rots[0]]);

			sprintf(string[0], "%d", ncfg[43]);
			printFont(18, 8, string[0], (statc[0] == 3) * (count % 2) * digitc[rots[0]]);

			for(i = 0; i < 9; i++) { //FONT
				sprintf(string[0], "%d", ncfg[62+(2*i)]);
				printFont(18, 9+i, string[0], ncfg[62+(2*i)]);
			}
			for(i = 0; i < 9; i++) {//DIGIT
				sprintf(string[0], "%d", ncfg[63+(2*i)]);
				printFont(38, 9+i, string[0], ncfg[63+(2*i)]);
			}

			if(ncfg[47] == 0) sprintf(string[0], "DEFAULT");
			else sprintf(string[0], "SHARP");
			printFont(18, 19, string[0], (statc[0] == 22) * (count % 2) * digitc[rots[0]]);
			
			if(ncfg[48] == 0) sprintf(string[0], "SINGLE");
			else sprintf(string[0], "DUAL");
			printFont(18, 20, string[0], (statc[0] == 23) * (count % 2) * digitc[rots[0]]);

			//breakefect
			if(ncfg[49] == 0) sprintf(string[0], "OFF");
			else sprintf(string[0], "ON");
			printFont(18, 21, string[0], (statc[0] == 24) * (count % 2) * digitc[rots[0]]);

			//showcombo
			if(ncfg[50] == 0) sprintf(string[0], "OFF");
			else sprintf(string[0], "ON");
			printFont(18, 22, string[0], (statc[0] == 25) * (count % 2) * digitc[rots[0]]);

			//top frame
			if(ncfg[51] == 0) sprintf(string[0], "TI");
			else sprintf(string[0], "ACE");
			printFont(18, 23, string[0], (statc[0] == 26) * (count % 2) * digitc[rots[0]]);

			statc[1] = 0;

			for(pl = 0; pl < 2; pl++) {
				padRepeat(pl);
				m=0;
				padRepeat2(pl);
				// ↑
				if((mpc2[0] == 1) || ((mpc2[0] > tame3) && (mpc2[0] % tame4 == 0)))
					if(getPressState(pl, 0)) m--;

				// ↓
				if((mpc2[0] == 1) || ((mpc2[0] > tame3) && (mpc2[0] % tame4 == 0)))
					if(getPressState(pl, 1)) m++;
				if(m) {
					PlaySE(5);
					statc[0] = (statc[0] + m + 27) % 27;
				}
				
				// HOLDボタンでページ切り替え #1.60c7k8
				if(getPushState(pl, 7)) {
					PlaySE(3);
					stat[0] = (stat[0] + 1 + pages)%pages;
					statc[0] = 0;
					statc[1] = 1;
				} else {
					m = getPushState(pl, 3) - getPushState(pl, 2);
					if(m) {
						PlaySE(5);
						if(statc[0] == 1) ncfg[8] = (ncfg[8] + 4 + m) % 4;	// blockflash
						if(statc[0] == 2) ncfg[9] = (ncfg[9] + 3 + m) % 3;	// background
						
						if(statc[0] == 4) ncfg[62] = (ncfg[62] + m + 10) % 10;
						if(statc[0] == 5) ncfg[64] = (ncfg[64] + m + 10) % 10;
						if(statc[0] == 6) ncfg[66] = (ncfg[66] + m + 10) % 10;
						if(statc[0] == 7) ncfg[68] = (ncfg[68] + m + 10) % 10; // WORLD2 FONT
						if(statc[0] == 8) ncfg[70] = (ncfg[70] + m + 10) % 10; // ARS FONT
						if(statc[0] == 9) ncfg[72] = (ncfg[72] + m + 10) % 10; // ARS2 FONT
						if(statc[0] == 10) ncfg[74] = (ncfg[74] + m + 10) % 10; // WORLD3 FONT
						if(statc[0] == 11) ncfg[76] = (ncfg[76] + m + 10) % 10;
						if(statc[0] == 12) ncfg[78] = (ncfg[78] + m + 10) % 10;
						
						if(statc[0] == 13) ncfg[63] = (ncfg[63] + m + 10) % 10;
						if(statc[0] == 14) ncfg[65] = (ncfg[65] + m + 10) % 10;
						if(statc[0] == 15) ncfg[67] = (ncfg[67] + m + 10) % 10;
						if(statc[0] == 16) ncfg[69] = (ncfg[69] + m + 10) % 10; // WORLD2 DIGIT
						if(statc[0] == 17) ncfg[71] = (ncfg[71] + m + 10) % 10; // ARS DIGIT
						if(statc[0] == 18) ncfg[73] = (ncfg[73] + m + 10) % 10; // ARS2 DIGIT
						if(statc[0] == 19) ncfg[75] = (ncfg[75] + m + 10) % 10; // WORLD3 DIGIT
						if(statc[0] == 20) ncfg[77] = (ncfg[77] + m + 10) % 10;
						if(statc[0] == 21) ncfg[79] = (ncfg[79] + m + 10) % 10;
						
						if(statc[0] == 22) ncfg[47] = !ncfg[47];
						if(statc[0] == 23) ncfg[48] = !ncfg[48];
						if(statc[0] == 24) ncfg[49] = !ncfg[49];	// breakeffect
						if(statc[0] == 25) ncfg[50] = !ncfg[50];	// showcombo
						if(statc[0] == 26) ncfg[51] = !ncfg[51];	// top_frame
						
						if(statc[0] == 0){
							PlaySE(3);
							stat[0] = (stat[0] + m + pages)%pages;
							statc[0] = 0;
						}
						statc[1] = 1;
					}
				}
				if(mp[pl] || m) {
					if(statc[0] == 3){	// fldtr
						if(m) ncfg[43] = (ncfg[43] + 257 + m) % 257;
						else if(mpc[pl] > 30) ncfg[43] = (ncfg[43] + 252 + mp[pl] * 2) % 257;
					}
					statc[1] = 1;
				}

				if(getPushState(pl, 4) || getPushState(pl, 5)) {	// A&B:mainに戻る
					PlaySE(3);
					PlaySE(5);
					stat[0] = 0;
					statc[0] = 0;
					statc[1] = 1;
				}
			}
		} else if(stat[0] == 2) {
			// input setting
			// menu
			if(statc[2] == 0) {
				printFont(23, 1, "- INPUT SETTING", fontc[rots[0]]);
				printFont(2,  3, "<< DESIGN <<              >> MAIN >>", digitc[rots[0]] * (statc[0] == 0) * (count % 2));
				printFont(2,  6, "[KEYBOARD 1P]", fontc[rots[0]] * (statc[0] == 1));
				printFont(2,  8, "[KEYBOARD 2P]", fontc[rots[0]] * (statc[0] == 2));
				printFont(2, 10, "[JOYSTICK 1P]", fontc[rots[0]] * (statc[0] == 3));
				printFont(2, 12, "[JOYSTICK 2P]", fontc[rots[0]] * (statc[0] == 4));
				printFont(2, 14, "[FUNCTION KEY]", fontc[rots[0]] * (statc[0] == 5));
				printFont(2, 16, "[INPUT TEST]", fontc[rots[0]] * (statc[0] == 6));
				printFont(2, 18, "[DISP ASSIGN]", fontc[rots[0]] * (statc[0] == 7));
				printFont(2, 28, "A:DO ASSIGN  B:RETURN MAIN", 9);

				if(statc[0] != 0) printFont(1, 4+statc[0]*2, "b", fontc[rots[0]]);
				else printFont(1, 3, "b", fontc[rots[0]]);

				for(pl = 0; pl < 2; pl++) {
//					m = getPushState(pl, 1) - getPushState(pl, 0);
					// ↑↓カーソルリピード #1.60c7k8
					m=0;
					padRepeat2(pl);
					// ↑
					if((mpc2[0] == 1) || ((mpc2[0] > tame3) && (mpc2[0] % tame4 == 0)))
						if(getPressState(pl, 0)) m--;

					// ↓
					if((mpc2[0] == 1) || ((mpc2[0] > tame3) && (mpc2[0] % tame4 == 0)))
						if(getPressState(pl, 1)) m++;
					if(m) {
						PlaySE(5);
						statc[0] = (statc[0] + m + 8) % 8;
					}
					// HOLDボタンでページ切り替え #1.60c7k8
					if(getPushState(pl, 7)) {
						PlaySE(3);
						stat[0] = (stat[0] + 1 + pages)%pages;
						statc[0] = 0;
						statc[1] = 1;
					} else {
						m = getPushState(pl, 3) - getPushState(pl, 2);
						if(m) {
							PlaySE(5);
							if(statc[0] == 0){
								PlaySE(3);
								stat[0] = (stat[0] + m + pages)%pages;
								statc[0] = 0;
								statc[1] = 1;
							}
						}
					}

					if(getPushState(pl, 4) && (statc[0] != 0)) {
						PlaySE(19);
						statc[2] = statc[0];
						statc[0] = 0;
					}

					if(getPushState(pl, 5)) {
						PlaySE(3);
						PlaySE(5);
						stat[0] = 0;
						statc[0] = 0;
						statc[1] = 1;
					}
				}
			} else if((statc[2] == 1) || (statc[2] == 2)) {
				// keyboard 1p&2p
				sprintf(string[0], "KEYBOARD %dP SETTING", statc[2]);
				printFont(2, 3, string[0], digitc[rots[0]]);

				printFont(3,  6, "UP      :", fontc[rots[0]] * (statc[0] == 0));
				printFont(3,  8, "DOWN    :", fontc[rots[0]] * (statc[0] == 1));
				printFont(3, 10, "LEFT    :", fontc[rots[0]] * (statc[0] == 2));
				printFont(3, 12, "RIGHT   :", fontc[rots[0]] * (statc[0] == 3));
				printFont(3, 14, "A(L-ROT):", fontc[rots[0]] * (statc[0] == 4));
				printFont(3, 16, "B(R-ROT):", fontc[rots[0]] * (statc[0] == 5));
				printFont(3, 18, "C(L-ROT):", fontc[rots[0]] * (statc[0] == 6));
				printFont(3, 20, "D(HOLD) :", fontc[rots[0]] * (statc[0] == 7));

				for(i = 0; i < statc[0]; i++) {
					sprintf(string[0], "%2X", ncfg[10 + i + (statc[2] - 1) * 10]);
					printFont(13, 6 + i * 2, string[0], digitc[rots[0]]);
				}

				if(statc[0] < 8) {
					printFont(13, 6 + statc[0] * 2, "_", digitc[rots[0]] * (count % 2));
					for(i = 0; i < getMaxKey(); i++) {
						if(IsPushKey(i)) {
							PlaySE(5);
							ncfg[10 + statc[0] + (statc[2] - 1) * 10] = i;
							statc[0]++;
							break;
						}
					}
				} else {
					printFont(3, 22, "OK[ENTER] / RETRY[DEL] / CANCEL[BS]", digitc[rots[0]] * (count % 2));
					if(IsPushReturnKey()) {
						PlaySE(10);
						for(i = 0; i < 20; i++) keyAssign[i] = ncfg[10 + i];
						statc[0] = 0;
						statc[2] = 0;
					}
					if(IsPushDeleteKey()) {
						PlaySE(5);
						for(i = 0; i < 20; i++) ncfg[10 + i] = keyAssign[i];
						statc[0] = 0;
					}
					if(IsPushBSKey()) {
						PlaySE(5);
						for(i = 0; i < 20; i++) ncfg[10 + i] = keyAssign[i];
						statc[0] = 0;
						statc[2] = 0;
					}
				}
			} else if((statc[2] == 3) || (statc[2] == 4)) {
				// Joystick
				pl = statc[2] - 3;
				sprintf(string[0], "JOYSTICK %dP SETTING",statc[2] - 2);
				printFont(2, 3, string[0], digitc[rots[0]]);

				printFont(3,  6, "A(L-ROT):", fontc[rots[0]] * (statc[0] == 0));
				printFont(3,  8, "B(R-ROT):", fontc[rots[0]] * (statc[0] == 1));
				printFont(3, 10, "C(L-ROT):", fontc[rots[0]] * (statc[0] == 2));
				printFont(3, 12, "D(HOLD) :", fontc[rots[0]] * (statc[0] == 3));
				printFont(3, 14, "GIVEUP  :", fontc[rots[0]] * (statc[0] == 4));
				printFont(3, 16, "PAUSE   :", fontc[rots[0]] * (statc[0] == 5));
				
				printFont(2, 28, "END(KEYBOARD): CANCEL", 9);
				
				j = 80 + ((statc[2] - 3) * 6);
				for(i = 0; i < statc[0]; i++) {
					sprintf(string[0], "%2d", ncfg[j]);
					printFont(13, 6 + i * 2, string[0], digitc[rots[0]]);
					j++;
				}

				if(statc[0] < 6) {
					printFont(13, 6 + statc[0] * 2, "_", digitc[rots[0]] * (count % 2));
					if(getPushState((statc[2] - 3), 1)) {
						PlaySE(5);
						ncfg[j] = -1;
						statc[0]++;
					} else {
						for(i=4; i<16; i++) {
							SelectJoyStick((statc[2] - 3));
							m = IsPushJoyKey(i);
							if(m) {
								PlaySE(5);
								ncfg[j] = i;
								statc[0]++;
								break;
							}
						}
					}
				} else {
					printFont(3, 22, "OK[ENTER] / RETRY[DEL] / CANCEL[BS]", digitc[rots[0]] * (count % 2));
					if(IsPushReturnKey()) {
				//	if(getPushState(pl, 1)) {
						PlaySE(10);
						j = 80 + (pl * 6);
						for(i = 0; i < 6; i++) {
							joykeyAssign[i+4+(4*pl)+(pl*6)] = ncfg[j];
							j++;
						}

						statc[0] = 0;
						statc[2] = 0;
					}
					if(IsPushDeleteKey()) {
				//	if(getPushState(pl, 0)) {
						PlaySE(5);
						j = 80 + (pl * 6);
						for(i = 0; i < 6; i++) {
							ncfg[j] = joykeyAssign[i+4+(4*pl)+(pl*6)];
							j++;
						}
						statc[0] = 0;
					}
					if(IsPushBSKey()) {
				//	if(getPushState(pl, 2) || getPushState(pl, 3)) {
						PlaySE(5);
						j = 80 + (pl * 6);
						for(i = 0; i < 6; i++) {
							ncfg[j] = joykeyAssign[i+4+(4*pl)+(pl*6)];
							j++;
						}
						statc[0] = 0;
						statc[2] = 0;
					}
				}
				
				// キーボードのENDで脱出 #1.60c7n4
				if( IsPushEndKey()) {
					PlaySE(5);
					j = 80 + (pl * 6);
					for(i = 0; i < 6; i++) {
						ncfg[j] = joykeyAssign[i+4];
						j++;
					}
					statc[0] = 0;
					statc[2] = 0;
				}
			} else if(statc[2] == 5) {
				// その他
				printFont(2,  3, "FUNCTION KEY SETTING", digitc[rots[0]]);
				printFont(3,  6, "GIVEUP KEY     :", fontc[rots[0]] * (statc[0] == 0));
				printFont(3,  8, "SNAPSHOT KEY   :", fontc[rots[0]] * (statc[0] == 1));
				printFont(3, 10, "PAUSE KEY 1P   :", fontc[rots[0]] * (statc[0] == 2));
				printFont(3, 12, "PAUSE KEY 2P   :", fontc[rots[0]] * (statc[0] == 3));
				printFont(3, 14, "DISPNEXT KEY 1P:", fontc[rots[0]] * (statc[0] == 4));
				printFont(3, 16, "DISPNEXT KEY 2P:", fontc[rots[0]] * (statc[0] == 5));

				j = 34;
				for(i = 0; i < statc[0]; i++) {
					sprintf(string[0], "%2X", ncfg[j]);
					printFont(20, 6 + i * 2, string[0], digitc[rots[0]]);
					if(i == 1) j = 55;
					else j++;
				}
				
				if(statc[0] < 6) {
					printFont(20, 6 + statc[0] * 2, "_", digitc[rots[0]] * (count % 2));
					for(i = 0; i < getMaxKey(); i++) {
						if(IsPushKey(i)) {
							PlaySE(5);
							ncfg[j] = i;
							statc[0]++;
							break;
						}
					}
				} else {
					printFont(3, 18, "OK[ENTER] / RETRY[DEL] / CANCEL[BS]", digitc[rots[0]] * (count % 2));
					if(IsPushReturnKey()) {
						PlaySE(10);
						giveupKey = ncfg[34];
						ssKey = ncfg[35];
						pausekey[0] = ncfg[55];
						pausekey[1] = ncfg[56];
						dispnextkey[0] = ncfg[57];
						dispnextkey[1] = ncfg[58];
						statc[0] = 0;
						statc[2] = 0;
					}
					if(IsPushDeleteKey()) {
						PlaySE(5);
						ncfg[34] = giveupKey;
						ncfg[35] = ssKey;
						ncfg[55] = pausekey[0];
						ncfg[56] = pausekey[1];
						ncfg[57] = dispnextkey[0];
						ncfg[58] = dispnextkey[1];
						statc[0] = 0;
					}
					if(IsPushBSKey()) {
						PlaySE(5);
						ncfg[34] = giveupKey;
						ncfg[35] = ssKey;
						ncfg[55] = pausekey[0];
						ncfg[56] = pausekey[1];
						ncfg[57] = dispnextkey[0];
						ncfg[58] = dispnextkey[1];
						statc[0] = 0;
						statc[2] = 0;
					}
				}
			} else if(statc[2] == 6) {
				// INPUT TEST #1.60c7n3
				printFont(2,  3, "INPUT TEST", digitc[rots[0]]);
				printFont(2, 28, "ENTER(KEYBOARD): EXIT", 9);
				
				for(pl=0; pl<2; pl++) {
					sprintf(string[0],"%dP", pl + 1);
					printFont(3, 6 + pl * 10, string[0], 2 - pl);
					
					for(i=0; i<=7; i++) {
						if(i == 0)      printFont(3, 7 + i + pl * 10, "UP            :", 0);
						else if(i == 1) printFont(3, 7 + i + pl * 10, "DOWN          :", 0);
						else if(i == 2) printFont(3, 7 + i + pl * 10, "LEFT          :", 0);
						else if(i == 3) printFont(3, 7 + i + pl * 10, "RIGHT         :", 0);
						else if(i == 4) printFont(3, 7 + i + pl * 10, "A(L/R     ROT):", 0);
						else if(i == 5) printFont(3, 7 + i + pl * 10, "B(R/L     ROT):", 0);
						else if(i == 6) printFont(3, 7 + i + pl * 10, "C(L/R/180 ROT):", 0);
						else if(i == 7) printFont(3, 7 + i + pl * 10, "D(HOLD)       :", 0);
						SelectJoyStick(pl);
						j = (IsPressKey(keyAssign[i + 10 * pl]) || IsPressJoyKey(joykeyAssign[i + 10 * pl]));	// キー入力状態取得
						if(j) sprintf(string[0],"d");
						else  sprintf(string[0],"c");
						printFont(19, 7 + i + pl * 10, string[0], j+1);
					}
				}
				
				// キーボードのENTERで終了
				if( IsPushReturnKey() ) {
					PlaySE(5);
					statc[0] = 0;
					statc[2] = 0;
				}
			} else if(statc[2] == 7) {
				// DISP ASSIGN #1.60c7n4
				printFont(2,  3, "DISP ASSIGN", digitc[rots[0]]);
				printFont(2, 28, "A/B: EXIT", 9);
				
				for(pl=0; pl<2; pl++) {
					sprintf(string[0],"%dP", pl + 1);
					printFont(3, 6 + pl * 10, string[0], 2 - pl);
					
					for(i=0; i<=7; i++) {
						if(i == 0)      printFont(3, 7 + i + pl * 10, "UP            :", 0);
						else if(i == 1) printFont(3, 7 + i + pl * 10, "DOWN          :", 0);
						else if(i == 2) printFont(3, 7 + i + pl * 10, "LEFT          :", 0);
						else if(i == 3) printFont(3, 7 + i + pl * 10, "RIGHT         :", 0);
						else if(i == 4) printFont(3, 7 + i + pl * 10, "A(L/R     ROT):", 0);
						else if(i == 5) printFont(3, 7 + i + pl * 10, "B(R/L     ROT):", 0);
						else if(i == 6) printFont(3, 7 + i + pl * 10, "C(L/R/180 ROT):", 0);
						else if(i == 7) printFont(3, 7 + i + pl * 10, "D(HOLD)       :", 0);
						
						sprintf(string[0],"%2X(JOY:%2d)", keyAssign[i + pl * 10], joykeyAssign[i + pl * 10]);
						printFont(19, 7 + i + pl * 10, string[0], 0);
					}
				}
				
				// A/B:戻る
				if(getPushState(0, 4) || getPushState(0, 5) || getPushState(1, 4) || getPushState(1, 5)) {
					PlaySE(5);
					statc[0] = 0;
					statc[2] = 0;
				}
			}
		}

		// exit setting menu
		else if(stat[0] == -1) break;
		spriteTime ();
	}
}

/* オプション #1.60c7n4 */
void GameOption() {
	int i, j, cursor;
	cursor = 0;
	
	loop {
		// 背景描画
		count++;
		if(background == 0) {
			for(i = 0; i <= 4; i++) {
				if(getDrawRate() == 1)
					BltFastRect(4, 96 * i - (count % 96) / 3, 0, 0, 0, 96, 240);
				else
					BltFastRect(4, 192 * i - (count % 32), 0, 0, 0, 192, 480);
			}
		} else if(background == 1) {
			for(i = 0; i <= 4; i++) {
				ExBltFastRect(4, 96 * i, 0, 0, 0, 96, 240);
			}
		} else {
			ExBltFast(30, 0, 0);
		}
		ExBltRect(77, count % 320, 16,  0, 28, 320 - (count % 320), 8);
		ExBltRect(77, 0, 16, 320 - (count % 320), 28, count % 320, 8);
		
		// メニュー描画
		printFont(1, 1, "OPTION", 4);
		
		printFont(1, 3 + cursor, "b", fontc[rots[0]]);
		
		sprintf(string[0],"DEBUG MODE        :");
		printFont(2, 3, string[0], (cursor == 0) * fontc[rots[0]]);
		if(debug == 0) sprintf(string[0], "OFF");
			else sprintf(string[0], "ON");
		printFont(22, 3, string[0], (cursor == 0) * fontc[rots[0]]);
		
		sprintf(string[0],"GRADE TYPE        : %d",enable_grade[0]);
		printFont(2, 4, string[0], (cursor == 1) * fontc[rots[0]]);
		
		sprintf(string[0],"IRS TYPE          :");
		printFont(2, 5, string[0], (cursor == 2) * fontc[rots[0]]);
		if(ace_irs == 0) sprintf(string[0], "CLASSIC");
		else if(ace_irs == 1)sprintf(string[0], "ACE");	
			else sprintf(string[0], "ACE+");
		printFont(22, 5, string[0], (cursor == 2) * fontc[rots[0]]);
		
		sprintf(string[0],"LANGUAGE          :");
		printFont(2, 6, string[0], (cursor == 3) * fontc[rots[0]]);
		if(english == 0) sprintf(string[0], "JAPANESE");
			else sprintf(string[0], "ENGLISH");
		printFont(22, 6, string[0], (cursor == 3) * fontc[rots[0]]);
		
		sprintf(string[0],"MINI SELECT       :");
		printFont(2, 7, string[0], (cursor == 4) * fontc[rots[0]]);
		if(mini_select == 0) sprintf(string[0], "OFF");
			else sprintf(string[0], "ON");
		printFont(22, 7, string[0], (cursor == 4) * fontc[rots[0]]);
		
		sprintf(string[0],"BIG MOVE TYPE     :");
		printFont(2, 8, string[0], (cursor == 5) * fontc[rots[0]]);
		if(bigtype == 0) sprintf(string[0], "1CELL");
		else if(bigtype == 1)sprintf(string[0], "2CELL");	
			else sprintf(string[0], "TOMOYO:1/OTHER:2");
		printFont(22, 8, string[0], (cursor == 5) * fontc[rots[0]]);
		
		sprintf(string[0],"ITEM INTERVAL     : %d",item_interval);
		printFont(2, 9, string[0], (cursor == 6) * fontc[rots[0]]);
		
		sprintf(string[0],"HIDE WAITS        :");
		printFont(2, 10,string[0], (cursor == 7) * fontc[rots[0]]);
		if(hide_wait == 0) sprintf(string[0], "NOT HIDE");
			else sprintf(string[0], "HIDE");
		printFont(22, 10, string[0], (cursor == 7) * fontc[rots[0]]);
		
		sprintf(string[0],"VS LIMIT TIME(SEC): %d",vs_time / 60);
		printFont(2, 11,string[0], (cursor == 8) * fontc[rots[0]]);
		
		sprintf(string[0],"MEDAL GRAPHICS    :");
		printFont(2, 12,string[0], (cursor == 9) * fontc[rots[0]]);
		if(medaltype == 0) sprintf(string[0], "f");//□
		else if(medaltype == 1) sprintf(string[0], "c");//○
			else sprintf(string[0], "OFF");
		printFont(22, 12, string[0], (cursor == 9) * fontc[rots[0]]);
		
		sprintf(string[0],"DEVIL RISE STARTLV: %d",p_shirase_start_level);
		printFont(2, 13,string[0], (cursor == 10) * fontc[rots[0]]);
		
		sprintf(string[0],"RISE TYPE         :");
		printFont(2, 14,string[0], (cursor == 11) * fontc[rots[0]]);
		if(uplinetype == 0) sprintf(string[0], "COPY");
		else if(uplinetype == 1)sprintf(string[0], "PATTERN");
		else if(uplinetype == 2)sprintf(string[0], "RANDOM");
			else sprintf(string[0], "REVERSE ERASE FIELD");
		printFont(22, 14, string[0], (cursor == 11) * fontc[rots[0]]);
		
		sprintf(string[0],"HOLD              :");
		printFont(2, 15,string[0], (cursor == 12) * fontc[rots[0]]);
		if(disable_hold == 0) sprintf(string[0], "ENABLE");
			else sprintf(string[0], "DISABLE");
		printFont(22, 15, string[0], (cursor == 12) * fontc[rots[0]]);
		
		sprintf(string[0],"IRS               :");
		printFont(2, 16,string[0], (cursor == 13) * fontc[rots[0]]);
		if(disable_irs == 0) sprintf(string[0], "ENABLE");
			else sprintf(string[0], "DISABLE");
		printFont(22, 16, string[0], (cursor == 13) * fontc[rots[0]]);
		
		sprintf(string[0],"USE CPU(1P)       :");
		printFont(2, 17,string[0], (cursor == 14) * fontc[rots[0]]);
		if(cp_player_1p == 0) sprintf(string[0], "OFF");
			else sprintf(string[0], "ON");
		printFont(22, 17, string[0], (cursor == 14) * fontc[rots[0]]);
		
		sprintf(string[0],"USE CPU(2P)       :");
		printFont(2, 18,string[0], (cursor == 15) * fontc[rots[0]]);
		if(cp_player_2p == 0) sprintf(string[0], "OFF");
			else sprintf(string[0], "ON");
		printFont(22, 18, string[0], (cursor == 15) * fontc[rots[0]]);
		
		sprintf(string[0],"CPU TYPE          :");
		printFont(2, 19,string[0], (cursor == 16) * fontc[rots[0]]);
		if(cp_type == 0) sprintf(string[0], "ERASE SOON");
			else sprintf(string[0], "ERASE STORE");
		printFont(22, 19, string[0], (cursor == 16) * fontc[rots[0]]);
		
		sprintf(string[0],"BLOCK ROTATE FRAME:");
		printFont(2, 20,string[0], (cursor == 17) * fontc[rots[0]]);
		if(block_rframe == 0) sprintf(string[0], "OFF");
			else sprintf(string[0], "ON");
		printFont(22, 20, string[0], (cursor == 17) * fontc[rots[0]]);
		
		sprintf(string[0],"WALL KICK         :");
		printFont(2, 21,string[0], (cursor == 18) * fontc[rots[0]]);
		if(disable_wallkick == 0) sprintf(string[0], "ENABLE");
			else sprintf(string[0], "DISABLE");
		printFont(22, 21, string[0], (cursor == 18) * fontc[rots[0]]);
		
		sprintf(string[0],"SHOW 1P CONTROL   :");
		printFont(2, 22,string[0], (cursor == 19) * fontc[rots[0]]);
		if(showctrl == 0) sprintf(string[0], "REPLAY ONLY");
		else if(showctrl == 1) sprintf(string[0], "ALWAYS ON");
		else sprintf(string[0], "ALWAYS OFF");
		printFont(22, 22, string[0], (cursor == 19) * fontc[rots[0]]);
		
		sprintf(string[0],"SPAWN Y TYPE      :");
		printFont(2, 23,string[0], (cursor == 20) * fontc[rots[0]]);
		if(spawn_y_type == 0) sprintf(string[0], "FIELD IN");
			else sprintf(string[0], "FIELD OUT");
		printFont(22, 23, string[0], (cursor == 20) * fontc[rots[0]]);
		
		sprintf(string[0],"HIDE INFO(TOMOYO) :");
		printFont(2, 24,string[0], (cursor == 21) * fontc[rots[0]]);
		if(hide_tomoyoinf == 0) sprintf(string[0], "NOT HIDE");
			else sprintf(string[0], "HIDE");
		printFont(22, 24, string[0], (cursor == 21) * fontc[rots[0]]);
		
		sprintf(string[0],"T-SPIN TYPE       :");
		printFont(2, 25,string[0], (cursor == 22) * fontc[rots[0]]);
		if(tspin_type == 0) sprintf(string[0], "NO CHECK");
		else if(tspin_type == 1) sprintf(string[0], "ERASE(NO BOUNS)");
		else if(tspin_type == 2)	sprintf(string[0], "3-C(BOUNS)");
		else if(tspin_type == 3)	sprintf(string[0], "BOTH(BONUS:3-C)");
		printFont(22, 25, string[0], (cursor == 22) * fontc[rots[0]]);
		
		sprintf(string[0],"BLOCK SPECTRUM    :");
		printFont(2, 26,string[0], (cursor == 23) * fontc[rots[0]]);
		if(block_spectrum == 0) sprintf(string[0], "OFF");
			else sprintf(string[0], "ON");
		printFont(22, 26, string[0], (cursor == 23) * fontc[rots[0]]);
		
		sprintf(string[0],"NEXT ADJUST       :");
		printFont(2, 27,string[0], (cursor == 24) * fontc[rots[0]]);
		if(next_adjust == 0) sprintf(string[0], "OFF");
			else sprintf(string[0], "ON(NOT FIRST OSZ)");
		printFont(22, 27, string[0], (cursor == 24) * fontc[rots[0]]);
		
		sprintf(string[0],"VIEW BEST TIME    :");
		printFont(2, 28,string[0], (cursor == 25) * fontc[rots[0]]);
		if(Isbesttime == 0) sprintf(string[0], "OFF");
			else sprintf(string[0], "ON");
		printFont(22, 28, string[0], (cursor == 25) * fontc[rots[0]]);
		
		sprintf(string[0],"BACK TO BACK      :");
		printFont(2, 29,string[0], (cursor == 26) * fontc[rots[0]]);
		if(b2bcheck == 0) sprintf(string[0], "OFF");
			else sprintf(string[0], "ON");
		printFont(22, 29, string[0], (cursor == 26) * fontc[rots[0]]);
		// キー入力
		KeyInput();
		
		padRepeat2(0);
		// ↑
		if( (mpc2[0] == 1) || ((mpc2[0] > tame3) && (mpc2[0] % tame4 == 0)) )
		if( getPressState(0, 0) ) {
			PlaySE(5);
			cursor--;
			if(cursor < 0) cursor = 26;
		}
		// ↓
		if( (mpc2[0] == 1) || ((mpc2[0] > tame3) && (mpc2[0] % tame4 == 0)) )
		if( getPressState(0, 1) ) {
			PlaySE(5);
			cursor++;
			if(cursor > 26) cursor = 0;
		}
		
		padRepeat(0);
		// ←
		if((mpc[0] == 1) || ((mpc[0] > tame1) && (mpc[0] % tame2 == 0)) || (getPressState(0, 6)))
		if( getPressState(0, 2) ) {
			PlaySE(3);
			
			// デバッグモード
			if(cursor == 0) {
				debug = !debug;
			}
			// 段位タイプ
			if(cursor == 1) {
				enable_grade[0]--;
				if(enable_grade[0] < 0) enable_grade[0] = 4;
			}			
			//IRS
			if(cursor == 2) {
				ace_irs--;
				if(ace_irs < 0) ace_irs = 2;
			}
			// 英語版
			if(cursor == 3) {
				english = !english;
			}
			// 簡易セレクト
			if(cursor == 4) {
				mini_select = !mini_select;
			}
			// BIG移動単位
			if(cursor == 5) {
				bigtype--;
				if(bigtype < 0) bigtype = 2;
			}
			// アイテム間隔
			if(cursor == 6) {
				item_interval--;
				if(item_interval < 1) item_interval = 50;
			}
			// WAIT値表示
			if(cursor == 7) {
				hide_wait =!hide_wait;
			}
			// VS制限時間
			if(cursor == 8) {
				vs_time = vs_time - 1800;
				if(vs_time < 0) vs_time = 36000;
			}
			// メダルタイプ
			if(cursor == 9) {
				medaltype--;
				if(medaltype < 0) medaltype = 2;
			}
			// せり上がり開始Lv
			if(cursor == 10) {
				p_shirase_start_level = p_shirase_start_level - 50;
				if(p_shirase_start_level < 0) p_shirase_start_level = 1000;
			}
			// せり上げタイプ
			if(cursor == 11) {
				uplinetype--;
				if(uplinetype < 0) uplinetype = 2;
			}
			// HOLD封印
			if(cursor == 12) {
				disable_hold = !disable_hold;
			}
			// IRS封印
			if(cursor == 13) {
				disable_irs = !disable_irs;
			}
			// CPU 1P
			if(cursor == 14) {
				cp_player_1p = !cp_player_1p;
			}
			// CPU 2P
			if(cursor == 15) {
				cp_player_2p = !cp_player_2p;
			}
			// CPUの性格
			if(cursor == 16) {
				cp_type = !cp_type;
			}
			// ブロックの周りの四角い枠
			if(cursor == 17) {
				block_rframe = !block_rframe;
			}
			// 壁蹴り封印
			if(cursor == 18) {
				disable_wallkick = !disable_wallkick;
			}
			// 操作状況表示
			if(cursor == 19) {
				showctrl--;
				if(showctrl < 0) showctrl = 2;
			}
			// 出現位置
			if(cursor == 20) {
				spawn_y_type = !spawn_y_type;
			}
			// TOMOYO詳細
			if(cursor == 21) {
				hide_tomoyoinf = !hide_tomoyoinf;
			}
			// T-SPIN判定方法
			if(cursor == 22) {
				tspin_type--;
				if(tspin_type < 0) tspin_type = 3;
			}
			// ブロックの残像
			if(cursor == 23) {
				block_spectrum = !block_spectrum;
			}
			//第一ブロック調整
			if(cursor == 24) {
				next_adjust = !next_adjust;
			}
			// ベストタイムを見せるか
			if(cursor == 25) {
				Isbesttime = !Isbesttime;
			}
			// Back to Back
			if(cursor == 26) {
				b2bcheck = !b2bcheck;
			}
		}
		
		// →
		if((mpc[0] == 1) || ((mpc[0] > tame1) && (mpc[0] % tame2 == 0)) || (getPressState(0, 6)))
		if( getPressState(0, 3) ) {
			PlaySE(3);
			
			// デバッグ
			if(cursor == 0) {
				debug = !debug;
			}
			// 段位
			if(cursor == 1) {
				enable_grade[0]++;
				if(enable_grade[0] > 4) enable_grade[0] = 0;
			}
			// IRS
			if(cursor == 2) {
				ace_irs++;
				if(ace_irs > 2) ace_irs = 0;
			}
			// 英語版
			if(cursor == 3) {
				english = !english;
			}
			// 簡易セレクト
			if(cursor == 4) {
				mini_select = !mini_select;
			}
			// BIG移動単位
			if(cursor == 5) {
				bigtype++;
				if(bigtype > 2) bigtype = 0;
			}
			// アイテム間隔
			if(cursor == 6) {
				item_interval++;
				if(item_interval > 50) item_interval = 1;
			}
			// WAIT値表示
			if(cursor == 7) {
				hide_wait =!hide_wait;
			}
			// VS制限時間
			if(cursor == 8) {
				vs_time = vs_time + 1800; //30秒
				if(vs_time > 36000) vs_time = 0;
			}
			// 操作状況表示
			if(cursor == 9) {
				medaltype++;
				if(medaltype > 2) medaltype = 0;
			}
			// せり上がり開始Lv
			if(cursor == 10) {
				p_shirase_start_level = p_shirase_start_level + 50;
				if(p_shirase_start_level > 1000) p_shirase_start_level = 0;
			}
			// せり上げタイプ
			if(cursor == 11) {
				uplinetype++;
				if(uplinetype > 2) uplinetype = 0;
			}
			// HOLD封印
			if(cursor == 12) {
				disable_hold = !disable_hold;
			}
			// IRS封印
			if(cursor == 13) {
				disable_irs = !disable_irs;
			}
			// CPU 1P
			if(cursor == 14) {
				cp_player_1p = !cp_player_1p;
			}
			// CPU 2P
			if(cursor == 15) {
				cp_player_2p = !cp_player_2p;
			}
			// CPUの性格
			if(cursor == 16) {
				cp_type = !cp_type;
			}
			// ブロックの周りの四角い枠
			if(cursor == 17) {
				block_rframe = !block_rframe;
			}
			// 壁蹴り封印
			if(cursor == 18) {
				disable_wallkick = !disable_wallkick;
			}
			// 操作状況表示
			if(cursor == 19) {
				showctrl++;
				if(showctrl > 2) showctrl = 0;
			}
			// 出現位置
			if(cursor == 20) {
				spawn_y_type = !spawn_y_type;
			}
			// ともよ詳細
			if(cursor == 21) {
				hide_tomoyoinf = !hide_tomoyoinf;
			}
			// T-SPIN判定方法
			if(cursor == 22) {
				tspin_type++;
				if(tspin_type > 3) tspin_type= 0;
			}
			// ブロックの残像
			if(cursor == 23) {
				block_spectrum = !block_spectrum;
			}
			//第一ブロック調整
			if(cursor == 24) {
				next_adjust = !next_adjust;
			}
			// ベストタイムを見せるか
			if(cursor == 25) {
				Isbesttime = !Isbesttime;
			}
			// Back to Back
			if(cursor == 26) {
				b2bcheck = !b2bcheck;
			}
		}
		
		// AかBで戻る
		if( getPushState(0, 4) || getPushState(0, 5) ) {
			backupSetups();
			return;
		}
		
		spriteTime();
	}
}
