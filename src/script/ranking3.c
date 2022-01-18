//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ランキング関連
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲

#include "../script/include.h"

// ランキング用グローバル変数
// (3名×2) × 10モード(二つ予備) × 2回転ルールタイプ
// 一ページに2モード3名ずつ
//ビギ、マス(1、2)、マス(3、4)、20G、デビ、トモ(Ti、EH)、トモ（）エス（NO、ANO）、エス(HE)、エンドレス
bool	allocked3 = false;
char	rkname3[ 6*14*2][4];			// 名前
int32_t		rkdata3[ 6*14*2];		// 花火／段位／ステージ／ライン
int32_t		rktime3[ 6*14*2];		// タイム
int32_t		rkclear3[ 6*14*2];		// 0=途中で窒息 1=ロール失敗 2=ロールクリア
int32_t		rkother3[ 6*14*2];		// その他（レベルやクリア率など）
int32_t		rkrots3[ 6*14*2];		// 回転ルール

int32_t		rkac3[ 6*14*2];		// ACメダル #LITE30.2
int32_t		rkst3[ 6*14*2];		// STメダル #LITE30.2
int32_t		rksk3[ 6*14*2];		// SKメダル #LITE30.2
int32_t		rkco3[ 6*14*2];		// COメダル #LITE30.2
int32_t		rkre3[ 6*14*2];		// REメダル #LITE30.2
int32_t		modecolor3[15] = {4,1,1,5,2,3,3,0,0,7,9,0};		// モード名表示色
int32_t		rkpage;

// ランキング初期化
void RankingInit3() {
	int32_t i;

	for(i = 0; i < ( 6*14*2); i++) {
		StrCpy(rkname3[i], "NOP");
		rkdata3[i] = 0;
		rktime3[i] = 1200*60;
		rkclear3[i] = 0;
		rkother3[i] = 0;
		rkrots3[i] = 0;
		rkac3[i] = 0;
		rkst3[i] = 0;
		rksk3[i] = 0;
		rkco3[i] = 0;
		rkre3[i] = 0;
	}
}

// 何位に入るかチェック（ランク外なら-1）
int32_t RankingCheck3(int32_t rmode, int32_t rex,int32_t rrots, int32_t rdata, int32_t rtime, int32_t rclear) {
	int32_t i, j, rank;

	if(playback || demo) return -1;

	rank = -1;

	for(i = 0; i < 3; i++) {
		j = RankingGet3(rmode,ARSSRSch(rrots), rex);

		if( (rdata > rkdata3[j + i]) ||  //段位花火到達ステージライン
		((rdata == rkdata3[j + i]) && (rclear > rkclear3[j + i]))||//ロールクリア＞失敗＞ロール以前
		((rdata == rkdata3[j + i]) && (rclear == rkclear3[j + i]) && (rtime < rktime3[j + i])) )
		{
			rank = i;
			break;
		}
	}
	return rank;
}

// ランキングに登録
void RankingRegist3(int32_t rmode, int32_t rex,int32_t rrots, int32_t rdata, int32_t rtime, int32_t rclear,
	int32_t rother, char *rname, int32_t rank, int32_t rac, int32_t rst, int32_t rsk, int32_t rco ,int32_t rre) {

	int32_t i, j, rcolor;

	if(playback || demo) return;
	if((rank < 0) || (rank > 2)) return;

	j = RankingGet3(rmode,ARSSRSch(rrots), rex );

	// ランキングをずらす
	for(i = 2+((rmode==9)*3); i > rank ; i--) {
		StrCpy(rkname3[j + i], rkname3[j + i - 1]);
		rkdata3[j + i] = rkdata3[j + i - 1];
		rktime3[j + i] = rktime3[j + i - 1];
		rkclear3[j + i] = rkclear3[j + i - 1];
		rkother3[j + i] = rkother3[j + i - 1];
		rkrots3[j + i] = rkrots3[j + i - 1];
		rkac3[j + i] = rkac3[j + i - 1];
		rkst3[j + i] = rkst3[j + i - 1];
		rksk3[j + i] = rksk3[j + i - 1];
		rkco3[j + i] = rkco3[j + i - 1];
		rkre3[j + i] = rkre3[j + i - 1];
	}

	// 順位のデータを元に新しいデータを登録
	StrCpy(rkname3[j + rank], rname);
	rkdata3[j + rank] = rdata;
	rktime3[j + rank] = rtime;
	rkclear3[j + rank] = rclear;
	rkother3[j + rank] = rother;
	rkrots3[j + rank] = rrots;

	// メダルを登録 #LITE30.2
	rkac3[j + rank] = rac;
	rkst3[j + rank] = rst;
	rksk3[j + rank] = rsk;
	rkco3[j + rank] = rco;
	rkre3[j + rank] = rre;
}

