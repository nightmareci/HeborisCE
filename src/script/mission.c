//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.23 - MISSIONモード セレクト画面
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲

#include "../script/include.h"

void statMissionSelect(int32_t player) {
	padRepeat(player);
	padRepeat2(player);
	if( (!IsPlayWave(62)) && (wavebgm >= 1) ) {
		PlayWave(62);
	}
	if(statusc[player * 10 + 4] > 0){
		statusc[player * 10 + 4]--;
		if(statusc[player * 10 + 4] == 0) PlaySE(5);
	} else if(statusc[player * 10 + 4] < 0){
		statusc[player * 10 + 4]++;
		if(statusc[player * 10 + 4] == 0) PlaySE(5);
	}

	printFont(15 + 24 * player - 12 * maxPlay, 5, "START!", (vslevel[0] <= 1) * count % 9);
	ExBltRect(77, 120 + 192 * player -96 * maxPlay , 52,  (count % 40) * 3, 28, 60, 8);

		printFont(15 + 24 * player - 12 * maxPlay, 10, "<        >", (vslevel[0] == 0) * count % 9);
		if(statusc[player * 10 + 4] < 0){
			ExBltRect(55,128 + ((16+statusc[player * 10 + 4]) * 4) + 192 * player -96 * maxPlay , 63,
				(64*(rots[player]+1))*(rots[player]<8),32*(fontc[(rots[player]+1)*(rots[player]<8)]),
				(statusc[player * 10 + 4]* -4),32);
			ExBltRect(55,128 + 192 * player -96 * maxPlay , 63,
				(64*rots[player])+(statusc[player * 10 + 4] * -4),32*fontc[rots[player]],((16+statusc[player * 10 + 4])* 4),32);
		} else if(statusc[player * 10 + 4] > 0){
			ExBltRect(55,128 + (statusc[player * 10 + 4] * 4) + 192 * player -96 * maxPlay , 63,
				(64*rots[player]),32*fontc[rots[player]],((16-statusc[player * 10 + 4])* 4),32);
			ExBltRect(55,128 + 192 * player -96 * maxPlay , 63,
				(64*(rots[player]-1))+576*(rots[player]==0)+((16-statusc[player * 10 + 4]) * 4),
				32*fontc[rots[player]-1+(9*(rots[player]==0))],(statusc[player * 10 + 4] * 4),32);
		} else ExBltRect(55,128 + 192 * player -96 * maxPlay , 63, 64*rots[player] ,32*fontc[rots[player]],64,32);
		ExBltRect(55,158 + 192 * player -96 * maxPlay , 40, (64*rots[player]) ,384,64,21);

//		ExBltRect(55,128 + 192 * player -96 * maxPlay , 55, 64*rots[player] ,32*fontc[rots[player]],64,32);
		if(!english)	//落下特性（日本語）
			ExBltRect(55,128 + 192 * player -96 * maxPlay , 96, 64*rots[player] ,320,64,32);
		else			//落下特性（English）
			ExBltRect(55,128 + 192 * player -96 * maxPlay , 96, 64*rots[player] ,352,64,32);
	printFont(15 + 24 * player - 12 * maxPlay, 16, "FILE", fontc[rots[player]]);

	getRoadName(mission_file);
	printFont(14 + 24 * player - 12 * maxPlay, 17, string[0], (vslevel[0] == 1) * count % 9 );

	printFont(15 + 24 * player - 12 * maxPlay, 19, "STAGE", fontc[rots[player]]);
	sprintf(string[0], "NO.%02d", c_mission);
	printFont(15 + 24 * player - 12 * maxPlay, 20, string[0], (vslevel[0] == 2) * count % 9 );

	if((mission_file >= 25) || (debug)){
		printFont(15 + 24 * player - 12 * maxPlay, 22, "LOAD", (vslevel[0] == 3) * count % 9);
		printFont(15 + 24 * player - 12 * maxPlay, 24, "SAVE", (vslevel[0] == 4) * count % 9);
	}else{
		ExBltRect(77, 120 + 192 * player -96 * maxPlay , 176,  160 - ((count % 40) * 3), 20, 80, 8);
	}

	// ↑
	if( (mpc2[player] == 1) || ((mpc2[player] > tame3) && (mpc2[player] % tame4 == 0)) )
	if( getPressState(player, 0) ) {
		PlaySE(5);
		vslevel[0]--;
		if(vslevel[0] < 0) vslevel[0] = 4 - (2 * ((mission_file < 25) && (!debug)));
	}

	// ↓
	if( (mpc2[player] == 1) || ((mpc2[player] > tame3) && (mpc2[player] % tame4 == 0)) )
	if( getPressState(player, 1) ) {
		PlaySE(5);
		vslevel[0]++;
		if(vslevel[0] > 4 - (2 * ((mission_file < 25) && (!debug)))) vslevel[0] = 0;
	}

	// ←
	if((mpc[player] == 1) || ((mpc[player] > tame1) && (mpc[player] % tame2 == 0)) || (getPressState(player, 6)))
	if( getPressState(player, 2) ) {
		PlaySE(3);

		if(vslevel[0] == 0) {
			rots[player]--;
			if(rots[player] < 0) rots[player] = 8;
			statusc[player * 10 + 4] = -16;
			setNextBlockColors(player, 1);
		}
		if(vslevel[0] == 1) {
			mission_file--;
			if(mission_file < 0) mission_file = 99;
			loadMissionData(mission_file);
		}
		if(vslevel[0] == 2) {
			c_mission--;
			if(c_mission < 0) c_mission = 29;
		}

	}

	// →
	if((mpc[player] == 1) || ((mpc[player] > tame1) && (mpc[player] % tame2 == 0)) || (getPressState(player, 6)))
	if( getPressState(player, 3) ) {
		PlaySE(3);

		if(vslevel[0] == 0) {
			rots[player]++;
			if(rots[player] > 8) rots[player] = 0;
			statusc[player * 10 + 4] = 16;
			setNextBlockColors(player, 1);
		}
		if(vslevel[0] == 1) {
			mission_file++;
			if(mission_file > 99) mission_file = 0;
			loadMissionData(mission_file);
		}
		if(vslevel[0] == 2) {
			c_mission++;
			if(c_mission > 29) c_mission = 0;
		}
	}

	// Bボタン
	if( getPushState(player, 5) ) {
		gameMode[0] = 0;
		vslevel[0] = 0;
		flag = 1;	// タイトルへ
	}

	// Aボタン
	if( getPushState(player, 4) ) {
		PlaySE(10);

		if(vslevel[0] <= 1) {
			// START!
			start_mission = c_mission;
			if(((mission_file == 6) || (mission_file == 18) || (mission_file == 24)) && (start_mission == 0)) PlaySE(18);
			StopAllBGM();
			//背景を設定
			if(mission_type[c_mission] == 19)
				backno = 10;
			else if((mission_type[c_mission] == 20) || (mission_type[c_mission] == 22) || (mission_type[c_mission] == 29))
				backno = 11;
			else {
			backno = ((c_mission+1) % 10)-1;
			if(backno < 0)
				backno = 9;
			}
			if(backno >= bg_max) backno = bg_max;
			if(mission_bgm[c_mission] != -1)
				bgmlv = mission_bgm[c_mission];
			else fadelv[0] = 10;
			if(repversw < 54)missionSetStatus();
			status[player] = 1;				// ブロックシャッター実行
			statusc[player * 10] = 0;			// ステータスカウンタを0に
			statusc[player * 10 + 1] = 3;		// シャッター後はステータスNo.3
			statusc[player * 10 + 4] = 0;
			return;
		}
		if((vslevel[0] == 2) && ((mission_file >= 25) || (debug))) {
			// EDIT
			StopAllBGM();
			statusc[player * 10 + 4] = 0;
			status[player] = 24;
		}
		if(vslevel[0] == 3) {
			// LOAD
			loadMissionData(mission_file);
		}
		if(vslevel[0] == 4) {
			// SAVE
			saveMissionData(mission_file);
		}
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.24 - MISSIONモード エディット画面
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void statMissionEditor(int32_t player) {
	padRepeat(player);
	padRepeat2(player);

	// 問題の種類
	printFont(15 + 24 * player - 12 * maxPlay, 5, "TYPE", fontc[rots[player]]);

	printFont(15 + 24 * player - 12 * maxPlay, 6, mission_name_editor[mission_type[c_mission]], (statusc[0 + player * 10] == 0) * count % 9);

	// ノルマ
	printFont(15 + 24 * player - 12 * maxPlay, 7, "NORM", fontc[rots[player]]);
	sprintf(string[0], "%d", mission_norm[c_mission]);
	printFont(15 + 24 * player - 12 * maxPlay, 8, string[0], (statusc[0 + player * 10] == 1) * count % 9);

	// 制限時間
	printFont(15 + 24 * player - 12 * maxPlay, 9, "TIME", fontc[rots[player]]);
	getTime(mission_time[c_mission]);
	printFont(15 + 24 * player - 12 * maxPlay, 10, string[0], (statusc[0 + player * 10] == 2) * count % 9);

	// レベル
	printFont(15 + 24 * player - 12 * maxPlay, 11, "LEVEL", fontc[rots[player]]);
	if(mission_lv[c_mission] >= 15)
		sprintf(string[0], "PLUS %d", mission_lv[c_mission] + 1 - 15);
	else
		sprintf(string[0], "%d", mission_lv[c_mission] + 1);
	printFont(15 + 24 * player - 12 * maxPlay, 12, string[0], (statusc[0 + player * 10] == 3) * count % 9);

	// エンディング
	printFont(15 + 24 * player - 12 * maxPlay, 13, "ENDING", fontc[rots[player]]);
	if(mission_end[c_mission] == 0) {
		sprintf(string[0], "NO");
	} else if(mission_end[c_mission] == 1) {
		sprintf(string[0], "END");
	} else if(mission_end[c_mission] == 2) {
		sprintf(string[0], "EXTRA");
	} else if(mission_end[c_mission] == 3) {
		sprintf(string[0], "EXTRA+");
	} else if(mission_end[c_mission] == 4) {
		sprintf(string[0], "STAFF ROLL");
	} else if(mission_end[c_mission] == 5) {
		sprintf(string[0], "M ROLL");
	} else if(mission_end[c_mission] == 6) {
		sprintf(string[0], "DEVIL+M ROLL");
	}
	printFont(15 + 24 * player - 12 * maxPlay, 14, string[0], (statusc[0 + player * 10] == 4) * count % 9);

	// 消去ライン数
	if(mission_erase[c_mission] < -20){
		printFont(15 + 24 * player - 12 * maxPlay, 15, "RISEE LINE", fontc[rots[player]]);
		sprintf(string[0], "%d", abs_YGS2K(mission_erase[c_mission]+20));
	}else if(mission_erase[c_mission] < 0){
		printFont(15 + 24 * player - 12 * maxPlay, 15, "RISEH LINE", fontc[rots[player]]);
		sprintf(string[0], "%d", abs_YGS2K(mission_erase[c_mission]));
	}else{
		printFont(15 + 24 * player - 12 * maxPlay, 15, "ERASE LINE", fontc[rots[player]]);
		sprintf(string[0], "%d", mission_erase[c_mission]);
	}
	printFont(15 + 24 * player - 12 * maxPlay, 16, string[0], (statusc[0 + player * 10] == 5) * count % 9);
	// 追加情報
	// 18行目MAX 19行目MIN 20行目OPT(ターゲットだと解説)　21行目解説
	printFont(15 + 24 * player - 12 * maxPlay, 17, "OPTIONS", fontc[rots[player]]);
	if((mission_type[c_mission] == 2)||(mission_type[c_mission] == 3)){
		sprintf(string[0], "GRADE:%d",mission_opt_1[c_mission]);
		printFont(15 + 24 * player - 12 * maxPlay, 18, string[0], (statusc[0 + player * 10] == 6) * count % 9);
		sprintf(string[0], "0=N1=E2=H");
		printFont(15 + 24 * player - 12 * maxPlay, 19, string[0], (statusc[0 + player * 10] == 7) * count % 9);
		sprintf(string[0], "NO OPTION");
		printFont(15 + 24 * player - 12 * maxPlay, 20, string[0], (statusc[0 + player * 10] == 8) * count % 9);

	}
	else if(mission_type[c_mission] == 6){   //ターゲット
		sprintf(string[0], "MINSTAGE:%d", mission_opt_1[c_mission]);
		printFont(15 + 24 * player - 12 * maxPlay, 18, string[0], (statusc[0 + player * 10] == 6) * count % 9);
		sprintf(string[0], "MAXSTAGE:%d", mission_opt_2[c_mission]);
		printFont(15 + 24 * player - 12 * maxPlay, 19, string[0], (statusc[0 + player * 10] == 7) * count % 9);
		if((statusc[0 + player * 10] == 6) || (statusc[0 + player * 10] == 7)){
			sprintf(string[0], "0-26=TI ");
			printFont(15 + 24 * player - 12 * maxPlay, 20, string[0], 3);
			sprintf(string[0], "27-44=EH");
			printFont(15 + 24 * player - 12 * maxPlay, 21, string[0], 3);
			sprintf(string[0], "45-71=ACE");
			printFont(15 + 24 * player - 12 * maxPlay, 22, string[0], 3);
			sprintf(string[0], "100-199=FP");
			printFont(15 + 24 * player - 12 * maxPlay, 23, string[0], 3);
		}else{
			if(!mission_opt_3[c_mission]){
				sprintf(string[0], "RANDTGT:OFF");
			}else if(mission_opt_3[c_mission] == 99){
				sprintf(string[0], "RANDTGT:FULL");
			}else{
				sprintf(string[0], "RANDTGT:%d", mission_opt_3[c_mission]);
			}
			printFont(15 + 24 * player - 12 * maxPlay, 21, string[0], (statusc[0 + player * 10] == 8) * count % 9);
		}
	}
	else if((mission_type[c_mission] == 7)||(mission_type[c_mission] == 39)){   //イレイさー
		sprintf(string[0], "MINLINE:%d", mission_opt_1[c_mission]);
		printFont(15 + 24 * player - 12 * maxPlay, 18, string[0], (statusc[0 + player * 10] == 6) * count % 9);
		sprintf(string[0], "MAXLINE:%d", mission_opt_2[c_mission]);
		printFont(15 + 24 * player - 12 * maxPlay, 19, string[0], (statusc[0 + player * 10] == 7) * count % 9);
		sprintf(string[0], "VIEWLINE:%d", mission_opt_3[c_mission]);
		printFont(15 + 24 * player - 12 * maxPlay, 20, string[0], (statusc[0 + player * 10] == 8) * count % 9);
		sprintf(string[0], "MAX:3");
		printFont(15 + 24 * player - 12 * maxPlay, 21, string[0], (statusc[0 + player * 10] == 8) * count % 9);
		sprintf(string[0], "MIN:0");
		printFont(15 + 24 * player - 12 * maxPlay, 22, string[0], (statusc[0 + player * 10] == 8) * count % 9);
	}
	else if(mission_type[c_mission] == 10){  //アナザー
		sprintf(string[0], "NO OPTION");
		printFont(15 + 24 * player - 12 * maxPlay, 18, string[0], (statusc[0 + player * 10] == 6) * count % 9);
		sprintf(string[0], "NO OPTION");
		printFont(15 + 24 * player - 12 * maxPlay, 19, string[0], (statusc[0 + player * 10] == 7) * count % 9);
		sprintf(string[0], "BLINDON:%d", mission_opt_3[c_mission]);
		printFont(15 + 24 * player - 12 * maxPlay, 20, string[0], (statusc[0 + player * 10] == 8) * count % 9);
		sprintf(string[0], "0:OFF 1:ON");
		printFont(15 + 24 * player - 12 * maxPlay, 21, string[0], (statusc[0 + player * 10] == 8) * count % 9);
	}
	else if(mission_type[c_mission] == 19){  //DEVIL 800
		sprintf(string[0], "NO OPTION");
		printFont(15 + 24 * player - 12 * maxPlay, 18, string[0], (statusc[0 + player * 10] == 6) * count % 9);
		sprintf(string[0], "NO OPTION");
		printFont(15 + 24 * player - 12 * maxPlay, 19, string[0], (statusc[0 + player * 10] == 7) * count % 9);
		sprintf(string[0], "RISEPACE:%d", mission_opt_3[c_mission]);
		printFont(15 + 24 * player - 12 * maxPlay, 20, string[0], (statusc[0 + player * 10] == 8) * count % 9);
		sprintf(string[0], "MAX:20");
		printFont(15 + 24 * player - 12 * maxPlay, 21, string[0], (statusc[0 + player * 10] == 8) * count % 9);
		sprintf(string[0], "MIN:1");
		printFont(15 + 24 * player - 12 * maxPlay, 22, string[0], (statusc[0 + player * 10] == 8) * count % 9);
	}
	else if(mission_type[c_mission] == 20){  //DEVIL 1200
		sprintf(string[0], "NO OPTION");
		printFont(15 + 24 * player - 12 * maxPlay, 18, string[0], (statusc[0 + player * 10] == 6) * count % 9);
		sprintf(string[0], "NO OPTION");
		printFont(15 + 24 * player - 12 * maxPlay, 19, string[0], (statusc[0 + player * 10] == 7) * count % 9);
		sprintf(string[0], "SPEED:%d", mission_opt_3[c_mission]);
		printFont(15 + 24 * player - 12 * maxPlay, 20, string[0], (statusc[0 + player * 10] == 8) * count % 9);
		sprintf(string[0], "0:OLD");
		printFont(15 + 24 * player - 12 * maxPlay, 21, string[0], (statusc[0 + player * 10] == 8) * count % 9);
		sprintf(string[0], "1:DOOM");
		printFont(15 + 24 * player - 12 * maxPlay, 22, string[0], (statusc[0 + player * 10] == 8) * count % 9);
	}
	else if(mission_type[c_mission] == 21){  //GARBAGE
		sprintf(string[0], "NO OPTION");
		printFont(15 + 24 * player - 12 * maxPlay, 18, string[0], (statusc[0 + player * 10] == 6) * count % 9);
		sprintf(string[0], "NO OPTION");
		printFont(15 + 24 * player - 12 * maxPlay, 19, string[0], (statusc[0 + player * 10] == 7) * count % 9);
		sprintf(string[0], "RISELINE:%d", mission_opt_3[c_mission]);
		printFont(15 + 24 * player - 12 * maxPlay, 20, string[0], (statusc[0 + player * 10] == 8) * count % 9);
		sprintf(string[0], "MAX:18");
		printFont(15 + 24 * player - 12 * maxPlay, 21, string[0], (statusc[0 + player * 10] == 8) * count % 9);
	}
	else if(mission_type[c_mission] == 26){  //ALLCLEAR
		sprintf(string[0], "NO OPTION");
		printFont(15 + 24 * player - 12 * maxPlay, 18, string[0], (statusc[0 + player * 10] == 6) * count % 9);
		sprintf(string[0], "NO OPTION");
		printFont(15 + 24 * player - 12 * maxPlay, 19, string[0], (statusc[0 + player * 10] == 7) * count % 9);
		sprintf(string[0], "NOBIG:%d", mission_opt_3[c_mission]);
		printFont(15 + 24 * player - 12 * maxPlay, 20, string[0], (statusc[0 + player * 10] == 8) * count % 9);
		sprintf(string[0], "0:OFF 1:ON");
		printFont(15 + 24 * player - 12 * maxPlay, 21, string[0], (statusc[0 + player * 10] == 8) * count % 9);
	}
	else if(mission_type[c_mission] == 27){  //COMBO
		sprintf(string[0], "NO OPTION");
		printFont(15 + 24 * player - 12 * maxPlay, 18, string[0], (statusc[0 + player * 10] == 6) * count % 9);
		sprintf(string[0], "NO OPTION");
		printFont(15 + 24 * player - 12 * maxPlay, 19, string[0], (statusc[0 + player * 10] == 7) * count % 9);
		sprintf(string[0], "NOT1LINE:%d", mission_opt_3[c_mission]);
		printFont(15 + 24 * player - 12 * maxPlay, 20, string[0], (statusc[0 + player * 10] == 8) * count % 9);
		sprintf(string[0], "0:OFF 1:ON");
		printFont(15 + 24 * player - 12 * maxPlay, 21, string[0], (statusc[0 + player * 10] == 8) * count % 9);
	}
	else if((mission_type[c_mission] >= 30) && (mission_type[c_mission] <= 33)||(mission_type[c_mission] == 38)){
		sprintf(string[0], "NO OPTION");
		printFont(15 + 24 * player - 12 * maxPlay, 18, string[0], (statusc[0 + player * 10] == 6) * count % 9);
		sprintf(string[0], "HOLD USE:%d",mission_opt_2[c_mission]);
		printFont(15 + 24 * player - 12 * maxPlay, 19, string[0], (statusc[0 + player * 10] == 7) * count % 9);
		sprintf(string[0], "BLOCK:%d", mission_opt_3[c_mission]);
		printFont(15 + 24 * player - 12 * maxPlay, 20, string[0], (statusc[0 + player * 10] == 8) * count % 9);
		printFont(15 + 24 * player - 12 * maxPlay, 21, "(0-8)", (statusc[0 + player * 10] == 8) * count % 9);
	}
	// その他
	else {
		sprintf(string[0], "NO OPTION");
		printFont(15 + 24 * player - 12 * maxPlay, 18, string[0], (statusc[0 + player * 10] == 6) * count % 9);
		sprintf(string[0], "NO OPTION");
		printFont(15 + 24 * player - 12 * maxPlay, 19, string[0], (statusc[0 + player * 10] == 7) * count % 9);
		sprintf(string[0], "NO OPTION");
		printFont(15 + 24 * player - 12 * maxPlay, 20, string[0], (statusc[0 + player * 10] == 8) * count % 9);
	}
	printFont(15 + 24 * player - 12 * maxPlay, 24, "BGM", fontc[rots[player]]);
	if(mission_bgm[c_mission] == -1)
		sprintf(string[0], "FADE");
	else
		sprintf(string[0], "%d", mission_bgm[c_mission]);
	printFont(19 + 24 * player - 12 * maxPlay, 24, string[0], (statusc[0 + player * 10] == 9) * count % 9);

	// ↑
	if( (mpc2[player] == 1) || ((mpc2[player] > tame3) && (mpc2[player] % tame4 == 0)) )
	if( getPressState(player, 0) ) {
		PlaySE(5);
		statusc[0 + player * 10]--;
		if(statusc[0 + player * 10] < 0) statusc[0 + player * 10] = 9;
	}

	// ↓
	if( (mpc2[player] == 1) || ((mpc2[player] > tame3) && (mpc2[player] % tame4 == 0)) )
	if( getPressState(player, 1) ) {
		PlaySE(5);
		statusc[0 + player * 10]++;
		if(statusc[0 + player * 10] > 9) statusc[0 + player * 10] = 0;
	}

	// ←
	if((mpc[player] == 1) || ((mpc[player] > tame1) && (mpc[player] % tame2 == 0)) || (getPressState(player, 6)))
	if( getPressState(player, 2) ) {
		PlaySE(3);

		// TYPE
		if(statusc[0 + player * 10] == 0) {
			mission_type[c_mission]--;
			if(mission_type[c_mission] < 0) mission_type[c_mission] = 41;
		}
		// NORM
		if(statusc[0 + player * 10] == 1) {
			mission_norm[c_mission]--;
			if(mission_norm[c_mission] < 0) mission_norm[c_mission] = 99;
		}
		// TIME
		if(statusc[0 + player * 10] == 2) {
			mission_time[c_mission] = mission_time[c_mission] - 60;
			if(mission_time[c_mission] < 0) mission_time[c_mission] = 60 * 60 * 20;
		}
		// LEVEL
		if(statusc[0 + player * 10] == 3) {
			mission_lv[c_mission]--;
			if(mission_lv[c_mission] < 0) mission_lv[c_mission] = 29;
		}
		// ENDING
		if(statusc[0 + player * 10] == 4) {
			mission_end[c_mission]--;
			if(mission_end[c_mission] < 0) mission_end[c_mission] = 6;
		}
		// ERASE LINE
		if(statusc[0 + player * 10] == 5) {
			mission_erase[c_mission]--;
			if(mission_erase[c_mission] < -40) mission_erase[c_mission] = 21;
		}
		// OPTIONS
		if(statusc[0 + player * 10] == 6) {
			mission_opt_1[c_mission]--;
			if(mission_opt_1[c_mission] < 0) mission_opt_1[c_mission] = 199;
		}
		if(statusc[0 + player * 10] == 7) {
			mission_opt_2[c_mission]--;
			if(mission_opt_2[c_mission] < 0) mission_opt_2[c_mission] = 199;
		}
		if(statusc[0 + player * 10] == 8) {
			mission_opt_3[c_mission]--;
			if(mission_opt_3[c_mission] < 0) mission_opt_3[c_mission] = 99;
		}
		if(statusc[0 + player * 10] == 9) {
			mission_bgm[c_mission]--;
			if(mission_bgm[c_mission] < -1) mission_bgm[c_mission] = 19;
		}
	}

	// →
	if((mpc[player] == 1) || ((mpc[player] > tame1) && (mpc[player] % tame2 == 0)) || (getPressState(player, 6)))
	if( getPressState(player, 3) ) {
		PlaySE(3);

		// TYPE
		if(statusc[0 + player * 10] == 0) {
			mission_type[c_mission]++;
			if(mission_type[c_mission] > 41) mission_type[c_mission] = 0;
		}
		// NORM
		if(statusc[0 + player * 10] == 1) {
			mission_norm[c_mission]++;
			if(mission_norm[c_mission] > 99) mission_norm[c_mission] = 0;
		}
		// TIME
		if(statusc[0 + player * 10] == 2) {
			mission_time[c_mission] = mission_time[c_mission] + 60;
			if(mission_time[c_mission] > 60 * 60 * 20) mission_time[c_mission] = 0;
		}
		// LEVEL
		if(statusc[0 + player * 10] == 3) {
			mission_lv[c_mission]++;
			if(mission_lv[c_mission] > 29) mission_lv[c_mission] = 0;
		}
		// ENDING
		if(statusc[0 + player * 10] == 4) {
			mission_end[c_mission]++;
			if(mission_end[c_mission] > 6) mission_end[c_mission] = 0;
		}
		// ERASE LINE
		if(statusc[0 + player * 10] == 5) {
			mission_erase[c_mission]++;
			if(mission_erase[c_mission] > 21) mission_erase[c_mission] = -40;
		}
		// OPTIONS
		if(statusc[0 + player * 10] == 6) {
			mission_opt_1[c_mission]++;
			if(mission_opt_1[c_mission] > 199) mission_opt_1[c_mission] = 0;
		}
		if(statusc[0 + player * 10] == 7) {
			mission_opt_2[c_mission]++;
			if(mission_opt_2[c_mission] > 199) mission_opt_2[c_mission] = 0;
		}
		if(statusc[0 + player * 10] == 8) {
			mission_opt_3[c_mission]++;
			if(mission_opt_3[c_mission] > 99) mission_opt_3[c_mission] = 0;
		}
		if(statusc[0 + player * 10] == 9) {
			mission_bgm[c_mission]++;
			if(mission_bgm[c_mission] > 19) mission_bgm[c_mission] = -1;
		}
	}

	// AまたはBボタン
	if( getPushState(player, 4) || getPushState(player, 5) ) {
		status[player] = 23;	// セレクト画面に戻る
		statusc[0 + player * 10] = 0;
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  問題の表示
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void viewMission() {
	int32_t i,tmp,minus,c_tmp,c_tmp1,c_tmp2;
	tmp = 0;
	minus = 0;

	// BltFastRect(プレーンナンバー,dx,dy,sx,sy,hx,hy);

	// 枠
	if(getDrawRate() == 1) {
		if(mission_file <= 24) BltFastRect(44, maxPlay * 208, 12, 160, (20 * mission_file) + (10 * (english)), 80, 10);
		BltFastRect(44, 208 * maxPlay, 112, 288, 368, 112, 128);
	} else {
		if(mission_file <= 24) BltFastRect(44, 8 + maxPlay * 408, 24, 32, (32 * mission_file) + (16 * (english)), 128, 16);
		BltFastRect(44, 16 + maxPlay * 400, 224, 448, 288, 192, 192);
	}

	// 問題の種類
	if((ending[0] <= 1) || (ending[0] >= 4)){
	if(getDrawRate() == 1) {
		TextLayerOn(0, 6 + 208 * maxPlay, 8 + 112);
		TextSize(0, 10);
	} else {
		TextLayerOn(0, 10 + 16 + maxPlay * 400, 8 + 228);
		TextSize(0, 16);
	}
	TextColor(0, 0, 0, 0);
	TextBackColorDisable(0);

	getMissionName(mission_type[c_mission],c_mission);

	// 落下速度レベル
	if((mission_type[c_mission] != 19) && (mission_type[c_mission] != 20) && (mission_type[c_mission] != 29)){
		sprintf(string[70], " LV%02d", mission_lv[c_mission] + 1 - 15 * (mission_lv[c_mission] >= 15));
		strcat(string[0], string[70]);
	}else if(mission_type[c_mission] == 20){
		if(!mission_opt_3[c_mission])
			strcat(string[0]," OLD");
		else
			strcat(string[0], " REAL");
	}

	// 描画
	TextOut(0, string[0]);
	TextBlt(0);

	// 制限時間
	if(getDrawRate() == 1) {
		TextMove(0, 6 + 208 * maxPlay, 30 + 112);
	} else {
		TextMove(0, 10 + 16 + maxPlay * 400, 30 + 228);
	}

	if(!english) strcpy(string[70], "制限時間 ");
	else strcpy(string[70], "Time ");

	getTime(mission_time[c_mission]);
	if(mission_time[c_mission]>0){
	strcat(string[70], string[0]);
	}else{
		if(!english) strcat(string[70], "無制限");
		else strcat(string[70], "NoLimit");
	}

	// 描画
	TextOut(0, string[70]);
	TextBlt(0);

	// 指令文
	if(getDrawRate() == 1) {
		TextMove(0, 6 + 208 * maxPlay, 52 + 112);
	} else {
		TextMove(0, 10 + 16 + maxPlay * 400, 52 + 228);
	}
	// LITE版を参考に整理
	if(!english) {//"\n\nを使って\n%dライン消せ！",
		sprintf(string[0], mission_info_jp[mission_type[c_mission]], mission_norm[c_mission]);
		if(((mission_type[c_mission] >= 30) && (mission_type[c_mission] <= 33)||(mission_type[c_mission] == 38))&&(mission_opt_2[c_mission]==1))strcat(string[0], "\n\n消す前にそのブロック\nを一度ホールドに入れろ");
		if(((mission_type[c_mission] == 2) || (mission_type[c_mission] == 3))&&(mission_opt_1[c_mission]==1))strcat(string[0], "\n\n指定以上でもカウント!");
		if(((mission_type[c_mission] == 2) || (mission_type[c_mission] == 3))&&(mission_opt_1[c_mission]==2))strcat(string[0], "\n\n指定以外で\nノルマリセット!");
		if((mission_end[c_mission] == 2) || (mission_end[c_mission] == 3))strcat(string[0], "\n\n最後のラインは\nダブルで消せ！");
	} else {
		sprintf(string[0], mission_info_en[mission_type[c_mission]], mission_norm[c_mission]);
		if(((mission_type[c_mission] >= 30) && (mission_type[c_mission] <= 33)||(mission_type[c_mission] == 38))&&(mission_opt_2[c_mission]==1))strcat(string[0], "\n\nBefore Erase \ninto HOLD");
		if(((mission_type[c_mission] == 2) || (mission_type[c_mission] == 3))&&(mission_opt_1[c_mission]==1))strcat(string[0], "\n\nErase over lines count");
		if(((mission_type[c_mission] == 2) || (mission_type[c_mission] == 3))&&(mission_opt_1[c_mission]==2))strcat(string[0], "\n\nDo not erase\n other lines!");
		if((mission_end[c_mission] == 2) || (mission_end[c_mission] == 3))strcat(string[0], "\n\nlast erase 2 lines");

	}

	//ブロックオーダー系のブロックの絵
	if((mission_type[c_mission] >= 30) && (mission_type[c_mission] <= 33)||(mission_type[c_mission] == 38)){
		if(mission_opt_3[c_mission]<=6){
			if( isWRule(0) ) {
				// ワールド
				c_tmp = wcol[mission_opt_3[c_mission]];
			} else if( (rots[0] >= 4 ) && (rots[0] != 8)) {
				// ARS
				c_tmp = acol[mission_opt_3[c_mission]];
			} else {
				// クラシック
				c_tmp = ccol[mission_opt_3[c_mission]];
			}
			//7JL 8SZ
			if ( getDrawRate() == 1 ){
				if(!english){
					drawBlockFast(2 + 25 * maxPlay, 20 + isWRule(0) * (mission_opt_3[c_mission] != 0) +
					((mission_opt_3[c_mission] == 0) && (rots[0] != 8)), mission_opt_3[c_mission], 0, c_tmp, 0, -1, 0, 0 ,0);
				}else{
					drawBlockFast(3 - (mission_type[c_mission] == 33) + 25 * maxPlay, 21 + (mission_type[c_mission] >= 31) +
					(mission_type[c_mission] == 33) + isWRule(0) * (mission_opt_3[c_mission] != 0) +
					((mission_opt_3[c_mission] == 0) && (rots[0] != 8)), mission_opt_3[c_mission], 0, c_tmp, 0, 1, 0, 0, 0);
				}
			}else{
				if(!english){
					drawBlockFast(2 + 25 * maxPlay, 16 + isWRule(0) * (mission_opt_3[c_mission] != 0) +
					((mission_opt_3[c_mission] == 0) && (rots[0] != 8)), mission_opt_3[c_mission], 0, c_tmp, 0, 2, 0, 0, 0);
				}else{
					drawBlockFast(3 - (mission_type[c_mission] == 33) + 25 * maxPlay, 17 + (mission_type[c_mission] >= 31) +
					(mission_type[c_mission] == 33) + isWRule(0) * (mission_opt_3[c_mission] != 0) +
					((mission_opt_3[c_mission] == 0) && (rots[0] != 8)), mission_opt_3[c_mission], 0, c_tmp, 0, 4, 0, 0, 0);
				}
			}
		}else if(mission_opt_3[c_mission]==7){//LJ
			if( isWRule(0) ) {
				// ワールド
				c_tmp1 = wcol[1];
				c_tmp2 = wcol[5];
			} else if( (rots[0] >= 4 ) && (rots[0] != 8)) {
				// ARS
				c_tmp1 = acol[1];
				c_tmp2 = acol[5];
			} else {
				// クラシック
				c_tmp1 = ccol[1];
				c_tmp2 = ccol[5];
			}
			//7JL 8SZ
			if ( getDrawRate() == 1 ){
				if(!english){
					drawBlockFast(2 + 25 * maxPlay, 20 + isWRule(0) * (mission_opt_3[c_mission] != 0) +
					((mission_opt_3[c_mission] == 0) && (rots[0] != 8)), 1, 0, c_tmp1, 0, -1, 0, 0, 0);
					drawBlockFast(6 + 25 * maxPlay, 20 + isWRule(0) * (mission_opt_3[c_mission] != 0) +
					((mission_opt_3[c_mission] == 0) && (rots[0] != 8)), 5, 0, c_tmp2, 0, -1, 0, 0, 0);
				}else{
					drawBlockFast(3 - (mission_type[c_mission] == 33) + 25 * maxPlay, 21 + (mission_type[c_mission] >= 31) +
					(mission_type[c_mission] == 33) + isWRule(0) * (mission_opt_3[c_mission] != 0) +
					((mission_opt_3[c_mission] == 0) && (rots[0] != 8)),1, 0, c_tmp1, 0, 1, 0, 0, 0);
					drawBlockFast(7 - (mission_type[c_mission] == 33) + 25 * maxPlay, 21 + (mission_type[c_mission] >= 31) +
					(mission_type[c_mission] == 33) + isWRule(0) * (mission_opt_3[c_mission] != 0) +
					((mission_opt_3[c_mission] == 0) && (rots[0] != 8)),5, 0, c_tmp2, 0, 1, 0, 0, 0);
				}
			}else{
				if(!english){
					drawBlockFast(2 + 25 * maxPlay, 16 + isWRule(0) * (mission_opt_3[c_mission] != 0) +
					((mission_opt_3[c_mission] == 0) && (rots[0] != 8)), 1, 0, c_tmp1, 0, 2, 0, 0, 0);
					drawBlockFast(6 + 25 * maxPlay, 16 + isWRule(0) * (mission_opt_3[c_mission] != 0) +
					((mission_opt_3[c_mission] == 0) && (rots[0] != 8)), 5, 0, c_tmp2, 0, 2, 0, 0, 0);
				}else{
					drawBlockFast(3 - (mission_type[c_mission] == 33) + 25 * maxPlay, 17 + (mission_type[c_mission] >= 31) +
					(mission_type[c_mission] == 33) + isWRule(0) * (mission_opt_3[c_mission] != 0) +
					((mission_opt_3[c_mission] == 0) && (rots[0] != 8)), 1, 0, c_tmp1, 0, 4, 0, 0, 0);
					drawBlockFast(7 - (mission_type[c_mission] == 33) + 25 * maxPlay, 17 + (mission_type[c_mission] >= 31) +
					(mission_type[c_mission] == 33) + isWRule(0) * (mission_opt_3[c_mission] != 0) +
					((mission_opt_3[c_mission] == 0) && (rots[0] != 8)), 5, 0, c_tmp2, 0, 4, 0, 0, 0);
				}
			}
		}else if(mission_opt_3[c_mission]==8){//SZ
			if( isWRule(0) ) {
				// ワールド
				c_tmp1 = wcol[3];
				c_tmp2 = wcol[6];
			} else if( (rots[0] >= 4 ) && (rots[0] != 8)) {
				// ARS
				c_tmp1 = acol[3];
				c_tmp2 = acol[6];
			} else {
				// クラシック
				c_tmp1 = ccol[3];
				c_tmp2 = ccol[6];
			}
			//7JL 8SZ
			if ( getDrawRate() == 1 ){
				if(!english){
					drawBlockFast(2 + 25 * maxPlay, 20 + isWRule(0) * (mission_opt_3[c_mission] != 0) +
					((mission_opt_3[c_mission] == 0) && (rots[0] != 8)), 3, 0, c_tmp1, 0, -1, 0, 0, 0);
					drawBlockFast(6 + 25 * maxPlay, 20 + isWRule(0) * (mission_opt_3[c_mission] != 0) +
					((mission_opt_3[c_mission] == 0) && (rots[0] != 8)), 6, 0, c_tmp2, 0, -1, 0, 0, 0);
				}else{
					drawBlockFast(3 - (mission_type[c_mission] == 33) + 25 * maxPlay, 21 + (mission_type[c_mission] >= 31) +
					(mission_type[c_mission] == 33) + isWRule(0) * (mission_opt_3[c_mission] != 0) +
					((mission_opt_3[c_mission] == 0) && (rots[0] != 8)), 3, 0, c_tmp1, 0, 1, 0, 0, 0);
					drawBlockFast(7 - (mission_type[c_mission] == 33) + 25 * maxPlay, 21 + (mission_type[c_mission] >= 31) +
					(mission_type[c_mission] == 33) + isWRule(0) * (mission_opt_3[c_mission] != 0) +
					((mission_opt_3[c_mission] == 0) && (rots[0] != 8)), 6, 0, c_tmp2, 0, 1, 0, 0, 0);
				}
			}else{
				if(!english){
					drawBlockFast(2 + 25 * maxPlay, 16 + isWRule(0) * (mission_opt_3[c_mission] != 0) +
					((mission_opt_3[c_mission] == 0) && (rots[0] != 8)), 3, 0, c_tmp1, 0, 2, 0, 0, 0);
					drawBlockFast(6 + 25 * maxPlay, 16 + isWRule(0) * (mission_opt_3[c_mission] != 0) +
					((mission_opt_3[c_mission] == 0) && (rots[0] != 8)), 6, 0, c_tmp2, 0, 2, 0, 0, 0);
				}else{
					drawBlockFast(3 - (mission_type[c_mission] == 33) + 25 * maxPlay, 17 + (mission_type[c_mission] >= 31) +
					(mission_type[c_mission] == 33) + isWRule(0) * (mission_opt_3[c_mission] != 0) +
					((mission_opt_3[c_mission] == 0) && (rots[0] != 8)), 3, 0, c_tmp1, 0, 4, 0, 0, 0);
					drawBlockFast(7 - (mission_type[c_mission] == 33) + 25 * maxPlay, 17 + (mission_type[c_mission] >= 31) +
					(mission_type[c_mission] == 33) + isWRule(0) * (mission_opt_3[c_mission] != 0) +
					((mission_opt_3[c_mission] == 0) && (rots[0] != 8)), 6, 0, c_tmp2, 0, 4, 0, 0, 0);
				}
			}
		}
	}

	// 描画
	TextOut(0, string[0]);
	TextBlt(0);

	// 使用終了
	TextLayerOff(0);
	while(c_mission - tmp > 4){
		tmp = tmp + 5;
	}

	// ミッション一覧
	for(i = tmp; i < (tmp+5); i++) {
		if(getDrawRate() == 1) {
			BltFastRect(44, 208 * maxPlay, 32 + (i-tmp) * 16, 288, 304 + (((c_mission == i) && (ending[0] == 0)) * 16) + ((c_mission > i) || (ending[0] != 0)) * 32, 112, 13);
			TextLayerOn(0, 1 + 208 * maxPlay, 1 + 32 + (i-tmp) * 16);
			TextSize(0, 10);
		} else {
			BltFastRect(44, 16 + maxPlay * 400, 64 + (i-tmp) * 32, 448, 224 + (((c_mission == i) && (ending[0] == 0)) * 16) + ((c_mission > i) || (ending[0] != 0)) * 32, 160, 16);
			TextLayerOn(0, 1 + 16 + maxPlay * 400, 1 + 64 + (i-tmp) * 32);
			TextSize(0, 14);
		}
		TextColor(0, 0, 0, 0);
		TextBackColorDisable(0);

		getMissionName(mission_type[i],i);

		// 描画
		TextOut(0, string[0]);
		TextBlt(0);

		// 使用終了
		TextLayerOff(0);

		// 終了フラグがある場合はここでループを抜ける
		if(mission_end[i]) break;
	}
	}	//if(!ending[0])
}

// ミッション名を取得
void getMissionName(int32_t mtype,int32_t number) {
	if(!english) {
		strcpy(string[0], mission_name_jp[mtype]);
	} else {
		strcpy(string[0], mission_name_en[mtype]);
	}
	if(mission_lv[number] >= 15)
		strcat(string[0],"+");
}
// ロード名を取得
void getRoadName(int32_t no){
	if(no == 0)
		sprintf(string[0], " BIG ROAD");
	else if(no == 1)
		sprintf(string[0], " TRICKY ROAD");
	else if(no == 2)
		sprintf(string[0], " GRAND ROAD");
	else if(no == 3)
		sprintf(string[0], " STAR ROAD");
	else if(no == 4)
		sprintf(string[0], "ANOTHER ROAD");
	else if(no == 5)
		sprintf(string[0], " DS ROAD");
	else if(no == 6)
		sprintf(string[0], " DEVIL ROAD");
	else if(no <= 16)
		sprintf(string[0], " TRIAL S%d", no - 6);
	else if(no == 17)
		sprintf(string[0], " TRIAL HM");
	else if(no == 18)
		sprintf(string[0], " TRIAL GOD");
	else if(no == 19)
		sprintf(string[0], "HEBO AMATEUR");
	else if(no == 20)
		sprintf(string[0], " HEBO PRO");
	else if(no == 21)
		sprintf(string[0], " HEBO BRONZE");
	else if(no == 22)
		sprintf(string[0], " HEBO SILVER ");
	else if(no == 23)
		sprintf(string[0], " HEBO GOLD");
	else if(no == 24)
		sprintf(string[0], "HEBO PLATINUM");
	else
		sprintf(string[0], " NO.%02d", no);
}
/* ミッションモードのノルマ上昇 */
void missionNormUp(int32_t lines) {
	int32_t i, j,gap;
	int32_t by2, tmp;

	// レベルスター系列（ビッグ･HS1&2･アナザー･X-RAY･カラー・ロールロール・ミラー）
	if( (mission_type[c_mission] == 0) || (mission_type[c_mission] == 1) || (mission_type[c_mission] == 8) ||
		(mission_type[c_mission] == 9) || (mission_type[c_mission] == 10) ||
		(mission_type[c_mission] == 12) || (mission_type[c_mission] == 13) || (mission_type[c_mission] == 14) ||
		(mission_type[c_mission] == 15) || (mission_type[c_mission] == 17) || (mission_type[c_mission] == 18) ||
		(mission_type[c_mission] == 19) || (mission_type[c_mission] == 20) || (mission_type[c_mission] == 21) ||
		(mission_type[c_mission] == 22) || (mission_type[c_mission] == 24) || (mission_type[c_mission] == 25))
	{
		c_norm[0] = c_norm[0] + lines;
	}

	// ダブル
	if( mission_type[c_mission] == 2 ) {
		if(mission_opt_1[c_mission] == 1){
			if(lines >= 2) c_norm[0]++;//ダブル以上も認める
		}else if(mission_opt_1[c_mission] == 2){
			if(lines == 2)c_norm[0]++;
			else if(c_norm[0] > 0){
			c_norm[0] = 0;
			PlaySE(45);
			}
		}
		else if(lines == 2) c_norm[0]++;
	}

	// トリプル
	if( mission_type[c_mission] == 3 ) {
		if(mission_opt_1[c_mission] == 1){
			if(lines >= 3) c_norm[0]++;//トリプル以上も認める
		}else if(mission_opt_1[c_mission] == 2){
			if(lines == 3)c_norm[0]++;
			else if(c_norm[0] > 0){
			c_norm[0] = 0;
			PlaySE(45);
			}
		}
		else if(lines == 3) c_norm[0]++;
	}

	// ヘボリス
	if( mission_type[c_mission] == 4 ) {
		if(lines == 4) c_norm[0]++;
	}

	// サイクル
	if( mission_type[c_mission] == 5 ) {
		if(!cycle[lines - 1]) {
			cycle[lines - 1] = 1;
			c_norm[0]++;
		}
	}

	// ターゲット
	if( mission_type[c_mission] == 6 ) {
		// プラチナブロックが0
		if(getPlatinumBlock(0) == 0) {
			PlaySE(31);
			c_norm[0]++;

			// 次のステージへ
			if(c_norm[0] < mission_norm[c_mission]) {
				upLineNo[0] = 0;
				upLines[0] = 0;
				timeOn[0] = 0;

				for(i = 0; i < 22; i++) erase[i] = 1;

				status[0] = 25;	// 全ライン消去
				statusc[0 * 10 + 0] = wait1[0];	//待ち時間スキップ
				statusc[0 * 10 + 1] = 3;	// Ready
				statusc[0 * 10 + 2] = 1;	// ステージ読み込みあり
			}
		}
	}

	// イレイサー
	if( (mission_type[c_mission] == 7 )||(mission_type[c_mission] == 39)) {
		if((lines >= 4)||(mission_type[c_mission] != 39)){
			for(i = 0; i < 4; i++) {
				// 調べるY座標を決める
				if(rots[0] == 8){
					by2 = (by[0] + blkDDataY[blk[0] * 16 + rt[0] * 4 + i]);
				} else if( isWRule(0) ) {
					by2 = (by[0] + blkWDataY[blk[0] * 16 + rt[0] * 4 + i]);
				} else {
					by2 = (by[0] + blkDataY[blk[0] * 16 + rt[0] * 4 + i]);
				}

				// 消すべきラインか調べる
				for(j = 0; j < eraser_now_lines; j++) {
					if(j >= 4) break;

					if( (by2 == eraser_lines[j]) && (erase[by2]) && (!eraser_cleared[j]) ) {
						eraser_cleared[j] = 1;
						c_norm[0]++;
					}
				}
			}

			// 全部消えているのにノルマ達成していない場合は新たなパターン出現
			if(c_norm[0] < mission_norm[c_mission]) {
				tmp = 1;

				for(i = 0; i < eraser_now_lines; i++) {
					if(i >= 4) break;

					if(!eraser_cleared[i]) {
						tmp = 0;
						break;
					}
				}

				if(tmp) {
					eraser_now_pattern++;
					setEraserLines();
				}
			}
		}
	}

	// シングル
	if( mission_type[c_mission] == 11 ) {
		if(lines == 1) c_norm[0]++;
	}

	// T-spin
	if( mission_type[c_mission] == 16 ) {
		if(tspin_flag[0] != 0) c_norm[0]++;
	}

	//耐久
	if((ltime[0] <= 0) && (mission_type[c_mission] == 23))
		c_norm[0] = mission_norm[c_mission];

	// 全消し
	if( mission_type[c_mission] == 26 ) {
		c_norm[0]++;
	}
	//コンボ
	if( mission_type[c_mission] == 27 ) {
		if(mission_opt_3[c_mission] == 1) c_norm[0] = combo2[0];
		else c_norm[0] = combo[0];
	}
	// B to Bヘボリス
	if( mission_type[c_mission] == 28 ) {
		if(lines == 4) c_norm[0]++;
		else if(c_norm[0] > 0){
			c_norm[0] = 0;
			gflash[0] = 120;
			gup_down[0] = 0;//regret
			PlaySE(45);
		}
	}

	// OOBAKA
	if( (mission_type[c_mission] == 29) && (status[0] == 6) && ((timeOn[0]) || (repversw < 58)) ) {
		c_norm[0]++;
	}

	// ブロックオーダー
	if( mission_type[c_mission] == 30 ) {
		if( (mission_opt_2[c_mission] == 0) ||
		    ((mission_opt_2[c_mission] == 1) && (dhold2[0]) && (hold_used[0] >= 2)) ){
			if((mission_opt_3[c_mission]>=0)&&(mission_opt_3[c_mission]<=6)){//単品
				if(blk[0] == mission_opt_3[c_mission]) c_norm[0] = c_norm[0] + lines;
			}else if(mission_opt_3[c_mission]==7){//LJ
				if((blk[0] == 1)||(blk[0] == 5)) c_norm[0] = c_norm[0] + lines;
			}else if(mission_opt_3[c_mission]==8){//SZ
				if((blk[0] == 3)||(blk[0] == 6)) c_norm[0] = c_norm[0] + lines;
			}
		}
	}

	// ブロックオーダーシングルSP
	if( mission_type[c_mission] == 31 ) {//0-6
		if( (mission_opt_2[c_mission] == 0) ||
		    ((mission_opt_2[c_mission] == 1) && (dhold2[0]) && (hold_used[0] >= 2)) ){
			if((mission_opt_3[c_mission]>=0)&&(mission_opt_3[c_mission]<=6)){//単品
				if((lines == 1) && (blk[0] == mission_opt_3[c_mission])) c_norm[0]++;
			}else if(mission_opt_3[c_mission]==7){//LJ
				if((lines == 1) && ((blk[0] == 1)||(blk[0] == 5))) c_norm[0]++;
			}else if(mission_opt_3[c_mission]==8){//SZ
				if((lines == 1) && ((blk[0] == 3)||(blk[0] == 6))) c_norm[0]++;
			}
		}
	}
	// ダブルオーダー
	if( mission_type[c_mission] == 32 ) {//0-6
		if( (mission_opt_2[c_mission] == 0) ||
		((mission_opt_2[c_mission] == 1) && (dhold2[0]) && (hold_used[0] >= 2)) ){
			if(mission_opt_1[c_mission]){
				if((mission_opt_3[c_mission]>=0)&&(mission_opt_3[c_mission]<=6)){//単品
					if((lines >= 2) && (blk[0] == mission_opt_3[c_mission])) c_norm[0]++;
				}else if(mission_opt_3[c_mission]==7){//LJ
					if((lines >= 2) && ((blk[0] == 1)||(blk[0] == 5))) c_norm[0]++;
				}else if(mission_opt_3[c_mission]==8){//SZ
					if((lines >= 2) && ((blk[0] == 3)||(blk[0] == 6))) c_norm[0]++;
				}
			}else{
				if((mission_opt_3[c_mission]>=0)&&(mission_opt_3[c_mission]<=6)){//単品
					if((lines == 2) && (blk[0] == mission_opt_3[c_mission])) c_norm[0]++;
				}else if(mission_opt_3[c_mission]==7){//LJ
					if((lines == 2) && ((blk[0] == 1)||(blk[0] == 5))) c_norm[0]++;
				}else if(mission_opt_3[c_mission]==8){//SZ
					if((lines == 2) && ((blk[0] == 3)||(blk[0] == 6))) c_norm[0]++;
				}
			}
		}
	}
	// 裏トリプルSP
	if( mission_type[c_mission] == 33 ) {
		if( (mission_opt_2[c_mission] == 0) ||
		    ((mission_opt_2[c_mission] == 1) && (dhold2[0]) && (hold_used[0] >= 2)) ){
			if((mission_opt_3[c_mission]>=0)&&(mission_opt_3[c_mission]<=6)){//単品
				if((lines == 3) && (blk[0] != mission_opt_3[c_mission])) c_norm[0]++;
			}else if(mission_opt_3[c_mission]==7){//LJ
				if((lines == 3) && ((blk[0] != 1)||(blk[0] != 5))) c_norm[0]++;
			}else if(mission_opt_3[c_mission]==8){//SZ
				if((lines == 3) && ((blk[0] != 3)||(blk[0] != 6))) c_norm[0]++;
			}
		}
	}

	// 中抜きダブル
	if( mission_type[c_mission] == 34 ) {
		if(lines == 2){
			gap=0;	// 中抜き判定
			j=0;	// 中抜き判定フラグ
			for(i=0;i<22;i++){
				if(erase[i])
					j++;
				else if(j!=0)
					gap++;
				if(j>=lines) break;
			}
			if(gap != 0) c_norm[0]++;
		}
	}

	// 中抜きトリプル
	if( mission_type[c_mission] == 35 ) {
		if(lines == 3){
			gap=0;	// 中抜き判定
			j=0;	// 中抜き判定フラグ
			for(i=0;i<22;i++){
				if(erase[i])
					j++;
				else if(j!=0)
					gap++;
				if(j>=lines) break;
			}
			if(gap != 0) c_norm[0]++;
		}
	}

	// 2段抜きダブル
	if( mission_type[c_mission] == 36 ) {
		if(lines == 2){
			gap=0;	// 中抜き判定
			j=0;	// 中抜き判定フラグ
			for(i=0;i<22;i++){
				if(erase[i])
					j++;
				else if(j!=0)
					gap++;
				if(j>=lines) break;
			}
			if(gap == 2) c_norm[0]++;
		}
	}

	// TSD
	if( mission_type[c_mission] == 37 ) {
		if((tspin_flag[0] == 2) && (lines >= 2)) c_norm[0]++;
	}
	// トリプルSP
	if( mission_type[c_mission] == 38 ) {
		if( (mission_opt_2[c_mission] == 0) ||
		    ((mission_opt_2[c_mission] == 1) && (dhold2[0]) && (hold_used[0] >= 2)) ){
			if((mission_opt_3[c_mission]>=0)&&(mission_opt_3[c_mission]<=6)){//単品
				if((lines == 3) && (blk[0] == mission_opt_3[c_mission])) c_norm[0]++;
			}else if(mission_opt_3[c_mission]==7){//LJ
				if((lines == 3) && ((blk[0] == 1)||(blk[0] == 5))) c_norm[0]++;
			}else if(mission_opt_3[c_mission]==8){//SZ
				if((lines == 3) && ((blk[0] == 3)||(blk[0] == 6))) c_norm[0]++;
			}
		}
	}
		// スクウェア／ゴールドスクウェア
	if((mission_type[c_mission] == 40) || (mission_type[c_mission] == 41))
	{
		 c_norm[0]++;
	}

	// ミッションクリア　EX突入時
	if((c_norm[0] >= mission_norm[c_mission]) && ((mission_end[c_mission] == 2) || (mission_end[c_mission] == 3)) && (lines == 2)) {
		PlaySE(43);//levelup.wavの代わりにstgstar.wav
		StopSE(32);
		if(mission_end[c_mission] == 3)
			PlaySE(18);
		objectCreate2(0, 8, Rand(20) + 180 + 192 * 0 - 96 * maxPlay, 20 + Rand(10), 0, 0, 0, 0);
		timeOn[0] = 0;
		c_mission++;
		clear_mission++;
		bgfadesw = 1;
		missionSetStatus();
		if(mission_bgm[c_mission] == -1)
			fadelv[0] = 10;
		else if(mission_bgm[c_mission] != mission_bgm[c_mission - 1]){
			StopAllBGM();
			bgmlv = mission_bgm[c_mission];
			changeBGM(0);
		}
	} else if(c_norm[0] >= mission_norm[c_mission]) { //普通クリア
		PlaySE(19);
		StopSE(32);

		timeOn[0] = 0;
		c_mission++;
		clear_mission++;

		// エンディング突入チェック
		if((c_mission > 29) || (mission_end[c_mission - 1] == 1) || (mission_end[c_mission - 1] >= 4) || (((mission_end[c_mission - 1] == 2) || (mission_end[c_mission - 1] == 3)) && (lines != 2))) {
			c_mission = c_mission - 1;
			c_norm[0] = mission_norm[c_mission];
			if(((mission_end[c_mission] == 2) || (mission_end[c_mission] == 3)) && (lines != 2))
				ending[0] = 4;
			else
				ending[0] = 1;
			pinch[0] = 0;
			if(mission_end[c_mission] < 4)	//スタッフロール開始のため、onRecordをそのままに
				onRecord[0] = 2;
			pinch[0] = 0;
		} else {
			bgfadesw = 1;
			missionSetStatus();
			if(mission_bgm[c_mission] == -1)
				fadelv[0] = 10;
			else if(mission_bgm[c_mission] != mission_bgm[c_mission - 1]){
				StopAllBGM();
				bgmlv = mission_bgm[c_mission];
				changeBGM(0);
			}
		}
	}
}

/* 問題の設定（落下速度etc）をゲームに反映させる */
void missionSetStatus() {
	int32_t i;
	int32_t tmp;
	// ノルマ
	c_norm[0] = 0;

	upLineNo[0] = 0;

	// タイム
	ltime[0] = mission_time[c_mission];

	// レベル
	lv[0] = mission_lv[c_mission] + 1;

	// 速度
	if(mission_lv[c_mission] < 15){
	if(mission_type[c_mission] <= 7){
		// 通常
		if((repversw < 56) || (mission_file == 6))
			sp[0] = lvTableAce55[mission_lv[c_mission]];
		else
			sp[0] = lvTableAce[mission_lv[c_mission]];
		wait1[0] = wait1_Ace;
		wait2[0] = wait2_Ace;
		wait3[0] = wait3_Ace;
		waitt[0] = waitt_Ace;
	} else if(mission_type[c_mission] == 8) {
		// ハイスピード１
		sp[0] = lvTableHiSpeed1[mission_lv[c_mission]];
		wait1[0] = wait1_HiSpeed1;
		wait2[0] = wait2_HiSpeed1;
		wait3[0] = wait3_HiSpeed1;
		waitt[0] = waitt_HiSpeed1;
	} else if(mission_type[c_mission] == 9) {
		// ハイスピード２
		sp[0] = 1200;
		wait1[0] = wait1_HiSpeed2;
		wait2[0] = wait2_HiSpeed2;
		wait3[0] = wait3_HiSpeed2;
		waitt[0] = waitt_HiSpeed2;
	} else if((mission_type[c_mission] == 10) || (mission_type[c_mission] == 23)) {
		// アナザー&耐久
		sp[0] = 1200;
		wait1[0] = wait1_Acea_tbl[mission_lv[c_mission]];
		wait2[0] = wait2_Acea_tbl[mission_lv[c_mission]];
		wait3[0] = wait3_Acea_tbl[mission_lv[c_mission]];
		waitt[0] = waitt_Acea_tbl[mission_lv[c_mission]];
	} else if((mission_type[c_mission] >= 11) && (mission_type[c_mission] <= 18) || (mission_type[c_mission] == 21)) {
		// X-RAY以降。テキトー
		if((repversw < 56) || (mission_file == 6))
			sp[0] = lvTableAce55[mission_lv[c_mission]];
		else
			sp[0] = lvTableAce[mission_lv[c_mission]];
		wait1[0] = wait1_Ace;
		wait2[0] = wait2_Ace;
		wait3[0] = wait3_Ace;
		waitt[0] = waitt_Ace;
	} else if(mission_type[c_mission] == 19) {
		// DEVIL 800
		sp[0] = 1200;
		wait1[0] = wait1_devil_tbl32[8];
		wait2[0] = wait2_devil_tbl32[8];
		wait3[0] = wait3_devil_tbl32[8];
		waitt[0] = waitt_devil_tbl32[8];
	} else if(mission_type[c_mission] == 20) {
		// DEVIL 1200
		sp[0] = 1200;
		if(!mission_opt_3[c_mission]){
			wait1[0] = wait1_devil_tbl32[12];
			wait2[0] = wait2_devil_tbl32[12];
			wait3[0] = wait3_devil_tbl32[12];
			waitt[0] = waitt_devil_tbl32[12];
		} else {
			if(repversw < 60){
				wait1[0] = wait1_devil_tbl[12];
				wait2[0] = wait2_devil_tbl[12];
				wait3[0] = wait3_devil_tbl[12];
				waitt[0] = waitt_devil_tbl[12];
			}else{
				wait1[0] = wait1_doom_tbl[12];
				wait2[0] = wait2_doom_tbl[12];
				wait3[0] = wait3_doom_tbl[12];
				waitt[0] = waitt_doom_tbl[12];
			}
		}
	} else if(mission_type[c_mission] == 22) {
		// オールドスタイル
		sp[0] = lvTableHeboGB[mission_lv[c_mission]];
		wait1[0] = wait1_HeboGB;
		wait2[0] = wait2_HeboGB;
		wait3[0] = wait3_HeboGB_tbl[mission_lv[c_mission]];
		waitt[0] = waitt_HeboGB_tbl[mission_lv[c_mission]];
	} else if(mission_type[c_mission] >= 24) {
		// とりあえず耐久以降
		if((repversw < 56) || (mission_file == 6))
			sp[0] = lvTableAce55[mission_lv[c_mission]];
		else
			sp[0] = lvTableAce[mission_lv[c_mission]];
		wait1[0] = wait1_Ace;
		wait2[0] = wait2_Ace;
		wait3[0] = wait3_Ace;
		waitt[0] = waitt_Ace;
	}
	}
	//+
	if((mission_lv[c_mission] >= 15) && (mission_type[c_mission] != 20)){
		sp[0] = 1200;
		wait1[0] = wait1_Acea_tbl[mission_lv[c_mission] - 15];
		wait2[0] = wait2_Acea_tbl[mission_lv[c_mission] - 15];
		wait3[0] = wait3_Acea_tbl[mission_lv[c_mission] - 15];
		waitt[0] = waitt_Acea_tbl[mission_lv[c_mission] - 15];
		if(mission_type[c_mission] == 19){
			if(repversw < 60){
				wait1[0] = wait1_devil_tbl[8];
				wait2[0] = wait2_devil_tbl[8];
				wait3[0] = wait3_devil_tbl[8];
				waitt[0] = waitt_devil_tbl[8];
			}else{
				wait1[0] = wait1_doom_tbl[8];
				wait2[0] = wait2_doom_tbl[8];
				wait3[0] = wait3_doom_tbl[8];
				waitt[0] = waitt_doom_tbl[8];
			}
		}
	}
	// OOBAKA
	if(mission_type[c_mission] == 29){
		sp[0] = 1200;
		wait1[0] = (repversw >= 60);
		wait2[0] = 0;
		wait3[0] = 0;
		waitt[0] = 5;
	}

	// サイクル用フラグ
	if(mission_type[c_mission] == 5) {
		for(i = 0; i < 4; i++) cycle[i] = 0;
	}
	// ターゲット用
	if(mission_type[c_mission] == 6) {
	//ステージ26までしか初期化していなかった問題を修正
		for(i = 0; i < 200; i++) target_cleared[i] = 0;
	}
	// イレイサー用
	if((mission_type[c_mission] == 7)||(mission_type[c_mission] == 39)) {
		eraser_now_pattern = 0;
		setEraserLines();
	}

	//アナザーでoptを1以上にした時　またはブラインド
	if(((mission_type[c_mission] == 10) && (mission_opt_3[c_mission])) || (mission_type[c_mission] == 25)){
		isblind[0] = 1;
		blind_flag[0] = 1;
	} else {
		isblind[0] = 0;
		blind_flag[0] = 0;
	}
	// X-RAY用
	if(mission_type[c_mission] == 12) {
		isxray[0] = 1;
		xray_flag[0] = 1;;
		xray_counter[0] = 0;
	} else {
		isxray[0] = 0;
		xray_flag[0] = 0;
	}

	// カラー用
	if(mission_type[c_mission] == 13) {
		iscolor[0] = 1;
		color_flag[0] = 1;
	} else {
		iscolor[0] = 0;
		color_flag[0] = 0;
	}

	// ロールロール用
	if(mission_type[c_mission] == 14) {
		rollroll_flag[0] = 1;
		isrollroll[0] = 1;
	} else {
		rollroll_flag[0] = 0;
		isrollroll[0] = 0;
	}

	// ミラー用
	if(mission_type[c_mission] == 15) {
		isfmirror[0] = 1;
	} else {
		isfmirror[0] = 0;
	}

	//回転不可
	if(mission_type[c_mission] == 17) {
		isrotatelock[0] = 1;
		rt_nblk[0 + 6 * 0] = gameRand(4,0);
		rt_nblk[1 + 6 * 0] = gameRand(4,0);
		rt_nblk[2 + 6 * 0] = gameRand(4,0);
		rt_nblk[3 + 6 * 0] = gameRand(4,0);
		rt_nblk[4 + 6 * 0] = gameRand(4,0);
		rt_nblk[5 + 6 * 0] = gameRand(4,0);
	} else {
		isrotatelock[0] = 0;
		rt_nblk[0 + 6 * 0] = 0;
		rt_nblk[1 + 6 * 0] = 0;
		rt_nblk[2 + 6 * 0] = 0;
		rt_nblk[3 + 6 * 0] = 0;
		rt_nblk[4 + 6 * 0] = 0;
		rt_nblk[5 + 6 * 0] = 0;
	}

	//NEXT不可視
	if(mission_type[c_mission] == 18) {
		ishidenext[0] = 1;
	} else {
		ishidenext[0] = 0;
	}
	//DEVIL 800(COPY RISING)
	if(mission_type[c_mission] == 19) {
		st_bdowncnt[0] = 0;
		upLineT[0] = 0;
		if(mission_opt_3[c_mission] == 0)
			mission_opt_3[c_mission] = 20;
		shirase[0] = mission_opt_3[c_mission];
	}else{
		st_bdowncnt[0] = 0;
	}

	//DEVIL 1200([ ])
	if(mission_type[c_mission] == 20) {
		over1000_block[0] = 2;
	} else {
		over1000_block[0] = 0;
	}
	//GARBAGE
	if(mission_type[c_mission] == 21){
		upLineT[0] = 2;
		upLines[0] = mission_opt_3[c_mission];
		if(repversw < 41){
			if(upLines[0] > 18-(2*(!spawn_y_type)))
				upLines[0] = 18-(2*(!spawn_y_type));
		}else{
			if(upLines[0] > 18)
				upLines[0] = 18;
			upLines[0] = upLines[0]-(2*(!spawn_y_type));
		}
	} else {
		upLineT[0] = uplinetype;
		if(repversw >= 40) upLines[0] = 0;
	}
	//オールドスタイル
	if(mission_type[c_mission] == 22) {
		heboGB[0] = 1;
		ishidenext[0] = 2;
		isholdlock[0] = 1;
	} else {
		heboGB[0] = 0;
		isholdlock[0] = 0;
	}
	//上下左右逆転
	if(mission_type[c_mission] == 24) {
		isLRreverse[0] = 1;
		isUDreverse[0] = 1;
	} else {
		isLRreverse[0] = 0;
		isUDreverse[0] = 0;
	}

	//ビッグ＆全消し
	if((mission_type[c_mission] == 1) || ((mission_type[c_mission] == 26) && (mission_opt_3[c_mission] == 0))) {
		IsBig[0] = 1;
	} else {
		IsBig[0] = 0;
	}

	if((mission_type[c_mission] == 40) || (mission_type[c_mission] == 41)) {
		squaremode[0] = 1;
	} else {
		squaremode[0] = 0;
	}
}
// フィールドの最上段を返す
//gamestart.cに移動 C7U0.8


// イレイサー用初期化
void setEraserLines() {
	int32_t i, tmp,max,min;
	int32_t line[22];	// ラインの重複防止用

	for(i = 0; i < 22; i++) line[i] = 0;

	eraser_now_lines = 0;
	// 枠内出現時は最高18段まで
	max = mission_opt_2[c_mission];
	min = mission_opt_1[c_mission];

	if((!spawn_y_type) && (mission_opt_1[c_mission] <= 2))
		min = 3;
	if(max >= fldsizeh[0])
		max = fldsizeh[0];

	if(min > max){
		min = i;
		min = max;
		i = max;
	}

	for(i = 0; i <= mission_opt_3[c_mission]; i++) {
		if(i >= 4) break;	// 最大で4本まで
		if(i >= mission_norm[c_mission] - c_norm[0]) break;
		if((eraser_now_pattern >= 2) && (i >= 1)) break;

		eraser_now_lines++;

		if(min == max) {
			// 位置固定
			tmp = min+1;
		}else if((i >= mission_norm[c_mission] - c_norm[0] - 1) && (mission_norm[c_mission] > 1) && (line[min + 1] != 1) && (repversw >= 54)){
			// 最後の1本
			tmp = min+1;
		}else{
			do {
				tmp = gameRand(22,0);
			} while( (tmp < min+1) || (tmp > max+1) || (line[tmp] == 1) );
		}
		line[tmp] = 1;
		eraser_lines[i] = tmp;
		eraser_cleared[i] = 0;
	}
}

// イレイサーの線を表示
void viewEraserLines() {
	int32_t i;

	for(i = 0; i < eraser_now_lines; i++) {
		if(i >= 4) break;	// 最大で4本まで

		if(getDrawRate() == 1)
			BltRect(44, (14 - 12 * maxPlay) * 8, (eraser_lines[i] + 3) * 8, 288, 256 - 64 * (mission_type[c_mission] == 39) + eraser_cleared[i] * 32, 96, 8);
		else
			BltRect(44, (14 - 12 * maxPlay) * 16, (eraser_lines[i] + 3) * 16, 448, 160 - 64 * (mission_type[c_mission] == 39)+ eraser_cleared[i] * 32, 192, 16);
	}
}

// ターゲットでのステージ読み込み
void loadTargetStage() {
	int32_t tmp;
statusc[0 * 10 + 6] = mission_opt_3[c_mission];
	if(mission_opt_1[c_mission] == mission_opt_2[c_mission]){	//ステージが1つだけ
		tmp = mission_opt_1[c_mission];
	}else{
		// 追加情報で出現ステージの範囲を指定可能 #1.60c7s2
		do {
			if(repversw < 46) tmp = gameRand(100,0);
			else tmp = gameRand(mission_opt_2[c_mission] + 1,0);
		} while( (tmp < mission_opt_1[c_mission]) || (tmp > mission_opt_2[c_mission]) || (target_cleared[tmp] == 1) );
	target_cleared[tmp] = 1;	// 出現済みフラグON
	}
	loadTomoyoStage(0, tmp);
	// プラチナブロックランダム化（せり上がるステージはランダムにしない）
	if((repversw >= 58) && (tomoyo_rise[0] == 0) && (mission_opt_3[c_mission] != 0)) randPlatinum(0, mission_opt_3[c_mission]);
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  問題データの読み込み／保存
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
// ファイルフォーマット (4byte単位)
//   0       バージョン (0=無効 1=c7r8)
//   1〜  19 未使用
//  20〜  39 問題１
//  40〜  59 問題２

/* ロード */
void loadMissionData(int32_t number) {
	int32_t i;

	FillMemory(&saveBuf, 50000 * 4, 0);

	sprintf(string[0], "config/mission/mission%02d.sav", number);

	LoadFile(string[0], &saveBuf, 930 * 4);

	// 問題データを読み込み
	for(i = 0; i < 30; i++) {
		mission_type[i]  = saveBuf[(i + 1) * 20 + 0];	// 種類
		mission_norm[i]  = saveBuf[(i + 1) * 20 + 1];	// ノルマ
		mission_time[i]  = saveBuf[(i + 1) * 20 + 2];	// 制限時間
		mission_end[i]   = saveBuf[(i + 1) * 20 + 3];	// 終了フラグ
		mission_lv[i]    = saveBuf[(i + 1) * 20 + 4];	// レベル
		mission_erase[i] = saveBuf[(i + 1) * 20 + 5];	// クリア時に上から消去するライン数
		mission_opt_1[i] = saveBuf[(i + 1) * 20 + 6];	// 追加情報１（下限）
		mission_opt_2[i] = saveBuf[(i + 1) * 20 + 7];	// 追加情報２（上限）
		mission_opt_3[i] = saveBuf[(i + 1) * 20 + 8];	// 追加情報３（イレイサーだと本数、SHIRASE800はせり上がり間隔、他は未使用）
		mission_bgm[i]   = saveBuf[(i + 1) * 20 + 9];	// BGM
	}
}

/* セーブ */
void saveMissionData(int32_t number) {
	int32_t i;

	FillMemory(&saveBuf, 50000 * 4, 0);

	// ヘッダ
	saveBuf[0] = 1;

	// 問題データ
	for(i = 0; i < 30; i++) {
		saveBuf[(i + 1) * 20 + 0] = mission_type[i];	// 種類
		saveBuf[(i + 1) * 20 + 1] = mission_norm[i];	// ノルマ
		saveBuf[(i + 1) * 20 + 2] = mission_time[i];	// 制限時間
		saveBuf[(i + 1) * 20 + 3] = mission_end[i];		// 終了フラグ
		saveBuf[(i + 1) * 20 + 4] = mission_lv[i];		// レベル
		saveBuf[(i + 1) * 20 + 5] = mission_erase[i];	// クリア時に上から消去するライン数
		saveBuf[(i + 1) * 20 + 6] = mission_opt_1[i];	// 追加情報１
		saveBuf[(i + 1) * 20 + 7] = mission_opt_2[i];	// 追加情報２
		saveBuf[(i + 1) * 20 + 8] = mission_opt_3[i];	// 追加情報３
		saveBuf[(i + 1) * 20 + 9] = mission_bgm[i];		// BGM
	}

	sprintf(string[0], "config/mission/mission%02d.sav", number);
	SaveFile(string[0], &saveBuf, 930 * 4);
}
