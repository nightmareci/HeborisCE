//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  セクションタイムランキング関連
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲

#include "../script/include.h"

int32_t		st_version = 2;	// バージョン
						// 0:無効 1:c7p6/
//30*3　(ビギ+マス123+20G), デビ+マス4,トモ が30づつ(ただし表示数は違う)
int32_t		st_time[90];	// タイム
int32_t		st_end[90];		// 0:未カンスト 1:ロール途中窒息 2:完全クリア
int32_t		st_lvstop[90];		//レベルストップした時間
int32_t		st_others[90];		//落としたブロック数

//cstr*		st_rkname[135];			// 名前
// 初期化
void ST_RankingInit(void) {
	int32_t	i, j;

	for(i=0; i<90; i++) {
		st_time[i] = 5400;
		st_end[i] = 0;
		st_lvstop[i] = 0;
		st_others[i] = 0;
	}

}

// STメダルの色を判定
int32_t ST_RankingCheck(int32_t player, int32_t rmode, int32_t section,int32_t enable_grade) {
	int32_t	tmp;

	if((rmode >= 4) || (rmode == 0)) return 0;

	tmp = ST_rankingGet(player,rmode,enable_grade);

	//-5秒更新
	if(lap_time[section + player * 100] <= st_time[section + tmp] - 300) {
		return 4;
	}
	// 記録更新
	if(lap_time[section + player * 100] <= st_time[section + tmp]) {
		return 3;
	}
	// 5秒以内
	if(lap_time[section + player * 100] <= st_time[section + tmp] + 300) {
		return 2;
	}
	// 10秒以内
	if(lap_time[section + player * 100] <= st_time[section + tmp] + 600) {
		return 1;
	}

	return 0;
}

// 記録更新したかチェック
int32_t ST_RankingCheckAll(int32_t player, int32_t rmode, int32_t enable_grade) {
	int32_t i,tmp2;

	tmp2 = ST_rankingGet(player,rmode,enable_grade);

	if(rmode == 6) {
			for(i=0; i<stage[player]+ (stage[player] == laststage[player]); i++) {
				if((stage_time[i + player * 30] <= st_time[i + tmp2]) && (stage_time[i + player * 30] != 0)) {
					return 1;
				}
			}
	}
	else if((rmode <= 3) && (rmode != 0)) {
		for(i=0; i<(tc[player] / (st_record_interval_tgm * 10)) + ((rmode >= 1)&&(rmode <= 2)&&(tc[player] == 999)); i++) {
			if((lap_time[i + player * 100] <= st_time[i + tmp2]) && (lap_time[i + player * 100] != 0)) {
				return 1;
			}
		}
	}
	return 0;
}
// ステージごとの記録更新したかチェック
int32_t Stage_RankingCheck(int32_t player, int32_t rmode) {
	int32_t i,tmp3;
	if(stage[player] >= 27) return 0;

	tmp3 = ST_rankingGet(player,rmode,0);

	if((stage_time[stage[player] + player * 30] <= st_time[stage[player] + tmp3]) && (stage_time[stage[player] + player * 30] != 0)) {
		return 1;
	}
	return 0;
}
// セクションタイムランキング更新
void ST_RankingUpdate(int32_t player, int32_t rmode, int32_t end,int32_t enable_grade) {
	int32_t i, temp,tmp4;

	tmp4 = ST_rankingGet(player,rmode,enable_grade);

	if(rmode == 6){//TOMOYO
		for(i=0; i<stage[player] + (stage[player] == laststage[player]); i++) {
			if((stage_time[i + player * 30] <= st_time[i + tmp4]) && (stage_time[i + player * 30] != 0)) {
				st_time[i + tmp4] = stage_time[i + player * 30];	// タイム
				st_end[i + tmp4] = end;
				st_others[i + tmp4] = st_other[i + player * 30];
			}
		}
	}
	else if(rmode >= 4) return;

	if(tc[player] < st_record_interval_tgm * 10) {
		return;
	}

	for(i=0; i<(tc[player] / (st_record_interval_tgm * 10)) + ((rmode >= 1)&&(rmode <= 2)&&(tc[player] == 999)); i++) {
		if((lap_time[i + player * 100] <= st_time[i + tmp4]) && (lap_time[i + player * 100] != 0)) {
			st_time[i + tmp4] = lap_time[i + player * 100];	// タイム
			st_end[i + tmp4] = end;		// エンディング到達
			st_lvstop[i + tmp4] = lvstop_time[i + player * 20];
			st_others[i + tmp4] = st_other[i + player * 30];
		}
	}
}

