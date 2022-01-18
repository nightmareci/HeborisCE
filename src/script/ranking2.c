//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ランキング関連
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲

#include "../script/include.h"

// ランキング用グローバル変数
// 5名 × 15モード × 2回転ルールタイプ(一個予備)
//カテゴリー
//ビギ、マス*4(段位別)、20G*2、デビ、デビ-、トモTi、トモEH、エス*3、エンドレス
char	rkname[5 * 15 * 2][4];			// 名前
int32_t		rkdata[5 * 15 * 2];		// 花火／段位／ステージ／ライン
int32_t		rktime2[5 * 15 * 2];		// タイム
int32_t		rkclear[5 * 15 * 2];		// 0=途中で窒息 1=ロール失敗 2=ロールクリア
int32_t		rkother[5 * 15 * 2];		// その他（レベルやクリア率など）
int32_t		rkrots[5 * 15 * 2];		// 回転ルール

int32_t		rkac[5 * 15 * 2];		// ACメダル #LITE30.2
int32_t		rkst[5 * 15 * 2];		// STメダル #LITE30.2
int32_t		rksk[5 * 15 * 2];		// SKメダル #LITE30.2
int32_t		rkco[5 * 15 * 2];		// COメダル #LITE30.2
int32_t		rkre[5 * 15 * 2];		// REメダル #LITE30.2
int32_t		modecolor[15] = {4,1,1,1,1,5,5,2,2,3,3,0,0,2,9};		// モード名表示色

// ランキング初期化
void RankingInit2() {
	int32_t i;

	for(i = 0; i < (5 * 15 * 2); i++) {
		StrCpy(rkname[i], "NOP");
		rkdata[i] = 0;
		rktime2[i] = 1200*60;
		rkclear[i] = 0;
		rkother[i] = 0;
		rkrots[i] = 0;
		rkac[i] = 0;
		rkst[i] = 0;
		rksk[i] = 0;
		rkco[i] = 0;
		rkre[i] = 0;
	}
}

// 何位に入るかチェック（ランク外なら-1）
int32_t RankingCheck2(int32_t rmode, int32_t rrots, int32_t rdata, int32_t rtime, int32_t rclear) {
	int32_t i, j, rank;

	if(playback || demo) return -1;
//	if(rmode >= 8) return -1;

	rank = -1;

	ARSSRSch(rrots);//ARSかSRSかを決める関数

	for(i = 0; i < 5; i++) {
		j = RankingGet(rmode, rankingrule);

		if(
			(rdata > rkdata[j + i]) ||  //段位花火到達ステージライン
			((rdata == rkdata[j + i]) && ((rclear > rkclear[j + i])//ロールクリア＞失敗＞ロール以前
			|| ((rclear == rkclear[j + i]) && (rtime < rktime2[j + i]))))
		  )
		{//if(rmode <= 3) return ((rmode * 60) + (rtype * 30) + (rrots * 3));
			rank = i;
			break;
		}
	}

	return rank;
}

// ランキングに登録
void RankingRegist2(int32_t rmode, int32_t rrots, int32_t rdata, int32_t rtime, int32_t rclear,
	int32_t rother, const char *rname, int32_t rank, int32_t rac, int32_t rst, int32_t rsk, int32_t rco ,int32_t rre) {

	int32_t i, j, rcolor;

	if(playback || demo) return;
	if((rank < 0) || (rank > 4)) return;

	ARSSRSch(rrots);
	j = RankingGet(rmode, rankingrule);

	// ランキングをずらす
	for(i = 4; i > rank ; i--) {
		StrCpy(rkname[j + i], rkname[j + i - 1]);
		rkdata[j + i] = rkdata[j + i - 1];
		rktime2[j + i] = rktime2[j + i - 1];
		rkclear[j + i] = rkclear[j + i - 1];
		rkother[j + i] = rkother[j + i - 1];
		rkrots[j + i] = rkrots[j + i - 1];
	}

	// 新しいデータを登録
	StrCpy(rkname[j + rank], rname);
	rkdata[j + rank] = rdata;
	rktime2[j + rank] = rtime;
	rkclear[j + rank] = rclear;
	rkother[j + rank] = rother;
	rkrots[j + rank] = rrots;

	// メダルを登録 #LITE30.2
	rkac[j + rank] = rac;
	rkst[j + rank] = rst;
	rksk[j + rank] = rsk;
	rkco[j + rank] = rco;
	rkre[j + rank] = rre;
}

