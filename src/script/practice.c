//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.09 - PRACTICEモードメニュー
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲

#include "../script/include.h"

void statSelectLevel(int32_t player) {
	int32_t i;
	int32_t color;

	color = 1 + p_shirase[0]; // SHIRASEモードで赤くする#1.60c6.2c

	// HOLDボタンでページ切り替え #1.60c7a
	if(getPushState(player, 7)) {
		PlayWave(5);
		if(vslevel[0] < 14)
			vslevel[0] = 14;
		else if(vslevel[0] < 29)	// 3ページ目 #1.60c7j5
			vslevel[0] = 29;
		else
			vslevel[0] = 0;
	}
		// 現在のページ番号を表示 C7T3
		printFont(15 + 24 * player - 12 * maxPlay,25 , "kn", count % 9);
		sprintf(string[0], "PAGE %d/3",1+(1*(vslevel[0]>=14))+(1*(vslevel[0]>=29)));
		printFont(18 + 24 * player - 12 * maxPlay,25 , string[0], 0);
		printFont(18 + 24 * player - 12 * maxPlay,26 , "C:RAPID", 0);

	if(vslevel[0] < 14) { // 1ページ目
		// START! BGM変更できるように#1.60c6.2d
		// 4番以降を選べるようにした#1.60c7f5
		printFont(15 - 12 * maxPlay, 5, "BGM", color);
		sprintf(string[0], "%2d", p_bgmlv);
		printFont(23 - 12 * maxPlay, 5, string[0], count % 9 * (vslevel[0] == 0));

		// 回転法則
		printFont(15 - 12 * maxPlay, 6, "ROT", color);
		if(rots[0] == 0) {
			printFont(18 - 12 * maxPlay, 6, "HEBORIS", count % 9 * (vslevel[0] == 1));
		} else if(rots[0] == 1) {
			printFont(19 - 12 * maxPlay, 6, "TI-ARS", count % 9 * (vslevel[0] == 1));
		} else if(rots[0] == 2) {
			printFont(20 - 12 * maxPlay, 6, "WORLD", count % 9 * (vslevel[0] == 1));
		} else if(rots[0] == 3) {
			printFont(18 - 12 * maxPlay, 6, "ACE-SRS", count % 9 * (vslevel[0] == 1));
		} else if(rots[0] == 4) {
			printFont(18 - 12 * maxPlay, 6, "ACE-ARS", count % 9 * (vslevel[0] == 1));
		} else if(rots[0] == 5) {
			printFont(21 - 12 * maxPlay, 6, "ARS2", count % 9 * (vslevel[0] == 1));
		} else if(rots[0] == 6) {
			printFont(19 - 12 * maxPlay, 6, "DS-SRS", count % 9 * (vslevel[0] == 1));
		} else if(rots[0] == 7) {
			printFont(20 - 12 * maxPlay, 6, "SRS-X", count % 9 * (vslevel[0] == 1));
		} else if(rots[0] == 8) {
			printFont(20 - 12 * maxPlay, 6, "D.R.S", count % 9 * (vslevel[0] == 1));
		}

		printFont(15 - 12 * maxPlay, 7, "ROTS.+", color);
		if(death_plus[0])
			printFont(23 - 12 * maxPlay, 7, "ON", count % 9 * (vslevel[0] == 2));
		else
			printFont(22 - 12 * maxPlay, 7, "OFF", count % 9 * (vslevel[0] == 2));

		// WAITS
		printFont(15 - 12 * maxPlay, 8, "WAITS", color);

		sprintf(string[0], "W1:%2d", wait1[player]);//SYUTUGEN
		printFont(15 - 12 * maxPlay, 9, string[0], count % 9 * (vslevel[player] == 3));
		sprintf(string[0], "W2:%2d", wait2[player]);//SYOUKYO
		printFont(20 - 12 * maxPlay, 9, string[0], count % 9 * (vslevel[player] == 4));
		sprintf(string[0], "W3:%2d", wait3[player]);//SETTYAKU
		printFont(15 - 12 * maxPlay, 11, string[0], count % 9 * (vslevel[player] == 5));
		sprintf(string[0], "WT:%2d", waitt[player]);//YOKOTAME
		printFont(20 - 12 * maxPlay, 11, string[0], count % 9 * (vslevel[player] == 6));

			//それぞれ項目名表示
		if(!english){
			if(vslevel[player] == 3)
				printFont(15 - 12 * maxPlay, 10, "SYUTUGEN",    8);
			else if(vslevel[player] == 4)
				printFont(15 - 12 * maxPlay, 10, "SYOUKYO",     8);
			else if(vslevel[player] == 5)
				printFont(15 - 12 * maxPlay, 12, "SETTYAKU",    8);
			else if(vslevel[player] == 6)
				printFont(15 - 12 * maxPlay, 12, "YOKOTAME",   8);
		} else {
			if(vslevel[player] == 3)
				printFont(15 - 12 * maxPlay, 10, "ARE",    8);
			else if(vslevel[player] == 4)
				printFont(15 - 12 * maxPlay, 10, "LINE CLEAR",     8);
			else if(vslevel[player] == 5)
				printFont(15 - 12 * maxPlay, 12, "LOCK DELAY",    8);
			else if(vslevel[player] == 6)
				printFont(15 - 12 * maxPlay, 12, "       DAS",   8);
		}
		// ブロック落下スピード(1200 = 20G)
		printFont(15 - 12 * maxPlay, 13, "SPEED", count % 9 * (vslevel[0] == 7));
		sprintf(string[0], "%7d", sp[0]);
		printFont(18 - 12 * maxPlay, 13, string[0], count % 9 * (vslevel[0] == 7));

		printFont(15 - 12 * maxPlay, 14, "OTHERS", color);

		// シングル台での表示ずれ修正#1.60c6.1a

		// HIDDEN
		sprintf(string[0], "HIDDEN  %2d", hidden[0]);
		if(hidden[player] <= 7){
			printFont(15 - 12 * maxPlay, 15, string[0], count % 9 * (vslevel[0] == 8));
		}else if(hidden[player] == 8){
			printFont(15 - 12 * maxPlay, 15, "HIDDEN   M", count % 9 * (vslevel[0] == 8));
		}else if(hidden[player] == 9){
			printFont(15 - 12 * maxPlay, 15, "HIDDEN UM3", count % 9 * (vslevel[0] == 8));
		}else if(hidden[player] == 10){
			printFont(15 - 12 * maxPlay, 15, "HIDDEN UM2", count % 9 * (vslevel[0] == 8));
		}else if(hidden[player] == 11){
			printFont(15 - 12 * maxPlay, 15, "HIDDEN UM1", count % 9 * (vslevel[0] == 8));
		}

		// エンディングのテスト#1.60cf
		if(p_ending == 0)
			printFont(15 - 12 * maxPlay, 16, "ENDING   e", count % 9 * (vslevel[0] == 9));
		else if(p_ending == 1)
			printFont(15 - 12 * maxPlay, 16, "ENDING 200", count % 9 * (vslevel[0] == 9));
		else if(p_ending == 2)
			printFont(15 - 12 * maxPlay, 16, "ENDING 999", count % 9 * (vslevel[0] == 9));
		else if(p_ending == 3)
			printFont(15 - 12 * maxPlay, 16, "ENDING999M", count % 9 * (vslevel[0] == 9));
		else if(p_ending == 4)
			printFont(15 - 12 * maxPlay, 16, "ENDING1300", count % 9 * (vslevel[0] == 9));
		else if(p_ending == 5)
			printFont(15 - 12 * maxPlay, 16, "ENDINGFAST", count % 9 * (vslevel[0] == 9));

		// FAVORITES (from hogeパッチ)
		printFont(15 - 12 * maxPlay, 17, "SP PRESET", color);
		printFont(15 - 12 * maxPlay, 18, waitname[p_setting], count % 9 * (vslevel[0] == 10));

		// Finishtype
		printFont(15 - 12 * maxPlay, 19, "GOAL TYPE", color);
		if(p_goaltype == 0)
			printFont(15 - 12 * maxPlay, 20, "ENDLESS", count % 9 * (vslevel[0] == 11));
		else if(p_goaltype == 1)
			printFont(15 - 12 * maxPlay, 20, "LEVEL", count % 9 * (vslevel[0] == 11));
		else if(p_goaltype == 2)
			printFont(15 - 12 * maxPlay, 20, "LINES", count % 9 * (vslevel[0] == 11));
		else if(p_goaltype == 3)
			printFont(15 - 12 * maxPlay, 20, "BLOCK", count % 9 * (vslevel[0] == 11));
		else if(p_goaltype == 4)
			printFont(15 - 12 * maxPlay, 20, "TIME", count % 9 * (vslevel[0] == 11));

		//それぞれ終了タイプの項目名表示
		//表示するレベルに変換
		if(p_goaltype==0){
		printFont(15 - 12 * maxPlay, 21, "NO OPTION", count % 9 * (vslevel[0] == 12));
		}else if(p_goaltype==1){
		sprintf(string[0], "LEVEL %3d", p_goaltypenumlist[p_goaltypenum]*10);
		printFont(15 - 12 * maxPlay, 21, string[0], count % 9 * (vslevel[0] == 12));
		}else if(p_goaltype==2){
		sprintf(string[0], "%3dLINES", p_goaltypenumlist[p_goaltypenum]);
		printFont(15 - 12 * maxPlay, 21, string[0], count % 9 * (vslevel[0] == 12));
		}else if(p_goaltype==3){
		sprintf(string[0], "%3dBLOCK", p_goaltypenumlist[p_goaltypenum]);
		printFont(15 - 12 * maxPlay, 21, string[0], count % 9 * (vslevel[0] == 12));
		}else if(p_goaltype==4){
		sprintf(string[0], "%3dSEC", p_goaltypenumlist[p_goaltypenum]*2);
		printFont(15 - 12 * maxPlay, 21, string[0], count % 9 * (vslevel[0] == 12));
		}

		// ITMブロック #1.60c7e // playerごと #1.60c7h8
		printFont(15 - 12 * maxPlay, 22, "ITEM", color);
		if(p_item_mode)
			printFont(23 - 12 * maxPlay, 22, "ON", count % 9 * (vslevel[0] == 13));
		else
			printFont(22 - 12 * maxPlay, 22, "OFF", count % 9 * (vslevel[0] == 13));

	} else if(vslevel[0] <29) {	// 2ページ目 #1.60c7j5
		printFont(15 - 12 * maxPlay, 5, "LINE UP", color);
		printFont(15 - 12 * maxPlay, 6, "MODE", count % 9 * (vslevel[0] == 14));
		if(p_shirase[0])
			printFont(23 - 12 * maxPlay, 6, "ON", count % 9 * (vslevel[0] == 14));
		else
			printFont(22 - 12 * maxPlay, 6, "OFF", count % 9 * (vslevel[0] == 14));

		printFont(15 - 12 * maxPlay, 7, "TYPE", count % 9 * (vslevel[0] == 15));
		if(upLineT[0] == 0)
			printFont(21 - 12 * maxPlay, 7, "COPY", count % 9 * (vslevel[0] == 15));
		else if(upLineT[0] == 1)
			printFont(20 - 12 * maxPlay, 7, "HEBO+", count % 9 * (vslevel[0] == 15));
		else if(upLineT[0] == 2)
			printFont(19 - 12 * maxPlay, 7, "RANDOM", count % 9 * (vslevel[0] == 15));

		sprintf(string[0], "LINE(S) %2d", raise_shirase_lines);
		printFont(15 - 12 * maxPlay, 8, string[0], count % 9 * (vslevel[0] == 16));

		sprintf(string[0], "INTER. %3d", raise_shirase_interval);
		printFont(15 - 12 * maxPlay, 9, string[0], count % 9 * (vslevel[0] == 17));

		// 背景 #1.60c7o9
		printFont(15 - 12 * maxPlay, 10, "BACKGROUND", color);
		sprintf(string[0],"NO.%d",p_backno);
		printFont(15 - 12 * maxPlay, 11, string[0], count % 9 * (vslevel[0] == 18));

		// TLSをレベルアップタイプと切り離す #1.60c7i2
		printFont(15 - 12 * maxPlay, 12, "TLS", color);
		if(tlsrange == 0) {
			printFont(22 - 12 * maxPlay, 12, "OFF", count % 9 * (vslevel[0] == 19));
		} else if(tlsrange == 1) {
			// PRACTICEでもT.L.S.を延長したため表記変更#1.60c6.2f
			printFont(20 - 12 * maxPlay, 12, "LV100", count % 9 * (vslevel[0] == 19));
		} else {
			printFont(19 - 12 * maxPlay, 12, "ALWAYS", count % 9 * (vslevel[0] == 19));
		}
		printFont(15 - 12 * maxPlay, 14, "GIMMICK", color);
		printFont(15 - 12 * maxPlay, 15, "BIG", color);
		if(IsBigStart[0])
			printFont(23 - 12 * maxPlay, 15, "ON", count % 9 * (vslevel[0] == 20));
		else
			printFont(22 - 12 * maxPlay, 15, "OFF", count % 9 * (vslevel[0] == 20));

		// []ブロック #1.60c7e // playerごと #1.60c7h8
		printFont(15 - 12 * maxPlay, 16, "[] BLK", color);
		if(p_over1000)
			printFont(23 - 12 * maxPlay, 16, "ON", count % 9 * (vslevel[0] == 21));
		else
			printFont(22 - 12 * maxPlay, 16, "OFF", count % 9 * (vslevel[0] == 21));

		// フィールドミラー #1.60c7j5
		printFont(15 - 12 * maxPlay, 17, "MIRROR", color);
		if(fmirror) {
			printFont(24 - 12 * maxPlay, 17, "c", count % 9 * (vslevel[0] == 22));
		} else {
			printFont(24 - 12 * maxPlay, 17, "e", count % 9 * (vslevel[0] == 22));
		}

		// ロールロール #1.60c7j5
		printFont(15 - 12 * maxPlay, 18, "ROLLROLL", color);
		if(rollroll) {
			printFont(24 - 12 * maxPlay, 18, "c", count % 9 * (vslevel[0] == 23));
		} else {
			printFont(24 - 12 * maxPlay, 18, "e", count % 9 * (vslevel[0] == 23));
		}

		// X-RAY #1.60c7p9ex
		printFont(15 - 12 * maxPlay, 19, "X-RAY", color);
		if(xray) {
			printFont(24 - 12 * maxPlay, 19, "c", count % 9 * (vslevel[0] == 24));
		} else {
			printFont(24 - 12 * maxPlay, 19, "e", count % 9 * (vslevel[0] == 24));
		}

		// COLOR #1.60c7p9ex
		printFont(15 - 12 * maxPlay, 20, "COLOR", color);
		if(fcolor) {
			printFont(24 - 12 * maxPlay, 20, "c", count % 9 * (vslevel[0] == 25));
		} else {
			printFont(24 - 12 * maxPlay, 20, "e", count % 9 * (vslevel[0] == 25));
		}
		// BLIND
		printFont(15 - 12 * maxPlay, 21, "BLIND", color);
		if(p_blind) {
			printFont(24 - 12 * maxPlay, 21, "c", count % 9 * (vslevel[0] == 26));
		} else {
			printFont(24 - 12 * maxPlay, 21, "e", count % 9 * (vslevel[0] == 26));
		}
		// thunder
		printFont(15 - 12 * maxPlay, 22, "THUNDER", color);
		if(thunder){
			printFont(24 - 12 * maxPlay, 22, "c", count % 9 * (vslevel[0] == 27));
		} else {
			printFont(24 - 12 * maxPlay, 22, "e", count % 9 * (vslevel[0] == 27));
		}
		// 初期状態フィールド #1.60c7f6
		printFont(15 - 12 * maxPlay, 23, "INIT FLD", color);
		if(p_stage==-1) printFont(24 - 12 * maxPlay, 24, "e", count % 9 * (vslevel[0] == 28));
		else{
			sprintf(string[0],"%3d",p_stage+1);
			printFont(22 - 12 * maxPlay, 24, string[0], count % 9 * (vslevel[0] == 28));
		}

	} else {	// 3ページ目 #1.60c7j5
			// 初手に黄色、紫、緑があるか #1.60c7f5
		printFont(15 - 12 * maxPlay, 5, "NEXT", color);
		printFont(15 - 12 * maxPlay, 6, "NEXTADJUST", color);
		if(p_next_adjust){
			printFont(23 - 12 * maxPlay, 7, "ON", count % 9 * (vslevel[0] == 29));
			printFont(15 - 12 * maxPlay, 8, "NOT1ST OSZ", count % 9 * (vslevel[0] == 29));
		}else{
			printFont(22 - 12 * maxPlay, 7, "OFF", count % 9 * (vslevel[0] == 29));
		}

		// NEXT #1.60c7f6
		printFont(15 - 12 * maxPlay,9, "NEXT BLOCK", color);
		sprintf(string[0], "NEXT1    %d", next[0]);
		printFont(15 - 12 * maxPlay, 10, string[0], count % 9 * (vslevel[0] == 30));
		sprintf(string[0], "NEXT2    %d", nextb[(nextc[0] + 1) % 1400]);
		printFont(15 - 12 * maxPlay, 11, string[0], count % 9 * (vslevel[0] == 31));
		sprintf(string[0], "NEXT3    %d", nextb[(nextc[0] + 2) % 1400]);
		printFont(15 - 12 * maxPlay, 12, string[0], count % 9 * (vslevel[0] == 32));
		sprintf(string[0], "NEXT4    %d", nextb[(nextc[0] + 3) % 1400]);
		printFont(15 - 12 * maxPlay, 13, string[0], count % 9 * (vslevel[0] == 33));
		sprintf(string[0], "NEXT5    %d", nextb[(nextc[0] + 4) % 1400]);
		printFont(15 - 12 * maxPlay, 14, string[0], count % 9 * (vslevel[0] == 34));
		sprintf(string[0], "NEXT6    %d", nextb[(nextc[0] + 5) % 1400]);
		printFont(15 - 12 * maxPlay, 15, string[0], count % 9 * (vslevel[0] == 35));


		// HOLD #1.60c7f6
		printFont(15 - 12 * maxPlay, 17, "HOLD", color);
		// -1(HOLDなし)の場合は×と表示
		if(hold[0] == -1) {
			printFont(24 - 12 * maxPlay, 17, "e", count % 9 * (vslevel[0] == 36));
		} else {
			sprintf(string[0], "%d", hold[0]);
			printFont(24 - 12 * maxPlay, 17, string[0], count % 9 * (vslevel[0] == 36));
		}

		// ツモ読み込み #1.60c7g3
		printFont(15 - 12 * maxPlay, 18, "NEXT PTN", color);
		if(p_nextblock==0){
			printFont(19 - 12 * maxPlay, 19, "RANDOM", count % 9 * (vslevel[0] == 37));
		}else if((p_nextblock>1)&&(p_nextblock<8)){
			sprintf(string[0], "HEBO%d", p_nextblock);
			printFont(20 - 12 * maxPlay, 19, string[0], count % 9 * (vslevel[0] == 37));
		}else if(p_nextblock==1){
			printFont(18 - 12 * maxPlay, 19, "MEMORY1", count % 9 * (vslevel[0] == 37));
		}else if(p_nextblock==8){
			printFont(18 - 12 * maxPlay, 19, "MEMORY4", count % 9 * (vslevel[0] == 37));
		}else if(p_nextblock==9){
			printFont(16 - 12 * maxPlay, 19, "GUIDELINE", count % 9 * (vslevel[0] == 37));
		}else if(p_nextblock==10){
			printFont(19 - 12 * maxPlay, 19, "DENGEN", count % 9 * (vslevel[0] == 37));
		}else if(p_nextblock==11){
			printFont(15 - 12 * maxPlay, 19, "TOMOYO PTN", count % 9 * (vslevel[0] == 37));
		}else if(p_nextblock==12){
			printFont(19 - 12 * maxPlay, 19, "FP PTN", count % 9 * (vslevel[0] == 37));
		}

		// ツモ送りの有無 #1.60c7j9
		printFont(15 - 12 * maxPlay, 20, "NEXT PASS", color);	// Y座標ズレ修正 #1.60c7p9ex改造
		if(p_nextpass) {
			printFont(24 - 12 * maxPlay, 20, "c", count % 9 * (vslevel[0] == 38));
		} else {
			printFont(24 - 12 * maxPlay, 20, "e", count % 9 * (vslevel[0] == 38));
		}

		// NEXTC
		printFont(15 - 12 * maxPlay, 21, "NEXTC", color);
		sprintf(string[0], "%4d", nextc[0]);
		printFont(21 - 12 * maxPlay, 21, string[0], count % 9 * (vslevel[0] == 39));

		// square
		printFont(15 - 12 * maxPlay, 22, "SQUARE", color);
		if(squaremode[0]) {
			printFont(24 - 12 * maxPlay, 22, "c", count % 9 * (vslevel[0] == 40));
		} else {
			printFont(24 - 12 * maxPlay, 22, "e", count % 9 * (vslevel[0] == 40));
		}

		// heboGB
		if(!p_heboGBlv){
			printFont(15 - 12 * maxPlay, 23, "OLDSTYLE e", count % 9 * (vslevel[0] == 41));
		}else{
			sprintf(string[0], "OLDSTYLE%2d", p_heboGBlv);
			printFont(15 - 12 * maxPlay, 23, string[0], count % 9 * (vslevel[0] == 41));
		}

		// FPS #1.60c7n7
		printFont(15 - 12 * maxPlay, 24, "FPS", color);			// Y座標ズレ修正 #1.60c7p9ex改造
		sprintf(string[0], "%2d", max_fps);
		printFont(23 - 12 * maxPlay, 24, string[0], count % 9 * (vslevel[0] == 42));

	}

	padRepeat2(0);
	// ↑
	if(((mpc2[0] == 1) || ((mpc2[0] > tame3) && (mpc2[0] % tame4 == 0))) && !(mpc2[0] == statusc[1])){
		if(getPressState(player, 0)) {
			PlaySE(5);

			vslevel[0]--;

			if(vslevel[0] < 0) vslevel[0] = 42;	// #1.60c7p9ex改造
		}
	}

	// ↓
	if(((mpc2[0] == 1) || ((mpc2[0] > tame3) && (mpc2[0] % tame4 == 0))) && !(mpc2[0] == statusc[1])){
		if(getPressState(player, 1)) {
			PlaySE(5);

			vslevel[0]++;
			if(vslevel[0] > 42) vslevel[0] = 0;	// #1.60c7p9ex改造
		}
	}
	padRepeat(0);

	// ← (Cボタンを押しながらだと高速に数値を変更します)
	if((mpc[0] == 1) || ((mpc[0] > tame1) && (mpc[0] % tame2 == 0)) || (getPressState(player, 6))){
	if(getPressState(player, 2)) {
		PlaySE(3);
		// 数値変更(手抜き)

		// BGM#1.60c6.2d
		if(vslevel[0] == 0) {
			p_bgmlv--;
			if(p_bgmlv < 0) p_bgmlv = 19;
		}
		// 回転法則
		if(vslevel[0] == 1) {
			rots[0]--;
			if(rots[0] < 0) rots[0] = 8;
			setNextBlockColors(0, 1);
			if(hold[0] != -1){		// HOLDブロックの色設定
				if( isWRule(player) ) {
					// ワールド
					c_hblk[0] = wcol[ hold[0] ];
				} else if( (rots[player] >= 4)&&(rots[player] != 8) ) {
					// ARS
					c_hblk[0] = acol[ hold[0] ];
				} else {
					// クラシック
					c_hblk[0] = ccol[ hold[0] ];
				}
			}
		}
		// death+
		if(vslevel[0] == 2) {
			death_plus[0] = !death_plus[0];
		}

		// 出現
		if(vslevel[0] == 3) {
			wait1[0]--;
			if(wait1[0] < 0) wait1[0] = 99;
		}
		// 消去
		if(vslevel[0] == 4) {
			wait2[0]--;
			if(wait2[0] < 0) wait2[0] = 99;
		}
		// 接着
		if(vslevel[0] == 5) {
			wait3[0]--;
			if(wait3[0] < 0) wait3[0] = 99; // 0以下だったら99に
		}
		// 横溜め
		if(vslevel[0] == 6) {
			waitt[0]--;
			if(waitt[0] < 0) waitt[0] = 99;
		}
		// 落下スピード
		if(vslevel[0] == 7) {
			sp[0]--;
			if(sp[0] < 0) sp[0] = 1200; // 0以下だったら1200に
		}
		// HIDDEN
		if(vslevel[0] == 8) {
			hidden[0]--;
			if(hidden[0] < 0) hidden[0] = 11;
		}
		// ENDING #1.60c7k7
		if(vslevel[0] == 9) {
			p_ending--;
			if(p_ending < 0) p_ending = 5;
		}
		// FAVORITES
		if(vslevel[0] == 10) {
			p_setting--;
			if(p_setting < 0) p_setting = skip_fwait;
		}
		// FINISHTYPE #1.60c7k7
		if(vslevel[0] == 11) {
			p_goaltype--;
			if(p_goaltype < 0) p_goaltype  = 4;
		}
		// FINISHTYPE #1.60c7k7
		if(vslevel[0] == 12) {
			p_goaltypenum--;
			if(p_goaltypenum < 0) p_goaltypenum  = 9;
		}

		// ITEM
		if(vslevel[0] == 13){
			p_item_mode = !p_item_mode;
		}

		// SERIAGARIKA?
		if(vslevel[0] == 14) {
			p_shirase[0] = !p_shirase[0];
		}
		if(vslevel[0] == 15) {
			upLineT[0]--;
			if(upLineT[0] < 0) upLineT[0] = 2;
		}
		if(vslevel[0] == 16) {
			raise_shirase_lines--;
			if(raise_shirase_lines < 1) raise_shirase_lines = 12;
		}
		if(vslevel[0] == 17) {
			raise_shirase_interval--;
			if(raise_shirase_interval < 2) raise_shirase_interval = 100;
		}
		// 背景 #1.60c7o9
		if(vslevel[0] == 18) {
			p_backno--;
			if(p_backno < 0) p_backno = 11;
			backno = p_backno;
		}
		// TLS
		if(vslevel[0] == 19) {
			tlsrange--;
			if(tlsrange < 0) tlsrange = 2;	// TLSをレベルアップタイプと切り離す #1.60c7i2
		}
		// BIG
		if(vslevel[0] == 20) {
			IsBigStart[0] = !IsBigStart[0];
		}
		// []ブロック #1.60c7e
		if(vslevel[0] == 21) {
			p_over1000 = !p_over1000;
			setNextBlockColors(0, 1);
		}
				// ミラー #1.60c7j5
		if(vslevel[0] == 22) {
			fmirror = !fmirror;
		}
		// ロールロール #1.60c7j5
		if(vslevel[0] == 23) {
			rollroll = !rollroll;
		}
		// X-RAY #1.60c7p9ex
		if(vslevel[0] == 24) {
			xray = !xray;
		}
		// COLOR #1.60c7p9exthunder
		if(vslevel[0] == 25) {
			fcolor = !fcolor;
		}
		// ブラインド
		if(vslevel[0] == 26) {
			p_blind = !p_blind;
		}
		// サンダー
		if(vslevel[0] == 27) {
			thunder = !thunder;
		}
		// 初期状態フィールド #1.60c7f6
		if(vslevel[0] == 28) {
			//use_fld = !use_fld;
			p_stage--;
			if(p_stage < -1) p_stage = 199;
			loadTomoyoStage(0,p_stage);	// 読み込み
		}//ここまで2ページ目

		// 初手ブロック調整#1.60c7f5
		if(vslevel[0] == 29) {
			p_next_adjust = !p_next_adjust;
			if(!p_next_adjust){
				do {
					next[0] = Rand(7);
				} while((next[0] != 2) && (next[0] != 3) && (next[0] != 6));
			} else {
				do {
					next[0] = Rand(7);
				} while((next[0] == 2) || (next[0] == 3) || (next[0] == 6));
			}
			setNextBlockColors(0, 1);
		}
		// NEXT＠激しく非効率 #1.60c7f6
		if(vslevel[0] == 30) {
			next[0]--;
			if(next[0] < 0) next[0] = 6;
			setNextBlockColors(0, 1);
		}
		if(vslevel[0] == 31) {
			nextb[(nextc[0] + 1) % 1400]--;
			if(nextb[(nextc[0] + 1) % 1400] < 0) nextb[(nextc[0] + 1) % 1400] = 6;
			setNextBlockColors(0, 1);
		}
		if(vslevel[0] == 32) {
			nextb[(nextc[0] + 2) % 1400]--;
			if(nextb[(nextc[0] + 2) % 1400] < 0) nextb[(nextc[0] + 2) % 1400] = 6;
			setNextBlockColors(0, 1);
		}
		if(vslevel[0] == 33) {
			nextb[(nextc[0] + 3) % 1400]--;
			if(nextb[(nextc[0] + 3) % 1400] < 0) nextb[(nextc[0] + 3) % 1400] = 6;
			setNextBlockColors(0, 1);
		}
		if(vslevel[0] == 34) {
			nextb[(nextc[0] + 4) % 1400]--;
			if(nextb[(nextc[0] + 4) % 1400] < 0) nextb[(nextc[0] + 4) % 1400] = 6;
			setNextBlockColors(0, 1);
		}
		if(vslevel[0] == 35) {
			nextb[(nextc[0] + 5) % 1400]--;
			if(nextb[(nextc[0] + 5) % 1400] < 0) nextb[(nextc[0] + 5) % 1400] = 6;
			setNextBlockColors(0, 1);
		}
		// HOLD #1.60c7f6
		if(vslevel[0] == 36) {
			hold[0]--;
			if(hold[0] < -1) hold[0] = 6;
			bakhold = hold[0];		// #1.60c7j5
//			c_hblk[0] = 1 + hold[0];
			if( isWRule(player) ) {
				// ワールド
				c_hblk[0] = wcol[ hold[0] ];
			} else if( rots[player] >= 4 ) {
				// ARS
				c_hblk[0] = acol[ hold[0] ];
			} else {
				// クラシック
				c_hblk[0] = ccol[ hold[0] ];
			}
		}

		// ツモ読み込み #1.60c7g3
		if(vslevel[0] == 37) {
			p_nextblock--;
			if(p_nextblock < 0)p_nextblock = 12;
			b_nextc[0] = 0;
			versusInit(0);					// ネクストを初期化
		}

		// ツモ送り #1.60c7j9
		if(vslevel[0] == 38) {
			p_nextpass = !p_nextpass;
		}

		// NEXTC
		if(vslevel[0] == 39) {
			nextc[0]--;
			if(nextc[0] < 0) nextc[0] = 1399;
			next[0] = nextb[nextc[0]];
			b_nextc[0] = nextc[0];
			setNextBlockColors(0, 1);
		}

				// SQUARE #1.60c7j9
		if(vslevel[0] == 40) {
			squaremode[0] = !squaremode[0];
		}

		// GB
		if(vslevel[0] == 41){
			p_heboGBlv--;
			if(p_heboGBlv < 0) p_heboGBlv = 15;
		}
		// FPS #1.60c7n7
		if(vslevel[0] == 42) {
			max_fps--;
			if(max_fps <= 0) max_fps = 99;
		}

		}
	}

	// → (Cボタンを押しながらだと高速に数値を変更します)
	if((mpc[0] == 1) || ((mpc[0] > tame1) && (mpc[0] % tame2 == 0)) || (getPressState(player, 6))){
		if(getPressState(player, 3)) {
			PlaySE(3);
			// 数値変更(これも手抜き)

			// BGM#1.60c6.2d
			// 4番以降を選べる#1.60c7f5
			if(vslevel[0] == 0) {
				p_bgmlv++;
				if(p_bgmlv > 19) p_bgmlv = 0;
			}
			// 回転法則
			if(vslevel[0] == 1) {
				rots[0]++;
				if(rots[0] > 8) rots[0] = 0;
				setNextBlockColors(0, 1);
				if(hold[0] != -1){		// HOLDブロックの色設定
					if( isWRule(player) ) {
						// ワールド
						c_hblk[0] = wcol[ hold[0] ];
					} else if( (rots[player] >= 4)&&(rots[player] != 8) ) {
						// ARS
						c_hblk[0] = acol[ hold[0] ];
					} else {
						// クラシック
						c_hblk[0] = ccol[ hold[0] ];
					}
				}
			}
			// death+
			if(vslevel[0] == 2) {
				death_plus[0] = !death_plus[0];
			}
			// 出現
			if(vslevel[0] == 3) {
				wait1[0]++;
				if(wait1[0] > 99) wait1[0] = 0;
			}
			// 消去
			if(vslevel[0] == 4) {
				wait2[0]++;
				if(wait2[0] > 99) wait2[0] = 0;
			}
			// 接着
			if(vslevel[0] == 5) {
				wait3[0]++;
				if(wait3[0] > 99) wait3[0] = 0;
			}
			// 横溜め
			if(vslevel[0] == 6) {
				waitt[0]++;
				if(waitt[0] > 99) waitt[0] = 0;
			}
			// 落下スピード
			if(vslevel[0] == 7) {
				sp[0]++;
				if(sp[0] > 1200) sp[0] = 0;
			}
			// HIDDEN
			if(vslevel[0] == 8) {
				hidden[0]++;
				if(hidden[0] > 11) hidden[0] = 0;	// LV8を選べる#1.60c6.2c
			}
			// ENDING #1.60c7k7
			if(vslevel[0] == 9) {
				p_ending++;
				if(p_ending > 5) p_ending = 0;
			}
			// FAVORITES
			if(vslevel[0] == 10) {
				p_setting++;
				if(p_setting > skip_fwait) p_setting = 0;
			}
			// FINISHTYPE #1.60c7k7
			if(vslevel[0] == 11) {
				p_goaltype++;
				if(p_goaltype > 4) p_goaltype  = 0;
			}
			// FINISHTYPE #1.60c7k7
			if(vslevel[0] == 12) {
				p_goaltypenum++;
				if(p_goaltypenum > 9) p_goaltypenum  = 0;
			}
			// ITEM
			if(vslevel[0] == 13){
				p_item_mode = !p_item_mode;
			}


			// SHIRASE flags
			if(vslevel[0] == 14) {
				p_shirase[0] = !p_shirase[0];
			}
			if(vslevel[0] == 15) {
				upLineT[0]++;
				if(upLineT[0] > 2) upLineT[0] = 0;
			}
			if(vslevel[0] == 16) {
				raise_shirase_lines++;
				if(raise_shirase_lines > 12) raise_shirase_lines = 1;
			}
			if(vslevel[0] == 17) {
				raise_shirase_interval++;
				if(raise_shirase_interval > 100) raise_shirase_interval = 2;
			}

			// 背景
			if(vslevel[0] == 18) {
				p_backno++;
				if(p_backno > 11) p_backno = 0;
				backno = p_backno;
			}
			// TLS
			if(vslevel[0] == 19) {
				tlsrange++;
				if(tlsrange > 2) tlsrange = 0;	// TLSをレベルアップタイプと切り離す #1.60c7i2
			}
				// BIG
			if(vslevel[0] == 20) {
				IsBigStart[0] = !IsBigStart[0];
			}
			// []ブロック #1.60c7e
			if(vslevel[0] == 21) {
			p_over1000 = !p_over1000;
			setNextBlockColors(0, 1);
			}
					// ミラー #1.60c7j5
			if(vslevel[0] == 22) {
				fmirror = !fmirror;
			}
			// ロールロール #1.60c7j5
			if(vslevel[0] == 23) {
				rollroll = !rollroll;
			}
			// X-RAY #1.60c7p9ex
			if(vslevel[0] == 24) {
				xray = !xray;
			}
			// COLOR #1.60c7p9ex
			if(vslevel[0] == 25) {
				fcolor = !fcolor;
			}
			// ブラインド
			if(vslevel[0] == 26) {
				p_blind = !p_blind;
			}
			// サンダー
			if(vslevel[0] == 27) {
				thunder = !thunder;
			}
					// 初期状態フィールド #1.60c7f6
			if(vslevel[0] == 28) {
				//use_fld = !use_fld;
				p_stage++;
				if(p_stage > 199) p_stage = -1;
				loadTomoyoStage(0,p_stage);	// 読み込み
			}//ここまで2ページ目
			// 初手ブロック調整#1.60c7f5
			if(vslevel[0] == 29) {
				p_next_adjust = !p_next_adjust;
				if(!p_next_adjust){
					do {
						next[0] = Rand(7);
					} while((next[0] != 2) && (next[0] != 3) && (next[0] != 6));
				} else {
					do {
						next[0] = Rand(7);
					} while((next[0] == 2) || (next[0] == 3) || (next[0] == 6));
				}
				setNextBlockColors(0, 1);
			}
			// NEXT＠激しく非効率 #1.60c7f6
			if(vslevel[0] == 30) {
				next[0]++;
				if(next[0] > 6) next[0] = 0;
				setNextBlockColors(0, 1);
			}
			if(vslevel[0] == 31) {
				nextb[(nextc[0] + 1) % 1400]++;
				if(nextb[(nextc[0] + 1) % 1400] > 6) nextb[(nextc[0] + 1) % 1400] = 0;
				setNextBlockColors(0, 1);
			}
			if(vslevel[0] == 32) {
				nextb[(nextc[0] + 2) % 1400]++;
				if(nextb[(nextc[0] + 2) % 1400] > 6) nextb[(nextc[0] + 2) % 1400] = 0;
				setNextBlockColors(0, 1);
			}
			if(vslevel[0] == 33) {
				nextb[(nextc[0] + 3) % 1400]++;
				if(nextb[(nextc[0] + 3) % 1400] > 6) nextb[(nextc[0] + 3) % 1400] = 0;
				setNextBlockColors(0, 1);
			}
			if(vslevel[0] == 34) {
				nextb[(nextc[0] + 4) % 1400]++;
				if(nextb[(nextc[0] + 4) % 1400] > 6) nextb[(nextc[0] + 4) % 1400] = 0;
				setNextBlockColors(0, 1);
			}
			if(vslevel[0] == 35) {
				nextb[(nextc[0] + 5) % 1400]++;
				if(nextb[(nextc[0] + 5) % 1400] > 6) nextb[(nextc[0] + 5) % 1400] = 0;
				setNextBlockColors(0, 1);
			}
			// HOLD #1.60c7f6
			if(vslevel[0] == 36) {
				hold[0]++;
				if(hold[0] > 6) hold[0] = -1;
				bakhold = hold[0];		// #1.60c7j5
//				c_hblk[0] = 1 + hold[0];
				if( isWRule(player) ) {
					// ワールド
					c_hblk[0] = wcol[ hold[0] ];
				} else if( rots[player] >= 4 ) {
					// ARS
					c_hblk[0] = acol[ hold[0] ];
				} else {
				// クラシック
					c_hblk[0] = ccol[ hold[0] ];
				}
			}

			// ツモ読み込み #1.60c7g3
			if(vslevel[0] == 37) {
				p_nextblock++;
				if(p_nextblock > 12)p_nextblock = 0;
				b_nextc[0] = 0;
				versusInit(0);					// ネクストを初期化
			}

			// ツモ送り #1.60c7j9
			if(vslevel[0] == 38) {
				p_nextpass = !p_nextpass;
			}

			// NEXTC
			if(vslevel[0] == 39) {
				nextc[0]++;
				if(nextc[0] > 1399) nextc[0] = 0;
				next[0] = nextb[nextc[0]];
				b_nextc[0] = nextc[0];
				setNextBlockColors(0, 1);
			}
					//square #1.60c7j9
			if(vslevel[0] == 40) {
				squaremode[0] = !squaremode[0];
			}
			// GB
			if(vslevel[0] == 41){
				p_heboGBlv++;
				if(p_heboGBlv > 15) p_heboGBlv = 0;
			}
			// FPS #1.60c7n7
			if(vslevel[0] == 42) {
				max_fps++;
				if(max_fps > 99) max_fps = 0;
			}
		}
	}

	// Bボタンでタイトル
	if(getPushState(player, 5)) {
		gameMode[0] = 0;
		gameMode[1] = 0;
		flag = 1;
	}

	// Aボタン
	if(getPushState(player, 4)) {
		if(vslevel[0] == 10) {
			// FAVORITES関連
			loadWait(0, p_setting); // 対応した設定を読み込む。
			PlaySE(3);
		} else if(vslevel[0] == 28) {
			// INIT FLD
			PlaySE(10);

			if(p_stage != -1) {
				stage[0] = p_stage;			// ステージ設定
				status[0] = 18;					// ステージエディタ
				statusc[0 * 10] = 1;				// ステータスカウンタを0に
				statusc[0 * 10 + 1] = 0;			// ステータスカウンタを0に
				statusc[0 * 10 + 2] = 9;			// 戻り先設定
				loadTomoyoStage(0,p_stage);	// 読み込み
			} else {
				for(i=0;i<220;i++) {
					fld[i] = 0;
					fldt[i] = -1;
				}
			}
		} else {
			// そうでないならゲーム開始
			PlaySE(10);
			PracticeStart();
			bgmlv = p_bgmlv; // BGM変更#1.60c6.2c
		}
	}
}