// ファイルフォーマット (4byte単位)
//   0      バージョン
//   1〜  9 未使用（自由に使用可
//  10〜 61 タイム			10-90
//  62〜113 エンディング到達		91-171
// 114〜165 名前			172-252
//					253-333

// セーブ
void ST_RankingSave(void) {//12345 6789
	int32_t i,j, temp2[3];

	FillMemory(&saveBuf, 5000 * 4, 0);

	saveBuf[0] = st_version;

	for(i=0;i<90;i++) {
		saveBuf[i + 10] = st_time[i];
		saveBuf[i + 99] = st_end[i];
		saveBuf[i + 188] = st_lvstop[i];
		saveBuf[i + 277] = st_others[i];
	}

	SaveFile("config/data/STRANKING.SAV", &saveBuf, 400 * 4);
}

// ロード
int32_t ST_RankingLoad(void) {
	int32_t i,j, temp2[3];

	FillMemory(&saveBuf, 5000 * 4, 0);

	// バージョンを見る
	LoadFile("config/data/STRANKING.SAV", &saveBuf, 4);

	if(saveBuf[0] != st_version) {
		return 1;
	}

	// 全体を読み込む
	LoadFile("config/data/STRANKING.SAV", &saveBuf, 400 * 4);

	for(i=0;i<90;i++) {
		st_time[i] = saveBuf[i + 10];
		st_end[i] = saveBuf[i + 99];
		st_lvstop[i] = saveBuf[i + 188];
		st_others[i] = saveBuf[i + 277];
	}
	return 0;
}
//ベストタイム(ACEのはranking2.c)
void viewbestSTtime(int32_t player){
	int32_t	tempbest,color;

	if(Isbesttime==0)return;

	color = (count % 4 / 2) * (sp[player] >= 1200) * digitc[rots[player]];
	if((gameMode[player]>=0)&&(gameMode[player]<=3)){//セクションタイム
	}else if((gameMode[player]==6) && (stage[player] < 27)){//ベストタイム
	//	if( (gameMode[player] == 6) && (!maxPlay) ) {
			printFont(26+2*((hnext[player] >= 4) && (!player)) + 7 * player - 12 * maxPlay, 11, "BEST TIME", fontc[rots[player]]);

			tempbest = ST_rankingGet(player,6,0);
			getTime(st_time[stage[player] + tempbest]);
			printFont(26+2*((hnext[player] >= 4) && (!player)) + 7 * player - 12 * maxPlay, 12, string[0], color);
	}
}
//ベストタイムスモールフォント
void viewbestSTtimes(int32_t player){
	int32_t	tempbest,color;

	if(Isbesttime==0)return;

	color = (count % 4 / 2) * (sp[player] >= 1200) * digitc[rots[player]];
	if((gameMode[player]>=0)&&(gameMode[player]<=3)){//セクションタイム

	}else if((gameMode[player]==6) && (stage[player] < 27)){//ベストタイム
		if( (gameMode[player] == 6) && ((!maxPlay) || (status[1 - player] == 0)) ) {
			//bestの文字
			ExBltRect(3, 208+20*((hnext[player] >= 4) && (!player)) + 70 * player - 96 * maxPlay, 95, 251, 91, 21, 7);
			//timeの文字
			ExBltRect(3, 230+20*((hnext[player] >= 4) && (!player)) + 70 * player - 96 * maxPlay, 95, 180, 119, 19, 7);

			tempbest = ST_rankingGet(player,6,0);
			getTime(st_time[stage[player] + tempbest]);
			printSMALLFont((26 + 8 * player - 12 * maxPlay)*8+20*((hnext[player] >= 4) && (!player)), 103, string[0], color);
		}
	}
}
//STランキングのアドレス指定
int32_t ST_rankingGet(int32_t player,int32_t rmode,int32_t enable_grade){
	int32_t	temp;
	if(rmode == 0){//ビギナー
		temp = 0;
	}else if(rmode == 1){//マスター
		if(enable_grade !=4){
		temp = 4;
		}else if(enable_grade==4){
		temp = 15;
		}
	}else if(rmode == 2){//20G
		temp = 30;
	}else if(rmode == 3){//デビル
		temp = 40;
	}else if(rmode == 6){//TOMOYO
		temp =  60;
	}
	return temp;
}