// ランキング表示
void RankingProc_2(int32_t cat) {
	count = 0;
	flag = 0;
//	category = cat;
		if(cat==0){	//ビギナー
		category = 0;
		}else if(cat==1){	//マスター
			if(enable_grade[0]==1){	//1
			category = 1;
			}else if(enable_grade[0]==2){	//2
			category = 2;
			}else if(enable_grade[0]==3){	//2+
			category = 3;
			}else if(enable_grade[0]==4){	//3
			category = 4;
			}
		}else if(cat==2){
			if(enable_grade[0]<=3){
			category = 5;
			}else if(enable_grade[0]==4){	//3
			category = 6;
			}
		}else if(cat==3){
			if(devil_minus[0]==0){
			category = 7;
			}else if(devil_minus[0]==1){
			category = 8;
			}
		}else if(cat==6){	//ともよ
			if(tomoyo_opt[0]==0){	//Ti
			category = 9;
			}else if(tomoyo_opt[0]==1){	//EH
			category = 10;
			}
		}else if(cat==7){
			if(anothermode[0]==0){
			category = 11;
			}else if(anothermode[0]==1){
			category = 12;
			}else if(anothermode[0]==2){
			category = 13;
			}
		}


	while(!flag) {
		count++;

		KeyInput();
		RankingView2();

		// AかBで戻る
		if( (count > 448) || getPushState(0,4) || getPushState(0,5) || getPushState(1,4) || getPushState(1,5) || IsPushEscKey() ) {
			flag = 1;
		}

		spriteTime();
	}
}

void RankingProc2_2() {
	category = 0;

	while(1) {
		KeyInput();

		RankingView2();
		// ←
		if( getPushState(0,2) ) {
			PlayWave( 5 );
			category--;//ゲームモード
			if(category < 0) category = 14;
		}

		// →
		if( getPushState(0,3) ) {
			PlayWave( 5 );
			category++;
			if(category > 14) category = 0;
		}

		// ↑
		if( getPushState(0,0) ) {
			PlayWave( 5 );
			rankingrule = !(rankingrule);//回転ルール
		}

		// ↓
		if( getPushState(0,1) ) {
			PlayWave( 5 );
			rankingrule  = !(rankingrule);
		}

		// AかBで戻る
		if( getPushState(0,4) || getPushState(0,5) || getPushState(1,4) || getPushState(1,5) || IsPushEscKey() ) {
			return;
		}

		spriteTime();
	}
}