// START!の部分だけ分離#1.60cf
void PracticeStart(void){
	playerInitial(1); // 2Pを初期化して結果を消去
	status[1] = 10; // 2Pの動きを停止

	TextLayerOff(0);
	upLines[0] = 0;
	shirase[0] = raise_shirase_interval;	// 最初のせり上がりカウントを設定 #1.60c7i2

	hiddenProc(0);

	if(p_goaltype==4){
		ltime[0] = p_goaltypenumlist[p_goaltypenum] * 2 * 60;
	}

	// エンディング #1.60c7k7
	if( p_ending == 1 ) {
		ending[0] = 1;
		tc[0] = 200;
	} else if ( p_ending == 2 ) {
		ending[0] = 1;
		tc[0] = 999;
		shadow_timer_flag[0] = 1;
	} else if ( p_ending == 3 ) {
		ending[0] = 1;
		tc[0] = 999;
		hidden[0] = 8;
	} else if ( p_ending == 4 ) {
		ending[0] = 1;
		tc[0] = 1300;
		p_over1000 = 1;
		IsBigStart[0] = 1;
	}else if ( p_ending == 5 ) {
		ending[0] = 6;
	}

	if(IsBigStart[0]) {
		IsBig[0] = 1;
	}

	// #1.60c7l7
	isfmirror[0] = fmirror;
	rollroll_flag[0] = rollroll;
	// #1.60c7p9ex
	xray_flag[0] = xray;
	color_flag[0] = fcolor;

	if(p_blind){
		isblind[0] = 1;
		blind_flag[0] = 1;
	}
	// ITEM
	item_mode[0] = p_item_mode;

	// wait値をバックアップ #1.60c7l2
	b_wait1[0] = wait1[0];
	b_wait2[0] = wait2[0];
	b_wait3[0] = wait3[0];
	b_waitt[0] = waitt[0];
	b_sp[0]    = sp[0];
	b_nextc[0] = nextc[0];

	// heboGB
	if(p_heboGBlv){
		heboGB[0] = 1;
		isholdlock[0] = 1;
		ishidenext[0] = 2;
		item_mode[0] = 0;
		sp[0] = lvTableHeboGB[p_heboGBlv-1];
		wait1[0] = wait1_HeboGB;
		wait2[0] = wait2_HeboGB;
		wait3[0] = wait3_HeboGB_tbl[p_heboGBlv-1];
		waitt[0] = waitt_HeboGB_tbl[p_heboGBlv-1];
	}

	// ブロックの色を設定 #1.60c7m9
	setNextBlockColors(0, 1);

	// FPSを設定 #1.60c7n7
	if(max_fps != max_fps_2) SetFPS(max_fps);

	// ステージ読み込み #1.60c7p1
	if(p_stage != -1) loadTomoyoStage(0,p_stage);


	if(!playback){
		GiziSRand(0);				// 擬似乱数のシードを生成（リプレイ時除く）

	}
	hole[0] = gameRand(10,0);
	status[0] = 3;					// Ready
	statusc[0 * 10] = 0;				// ステータスカウンタを0に
	statusc[0 * 10 + 1] = 0;
}