// セクションタイムランキング表示
void ST_RankingView() {
	int32_t i, max, mode, tmp, tmp5, bps, bps1,bps2,s;

	mode = 1;

	while(1) {
		// 背景描画
		count++;
		if(background == 0) {
			for(i = 0; i <= 6; i++) {
				ExBltFastRect(4 + (mode >= 3) + (mode == 4), 96 * i - (count % 96) / 3, 0, 0, 0, 96, 240);
			}
		} else if(background == 1) {
			for(i = 0; i <= 6; i++) {
				ExBltFastRect(4 + (mode >= 3) + (mode == 4), 96 * i, 0, 0, 0, 96, 240);
			}
		} else {
			ExBltFast(30, 0, 0);
		}
		ExBltRect(77, 0, 208,  count % 320, 20, 320 - (count % 320), 8);
		ExBltRect(77, 320 - (count % 320), 208,  0, 20, count % 320, 8);

		ExBltRect(77, count % 320, 16,  0, 28, 320 - (count % 320), 8);
		ExBltRect(77, 0, 16, 320 - (count % 320), 28, count % 320, 8);

		//アドレス指定
		if(mode == 0){
			tmp5 = 0;
		}else if(mode == 1){
			tmp5 = 4;
		}else if(mode == 2){
			tmp5 = 15;
		}else if(mode == 3){
			tmp5 = 30;
		}else if(mode == 4){
			tmp5 = 40;
		}else if(mode == 5){
			tmp5 = 60;
		}

		// モード名表示
		if(mode == 0)
			printFont(11, 1, "- BEGINNER MODE -", 4);
		else if(mode == 1)
			printFont(11, 1, "-  MASTER MODE  -", 1);
		else if(mode == 2)
			printFont(9, 1, "-  MASTER GRADE4 MODE  -", 1);
		else if(mode == 3)
			printFont(11, 1, "-   20G MODE    -", 7);
		else if(mode == 4)
			printFont(11, 1, "-  DEVIL MODE   -", 2);
		else if(mode == 5)
			printFont(11, 1, "-  TOMOYO MODE   -", 3);

		if(mode == 5){
			printFont(1,  3, "STAGE",1);
			printFont(8,  3, "TIME", 1);
			printFont(15, 3, "USE", 1);
			printFont(20,  3, "STAGE",1);
			printFont(28,  3, "TIME", 1);
			printFont(35, 3, "USE", 1);
		}else if(mode < 5){
			printFont(2,  3, "LEVEL",1);
			printFont(12,  3, "TIME", 1);
			printFont(21, 3, "LVSTOP", 1);
			printFont(28, 3, "USE", 1);
			printFont(32, 3, "BLOCK/S ", 1);
		}

		// 表示数を決める
		if(mode == 0)
			max = 2;
		else if( (mode == 1) || (mode == 2)|| (mode == 3) )
			max = 10;
		else if(mode == 4)
			max = 13;
		else if(mode == 5)
			max =27;

		// ランキング表示
		for(i=0; i<max; i++) {
			if(st_end[i + tmp5] == 1)
				tmp = 4;
			else if(st_end[i + tmp5] == 2)
				tmp = 7;
			else
				tmp = 0;

			if(mode != 5){//その他
				if( ((mode == 1) || (mode == 2)|| (mode == 3)) && (i == 9) ){
					sprintf(string[0], " 900-999");
				} else if(mode <= 4){
					sprintf(string[0], "%4d-%3d", i*100, (i+1)*100);
				}

				printFont(1, 4+i, string[0], tmp);

				//タイム
				getTime(st_time[i + tmp5]);
				printFont(11, 4+i, string[0], tmp);

				//レベルストップ
				getSTime(st_lvstop[i + tmp5]);
				printFont(21, 4+i, string[0], tmp);

				//USEブロック数
				sprintf(string[0],"%d",st_others[i + tmp5]);
				printFont(29, 4+i, string[0], tmp);

				//BPS
				bps = (st_others[i + tmp5] * 1000) / (st_time[i + tmp5] / 60);
				bps1 = bps / 1000;//整数
				bps2 = bps % 1000;//下三桁
				sprintf(string[0],"%d.",bps1);
				printFont(32, 4+i, string[0], tmp);
				if(bps2>=100){
				sprintf(string[0],"%3d",bps2);
				printFont(34, 4+i, string[0], tmp);
				}else if(bps2>=10){
				sprintf(string[0],"0%2d",bps2);
				printFont(34, 4+i, string[0], tmp);
				}else{
				sprintf(string[0],"00%d",bps2);
				printFont(34, 4+i, string[0], tmp);
				}
				if(mode==2){
					printFont(2, 28, "GRADE HISTORY", 2);
					for(s=0;s<5;s++){
					sprintf(string[0],"%d",grade_his[s]);
					printFont(3*s, 29, string[0], tmp);
					}
				}
			}else{ //TOMOYO
				if(i <= 19){
					sprintf(string[0], "%d",i+1);
					printFont(2, 4+i, string[0], tmp);

					//タイム
					getTime(st_time[i + tmp5]);
					printFont(6, 4+i, string[0], tmp);

					//使用ブロック数
					sprintf(string[0],"%d",st_others[i + tmp5]);
					printFont(16, 4+i, string[0], tmp);

				}else if(i > 19){
					sprintf(string[0], "EX%d",i - 19);
					printFont(20, i - 16, string[0], tmp);

					//タイム
					getTime(st_time[i + tmp5]);
					printFont(26, i - 16, string[0], tmp);

					//使用ブロック数
					sprintf(string[0],"%d",st_others[i + tmp5]);
					printFont(36, i-16, string[0], tmp);
				}
			}
		}

		// 合計数表示
		tmp = 0;
		for(i=0; i<max; i++) {
			tmp = tmp + st_time[i + tmp5];
		}
		printFont(1, 25, "TOTAL TIME", 1);
		getTime(tmp);
		printFont(12, 25, string[0], 2);

		KeyInput();

		if(getPushState(0, 4) || getPushState(0, 5) || IsPushEscKey()) {
			// AかBで戻る
			return;
		}

		if(getPushState(0, 2)) {
			// ←
			PlaySE(5);
			mode--;
			if(mode < 1) mode = 5;
		}
		if(getPushState(0, 3)) {
			// →
			PlaySE(5);
			mode++;
			if(mode > 5) mode = 1;
		}

		spriteTime();//halt
	}
}