// ランキング表示(デモ画面、プレイ後に表示)
// 2pagesはランキング画面が2ページ目
void RankingProc_3(int32_t cat,int32_t pages2) {
	count = 0;
	flag = 0;
	if(cat==0){	//ビギナー
		rkpage = 0;
	}else if(cat==1){//マスター
		if(pages2==0){	//1,2
			rkpage = 1;
		}else if(pages2==1){	//3,4
			rkpage = 2;
		}
	}else if(cat==2){//20G
		rkpage = 3;
	}else if(cat==3){//DE
		rkpage = 4;
	}else if(cat==6){//TO
		if(pages2==0){	//TiEH
			rkpage = 5;
		}else if(pages2==1){//FP
			rkpage = 6;
		}
	}else if(cat==7){//ACE
		if(pages2==0){
			rkpage = 7;
		}else if(pages2==1){
			rkpage = 8;
		}
	}else if(cat==9){//STD
		if(pages2==0){
			rkpage = 10;
		}else if(pages2==1){
			rkpage = 11;
		}
	}else if(cat==10){//ori
			rkpage = 9;
	}

	while(!flag) {
//		count++;

		KeyInput();

		RankingView3();//3位まで表示
		// AかBで戻る
		if( (count > 448) || getPushState(0,4) || getPushState(0,5) || getPushState(1,4) || getPushState(1,5) || IsPushEscKey() ) {
			flag = 1;
		}

		spriteTime();
	}
}
//ランキング表示（自分で項目選択して表示）
void RankingProc2_3() {
	rkpage = 0;

	while(1) {
		KeyInput();

		RankingView3();
		// ←
		if( getPushState(0,2) ) {
			PlaySE( 5 );
			rkpage--;//ゲームモード
			if(rkpage < 0) rkpage = 11;
		}

		// →
		if( getPushState(0,3) ) {
			PlaySE( 5 );
			rkpage++;
			if(rkpage > 11) rkpage = 0;
		}

		// ↑
		if( getPushState(0,0) ) {
			PlaySE( 5 );
			rankingrule = !(rankingrule);//回転ルール
		}

		// ↓
		if( getPushState(0,1) ) {
			PlaySE( 5 );
			rankingrule  = !(rankingrule);
		}

		// AかBで戻る
		if( getPushState(0,4) || getPushState(0,5) || getPushState(1,4) || getPushState(1,5) || IsPushEscKey() ) {
			return;
		}

		spriteTime();
	}
}

