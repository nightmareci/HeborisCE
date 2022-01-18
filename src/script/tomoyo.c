#include "../script/include.h"

/* TOMOYOモードでの部分的初期化処理 #1.60c7l8 */
void tomoyoInitial(int32_t player) {
	int32_t sbak[20];

	// 初期化されては困るものをバックアップにコピー
	sbak[0]  = stage[player];	// ステージ
	sbak[1]  = ltime[player];	// リミットタイム
	sbak[2]  = nextc[player];	// NEXTカウント
	sbak[3]  = next[player];		// NEXTブロック
	sbak[4]  = time[player];		// タイム
	sbak[5]  = clearnum[player];	// クリアステージ数
	sbak[6]  = clearp[player];	// クリア率
	sbak[7]  = replay_save[player];	// 記録可能フラグ
	sbak[8]  = time2[player];		// TOMOYOリプレイ用プレイタイム
	sbak[9]  = bgmlv;			// BGM
	sbak[10] = t_training[player];	// トレーニングモード
	sbak[11] = repversw;	//これを初期化されてはミラーがずれる c7t3.1
	sbak[12] = tomoyo_opt[player];
	sbak[13] = laststage[player];	//これを初期化されるとEH、ACE-TGTの最終ステージが正しく設定されない c7T3.2
	sbak[14] = sc[player];		// F-Pointのスコア
	sbak[15] = fpbas_mode[player];  //FPエンドレスモード
	sbak[16] = lv[player];		//FP用レベル(一応全てキープ)

	// 初期化
	playerInitial(player);

	// 初期化されては困るものをバックアップから戻す
	stage[player]       = sbak[0];
	ltime[player]       = sbak[1];
	nextc[player]       = sbak[2];
	next[player]        = sbak[3];
	time[player]        = sbak[4];
	clearnum[player]    = sbak[5];
	clearp[player]      = sbak[6];
	replay_save[player] = sbak[7];
	time2[player]       = sbak[8];
	bgmlv               = sbak[9];
	t_training[player]  = sbak[10];
	repversw            = sbak[11];
	tomoyo_opt[player]  = sbak[12];
	laststage[player]   = sbak[13];
	sc[player]          = sbak[14];
	fpbas_mode[player]  = sbak[15];

	if((tomoyo_opt[player]==4)&&(fpbas_mode[player])){
		lv[player] = sbak[16];	//FPBASICだけ戻す他のモードは初期化
	}

	backno = 0;

	// 初期スピード値と初期wait値読み込み #1.60c7m2
	setStartLevel(player);

	// NEXTブロックの色を設定 #1.60c7n1
	setNextBlockColors(player, 1);
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.17 - TOMOYOモード ステージクリア／タイムオーバー
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void statTomoyoNextStage(int32_t player) {
	int32_t i, j, ext, timelimit_tomoyo;	//laststage[player]はgamestartへ
	int32_t ltime_s[2],Stage_update;
	ext = 0;
	timelimit_tomoyo = 320*60;	//EX7への足切り

	statusc[player * 10 + 0]++;

	StopSE(32);
	pinch[player] = 0;
	isxray[player] = 0;			// #1.60c7p9ex
	iscolor[player] = 0;		// #1.60c7p9ex
	Stage_update = 0;

	// EX面突入前にBGMを消す #1.60c7o8
	if( (stage[player] == 19) && (!t_training[player]) ) {
		StopAllBGM();
		fadelv[player] = 0;
		bgmlv = 9;		// EX面BGM
	}
	if( (stage[player] == 43) && (!t_training[player]) ) {
		StopAllBGM();
		fadelv[player] = 0;
		bgmlv = 19;
	}

	if(stage[player] <= 26){
	Stage_update = Stage_RankingCheck(player, 6);
	}

	// STAGE XX
	printFont(16-(1*((stage[player] >= 45) && (stage[player] <= 67) || (stage[player] >= 100)))-(1*((stage[player] >= 36) && (stage[player] <= 44))) + 24 * player - 12 * maxPlay, 6, "STAGE", 4);

	// 21面以降はEX○と表示 #1.60c7m4
	if( (stage[player] >= 20) && (stage[player] <= 26) ) {
		sprintf(string[0],"EX%d",stage[player] - 19);
	} else if( (stage[player] >= 27) && (stage[player] <= 44) ){
		sprintf(string[0],"EH%d",stage[player] - 26);
	} else if( (stage[player] >= 45) && (stage[player] <= 71) ){
		sprintf(string[0],"TGT%d",stage[player] - 44);
	} else if(stage[player] >= 100){
		sprintf(string[0],"FP%d",stage[player] - 99);
	} else {
		sprintf(string[0],"%d",stage[player] + 1);
	}
	printFont(22-(1*(stage[player] >= 45) && (stage[player] <= 67) || (stage[player] >= 100))-(1*(stage[player] >= 36) && (stage[player] <= 44)) + 24 * player - 12 * maxPlay, 6, string[0], 0);

	// CLEAR/TIME OVER
	if(sclear[player]) {
		printFont(17 + 24 * player - 12 * maxPlay, 8, "CLEAR", 0);
		if((stage[player] >= 0) && (stage[player] <= 26)){
		}
	} else if(stime[player] == 0) {
		printFont(16 + 24 * player - 12 * maxPlay, 8, "TIME OVER", 0);
		if((stage[player] >= 0) && (stage[player] <= 26)){
		}
	} else {
		printFont(17 + 24 * player - 12 * maxPlay, 8, "SKIPED", 0);
		if((stage[player] >= 0) && (stage[player] <= 26)){
		recFaultTime(player);//ステージNOTランキング用タイム
		}
		// リプレイ記録不可能になった事を表示 #1.60c7n8
		printFont(15 + 24 * player - 12 * maxPlay, 13, "THIS PLAY", 2);
		printFont(15 + 24 * player - 12 * maxPlay, 14, "  CAN'T BE", 2);
		printFont(15 + 24 * player - 12 * maxPlay, 15, "     SAVED", 2);
	}

	if(sclear[player]) {
		if(statusc[player * 10 + 0] == 1){
		for(i = 0; i < 10; i++)	//フィールド消去 C7T3.2
			for(j = 0; j < 22; j++) {
				fld[i + j * 10 + player * 220] = 0;
				fldt[i + j * 10 + player * 220] = 0;
				fldi[i + j * 10 + player * 220] = 0;
			}
		}
		// 花火
		if((statusc[player * 10 + 0] % 10 == 0) && (tomoyo_hanabi[player] < 10)) {
			PlaySE(35);
			objectCreate2(player, 7, Rand(80) + 72 + 192 * player - 96 * maxPlay, 32 + Rand(20), 0, 0, Rand(7)+1, 0);
			tomoyo_hanabi[player]++;
		}
		if(stage[player] < 100){
			// TIME EXTEND
			printSMALLFont( (15 + 24 * player - 12 * maxPlay)*8 + 8, 13*8, "Time Extend", 3 );

			// リミットタイム増加量を決める
			if(ctime[player] < 10 * 60) {
				ext = 10;
			}else if(ctime[player] < 20 * 60) {
				ext = 5;
			}

			sprintf(string[0],"%2d sec",ext);
			printSMALLFont( (15 + 24 * player - 12 * maxPlay)*8 + 8, 14*8 + 1, string[0], 0 );
		}else{
			printSMALLFont( (15 + 24 * player - 12 * maxPlay)*8 + 8, 13*8, "Score Bonus", 3 );
			sprintf(string[0],"%5d pts",FP_bonus[player]);
			printSMALLFont( (15 + 24 * player - 12 * maxPlay)*8 + 8, 14*8 + 1, string[0], 0 );
		}
		// CLEAR TIME
		printSMALLFont( (15 + 24 * player - 12 * maxPlay)*8 + 8, 16*8, "Clear Time", 3 );
		getTime(ctime[player]);
		printSMALLFont( (15 + 24 * player - 12 * maxPlay)*8 + 8, 17*8 + 1, string[0], 0 );

		if(Stage_update) {
			printFont(15 + 24 * player - 12 * maxPlay, 22, "STAGE", 2);
			printFont(15 + 24 * player - 12 * maxPlay, 23, "   RECORD", 2);
		}
	}
	if(stage[player] < 100){
		// LIMIT TIME
		if(ext_s[player] < (ext*60)){
			ext_s[player] = ext_s[player]+4;
		}
		if((timeextend_snd_cnt[player] < ext) && ((statusc[player * 10 + 0]-1) % 15 == 0)){
			PlaySE(42);
			timeextend_snd_cnt[player]++;
		}

		ltime_s[player] = ltime[player]+ext_s[player];
	}else{
		ltime_s[player] = ltime[player];
	}
	if(!fpbas_mode[player]){
		printSMALLFont( (15 + 24 * player - 12 * maxPlay)*8 + 8, 10*8, "Limit Time", 3 );
		getTime(ltime_s[player]);
		printSMALLFont( (15 + 24 * player - 12 * maxPlay)*8 + 8, 11*8 + 1, string[0], ((count % 4 / 2) * digitc[rots[player]])*(ext != 0) );
	}
	if(!hide_tomoyo_totaltime) {
		printSMALLFont( (15 + 24 * player - 12 * maxPlay)*8 + 8, 19*8, "Total Time", 3);
		getTime(time[player]);
		printSMALLFont( (15 + 24 * player - 12 * maxPlay)*8 + 8, 20*8 + 1, string[0], 0);
	}

	// クリア率計算
	if(!t_training[player]) clearp[player] = ((clearnum[player]*100) / (stage[player] + 1 - 27 *(start_stage[player] >= 27) ));

	// ボタンを押すと早く次のステージへ
	if( getPushState(player, 4) ) {
		statusc[player * 10 + 0] = 300;
	}

	// 次のステージ
	if(statusc[player * 10 + 0] >= 300) {
		if(start_stage[player] >= 27) {
			// 自作面 #1.60c7r6
			if((start_stage[player] >= 27) && (start_stage[player] < 45))
				laststage[player] = 44;	// H-Heart
			else if((start_stage[player] >= 45) && (start_stage[player] < 72))
				laststage[player] = 71;	// ACE-Target
			else if((start_stage[player] >= 69) && (start_stage[player] < 100))
				laststage[player] = 99;	// Edit stage
			else
				laststage[player] = 199;// F-Point
		} else if(t_noborder[player]) {
			// 足切りなしモードの場合は常にラストステージをEX7にする #1.60c7m5
			laststage[player] = 26;
		} else {
			/* クリア率でラストステージを決定する */
			if(clearp[player] < 90) {
				// 90%以下はEXなし（ラストステージは20）
				laststage[player] = 19;
			} else if(clearp[player] < 100) {
				// 90〜99%はEX3まで
				laststage[player] = 22;
			} else if((clearp[player] == 100)&(stage[player]==19)){
				// 100%の場合はステージ20までのタイムで決める
				if (time[player] < timelimit_tomoyo) {
					// 5分以内なら最後まで
					laststage[player] = 26;
					//EX7行きが決まれば流れ星
					objectCreate2(player, 8, Rand(20) + 180 + 192 * player - 96 * maxPlay, 20 + Rand(10), 0, 0, 0, 0);
					PlaySE(43);
				}else {
					// 5分超えている場合はEX5
					laststage[player] = 24;
				}
			}
		}
		// リミットタイム増加
		ltime[player] = ltime[player] + (ext * 60);

		if(stage[player] >= 100)
			sc[player] = sc[player] + FP_bonus[player];

		if( stage[player] == 19 ) ltime[player] = ltime[player] + (60 * 60);	// 20面クリアで1分増加 #1.60c7l9

		if( !t_training[player] ) {
			stage[player]++;	// トレーニングでないなら次のステージへ
		}
		// トレーニングBではNEXTCを開始時に戻す #1.60c7o8
		if( t_training[player] == 2 ) {
			nextc[player] = stage_nextc[player];
			next[player] = nextb[nextc[player] + player * 1400];
		}
		// 初期化
		tomoyoInitial(player);
		if((fpbas_mode[player]) || (repversw < 58)){
			if(lv[player] >= 7)lv[player] = 6;
			else if(lv[player] > 0)lv[player]--;
			sp[player] = lvTabletomoyohebo[lv[player]];
		}
		status[player] = 3;					// Ready
		statusc[player * 10] = 0;				// ステータスカウンタを0に
		statusc[player * 10 + 1] = 0;			//
		if((stage[player] >= 100) && (!t_training[player])){
			status[player] = 36;				// F-Pointセレクト
			statusc[player * 10] = 1;
			statusc[player * 10 + 1] = 0;
			statusc[player * 10 + 2] = 0;
			statusc[player * 10 + 3] = 0;
		}

		if( (stage[player] > laststage[player]) && (!t_training[player])) {
			// ラストステージクリアでエンディング
			// ただしトレーニングでは無し #1.60c7o7
			PlayWave(28);
			stage[player] = laststage[player];
			ending[player] = 1;
			status[player] = 13;
			if(laststage[player] == 199)
				sc[player] = sc[player] + 100000;
			end_f[player] = 1 + (stage[player] >= 26);
		} else {
			stage_nextc[player] = nextc[player];	// ステージスタート時のNEXTCを記憶 #1.60c7l9
		}
	}
}

// ステージクリア判定 #1.60c7l8
void tomoyoCheckStageClear(int32_t player) {
	if(gameMode[player] == 6) {
		// 残りプラチナブロック数を数える
		rest_pblock[player] = getPlatinumBlock(player);

		// プラチナブロックが0個ならクリアフラグON
		if( (!rest_pblock[player]) && (!sclear[player]) ) {
			sclear[player] = 1;
		}
	}
}

// フィールド内に残っているプラチナブロックの数を数える
int32_t getPlatinumBlock(int32_t player) {
	int32_t i, j, result;
	result = 0;

	for(i = 0; i < 10; i++)
		for(j = 0; j < 22; j++) {
			if(fld[i + j * 10 + player * 220] >= 11) {
				result++;
			}
		}
	if(tomoyo_rise[player]){	//せりあがりがONなら、まだせり上がっていない部分もカウント
		for(i = 0; i < 10; i++)
			for(j = upLineNo[player]; j <= 20; j++) {
				if(tomoyo_rise_fld[i + j * 10 + player * 200] >= 11)
					result++;
			}
	}
	return result;
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.18 - TOMOYOモード ステージエディタ
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
// statusc[player * 10]     : カーソル位置
// statusc[player * 10 + 1] : メニュー／フィールド
// statusc[player * 10 + 2] : 戻り先ステータス
// statusc[player * 10 + 3] : フィールド画面に入った瞬間ブロックを置いてしまうのを防ぐフラグ
void statTomoyoEditor(int32_t player) {
	int32_t tmp, bx1, by1;

	padRepeat(player);
	padRepeat2(player);

	/* ■メニュー画面■ */
	if( statusc[player * 10 + 1] == 0 ) {
		printFont(15 + 24 * player - 12 * maxPlay, 8 + statusc[player * 10] +(1*(statusc[player * 10] >= 13)) , "b", 1);

		sprintf(string[0],"STAGE:%2d",stage[player] + 1);
		printFont(16 + 24 * player - 12 * maxPlay, 8, string[0],  (statusc[player * 10] == 0) );

		printFont(16 + 24 * player - 12 * maxPlay, 9, "FIELD",    (statusc[player * 10] == 1) );

		printFont(16 + 24 * player - 12 * maxPlay,10, "MIRROR",   (statusc[player * 10] == 2) + (isfmirror[player] * 2) );
		printFont(16 + 24 * player - 12 * maxPlay,11, "ROLL ROLL",(statusc[player * 10] == 3) + (rollroll_flag[player] * 2) );
		printFont(16 + 24 * player - 12 * maxPlay,12, "X-RAY",    (statusc[player * 10] == 4) + (xray_flag[player] * 2) );
		printFont(16 + 24 * player - 12 * maxPlay,13, "COLOR",    (statusc[player * 10] == 5) + (color_flag[player] * 2) );
		printFont(16 + 24 * player - 12 * maxPlay,14, "BIG",      (statusc[player * 10] == 6) + (IsBig[player] * 2) );
		printFont(16 + 24 * player - 12 * maxPlay,15, "BLIND",    (statusc[player * 10] == 7) + (blind_flag[player] * 2) );
		sprintf(string[0],"SPEED:%4d",min_speed[player]);
		printFont(16 + 24 * player - 12 * maxPlay,16, string[0],   (statusc[player * 10] == 8) );
		printFont(16 + 24 * player - 12 * maxPlay,17, "THUNDER",   (statusc[player * 10] == 9) + (isthunder[player] * 2) );
		printFont(16 + 24 * player - 12 * maxPlay,18, "HIDE NEXT", (statusc[player * 10] == 10) + (hnext_flag[player] * 2) );
		printFont(16 + 24 * player - 12 * maxPlay,19, "BOOST FIRE",(statusc[player * 10] == 11) + (tomoyo_waits[player] * 2) );

		if(!tomoyo_rise[player])
			printFont(16 + 24 * player - 12 * maxPlay,20, "RISE:OFF",   (statusc[player * 10] == 12));
		else{
			printFont(16 + 24 * player - 12 * maxPlay,20, "RISE:ON",   (statusc[player * 10] == 12));
			sprintf(string[0],"INTER:%3d",tomoyo_rise[player]);
			printFont(16 + 24 * player - 12 * maxPlay,21, string[0],   (statusc[player * 10] == 12) + 4);
		}
		printFont(16 + 24 * player - 12 * maxPlay,22, " [LOAD]",     (statusc[player * 10] == 13) );
		printFont(16 + 24 * player - 12 * maxPlay,23, " [SAVE]",     (statusc[player * 10] == 14) );

		// ↑
		if( (mpc2[player] == 1) || ((mpc2[player] > tame3) && (mpc2[player] % tame4 == 0)) )
		if( getPressState(player, 0) ) {
			PlayWave(5);
			statusc[player * 10]--;
			if(statusc[player * 10] < 0) statusc[player * 10] = 14;
		}

		// ↓
		if( (mpc2[player] == 1) || ((mpc2[player] > tame3) && (mpc2[player] % tame4 == 0)) )
		if( getPressState(player, 1) ) {
			PlayWave(5);
			statusc[player * 10]++;
			if(statusc[player * 10] > 14) statusc[player * 10] = 0;
		}

		// ←
		if((mpc[player] == 1) || ((mpc[player] > tame1) && (mpc[player] % tame2 == 0)) || (getPressState(player, 6)))
		if(getPressState(player, 2)) {
			PlayWave(3);

			// STAGE
			if(statusc[player * 10] == 0) {
				stage[player]--;
				if(stage[player] < 0) stage[player] = 199;
			}
			// MIRROR
			if(statusc[player * 10] == 2) {
				isfmirror[player] = !isfmirror[player];
			}
			// ROLL ROLL
			if(statusc[player * 10] == 3) {
				rollroll_flag[player] = !rollroll_flag[player];
			}
			// X-RAY #1.60c7p9ex
			if(statusc[player * 10] == 4) {
				xray_flag[player] = !xray_flag[player];
			}
			// COLOR #1.60c7p9ex
			if(statusc[player * 10] == 5) {
				color_flag[player] = !color_flag[player];
			}
			// BIG #1.60c7l8
			if(statusc[player * 10] == 6) {
				IsBig[player] = !IsBig[player];
			}
			// BLIND
			if(statusc[player * 10] == 7) {
				blind_flag[player] = !blind_flag[player];
			}
			// 最低速度
			if(statusc[player * 10] == 8) {
				min_speed[player]--;
				if(min_speed[player] < 0) min_speed[player] = 1200;
			}
			if(statusc[player * 10] == 9) {
				isthunder[player] = !isthunder[player];
			}
			if(statusc[player * 10] == 10) {
				hnext_flag[player] = !hnext_flag[player];
			}
			if(statusc[player * 10] == 11) {
				tomoyo_waits[player] = !tomoyo_waits[player];
			}
			if(statusc[player * 10] == 12) {
				tomoyo_rise[player]--;
				if(tomoyo_rise[player] < 0) tomoyo_rise[player] = 99;
			}
		}

		// →
		if((mpc[player] == 1) || ((mpc[player] > tame1) && (mpc[player] % tame2 == 0)) || (getPressState(player, 6)))
		if(getPressState(player, 3)) {
			PlayWave(3);

			// STAGE
			if(statusc[player * 10] == 0) {
				stage[player]++;
				if(stage[player] > 199) stage[player] = 0;
			}
			// MIRROR
			if(statusc[player * 10] == 2) {
				isfmirror[player] = !isfmirror[player];
			}
			// ROLL ROLL
			if(statusc[player * 10] == 3) {
				rollroll_flag[player] = !rollroll_flag[player];
			}
			// X-RAY #1.60c7p9ex
			if(statusc[player * 10] == 4) {
				xray_flag[player] = !xray_flag[player];
			}
			// COLOR #1.60c7p9ex
			if(statusc[player * 10] == 5) {
				color_flag[player] = !color_flag[player];
			}
			// BIG #1.60c7l8
			if(statusc[player * 10] == 6) {
				IsBig[player] = !IsBig[player];
			}
			if(statusc[player * 10] == 7) {
				blind_flag[player] = !blind_flag[player];
			}
			if(statusc[player * 10] == 8) {
				min_speed[player]++;
				if(min_speed[player] > 1200) min_speed[player] = 0;
			}
			if(statusc[player * 10] == 9) {
				isthunder[player] = !isthunder[player];
			}
			if(statusc[player * 10] == 10) {
				hnext_flag[player] = !hnext_flag[player];
			}
			if(statusc[player * 10] == 11) {
				tomoyo_waits[player] = !tomoyo_waits[player];
			}
			if(statusc[player * 10] == 12) {
				tomoyo_rise[player]++;
				if(tomoyo_rise[player] > 99) tomoyo_rise[player] = 0;
			}
		}

		// Aボタン
		if( getPushState(player, 4) ) {
			PlayWave(10);

			// STAGE
			if(statusc[player * 10] == 0) {
				loadTomoyoStage(player, stage[player]);
			}
			// FIELD
			if(statusc[player * 10] == 1) {
				bx[player] = 0;					// X座標
				by[player] = 1;					// Y座標
				blk[player] = 0;				// 配置するブロック
				statusc[player * 10 + 1] = 1;		// ステータスカウンタを1にしてフィールド画面へ
				statusc[player * 10 + 3] = 1;		// そのままだと画面が切り替わった瞬間、ブロックを配置してしまうので
												// それを回避するためのフラグ
			}
			// RISE
			if(statusc[player * 10] == 12) {
				bx[player] = 0;					// X座標
				by[player] = 1;					// Y座標
				blk[player] = 0;				// 配置するブロック
				statusc[player * 10 + 1] = 2;		// ステータスカウンタを2にしてせり上がりフィールド画面へ
				statusc[player * 10 + 3] = 1;		// そのままだと画面が切り替わった瞬間、ブロックを配置してしまうので
												// それを回避するためのフラグ
			}
			// MIRROR
			if(statusc[player * 10] == 2) {
				isfmirror[player] = !isfmirror[player];
			}
			// ROLL ROLL
			if(statusc[player * 10] == 3) {
				rollroll_flag[player] = !rollroll_flag[player];
			}
			// X-RAY #1.60c7p9ex
			if(statusc[player * 10] == 4) {
				xray_flag[player] = !xray_flag[player];
			}
			// COLOR #1.60c7p9ex
			if(statusc[player * 10] == 5) {
				color_flag[player] = !color_flag[player];
			}
			// BIG #1.60c7l8
			if(statusc[player * 10] == 6) {
				IsBig[player] = !IsBig[player];
			}
			if(statusc[player * 10] == 7) {
				blind_flag[player] = !blind_flag[player];
			}
			if(statusc[player * 10] == 9) {
				isthunder[player] = !isthunder[player];
			}
			if(statusc[player * 10] == 10) {
				hnext_flag[player] = !hnext_flag[player];
			}
			if(statusc[player * 10] == 11) {
				tomoyo_waits[player] = !tomoyo_waits[player];
			}
			// LOAD
			if(statusc[player * 10] == 13) {
				loadTomoyoStage(player, stage[player]);
			}
			// SAVE
			if(statusc[player * 10] == 14) {
				saveTomoyoStage(player, stage[player]);
			}
		}

		// Bボタン
		if( getPushState(player, 5) ) {
			status[player] = statusc[player * 10 + 2];	// 戻り先へ
			statusc[player * 10] = 0;					// ステータスカウンタを0に
			statusc[player * 10 + 1] = 0;				//
			statusc[player * 10 + 2] = 0;
		}

	/* ■フィールド画面■ */
	} else if( statusc[player * 10 + 1] == 1 ){		// FIELD
		// カーソル表示
		bx1 = (bx[player] + 15 + 24 * player - 12 * maxPlay) * 8;
		by1 = (by[player] + 3) * 8;

		ExBltFastRect(3,  bx1 - 1, by1 - 1, 0, 34, 10, 10);
		ExBltFastRect(getBlockPlaneNo(player, blk[player]), bx1, by1, blk[player] * 8 - (blk[player] >= 10) * 80, 0, 8, 8);

		// ↑
		if( (mpc2[player] == 1) || ((mpc2[player] > tame3) && (mpc2[player] % tame4 == 0)) )
		if( getPressState(player, 0) ) {
			PlayWave(5);
			by[player]--;
			if(by[player] < 1) by[player] = fldsizeh[player];
		}

		// ↓
		if( (mpc2[player] == 1) || ((mpc2[player] > tame3) && (mpc2[player] % tame4 == 0)) )
		if( getPressState(player, 1) ) {
			PlayWave(5);
			by[player]++;
			if(by[player] > fldsizeh[player]) by[player] = 1;
		}

		// ←
		if( (mpc[player] == 1) || ((mpc[player] > tame1) && (mpc[player] % tame2 == 0)) )
		if( getPressState(player, 2) ) {
			PlayWave(5);

			// ブロック選択
			if( getPressState(player, 6) ) {
				blk[player]--;
				if(blk[player] < 0) blk[player] = 16;
			}
			// カーソル移動
			else {
				bx[player]--;
				if(bx[player] < 0) bx[player] = fldsizew[player] - 1;
			}
		}

		// →
		if( (mpc[player] == 1) || ((mpc[player] > tame1) && (mpc[player] % tame2 == 0)) )
		if( getPressState(player, 3) ) {
			PlayWave(5);

			// ブロック選択
			if( getPressState(player, 6) ) {
				blk[player]++;
				if(blk[player] > 16) blk[player] = 0;
			}
			// カーソル移動
			else {
				bx[player]++;
				if(bx[player] > fldsizew[player] - 1) bx[player] = 0;
			}
		}

		// Aボタンで配置
		if( getPressState(player, 4) && !statusc[player * 10 + 3] ) {
			if(fld[bx[player] + by[player] * fldsizew[player] + player * 220] != blk[player] + 1) {
				PlayWave(3);
			}
			fld[bx[player] + by[player] * fldsizew[player] + player * 220] = blk[player] + 1;
			fldt[bx[player] + by[player] * fldsizew[player] + player * 220] = -1;
		}
		if( !getPressState(player, 4) ) {
			statusc[player * 10 + 3] = 0;
		}

		// HOLDボタンでブロック消去
		if( getPressState(player, 7) ) {
			if(fld[bx[player] + by[player] * fldsizew[player] + player * 220] != 0) {
				PlayWave(3);
			}
			fld[bx[player] + by[player] * fldsizew[player] + player * 220] = 0;
			fldt[bx[player] + by[player] * fldsizew[player] + player * 220] = 0;
		}

		// Bボタンで戻る
		if( getPushState(player, 5) ) {
			statusc[player * 10 + 1] = 0;			// ステータスカウンタを0にしてメニューに戻る
			statusc[player * 10 + 3] = 0;
		}
	} else if( statusc[player * 10 + 1] == 2 ){		// RISE
		// カーソル表示
		bx1 = (bx[player] + 15 + 24 * player - 12 * maxPlay) * 8;
		by1 = (by[player] + 4) * 8;

		ExBltFastRect(3,  bx1 - 1, by1 - 1, 0, 34, 10, 10);
		ExBltFastRect(getBlockPlaneNo(player, blk[player]), bx1, by1, blk[player] * 8 - (blk[player] >= 10) * 80, 0, 8, 8);

		// ↑
		if( (mpc2[player] == 1) || ((mpc2[player] > tame3) && (mpc2[player] % tame4 == 0)) )
		if( getPressState(player, 0) ) {
			PlayWave(5);
			by[player]--;
			if(by[player] < 1) by[player] = fldsizeh[player];
		}

		// ↓
		if( (mpc2[player] == 1) || ((mpc2[player] > tame3) && (mpc2[player] % tame4 == 0)) )
		if( getPressState(player, 1) ) {
			PlayWave(5);
			by[player]++;
			if(by[player] > fldsizeh[player]) by[player] = 1;
		}

		// ←
		if( (mpc[player] == 1) || ((mpc[player] > tame1) && (mpc[player] % tame2 == 0)) )
		if( getPressState(player, 2) ) {
			PlayWave(5);

			// ブロック選択
			if( getPressState(player, 6) ) {
				blk[player]--;
				if(blk[player] < 0) blk[player] = 16;
			}
			// カーソル移動
			else {
				bx[player]--;
				if(bx[player] < 0) bx[player] = fldsizew[player] - 1;
			}
		}

		// →
		if( (mpc[player] == 1) || ((mpc[player] > tame1) && (mpc[player] % tame2 == 0)) )
		if( getPressState(player, 3) ) {
			PlayWave(5);

			// ブロック選択
			if( getPressState(player, 6) ) {
				blk[player]++;
				if(blk[player] > 16) blk[player] = 0;
			}
			// カーソル移動
			else {
				bx[player]++;
				if(bx[player] > fldsizew[player] - 1) bx[player] = 0;
			}
		}

		// Aボタンで配置
		if( getPressState(player, 4) && !statusc[player * 10 + 3] ) {
			if(tomoyo_rise_fld[bx[player] + (by[player]-1) * fldsizew[player] + player * 200] != blk[player] + 1) {
				PlayWave(3);
			}
			tomoyo_rise_fld[bx[player] + (by[player]-1) * fldsizew[player] + player * 200] = blk[player] + 1;
//			fldt[bx[player] + by[player] * fldsizew[player] + player * 210] = -1;
		}
		if( !getPressState(player, 4) ) {
			statusc[player * 10 + 3] = 0;
		}

		// HOLDボタンでブロック消去
		if( getPressState(player, 7) ) {
			if(tomoyo_rise_fld[bx[player] + (by[player]-1) * fldsizew[player] + player * 200] != 0) {
				PlayWave(3);
			}
			tomoyo_rise_fld[bx[player] + (by[player]-1) * fldsizew[player] + player * 200] = 0;
//			fldt[bx[player] + by[player] * fldsizew[player] + player * 210] = 0;
		}

		// Bボタンで戻る
		if( getPushState(player, 5) ) {
			statusc[player * 10 + 1] = 0;			// ステータスカウンタを0にしてメニューに戻る
			statusc[player * 10 + 3] = 0;
		}
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.19 - TOMOYOモード セレクト画面
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
// statusc[player * 10]     : カーソル位置
// statusc[player * 10 + 1] : プレイ／トレーニングA／トレーニングB／エディット
void statTomoyoSelect(int32_t player) {
	int32_t i;

	padRepeat(player);
	padRepeat2(player);

	printFont(15 + 24 * player - 12 * maxPlay, 6, "SELECT", 4);

	// ステージ番号
	printFont(15 + 24 * player - 12 * maxPlay, 8, "STAGE", 3);

	// 21面以降はEX○と表示 #1.60c7m4
	if( (start_stage[player] >= 20) && (start_stage[player] <= 26) ) {
		sprintf(string[0],"EX%d",start_stage[player] - 19);
	} else if( (start_stage[player] >= 27) && (start_stage[player] <= 44) ){
		sprintf(string[0],"E-HEART%d",start_stage[player] - 26);
	} else if( (start_stage[player] >= 45) && (start_stage[player] <= 71) ){
		sprintf(string[0],"ACE-TGT%d",start_stage[player] - 44);
	} else if( start_stage[player] >= 100 ){
		sprintf(string[0],"F-POINT %d",start_stage[player] - 99);
	} else {
		sprintf(string[0],"%2d",start_stage[player] + 1);
	}

	printFont(15 + 24 * player - 12 * maxPlay, 9, string[0],  (statusc[player * 10] == 0) * count % 9 );

	// NEXTC #1.60c7l7
	printFont(15 + 24 * player - 12 * maxPlay, 11, "NEXTC", 3);
	sprintf(string[0],"%d",start_nextc[player]);
	printFont(15 + 24 * player - 12 * maxPlay, 12, string[0],  (statusc[player * 10] == 1) * count % 9 );

	// ゲームモード
	printFont(15 + 24 * player - 12 * maxPlay, 14, "MODE", 3);
	if(statusc[player * 10 + 1] == 0)
		printFont(15 + 24 * player - 12 * maxPlay, 15, "PLAY", (statusc[player * 10] == 2) * count % 9);
	if(statusc[player * 10 + 1] == 1)
		printFont(15 + 24 * player - 12 * maxPlay, 15, "TRAINING-A", (statusc[player * 10] == 2) * count % 9);
	if(statusc[player * 10 + 1] == 2)
		printFont(15 + 24 * player - 12 * maxPlay, 15, "TRAINING-B", (statusc[player * 10] == 2) * count % 9);
	if(statusc[player * 10 + 1] == 3)
		printFont(15 + 24 * player - 12 * maxPlay, 15, "EDIT", (statusc[player * 10] == 2) * count % 9);

	// ランダムモード #1.60c7l9
	// DUAL対応 #1.60c7m1
	printFont(15 + 24 * player - 12 * maxPlay, 17, "RANDOM", 3);
	if(randommode[player])
		printFont(15 + 24 * player - 12 * maxPlay, 18, "ON", (statusc[player * 10] == 3) * count % 9);
	else
		printFont(15 + 24 * player - 12 * maxPlay, 18, "OFF", (statusc[player * 10] == 3) * count % 9);

	// 足切りON/OFF #1.60c7m5
	printFont(15 + 24 * player - 12 * maxPlay, 20, "BORDER", 3);
	if(t_noborder[player])
		printFont(15 + 24 * player - 12 * maxPlay, 21, "OFF", (statusc[player * 10] == 4) * count % 9);
	else
		printFont(15 + 24 * player - 12 * maxPlay, 21, "ON", (statusc[player * 10] == 4) * count % 9);

	// ↑
	if( (mpc2[player] == 1) || ((mpc2[player] > tame3) && (mpc2[player] % tame4 == 0)) )
	if( getPressState(player, 0) ) {
		PlayWave(5);
		statusc[player * 10]--;
		if(statusc[player * 10] < 0) statusc[player * 10] = 4;
	}

	// ↓
	if( (mpc2[player] == 1) || ((mpc2[player] > tame3) && (mpc2[player] % tame4 == 0)) )
	if( getPressState(player, 1) ) {
		PlayWave(5);
		statusc[player * 10]++;
		if(statusc[player * 10] > 4) statusc[player * 10] = 0;
	}

	// ←
	if((mpc[player] == 1) || ((mpc[player] > tame1) && (mpc[player] % tame2 == 0)) || (getPressState(player, 6)))
	if( getPressState(player, 2) ) {
		PlayWave(3);

		// 回転法則
		/*
		if(statusc[player * 10] == 0) {
			rots[player]--;
			if(rots[player] < 0) rots[player] = 3;
		}
		*/
		// ステージ番号
		if(statusc[player * 10] == 0) {
			start_stage[player]--;
			if(start_stage[player] < 0) start_stage[player] = 199;
			// ステージデータ読み込み #1.60c7l9
			loadTomoyoStage(player, start_stage[player]);
		}
		// NEXTC #1.60c7l7
		if(statusc[player * 10] == 1) {
			start_nextc[player]--;
			if(start_nextc[player] < 0) start_nextc[player] = 1399;
		}
		// モード
		if(statusc[player * 10] == 2) {
			statusc[player * 10 + 1]--;
			if(statusc[player * 10 + 1] < 0) statusc[player * 10 + 1] = 3;
		}
		// ランダムモード #1.60c7l9
		if(statusc[player * 10] == 3) {
			randommode[player] = !randommode[player];
			versusInit(player);
		}
		// 足切りON/OFF #1.60c7m5
		if(statusc[player * 10] == 4) {
			t_noborder[player] = !t_noborder[player];
		}
	}

	// →
	if((mpc[player] == 1) || ((mpc[player] > tame1) && (mpc[player] % tame2 == 0)) || (getPressState(player, 6)))
	if( getPressState(player, 3) ) {
		PlayWave(3);

		// 回転法則
		/*
		if(statusc[player * 10] == 0) {
			rots[player]++;
			if(rots[player] > 3) rots[player] = 0;
		}
		*/
		// ステージ番号
		if(statusc[player * 10] == 0) {
			start_stage[player]++;
			if(start_stage[player] > 199) start_stage[player] = 0;
			// ステージデータ読み込み #1.60c7l9
			loadTomoyoStage(player, start_stage[player]);
		}
		// NEXTC #1.60c7l7
		if(statusc[player * 10] == 1) {
			start_nextc[player]++;
			if(start_nextc[player] > 1399) start_nextc[player] = 0;
		}
		// モード
		if(statusc[player * 10] == 2) {
			statusc[player * 10 + 1]++;
			if(statusc[player * 10 + 1] > 3) statusc[player * 10 + 1] = 0;
		}
		// ランダムモード #1.60c7l9
		if(statusc[player * 10] == 3) {
			randommode[player] = !randommode[player];
			versusInit(player);
		}
		// 足切りON/OFF #1.60c7m5
		if(statusc[player * 10] == 4) {
			t_noborder[player] = !t_noborder[player];
		}
	}

	// Bボタン
	if( getPushState(player, 5) ) {
		for(i=0;i<220;i++) {
			// フィールド全消去 #1.60c7m2
			fld[i + 220 * player] = 0;
			fldt[i + 220 * player] = 0;
		}
		status[player] = 2;					// SOLO MODEモードセレクト
		statusc[player * 10] = 0;				// ステータスカウンタを0に
		statusc[player * 10 + 1] = 0;			//
	}

	// HOLDボタンでnextcを0に戻す
	if( getPushState(player, 7) ) {
		PlayWave(3);
		start_nextc[player] = 0;
	}

	// Aボタン
	if( getPushState(player, 4) ) {
		PlayWave(10);
		stage[player] = start_stage[player];
		if((start_stage[player] >= 27) && (start_stage[player] <= 44) )
			ltime[player] = 600 * 60;
		else if(start_stage[player] < 100)
			ltime[player] = 180 * 60;
		else
			ltime[player] = 1080 * 60;
		stage_nextc[player] = start_nextc[player];	// ステージスタート時のNEXTC #1.60c7l9

		bgmlv = setstartBGM(gameMode[player], player);		// BGM設定 #1.60c7o8

		if( statusc[player * 10 + 1] == 3 ) {
			// エディット
			status[player] = 18;					// ステージエディタ
			statusc[player * 10] = 1;				// ステータスカウンタを0に
			statusc[player * 10 + 1] = 0;			// ステータスカウンタを0に
			statusc[player * 10 + 2] = 19;		// 戻り先設定
		} else if( statusc[player * 10 + 1] == 2 ) {
			// トレーニングB
			t_training[player] = 2;				// トレーニングON
			status[player] = 1;					// ブロックシャッター
			statusc[player * 10] = 0;				// ステータスカウンタを0に
			statusc[player * 10 + 1] = 3;			// Ready
		} else if( statusc[player * 10 + 1] == 1 ) {
			// トレーニングA
			t_training[player] = 1;				// トレーニングON
			status[player] = 1;					// ブロックシャッター
			statusc[player * 10] = 0;				// ステータスカウンタを0に
			statusc[player * 10 + 1] = 3;			// Ready
		} else {
			// プレイ
			t_training[player] = 0;				// トレーニングOFF
			status[player] = 1;					// ブロックシャッター
			statusc[player * 10] = 0;				// ステータスカウンタを0に
			statusc[player * 10 + 1] = 3;			// Ready
		}
	}
	// NEXTをまとめてセット #1.60c7l9
	nextc[player] = start_nextc[player];
	next[player] = nextb[nextc[player] + player * 1400];
	setNextBlockColors(player, 1);		// NEXTブロックの色を設定 #1.60c7m9
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.20 - TOMOYOモード 結果表示
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
// statusc[player * 10]     : コンティニューするか
// statusc[player * 10 + 1] : コンティニュー残り時間
// statusc[player * 10 + 2] : 2人同時でプレイしているか
// statusc[player * 10 + 3] : リプレイセーブ表示
void statTomoyoResult(int32_t player) {
	int32_t tmp;

	statusc[player * 10 + 1]++;

	// 音楽を流す
	// 2人同時で重ならないように修正 #1.60c7m1
	if( (status[1 - player] == 0) && (!IsPlayWave(63)) && (wavebgm >= 1) ) PlayWave(63);

	// 警告音が鳴っていたら止める
	StopSE(40);

	// リプレイ保存
	if((textguide) && (!playback) && (!demo)){
		if(!player) printFont(13+24 * player - 12 * maxPlay, 26, "0-9 KEY:REPLAY", 0);
		else printFont(13+24 * player - 12 * maxPlay, 26, "Q-P KEY:REPLAY", 0);
	}

	// リプレイセーブ #1.60c7n8
	ReplaySaveCheck(player, 3);
	if(statusc[player * 10 + 3]) {
		sprintf(string[0], "SAVED : %02d", statusc[player * 10 + 3]);
		printFont(15 + 24 * player - 12 * maxPlay, 24, string[0], 5);
	}

	printFont(15 + 24 * player - 12 * maxPlay, 6, "PLAY DATA", 4);

	// STAGE
	ExBltRect(3, 120 + 192 * player - 96 * maxPlay, 64, 228, 112, 26, 7);

	if((stage[player] >= 26) && (ending[player])) {
		// ALLクリア
		printSMALLFont(182 + 192 * player - 96 * maxPlay, 64, "ALL", 0);
	} else if ((stage[player] > 19) && (stage[player] <= 26)) {
		// EX
		printSMALLFont(182 + 192 * player - 96 * maxPlay, 64, "EX", 0);
		sprintf(string[0], "%d", stage[player] - 19);
		printSMALLFont(194 + 192 * player - 96 * maxPlay, 64, string[0], 0);
	} else if ((stage[player] > 26) && (stage[player] <= 44)) {
		// E-Heart
		printSMALLFont(182-(6*(stage[player] - 26 >= 10)) + 192 * player - 96 * maxPlay, 64, "EH", 0);
		sprintf(string[0], "%d", stage[player] - 26);
		printSMALLFont(194-(6*(stage[player] - 26 >= 10)) + 192 * player - 96 * maxPlay, 64, string[0], 0);
	} else if ((stage[player] > 44) && (stage[player] <= 71)) {
		// ACE-Target
		printSMALLFont(176-(6*(stage[player] - 44 >= 10)) + 192 * player - 96 * maxPlay, 64, "TGT", 0);
		sprintf(string[0], "%d", stage[player] - 44);
		printSMALLFont(194-(6*(stage[player] - 44 >= 10)) + 192 * player - 96 * maxPlay, 64, string[0], 0);
	} else if (stage[player] >= 100) {
		// F-Point
		printSMALLFont(182-(6*(stage[player] - 99 >= 10))-(6*(stage[player] - 99 >= 100)) + 192 * player - 96 * maxPlay, 64, "FP", 0);
		sprintf(string[0], "%d", stage[player] - 99);
		printSMALLFont(194-(6*(stage[player] - 99 >= 10))-(6*(stage[player] - 99 >= 100)) + 192 * player - 96 * maxPlay, 64, string[0], 0);
	} else {
		// 通常
		sprintf(string[0], "%2d", stage[player] + 1);
		printSMALLFont(188 + 192 * player - 96 * maxPlay, 64, string[0], 0);
	}

	// TIME
	ExBltRect(3, 120 + 192 * player - 96 * maxPlay, 75, 180, 119, 19, 7);
	getTime(time[player]);
	printSMALLFont(152 + 192 * player - 96 * maxPlay, 75, string[0], 0);

	// CLEAR #1.60c7m5
	if((!t_training[player]) && (tomoyo_opt[player] == 0) && (stage[player] <= 26)) {
		ExBltRect(3, 120 + 192 * player - 96 * maxPlay, 86, 254, 112, 26, 7);
		sprintf(string[0], "%3d>", clearp[player] );
		printSMALLFont(176 + 192 * player - 96 * maxPlay, 86, string[0], 0);
	}
	// SCORE(F-Point)
	if(stage[player] >= 100){
		ExBltRect(3, 120 + 192 * player - 96 * maxPlay, 86, 154, 112, 26, 7);
		sprintf(string[0], "%7d", sc[player]);
		printSMALLFont(158 + 192 * player - 96 * maxPlay, 86, string[0], 0);
	}
	// コンティニュー選択
	if( (!playback) && (!ending[player]) ) {
		printSMALLFont(120 + 192 * player - 96 * maxPlay, 144, "CONTINUE", (statusc[player * 10] == 0) * fontc[rots[player]]);
		printSMALLFont(120 + 192 * player - 96 * maxPlay, 154, "END", (statusc[player * 10] == 1) * fontc[rots[player]]);

		sprintf(string[0], "%2d", 10 - (statusc[player * 10 + 1] / 60));
		printFont(17 + 24 * player - 12 * maxPlay, 22, "TIME", 6);
		printFont(21 + 24 * player - 12 * maxPlay, 22, string[0], 0);
	}

	// ↑↓
	if(getPushState(player, 0) || getPushState(player, 1)) {
		if(!ending[player]) {
			PlayWave(5);
			statusc[player * 10] = !statusc[player * 10];
		}
	}

	// Aボタン
	if( (getPushState(player, 4)) || (statusc[player * 10 + 1] >= 10 * 60) ) {
		StopAllBGM();

		if( playback ) {
			// リプレイ #1.60c7n8
			status[player] = 0;	// 参加待ち（タイトルへ）
		} else if( (!statusc[player * 10]) && (statusc[player * 10 + 1] < 10 * 60) && (!ending[player]) ) {
			// YES
			PlayWave(10);

			if((stage[player] >= 27) && (stage[player] <= 44) )
				ltime[player] = 600 * 60;
			else
				ltime[player] = 180 * 60;

			nextc[player] = stage_nextc[player];
			next[player] = nextb[nextc[player] + player * 1400];

			clearnum[player] = 0;
			clearp[player] = 0;

			// 初期化
			tomoyoInitial(player);

			// スタート時のステージを設定 #1.60c7n8
			start_stage[player] = stage[player];
			start_nextc[player] = stage_nextc[player];

			time2[player] = 0;					// TOMOYO用プレイタイムをリセット
			replay_save[player] = 1;			// リプレイセーブを可能にする

			status[player] = 1;					// ブロックシャッター
			statusc[player * 10] = 0;				// ステータスカウンタを0に
			statusc[player * 10 + 1] = 3;			// Ready
		} else {
			// NO
			status[player] = 11;				// ネームエントリ表示 #1.60c7p1
			statusc[player * 10] = 0;			// ステータスカウンタを0に
			statusc[player * 10 + 1] = 0;
			statusc[player * 10 + 2] = 0;
			statusc[player * 10 + 3] = 0;
			statusc[player * 10 + 4] = 0;
			statusc[player * 10 + 5] = 0;
		}
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.36 - TOMOYOモード F-Point用　ステージセレクト
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
// statusc[player * 10]     : モード（0=最初 1=途中）
// statusc[player * 10 + 1] : タイマー
// statusc[player * 10 + 2] : カーソル位置
// statusc[player * 10 + 3] : 現在のステージ
void statTomoyoSelect_FP(int32_t player) {
	int32_t i;

	if(statusc[player * 10 + 1] == 0){
		loadTomoyoStage(player, stage[player]);
		FP_bonus[player] = 1000 * (((stage[player]-100) / 4) + 1);
		if(repversw <= 46) FP_bonus[player] = 10800;
		statusc[player * 10 + 3] = stage[player] - 99;
		if(stage[player] == 190){	//ステージ90突破で流れ星
			StopAllBGM();
			PlayWave(43);
			objectCreate2(player, 8, Rand(20) + 180 + 192 * player - 96 * maxPlay, 20 + Rand(10), 0, 0, 0, 0);
		}
	}

	statusc[player * 10 + 1]++;

	padRepeat(player);
	padRepeat2(player);

	printFont(15 + 24 * player - 12 * maxPlay, 6, "SELECT", 4);

	// ステージ番号
	printFont(15 + 24 * player - 12 * maxPlay, 8, "    STAGE", 4);

	if(statusc[player * 10] == 0){	//最初
		printFont(15 + 24 * player - 12 * maxPlay, 7, "  FIRST", 4);

		sprintf(string[0],"F-POINT%3d",start_stage[player] - 99);
		printFont(15 + 24 * player - 12 * maxPlay, 14, string[0], 0);
		printFont(14 + 24 * player - 12 * maxPlay, 14, "<          >", count % 9);

		// ←
		if((mpc[player] == 1) || ((mpc[player] > tame1) && (mpc[player] % tame2 == 0)) || (getPressState(player, 6)))
		if( getPressState(player, 2) ) {
			PlayWave(3);

			// ステージ番号
			start_stage[player]--;
			if(start_stage[player] < 100) start_stage[player] = 189;
			if(repversw >= 47) FP_bonus[player] = 1000 * (((start_stage[player]-100) / 4) + 1);
			// ステージデータ読み込み #1.60c7l9
			loadTomoyoStage(player, start_stage[player]);
		}

		// →
		if((mpc[player] == 1) || ((mpc[player] > tame1) && (mpc[player] % tame2 == 0)) || (getPressState(player, 6)))
		if( getPressState(player, 3) ) {
			PlayWave(3);

			// ステージ番号
			start_stage[player]++;
			if(start_stage[player] > 189) start_stage[player] = 100;
			if(repversw >= 47) FP_bonus[player] = 1000 * (((start_stage[player]-100) / 4) + 1);
			// ステージデータ読み込み #1.60c7l9
			loadTomoyoStage(player, start_stage[player]);
		}
	} else {
		printFont(15 + 24 * player - 12 * maxPlay, 7, "   NEXT", 4);

		sprintf(string[0], "%2d", 10 - (statusc[player * 10 + 1] / 60));
		printFont(17 + 24 * player - 12 * maxPlay, 23, "TIME", 6);
		printFont(21 + 24 * player - 12 * maxPlay, 23, string[0], 0);

		for(i=0;i<=3;i++){
			if(statusc[player * 10 + 3] < 90){
				if(statusc[player * 10 + 3] + FP_next[i] <= 90)
				sprintf(string[0], "%2d", statusc[player * 10 + 3] + FP_next[i]);
				else sprintf(string[0], "90");
			}else{
				if(statusc[player * 10 + 3] + FP_next[i] <= 100)
					sprintf(string[0], "%2d", statusc[player * 10 + 3] + FP_next[i]);
				else sprintf(string[0], "100");
			}
			printFont(18 + 24 * player - 12 * maxPlay, 10 + (i * 2), string[0], 0);
		}

		printFont(15 + 24 * player - 12 * maxPlay, 10 + ( statusc[player * 10 + 2] * 2), "b", fontc[rots[player]]);

		// ↑
		if( (mpc2[player] == 1) || ((mpc2[player] > tame3) && (mpc2[player] % tame4 == 0)) )
		if( getPressState(player, 0) ) {
			PlayWave(5);
			statusc[player * 10 + 2]--;
			if(statusc[player * 10 + 2] < 0) statusc[player * 10 + 2] = 3;
			if(statusc[player * 10 + 3] < 90){
				if(statusc[player * 10 + 3] + FP_next[statusc[player * 10 + 2]] <= 90)
				stage[player] = statusc[player * 10 + 3] + FP_next[statusc[player * 10 + 2]] + 99;
				else stage[player] = 189;
				if(repversw >= 47) FP_bonus[player] = 1000 * (((stage[player]-100) / 4) + 1);
				loadTomoyoStage(player, stage[player]);
			}else{
				if(statusc[player * 10 + 3] + FP_next[statusc[player * 10 + 2]] <= 100)
					stage[player] = statusc[player * 10 + 3] + FP_next[statusc[player * 10 + 2]] + 99;
				else stage[player] = 199;
				if(repversw >= 47) FP_bonus[player] = 1000 * (((stage[player]-100) / 4) + 1);
				loadTomoyoStage(player, stage[player]);
			}
		}

		// ↓
		if( (mpc2[player] == 1) || ((mpc2[player] > tame3) && (mpc2[player] % tame4 == 0)) )
		if( getPressState(player, 1) ) {
			PlayWave(5);
			statusc[player * 10 + 2]++;
			if(statusc[player * 10 + 2] > 3) statusc[player * 10 + 2] = 0;
			if(statusc[player * 10 + 3] < 90){
				if(statusc[player * 10 + 3] + FP_next[statusc[player * 10 + 2]] <= 90)
				stage[player] = statusc[player * 10 + 3] + FP_next[statusc[player * 10 + 2]] + 99;
				else stage[player] = 189;
				if(repversw >= 47) FP_bonus[player] = 1000 * (((stage[player]-100) / 4) + 1);
				loadTomoyoStage(player, stage[player]);
			}else{
				if(statusc[player * 10 + 3] + FP_next[statusc[player * 10 + 2]] <= 100)
					stage[player] = statusc[player * 10 + 3] + FP_next[statusc[player * 10 + 2]] + 99;
				else stage[player] = 199;
				if(repversw >= 47) FP_bonus[player] = 1000 * (((stage[player]-100) / 4) + 1);
				loadTomoyoStage(player, stage[player]);
			}
		}
	}

	// Aボタン
	if( (getPushState(player, 4)) || ((statusc[player * 10] != 0) && (statusc[player * 10 + 1] > 600)) ) {
		PlayWave(10);
		if( statusc[player * 10] == 0 ) {
			// 最初
			stage[player] = start_stage[player];
			bgmlv = setstartBGM(gameMode[player], player);	//BGMセット
			if(repversw >= 47) FP_bonus[player] = 1000 * (((stage[player]-100) / 4) + 1);
			status[player] = 1;					// ブロックシャッター
			statusc[player * 10] = 0;				// ステータスカウンタを0に
			statusc[player * 10 + 1] = 3;			// Ready
			statusc[player * 10 + 2] = 0;
			statusc[player * 10 + 3] = 0;
		}else{
			//途中
			//BGM切り替え判定
			if(stage[player] <= 129) i = 8;
			else if(stage[player] <= 159) i = 14;
			else if(stage[player] <= 189) i = 9;
			else i = 19;

			if(bgmlv != i){
				bgmlv = i;
				changeBGM(0);
			}
			if(repversw >= 47) FP_bonus[player] = 1000 * (((stage[player]-100) / 4) + 1);
			status[player] = 3;					// Ready
			statusc[player * 10] = 0;				// ステータスカウンタを0に
			statusc[player * 10 + 1] = 0;			//
			statusc[player * 10 + 2] = 0;
			statusc[player * 10 + 3] = 0;
		}
	}
	if(( statusc[player * 10] == 0 ) && (getPushState(player, 5))) {		// Bボタンでモード選択画面に戻る
		sp[player] = 1;
		PlayWave(5);
		randommode[player] = 1;
		versusInit(player);				// NEXTを生成し直す
		randommode[player] = 0;
		status[player] = 1;				// ブロックシャッター実行
		statusc[player * 10] = 0;			// ステータスカウンタを0に
		statusc[player * 10 + 1] = 2;		// シャッター後はステータスNo.2
		relaymode[player] = 0;
	}
}
//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステージ上のプラチナブロック配置をランダムにする
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void randPlatinum(int32_t player, int32_t tgtnum){
	int32_t empty, i, j, put_flag[22], end_flag, put_num;

	if((tomoyo_rise[player]) || (tgtnum == 0)) return;
	for(i = 0; i < 22; i++) put_flag[i] = 0;
	put_num = 0;
	empty = 0;
	end_flag = 0;

	// プラチナブロックを普通のブロックにする
	for(i = 0; i < 220; i++) {
		if(fld[i + player * 220] >= 11) {
			fld[i + player * 220] = fld[i + player * 220] - 9;
		}
	}

	while(!end_flag){
		for(i = 0; i <= fldsizeh[player]; i++) {
			empty = 1;
			if((!put_flag[i]) || (tgtnum == 99)){
				for(j = 0; j < fldsizew[player]; j++) {
					if((fld[j + i * fldsizew[player] + player * 220] >= 2) && (fld[j + i * fldsizew[player] + player * 220] <= 8)) {
						empty = 0;
						if((tgtnum == 99) || (gameRand(10, player) == 0)) {
							fld[j + i * fldsizew[player] + player * 220] = fld[j + i * fldsizew[player] + player * 220] + 9;
							put_num++;
							put_flag[i] = 1;
							if((put_num >= tgtnum) && (tgtnum != 99)) return;
							if(tgtnum != 99) break;
						}
					}
				}
			}
			if(empty) put_flag[i] = 1;
		}
		end_flag = 1;
		for(i = 0; i < 22; i++){
			if(!put_flag[i]){
				end_flag = 0;
				break;
			}
		}
	}
}


//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  TOMOYOモード ステージ読み込み／保存
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
// ファイルフォーマット (4byte単位)
//   0〜  209 フィールドデータ（横10 * 縦21マス）
// 210        MIRROR
// 211        ROLL ROLL
// 212        DEATH BLOCK
// 213        X-RAY
// 214        COLOR
// 215〜249   未使用（自由に使用可能）
// 250〜449   せり上がりフィールド

/* ステージデータをロード */
void loadTomoyoStage(int32_t player, int32_t number) {
	int32_t i;

	FillMemory(&saveBuf, 50000 * 4, 0);

	sprintf(string[0], "config/stage/stage%02d.sav", number);
	LoadFile(string[0], &saveBuf, 450 * 4);

	// フィールド読み込み
	for(i=0;i<210;i++) {
		fld[(i+10) + 220 * player] = saveBuf[i];
		// 初期配置を見えるようにする
		fldt[(i+10) + 220 * player] = (fld[(i+10) + 220 * player] != 0) * -1;
	}

	for(i=0;i<200;i++)
		tomoyo_rise_fld[i + 200 * player] = saveBuf[i+250];

	if(gameMode[player] == 6) {
		// MIRROR
		isfmirror[player] = saveBuf[210]; // #1.60c7l7

		// ROLL ROLL
		rollroll_flag[player] = saveBuf[211]; // #1.60c7l7

		// DEATH BLOCK #1.60c7l8
		IsBig[player] = saveBuf[212];

		// X-RAY #1.60c7p9ex
		xray_flag[player] = saveBuf[213];

		// COLOR #1.60c7p9ex
		color_flag[player] = saveBuf[214];

		// BLIND C7T6.5
		blind_flag[player] = saveBuf[215];

		// 最低落下速度
		min_speed[player] = saveBuf[216];

		// サンダー
		isthunder[player] = saveBuf[217];


		// NEXT隠し
		hnext_flag[player] = saveBuf[219];

		tomoyo_waits[player] = saveBuf[220];
	}
	// せり上がり （ミッションでも使用）
	tomoyo_rise[player] = saveBuf[218];
}

/* ステージデータをセーブ */
void saveTomoyoStage(int32_t player, int32_t number) {
	int32_t i;

	FillMemory(&saveBuf, 50000 * 4, 0);

	// フィールドをセーブバッファにコピー
	for(i=0;i<210;i++) {
		saveBuf[i] = fld[(i+10) + 220 * player];
	}

	for(i=0;i<200;i++)
		saveBuf[i+250] = tomoyo_rise_fld[i + 200 * player];

	// MIRROR
	//saveBuf[210] = fmirror;
	saveBuf[210] = isfmirror[player]; // #1.60c7l7

	// ROLL ROLL
	//saveBuf[211] = rollroll];
	saveBuf[211] = rollroll_flag[player]; // #1.60c7l7

	// DEATH BLOCK #1.60c7l8
	saveBuf[212] = IsBig[player];

	// X-RAY #1.60c7p9ex
	saveBuf[213] = xray_flag[player];

	// COLOR #1.60c7p9ex
	saveBuf[214] = color_flag[player];

	// BLIND C7T6.5
	saveBuf[215] = blind_flag[player];

	// 最低落下速度
	saveBuf[216] = min_speed[player];

	// サンダー
	saveBuf[217] = isthunder[player];

	// せり上がり
	saveBuf[218] = tomoyo_rise[player];

	// NEXT隠し
	saveBuf[219] = hnext_flag[player];

	saveBuf[220] = tomoyo_waits[player];

	// 保存する
	sprintf(string[0], "config/stage/stage%02d.sav", number);
	SaveFile(string[0], &saveBuf, 450 * 4);
}