void RankingView2() {//5位まで
	int32_t i, j, k, color;

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

	// ルール名表示
	getRuleNameEx(rankingrule, 0);
	StrCat(string[0], " TYPE RULE - ");

	// モード名表示
	getModeNameEx(category, 1);
	StrCat(string[1], " MODE");

	StrCat(string[0], string[1]);
	printFont(1, 1, string[0], modecolor[category]);

	// ランキング表示
	if(category == 0) {//beginner
		printFont(1, 3, "HANABI RANKING", 1);
	} else if(category == 9) {//ともよTI
		printFont(1, 3, "TI COURSE RANKING", 1);
	} else if(category == 10) {//ともよEH
		printFont(1, 3, "E-HEART COURSE RANKING", 1);
	} else if(category == 14) {//エンドレス
		printFont(1, 3, "ENDLESS RANKING", 1);
	} else if((category >= 1) && (category <= 6)) {//マスター・20G
		printFont(1, 3, "GRANDMASTER RANKING", 1);
	} else {
		printFont(1,  3, "TIME TRIAL RANKING", 1);
	}

	if(category == 0)//ビギ
		printFont(1, 5  , "RANK  NAME  HANABI LEVEL  TIME", 2);
	else if(category == 9)//ともよTI
		printFont(1, 5  , "RANK  NAME  STAGE  CLEAR  TIME", 2);
	else if(category == 10)//ともよEH
		printFont(1, 5  , "RANK  NAME  STAGE         TIME", 2);
	else if((category == 11)||(category == 12)||(category == 13))//ace系
		printFont(1, 5  , "RANK  NAME  LINES  LEVEL  TIME", 2);
	else if(category == 14)//エンドレス
		printFont(1, 5  , "RANK  NAME  LEVEL  MODE   TIME", 2);
	else
		printFont(1, 5  , "RANK  NAME  CLASS  LEVEL  TIME", 2);
	if(rankingrule == 0){
		printFont(16, 27  , "HEBORIS TI-ARS", 0);//ARS
		printFont(16, 28  , "ACE-ARS ACE-ARS2 D.R.S", 0);
	}else{
		printFont(16, 27  , "TI-WORLD ACE-SRS", 0);//SRS
		printFont(16, 28  , "DS-WORLD SRS-X", 0);
	}
		for(i = 0; i < 5; i++) {
			j = RankingGet(category, rankingrule);
			// 順位
			if(i == 0) {sprintf(string[0], "1ST");
			printFont(1, 6 + (i * 4)  , string[0], 2);
			}
			if(i == 1) {sprintf(string[0], "2ND");
			printFont(1, 6 + (i * 4)  , string[0], 9);
			}
			if(i == 2) {sprintf(string[0], "3RD");
			printFont(1, 6 + (i * 4)  , string[0], 4);
			}
			if(i == 3) sprintf(string[0], "4TH");
			if(i == 4) sprintf(string[0], "5TH");
			printFont(1, 6 + (i * 4)  , string[0], 0);

			// 名前
			printFont(7, 6 + (i * 4)  , rkname[j + i], digitc[rkrots[j + i]]);

			if(category == 0) {
				// スコア
				sprintf(string[0], "%d", rkdata[j + i]);
				printFont(13, 6 + (i * 4)  , string[0],digitc[rkrots[j + i]]);

				// レベル
				sprintf(string[0], "%d", rkother[j + i]);
        			printFont(20, 6 + (i * 4)  , string[0], digitc[rkrots[j + i]]);
	         } else if( (category >= 1) && (category <= 8) ) {
				// 段位
               			sprintf(string[0], "%d", rkother[j + i]);
				if((category == 7)||(category == 8)){//devil
					printFont(13, 6 + (i * 4) , dgname[rkother[j + i]], digitc[rkrots[j + i]]);
					sprintf(string[0], "%d", rkdata[j + i]);
				} else if( ((category == 1)||(category == 3))&&(rkdata[j + i] == 18))
					printFont(13, 6 + (i * 4)  , "GM", digitc[rkrots[j + i]]);
				else
					printFont(13, 6 + (i * 4)  , gname2[rkdata[j + i]], digitc[rkrots[j + i]]);

            // レベル
            printFont(20, 6 + (i * 4)  , string[0], digitc[rkrots[j + i]]);
         } else if((category == 9) || (category == 10)) {
            // ステージ

				if(rkclear[j + i] == 2)
					sprintf(string[0], "ALL");
				else{
				if(rkdata[j + i] <= 19)
					sprintf(string[0], "%d", rkdata[j + i] + 1);
				else if((rkdata[j + i] <= 26)&&(rkdata[j + i] >= 20))
					sprintf(string[0], "EX%d", rkdata[j + i] - 19);
				else if(rkdata[j + i] >=27 )
					sprintf(string[0], "%d", rkdata[j + i] - 26);
				}
				printFont(13, 6 + (i * 4)  , string[0], digitc[rkrots[j + i]]);

				// クリア率
				if(category == 9){
				sprintf(string[0], "%d%%", rkother[j + i]);
				printFont(20, 6 + (i * 4)  , string[0], digitc[rkrots[j + i]]);
				}
			} else if((category == 11)||(category == 12)||(category == 13)){//ace系
				// ライン
				sprintf(string[0], "%d", rkdata[j + i]);
				printFont(13, 6 + (i * 4)  , string[0], digitc[rkrots[j + i]]);

				// レベル
				sprintf(string[0], "%d", rkother[j + i] + 1);
				printFont(20, 6 + (i * 4)  , string[0], digitc[rkrots[j + i]]);
			}else {
			// レベル
				sprintf(string[0], "%d", rkdata[j + i]);
				printFont(13, 6 + (i * 4)  , string[0], digitc[rkrots[j + i]]);

				if(rkother[j + i]==0)sprintf(string[0], "BEGINN");
				if(rkother[j + i]==1)sprintf(string[0], "MASTER");
				if(rkother[j + i]==2)sprintf(string[0], "20G");
				if(rkother[j + i]==3)sprintf(string[0], "DEVIL");
				printFont(20, 6 + (i * 4)  , string[0], digitc[rkrots[j + i]]);
			}

			// タイム
			getTime(rktime2[j + i]);
			printFont(27, 6 + (i * 4), string[0], digitc[rkrots[j + i]]);

			// ライン
			if(rkclear[j + i] == 1) {
			//printFont(20, 8+(i*4)  , "GREENLINE", 4);
			ExBltRect(77, 110, (7 + (i * 4)  ) *8,  0, 2, 170, 2);
			}else if(rkclear[j + i] == 2) {
			//printFont(20, 8+(i*4)  , "YELLOWLINE", 7);
			ExBltRect(77, 110, (7 + (i * 4)  ) *8,  0, 0, 170, 2);
			}

			// 回転
				if(rkrots[j + i]==0)sprintf(string[0], "HEBORIS");
				if(rkrots[j + i]==1)sprintf(string[0], "TI-ARS");
				if(rkrots[j + i]==2)sprintf(string[0], "TI-WORLD");
				if(rkrots[j + i]==3)sprintf(string[0], "ACE-SRS");
				if(rkrots[j + i]==4)sprintf(string[0], "ACE-ARS");
				if(rkrots[j + i]==5)sprintf(string[0], "ACE-ARS2");
				if(rkrots[j + i]==6)sprintf(string[0], "DS-WORLD");
				if(rkrots[j + i]==7)sprintf(string[0], "SRS-X");
				if(rkrots[j + i]==8)sprintf(string[0], "D.R.S");
				printFont(27, 8 + (i * 4)  , string[0], fontc[rkrots[j + i]]);

			// メダル
			if((category <= 8)||(category == 14)) {
				if(rkac[j + i]) ExBltRect(0, 20, (7 + (i * 4)  ) * 8,  0, 52 - rkac[j + i] * 13, 15, 12);
					else ExBltRect(0, 20, (7 + (i * 4)  ) * 8,  0, 52, 15, 12);
				if(rkst[j + i]) ExBltRect(0, 40, (7 + (i * 4)  ) * 8, 16, 52 - rkst[j + i] * 13, 15, 12);
					else ExBltRect(0, 40, (7 + (i * 4)  ) * 8, 16, 52 , 15, 12);
				if(rksk[j + i]) ExBltRect(0, 60, (7 + (i * 4)  ) * 8, 32, 52 - rksk[j + i] * 13, 15, 12);
					else ExBltRect(0, 60, (7 + (i * 4)  ) * 8, 32, 52, 15, 12);
				if(rkco[j + i]) ExBltRect(0, 80, (7 + (i * 4)  ) * 8, 80, 52 - rkco[j + i] * 13, 15, 12);
				 	else ExBltRect(0, 80, (7 + (i * 4)  ) * 8, 80, 52, 15, 12);
				if(rkre[j + i]) ExBltRect(0, 100,(7 + (i * 4)  ) * 8, 48, 52 - rkre[j + i] * 13, 15, 12);
					else ExBltRect(0, 100,(7 + (i * 4)  ) * 8, 48, 52, 15, 12);
			}
		}
		printFont(1, 27  , "k n ROT TYPE", 0);
		printFont(1, 28  , "< > GAME MODE", 0);

}
// ランキングを保存
void RankingSave2() {
	int32_t i;

	FillMemory(&saveBuf, 5000 * 4, 0);

	// ヘッダ
	saveBuf[0] = 0x4F424549;
	saveBuf[1] = 0x20534953;
	saveBuf[2] = 0x48474949;
	saveBuf[3] = 0x34764354;

	// ランキングデータ
	for(i = 0; i < (5 * 15 * 2); i++) {
		// 名前
		saveBuf[4 + i] = (int32_t)(
			((uint32_t)rkname[i][0] <<  0) |
			((uint32_t)rkname[i][1] <<  8) |
			((uint32_t)rkname[i][2] << 16) |
			((uint32_t)rkname[i][3] << 24)
		);

		// 段位
		saveBuf[4 + i + (5 * 15 * 2) * 1] = rkdata[i];//2

		// タイム
		saveBuf[4 + i + (5 * 15 * 2) * 2] = rktime2[i];//3

		// ロールクリア
		saveBuf[4 + i + (5 * 15 * 2) * 3] = rkclear[i];//4

		// その他
		saveBuf[4 + i + (5 * 15 * 2) * 4] = rkother[i];//5

		// メダル #LITE30.2
		saveBuf[4 + i + (5 * 15 * 2) * 5] = rkac[i];//6
		saveBuf[4 + i + (5 * 15 * 2) * 6] = rkst[i];//7
		saveBuf[4 + i + (5 * 15 * 2) * 7] = rksk[i];//8
		saveBuf[4 + i + (5 * 15 * 2) * 8] = rkco[i];//9
		saveBuf[4 + i + (5 * 15 * 2) * 9] = rkre[i];//10
		saveBuf[4 + i + (5 * 15 * 2) * 10] = rkrots[i];//11
	}

	SaveFile("config/data/RANKING2.SAV", &saveBuf, 1654 * 4);//1654=4+(5*15*2*11)
}