void RankingView3() {//3位まで
	int32_t i, j, k, l, color;

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
	ExBltRect(77, 0, 24,  count % 320, 20, 320 - (count % 320), 8);
	ExBltRect(77, 320 - (count % 320), 24,  0, 20, count % 320, 8);

	ExBltRect(77, 0, 128,  count % 320, 20, 320 - (count % 320), 8);
	ExBltRect(77, 320 - (count % 320), 128,  0, 20, count % 320, 8);

	// ルール名表示
	getRuleNameEx3(rankingrule, 0);
	StrCat(string[0], " TYPE RULE - ");

	// モード名表示
	getModeNameEx3(rkpage, 1);
	StrCat(string[1], " MODE");

	StrCat(string[0], string[1]);
	printFont(1, 1, string[0], modecolor3[rkpage]);

	// ランキング表示
	if(rkpage == 0) {//beginner
		printFont(1, 3, "HANABI RANKING", 1);
		printFont(1, 16,"SCORE RANKING", 1);
	} else if(rkpage == 1) {//マスター
		printFont(1, 3, "GRANDMASTER RANKING GRADE1 ", 1);
		printFont(1, 16,"GRANDMASTER RANKING GRADE2 ", 1);
	} else if(rkpage == 2) {//マスター
		printFont(1, 3, "GRANDMASTER RANKING GRADE3 ", 1);
		printFont(1, 16,"GRANDMASTER RANKING GRADE4 ", 1);
	} else if(rkpage == 3) {//20G
		printFont(1, 3, "GRANDMASTER RANKING GRADE1-GRADE3", 5);
		printFont(1, 16,"GRANDMASTER RANKING GRADE4", 5);
	}else if(rkpage == 4) {//d
		printFont(1, 3, "DOOM RANKING", 2);
		printFont(1, 16,"MINUS RANKING", 2);
	} else if(rkpage == 5) {//ともよTI EH
		printFont(1, 3, "TI COURSE RANKING", 3);
		printFont(1, 16,"E-HEART COURSE RANKING", 3);
	} else if(rkpage == 6) {//FP
		printFont(1, 3, "TGT RANKING", 3);
		printFont(1, 16,"F-POINT RANKING", 3);
	} else if(rkpage == 7) {//ace
		printFont(1, 3, "NORMAL RANKING", 0);
		printFont(1, 16,"ANOTHER RANKING", 0);
	} else if(rkpage == 8) {//ace2
		printFont(1, 3, "HELL RANKING", 2);
		printFont(1, 16,"OLD STYLE RANKING", 9);
	} else if(rkpage == 9) {//ORI
		printFont(1, 3, "ORIGINAL RANKING", 9);
	} else if(rkpage == 10) {//STANDARD
		printFont(1, 3, "40LINES RANKING", 9);
		printFont(1, 16,"ULTRA RANKING", 9);
	}else if(rkpage == 11) {//STANDARD
		printFont(1, 3, "SQUARE RANKING", 9);
		printFont(1, 16,"MARATHON RANKING", 9);

	}

	if(rankingrule == 0){
		printFont(0, 29  , "HEBORIS TI-ARS ACE-ARS ACE-ARS2 D.R.S", 0);//ARS
	}else{
		printFont(0, 29  , "TI-WORLD ACE-SRS DS-WORLD SRS-X", 0);//SRS
	}
	for(l = 0; l < 2 ; l++){//l==0上の3段 l==1下の3段
		// 順位
		if(rkpage!=9){//オリ以外
			sprintf(string[0], "1ST");
			printFont(1, 5 + (l * 13)  , string[0], 2);
			sprintf(string[0], "2ND");
			printFont(1, 9 + (l * 13), string[0], 9);
			sprintf(string[0], "3RD");
			printFont(1, 13+ (l * 13) , string[0], 4);
		}else if(rkpage==9){
			sprintf(string[0], "1ST");
			printFont(1, 5  , string[0], 2);
			sprintf(string[0], "2ND");
			printFont(1, 9, string[0], 9);
			sprintf(string[0], "3RD");
			printFont(1, 13 , string[0], 4);
			sprintf(string[0], "4TH");
			printFont(1, 18   , string[0], 0);
			sprintf(string[0], "5TH");
			printFont(1, 22, string[0], 0);
			sprintf(string[0], "6TH");
			printFont(1, 26 , string[0], 0);
		}
			ExBltRect(85, 8, 32 + ( l * 13) * 8, 35, 7*3, 21, 7);//RANKの文字
			ExBltRect(85, 40 , 32 + (l * 13) * 8, 35, 0, 21, 7);//nameの文字,文字を置く座標,切り取る座標,切り取るサイズ
			ExBltRect(85, 190, 32 + (l * 13) * 8, 35, 7*12, 21, 7);//TIMEの文字
			ExBltRect(85, 260, 32 + ( l * 13) * 8, 35, 7*6, 21, 7);//ROTSの文字

		if(rkpage==0){//ビギナー
			if(l){//SCORE(sita)
			ExBltRect(85, 78 , 32 + (l * 13) * 8, 35, 7*7, 25, 7);//SCOREの文字
			ExBltRect(85, 128, 32 + (l * 13) * 8, 0, 7*9, 26, 7);//LEVELの文字
			}else{//HANABI(ue)
			ExBltRect(85, 80 , 32 + (l * 13) * 8, 35, 7*11, 29, 7);//HANABIの文字
			ExBltRect(85, 128, 32 + (l * 13) * 8, 0, 7*9, 26, 7);//LEVELの文字
			}
		}else if(rkpage==5){//TI EH
			ExBltRect(85, 80 , 32 + (l * 13) * 8, 35, 7*10, 27, 7);//STAGEの文字
			ExBltRect(85, 128 , 32 + (l * 13) * 8, 0, 7*4, 25, 7);//CLEARの文字
		}else if(rkpage==6){//FP
			ExBltRect(85, 80 , 32 + (l * 13) * 8, 35, 7*10, 27, 7);//STAGEの文字
			ExBltRect(85, 126 , 32 + (l * 13) * 8, 35, 7*7, 28, 7);//SCOREの文字
		}else if((rkpage>=7)&&(rkpage<=8)){//ACE
			ExBltRect(85, 78 , 32 + (l * 13) * 8, 0, 7*10, 35, 7);//LINESの文字
			ExBltRect(85, 128, 32 + (l * 13) * 8, 0, 7*9, 26, 7);//LEVELの文字
		}else if(rkpage==9){//ORIJINAL
			ExBltRect(85, 80, 32 + (l * 13) * 8, 35, 7*7, 28, 7);//LEVELの文字
			ExBltRect(85, 128 , 32 + (l * 13) * 8, 0, 14*7, 21, 7);//MODEの文字
		}else if(rkpage==10){//STANDARD
			ExBltRect(85, 78 , 32 + (l * 13) * 8, 0, 7*10, 35, 7);//LINESの文字
			ExBltRect(85, 128 , 32 + (l * 13) * 8, 0, 2*7, 30, 7);//BLOCKの文字
		}else if(rkpage==11){
			if(l){//MARATHON(sita)
			ExBltRect(85, 78 , 32 + (l * 13) * 8, 0, 7*10, 35, 7);//LINESの文字
			ExBltRect(85, 128, 32 + (l * 13) * 8, 0, 7*9, 26, 7);//LEVELの文字
			}else{//SQUARE(ue)
			ExBltRect(85, 78 , 32 + (l * 13) * 8, 35, 7*9, 35, 7);//SQUAREの文字
			ExBltRect(85, 128, 32 + (l * 13) * 8, 0, 7*9, 26, 7);//LEVELの文字
			}
		}else{
			ExBltRect(85, 78 , 32 + (l * 13) * 8, 0, 3*7, 27, 7);//CLASSの文字
			ExBltRect(85, 128, 32 + (l * 13) * 8, 0, 7*9, 26, 7);//LEVELの文字
		}

		for(i = 0; i < 3; i++) {
			j = RankingGet3(rkpage,rankingrule,l);

			// 名前
			printFont(5, 5 + (i * 4) + (l * 13), rkname3[j + i], digitc[rkrots3[j + i]]);

			if(rkpage == 0) {
				// スコア
				sprintf(string[0], "%d", rkdata3[j + i]);
				printFont(10, 5 + (i * 4) + (l * 13), string[0],digitc[rkrots3[j + i]]);

				// レベル
				sprintf(string[0], "%d", rkother3[j + i]);
				printFont(16, 5 + (i * 4) + (l * 13), string[0], digitc[rkrots3[j + i]]);
			} else if( (rkpage >= 1) && (rkpage <= 4) ) {
				if(rkpage == 4){//devil
					//devilは先に段位を表示
					// 段位
					sprintf(string[0], "%d", rkother3[j + i]);
					printFont(10, 5 + (i * 4) + l * 13, dgname[rkother3[j + i]], digitc[rkrots3[j + i]]);
					sprintf(string[0], "%d", rkdata3[j + i]); //devilレベル
				}else{
					sprintf(string[0], "%d", rkdata3[j + i]); //mas 20G段位
					printFont(10, 5 + (i * 4) + (l * 13), gname2[rkdata3[j + i]], digitc[rkrots3[j + i]]);
					// 段位
					sprintf(string[0], "%d", rkother3[j + i]);
				}
				// レベル
				printFont(16, 5 + (i * 4) + (l * 13), string[0], digitc[rkrots3[j + i]]);
			}else if((rkpage == 5) || (rkpage == 6)) {
				// クリア率
				if(rkpage == 5){
					// ステージ
					if(rkclear3[j + i] == 2){
						sprintf(string[0], "ALL");
					}else{
						if(rkdata3[j + i] <= 19)
							sprintf(string[0], "%d", rkdata3[j + i] + 1);
						else if((rkdata3[j + i] <= 26)&&(rkdata3[j + i] >= 20))
							sprintf(string[0], "EX%d", rkdata3[j + i] - 19);
						else if(rkdata3[j + i] >=27 )
							sprintf(string[0], "%d", rkdata3[j + i] - 26);
					}
					printFont(10, 5 + (i * 4) + (l * 13), string[0], digitc[rkrots3[j + i]]);
					sprintf(string[0], "%d%%", rkother3[j + i]);
					printFont(16, 5 + (i * 4) + (l * 13), string[0], digitc[rkrots3[j + i]]);
				}else if(rkpage == 6){
					if(rkdata3[j+i]>100){
						sprintf(string[0], "%d", rkdata3[j + i] - 99);
						printFont(10, 5 + (i * 4) + (l * 13), string[0], digitc[rkrots3[j + i]]);
					}else{
						sprintf(string[0], "%d", rkdata3[j + i]);
						printFont(10, 5 + (i * 4) + (l * 13), string[0], digitc[rkrots3[j + i]]);
					}
					sprintf(string[0], "%d", rkother3[j + i]);
					printFont(16, 5 + (i * 4) + (l * 13), string[0], digitc[rkrots3[j + i]]);
				}
			} else if((rkpage == 7)||(rkpage == 8)){//ace系
				// ライン
				sprintf(string[0], "%d", rkdata3[j + i]);
				printFont(10, 5 + (i * 4) + (l * 13), string[0], digitc[rkrots3[j + i]]);

				// レベル
				sprintf(string[0], "%d", rkother3[j + i] + 1);
				printFont(16, 5 + (i * 4) + (l * 13), string[0], digitc[rkrots3[j + i]]);
			} else if((rkpage == 10)||(rkpage == 11)){//STANDARD系
				// ライン
				sprintf(string[0], "%d", rkdata3[j + i]);
				printFont(10, 5 + (i * 4) + (l * 13), string[0], digitc[rkrots3[j + i]]);

				// レベル
				sprintf(string[0], "%d", rkother3[j + i] + 1);
				printFont(16, 5 + (i * 4) + (l * 13), string[0], digitc[rkrots3[j + i]]);
			}else {
			// スコア
				sprintf(string[0], "%d", rkdata3[j + i]);
				printFont(10, 5 + (i * 4) + (l * 13), string[0], digitc[rkrots3[j + i]]);

				if(rkother3[j + i]==0)sprintf(string[0], "BEG");
				if(rkother3[j + i]==1)sprintf(string[0], "MAS");
				if(rkother3[j + i]==2)sprintf(string[0], "20G");
				if(rkother3[j + i]==3)sprintf(string[0], "DEV");
				printFont(16, 5 + (i * 4) + (l * 13), string[0], digitc[rkrots3[j + i]]);
			}

			// タイム
			getTime(rktime3[j + i]);
			printFont(22, 5 + (i * 4)+ l * 13, string[0], digitc[rkrots3[j + i]]);

			// ライン
			if(rkclear3[j + i] == 1) {
				ExBltFastRect(77, 70, (6 + (i * 4) + (l * 13)) *8,  0, 2, 240, 2);
			}else if(rkclear3[j + i] == 2) {
				ExBltRect(77, 70, (6 + (i * 4) + (l * 13)) *8,  0, 0, 240, 2);
			}
				// 回転
			if(rkrots3[j + i]==0)sprintf(string[0], "HEBORIS");
			if(rkrots3[j + i]==1)sprintf(string[0], "TI-ARS");
			if(rkrots3[j + i]==2)sprintf(string[0], "TI-WORLD");
			if(rkrots3[j + i]==3)sprintf(string[0], "ACE-SRS");
			if(rkrots3[j + i]==4)sprintf(string[0], "ACE-ARS");
			if(rkrots3[j + i]==5)sprintf(string[0], "ACE-ARS2");
			if(rkrots3[j + i]==6)sprintf(string[0], "DS-WORLD");
			if(rkrots3[j + i]==7)sprintf(string[0], "SRS-X");
			if(rkrots3[j + i]==8)sprintf(string[0], "D.R.S");
			printFont(31, 5 + (i * 4) + (l * 13), string[0], fontc[rkrots3[j + i]]);

			// メダル
			if((rkpage >= 1)&&(rkpage <= 4)) {
				ExBltRect(85, 16,  5 + ((6 + (i * 4) + (l * 13)) * 8), 0, 77, 27, 7);
				if(rkac3[j + i]) ExBltRect(0,  50, 3 + ((6 + (i * 4) + (l * 13)) * 8),  0, 52 - rkac3[j + i] * 13, 15, 12);
					else ExBltRect(0,  50, 3 + ((6 + (i * 4) + (l * 13)) * 8),  0, 52, 15, 12);
				if(rkst3[j + i]) ExBltRect(0,  70, 3 + ((6 + (i * 4) + (l * 13)) * 8), 16, 52 - rkst3[j + i] * 13, 15, 12);
					else ExBltRect(0,  70, 3 + ((6 + (i * 4) + (l * 13)) * 8), 16, 52 , 15, 12);
				if(rksk3[j + i]) ExBltRect(0,  90, 3 + ((6 + (i * 4) + (l * 13)) * 8), 32, 52 - rksk3[j + i] * 13, 15, 12);
					else ExBltRect(0,  90, 3 + ((6 + (i * 4) + (l * 13)) * 8), 32, 52, 15, 12);
				if(rkco3[j + i]) ExBltRect(0, 110, 3 + ((6 + (i * 4) + (l * 13)) * 8), 80, 52 - rkco3[j + i] * 13, 15, 12);
					else ExBltRect(0, 110, 3 + ((6 + (i * 4) + (l * 13)) * 8), 80, 52, 15, 12);
				if(rkre3[j + i]) ExBltRect(0, 130, 3 + ((6 + (i * 4) + (l * 13)) * 8), 48, 52 - rkre3[j + i] * 13, 15, 12);
					else ExBltRect(0, 130, 3 + ((6 + (i * 4) + (l * 13)) * 8), 48, 52, 15, 12);
			}
		}
	}
}
// ランキングを保存
void RankingSave3() {
	int32_t i;

	FillMemory(&saveBuf, 5000 * 4, 0);

	// ヘッダ
	saveBuf[0] = 0x4F424501;
	saveBuf[1] = 0x20534901;
	saveBuf[2] = 0x48474901;
	saveBuf[3] = 0x34764301;

	// ランキングデータ
	for(i = 0; i < ( 6*14*2); i++) {// 6*14*2=6*14*2
		// 名前
		saveBuf[4 + i] = (int32_t)(
			((uint32_t)rkname3[i][0] <<  0) |
			((uint32_t)rkname3[i][1] <<  8) |
			((uint32_t)rkname3[i][2] << 16) |
			((uint32_t)rkname3[i][3] << 24)
		);

		// 段位
		saveBuf[4 + i + ( 6*14*2) * 1] = rkdata3[i];//2

		// タイム
		saveBuf[4 + i + ( 6*14*2) * 2] = rktime3[i];//3

		// ロールクリア
		saveBuf[4 + i + ( 6*14*2) * 3] = rkclear3[i];//4

		// その他
		saveBuf[4 + i + ( 6*14*2) * 4] = rkother3[i];//5

		// メダル #LITE30.2
		saveBuf[4 + i + ( 6*14*2) * 5] = rkac3[i];//6
		saveBuf[4 + i + ( 6*14*2) * 6] = rkst3[i];//7
		saveBuf[4 + i + ( 6*14*2) * 7] = rksk3[i];//8
		saveBuf[4 + i + ( 6*14*2) * 8] = rkco3[i];//9
		saveBuf[4 + i + ( 6*14*2) * 9] = rkre3[i];//10
		saveBuf[4 + i + ( 6*14*2) * 10] = rkrots3[i];//11
	}

	SaveFile("config/data/RANKING3.SAV", &saveBuf, ( (6*14*2*11)+4) * 4);//
}

