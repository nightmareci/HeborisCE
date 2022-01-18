//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  デモ画面での操作状況
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲

#include "../script/include.h"

void showControl(void) {
	int32_t		zx, zy, bai;
	if(!onRecord[0]) return;
	zx = - getPressState(0, 2) + getPressState(0, 3);
	zy = - getPressState(0, 0) + getPressState(0, 1);
	bai = 10 + ((!zx) || (!zy)) * 4;

	ExBltRect(3, 20 + 96 * (maxPlay + 2 * ((gameMode[0] == 8) && (!maxPlay))), 126, 198, 38, 20, 20);				// レバーのカバー部分

	ExBltRect(3, 44 + 96 * (maxPlay + 2 * ((gameMode[0] == 8) && (!maxPlay))), 126, 224 + getPressState(0, 4) * 32, 0, 32, 32);	// Aボタン
	ExBltRect(3, 64 + 96 * (maxPlay + 2 * ((gameMode[0] == 8) && (!maxPlay))), 132, 224 + getPressState(0, 5) * 32, 0, 32, 32);	// Bボタン
	ExBltRect(3, 84 + 96 * (maxPlay + 2 * ((gameMode[0] == 8) && (!maxPlay))), 138, 224 + getPressState(0, 6) * 32, 0, 32, 32);	// Cボタン
	ExBltRect(3, 44 + 96 * (maxPlay + 2 * ((gameMode[0] == 8) && (!maxPlay))), 144, 224 + getPressState(0, 7) * 32, 0, 32, 32); 	// Holdボタンの操作状況 #1.60c

	ExBltRect(3, 14 + zx * bai + 96 * (maxPlay + 2 * ((gameMode[0] == 8) && (!maxPlay))), 120 + zy * bai, 192, 0, 32, 32);	// レバー
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  HIDDEN処理
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void hiddenProc(int32_t player) {
	if(hidden[player] == 0){
		if(hiddeny[player] < 22)
			hiddeny[player]++;
	}
	if(hidden[player] == 1) {
		hiddentm[player]++;
		if(hiddentm[player] > (24 - hiddeny[player]) * 120) {
			hiddentm[player] = 0;
			hiddeny[player]--;
			if(hiddeny[player] < 7)
				hiddeny[player] = 7;
			else
				hiddenc[player] = 0;
		}
	}

	if(hidden[player] == 2) {
		hiddentm[player]++;
		if(hiddentm[player] > (23 - hiddeny[player]) * 100) {
			hiddentm[player] = 0;
			hiddeny[player]--;
			if(hiddeny[player] < 5)
				hiddeny[player] = 5;
			else
				hiddenc[player] = 0;
		}
	}

	if(hidden[player] == 3) {
		hiddentm[player]++;
		if(hiddentm[player] > (22 - hiddeny[player]) * 60 + 60) {
			hiddentm[player] = 0;
			hiddeny[player]--;
			if(hiddeny[player] < 3)
				hiddeny[player] = 3;
			else
				hiddenc[player] = 0;
		}
	}

	if(hidden[player] == 4) {
		hiddentm[player]++;
		if(hiddentm[player] > (22 - hiddeny[player]) * 30 + 45) {
			hiddentm[player] = 0;
			hiddeny[player]--;
			if(hiddeny[player] < 3)
				hiddeny[player] = 3;
			else
				hiddenc[player] = 0;
		}
	}

	if(hidden[player] == 5) {
		hiddentm[player]++;
		if(hiddentm[player] > (22 - hiddeny[player]) * 10 + 30) {
			hiddentm[player] = 0;
			hiddeny[player]--;
			if(hiddeny[player] < 3)
				hiddeny[player] = 3;
			else
				hiddenc[player] = 0;
		}
	}

	if(hidden[player] == 6) {
		hiddentm[player]++;
		if(hiddentm[player] > (((22 - hiddeny[player]) * 2) + 15)) {
			hiddentm[player] = 0;
			hiddeny[player]--;
			if(hiddeny[player] < 3)
				hiddeny[player] = 3;
			else
				hiddenc[player] = 0;
		}
	}

	if(hidden[player] == 7) {
		hiddentm[player]++;
		if(hiddentm[player] > ((22 - hiddeny[player]) + 15)) {
			hiddentm[player] = 0;
			hiddeny[player]--;
			if(hiddeny[player] < 0)
				hiddeny[player] = 0;
			else
				hiddenc[player] = 0;
		}
	}
	if(hidden[player] == 8){
//		hiddeny[player] = 1;// hoge 全て隠す
		if(pause[player])
			hiddeny[player] = 21;
	}
	if(hidden[player] >= 9) shadow_timer_flag[player] = 1;//じわじわ隠れる
}


//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  シャドウタイマー解除 #1.60c7k7
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void disableShadowTimer(int32_t player) {
	int32_t i,j;
	shadow_timer_flag[player] = 0;

	// 全てのブロックが見えるようにする
	for(i = 0; i < 10; i++)
		for(j = 0; j < 22; j++) {
			fldt[i + j * 10 + player * 220] = -1;
		}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  タイムを時間表記にする (string[0]に格納してくれます)
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void getTime(int32_t time) {
	sprintf(string[0], "%02d:%02d:%02d", time / 3600, (time / 60) % 60, (time % 60) * 5 / 3);
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  タイムを秒までの時間表記にする (string[0]に格納してくれます)
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void getSTime(int32_t time) {
	sprintf(string[0], "%02d:%02d", (time / 60) % 60, (time % 60) * 5 / 3);
}
//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ピンチかどうかチェック
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void pinchCheck(int32_t player) {
	int32_t		i, j;

	pinch[player] = 0;
	if(ending[player]) return;

	for(i = 0; i <= 7; i++)
		for(j = 2; j <= 7; j++)
			if(fld[j + i * 7 + player * 220]) {
				pinch[player] = 1;
				pinch2[player] = !(pinch2[player]);
				break;
			}
}
//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ピンチ回復できたかどうかチェック2
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void pinchCheck2(int32_t player) {
	int32_t		i, j;

	pinch2[player] = 1;

	for(i = 0; i <= 18/*7*/; i++)
		for(j = 2; j <= 7; j++)
			if(fld[j + i * 7 + player * 220]) {
				pinch2[player] = 0;
				break;
			}
}
//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  背景表示
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void viewBack(void) {
	int32_t i, j, tmp;

	if(backno == 61) tmp = 12;
	else tmp = backno;

	// 通常描画
	if( (bgfadesw == 0) || (demo) ) {
		//ExBltFast(10 + backno, 0, 0);

		if(back_mov_f[tmp] == 0) {
			// 静止画
			ExBltFastRect(10 + backno, 0, 0, 0, 0, 320, 240);
		} else {
			// 動画
			ExBltFastRect(10 + backno, 0, 0, (back_acnt / 10) * 320, (back_acnt % 10) * 240, 320, 240);

			if(count % back_mov_inter[tmp] == 0)
				back_acnt++;

			if(back_acnt > back_mov_f[tmp] - 1)
				back_acnt = 0;
		}

		return;
	}

	// フェード無効時の背景切り替え
	// if(!enable_bgfade && bgfadesw) {

	if(bgfadesw) {
		if((backno >= bg_max) && ((gameMode[0] == 8) &&
		((mission_type[c_mission] == 20) || (mission_type[c_mission] == 22) || (mission_type[c_mission] == 29)) &&
		((mission_type[c_mission-1] == 20) || (mission_type[c_mission-1] == 22) || (mission_type[c_mission-1] == 29)))) {
			backno = bg_max;
			bgfadesw = 0;
		}
		//ExBltFast(10 + backno, 0, 0);

		if(back_mov_f[tmp] == 0) {
			// 静止画
			ExBltFastRect(10 + backno, 0, 0, 0, 0, 320, 240);
		} else {
			// 動画
			ExBltFastRect(10 + backno, 0, 0, (back_acnt / 10) * 320, (back_acnt % 10) * 240, 320, 240);

			if(count % back_mov_inter[tmp] == 0)
				back_acnt++;

			if(back_acnt > back_mov_f[tmp] - 1)
				back_acnt = 0;
		}

		if((backno < bg_max) || (gameMode[0] == 8) || (overcount != 0)){
		if(bgfade_cnt < 14){
			bgfade_cnt++;
			for(i=0;i<5*getDrawRate();i++){
				for(j=0;j<5*getDrawRate();j++){
					BltRect(72,i*64,j*48,0,bgfade_cnt*48,64,48);
				}
			}
			if(bgfade_cnt == 6) {
				backno++;
				if((overcount != 0) || (status[0] == 23)) backno = 0;
				else if(gameMode[0] == 8){
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
				}
				back_acnt = 0;
			}
		}else{
		if(backno >= bg_max) backno = bg_max;
			bgfadesw = 0;
		bgfade_cnt = 0;
		}
		}
		return;
	}
}


//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  スコア表示（SMALL）
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void viewScoreSmall(void) {
	int32_t		i, color,tmp, j,k[2],zoomx[2],zoomy[2],gradecolor[2],bps[2],bps1[2],bps2[2],add;

	for(i = 0; i < 1 + maxPlay; i++) {
	if( ((status[i] != 2) || ((status[i] == 2) && (statusc[i * 10 + 2] != 0))) && (status[i] != 0) && ((status[i] !=1) || ((status[i] ==1) && (statusc[i * 10 + 1] != 2))) ){
		if((i) && ((gameMode[0] == 8) || ((playback) && (gameMode[0] != 4)) )) return;
		color = (count % 4 / 2) * ((sp[i] >= 1200) || (isboost[i])) * digitc[rots[i]];
		gradecolor[i] = (count % 4 / 2) * ((sp[i] >= 1200) || (isboost[i]));
		if(sc[i] > 9999999) sc[i] = 9999999;
		if(li[i] > 9999) li[i] = 9999;
		if((gameMode[i]!=10) && (gameMode[i] > 6) && (lv[i] > 999)) lv[i] = 999;

		if(!hide_score) {
			add = 20*((hnext[i] >= 4) && (!i));
//GRADE
			if( (gameMode[i] >= 1) && (gameMode[i] <= 3)&&(examination[i]==0)) {
				if( ((gameMode[i] <= 2) && (enable_grade[i] != 0) && (enable_grade[i] != 4) && (!examination[i])) ||
					((gameMode[i] == 3) && ( (grade[i] > 0) && (!devil_minus[i]) )) )
					ExBltRect(85, 208+add + 70 * i - 96 * maxPlay, 32, 0, 42, 35, 7);
					//段位(エフェクト中)
				if(gflash[i] > 108) {
					k[i] =  gflash[i] -96;
					zoomx[i] = (90 * k[i] /12 - 90) / 2;
					zoomy[i] = (24 * k[i] /12 - 24) / 2;
					k[i] = 65536 * k[i] / 12;
					if(((gameMode[i]==1)||(gameMode[i]==2))&&(enable_grade[i]!=4)){
					// 非DEVIL
						if(grade[i]==32) {
							if(enable_grade[i]==1){//GM
							ExBltRectR(26,188-zoomx[i]+add + 54 * i - 96 * maxPlay, 40-zoomy[i], 450 ,(gradecolor[i]*216)+180,90,32,k[i],k[i]);
							}else{
							// Gm
							ExBltRectR(26,188-zoomx[i]+add + 54 * i - 96 * maxPlay, 40-zoomy[i], 450 ,(gradecolor[i]*216)+150,90,30,k[i],k[i]);
							}
						} else if( grade[i] >= 28 ) {
							// MK〜MM
							ExBltRectR(26,188-zoomx[i]+add + 52 * i - 96 * maxPlay, 40-zoomy[i],450 ,(gradecolor[i]*216)+((grade[i]-28)*25),90,24,k[i],k[i]);
						} else if( grade[i] == 27 ) {
							if(enable_grade[i] == 2){
							//M
							ExBltRectR(26,188-zoomx[i]+add + 64 * i - 96 * maxPlay, 40-zoomy[i],450 ,(gradecolor[i]*216)+((1)*25),90,24,k[i],k[i]);
							}else{
							//m
							ExBltRectR(26,188-zoomx[i]+add + 64 * i - 96 * maxPlay, 40-zoomy[i],450 ,(gradecolor[i]*216)+((0)*24),90,24,k[i],k[i]);
							}
						} else if( grade[i] >= 18 ) {
							// m1〜m9
							ExBltRectR(26,180-zoomx[i]+add + 62 * i - 96 * maxPlay, 40-zoomy[i], 180 ,(gradecolor[i]*216)+((grade[i]-18)*24),90,24,k[i],k[i]);
						} else if( grade[i] >= 9 ) {
							// S1〜S9
							ExBltRectR(26,180-zoomx[i]+add + 62 * i - 96 * maxPlay, 40-zoomy[i], 90 ,(gradecolor[i]*216)+((grade[i]-9)*24),90,24,k[i],k[i]);
						} else {
							// 9〜1
							ExBltRectR(26,180-zoomx[i]+add + 62 * i - 96 * maxPlay, 40-zoomy[i], 0 ,(gradecolor[i]*216)+(grade[i]*24),90,24,k[i],k[i]);
						}
					}else if(gameMode[i]==3){
						// DEVIL
						if( (grade[i] > 0) || (!devil_minus[i]) ) {
							// S1〜S13
							if(grade[i]== 15)  //Gm
								ExBltRectR(26,188- zoomx[i] +add + 54 * i - 96 * maxPlay, 40- zoomy[i], 450 ,(gradecolor[i]*216)+150,90,30,k[i],k[i]);
							else if(grade[i] == 14)  //m
								ExBltRectR(26,188- zoomx[i] +add + 58 * i - 96 * maxPlay, 40- zoomy[i],450 ,(gradecolor[i]*216)+((0)*24),90,24,k[i],k[i]);
							else if(grade[i] >= 10)  //S10〜S13と2つ飛ばしてgod
								ExBltRectR(26,180- zoomx[i] +add + 62 * i - 96 * maxPlay, 40- zoomy[i], 360 ,(gradecolor[i]*216)+((grade[i]-10)*24) + (24 * (grade[i] == 16)),90,24 + (4 * (grade[i] == 16)),k[i],k[i]);
							else if(grade[i] > 0)//S9以下
								ExBltRectR(26,180- zoomx[i] +add + 62 * i - 96 * maxPlay, 40- zoomy[i], 90 ,(gradecolor[i]*216)+((grade[i]-1)*24),90,24,k[i],k[i]);
							else  //1（DOOMのみ）
								ExBltRectR(26,180- zoomx[i] +add + 62 * i - 96 * maxPlay, 40- zoomy[i], 0 ,(gradecolor[i]*216)+192,90,24,k[i],k[i]);
						}
					}
				}else{//(エフェクト以外)
					if(((gameMode[i]==1)||(gameMode[i]==2))&&(enable_grade[i]!=4)){
					// 非DEVIL
						if(grade[i]==32) {
							if(enable_grade[i]==1){
								//GM
								ExBltRect(26,188+add + 54 * i - 96 * maxPlay, 40, 450 ,(gradecolor[i]*216)+180,90,32);
							}else{
								// Gm
								ExBltRect(26,188+add + 54 * i - 96 * maxPlay, 40, 450 ,(gradecolor[i]*216)+150,90,30);
							}
						} else if( grade[i] >= 28 ) {
							// MK〜MM
							ExBltRect(26,188+add + 52 * i - 96 * maxPlay, 40,450 ,(gradecolor[i]*216)+((grade[i]-28)*24),90,24);
						} else if( grade[i] == 27 ) {
							if(enable_grade[i] == 2){
							//M
							ExBltRect(26,188+add + 64 * i - 96 * maxPlay, 40,450 ,(gradecolor[i]*216)+24,90,24);
							}else{
							//m
							ExBltRect(26,188+add + 64 * i - 96 * maxPlay, 40,450 ,(gradecolor[i]*216),90,24);
							}
						} else if( grade[i] >= 18 ) {
							// m1〜m9
							ExBltRect(26,180+add + 62 * i - 96 * maxPlay, 40, 180 ,(gradecolor[i]*216)+((grade[i]-18)*24),90,24);
						} else if( grade[i] >= 9 ) {
							// S1〜S9
							ExBltRect(26,180+add + 62 * i - 96 * maxPlay, 40, 90 ,(gradecolor[i]*216)+((grade[i]-9)*24),90,24);
						} else {
							// 9〜1
							ExBltRect(26,180+add + 62 * i - 96 * maxPlay, 40, 0 ,(gradecolor[i]*216)+(grade[i]*24),90,24);
						}
					}else if(gameMode[i]==3){
						// DEVIL
						if( (grade[i] > 0) || (!devil_minus[i]) ) {
							// S1〜S13
							if(grade[i]== 15)  //Gm
								ExBltRect(26,188+add + 54 * i - 96 * maxPlay, 40, 450 ,(gradecolor[i]*216)+150,90,30);
							else if(grade[i] == 14)  //m
								ExBltRect(26,188+add + 58 * i - 96 * maxPlay, 40,450 ,(gradecolor[i]*216)+((0)*24),90,24);
							else if(grade[i] >= 10)  //S10〜S13
								ExBltRect(26,180+add + 62 * i - 96 * maxPlay, 40, 360 ,(gradecolor[i]*216)+((grade[i]-10)*24) + (24 * (grade[i] == 16)),90,24 + (4 * (grade[i] == 16)));
							else if(grade[i] > 0) //S9以下
								ExBltRect(26,180+add + 62 * i - 96 * maxPlay, 40, 90 ,(gradecolor[i]*216)+((grade[i]-1)*24),90,24);
							else if(status[i] != 2)  //1（DOOMのみ）
								ExBltRect(26,180+add + 62 * i - 96 * maxPlay, 40, 0 ,(gradecolor[i]*216)+192,90,24);
						}
					}
				}
				if((gameMode[i] == 3) && ( (grade[i] > 0) || (!devil_minus[i]) ))
					ExBltRect(85, 208+add + 66 * i - 96 * maxPlay, 32, 0, 42, 35, 7);
			}
// QUALIFY
			if((gameMode[i] == 1) &&(examination[i]) && (status[i] != 30)) {
				ExBltRect(85, 208+add + 66 * i - 96 * maxPlay, 32, 70, 7*10, 35, 7);
				if(exam_grade[i]==32) {
					if(enable_grade[i]==1){
						//GM
						ExBltRect(26,188+add + 54 * i - 96 * maxPlay, 40, 450 ,(gradecolor[i]*216)+180,90,32);
					}else{
						// Gm
						ExBltRect(26,188+add + 54 * i - 96 * maxPlay, 40, 450 ,(gradecolor[i]*216)+150,90,30);
					}
				} else if( exam_grade[i] >= 28 ) {
					// MK〜MM
					ExBltRect(26,188+add + 52 * i - 96 * maxPlay, 40,450 ,(gradecolor[i]*216)+((exam_grade[i]-28)*24),90,24);
				} else if( exam_grade[i] == 27 ) {
					if((enable_grade[i] == 2)||(enable_grade[i] == 4)){
					//M
					ExBltRect(26,188+add + 64 * i - 96 * maxPlay, 40,450 ,(gradecolor[i]*216)+24,90,24);
					}else{
					//m
					ExBltRect(26,188+add + 64 * i - 96 * maxPlay, 40,450 ,(gradecolor[i]*216),90,24);
					}
				} else if( exam_grade[i] >= 18 ) {
					// m1〜m9
					ExBltRect(26,180+add + 62 * i - 96 * maxPlay, 40, 180 ,(gradecolor[i]*216)+((exam_grade[i]-18)*24),90,24);
				} else if( exam_grade[i] >= 9 ) {
					// S1〜S9
					ExBltRect(26,180+add + 62 * i - 96 * maxPlay, 40, 90 ,(gradecolor[i]*216)+((exam_grade[i]-9)*24),90,24);
				} else {
					// 9〜1
					ExBltRect(26,180+add + 62 * i - 96 * maxPlay, 40, 0 ,(gradecolor[i]*216)+(exam_grade[i]*24),90,24);
				}
			}
// BEGINNER
			if (gameMode[i] == 0){
				//TAMAYA
				if(!novice_mode[i]){
					ExBltRect(85, 208+add + 65 * i - 96 * maxPlay, 32, 35, 7*11, 35, 7);
					if(i) sprintf(string[0], "%7d", hanabi_total[i]);
					else sprintf(string[0], "%d", hanabi_total[i]);
					printSMALLFont(208+add + 52 * i - 96 * maxPlay, 40, string[0], color);
				}
				//SCORE
				ExBltRect(3, 208+add + 70 * i - 96 * maxPlay, 50, 154, 112, 26, 7);
				if(i) sprintf(string[0], "%7d", sc[i]);
				else sprintf(string[0], "%d", sc[i]);
				printSMALLFont(208+add +55 * i - 96 * maxPlay, 58, string[0], color);
				//LINES
				ExBltRect(85, 208+add + 73 * i - 96 * maxPlay, 68, 0, 10*7, 35, 7);
				if(i) sprintf(string[0], "%7d", li[i]);
				else sprintf(string[0], "%d", li[i]);
				printSMALLFont(208+add +55 * i - 96 * maxPlay, 76, string[0], color);
			}

// MASTER & 20G
			if((gameMode[i] >= 1) && (gameMode[i] <= 2)) {
				//GRADE INFO
				if(enable_grade[i]==1){
					ExBltRect(85, 208+add + 48 * i - 96 * maxPlay, 86, 0, 7*29, 100, 14);
					if(grade[i] < 17){	// 次の段位
						if(i) sprintf(string[0], "%6d", gscore[grade[i]]);
						else sprintf(string[0], "%d", gscore[grade[i]]);
						printSMALLFont(220+add + 48 * i - 96 * maxPlay, 94, string[0], color);
					}else {	// 次の段位
						sprintf(string[0], ";;;;;;");
						printSMALLFont(220+add + 48 * i - 96 * maxPlay, 94, string[0], color);
					}
				}else if(enable_grade[i] == 2){	// 現在のポイント
					ExBltRect(85, 208+add + 61 * i - 96 * maxPlay, 86, 70, 12*7, 35, 7);
					if(i) {sprintf(string[0], "%7d", gpoint[i]-((gpoint[i]*2) * (gpoint[i] < 0)));
					}else{ sprintf(string[0], "%d", gpoint[i]-((gpoint[i]*2) * (gpoint[i] < 0)));
					}
					if(gpoint[i]>=0){
						printSMALLFont(208+add + 52 * i - 96 * maxPlay, 94, string[0], color);
					}else{
						printSMALLFont(208+add + 52 * i - 96 * maxPlay, 94, string[0], 2);
					}
				}else if(enable_grade[i] == 4){	// 現在のスピード
					ExBltRect(85, 208+add + 62 * i - 96 * maxPlay, 86, 70, 11*7, 35, 7);
					if(i) sprintf(string[0], "%7d", tr2[i]);
					else sprintf(string[0], "%d", tr2[i]);
					printSMALLFont(208+add +52 * i - 96 * maxPlay, 94, string[0], color);
				}
				//SCORE
				ExBltRect(3, 208+add + 70 * i - 96 * maxPlay, 68, 154, 112, 26, 7);
				if(i) sprintf(string[0], "%7d", sc[i]);
				else sprintf(string[0], "%d", sc[i]);
				printSMALLFont(208+add +55 * i - 96 * maxPlay, 76, string[0], color);
				//LINES
				ExBltRect(85, 208+add + 73 * i - 96 * maxPlay, 104, 0, 10*7, 35, 7);
				if(i) sprintf(string[0], "%7d", li[i]);
				else sprintf(string[0], "%d", li[i]);
				printSMALLFont(208+add +55 * i - 96 * maxPlay, 112, string[0], color);
			}

// DEVIL
			if(gameMode[i]==3){
				//SCORE
				ExBltRect(3, 208+add + 70 * i - 96 * maxPlay, 68, 154, 112, 26, 7);
				if(i) sprintf(string[0], "%7d", sc[i]);
				else sprintf(string[0], "%d", sc[i]);
				printSMALLFont(208+add +55 * i - 96 * maxPlay, 76, string[0], color);
				//LINES
				ExBltRect(85, 208+add + 73 * i - 96 * maxPlay, 86, 0, 10*7, 35, 7);
				if(i) sprintf(string[0], "%7d", li[i]);
				else sprintf(string[0], "%d", li[i]);
				printSMALLFont(208+add +55 * i - 96 * maxPlay, 94, string[0], color);
			}

// VERSUS or ITEM MODE
			if(item_name[i])	//アイテム名
				ExBltRect(28, 80 + i * 192+(96*(!maxPlay)), 0, 0, (item_name[i] - 1) * 16, 48, 16);
			if( (gameMode[i] == 4) || (item_mode[i])) {
				if(gameMode[i] == 4){
					//WINS
					ExBltRect(85, 208+add + 82 * i - 96, 32, 70, 7*13, 25, 7);
					if(i) sprintf(string[0], "%2d", vs_win[i]);
					else sprintf(string[0], "%d", vs_win[i]);
					printSMALLFont(208+add + 84 * i - 96, 40, string[0], color);
					//POINTS
					for(j = 1; j <= winpoint; j++)
						ExBltRect(54,228 + add + 44 * i - 96,24 + (12 * j),12 * (j <= vs_points[i]),75,12,12);
					// アイテムゲージ
					if(!noitem) {
						//ITEM
						ExBltRect(85, 208+add + 78 * i - 96, 57, 0, 56, 18, 7);
						if(i) sprintf(string[0], "%2d", item_inter[i] - item_g[i] );
						else sprintf(string[0], "%d", item_inter[i] - item_g[i] );
						printSMALLFont(208+add + 84 * i - 96, 65, string[0], color);
						ExBltRect(54,210 + 79 * i - 96,80,0,0,12,75);//中身
						ExBltRect(54,210 + 79 * i - 96,153-((70*item_g[i])/item_inter[i]),12,72-((70*item_g[i])/item_inter[i]),12,((70*item_g[i])/item_inter[i]));//目隠し

						if(ismiss[i]){//×のしるし
							ExBltRect(3,208 + 83 * i - 96, 50, 274, 370, 27, 27);
							if(repversw >= 62){
								ExBltRect(3,228 + 38 * i - 96 * maxPlay, 154, 274, 370, 27, 27);
							}
						}if (itemappear_c[i] > 0){  //フラッシュ
							if((itemappear_c[i]%2)==0)
								ExBltRect(54,210 + 79 * i - 96,80,24,0,12,75);
							itemappear_c[i]--;
						}
					}
				} else if(item_mode[i]){//アイテムモードの時はメダルと被るので左に配置
					ExBltRect(54,98 + 304 * i - 96 * maxPlay,80,0,0,12,75);//中身
					ExBltRect(54,98 + 304 * i - 96 * maxPlay,153-((70*item_g[i])/item_inter[i]),12,72-((70*item_g[i])/item_inter[i]),12,((70*item_g[i])/item_inter[i]));//目隠し
					if(ismiss[i]){//×のしるし
						if(repversw < 62) ExBltRect(3,98 + 304 * i - 96 * maxPlay, 50, 274, 370, 27, 27);
					}
					if (itemappear_c[i] > 0){  //フラッシュ
						if((itemappear_c[i]%2)==0)
							ExBltRect(54,98 + 304 * i - 96 * maxPlay,80,24,0,12,75);
						itemappear_c[i]--;//100
					}
				}
			}
// PRACTICE
			if(gameMode[i] == 5){
				//SCORE
				ExBltRect(3, 208+add + 70 * i - 96 * maxPlay, 32, 154, 112, 26, 7);
				if(i) sprintf(string[0], "%7d", sc[i]);
				else sprintf(string[0], "%d", sc[i]);
				printSMALLFont(208+add +55 * i - 96 * maxPlay, 40, string[0], color);
				//LINES
				ExBltRect(85, 208+add + 73 * i - 96 * maxPlay, 50, 0, 10*7, 35, 7);
				if(i) sprintf(string[0], "%7d", li[i]);
				else sprintf(string[0], "%d", li[i]);
				printSMALLFont(208+add +55 * i - 96 * maxPlay, 58, string[0], color);
			}
// TOMOYO
			if( gameMode[i] == 6) {
				// STAGE
				ExBltRect(3, 208+add + 70 * i - 96 * maxPlay, 32, 228, 112, 26, 7);

				if((stage[i] >= 26) && (ending[i] == 3)){
					// ALL（表示条件を変更 C7T3.2）
					printSMALLFont(208+add + 78 * i - 96 * maxPlay, 40, "ALL", count % 9);
				} else if ((stage[i] > 19) && (stage[i] <= 26)) {
					// EX
					sprintf(string[0], "EX%d", stage[i] - 19);
					printSMALLFont(208+add + 78 * i - 96 * maxPlay, 40, string[0], color);
				} else if ((stage[i] > 26) && (stage[i] <= 44)) {
					// EHeart C7T3.2
					sprintf(string[0], "EH%d", stage[i] - 26);
					printSMALLFont(208+add + 78 * i - 96 * maxPlay, 40, string[0], color);
				} else if ((stage[i] > 44) && (stage[i] <= 71)) {
					// ACE-TGT C7T3.2
					sprintf(string[0], "TGT%d", stage[i] - 44);
					printSMALLFont(208+add-(6*(i)) + 78 * i - 96 * maxPlay, 40, string[0], color);
				} else if (stage[i] >= 99) {
					// F-Point
					sprintf(string[0], "FP%d", stage[i] - 99);
					printSMALLFont(208+add-(6*(i)) + 78 * i - 96 * maxPlay, 40, string[0], color);
				} else {
					// 通常
					if(i) sprintf(string[0], "%2d", stage[i] + 1);
					else sprintf(string[0], "%d", stage[i] + 1);
					printSMALLFont(208+add + 84 * i - 96 * maxPlay, 40, string[0], color);
				}
				ExBltRect(85, 208+add + 75 * i - 96 * maxPlay, 50, 35, 5*7, 21, 7);
				sprintf(string[0], "%d", rest_pblock[i] );
				printSMALLFont(208+add + 90 * i - 96 * maxPlay, 58, string[0], color);
				if(stage[i] <= 26){
				// CLEAR率
					if((!t_training[i]) && (stage[i] <= 26)) {
						ExBltRect(85, 208+add + 70 * i - 96 * maxPlay, 68, 0, 4*7, 26, 7);
						if(i) sprintf(string[0], "%3d>", clearp[i] );
						else sprintf(string[0], "%d>", clearp[i] );
						printSMALLFont(208+add + 72 * i - 96 * maxPlay, 76, string[0], color);
					}
				}
				// SCORE(F-Point)
				if(stage[i] >= 99){
					ExBltRect(85, 208+add + 70 * i - 96 * maxPlay, 68, 35, 7*7, 26, 7);
					if(i) sprintf(string[0], "%7d", sc[i]);
					else sprintf(string[0], "%d", sc[i]);
					printSMALLFont(208+add + 55 * i - 96 * maxPlay, 76, string[0], color);
					//枠の下に表示してあるボーナス
					printFont(14 + 24 * i - 12 * maxPlay, 25, "BONUS", fontc[rots[i]]);
					sprintf(string[0], "%5d", FP_bonus[i] );
					printFont(20 + 24 * i - 12 * maxPlay, 25, string[0], color);
				}
				if((!maxPlay)&&(!ending[i])) {
					// BESTセクションタイム
					viewbestSTtimes(i);
				}
				// MIRRORなどの各種ギミック表示
				// DUALだと長いので短縮した #1.60c7l9 stageの下にしました
				if( tomoyo_waits[i])
					printSMALLFont(208+add + 60 * i - 96 * maxPlay, 85, "BOOST", fontc[rots[i]]);
				else if( isfmirror[i] )
					printSMALLFont(208+add + 60 * i - 96 * maxPlay, 85, "Mirror", fontc[rots[i]]);
				else if( rollroll_flag[i] )
					printSMALLFont(208+add + 60 * i - 96 * maxPlay, 85, "RollX2", fontc[rots[i]]);
				else if( xray_flag[i] )
					printSMALLFont(208+add + 66 * i - 96 * maxPlay, 85, "X Ray", fontc[rots[i]]);
				else if( color_flag[i] )
					printSMALLFont(208+add + 66 * i - 96 * maxPlay, 85, "Color", fontc[rots[i]]);	// #1.60c7p9ex改造
				else if( IsBig[i] )
					printSMALLFont(208+add + 66 * i - 96 * maxPlay, 85, "Death", fontc[rots[i]]);
				else if( blind_flag[i] )
					printSMALLFont(208+add + 66 * i - 96 * maxPlay, 85, "Blind", fontc[rots[i]]);
				else if( isthunder[i] )
					printSMALLFont(208+add + 66 * i - 96 * maxPlay, 85, "Thunder", fontc[rots[i]]);
				else if( tomoyo_rise[i] )
					printSMALLFont(208+add + 66 * i - 96 * maxPlay, 85, "Rise", fontc[rots[i]]);

				if(!hide_tomoyoinf && (status[i] >= 3) && (status[i] < 18) || (status[i] == 19) || (status[i] == 20)) {
					// NEXTC
					printFont(26+2*((hnext[i] >= 4) && (!i)) + 7 * i - 12 * maxPlay, 14, "NEXTC", fontc[rots[i]]);
					sprintf(string[0],"%d",nextc[i]);
					printFont(26+2*((hnext[i] >= 4) && (!i)) + 11 * i - 12 * maxPlay, 15, string[0], color);

					// 固定した回数
					printFont(26+2*((hnext[i] >= 4) && (!i)) + 7 * i - 12 * maxPlay, 16, "BDOWN", fontc[rots[i]]);
					sprintf(string[0],"%d",bdowncnt[i]);
					printFont(26+2*((hnext[i] >= 4) && (!i)) + 11 * i - 12 * maxPlay, 17, string[0], color);

					// 消去した回数
					printFont(26+2*((hnext[i] >= 4) && (!i)) + 7 * i - 12 * maxPlay, 18, "ERASE", fontc[rots[i]]);
					sprintf(string[0],"%d",erasecnt[i]);
					printFont(26+2*((hnext[i] >= 4) && (!i)) + 11 * i - 12 * maxPlay, 19, string[0], color);
				}
			}
// ACE & OLD STYLE
			if(gameMode[i]==7){
				//LEVEL
				ExBltRect(85, 208+add + 70 * i - 96 * maxPlay, 32, 0, 7*9, 26, 7);
				if(i) sprintf(string[0], "%2d", lv[i]);
				else sprintf(string[0], "%d", lv[i]);
				printSMALLFont(208+add + 84 * i - 96 * maxPlay, 40, string[0], color);
				if(anothermode[i] != 3){
					//TOTAL TIME
					ExBltRect(85, 208+add + 54 * i - 96 * maxPlay, 50, 35, 13*7, 25, 7);
					ExBltRect(85, 231+add + 54 * i - 96 * maxPlay, 50, 35, 12*7, 19, 7);
					getTime(time[i]);
					printSMALLFont((26 + 6 * i - 12 * maxPlay)*8+add, 58, string[0], color);
					// BEST TIME
					if(ranking_type==1){
						viewbesttimes(i);//（本体はranking2.c）
					}else if(ranking_type==2){
						viewbesttimes3(i,(26 + 8 * i - 12 * maxPlay)*8+add,68,0);
					}
				}else{	//OLD STYLE
					ExBltRect(85, 208+add + 73 * i - 96 * maxPlay, 50, 0, 10*7, 35, 7);
					if(i) sprintf(string[0], "%7d", li[i]);
					else sprintf(string[0], "%d", li[i]);
					printSMALLFont(208+add +52 * i - 96 * maxPlay, 58, string[0], color);

					ExBltRect(3, 208+add + 70 * i - 96 * maxPlay, 68, 154, 112, 26, 7);
					if(i) sprintf(string[0], "%7d", sc[i]);
					else sprintf(string[0], "%d", sc[i]);
					printSMALLFont(208+add +52 * i - 96 * maxPlay, 76, string[0], color);
				}
			}
// MISSION
			if(gameMode[i] == 8){
				ExBltRect(85, 208+add + 54 * i - 96 * maxPlay, 50, 35, 13*7, 25, 7);
				ExBltRect(85, 231+add + 54 * i - 96 * maxPlay, 50, 35, 12*7, 19, 7);
				getTime(time[i]);
				printSMALLFont((26 + 6 * i - 12 * maxPlay)*8+add, 58, string[0], color);
			}
// SIMPLE (STANDARD)
			if(gameMode[i] == 9){
				if(std_opt[i]<=1){
					if((relaymode[i]) && (!playback)){
						ExBltRect(85, 208+add + 70 * i - 96 * maxPlay, 32, 70, 105, 26, 7);
						if(i) sprintf(string[0], "%2d", relayround[i] + (1 * (relayround[i] < 9)));
						else sprintf(string[0], "%d", relayround[i] + (1 * (relayround[i] < 9)));
						printSMALLFont(208+add + 84 * i - 96 * maxPlay, 40, string[0], color);
						if(status[i] != 2){
							ExBltRect(87,208 + 48 * i - 96 * maxPlay, 102, (time[i]%6)*48 ,(time[i]%30/6)*48,48,48);
							ExBltRect(85,208 + 70 * i - 96 * maxPlay, 105, 35, 7*6, 22, 7);
							ExBltRect(55,201 + 48 * i - 96 * maxPlay, 121 - (3 * (getDrawRate() == 1)), 64*rots[i] ,
									32*fontc[rots[i]] + (7 * (getDrawRate() == 1)),64,12 + (5 * (getDrawRate() == 1)));
						}
					}else{
						/* BEST TIME STDモードはこっち*/
						if(ranking_type==1){
						viewbesttimes(i);//（本体はranking2.c）
						}else if(ranking_type==2){
						viewbesttimes3(i,(26 + 7 * i - 12 * maxPlay)*8+add,32,0);
						}
					}
					// LINES/min
					if(i) {
						ExBltRect(85, 208+add + 69 * i - 96 * maxPlay, 50, 0, 7*33,35, 14);
					}else{
						ExBltRect(85, 208+add + 64 * i - 96 * maxPlay, 50, 0, 7*10,35, 7);
						ExBltRect(85, 238+add + 45* i - 96 * maxPlay, 50, 70, 0,7, 7);
						ExBltRect(85, 244+add + 45 * i - 96 * maxPlay, 50, 70, 7*9,35, 7);
					}
					if(time[i]>120){//開始二秒は表示させない
						bps[i] = (li[i] * 100 *60*60) / (time[i]);
						bps1[i] = bps[i] / 100;//整数
						bps2[i] = bps[i] % 100;//下三桁
						if(i) {sprintf(string[0], "%3d?", bps1[i]);
						}else {sprintf(string[0], "%d?", bps1[i]);
						}
						printSMALLFont(208+add + 62 * i - 96 * maxPlay, 58+10*i, string[0], color);
						//下2桁
						if(bps2[i]>=10){
							if(i) {sprintf(string[0], "%2d", bps2[i]);
							}else {sprintf(string[0], "%d", bps2[i]);
							}
						}else{
							if(i) {sprintf(string[0], "0%d", bps2[i]);
							}else {sprintf(string[0], "0%d", bps2[i]);
							}
						}
						printSMALLFont(218+((bps1[i]>=10)*8)+add + 52 * i - 96 * maxPlay, 58+10*i, string[0], color);
					}else{
						if(i) sprintf(string[0], "%2d", 0);
						else sprintf(string[0], "%d", 0);
						printSMALLFont(208+add + 52 * i - 96 * maxPlay, 58+10*i, string[0], color);

					}

					//BLOCK/sec
					if(i) {
						ExBltRect(85, 208+add + 69 * i - 96 * maxPlay, 70, 0, 7*35,35, 14);
					}else{
						ExBltRect(85, 208+add + 69 * i - 96 * maxPlay, 70, 0, 7*2,35, 7);
						ExBltRect(85, 234+add + 50 * i - 96 * maxPlay, 70, 70, 0,7, 7);
						ExBltRect(85, 240+add + 50 * i - 96 * maxPlay, 70, 0, 7*12,35, 7);
					}

					if(time[i]>120){
						bps[i] = (bdowncnt[i] * 10000*60) / (time[i]);
						bps1[i] = bps[i] / 10000;//整数
						bps2[i] = bps[i] % 10000;//下三桁
						if(i) {
							sprintf(string[0], "%2d?", bps1[i]);
						}else {
							sprintf(string[0], "%d?", bps1[i]);
						}
						printSMALLFont(208+add + 84 * i - 96 * maxPlay, 78+8*i, string[0], color);
						//下4桁
						if(bps2[i]>=1000){
							if(i) {sprintf(string[0], "%4d", bps2[i]);
							}else {sprintf(string[0], "%d", bps2[i]);
							}
						}else if(bps2[i]>=100){
							if(i) {sprintf(string[0], "0%3d", bps2[i]);
							}else {sprintf(string[0], "0%d", bps2[i]);
							}
						}else if(bps2[i]>=10){
							if(i) {sprintf(string[0], "00%2d", bps2[i]);
							}else {sprintf(string[0], "00%d", bps2[i]);
							}
						}else{
							if(i) {sprintf(string[0], "000%d", bps2[i]);
							}else {sprintf(string[0], "000%d", bps2[i]);
							}
						}
					printSMALLFont(218+add + 84 * i - 96 * maxPlay, 78+8*i, string[0], color);
					}else{
						if(i) sprintf(string[0], "%2d", 0);
						else sprintf(string[0], "%d", 0);
						printSMALLFont(208+add + 84 * i - 96 * maxPlay, 78+8*i, string[0], color);
					}
				}else{
					//LEVEL
					ExBltRect(85, 208+add + 70 * i - 96 * maxPlay, 32, 0, 7*9, 26, 7);
					if(i) sprintf(string[0], "%2d", lv[i]);
					else sprintf(string[0], "%d", lv[i]);
					printSMALLFont(208+add + 84 * i - 96 * maxPlay, 40, string[0], color);
				}
				if(std_opt[i]==2){
				//SQUARE
					ExBltRect(85, 208+add + 64 * i - 96 * maxPlay, 50, 35, 7*9,33, 7);
					if(i) sprintf(string[0], "%2d", squarecnt[i]);
					else sprintf(string[0], "%d", squarecnt[i]);
					printSMALLFont(208+add + 84 * i - 96 * maxPlay, 58, string[0], color);
				} else if(std_opt[i]==3){
					if(i) {
						ExBltRect(85, 208+add + 69 * i - 96 * maxPlay, 50, 0, 7*37,35, 14);
					}else{
						ExBltRect(85, 208+add + 58 * i - 96 * maxPlay, 50, 35, 7*7,35, 7);
						ExBltRect(85, 234+add + 34 * i - 96 * maxPlay, 50, 70, 0,7, 7);
						ExBltRect(85, 240+add + 34 * i - 96 * maxPlay, 50, 0, 7*10,35, 7);
					}
					//SCORE/ライン

					if((time[i]>120)&&(li[i]>0)){
						bps1[i] = sc[i] / li[i];//整数
						bps2[i] = sc[i] % li[i];//下三桁
						if(i) {sprintf(string[0], "%2d?", bps1[i]);
						}else {sprintf(string[0], "%d?", bps1[i]);
						}
						printSMALLFont(208+add + 84 * i - 96 * maxPlay, 58+8*i, string[0], color);

							if(bps2[i]>=10){
								if(i) {sprintf(string[0], "%2d", bps2[i]);
								}else {sprintf(string[0], "%d", bps2[i]);
								}
							}else{
								if(i) {sprintf(string[0], "0%d", bps2[i]);
								}else {sprintf(string[0], "0%d", bps2[i]);
								}
							}//整数の値によって下三桁をずら
							printSMALLFont(218+(8*(bps1[i]>=10))+(8*(bps1[i]>=100))+(8*(bps1[i]>=1000))+add + 84 * i - 96 * maxPlay, 58+8*i, string[0], color);
						}else{
							if(i) sprintf(string[0], "%2d", 0);
							else sprintf(string[0], "%d", 0);
							printSMALLFont(208+add + 84 * i - 96 * maxPlay, 58+8*i, string[0], color);
						}
					}
				}
			}
// ORIGINAL
			if(gameMode[i] == 10){
				//LEVEL
				ExBltRect(85, 208+add + 70 * i - 96 * maxPlay, 32, 0, 7*9, 26, 7);
				if(i) sprintf(string[0], "%2d", lv[i]);
				else sprintf(string[0], "%d", lv[i]);
				printSMALLFont(208+add + 84 * i - 96 * maxPlay, 40, string[0], color);
				//LINES
				ExBltRect(85, 208+add + 73 * i - 96 * maxPlay, 50, 0, 10*7, 35, 7);
				if(i) sprintf(string[0], "%7d", li[i]);
				else sprintf(string[0], "%d", li[i]);
				printSMALLFont(208+add +52 * i - 96 * maxPlay, 58, string[0], color);
				//SCORE
				ExBltRect(3, 208+add + 70 * i - 96 * maxPlay, 68, 154, 112, 26, 7);
				if(i) sprintf(string[0], "%7d", sc[i]);
				else sprintf(string[0], "%d", sc[i]);
				printSMALLFont(208+add +55 * i - 96 * maxPlay, 76, string[0], color);
			}


			// レベル部分
			if(dtc && (gameMode[i] <= 9)) {
					// TGMシリーズ風カウンタ	#1.60c
				if((!fpbas_mode[i])&&(gameMode[i]!=5)) {
					if((gameMode[i]>=7)&&(gameMode[i]<=9)){
						if((gameMode[i] != 8) || (mission_type[c_mission] != 23)){
							// NORM
							if(gameMode[i]!=9){
								ExBltRect(85,217 + 66 * i - 96 * maxPlay, 161, 35, 7*2, 22, 7);//normの文字画像
							}else{
								ExBltRect(85,217 + 66 * i - 96 * maxPlay, 161, 0, 7*5, 25, 7);//GOALの文字
							}
							/* 分し */
							if(gameMode[i] == 7) sprintf(string[0], "%3d", li[i]);
							else if(gameMode[i] == 8)sprintf(string[0], "%3d", c_norm[i]);
							else if(gameMode[i] == 9) {
                                if(std_opt[i]<2){
                                    sprintf(string[0], "%3d", li[i]);
                                }else{
                                    sprintf(string[0], "%3d", c_norm[i]);
                                }
                            }
							printSMALLFont(218 + 61 * i - 96 * maxPlay, 169, string[0], color);

							/* 横線 */
							ExBltRect(3,218 + 61 * i - 96 * maxPlay,180, 38 + (24 * ((sp[i] >= 1200) || (isboost[i]))),34 ,24,3);
							if((sp[i] <= 600) && (!isboost[i])) {
								ExBltFastRect(3,219 + 61 * i - 96 * maxPlay,181, 60- (( sp[i] * 24 ) / 660 ),37 ,22,1);
							}else{
								ExBltFastRect(3,219 + 62 * i - 96 * maxPlay,181, 38,37 ,22,1);
							}
							/* 分ぼ */
							if((gameMode[i] == 7)&&(anothermode[i]!=3)) sprintf(string[0], "%3d", lv[i] * 10);
							else if((gameMode[i] == 7)&&(anothermode[i]==3)) sprintf(string[0], ";;;");
							else if(gameMode[i] == 8)sprintf(string[0], "%3d", mission_norm[c_mission]);
							else if(gameMode[i] == 9){
								if(std_opt[i] == 0)sprintf(string[0], "%3d", 40);
								else if(std_opt[i] == 1)sprintf(string[0], ";;;");
								else if(std_opt[i] == 2)sprintf(string[0], "%3d", lv[i]*10);
								else if(std_opt[i] == 3)sprintf(string[0], "%3d",lv[i]*5);
							}
							printSMALLFont(218 + 61 * i - 96 * maxPlay, 185, string[0], color);
						}
					}else if((gameMode[i]==4)&&(wintype==1)){//line
						ExBltRect(85, 208 + 75 * i - 96 * maxPlay, 161, 35, 7*2, 22, 7);//normの文字画像
						/* 分子 */
						sprintf(string[0], "%3d", li[i]);
						printSMALLFont(210 + 68 * i - 96 * maxPlay, 169, string[0], color);

						ExBltRect(3,210 + 68 * i - 96 * maxPlay,180, 38 + (24 * ((sp[i] >= 1200) || (isboost[i]))),34 ,24,3);
						// 中身
						if((sp[i] <= 600) && (!isboost[i])) {
							ExBltFastRect(3,211 + 68 * i - 96 * maxPlay,181, 60 - (( sp[i] * 24 ) / 660 ),37 ,22,1);
						} else {
							ExBltFastRect(3,211 + 68 * i - 96 * maxPlay,181, 38,37 ,22,1);
						}
						sprintf(string[0], "%3d", vs_goal/10);
						printSMALLFont(210 + 68 * i - 96 * maxPlay, 185, string[0], color);
					}else{//LINE

						ExBltRect(85, 208 + 70 * i - 96 * maxPlay, 161, 0, 63 + (210 * ((gameMode[i] == 3) && (!devil_minus[i]))), 26, 7);//LEVELの文字画像
						/* 分子 */
						sprintf(string[0], "%3d", tc[i]);

						// -1なら0と表示する(内部的には最初は-1になっている)#1.60c7i7
						if(tc[i] == -1) {
							sprintf(string[0], "%3d", 0);
						}
						printSMALLFont(210 + 68 * i - 96 * maxPlay, 169, string[0], color);

						/* 横線 */
						// 横線の枠 #1.60c7o3
						ExBltRect(3,210 + 68 * i - 96 * maxPlay,180, 38 + (24 * ((sp[i] >= 1200) || (isboost[i]))),34 ,24,3);
						// 中身
						if((sp[i] <= 600) && (!isboost[i])) {
							ExBltFastRect(3,211 + 68 * i - 96 * maxPlay,181, 60 - (( sp[i] * 24 ) / 660 ),37 ,22,1);
						} else {
							ExBltFastRect(3,211 + 68 * i - 96 * maxPlay,181, 38,37 ,22,1);
						}

						/* 分母 */
						if(ending[i]) {
							// BEGINNERとDEVILに対応したつもり#1.60c7g4
							// とりカンにも対応したつもり#1.60c7g7
							sprintf(string[0], "%3d", tc[i]);
						} else if((gameMode[i]==0)&&(novice_mode[i])){
							sprintf(string[0], "%3d", 300);
						} else if((gameMode[i]==0)&&(!novice_mode[i])){
							sprintf(string[0], "%3d", 200);
						} else if((gameMode[i]==4)&&(wintype==0)){
							sprintf(string[0], "%3d", vs_goal);
						} else if((gameMode[i]==4)&&(wintype==1)){
							sprintf(string[0], "%3d", vs_goal/10);
						}else {
							// DEVILでの表示修正#1.60c7h5
							sprintf(string[0], "%3d", tc[i] / 100 * 100 + 100 - (tc[i] >= 900) *(gameMode[i] <= 2));
						}
						printSMALLFont(210 + 68 * i - 96 * maxPlay, 185, string[0], color);
					}
				}else if(gameMode[i] == 5){
						if(p_goaltype==2){//ライン
							ExBltRect(85, 208 + 68 * i - 96 * maxPlay, 161, 35, 7*2, 22, 7);//normの文字画像
							/* 分子 */
							sprintf(string[0], "%3d", li[i]);

							// -1なら0と表示する(内部的には最初は-1になっている)#1.60c7i7
							if(li[i] == -1) {
								sprintf(string[0], "%3d", 0);
							}
						}else if(p_goaltype==3){//ブロック
							ExBltRect(85, 208 + 70 * i - 96 * maxPlay, 161, 0, 7*2, 30, 7);//BLOCKの文字画像
							/* 分子 */
							sprintf(string[0], "%3d", bdowncnt[i]);

								// -1なら0と表示する(内部的には最初は-1になっている)#1.60c7i7
							if(tc[i] == -1) {
								sprintf(string[0], "%3d", 0);
							}
						}else{
							ExBltRect(85, 208 + 70 * i - 96 * maxPlay, 161, 0, 7*9, 26, 7);//LEVELの文字画像
							/* 分子 */
							sprintf(string[0], "%3d", tc[i]);

							// -1なら0と表示する(内部的には最初は-1になっている)#1.60c7i7
							if(tc[i] == -1) {
								sprintf(string[0], "%3d", 0);
							}
						}

						printSMALLFont(210 + 68 * i - 96 * maxPlay, 169, string[0], color);

						/* 横線 */
						// 横線の枠 #1.60c7o3
						ExBltRect(3,210 + 68 * i - 96 * maxPlay,180, 38 + (24 * ((sp[i] >= 1200) || (isboost[i]))),34 ,24,3);
						// 中身
						if((sp[i] <= 600) && (!isboost[i])) {
							ExBltFastRect(3,211 + 68 * i - 96 * maxPlay,181, 60 - (( sp[i] * 24 ) / 660 ),37 ,22,1);
						} else {
							ExBltFastRect(3,211 + 68 * i - 96 * maxPlay,181, 38,37 ,22,1);
							}

						/* 分母 */
						if(p_goaltype==1) {
							sprintf(string[0], "%3d", p_goaltypenumlist[p_goaltypenum]*10);
						} else if((p_goaltype==2)||(p_goaltype==3)){
							sprintf(string[0], "%3d", p_goaltypenumlist[p_goaltypenum]);
						} else {
							// DEVILでの表示修正#1.60c7h5
							sprintf(string[0], "%3d", tc[i] / 100 * 100 + 100 - (tc[i] >= 900) *(gameMode[i] <= 2));
						}
							printSMALLFont(210 + 68 * i - 96 * maxPlay, 185, string[0], color);
				}else{
					// HEBORISレベルの表示位置をTGMレベルと同じにした #1.60c7i2
					ExBltRect(85, 208 + 70 * i - 96 * maxPlay, 161, 0, 7*9, 26, 7);//LEVELの文字画像
					if(i) sprintf(string[0], "%4d", lv[i]);
					else sprintf(string[0], "%d", lv[i]);
					printSMALLFont(208 + 72 * i - 96 * maxPlay, 169, string[0], color);
				}
			}
			/* メダル表示 */
			// 条件分岐を簡略化
			if((gameMode[i] != 0)&&(gameMode[i] != 4) && (gameMode[i] <= 5)&&(status[i]!=2)) {
				if( medaltype == 0 ) {//メダル絵はmedal.bmp
					// AC
					if(medal_ac[i]) {
						ExBltRect(0, 208 + 81 * i - 96 * maxPlay, 122, 0, 13 * (4-medal_ac[i]), 15, 12);
					} else if(medal_info) {
						ExBltRect(0, 208 + 81 * i - 96 * maxPlay, 122, 0, 52, 15, 12);
					}

					// SK
					if(medal_sk[i]) {
						ExBltRect(0, 208 + 81 * i - 96 * maxPlay, 135, 32, 13 * (4-medal_sk[i]), 15, 12);
					} else if(medal_info) {
						ExBltRect(0, 208 + 81 * i - 96 * maxPlay, 135, 32, 52, 15, 12);
					}

					// CO(==なのは状態で見てるから)
					if(medal_co[i]) {
						ExBltRect(0, 233 + 31 * i - 96 * maxPlay, 148, 80, 13 * (4-medal_co[i]), 15, 12);
					} else if(medal_info) {
						ExBltRect(0, 233 + 31 * i - 96 * maxPlay, 148, 80, 52, 15, 12);
					}

					// RE
					if(medal_re[i]) {
						ExBltRect(0, 208 + 81 * i - 96 * maxPlay, 148, 48, 13 * (4-medal_re[i]), 15, 12);
					} else if(medal_info) {
						ExBltRect(0, 208 + 81 * i - 96 * maxPlay, 148, 48, 52, 15, 12);
					}

					// ST(==なのは状態で見てるから)
					if(medal_st[i]) {
						ExBltRect(0, 233 + 31 * i - 96 * maxPlay, 122, 16, 13 * (4-medal_st[i]), 15, 12);
					} else if(medal_info) {	// 未点灯では表示しない #1.60c7q2ex
						ExBltRect(0, 233 + 31 * i - 96 * maxPlay, 122, 16, 52, 15, 12);
					}
				} else if( medaltype == 1 ) {//メダル絵はmedal_ti.bmp
					// AC
					if(medal_ac[i]) {
						ExBltRect(56, 208 + 81 * i - 96 * maxPlay, 122, 0, 12 * (4-medal_ac[i]), 16, 12);
					} else if(medal_info) {
						ExBltRect(56, 208 + 81 * i - 96 * maxPlay, 122, 0, 48, 16, 12);
					}

					// SK
					if(medal_sk[i]) {
						ExBltRect(56, 208 + 81 * i - 96 * maxPlay, 135, 32, 12 * (4-medal_sk[i]), 16, 12);
					} else if(medal_info) {
						ExBltRect(56, 208 + 81 * i - 96 * maxPlay, 135, 32, 48, 16, 12);
					}

					// CO(==なのは状態で見てるから)
					if(medal_co[i]) {
						ExBltRect(56, 233 + 31 * i - 96 * maxPlay, 148, 80, 12 * (4-medal_co[i]), 16, 12);
					} else if(medal_info) {
						ExBltRect(56, 233 + 31 * i - 96 * maxPlay, 148, 80, 48, 16, 12);
					}

				// RE
					if(medal_re[i]){
						ExBltRect(56, 208 + 81 * i - 96 * maxPlay, 148, 48, 12 * (4-medal_re[i]), 16, 12);
					} else if(medal_info) {
						ExBltRect(56, 208 + 81 * i - 96 * maxPlay, 148, 48, 48, 16, 12);
					}

					// ST(==なのは状態で見てるから)
					if(medal_st[i]) {
						ExBltRect(56, 233 + 31 * i - 96 * maxPlay, 122, 16, 12 * (4-medal_st[i]), 16, 12);
					} else if(medal_info) {	// 未点灯では表示しない #1.60c7q2ex
						ExBltRect(56, 233 + 31 * i - 96 * maxPlay, 122, 16, 48, 16, 12);
					}
				}
			/* メダル回数表示 */
				// AC
				if( medal_info ) {
					if(i) sprintf(string[0],"%2d",allclear[i]);
					else sprintf(string[0],"%d",allclear[i]);
					printTinyFont(225 + 51 * i - 96 * maxPlay, 127, string[0]);
				// SK
					if(i) sprintf(string[0],"%2d",skill[i]);
					else sprintf(string[0],"%d",skill[i]);
					printTinyFont(225 + 51 * i - 96 * maxPlay, 140, string[0]);
				// CO
					if(i) sprintf(string[0],"%2d",combo2[i]);
					else sprintf(string[0],"%d",combo2[i]);
					printTinyFont(249 + 3 * i - 96 * maxPlay, 154, string[0]);
				// RE
					if(i) sprintf(string[0],"%2d",rescue[i]);
					else sprintf(string[0],"%d",rescue[i]);
					printTinyFont(225 + 51 * i - 96 * maxPlay, 153, string[0]);
				// ST
					if(i) sprintf(string[0],"%2d",stp[i]);//プラチナ
					else sprintf(string[0],"%d",stp[i]);
					printTinyFont(233 + 18 * i - 96 * maxPlay, 134, string[0]);
					if(i) sprintf(string[0],"%2d",stg[i]);//金
					else sprintf(string[0],"%d",stg[i]);
					printTinyFont(239 + 18 * i - 96 * maxPlay, 134, string[0]);
					if(i) sprintf(string[0],"%2d",sts[i]);//銀
					else sprintf(string[0],"%d",sts[i]);
					printTinyFont(245 + 18 * i - 96 * maxPlay, 134, string[0]);
					if(i) sprintf(string[0],"%2d",stb[i]);//銅
					else sprintf(string[0],"%d",stb[i]);
					printTinyFont(251 + 18 * i - 96 * maxPlay, 134, string[0]);
				}
			}

			// WAIT値表示
			// SMALL対応 #1.60c7m9
			if((!hide_wait) || ((gameMode[i] == 9) && (std_opt[i] <= 1))) {
				sprintf(string[0], "%3d", wait1[i]);				// wait1
				printSMALLFont(202 + 68 * i - 96 * maxPlay, 208, string[0], color);

				sprintf(string[0], "%3d", wait2[i]);				// wait2
				printSMALLFont(218 + 68 * i - 96 * maxPlay, 208, string[0], color);

				sprintf(string[0], "%3d", wait3[i]);				// wait3
				printSMALLFont(202 + 68 * i - 96 * maxPlay, 217, string[0], color);

				sprintf(string[0], "%3d", waitt[i]);				// waitt
				printSMALLFont(218 + 68 * i - 96 * maxPlay, 217, string[0], color);

				// speed
				sprintf(string[0], "%4d", sp[i]);
				printSMALLFont(212 + 68 * i - 96 * maxPlay, 226, string[0], color);
			}
		}
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  メダル＆タイム表示
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void viewTime(void) {
	int32_t		i, color, j,tmp,tmp1;
	int32_t		color2,mp;
	mp = maxPlay;
	if((gameMode[0] == 8) || ((playback) && (gameMode[0] != 4))) mp = 0;

	for(i = 0; i < 1 + mp; i++) {
		color = (count % 4 / 2) * (sp[i] >= 1200) * digitc[rots[i]];
		/* セクションタイム表示 */
		if(!hide_st_record) {
			// split time and lap time
			// タイムが00:00:00の時は表示しない #1.60c7l1
			if(gameMode[i]!=10) {
				if(tc[i] >= st_record_interval_tgm * 10) {
					// SINGLE台なら左側にlap_timeを, 右側にsplit_time描画
					// 記録タイムをシングル台でも右側に出す設定追加 #1.60c7j8
					if((!maxPlay)&&(!st_record_force_viewright)) {
						for(j = 0; j < (tc[i] / (st_record_interval_tgm * 10)); j++) {
							if( (lap_time[j] != 0) && (split_time[j] != 0) ) {
								sprintf(string[0], "%4d", (j + 1) * st_record_interval_tgm * 10);
								printFont(0, j, string[0], fontc[rots[i]]);
								getTime(lap_time[j]);
								printFont(5, j, string[0], st_new[j + i * 13]);
								getTime(split_time[j]);
								printFont(32, j, string[0], color);
							}
						}
					// MASTERでL999の場合は表示が被ってしまうため非表示 #1.60c7k7
					} else if( (gameMode[i] >= 3) || (tc[i] != 999) ) {
						j = split_time[tc[i] / (st_record_interval_tgm * 10) - 1 + i * 130];
						if(j != 0) {
							getTime(j);
							printFont(26 + 4 * i - 12 * maxPlay, i * 2, string[0], color);
						}

						j = lap_time[tc[i] / (st_record_interval_tgm * 10) - 1 + i * 130];
						if(j != 0) {
							getTime(j);
							printFont(26 + 4 * i - 12 * maxPlay, 1 + i * 2, string[0], st_new[tc[i] / (st_record_interval_tgm * 10) - 1 + i * 13]);
						}
					}
				}
				// MASTERの900〜999 #1.60c7k7
				if((gameMode[i] >= 1) && (gameMode[i] <= 2) && (tc[i] == 999)) {
					// シングル台
					if((!maxPlay)&&(!st_record_force_viewright)) {
						if( (lap_time[9] != 0) && (split_time[9] != 0) ) {
							printFont(0, 9, " 999", fontc[rots[i]]);
							getTime(lap_time[9]);
							printFont(5, 9, string[0], st_new[9]);
							getTime(split_time[9]);
							printFont(32, 9, string[0], color);
						}
					// ダブル台
					} else {
						j = split_time[1000 / (st_record_interval_tgm * 10) - 1 + i * 130];
						if(j != 0){
							getTime(j);
							printFont(26 + 4 * i - 12 * maxPlay, i * 2, string[0], st_new[1000 / (st_record_interval_tgm * 10) - 1 + i * 13]);
						}

						j = lap_time[1000 / (st_record_interval_tgm * 10) - 1 + i * 130];
						if(j != 0){
							getTime(j);
							printFont(26 + 4 * i - 12 * maxPlay, 1 + i * 2, string[0], st_new[9]);
						}
					}
				}
			} else {
				if(lv[i] >= st_record_interval_heb) {
					// SINGLE台なら左側にlap_timeを, 右側にsplit_timeを描画
					// 記録タイムをシングル台でも右側に出す設定追加 #1.60c7j8
					if((!maxPlay)&&(!st_record_force_viewright)) {
						for(j = 0; j < (lv[i] / st_record_interval_heb); j++) {
							if( (lap_time[j] != 0) && (split_time[j] != 0) ) {
								sprintf(string[0], "%4d", (j + 1) * st_record_interval_heb);
								printFont(0, j, string[0], fontc[rots[i]]);
								getTime(lap_time[j]);
								printFont(5, j, string[0], color);
								getTime(split_time[j]);
								printFont(32, j, string[0], color);
							}
						}
					} else {
						j = split_time[lv[i] / st_record_interval_heb - 1 + i * 130];
						if(j != 0) {
							getTime(j);
							printFont(26 + 4 * i - 12 * maxPlay, i * 2, string[0], color);
						}

						j = lap_time[lv[i] / st_record_interval_heb - 1 + i * 130];
						if(j != 0) {
							getTime(j);
							printFont(26 + 4 * i - 12 * maxPlay, 1 + i * 2, string[0], color);
						}
					}
				}
			}
		}

		/* タイム表示 */
		if(time[i] > 359999) time[i] = 359999;

		// 持久モード
		if( gameMode[i] != 6 ) {
			if( (ending[i] == 2) && ((gameMode[i] == 5) || (debug)) ) {
				getTime(3740 - edrec[i]);	// エンディング残り時間
			} else if( ((gameMode[i] == 7)&(anothermode[i]!=3)) || (gameMode[i] == 8) ||((gameMode[i]==9)&&(std_opt[i]==1))||((gameMode[i]==5)&&(p_goaltype==4))) {
				getTime(ltime[i]);			// ACEモード制限時間
			} else {
				getTime(time[i]);			// 経過時間
			}

			if( (gameMode[i] == 4) && (i == 0) ) {
				// 対戦用
				if(vs_time == 0) {
					ExBltRect(81, 110 - 96, 215,0,228+18*english, 100, 18);
				} else {
					// 制限時間 #1.60c7r5
					color2 = (count % 4 / 2) * (ltime[i] <= 15 * 60) * 2;
					getTime(ltime[i]);
					printBIGFont(112, 216, string[0], color2);
				}
				ExBltRect(85, 150, 190, 35, 12*7, 19, 7);
				getTime(time[i]);
				printSMALLFont(136, 199, string[0], 0);
				if((vsmodesl>0)&&(!demo)){
					ExBltRect(85, 135, 100, 0, 7*16+vsmodesl*7, 100, 7);
				}
			} else if(gameMode[i] != 4) {
				// 通常
	//			color2 = color;
				if((gameMode[i] == 8) && (((ending[i] == 2) || (ending[i] == 3)) && (mission_end[c_mission] >= 4)) ){
					if(3740 - edrec[i] < 600)
						color2 = (count % 4 / 2) * 6;
					else
						color2 = color;
					getTime(3740 - edrec[i] + 1);
				}else if(((gameMode[i] == 7) || (gameMode[i] == 8)) && (ltime[i] <= 10 * 60) && (timeOn[i]))
					color2 = (count % 4 / 2) * (2 + (4 * ((gameMode[i] == 8) && (mission_type[c_mission] == 23))));
				else
					color2 = color;
				if((gameMode[i]!=8)||(mission_time[c_mission] > 0)){
					printBIGFont(112 + 192 * i - 96 * maxPlay, 216, string[0], color2);
				}else{
					ExBltRect(81, 110 + 192 * i - 96 * maxPlay, 215,0,228+18*english, 100, 18);
				}
			}
		}
		// TOMOYOモード
		else {
			// 通常ステージ
			if( stage[i] <= 19 ) {
				// ステージタイム
				getTime(stime[i]);
				printFont(16 + 24 * i - 12 * maxPlay, 27, string[0], 1);

				// リミットタイム
				color2 = (count % 4 / 2) * (ltime[i] <= 10 * 60) * 2;
				getTime(ltime[i]);
				printFont(16 + 24 * i - 12 * maxPlay, 28, string[0], color2);
			}

			// EXステージ #1.60c7m1fpbas_mode
			else if(!fpbas_mode[i]){
				if(ltime[i] <= 10 * 60){
					color2 = (count % 4 / 2) * 2;
				}else{
					color2 = color;
				}
				getTime(ltime[i]);
				printBIGFont(112 + 192 * i - 96 * maxPlay, 216, string[0], color2);
			}else {
				getTime(time[i]);			// 経過時間
				printBIGFont(112 + 192 * i - 96 * maxPlay, 216, string[0], color);
			}
		}

		if( tspin_c[i] > 0+(60*(isScore(i)*((tspin_type!=0) && (last_tspin[i]!=0)) )) ) {
			// T-SPIN #1.60c7n6
			if(last_tspin[i]==0)
				printFont(16 + 24 * i - 12 * maxPlay, 26, "T-SPIN", (count % 4 / 2) * digitc[rots[i]]);
			else if((last_tspin[i]==1) || (b_to_b_flag[i]==2))
				printFont(13 + 24 * i - 12 * maxPlay, 26, "T-SPIN SINGLE!", (count % 4 / 2) * digitc[rots[i]]);
			else if((last_tspin[i]==2) || (b_to_b_flag[i]==3))
				printFont(13 + 24 * i - 12 * maxPlay, 26, "T-SPIN DOUBLE!", (count % 4 / 2) * digitc[rots[i]]);
			else if((last_tspin[i]>=3) || (b_to_b_flag[i]>=4))
				printFont(13 + 24 * i - 12 * maxPlay, 26, "T-SPIN TRIPLE!", (count % 4 / 2) * digitc[rots[i]]);
		} else if(tspin_c[i]){
				printFont(19 + 24 * i - 12 * maxPlay, 26, "e 4", (count % 4 / 2) * digitc[rots[i]]);
		} else if((hidden[i] > 0) && (!hide_hidden)&&(hidden[i]<8)) {
			// HIDDEN LV表示 (隠す設定を追加#1.60c7i9)
			printFont(15 + 24 * i - 12 * maxPlay, 26, "HIDDEN LV", 2);
			sprintf(string[0], "%1d", hidden[i]);
			printFont(24 + 24 * i - 12 * maxPlay, 26, string[0], 0);
		}else if( (gflash[i] > 0) && (((gameMode[i] == 1) ||(gameMode[i] == 2))&&(enable_grade[i]==4))) {
		// COOL / REGRET
			if(gup_down[i]){
				printFont(17 + i * 24 - 12 * maxPlay, 26, "COOL!!", (count % 4 / 2) * digitc[rots[i]]);
			}else{
				printFont(17 + i * 24 - 12 * maxPlay, 26, "REGRET", (count % 4 / 2) * digitc[rots[i]]);
			}
		}

		if( b_to_b_c[i] > 0+(60*isScore(i)) )
			printFont(14 + 24 * i - 12 * maxPlay, 25, "BACK TO BACK", (count % 4 / 2) * digitc[rots[i]]);
		else if(b_to_b_c[i])
			printFont(17 + 24 * i - 12 * maxPlay, 25, "e 1.5", (count % 4 / 2) * digitc[rots[i]]);



		if(upLines[i]) {//lineup
			if((!hebo_plus[i]) && (gameMode[i] != 5) && (gameMode[i] >= 4) && ((timeOn[i]) || (gameMode[i] == 8))){
				sprintf(string[0], "%2dLINE UP", upLines[i] );	// せりあがるライン数
				printFont(15 + 24 * i - 12 * maxPlay, 28-(17*(gameMode[i] == 8))-(3*(gameMode[i] == 6)), string[0], fontc[rots[i]]);
				tmp1 = 0;
				if(gameMode[i] != 6)
				for(tmp = 1;tmp <= upLines[i];tmp++){
					if(upLines[i] - tmp >= 9){
						printFont(15 + tmp1 + 24 * i - 12 * maxPlay, 29-(17*(gameMode[i] == 8))-(3*(gameMode[i] == 6)), "g", digitc[rots[i]]);
						tmp = tmp + 9;
					}else{
						printFont(15 + tmp1 + 24 * i - 12 * maxPlay, 29-(17*(gameMode[i] == 8))-(3*(gameMode[i] == 6)), "f", fontc[rots[i]]);
					}
					tmp1++;
				}
			}
		}else if(gameMode[i] == 4){
			ExBltRect(55,128 + 192 * i -96 * maxPlay , 218 - (4 * (getDrawRate() == 1)), 64*rots[i] ,
				32*fontc[rots[i]] + (7 * (getDrawRate() == 1)),64,12 + (5 * (getDrawRate() == 1)));

			if((!disrise) && ((timeOn[i]) || (flag == -2) || (flag == -3))){
				if(vs_style[i] == 0){
					printSMALLFont(143 + 192 * i - 96 * maxPlay, 231, "NORMAL", 0);
				}else if(vs_style[i] == 1){
					printSMALLFont(143 + 192 * i - 96 * maxPlay, 231, "ATTACK", 2);
				}else if(vs_style[i] == 2){
					printSMALLFont(140 + 192 * i - 96 * maxPlay, 231, "DEFENCE", 1);
				}else if(vs_style[i] == 3){
					printSMALLFont(143 + 192 * i - 96 * maxPlay, 231, " ITEM ", 7);
				}
			}
		}
		if((ismiss[i]) && (gameMode[i] != 4) && (repversw >= 62)){
			if(gameMode[i] <= 3){
				ExBltRect(3,228 + 38 * i - 96 * maxPlay, 154, 274, 370, 27, 27);
				ExBltRect(3,210 + 78 * i - 96 * maxPlay, 60, 274, 370, 27, 27);
				ExBltRect(3,210 + 78 * i - 96 * maxPlay, 96 - 18 * (gameMode[i] == 3), 274, 370, 27, 27);
			}else if(gameMode[i] == 5){
				if(p_goaltype < 2) ExBltRect(3,228 + 48 * i - 96 * maxPlay, 154, 274, 370, 27, 27);
				ExBltRect(3,210 + 78 * i - 96 * maxPlay, 24, 274, 370, 27, 27);
				ExBltRect(3,210 + 78 * i - 96 * maxPlay, 44, 274, 370, 27, 27);
			}
		}
		if(istimestop[i]){
			printFont(18 + 24 * i - 12 * maxPlay, 10, "TIME", 0);
			printFont(18 + 24 * i - 12 * maxPlay, 11, "STOP", 0);
			sprintf(string[0], "%d", item_timer[i] / 60 );
			printBIGFont(152 + 192 * i - 96 * maxPlay, 96, string[0], 0);
		}
		if(debug){		// 数値チェック用スペース	バグチェックにどうぞ。
			sprintf(string[0], "STAT:%2d", status[i] );
			printFont(25 + 7 * i - 12 * maxPlay, 0, string[0], 1);
			sprintf(string[0], "0:%3d", statusc[i * 10] );
			printFont(26 + 7 * i - 12 * maxPlay, 1, string[0], 0);
			sprintf(string[0], "1:%3d", statusc[i * 10 + 1] );
			printFont(26 + 7 * i - 12 * maxPlay, 2, string[0], 0);
			sprintf(string[0], "2:%3d", statusc[i * 10 + 2] );
			printFont(26 + 7 * i - 12 * maxPlay, 3, string[0], 0);
			sprintf(string[0], "3:%3d", statusc[i * 10 + 3] );
			printFont(26 + 7 * i - 12 * maxPlay, 4, string[0], 0);
			sprintf(string[0], "4:%3d", statusc[i * 10 + 4] );
			printFont(26 + 7 * i - 12 * maxPlay, 5, string[0], 0);
			sprintf(string[0], "5:%3d", statusc[i * 10 + 5] );
			printFont(26 + 7 * i - 12 * maxPlay, 6, string[0], 0);
			sprintf(string[0], "6:%3d", statusc[i * 10 + 6] );
			printFont(26 + 7 * i - 12 * maxPlay, 7, string[0], 0);
			sprintf(string[0], "7:%3d", statusc[i * 10 + 7] );
			printFont(26 + 7 * i - 12 * maxPlay, 8, string[0], 0);
			sprintf(string[0], "8:%3d", statusc[i * 10 + 8] );
			printFont(26 + 7 * i - 12 * maxPlay, 9, string[0], 0);
			sprintf(string[0], "9:%3d", statusc[i * 10 + 9] );
			printFont(26 + 7 * i - 12 * maxPlay, 10, string[0], 0);
		}
	}
	if((playback) && (!demo) && (gameMode[0] != 6) && (gameMode[0] != 8) && (gameMode[0] != 4)){
		if((onRecord[0]) || (ending[0]) || (status[0] == 7) || (status[0] == 14)){
			if(dispLinfo){
				printFont(27 + (1 * (!maxPlay)), 26, "k HOLD:HIDE", 0);
				printFont(27 + (3 * (!maxPlay)), 5, "iLINE INFO ", 3);
				viewLineInfo();
			}else{
				printFont(27 + (1 * (!maxPlay)), 26, "k HOLD:SHOW", 0);
				printFont(27 + (1 * (!maxPlay)), 27, "   LINE INFO", 0);
			}
		}else{
			printFont(27 + (3 * (!maxPlay)), 14, "LINE INFO:", 0);
			printFont(27 + (3 * (!maxPlay)), 16, "PUSH HOLD", 0);
			if(dispLinfo)
				printFont(27 + (3 * (!maxPlay)), 15, "   ON", 0);
			else
				printFont(27 + (3 * (!maxPlay)), 15, "   OFF", 0);
		}
	}
}
//PRACTICE終了時のライン消去情報
void viewLineInfo(void){

	sprintf(string[0], "SINGLE %3d", p_erase_c[0]);
	printFont(27 + (3 * (!maxPlay)), 6, string[0], 1);

	sprintf(string[0], "DOUBLE %3d", p_erase_c[1]);
	printFont(27 + (3 * (!maxPlay)), 7, string[0], 4);

	sprintf(string[0], "TRIPLE %3d", p_erase_c[2]);
	printFont(27 + (3 * (!maxPlay)), 8, string[0], 5);

	sprintf(string[0], "HEBORIS%3d", p_erase_c[3]);
	printFont(27 + (3 * (!maxPlay)), 9, string[0], 2);

	if(tspin_type > 1){
		printFont(27 + (3 * (!maxPlay)), 10, "T-SPIN", 6);
		sprintf(string[0], " SINGLE%3d", p_erase_c[5]);
		printFont(27 + (3 * (!maxPlay)), 11, string[0], 6);

		printFont(27 + (3 * (!maxPlay)), 12, "T-SPIN", 9);
		sprintf(string[0], " DOUBLE%3d", p_erase_c[6]);
		printFont(27 + (3 * (!maxPlay)), 13, string[0], 9);

		printFont(27 + (3 * (!maxPlay)), 14, "T-SPIN", 7);
		sprintf(string[0], " TRIPLE%3d", p_erase_c[7]);
		printFont(27 + (3 * (!maxPlay)), 15, string[0], 7);
	}
	if(b2bcheck){
		printFont(26 + (2 * (!maxPlay)), 17, "BACK TO BACK", 3);
		sprintf(string[0], "HEBORIS%3d", p_erase_c[4]);
		printFont(27 + (3 * (!maxPlay)), 18, string[0], 2);
		if(tspin_type > 1){
			printFont(27 + (3 * (!maxPlay)), 19, "T-SPIN", 6);
			sprintf(string[0], " SINGLE%3d", p_erase_c[8]);
			printFont(27 + (3 * (!maxPlay)), 20, string[0], 6);

			printFont(27 + (3 * (!maxPlay)), 21, "T-SPIN", 9);
			sprintf(string[0], " DOUBLE%3d", p_erase_c[9]);
			printFont(27 + (3 * (!maxPlay)), 22, string[0], 9);

			printFont(27 + (3 * (!maxPlay)), 23, "T-SPIN", 7);
			sprintf(string[0], " TRIPLE%3d", p_erase_c[10]);
			printFont(27 + (3 * (!maxPlay)), 24, string[0], 7);
		}
	}
}
// B to B で「x1.5」を出すかどうかの判定
int32_t isScore(int32_t player) {
	if((gameMode[player] < 4) && (ending[player] < 2)) return 1;
	else if(gameMode[player] == 5) return 1;
	else if((gameMode[player] == 7) && (anothermode[player] == 3)) return 1;
	else if(gameMode[player] == 10) return 1;
	else return 0;
}
void viewScore(void) {
	// 描画処理は別の関数に分けました #1.60c7m9
	viewScoreSmall();
	viewTime();
	if(gameMode[0] == 8) viewMission();	// mission.c
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  フィールドの描画
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
// Big対応 #1.60c7
void viewField(void) {
	int32_t		b, i, j, k, fi, color, add, tmp[4];
	int32_t		gm;	// gamemode #1.60c7f4
	int32_t		fldt3, fldt4, fldt5;	//シャドウタイマーでうっすらと消えていく演出に使用 #1.60c7j9
	int32_t		mp;
	int32_t		tmp_x1, tmp_x2, tmp_y;	// 枠表示用

	mp = maxPlay;
	if((gameMode[0] == 8) || ((playback) && (gameMode[0] != 4))) mp = 0;

	// フィールド枠
	viewFldFrame(0,0);
	if(maxPlay) viewFldFrame(0,1);

	for(i = 0; i <= mp; i++) {
		if(skip_viewbg) ExBltRect(3, 112 + 192 * i - 96 * maxPlay, 0, 0, 128, 100, 32);

		// イレイサーの線 #1.60c7s2
		if( (gameMode[0] == 8) && (i == 0)  && ((mission_type[c_mission] == 7)||(mission_type[c_mission] == 39))){
			viewEraserLines();
		}

		// フィールドを描画
		// オマケ
		if((gameMode[i] == 4) && (upLineT[i] == 3) && (timeOn[i])){
				for(j = 0; j < 5; j++)
					for(k = 0; k < 10; k++) {
						fi = fldu[k + j * 10 + i * 220] - 1;
						if(fi != -1) {
							if((disable_giji3D < 3) && (getFieldBlock3(i, k + (1-2*i), j) == 0)){
								if(fi < 8) tmp[0]=getBigBlock_add(i);
								else tmp[0] = 8;
								ExBltFastRect(83, (k + 15 + 24 * i - 12 * maxPlay) * 8 + 8 - (10 * i), (j+5+(21*(gameMode[i] == 4))) * 8,
										(fi * 2) - ((fi >= 10) * 20) + (20 * tmp[0]), 4*8, 2, 8 );
							}
							ExBltFastRect(getBlockPlaneNo(i, fi), (k + 15 + 24 * i - 12 * maxPlay) * 8, (j+5+(21*(gameMode[i] == 4))) * 8, (fi * 8) - ((fi >= 10) * 80), 4 * 8, 8, 8);
						}
					}
		}
		// TOMOYO せり上がりフィールドエディタ使用中
		if((status[i] == 18) && (statusc[i * 10 + 1] == 2)){
				for(j = 0; j < 20; j++)
					for(k = 0; k < 10; k++) {
						fi = tomoyo_rise_fld[k + j * 10 + i * 200] - 1;
						if(fi != -1) {
							if((disable_giji3D < 3) && (getFieldBlock2(i, k + (1-2*i), j) == 0)){
								if(fi < 8) tmp[0]=getBigBlock_add(i);
								else tmp[0] = 8;
								ExBltFastRect(83, (k + 15 + 24 * i - 12 * maxPlay) * 8 + 8 - (10 * i), (j+1+4) * 8,
									(fi * 2) - ((fi >= 10) * 20) + (20 * tmp[0]), 4 * 8, 2, 8 );
								}
							ExBltFastRect(getBlockPlaneNo(i, fi), (k + 15 + 24 * i - 12 * maxPlay) * 8, (j+1+ 4) * 8, (fi * 8) - ((fi >= 10) * 80), 4 * 8, 8, 8);
						}
					}
		//通常
		} else if((!pause[i]) || (debug) || ((gameMode[i] == 5) && (pause[i]))) {
			for(b = 0; b < 2; b++)
				for(j = 0; j < hiddeny[i]; j++)
					for(k = 0; k < 10; k++) {
						fi = fld[k + j * 10 + i * 220] - 1;
						fldt3 = fldt[k + j * 10 + i * 220];
						fldt4 = fldt[k + j * 10 + i * 220] * 32;
						fldt5 = 256 - (fldt[k + j * 10 + i * 220] * 32);

						// アイテムの場合は見た目を変える #1.60c7n1
						if( fldi[k + j * 10 + i * 220] ) {
							fi = 17 + fldi[k + j * 10 + i * 220] - 1;
						}
						add = 0;
						if(fi < -1){
							add = 9;
							fi = fi + 9;
						}
						if((item_monochrome[i]) && (fi != -1)){	//アイテムによる[ ]
							fi = 8;
							add = 0;
						}
						if(fi != -1) {
							if(!b) {
								if ( iscolor[i] ) {
									// COLORモード計算 #1.60c7p9ex
									color = k;
									if ( color >= 5 ) color = 9 - color;
									color = color_tbl_max - ( (((20 - j) + color) * 4 + color_counter[i]) % color_tbl_max );
									color = (erase[j + i * 22] == 0) * (7 + 10 - color_tbl[color]);
								} else if(thunder_timer){
									color = Rand(8)+6;
								} else if((fldi[k + j * 10 + i * 220] == fldisno) || (fldi[k + j * 10 + i * 220] == fldigsno)) {
								// 正方形
									if(flds[k + j * 10 + i * 220] >= -10) {
										color = 17 + flds[k + j * 10 + i * 220];
										flds[k + j * 10 + i * 220]--;
									} else if(flds[k + j * 10 + i * 220] >= -19) {
										color = abs_YGS2K(flds[k + j * 10 + i * 220]) - 10 + 7;
										flds[k + j * 10 + i * 220]--;
									} else {
										color = (erase[j + i * 22] == 0) * 4;
									}
								} else {
									if((fi >= 10) && (fi < 17 + fldihardno - 1))
										color = 7 + (time2[i] % 55 >= 45);
									else
										color = ((erase[j + i * 22] == 0) && (heboGB[i] == 0)) * 4;
								}
							//	if(hidden[i] == 8) color = 20;
								if ( ( color <= 16 ) && ( (!isxray[i]) || (xray_counter[i] % p_xray_frame == k) ) && (!isblind[i])){
									// フィールドタイマーが1〜8ならそのブロックは半透明になる(8段階)
									// 0ならそのブロックは完全透明となる
									// それ以外の値なら無効(通常の描画)
									if((!isdark[i]) || ((isdark[i]) && (item_timer[i] > 420) && (item_timer[i] % 2 == 0))){
										if((fldt3 >= 1) && (fldt3 <= 8)) {
											// タイマーが8以下1以上なら半透明描画で消えていく #1.60c7k2
											if((disable_giji3D < 4) && (!iscolor[i]) && (getFieldBlock(i, k + (1-2*i), j, 0) == 0) && (add == 0) && (fi != 8) && (fi != 9) && (heboGB[i]==0)){
												if(fi < 8) tmp[0]=getBigBlock_add(i);
												else tmp[0] = 8;
												BlendExBltRect(83, (k + 15 + 24 * i - 12 * maxPlay) * 8 + ofs_x2[i] + 8 - (10 * i), (j+ 3) * 8 + ofs_y2[i],
													(fi * 2) - ((fi >= 10) * 20) + (20 * tmp[0]), color * 8, 2, 8 ,fldt4,fldt4,fldt4,fldt5,fldt5,fldt5);
											}
											BlendExBltRect(getBlockPlaneNo(i, fi),(k + 15 + 24 * i - 12 * maxPlay) * 8 + ofs_x2[i], (j+ 3) * 8 + ofs_y2[i],
												((fi+add) * 8) - ((fi >= 10) * 80), color * 8, 8, 8 ,fldt4,fldt4,fldt4,fldt5,fldt5,fldt5);
										} else if((fldt3 != 0) && (fldt3 > -10)) {
											// タイマーが0なら無視する #1.60c7k4
											if((disable_giji3D < 4) && (!iscolor[i]) && (getFieldBlock(i, k + (1-2*i), j, 0) == 0) && (add == 0) && (fi != 8) && (fi != 9) && (heboGB[i]==0)){
												if(fi < 8) tmp[0]=getBigBlock_add(i);
												else tmp[0] = 8;
												ExBltFastRect(83, (k + 15 + 24 * i - 12 * maxPlay) * 8 + ofs_x2[i] + 8 - (10 * i), (j+ 3) * 8 + ofs_y2[i],
													(fi * 2) - ((fi >= 10) * 20) + (20 * tmp[0]), color * 8, 2, 8 );
											}
											ExBltFastRect(getBlockPlaneNo(i, fi), (k + 15 + 24 * i - 12 * maxPlay) * 8 + ofs_x2[i],
												(j+ 3) * 8 + ofs_y2[i], ((fi+add) * 8) - ((fi >= 10) * 80), color * 8, 8, 8);
										}
									}
								}
							} else {
								// X-RAY中は一部分だけ枠を描く #1.60c7p9ex改造
								if( (isblind[i]) || (blockflash != 2) && ( (!isxray[i]) || ( (xray_counter[i] % p_xray_frame == k) && (!xray_drawblockframe) ) ) ) {
									// PRACTICEの2P側で[]の枠線が出ないように修正#1.60c7k2
									if( ((fi != 8)&&(fi != 9) && (add == 0)) || (isblind[i])) {
										// シャドウタイマーが残っていたら枠を描画 #1.60c7k1
										// タイマーが-1（せり上がりまたは初期配置）な場合も表示してみる #1.60c7k3
										// 条件を単純化(しすぎ？) #1.60c7k6
										// NOTE: if(fldt3 == -1、1〜8)の条件を削除するとDTET風に。
										if( (fldt3 == -1) || ((fldt3 != 0) && (fldt3 >= 8) && (m_roll_blockframe)) || ((isblind[i]) && (fldt3 != 0)) || (fldt3 < -1) || ((gameMode[i] == 5) && (pause[i]))) {
											// 死んだとき枠を消す #1.60c7k9
											// COLOR中は枠を描かない#1.60c7p9ex
											if(( !color_flag[i] && ((!dead_blockframe) || (gameMode[i] == 5) || (status[i] != 7)) )&& (heboGB[i]==0) || (isblind[i])) {
												// フィールド端では枠の一部分を表示しない #1.60c7s5
												if((!isdark[i]) || ((gameMode[i] == 5) && (pause[i]))){
													// 新・枠線処理
													if(fldt3 <= -10) add = 60 + fldt3 + 20;	//TI風消えロール時
													else if(isblind[i]) add = 10;
													else add = 0;
													tmp[0] = getFieldBlock(i, k - 1, j    , 0);	//左
													tmp[1] = getFieldBlock(i, k + 1, j    , 0);	//右
													tmp[2] = getFieldBlock(i, k    , j - 1, 0);	//上
													tmp[3] = getFieldBlock(i, k    , j + 1, 0);	//下
													// 左
													if(tmp[0] == 0)
														ExBltRect(29, (k + 15 + 24 * i - 12 * maxPlay) * 8 - 1 + ofs_x2[i], (j + 3) * 8 - 1 + ofs_y2[i], 63, 0 + add, 10, 10);
													// 右
													if(tmp[1] == 0)
														ExBltRect(29, (k + 15 + 24 * i - 12 * maxPlay) * 8 - 1 + ofs_x2[i], (j + 3) * 8 - 1 + ofs_y2[i], 73, 0 + add, 10, 10);
													// 上
													if(tmp[2] == 0)
														ExBltRect(29, (k + 15 + 24 * i - 12 * maxPlay) * 8 - 1 + ofs_x2[i], (j + 3) * 8 - 1 + ofs_y2[i], 83, 0 + add, 10, 10);
													// 下
													if(tmp[3] == 0)
														ExBltRect(29, (k + 15 + 24 * i - 12 * maxPlay) * 8 - 1 + ofs_x2[i], (j + 3) * 8 - 1 + ofs_y2[i], 93, 0 + add, 10, 10);
													add = 0;
												}
											}
										}
									}
								}
							}
						}/* if(fi != -1) */
						if((navigation) && (gameMode[i] == 0) && (tc[i] < navigation_limitLv) && (b) && (navigation_time[i] <= navigation_limittime) && (status[i] == 5)){
							if((fld[k + j * 10 + i * 220] == 0) && (cp_fld[k + j * 10 + i * 220] == c_cblk_r[i] + 1)){
								ExBltRect(29, (k + 15 + 24 * i - 12 * maxPlay) * 8, (j+ 3) * 8, ((c_cblk[i]-1) * 8), (((navigation_time[i] % 20)/2) * 8)+38, 8, 8);
							}
						}
					}

			if(hiddenc[i] < 15) {
				hiddenc[i]++;
				color = hiddenc[i] / 3;
			} else{
				color = 5;
			}

			// HIDDEN LV8なら、TAPのシャドーロール風に #1.60c7d
			if(hidden[i] != 8) {
				for(j = hiddeny[i]; j < 22; j++) {
					for(k = 0; k < 10; k++) {
						ExBltFastRect(getBlockPlaneNo(i, 0), (k + 15 + 24 * i - 12 * maxPlay) * 8, (j+ 3) * 8, 0, color * 8, 8, 8);
					}
					color = 5;
				}
			}
		}
		// フィールド枠
		viewFldFrame(1,i);
	}
}
//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  NEXTブロックとHOLDブロックの表示
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void viewPreview(void){
	int32_t n2[2], n3[2], n4[2], n5[2], n6[2]; // 3Next表示用 #1.60c
	int32_t nf1[2];//偽者用
	int32_t i, j, c_tmp[6], mp;

	mp = maxPlay;
	if((gameMode[0] == 8) || ((playback) && (gameMode[0] != 4))) mp = 0;

	for(i = 0; i <= mp; i++) {
		// hold・3Nextに合わせて画面上部変更 #1.60c
		if( (hnext[i] >= 1) && (status[i] != 0) && ((status[i] != 2) || ((status[i] == 2) && (statusc[i * 10 + 2] != 0))) ) {
			// NEXTをplayerごとに #1.60c7m1
			if(!isfakenext[i]){
				if(i == 0) {//普通
					n2[i] = nextb[(nextc[i] + 1-(!ndelay[i])) % 1400];
					n3[i] = nextb[(nextc[i] + 2-(!ndelay[i])) % 1400];
					n4[i] = nextb[(nextc[i] + 3-(!ndelay[i])) % 1400];
					n5[i] = nextb[(nextc[i] + 4-(!ndelay[i])) % 1400];
					n6[i] = nextb[(nextc[i] + 5-(!ndelay[i])) % 1400];
				} else {
					n2[i] = nextb[( (nextc[i] + i * 1400) + 1 -(!ndelay[i])) % 2800];
					n3[i] = nextb[( (nextc[i] + i * 1400) + 2 -(!ndelay[i])) % 2800];
					n4[i] = nextb[( (nextc[i] + i * 1400) + 3 -(!ndelay[i])) % 2800];
					n5[i] = nextb[( (nextc[i] + i * 1400) + 4 -(!ndelay[i])) % 2800];
					n6[i] = nextb[( (nextc[i] + i * 1400) + 5 -(!ndelay[i])) % 2800];
				}
			}else{//偽者　nextcは保持されている
					nf1[i]= nextb[(nextc[i] + 50) % 1400];
					n2[i] = nextb[(nextc[i] + 100 + 1-(!ndelay[i])) % 1400];
					n3[i] = nextb[(nextc[i] + 150+ 2-(!ndelay[i])) % 1400];
					n4[i] = nextb[(nextc[i] + 200+ 3-(!ndelay[i])) % 1400];
					n5[i] = nextb[(nextc[i] + 250+ 4-(!ndelay[i])) % 1400];
					n6[i] = nextb[(nextc[i] + 300+ 5-(!ndelay[i])) % 1400];
			}

			// 表示する色を決める
			for(j=0; j<6; j++) {
				if(item_nblk[j + i * 6]){
					c_tmp[j] = 16 + item_nblk[j + i * 6];
				}else{
					c_tmp[j] = c_nblk[j + i * 6];
				}
			}

			//FEVER中
			if((isfever[i]) && (item_timer[i] > 0) && ((item_timer[i] >= 90) || (item_timer[i] % 2 == 1))){
				n2[i] = 0;
				n3[i] = 0;
				n4[i] = 0;
				n5[i] = 0;
				n6[i] = 0;
				for(j=0; j<6; j++) {
					if(!item_nblk[j + i * 6]){
						if(isWRule(i)){
							c_tmp[j] = wcol[0];
						} else if((rots[i] >= 4) && (rots[i] != 8)){
							c_tmp[j] = acol[0];
						} else{
							c_tmp[j] = ccol[0];
						}
					}
				}
			}
			//NEXT一個目
			// 棒の場合は1ブロック分下に表示 #1.60c7p9ex
			// []の場合は棒を押し下げない #1.60c7p9ex改造
			if(ishidenext[i] != 1){
				if((isfakenext[i]) && (ndelay[i])){//偽者
					drawBlockFast(18+ 24 * i - 12 * maxPlay, 1 + isWRule(i) * (next[i] != 0) , nf1[i], rt_nblk[0 + 6 * i], c_tmp[0], 0, - 2, 1, i, (disable_giji3D < 3));
				}else if(ndelay[i]){
					drawBlockFast(18+ 24 * i - 12 * maxPlay, 1 + isWRule(i) * (next[i] != 0) + ((next[i] == 0) && ((c_nblk[0 + i * 6] != 8) || (c_nblk[0 + i * 6] <= 0)) && (rots[i] != 8)), next[i], rt_nblk[0 + 6 * i], c_tmp[0], 0, - 2, 1, i, (disable_giji3D < 3));
				}else if((!ndelay[i]) && ((hnext[i] == 1) || (hebo_plus[i]) || (death_plus[i])) && (!heboGB[i])){
					drawhBlock(160 + 192 * i - 96 * maxPlay, 18 + isWRule(i) * (next[i] != 0) + ((next[i] == 0) && ((c_nblk[0 + i * 6] != 8) || (c_nblk[0 + i * 6] <= 0))), next[i], c_tmp[0], 4, i,rt_nblk[0 + 6 * i],(disable_giji3D < 3));
				}
			}
			//next2個目〜6個目
			if((ishidenext[i] == 0) && (!death_plus[i])&&(!hebo_plus[i])&&(!heboGB[i])){
				// 表示位置を微調整 #1.60c7p9ex
				if(hnext[i] >= 2)
					drawhBlock(180 + 192 * i - 96 * maxPlay, 18 + isWRule(i) * (n2[i] != 0) * 4-(6*(top_frame)), n2[i], c_tmp[1-(!ndelay[i])], 4, i,rt_nblk[(1-(!ndelay[i])) + 6 * i],(disable_giji3D < 3));
				if(hnext[i] >= 3)
					drawhBlock(200 + 192 * i - 96 * maxPlay, 18 + isWRule(i) * (n3[i] != 0) * 4-(6*(top_frame)), n3[i], c_tmp[2-(!ndelay[i])], 4, i,rt_nblk[(2-(!ndelay[i])) + 6 * i],(disable_giji3D < 3));
				if(hnext[i] >= 4)
					drawhBlock(210 + 192 * i - 96 * maxPlay, 40 + isWRule(i) * (n4[i] != 0) * 4-8, n4[i], c_tmp[3-(!ndelay[i])], 4, i,rt_nblk[(3-(!ndelay[i])) + 6 * i],(disable_giji3D < 3));
				if(hnext[i] >= 5)
					drawhBlock(210 + 192 * i - 96 * maxPlay, 58 + isWRule(i) * (n5[i] != 0) * 4-8, n5[i], c_tmp[4-(!ndelay[i])], 4, i,rt_nblk[(4-(!ndelay[i])) + 6 * i],(disable_giji3D < 3));
				if(hnext[i] >= 6)
					drawhBlock(210 + 192 * i - 96 * maxPlay, 76 + isWRule(i) * (n6[i] != 0) * 4-8, n6[i], c_tmp[5-(!ndelay[i])], 4, i,rt_nblk[(5-(!ndelay[i])) + 6 * i],(disable_giji3D < 3));
			}
		}

		if(!top_frame)
			ExBltRect(3, 112 + 192 * i - 96 * maxPlay, 0, 0, 128, 100, 33);
		else
			ExBltRect(3, 97 + 192 * i - 96 * maxPlay, 0, 0, 423, 126, 35);
		if(hnext[i] >= 1)
			ExBltRect(3, 150 + 192 * i - 96 * maxPlay, 4*(!top_frame), 128, 112, 26, 7);
		if(!disable_hold)
			ExBltRect(3, 112 + 192 * i - 96 * maxPlay, 4, 128, 119, 26, 7);//HOLDという文字

		// HOLDブロック
		if((!disable_hold) && !((ishidenext[i] == 1) && (hnext_flag[i]))) {
			if( (i == 0) || ((!playback)&&(gameMode[i] != 5)) || (gameMode[i] == 4)) {
				if(hold[i] != -1) {
					// 表示する色を決定する #1.60c7p9ex改造
					if((dhold[i]) || ((ishidenext[i]) && (dhold[i])) || (isholdlock[i] == 1)||(heboGB[i])) {
						// HOLDが使用できないとき
						if(c_hblk[i] == 8)
							j = 9;
						else if(c_hblk[i] >= 0)
							j = 0;
						else
							j = c_hblk[i];
					} else {
						// HOLDが使用できるとき
						j = c_hblk[i];
					}
					if(!isfakenext[i]){
						// 表示位置を微調整 #1.60c7p9ex
						if((ishidenext[i] != 1) || ((ishidenext[i]) && (j != 0) && (j != 9))){
							drawhBlock(116 + 192 * i - 96 * maxPlay, 12 + isWRule(i) * (hold[i]!=0) * 4, hold[i], j, 4, i,0,(disable_giji3D < 3));
						}else{	//HIDE NEXT時の「？」
							printFont(15 + 24 * i - 12 * maxPlay,2,"?",0);
						}
					}else{//偽者
						// 表示位置を微調整 #1.60c7p9ex
						if((ishidenext[i] != 1) || ((ishidenext[i]) && (j != 0) && (j != 9))){
							drawhBlock(116 + 192 * i - 96 * maxPlay, 12 + isWRule(i) * (hold[i]!=0) * 4, nextb[(nextc[i] + 50) % 1400], j, 4, i,0,(disable_giji3D < 3));
						}else{	//HIDE NEXT時の「!」
							printFont(15 + 24 * i - 12 * maxPlay,2,"!",0);
						}
					}
				}
				if((disable_hold) || (isholdlock[i]) || (death_plus[i])||(hebo_plus[i])||(heboGB[i]))	//HOLD LOCKの錠鍵
					ExBltRect(3, 110 + 192 * i - 96 * maxPlay, 5, 275, 430, 27, 27);
			}
		}
	}
}
/* フィールド背景を表示 #1.60c7n5 */
void viewFldBackground(void) {
	int32_t i, k;

	for(i = 0; i <= maxPlay; i++) {
		// ------------------------
		// ここから C++版上部枠表示

		int32_t		tr;

		if(background == 2) {
			BlendExBltRect(24, 120 + 192 * i - 96 * maxPlay, 40, 0, 0, 80, 160, 256 - fldtr, 256 - fldtr, 256 - fldtr, fldtr, fldtr, fldtr);
		}

		// GBモードでなければ上を描画
		// グラデーション部分をなめらかにしない
		if (!((heboGB[i]==1) || (((tc[i] >= over1000_start) || (over1000_block[i]) || ((p_over1000)&&(gameMode[i] == 5))) && (rots[i] == 6))))
		{
			// グラデーション部分をなめらかに#1.60c7p9ex
			if(!top_frame){
				BlendExBltRect(3, 112 + 192 * i - 96 * maxPlay, 0, 101, 128, 100 - 20, 32, 256 - fldtr, 256 - fldtr, 256 - fldtr, fldtr, fldtr, fldtr);
				for(k = 0; k < 20; k++) {
					tr = (256 - fldtr) * (19 - k) / 20;
					tr = 256 - tr;
					BlendExBltRect(3, 112 + 192 * i - 96 * maxPlay + (80 + k), 0, 101 + (80 + k), 128, 1, 32, 256 - tr, 256 - tr, 256 - tr, tr, tr, tr);
				}
			}
			else{
				BlendExBltRect(3, 97 + 192 * i - 96 * maxPlay, 0, 127, 423, 126, 35, 256 - fldtr, 256 - fldtr, 256 - fldtr, fldtr, fldtr, fldtr);
			}
		}

		if(!top_frame)
			ExBltRect(3, 112 + 192 * i - 96 * maxPlay, 0, 0, 128, 100, 32);
		else
			ExBltRect(3, 97 + 192 * i - 96 * maxPlay, 0, 0, 423, 126, 35);

		// ここまで C++版上部枠表示
		// ------------------------

		if((heboGB[i]==1) || (((tc[i] >= over1000_start) || (over1000_block[i]) || ((p_over1000)&&(gameMode[i] == 5))) && (rots[i] == 6))){
		//フィールド背景を白く
			ExBltFastRect(2, 112 + ofs_x[i] + 192 * i - 96 * maxPlay, 32, 96 * 7, 0, 96, 176);
			ExBltRect(3, 110 + 192 * i - 96 * maxPlay, 8, 184, 275 + (24 * top_frame), 107, 24);
		}
		// XRAY・COLORのときに黒背景を描く
		else if ( ( xray_flag[i] && !xray_bgblack ) || ( color_flag[i] && !color_bgblack ) || (heboGB[i] == 2))
		{
			ExBltFastRect(24, 120 + ofs_x[i] + 192 * i - 96 * maxPlay, 40 + ofs_y[i], 0, 0, 80, 160);
		}
		else if(background == 0)
		{
			if(getDrawRate() == 1)
			{
				BlendBltRect(4+pinch[i] * 2, 120 + ofs_x[i] + 192 * i - 96 * maxPlay, 40 + ofs_y[i], count%128 / 4, count%256 /8, 80, 160, 256 - fldtr, 256 - fldtr, 256 - fldtr, fldtr, fldtr, fldtr);
			}
			else
			{
				BlendBltRect(4+pinch[i] * 2, 240 + (ofs_x[i]*2) + 384 * i - 192 * maxPlay, 80 + (ofs_y[i]*2), count%64 / 2, count%64 / 2, 160, 320, 256 - fldtr, 256 - fldtr, 256 - fldtr, fldtr, fldtr, fldtr);
			}
		}
		else if(background == 1)
		{
			ExBlendBltRect(4+pinch[i] * 2, 120 + ofs_x[i] + 192 * i - 96 * maxPlay, 40 + ofs_y[i], 0, 0, 80, 160, 256 - fldtr, 256 - fldtr, 256 - fldtr, fldtr, fldtr, fldtr);
		}
	}
}

/* フィールド枠を表示(uponlyを1にすると上部分だけ表示) */
void viewFldFrame(int32_t uponly,int32_t i) {
	int32_t j, gm[2],waiting[2],ismission[2];

		// GAMEMODEに応じて枠の色を変更 #1.60c7f4
		gm[i] = gameMode[i];
		waiting[i] = 0;
		if(gameMode[i] == 10)//ORI
			gm[i] = 9;
		else if(gameMode[i] == 8)
			gm[i] = 5;
		else if(examination[i]==1)
			gm[i] = 3 - (2 * ((purupuru[i] == 1)||(examination[i]==2)));
		else if(hebo_plus[i])
			gm [i]= 6;
		else if((gm[i] > 1) && (gm[i] < 4) && (!item_mode[i])) {
			if(((tc[i] >= over1000_start) || (over1000_block[i]) || ((p_over1000)&&(gameMode[i] == 5))) && (rots[i] == 6))
				gm[i] = 7;//オールドスタイルの石のやつ
			else
				gm[i]--;
		} else if((gm[i] == 4) || (item_mode[i]) || ((gameMode[i] == 0) && (novice_mode[i]))) {
			// VERSUSモード
			// BEGINNERでも枠が青なのを修正#1.60c7g4
			// アイテムを食らっている時は色を変える #1.60c7n4
			if((status[i] == 31) && (statusc[i * 10] < 120)){
				gm[i] = 1 + (5 * (count%4 <= 1));
			} else if((status[i] == 25) || (status[i] == 33) || (isallclear[i]) || ((isreflect[i]) && (gameMode[i] != 4) && (!isfever[i]))){
				gm[i] = 6;		//サポートアイテム（明るい青）
			} else if( (doexchg[i]) || ((status[i] == 29) && (!statusc[(1-i) * 10 + 3])) || (isFreefall[i]) || ((status[i] == 26) && (statusc[i * 10 + 2])) ){
				gm[i] = 0;		//その他（緑）
			} else if( ((IsBig[i])&&(!IsBigStart[i])) || (isfmirror[i]) || (isrollroll[i]) || (isxray[i]) || (iscolor[i]) ||
			(isrotatelock[i]) || (isholdlock[i]) || (istimestop[i]) || (ishidenext[i]) || (ismagnetblock[i]) ||
			 (isLRreverse[i]) || (isUDreverse[i]) || (isboost[i]) || (isremote[i]) || (isdark[i]) || (istrance[i]) ||
			 (dolaser[i]) || (status[i] == 27) || (donega[i])  || (status[i] == 26) || (status[i] == 28) || (doshotgun[i]) ||
			 (item_nblk[0 + i * 6] == fldihardno) || (item[i] == fldihardno) || (isbanana[i]) || (do180field[i])  || (status[i] == 34)||
			 (isdouble[i])||(docopyfld[i])||(isfakenext[i])||(item_monochrome[i])||(ismiss[i])||(status[i] == 29) && (statusc[(1-i) * 10 + 3]) ){
				gm[i] = 2;		//攻撃アイテム（赤）
			} else if(isfever[i]){
				gm[i] = 3;		//フィーバー（黄）
			}else if(gameMode[i] == 3){
				gm[i] = 2;
			} else {
				gm[i] = 1-(1*(gameMode[i] == 0));
			}
		}else if(gm[i] == 5) gm[i] = 3;
		else if(gm[i] == 6) gm[i] = 4;	// tomoyo #1.60c7l6
		else if( (gm[i] == 7)||(gm[i] == 8) ) gm[i] = 5 - (3 * (anothermode[i] == 2));
		else if(gm[i] == 9) gm[i] = 5 - (2 * (relaymode[i]));

		if((status[i] == 0) || ((status[i] == 2) && (statusc[i * 10 + 2] == 0)) || ((status[i] ==1) && (statusc[i * 10 + 1] == 2))){
			gm[i] = 5;
			waiting[i] = 1;
		}

		if(heboGB[i]==1)	//mission オールドスタイル
			gm[i] = 7;

		if(heboGB[i]==2)	//mission オールドスタイル(天井アリ)
			gm[i] = 8;

		ismission[i] = ((((gameMode[i] >= 8)&&(gameMode[i] < 10)) || ((gameMode[i] == 7) && (anothermode[i] < 3))) && (!uponly) && (!waiting[i]) && (heboGB[i]==0));

		if((ismission[i]) && (disable_giji3D < 4))
			ExBltRect(2, 104 + 192 * i - 96 * maxPlay + ofs_x[i] + 2 - (4 * i), 32 + ofs_y[i], 784, 184, 112, 184);

		if((isfmirror[i]) && (fmirror_cnt[i] > -20) && (fmirror_cnt[i] < 0)){
			if(uponly == 0)
				ExBltRect(66+(gm[i]-2), 112 + 192 * i - 96 * maxPlay, 17, 96 * ((fmirror_cnt[i]+20-1) % 5), 206 * ((fmirror_cnt[i]+20-1) / 5), 96, 206);
		}else{
			if((!uponly) && (disable_giji3D < 4) && !( color_flag[i] && !color_bgblack ) && gm[i] <= 6)
				ExBltRect(2, 112 + 192 * i - 96 * maxPlay + ofs_x[i] + 2 - (4 * i), 32 + ofs_y[i], 96 * gm[i], 184, 96, 184);
			ExBltRect(2, 112 + (4 * uponly) + 192 * i - 96 * maxPlay + ofs_x[i], 32 + ofs_y[i], 96 * gm[i]+ (4 * uponly), 0, 96 - (8 * uponly), 184 - uponly * 176);
		}

		if(ismission[i])//MISSIONのときの枠の出っ張り
			ExBltRect(2, 104 + 192 * i - 96 * maxPlay + ofs_x[i], 32 + ofs_y[i], 672, 184, 112, 184);
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  フォントを表示する
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void printFont(int32_t fontX, int32_t fontY, const char *fontStr, int32_t fontColor) {
	int32_t		i, sx, sy, stringLength, stringChar;

	stringLength = StrLen(fontStr);
	for(i = 0; i < stringLength; i++) {
		stringChar = CharAt(fontStr, i);
		sx = ((stringChar - 32) % 32) * 8;
		sy = ((stringChar - 32) / 32) * 8 + fontColor * 24;
		ExBltRect(1, (fontX + i) * 8, fontY * 8, sx, sy, 8, 8);
	}
}

// 1ドットずつY座標を指定できるバージョン #1.60c7k3
void printFontEx(int32_t fontX, int32_t fontY, const char *fontStr, int32_t fontColor) {
	int32_t		i, sx, sy, stringLength, stringChar;

	stringLength = StrLen(fontStr);
	for(i = 0; i < stringLength; i++) {
		stringChar = CharAt(fontStr, i);
		sx = ((stringChar - 32) % 32) * 8;
		sy = ((stringChar - 32) / 32) * 8 + fontColor * 24;
		ExBltRect(1, (fontX + i) * 8, fontY, sx, sy, 8, 8);
	}
}

// 小さいバージョン
//　代替文字
// ;→?
// <→-
// =→m
// >→%
// ?→.
void printSMALLFont(int32_t fontX, int32_t fontY, const char *fontStr, int32_t fontColor) {
	int32_t		i, sx, sy, stringLength, stringChar;

	stringLength = StrLen(fontStr);
	for(i = 0; i < stringLength; i++) {
		stringChar = CharAt(fontStr, i);

		// スペースでない場合
		if(stringChar != 32) {
			// 数字
			if(stringChar <= 64) {
				sx = (stringChar - 48) * 6 + 132;
				sy = fontColor * 9;
				ExBltRect(31, fontX + 6 * i, fontY, sx, sy + (90 * fontsize), 6, 9);
			}
			// 大文字
			else if(stringChar <= 96) {
				sx = (stringChar - 65) * 6;
				sy = fontColor * 18;
				ExBltRect(22, fontX + 6 * i, fontY, sx, sy, 6, 9);
			}
			// 小文字
			else {
				sx = (stringChar - 97) * 6;
				sy = (fontColor * 18) + 9;
				ExBltRect(22, fontX + 6 * i, fontY, sx, sy, 6, 9);
			}
		}
	}
}
// 6 x 9のグリッド単位で座標を指定できるバージョン
void printSMALLFontEX(int32_t fontX, int32_t fontY, const char *fontStr, int32_t fontColor) {
	int32_t		i, sx, sy, stringLength, stringChar;

	stringLength = StrLen(fontStr);
	for(i = 0; i < stringLength; i++) {
		stringChar = CharAt(fontStr, i);

		// スペースでない場合
		if(stringChar != 32) {
			// 数字
			if(stringChar <= 64) {
				sx = (stringChar - 48) * 6 + 132;
				sy = fontColor * 9;
				ExBltRect(31, (fontX + i) * 6, fontY * 9, sx, sy + (90 * fontsize), 6, 9);
			}
			// 大文字
			else if(stringChar <= 96) {
				sx = (stringChar - 65) * 6;
				sy = fontColor * 18;
				ExBltRect(22, (fontX + i) * 6, fontY * 9, sx, sy, 6, 9);
			}
			// 小文字
			else {
				sx = (stringChar - 97) * 6;
				sy = (fontColor * 18) + 9;
				ExBltRect(22, (fontX + i) * 6, fontY * 9, sx, sy, 6, 9);
			}
		}
	}
}

// 大きいバージョン（数字のみ）
void printBIGFont(int32_t fontX, int32_t fontY, const char *fontStr, int32_t fontColor) {
	int32_t		i, sx, sy, stringLength, stringChar;

	stringLength = StrLen(fontStr);
	for(i = 0; i < stringLength; i++) {
		stringChar = CharAt(fontStr, i);
		sx = (stringChar -48) * 12;
		sy = fontColor * 16;
		if(sx >= 0) ExBltRect(31, fontX + 12 * i, fontY, sx, sy + (160 * fontsize), 12, 16);
	}
}
// とっても小さいバージョン　6x7
// 数字、/ : < > 、アルファベット大文字が使用可能　ただし字詰め無し
void printTinyFont(int32_t fontX, int32_t fontY, const char *fontStr) {
	int32_t		i, sx, sy, stringLength, stringChar;

	stringLength = StrLen(fontStr);
	for(i = 0; i < stringLength; i++) {
		stringChar = CharAt(fontStr, i);
		sx = ((stringChar - 32) % 32) * 6;
		sy = ((stringChar - 32) / 32) * 7;
		ExBltRect(85, fontX + i * 6, fontY, sx, 287 + sy, 6, 7);
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  描画すべきブロックの画像を決める #1.60c7r7
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
int32_t getBlockPlaneNo(int32_t player, int32_t color) {
	// プラチナブロックやアイテムなど
	if(color >= 10)
		return 46;

	if(heboGB[player] == 1)
		return 74;

	if(heboGB[player] == 2)
		return 64;

	// TGM
	if((rots[player] == 0)||(heboGB[player]==2))
		return 40;

	// ARS
	if((rots[player] == 4) || (rots[player] == 5))
		return 73;

	// TI
	if(rots[player] == 1)
		return 41;

	// WORLD & WORLD2
	if( (rots[player] == 2) || (rots[player] == 3) )
		return 42;

	// WORLD3
	if(rots[player] == 6)
		return 43;

	//SRS-X
	if(rots[player] == 7)
		return 64;
	//DRS
	return 80;
}
// beboblk_big.png使用時の数値を求める（BIGブロックとミニブロック）
int32_t getBigBlock_add(int32_t player){
	if(heboGB[player]==1)
		return 6;
	if(heboGB[player]==2)
		return 5;
	if(rots[player] == 0)
		return 0;
	if(rots[player] == 1)
		return 1;
	if((rots[player] == 2) || (rots[player] == 3))
		return 2;
	if((rots[player] == 4) || (rots[player] == 5))
		return 3;
	if(rots[player] == 6)
		return 4;
	if(rots[player] == 7)
		return 5;
	if(rots[player] == 8)
		return 7;
	return 1;
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ブロックを描画する
//  (枠無し NEXTとTLSに使用#1.60c7j1)
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void drawBlockFast(int32_t bx1, int32_t by1, int32_t kind, int32_t rotate, int32_t color, int32_t bright, int32_t offset, int32_t flash, int32_t player, int32_t use3D) {
	int32_t	i, j, bx2, by2, add, tmp, bx5, by5;
	add = 0;
	tmp = 1;
	if(color < -1){
		add = 9;
		color = color + 9;
	}
	if((isrollroll[player]) && (color < 8) && (gameMode[player] != 6))
		color = 0;
	if((isfever[player]) && (color < 8) && (item_timer[player] > 0))
		color = count % 7 + 1;
	if((istrance[player]) && (color < 8)){
		if(count%2==0) color=0;
		else color = ((count % 14) /2) +1;
	}
	if(item_monochrome[player])	//アイテムによる[ ]
		color = 8;

	if((add == 0) && (color != 8) && (color != 9)) tmp = 0;

	for(j = ((!use3D) || (tmp) || (heboGB[player]>0)); j < 2; j++)
	for(i = 0; i < 4; i++) {
		if(rots[player] == 8){
			bx2 = (bx1 + blkDDataX[kind * 16 + rotate * 4 + i]) * 8;
			by2 = (by1 + blkDDataY[kind * 16 + rotate * 4 + i]) * 8;
		}else if( isWRule(player) ) {		// WorldType用 #1.60c5
			bx2 = (bx1 + blkWDataX[kind * 16 + rotate * 4 + i]) * 8;
			by2 = (by1 + blkWDataY[kind * 16 + rotate * 4 + i]) * 8;
		} else {
			bx2 = (bx1 + blkDataX[kind * 16 + rotate * 4 + i]) * 8;
			by2 = (by1 + blkDataY[kind * 16 + rotate * 4 + i]) * 8;
		}
		if(!j){
			if(color < 8) tmp = getBigBlock_add(player);
			else tmp = 8;
			if((flash == 999) || (flash == 0)){
				if(rots[player] == 8){
					bx5 = (bx[player] + blkDDataX[kind * 16 + rotate * 4 + i]);
					by5 = ((by1 - 3) + blkDDataY[kind * 16 + rotate * 4 + i]);
				}else if( isWRule(player) ) {		// WorldType用 #1.60c5
					bx5 = (bx[player] + blkWDataX[kind * 16 + rotate * 4 + i]);
					by5 = ((by1 - 3) + blkWDataY[kind * 16 + rotate * 4 + i]);
				} else {
					bx5 = (bx[player] + blkDataX[kind * 16 + rotate * 4 + i]);
					by5 = ((by1 - 3) + blkDataY[kind * 16 + rotate * 4 + i]);
				}
				if(getFieldBlock(player, bx5 + 1 - (2 * player), by5, 1) == 0){
					if(flash == 999)
						BlendExBltRect(83, bx2 + 8 - (10 * player), by2 + offset, (color * 2) - ((color >= 10) * 20) + (20 * tmp), bright * 8, 2, 8, 112, 112, 112, 154, 154, 154);
					else
						ExBltFastRect(83, bx2 + 8 - (10 * player), by2 + offset, (color * 2) - ((color >= 10) * 20) + (20 * tmp), bright * 8, 2, 8 );
				}
			}else if(flash == 1)
				ExBltFastRect(83, bx2 + 8 - (10 * player), by2 + offset, (color * 2) - ((color >= 10) * 20) + (20 * tmp), bright * 8, 2, 8 );
		}

		// TLS用#1.60c7j3
		if(flash == 999) {
			if(j) BlendExBltRect(getBlockPlaneNo(player, color), bx2, by2 + offset, ((color+add) * 8) - ((color >= 10) * 80), bright * 8, 8, 8, 112, 112, 112, 154, 154, 154);
		// 普通
		} else {
			ExBltFastRect(getBlockPlaneNo(player, color), bx2, by2 + offset, ((color+add) * 8) - ((color >= 10) * 80), bright * 8, 8, 8);
		}
	}
}
//BIG版　用途はTLSのみ
void drawBigBlockFast(int32_t bx1, int32_t by1, int32_t kind, int32_t rotate, int32_t color, int32_t bright, int32_t offset, int32_t flash, int32_t player, int32_t use3D) {
	int32_t	i, j, bx2, by2, add, tmp, bx5, by5, pos[2], k, l = 0;
	add = 0;
	tmp = 1;
	if(color < -1){
		add = 9;
		color = color + 9;
	}
	if((isrollroll[player]) && (color < 8) && (gameMode[player] != 6))
		color = 0;
	if((isfever[player]) && (color < 8) && (item_timer[player] > 0))
		color = count % 7 + 1;
	if((istrance[player]) && (color < 8)){
		if(count%2==0) color=0;
		else color = ((count % 14) /2) +1;
	}
	if(item_monochrome[player])	//アイテムによる[ ]
		color = 8;
	if((add == 0) && (color != 8) && (color != 9)) tmp = 0;

	for(j = ((!use3D) || (tmp) || (heboGB[player]>0)); j < 2; j++)
	for(i = 0; i < 4; i++) {
		if(rots[player] == 8){
			bx2 = (bx1 + blkDDataX[kind * 16 + rotate * 4 + i] * 2) * 8;
			by2 = (by1 + blkDDataY[kind * 16 + rotate * 4 + i] * 2) * 8;
		}else if( isWRule(player) ) {		// WorldType用 #1.60c5
			bx2 = (bx1 + blkWDataX[kind * 16 + rotate * 4 + i] * 2) * 8;
			by2 = (by1 + blkWDataY[kind * 16 + rotate * 4 + i] * 2) * 8;
		} else {
			bx2 = (bx1 + blkDataX[kind * 16 + rotate * 4 + i] * 2) * 8;
			by2 = (by1 + blkDataY[kind * 16 + rotate * 4 + i] * 2) * 8;
		}
		k = getBigBlock_add(player);

		if(!j){
			if(color < 8) tmp = getBigBlock_add(player);
			else tmp = 8;
				if(rots[player] == 8){
					bx5 = (bx[player] + blkDDataX[kind * 16 + rotate * 4 + i] * 2);
					by5 = ((by1 - 3) + blkDDataY[kind * 16 + rotate * 4 + i] * 2);
				}else if( isWRule(player) ) {		// WorldType用 #1.60c5
					bx5 = (bx[player] + blkWDataX[kind * 16 + rotate * 4 + i] * 2);
					by5 = ((by1 - 3) + blkWDataY[kind * 16 + rotate * 4 + i] * 2);
				} else {
					bx5 = (bx[player] + blkDataX[kind * 16 + rotate * 4 + i] * 2);
					by5 = ((by1 - 3) + blkDataY[kind * 16 + rotate * 4 + i] * 2);
				}
				pos[0] = getFieldBlock(player, bx5 + 2 - (3 * player), by5, 1);
				pos[1] = getFieldBlock(player, bx5 + 2 - (3 * player), by5 + 1, 1);
				if((pos[0] == 0) || (pos[1] == 0)){
					if(flash == 999){
						if(color > 10)
							BlendExBltRectR(83, bx2 + 16 - (22 * player), by2 + offset + (8 * (pos[0] != 0)), (color * 2) - 20 + (20 * tmp), bright * 8 + (4 * (pos[0])), 2, 4 + (4 * ((!pos[1]) && (!pos[0]))) ,  112, 112, 112, 154, 154, 154, 65536*2,65536*2);
						else
							BlendExBltRect(83, bx2 + 16 - (20 * player), by2 + offset + (8 * (pos[0] != 0)), (color * 4) + (40 * tmp), 136 + bright * 16 + (8 * (pos[0])), 4, 8 + (8 * ((!pos[1]) && (!pos[0]))), 112, 112, 112, 154, 154, 154  );
					}else{
						if(color > 10)
							ExBltFastRectR(83, bx2 + 16 - (22 * player), by2 + offset + (8 * (pos[0] != 0)), (color * 2) - 20 + (20 * tmp), bright * 8 + (4 * (pos[0])), 2, 4 + (4 * ((!pos[1]) && (!pos[0]))) , 65536*2,65536*2);
						else
							ExBltFastRect(83, bx2 + 16 - (20 * player), by2 + offset + (8 * (pos[0] != 0)), (color * 4) + (40 * tmp), 136 + bright * 16 + (8 * (pos[0])), 4, 8 + (8 * ((!pos[1]) && (!pos[0]))) );
					}
				}
		}
		// 半透明TLS
		if(flash == 999) {
			if(j){
				if(color > 10)
					BlendExBltRectR(getBlockPlaneNo(player, color), bx2, by2 + offset,(color+add) * 8 - ((color >= 10) * 80), bright * 8, 8, 8, 112, 112, 112, 154, 154, 154, 65536*2,65536*2);
				else
					BlendExBltRect(76, bx2, by2 + offset,((color+add) * 16) + (l * 160), (bright * 16) + (k * 112), 16, 16, 112, 112, 112, 154, 154, 154);
			}
		// 不透明TLS
		} else {
			if(j){
				if(color > 10)
					ExBltFastRectR(getBlockPlaneNo(player, color), bx2, by2 + offset, (color+add) * 8 - ((color >= 10) * 80), bright * 8, 8, 8, 65536*2,65536*2);
				else
					ExBltFastRect(83, bx2, by2 + offset,((color+add) * 16), 272 + (k * 16), 16, 16);
			}
		}
	}
}
//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ブロックを描画する
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
// c7g6 drawBlockにplayer関数を追加しました。
void drawBlock(int32_t bx1, int32_t by1, int32_t kind, int32_t rotate, int32_t color, int32_t bright, int32_t offset, int32_t flash, int32_t player, int32_t use3D) {
	int32_t	i, j, bx2, by2, add, tmp, bx5, by5;
	add = 0;
	tmp = 1;
	if( ((isrotatelock[player]) && (color < 8)) || ((ismagnetblock[player]) && (count%2 == 0)) || ((isrollroll[player]) && (color < 8)) )
		color = 0;
	if((isdark[player]) && (hidden[player] == 8))	//HIDDEN LV8でのダークブロックは操作ブロックがステルス化
		return;
	if(color < -1){
		add = 9;
		color = color + 9;
	}


	if((isfever[player]) && (color < 8))
		color = count % 7 + 1;
	if((istrance[player]) && (color < 8)){
		if(count%2==0) color=0;
		else color = ((count % 14) /2) +1;
	}
	if(item_monochrome[player])	//アイテムによる[ ]
		color = 8;

	if((add == 0) && (color != 8) && (color != 9)) tmp = 0;

	for(j = ((!use3D) || (tmp) || (flash == 999) || (heboGB[player]>0)); j < 3; j++){
		for(i = 0; i < 4; i++) {
			if(rots[player] == 8){
				bx2 = (bx1 + blkDDataX[kind * 16 + rotate * 4 + i]) * 8;
				by2 = (by1 + blkDDataY[kind * 16 + rotate * 4 + i]) * 8;
			}else if( isWRule(player) ) {		// WorldType用 #1.60c5
				bx2 = (bx1 + blkWDataX[kind * 16 + rotate * 4 + i]) * 8;
				by2 = (by1 + blkWDataY[kind * 16 + rotate * 4 + i]) * 8;
			} else {
				bx2 = (bx1 + blkDataX[kind * 16 + rotate * 4 + i]) * 8;
				by2 = (by1 + blkDataY[kind * 16 + rotate * 4 + i]) * 8;
			}
			if(j == 0){
				if(rots[player] == 8){
					bx5 = (bx[player] + blkDDataX[kind * 16 + rotate * 4 + i]);
					by5 = (by[player] + blkDDataY[kind * 16 + rotate * 4 + i]);
				}else if( isWRule(player) ) {		// WorldType用 #1.60c5
					bx5 = (bx[player] + blkWDataX[kind * 16 + rotate * 4 + i]);
					by5 = (by[player] + blkWDataY[kind * 16 + rotate * 4 + i]);
				} else {
					bx5 = (bx[player] + blkDataX[kind * 16 + rotate * 4 + i]);
					by5 = (by[player] + blkDataY[kind * 16 + rotate * 4 + i]);
				}
				if(getFieldBlock(player, bx5 + 1 - (2 * player), by5, 1) == 0){
					if(color < 8) tmp = getBigBlock_add(player);
					else tmp = 8;
					if(flash != 10)
						ExBltFastRect(83, bx2 + 8 - (10 * player), by2 + offset, (color * 2) - ((color >= 10) * 20) + (20 * tmp), bright * 8, 2, 8 );
					else if(hidden[player] != 8){
						if( (color < 8) && (rots[player] != 6)) {
							ExBltFastRect(83, bx2 + 8 - (10 * player), by2 + offset, 20 * tmp, 0, 2, 8 );
						} else {
							ExBltFastRect(83, bx2 + 8 - (10 * player), by2 + offset, (color * 2) - ((color >= 10) * 20) + (20 * tmp), 0, 2, 8 );
						}
					}
				}
			} else if(j == 1) {
				// ブロック枠がONの時のみ枠を表示 #1.60c7f
				if(((blockflash != 2) && (heboGB[player]==0)) && !((hidden[player] == 8) && (flash == 10))) {
					// LV1000以下の時だけ表示する #1.60c7d
					// PRACTICE対応 #1.60c7e
					// playerごと #1.60c7h8
					if((flash > 0) && (flash < 999) && (color <= 7) && (add == 0))
						ExBltFastRect(29, bx2 - 1, by2 - 1 + offset, 60, 80, 10, 10);
				}
				// HOLDは個別設定#1.60c7i1
				if((enable_hold_frame) && (hold_used[player] == 2) && (dhold[player] == 1) && (flash != 10)) {
					// 点滅させる#1.60c7p9ex
					if (count % 16 < 8)
						ExBltFastRect(29, bx2 - 1, by2 - 1 + offset, 80, 80, 10, 10);
					else
						ExBltFastRect(29, bx2 - 1, by2 - 1 + offset, 70, 80, 10, 10);
				}
			} else {
				if(flash == 999) BlendExBltRect(getBlockPlaneNo(player, color), bx2, by2 + offset, (color+add * 8) - ((color >= 10) * 80), bright * 8, 8, 8, 112, 112, 112, 154, 154, 154);
				else if(flash == 10) {
					// []ブロックの時は固定時の点滅をしない #1.60c7f
					// playerごと #1.60c7h8
					// 条件を簡略化#1.60c7i3
					if((hidden[player] == 8) && (m_roll_blockframe)){
						ExBltFastRect(24, bx2, by2 + offset, 0, 0, 8, 8);
					} else if( (color < 8) && ((rots[player] != 6) && (add == 0))) {
						ExBltFastRect(getBlockPlaneNo(player, 0), bx2, by2 + offset, 0, 0, 8, 8);
					} else if((rots[player] != 6) && (color < 10)) {
						ExBltFastRect(getBlockPlaneNo(player, 0), bx2, by2 + offset, 64, 0, 8, 8);
					} else {
						ExBltFastRect(getBlockPlaneNo(player, color), bx2, by2 + offset, ((color+add) * 8) - ((color >= 10) * 80), 0, 8, 8);
					}
				}
				else if(flash < 2){
					ExBltFastRect(getBlockPlaneNo(player, color), bx2, by2 + offset, ((color+add) * 8) - ((color >= 10) * 80), bright * 8, 8, 8);
				}
			}
		}
		if((j == 2) && ((flash != 10) || (status[player] == 5))){
			// 周り枠 #1.60c7o5
			if( (flash != 999) && (block_rframe) ) {
				if(item[player] != 0)
					ExBltRect(28, (bx1-1)*8  ,by1*8 - 16 + offset - (8 * ((kind == 2) && (isWRule(player))) ) , 0, (item[player] - 1) * 16, 48, 16);
				if((kind != 0) && (kind != 2))
					ExBltRect(29, bx1*8 - 1, by1*8 - 1 + offset, 0, 0, 26, 26);
				else
					ExBltRect(29, bx1*8 - 1, by1*8 - 1 + offset - 8 * ((isWRule(player)) && (kind == 2)), 27, 0, 34, 34);
				sprintf(string[0],"%2d",wait3[player] - bk[player]);
				printTinyFont(bx1*8 - 1 + 16 + (8 * ((kind == 0) || (kind == 2))),(by1 - 3)*8 - 1 + offset + 50 + (8 * ( (kind == 0) || ((kind == 2) && (!isWRule(player))) ) ) ,string[0]);
			}
		}
	}
}

/* BIGブロック描画#1.60c7j8 */
void drawBigBlockNew(int32_t bx1, int32_t by1, int32_t kind, int32_t rotate, int32_t color, int32_t bright, int32_t offset, int32_t flash, int32_t player, int32_t use3D) {
	int32_t	i, j, bx2, by2, tmp, pos[2];
	int32_t	k, l, bx3, by3, add, bx5, by5;
	add = 0;
	tmp = 1;
	if( ((isrotatelock[player]) && (color < 8)) || ((ismagnetblock[player]) && (count%2 == 0)) || ((isrollroll[player]) && (color < 8)) )
		color = 0;
	if((isdark[player]) && (hidden[player] == 8))	//HIDDEN LV8でのダークブロックは操作ブロックがステルス化
		return;
	if(color < -1){
		add = 9;
		color = color + 9;
	}
	if((isfever[player]) && (color < 8))
		color = count % 7 + 1;
	if((istrance[player]) && (color < 8)){
		if(count%2==0) color=0;
		else color = ((count % 14) /2) +1;
	}

	if(item_monochrome[player])	//アイテムによる[ ]
		color = 8;

	if((add == 0) && (color != 8) && (color != 9)) tmp = 0;

	for(j = ((!use3D) || (tmp) || (flash == 999) || (heboGB[player])); j < 3; j++){
		for(i = 0; i < 4; i++) {
			if(rots[player] == 8){
				bx2 = (bx1 + blkDDataX[kind * 16 + rotate * 4 + i] * 2) * 8;
				by2 = (by1 + blkDDataY[kind * 16 + rotate * 4 + i] * 2) * 8;
			}else if( isWRule(player) ) {		// WorldType用 #1.60c5
				bx2 = (bx1 + blkWDataX[kind * 16 + rotate * 4 + i] * 2) * 8;
				by2 = (by1 + blkWDataY[kind * 16 + rotate * 4 + i] * 2) * 8;
			} else {
				bx2 = (bx1 + blkDataX[kind * 16 + rotate * 4 + i] * 2) * 8;
				by2 = (by1 + blkDataY[kind * 16 + rotate * 4 + i] * 2) * 8;
			}
			if(j == 0){
				if(rots[player] == 8){
					bx5 = (bx[player] + blkDDataX[kind * 16 + rotate * 4 + i] * 2);
					by5 = (by[player] + blkDDataY[kind * 16 + rotate * 4 + i] * 2);
				}else if( isWRule(player) ) {		// WorldType用 #1.60c5
					bx5 = (bx[player] + blkWDataX[kind * 16 + rotate * 4 + i] * 2);
					by5 = (by[player] + blkWDataY[kind * 16 + rotate * 4 + i] * 2);
				} else {
					bx5 = (bx[player] + blkDataX[kind * 16 + rotate * 4 + i] * 2);
					by5 = (by[player] + blkDataY[kind * 16 + rotate * 4 + i] * 2);
				}
				pos[0] = getFieldBlock(player, bx5 + 2 - (3 * player), by5    , 1);
				pos[1] = getFieldBlock(player, bx5 + 2 - (3 * player), by5 + 1, 1);
				if((pos[0] == 0) || (pos[1] == 0)){
					if(color < 8) tmp = getBigBlock_add(player);
					else tmp = 8;
					if(flash != 10){
						if(color > 10)
							ExBltFastRectR(83, bx2 + 16 - (22 * player), by2 + offset + (8 * (pos[0] != 0)), (color * 2) - 20 + (20 * tmp), bright * 8 + (4 * (pos[0])), 2, 4 + (4 * ((!pos[1]) && (!pos[0]))) , 65536*2,65536*2);
						else
							ExBltFastRect(83, bx2 + 16 - (20 * player), by2 + offset + (8 * (pos[0] != 0)), (color * 4) + (40 * tmp), 136 + bright * 16 + (8 * (pos[0])), 4, 8 + (8 * ((!pos[1]) && (!pos[0]))) );
					}else if((hidden[player] != 8) && (color <= 10)){
						if( (color < 8) && (rots[player] != 6)) {
							ExBltFastRect(83, bx2 + 16 - (20 * player), by2 + offset + (8 * (pos[0] != 0)), 40 * tmp, 136 + (8 * (pos[0])), 4, 8 + (8 * ((!pos[1]) && (!pos[0]))) );
						} else {
							ExBltFastRect(83, bx2 + 16 - (20 * player), by2 + offset + (8 * (pos[0] != 0)), (color * 4) + (40 * tmp), 136 + (8 * (pos[0])), 4, 8 + (8 * ((!pos[1]) && (!pos[0]))) );
						}
					}
				}
			} else if(j == 1) {
				if( flash != 999 ) {
					// 枠
					if(((blockflash != 2) && (!heboGB[player])) && !((hidden[player] == 8) && (flash == 10))) {
						if((flash > 0) && (flash < 999) && (color <= 7) && (add == 0))
							ExBltRect(29, bx2 - 2, by2 - 2 + offset, 60, 90, 20, 20);
					}
					// HOLD
					if((enable_hold_frame) && (hold_used[player] == 2) && (dhold[player] == 1) && (flash != 10)) {
						// 点滅させる#1.60c7p9ex
						if (count % 16 < 8)
							ExBltRect(29, bx2 - 2, by2 - 2 + offset, 100, 90, 20, 20);
						else
							ExBltRect(29, bx2 - 2, by2 - 2 + offset, 80, 90, 20, 20);
					}
				}
			} else {
				if(color > 10){	//アイテムブロック
					if(flash == 999) BlendExBltRectR(getBlockPlaneNo(player, color), bx2, by2 + offset,(color+add) * 8 - ((color >= 10) * 80), bright * 8, 8, 8, 112, 112, 112, 154, 154, 154, 65536*2,65536*2);
					else if(flash == 10) {
						// 固定時の点滅
						ExBltFastRectR(getBlockPlaneNo(player, color), bx2, by2 + offset, ((color+add) * 8) - ((color >= 10) * 80), 0, 8, 8, 65536*2,65536*2);
					} else if(flash < 2) ExBltFastRectR(getBlockPlaneNo(player, color), bx2, by2 + offset, (color+add) * 8 - ((color >= 10) * 80), bright * 8, 8, 8, 65536*2,65536*2);
				} else {	//通常のブロック
					k = getBigBlock_add(player);
					l = 0;
					if(k >= 4){
						k = k-4;
						l = 1;
					}
					if(flash == 999) BlendExBltRect(76, bx2, by2 + offset,((color+add) * 16) + (l * 160), (bright * 16) + (k * 112), 16, 16, 112, 112, 112, 154, 154, 154);
					else if(flash == 10) {
						// 固定時の点滅
						if((hidden[player] == 8) && (m_roll_blockframe)){
							ExBltFastRect(24, bx2, by2 + offset, 0, 0, 16, 16);
						} else if( (color < 8) && ((rots[player] != 6) && (add == 0))) {
							ExBltFastRect(76, bx2, by2 + offset,0 + (l * 160), 0 + (k * 112), 16, 16);
						} else if((rots[player] != 6) && (color < 10)) {
							ExBltFastRect(76, bx2, by2 + offset,128 + (l * 160), 0 + (k * 112), 16, 16);
						} else {
							ExBltFastRect(76, bx2, by2 + offset,((color+add) * 16) + (l * 160), (bright * 16) + (k * 112), 16, 16);
						}
					}
					else if(flash < 2) ExBltFastRect(76, bx2, by2 + offset,((color+add) * 16) + (l * 160), (bright * 16) + (k * 112), 16, 16);
				}
			}
		}
		if((j == 2) && ((flash != 10) || (status[player] == 5))){
			// 周り枠 #1.60c7o5
			if( (flash != 999) && (block_rframe) ) {
				if(item[player] != 0)
					ExBltRect(28, (bx1-1)*8  ,by1*8 - 16 + offset - (16 * ((kind == 2) && (isWRule(player))) ) , 0, (item[player] - 1) * 16, 48, 16);
				if((kind != 0) && (kind != 2))
					ExBltRectR(29, bx1*8 - 2, by1*8 - 2 + offset, 0, 0, 26, 26, 65536*2,65536*2);
				else
					ExBltRectR(29, bx1*8 - 2, by1*8 - 2 + offset - 16 * ((isWRule(player)) && (kind == 2)), 27, 0, 34, 34, 65536*2,65536*2);
				sprintf(string[0],"%2d",wait3[player] - bk[player]);
				printTinyFont(bx1*8 - 2 + 40 + (16 * ((kind == 0) || (kind == 2))),(by1 - 3)*8 - 2 + offset + 75 + (16 * ( (kind == 0) || ((kind == 2) && (!isWRule(player))) ) ) ,string[0]);
			}
		}
	}
}

// Draw Current Block
// 現在のツモを表示 Big対応 #1.60c7
void drawCBlock(int32_t player, int32_t color, int32_t bright, int32_t offset, int32_t flash, int32_t not3D) {
	/* BIG時の描画は別関数に独立しました #1.60c7j8 */

	// アイテム #1.60c7n1
	if( item[player] ) {
		c_cblk[player] = 16 + item[player];
	}

	if(heboGB[player] != 0) bright = 0;

	// 描画
	if(IsBig[player]) {
		// BIG時
		drawBigBlockNew(bx[player] + 15 + 24 * player - 12 * maxPlay, by[player] + 3,
			blk[player], rt[player], c_cblk[player], bright, offset, flash, player,((disable_giji3D < 3) && (!not3D)));
	} else {
		// 通常時
		drawBlock(bx[player] + 15 + 24 * player - 12 * maxPlay, by[player] + 3,
			blk[player], rt[player], c_cblk[player], bright, offset, flash, player, ((disable_giji3D < 3) && (!not3D)));
	}
}

// T.L.S. (Temporary Landing System) もどき
// classic.cとworld.cから独立 #1.60c7k4
void drawTLSBlock(int32_t player, int32_t bottom) {
	if( !isTLS(player) ) return;

	// 描画
	if(IsBig[player]) {
		// BIG時
		drawBigBlockFast(bx[player] + 15 + 24 * player - 12 * maxPlay, bottom + 2, blk[player],
					rt[player], c_cblk[player], (7 + 5 * (c_cblk[player] > 10)) * (alpha_tls < 2), 0, 999 * (alpha_tls == 2), player, (disable_giji3D == 0));
	} else {
		drawBlockFast(bx[player] + 15 + 24 * player - 12 * maxPlay, bottom + 2, blk[player]
					, rt[player], c_cblk[player], 12 * (!alpha_tls), 0, 999 * (alpha_tls), player, (disable_giji3D < 2));
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  hold・3Next用縮小drawBlock
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
// 変数playerを追加しました#1.60c7j1
// ブロックの向きも渡すように変更 C7U1
// 専用の画像を使用 C7U1.7
void drawhBlock(int32_t bx1, int32_t by1, int32_t kind, int32_t color, int32_t r, int32_t player, int32_t rotate, int32_t use3D) {
	int32_t		i, j,add,k,tmp;
	add = 0;
	tmp = 1;
	if(color < -1){
		add = 9;
		color = color + 9;
	}
	if(item_monochrome[player])	//アイテムによる[ ]
		color = 8;
	if((add == 0) && (color != 8) && (color != 9)) tmp = 0;

	if(color < 10){
		k = getBigBlock_add(player);
	}else{
		k = 8;
		color = color - 17;
	}

	for(j = ((tmp) || (!use3D)); j < 2; j++)
	for(i = 0; i < 4; i++)
		if(!j){
			if(rots[player] == 8)
				ExBltFastRect(83,
				bx1 + blkDDataX[kind * 16 + rotate * 4 + i] * r + (4 - (5 * player)),
				by1 + blkDDataY[kind * 16 + rotate * 4 + i] * r,
				color + ((k == 8) * 7) + (10 * k), 264, 1, 4);
			else if( isWRule(player) )		// WorldType用 #1.60c5
				ExBltFastRect(83,
				bx1 + blkWDataX[kind * 16 + rotate * 4 + i] * r + (4 - (5 * player)),
				by1 + blkWDataY[kind * 16 + rotate * 4 + i] * r,
				color + ((k == 8) * 7) + (10 * k), 264, 1, 4);
			else
				ExBltFastRect(83,
				bx1 + blkDataX[kind * 16 + rotate * 4 + i] * r + (4 - (5 * player)),
				by1 + blkDataY[kind * 16 + rotate * 4 + i] * r,
				color + ((k == 8) * 7) + (10 * k), 264, 1, 4);
		}else{
			if(rots[player] == 8)
				ExBltFastRect(76,
				bx1 + blkDDataX[kind * 16 + rotate * 4 + i] * r,
				by1 + blkDDataY[kind * 16 + rotate * 4 + i] * r,
				0 + ((color+add) * 4),448 + (k * 4), 4, 4);
			else if( isWRule(player) )		// WorldType用 #1.60c5
				ExBltFastRect(76,
				bx1 + blkWDataX[kind * 16 + rotate * 4 + i] * r,
				by1 + blkWDataY[kind * 16 + rotate * 4 + i] * r,
				0 + ((color+add) * 4),448 + (k * 4), 4, 4);
			else
				ExBltFastRect(76,
				bx1 + blkDataX[kind * 16 + rotate * 4 + i] * r,
				by1 + blkDataY[kind * 16 + rotate * 4 + i] * r,
				0 + ((color+add) * 4),448 + (k * 4), 4, 4);
		}
}