// ランキングを読み込み
int32_t RankingLoad2() {
	int32_t i;

	// ヘッダだけ読み込み
	FillMemory(&saveBuf, 5000 * 4, 0);
	LoadFile("config/data/RANKING2.SAV", &saveBuf, 16);

	// バージョン違いなら初期化
	if(saveBuf[0] != 0x4F424549) return 1;
	if(saveBuf[1] != 0x20534953) return 1;
	if(saveBuf[2] != 0x48474949) return 1;
	if(saveBuf[3] != 0x34764354) return 1;

	// 全部読み込み
	LoadFile("config/data/RANKING2.SAV", &saveBuf, 1654 * 4);

	for(i = 0; i < (5 * 15 * 2); i++) {
		// 名前
		rkname[i][0] = (char)((saveBuf[4 + i] >>  0) & 0xFF);
		rkname[i][1] = (char)((saveBuf[4 + i] >>  8) & 0xFF);
		rkname[i][2] = (char)((saveBuf[4 + i] >> 16) & 0xFF);
		rkname[i][3] = (char)((saveBuf[4 + i] >> 24) & 0xFF);

		// 段位
		rkdata[i]  = saveBuf[4 + i + (5 * 15 * 2) * 1];

		// タイム
		rktime2[i]  = saveBuf[4 + i + (5 * 15 * 2) * 2];

		// ロールクリア
		rkclear[i] = saveBuf[4 + i + (5 * 15 * 2) * 3];

		// その他
		rkother[i] = saveBuf[4 + i + (5 * 15 * 2) * 4];

		// メダル #LITE30.2
		rkac[i] = saveBuf[4 + i + (5 * 15 * 2) * 5];
		rkst[i] = saveBuf[4 + i + (5 * 15 * 2) * 6];
		rksk[i] = saveBuf[4 + i + (5 * 15 * 2) * 7];
		rkco[i] = saveBuf[4 + i + (5 * 15 * 2) * 8];
		rkre[i] = saveBuf[4 + i + (5 * 15 * 2) * 9];
		rkrots[i] = saveBuf[4 + i + (5 * 15 * 2) * 10];
	}

	return 0;
}