// ランキングを読み込み
int32_t RankingLoad3() {
	int32_t i;

	// ヘッダだけ読み込み
	FillMemory(&saveBuf, 5000 * 4, 0);
	LoadFile("config/data/RANKING3.SAV", &saveBuf, 16);
	//
	if(saveBuf[0] != 0x4F424501) return 1;
	if(saveBuf[1] != 0x20534901) return 1;
	if(saveBuf[2] != 0x48474901) return 1;
	if(saveBuf[3] != 0x34764301) return 1;

	// 全部読み込み
	LoadFile("config/data/RANKING3.SAV", &saveBuf, ( 6*14*2*11+4) * 4);

	for(i = 0; i < ( 6*14*2); i++) {
		// 名前
		rkname3[i][0] = (char)((saveBuf[4 + i] >>  0) & 0xFF);
		rkname3[i][1] = (char)((saveBuf[4 + i] >>  8) & 0xFF);
		rkname3[i][2] = (char)((saveBuf[4 + i] >> 16) & 0xFF);
		rkname3[i][3] = (char)((saveBuf[4 + i] >> 24) & 0xFF);

		// 段位
		rkdata3[i]  = saveBuf[4 + i + ( 6*14*2) * 1];

		// タイム
		rktime3[i]  = saveBuf[4 + i + ( 6*14*2) * 2];

		// ロールクリア
		rkclear3[i] = saveBuf[4 + i + ( 6*14*2) * 3];

		// その他
		rkother3[i] = saveBuf[4 + i + ( 6*14*2) * 4];

		// メダル #LITE30.2
		rkac3[i] = saveBuf[4 + i + ( 6*14*2) * 5];
		rkst3[i] = saveBuf[4 + i + ( 6*14*2) * 6];
		rksk3[i] = saveBuf[4 + i + ( 6*14*2) * 7];
		rkco3[i] = saveBuf[4 + i + ( 6*14*2) * 8];
		rkre3[i] = saveBuf[4 + i + ( 6*14*2) * 9];
		rkrots3[i] = saveBuf[4 + i + ( 6*14*2) * 10];
	}

	return 0;
}