void loadWait(int32_t player, int32_t no) { // hoge heboris.iniで設定されたwaitを実際のwaitに代入するための関数。
	if(syutugen[no] != -1)	wait1[player]  = syutugen[no];
	if(syoukyo[no]  != -1)	wait2[player]  =  syoukyo[no];
	if(settyaku[no] != -1)	wait3[player]  = settyaku[no];
	if(yokotame[no] != -1)	waitt[player]  = yokotame[no];

	if(gameMode[player] == 5){
		if(hiddenlv[no] != -1)	hidden[player]  = hiddenlv[no];
		if(fps[no]      != -1)	max_fps        = fps[no];
		if(blind[no]    != -1)	p_blind        = blind[no];
		if(p_bgmload[no]    != -1)p_bgmlv        = p_bgmload[no];
	}
	// スピード設定を読み込み#1.60c6.c
	if(speed[no]    != -1)	sp[player]     = speed[no];
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  プラクティスで死んだとき#1.60cd
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void PracticeDeath() {
	int32_t i;

	// FPSを戻す #1.60c7n7
	if(GetFPS() != max_fps_2) SetFPS(max_fps_2);

	timeOn[0] = 0;
	onRecord[0] = 0;

	// BGM停止#1.60c6.2d
	if(wavebgm >= 1)
		StopAllBGM();

	// 条件の単純化に合わせ、すべてのブロックの透明を無効化する #1.60c7k6
	for(i = 0 ; i < 220 ; i++) {
		fldt[i] = -1;
	}

	if(hiddeny[0] < 22)
		hiddeny[0]++;

	if(tspin_c[0])
		tspin_c[0] = 0;
	if(b_to_b_c[0])
		b_to_b_c[0] = 0;

	printFont(28 + (3 * (!maxPlay)), 5, "-RESULT-", 3);
	viewLineInfo();

	// SECRET GRADE #1.60c7o1
	if( sgrade[0] >= min_sgrade ) {
		printFont(15 - 12 * maxPlay, 26, "S-GRADE", fontc[rots[0]]);
		printFont(23 + (sgrade[0] < 9) - 12 * maxPlay, 26, gname[sgrade[0]], 0);
	}
	printFont(16 - 12 * maxPlay, 25, "PRESS C", fontc[rots[0]]);
	if(getPushState(0, 6)) {
		PracticeOver();
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  Practiceモードで窒息or捨てゲーした時の処理#1.60cd
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void PracticeOver(void) {
	int32_t i,tmp;

	// FPSを戻す #1.60c7n7
	if(GetFPS() != max_fps_2) SetFPS(max_fps_2);

	// BGM停止#1.60c6.2d
	if(wavebgm >= 1)
		StopAllBGM();

	// 1Pの結果を2Pにコピー (from hogeパッチ)
	if(maxPlay){
		for(i = 0; i < 220; i++) {// hoge
			fld[i + 220] = fld[i];// 1Pのフィールドを2P側(RESULT)にコピー
			fldt[i + 220] = (fld[i] > 0) * -1 ;// 1Pのフィールドを2P側(RESULT)にコピー
			fldi[i + 220] = fldi[i];
			fldu[i + 220] = 0;
			flds[i + 220] = flds[i];
		}

		sc[1] = sc[0];				// hoge 1Pの成績等を2P側にコピー
		li[1] = li[0];				// Line
		lv[1] = lv[0];				// HeboLevel
		tc[1] = tc[0];				// TGMLevel
		grade[1] = grade[0];		// Grade
		medal_sk[1] = medal_sk[0];		// Skill
		medal_ac[1] = medal_ac[0];	// AC #1.60c7k4
		medal_co[1] = medal_co[0];	// CO

		time[1] = time[0];			// Time
		wait1[1] = wait1[0];		//
		wait2[1] = wait2[0];		//
		wait3[1] = wait3[0];		//
		waitt[1] = waitt[0];		//

		sp[1] = sp[0];					// スピードもコピー
		rots[1] = rots[0];				// 回転法則もコピー
		IsBig[1] = IsBig[0];
	}
	IsBigStart[1] = IsBigStart[0];
	hidden[1] = hidden[0];		// Hidden

	versusInit(0);					// ネクストを初期化 #1.60c7
	playerInitial(0);				// 1Pのみ初期化
	gameMode[0] = 5;				// VSモード
	status[0] = 9;					// ステータスNo.9(VSモードレベルセレクト)
	status[1] = 10;					// 2PはステータスNo.10(WAIT)
	backno = p_backno;
	flag = 0;

	// waitなどを2Pから書き戻す(↑のplayerInitialで初期化されるため)
	hidden[0] = hidden[1];
	IsBigStart[0] = IsBigStart[1];

	// バックアップからwaitを書き戻す #1.60c7l2
	wait1[0] = b_wait1[0];
	wait2[0] = b_wait2[0];
	wait3[0] = b_wait3[0];
	waitt[0] = b_waitt[0];
	sp[0] = b_sp[0];

	// シャドウタイマー解除 #1.60c7k1
	shadow_timer_flag[0] = 0;
	shadow_timer_flag[1] = 0;

	// エンディング #1.60c7k7
	if ( (p_ending == 2) || (p_ending == 3) ) {
		hidden[0] = 0;
	} else if ( p_ending == 4 ) {
		p_over1000 = 0;
		IsBigStart[0] = 0;
	}

	// ブロックの色を設定 #1.60c7m9
	setNextBlockColors(0, 1);
	setNextBlockColors(1, 1);
}