// ランキングを読み書きする時に使用
int32_t RankingGet(int32_t rmode, int32_t rrots) {
	// 普通のモード
	return ((rmode * 10) + (rrots * 5));
}
// 外部からランキングのデータを使いたい時に使用
int32_t RankingGet2(int32_t rmode,int32_t rtype, int32_t rrots,int32_t player) {
	int32_t cat[2];
	if((rrots==0)||(rrots==1)||(rrots==4)||(rrots==5)){
		rankingrule = 0 ;//ARS
	}else{
		rankingrule = 1;//SRS
	}

	if((rmode<=3)&&(rtype)){
		cat[player] = 14;
		}else if(rmode==0){
		cat[player] = 0;
		}else if(rmode==1){
			if(enable_grade[player]==1){
			cat[player] = 1;
			}else if(enable_grade[player]==2){
			cat[player] = 2;
			}else if(enable_grade[player]==3){
			cat[player] = 3;
			}else if(enable_grade[player]==4){
			cat[player] = 4;
			}
		}else if(rmode==2){
			if(enable_grade[player]<=3){
			cat[player] = 5;
			}else if(enable_grade[player]==4){
			cat[player] = 6;
			}
		}else if(rmode==3){
			if(devil_minus[player]==0){
			cat[player] = 7;
			}else if(devil_minus[player]==1){
			cat[player] = 8;
			}
		}else if(rmode==6){
			if(tomoyo_opt[player]==0){
			cat[player] = 9;
			}else if(tomoyo_opt[player]==1){
			cat[player] = 10;
			}
		}else if(rmode==7){
			if(anothermode[player]==0){
			cat[player] = 11;
			}else if(anothermode[player]==1){
			cat[player] = 12;
			}else if(anothermode[player]==2){
			cat[player] = 13;
			}
	}
	return ((cat[player] * 10) + (rankingrule * 5));
}