// ランキングを書きする時に使用
int32_t RankingGet3(int32_t rmode, int32_t rrots, int32_t rex) {//5*15*2*11,3*2*14*2*11
	// 普通のモード
	return (rmode * 14) + (rrots * 6) + (rex * 3);
}

void getModeNameEx3( int32_t mode, int32_t number ) {
	if(mode == 0)
		sprintf(string[number], "BEGINNER");
	else if(mode == 1)
		sprintf(string[number], "MASTER ");
	else if(mode == 2)
		sprintf(string[number], "MASTER ");
	else if(mode == 3)
		sprintf(string[number], "20G");
	else if(mode == 4)
		sprintf(string[number], "DEVIL");
	else if(mode == 5)
		sprintf(string[number], "TOMOYO");
	else if(mode == 6)
		sprintf(string[number], "TOMOYO");
	else if(mode == 7)
		sprintf(string[number], "ACE");
	else if(mode == 8)
		sprintf(string[number], "ACE");
	else if(mode == 9)
		sprintf(string[number], "ORIGINAL");
	else if(mode == 10)
		sprintf(string[number], "SIMPLE");
	else if(mode == 11)
		sprintf(string[number], "SIMPLE");
}

void getRuleNameEx3( int32_t rule, int32_t number ) {

		if(rule == 0)
			sprintf(string[number], "ARS");
		else if(rule == 1)
			sprintf(string[number], "SRS");
}