// ファイルフォーマット (4byte単位)
//   0      バージョン
//   1〜 10 段位ヒストリ
//  10〜 11 認定段位
//  12〜 13

// プレイヤーデータセーブ
void PlayerdataSave(void) {//12345 6789
	int32_t i,j, temp2[3];

	FillMemory(&saveBuf, 500 * 4, 0);

	saveBuf[0] = 0x4F424503;

	for( j = 0 ; j < 2 ; j++){
		for(i=0;i<5;i++) {
			saveBuf[1 + i + 5 * j] =grade_his[i+j*5];
		}
		saveBuf[1 + 10 + j]=admit_grade[j];
		saveBuf[1 + 12 + j]=grade_pasttime[j];
	}

	SaveFile("config/data/PLAYERDATA.SAV", &saveBuf, 100 * 4);
}

// プレイヤーデータロード
int32_t PlayerdataLoad(void) {
	int32_t i,j, temp2[3];

	FillMemory(&saveBuf, 500 * 4, 0);

	// バージョンを見る
	LoadFile("config/data/PLAYERDATA.SAV", &saveBuf, 4);

	if(saveBuf[0] != 0x4F424503) {
		return 1;
	}

	// 全体を読み込む
	LoadFile("config/data/PLAYERDATA.SAV", &saveBuf, 100 * 4);

	for( j = 0 ; j < 2 ; j++){
		for(i=0;i<5;i++) {
			grade_his[i+j*5]=saveBuf[1 + i + 5 * j];
		}
		admit_grade[j] = saveBuf[1 + 10 + j];
		grade_pasttime[j]=saveBuf[1 + 12 + j];
	}
	return 0;
}