void getModeNameEx( int32_t mode, int32_t number ) {
	if(mode == 0)
		sprintf(string[number], "BEGINNER");
	else if(mode == 1)
		sprintf(string[number], "MASTER GRADE 1");
	else if(mode == 2)
		sprintf(string[number], "MASTER GRADE 2");
	else if(mode == 3)
		sprintf(string[number], "MASTER GRADE 3");
	else if(mode == 4)
		sprintf(string[number], "MASTER GRADE 4");
	else if(mode == 5)
		sprintf(string[number], "20G");
	else if(mode == 6)
		sprintf(string[number], "20G GRADE 4");
	else if(mode == 7)
		sprintf(string[number], "DEVIL");
	else if(mode == 8)
		sprintf(string[number], "DEVIL-");
	else if((mode == 9)||(mode == 10))
		sprintf(string[number], "TOMOYO");
	else if(mode == 11)
		sprintf(string[number], "ACE(NORMAL)");
	else if(mode == 12)
		sprintf(string[number], "ACE(ANOTHER)");
	else if(mode == 13)
		sprintf(string[number], "ACE(HELL)");
	else if(mode == 14)
		sprintf(string[number], "ENDLESS");
}

void getRuleNameEx( int32_t rule, int32_t number ) {

		if(rule == 0)
			sprintf(string[number], "ARS");
		else if(rule == 1)
			sprintf(string[number], "SRS");
}
//besttime(ACEのみ)
void viewbesttime(int32_t player){
	int32_t	tmp,color;

	if(Isbesttime==0)return;

	color = (count % 4 / 2) * (sp[player] >= 1200) * digitc[rots[player]];
	/* ベストタイム */
	if( (gameMode[player] == 7) && ((!maxPlay) || (status[1 - player] == 0)) ) {
		printFont(26+2*((hnext[player] >= 4) && (!player)) + 7 * player - 12 * maxPlay, 10, "BEST TIME", fontc[rots[player]]);

		tmp = RankingGet2(gameMode[player], anothermode[player], rots[player],player);
		if( ((rkdata[tmp] >= 150)&&(anothermode[player] <= 1)) || ((rkdata[tmp] >= 200)&&(anothermode[player] == 2)) )
		getTime(rktime2[tmp]);
		else getTime(0);
		printFont(26+2*((hnext[player] >= 4) && (!player)) + 7 * player - 12 * maxPlay, 11, string[0], color);
	}
}
void viewbesttimes(int32_t player){
	int32_t	tmp,color;

	if(Isbesttime==0)return;
	color = (count % 4 / 2) * (sp[player] >= 1200) * digitc[rots[player]];
	/* ベストタイム */
	if( (gameMode[player] == 7) && ((!maxPlay) || (status[1 - player] == 0)) ) {
			//bestの文字
			ExBltRect(3, 208+20*((hnext[player] >= 4) && (!player)) + 70 * player - 96 * maxPlay, 78, 251, 91, 21, 7);
			//timeの文字
			ExBltRect(3, 230+20*((hnext[player] >= 4) && (!player)) + 70 * player - 96 * maxPlay, 78, 180, 119, 19, 7);
			tmp = RankingGet2(gameMode[player], anothermode[player], rots[player],player);
			if( ((rkdata[tmp] >= 150)&&(anothermode[player] <= 1)) || ((rkdata[tmp] >= 200)&&(anothermode[player] == 2)) )
			getTime(rktime2[tmp]);
			else getTime(0);
			printSMALLFont((26 + 8 * player - 12 * maxPlay)*8+20*((hnext[player] >= 4) && (!player)), 86, string[0], color);
	}
}

int32_t ARSSRSch(int32_t rots){
	if((rots==0)||(rots==1)||(rots==4)||(rots==5)||(rots==8)){
	rankingrule = 0 ;//ARS
	}else{
	rankingrule = 1;//SRS
	}
	return rankingrule;
}