void viewbesttimes3(int32_t player,int32_t x,int32_t y,int32_t type){
	int32_t	tmp,color[2],cat[2],ex[2];
	if(ranking_type<2){
	return;
	}
	if(Isbesttime==0)return;

	if(gameMode[player]==7){
		if(anothermode[player]==0){
			cat[player] = 7;
			ex[player] = 0;
		}else if(anothermode[player]==1){
			cat[player] = 7;
			ex[player] = 1;
		}else if(anothermode[player]==2){
			cat[player] = 8;
			ex[player] = 0;
		}else if(anothermode[player]==3){
			cat[player] = 8;
			ex[player] = 1;
		}
	}else if(gameMode[player]==9){
		if(std_opt[player]==0){
			cat[player] = 10;
			ex[player] = 0;
		}else if(std_opt[player]==1){
			cat[player] = 10;
			ex[player] = 1;
		}else if(std_opt[player]==2){
			cat[player] = 11;
			ex[player] = 1;
		}else if(std_opt[player]==3){
			cat[player] = 11;
			ex[player] = 1;
		}
	}else{
		return;
	}
	color[player] = (count % 4 / 2) * (sp[player] >= 1200) * digitc[rots[player]];
	if(1) {
		tmp = RankingGet3(cat[player],ARSSRSch(rots[player]), ex[player] );
		if( (gameMode[player]==7)&&(anothermode[player] <= 1)){
			if(rkdata3[tmp] < 150){
			type = 1;//まだクリアしてないからデータを表示
			}else{
			type = 0;
			}
		}
		if( (gameMode[player]==7)&&(anothermode[player] == 2)){
			if(rkdata3[tmp] < 200){
			type = 1;//まだクリアしてないからデータを表示
			}else{
			type = 0;
			}
		}
		if(gameMode[player]==9){
			if(ex[player]){
			type = 1;
			}else{
			type = 0;
			}
		}
		if(type==1){//タイムかデータか？
			//データ
			ExBltRect(85, x-(player*12),y, 0, 0,35, 7);//bestの文字
			ExBltRect(85, x+22-(player*16),y, 35,7*4,35, 7);//
			sprintf(string[0], "%d", rkdata3[tmp]);

			printSMALLFont(x-(player*8), y+8, string[0], color[player]);
			ExBltRect(85, x+24-(player*12),y+8, 0,7*10,35, 7);//LINES
		}else{
			//タイム
			ExBltRect(85, x,y, 0, 0,35, 7);
			ExBltRect(85, x+22,y, 35,7*12,35, 7);
			getTime(rktime3[tmp]);
			printSMALLFont(x-(player*8), y+8, string[0], color[player]);
		}

	}
}

void viewbesttime3(int32_t player,int32_t x,int32_t y,int32_t type){
	int32_t	tmp,color[2],cat[2],ex[2];
	if(ranking_type<2){
	return;
	}
	if(Isbesttime==0)return;

	if(gameMode[player]==7){
		if(anothermode[player]==0){
			cat[player] = 7;
			ex[player] = 0;
		}else if(anothermode[player]==1){
			cat[player] = 7;
			ex[player] = 1;
		}else if(anothermode[player]==2){
			cat[player] = 8;
			ex[player] = 0;
		}else if(anothermode[player]==3){
			cat[player] = 8;
			ex[player] = 1;
		}
	}else if(gameMode[player]==9){
		if(std_opt[player]==0){
			cat[player] = 10;
			ex[player] = 0;
		}else if(std_opt[player]==1){
			cat[player] = 10;
			ex[player] = 1;
		}else if(std_opt[player]==2){
			cat[player] = 11;
			ex[player] = 1;
		}else if(std_opt[player]==3){
			cat[player] = 11;
			ex[player] = 1;
		}
	}else{
		return ;
	}
	color[player] = (count % 4 / 2) * (sp[player] >= 1200) * digitc[rots[player]];
	/* ベストタイム/データ */
	if( (gameMode[player] >= 7) && ((!maxPlay)) ) {
			tmp = RankingGet3(cat[player],ARSSRSch(rots[player]), ex[player] );
			if( (gameMode[player]==7)&&(anothermode[player] <= 1)){
				if(rkdata3[tmp] < 150){
				type = 1;//まだクリアしてないからデータを表示
				}else{
				type = 0;//rkdata3[RankingGet3(cat[player],ARSSRSch(rots[player]), ex[player] )] < 200
				}
			}
			if( (gameMode[player]==7)&&(anothermode[player] == 2)){
				if(rkdata3[tmp] < 200){
				type = 1;//まだクリアしてないからデータを表示
				}else{
				type = 0;
				}
			}
			if(type==1){//タイムかデータか？
				printFont(x, y-1, "BEST SCORE", fontc[rots[player]]);
				sprintf(string[0], "%d", rkdata3[tmp]);
				printFont(x+3, y, "LINES", fontc[rots[player]]);
			}else{
				printFont(x, y-1, "BEST TIME", fontc[rots[player]]);
				getTime(rktime3[tmp]);
			}
			printFont(x, y, string[0], color[player]);

			printFont(x, y, string[0], color[player]);
	}
}
int32_t viewgrade(int32_t player){//masterモードのgrade4の段位を見る
	int32_t	tmp,color[2],cat[2],ex[2];
	if(ranking_type<2){
	return 0;
	}
	if(Isbesttime==0)return 0;
	tmp = RankingGet3(2,ARSSRSch(rots[player]), 1 );
	return rkdata3[tmp];
}
