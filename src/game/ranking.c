//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ランキング関連
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲

#include "common.h"

void RankingInit(void) {
	int32_t	i, j;

	for(i = 0; i < 8; i++) {
		for(j = 0; j < 5; j++) {
			rksc[i * 5 + j] = 5000 - 1000 * j;
			rkbl[i * 5 + j] = 25 - 5* j;
			rklv[i * 5 + j] = 50 - 10 * j;
			rktime[i * 5 + j] = 10800 - 1800 * j;
			SDL_strlcpy(string[30 + 5 * i + j], "NOP", STRING_LENGTH);
			rkfl[i * 5 + j] = 0;
		}
	}
}

void RankingConvert(void) {
	int32_t	i, j, temp;

	APP_ReadFile32("config/data/RANKING.SAV", saveBuf, 328, 0);
	for(i = 0; i < 40; i++) {
		temp = (i + 1) << 3;
		rksc[i] = saveBuf[temp + 0];
		rkbl[i] = saveBuf[temp + 1];
		rklv[i] = saveBuf[temp + 2];
		rktime[i] = saveBuf[temp + 3];
		string[30 + i][0] = (char)((saveBuf[temp + 4] >>  0) & 0xFF);
		string[30 + i][1] = (char)((saveBuf[temp + 4] >>  8) & 0xFF);
		string[30 + i][2] = (char)((saveBuf[temp + 4] >> 16) & 0xFF);
		string[30 + i][3] = (char)((saveBuf[temp + 4] >> 24) & 0xFF);
	}

	for(i = 0; i < 4; i++) {
		for(j = 0; j < 5; j++) {
			rksc[i * 10 + j + 5] = 5000 - 1000 * j;
			rkbl[i * 10 + j + 5] = 25 - 5* j;
			rklv[i * 10 + j + 5] = 50 - 10 * j;
			rktime[i * 10 + j + 5] = 10800 - 1800 * j;
			rkfl[i * 10 + j + 5] = 0;
			SDL_strlcpy(string[30 + 10 * i + j + 5], "NOP", STRING_LENGTH);
		}
	}
}

// 何位に入るかチェック（ランク外なら-1）
// 変数end追加 #1.60c7k3
int32_t RankingCheck(int32_t rmode, int32_t rtt, int32_t rsc, int32_t rtime, int32_t rlv, int32_t end) {
	int32_t		i, j, rank;

	if(rmode >= 4) return -1;	// 通常でないモードではランキングなし#1.60c7l5

	j = rmode * 10 +rtt * 5;
	rank = -1;

	if(rtt) {
		for(i = 0; i < 5; i++) {
			// 完全クリアフラグが立っていたら上位に来る #1.60c7k3
			if( (end >= rkfl[j+ i]) && (((rlv > rklv[j+ i]) || (rlv == rklv[j+ i]))&&(rtime < rktime[j+ i])) ) {
				rank = i;
				break;
			}
		}
	} else {
		for(i = 0; i < 5; i++) {
			if(rsc > rksc[j + i]) {
				rank = i;
				break;
			}
		}
	}

	return (rank);
}

void RankingRegist(int32_t rmode, int32_t rtt, int32_t rsc, int32_t rli, int32_t rlv, int32_t rtime, int32_t end, const char *rname) {
	int32_t		i, rank, temp;

	rank = RankingCheck(rmode, rtt, rsc, rtime, rlv, end);

	if(rank == -1) return;

	for(i = 4; i > rank ; i--) {
		temp = rmode* 10 + rtt* 5 + i;
		rksc[temp] = rksc[temp - 1];
		rkbl[temp] = rkbl[temp - 1];
		rklv[temp] = rklv[temp - 1];
		rktime[temp] = rktime[temp - 1];
		SDL_strlcpy(string[30 + temp], string[30 + temp - 1], STRING_LENGTH);
		rkfl[temp] = rkfl[temp - 1];
	}

	temp = rmode* 10 + rtt* 5 + rank;
	rksc[temp] = rsc;
	rkbl[temp] = rli;
	rklv[temp] = rlv;
	rktime[temp] = rtime;
	SDL_strlcpy(string[30 + temp], rname, STRING_LENGTH);
	rkfl[temp] = end;
}

void RankingProc_1() {
	if (init) {
		count = 0;
		flag = 0;
		rankingmode = 0;

		RankingCreate(category, rankingmode);

		init = false;
	}

	count++;

	if(count > 448) {
		count = 0;
		if(rankingmode == 0) {
			rankingmode++;
			RankingCreate(category, rankingmode);
		} else
			flag = 1;
	}

	if (!flag) flag = RankingView();

	if (flag) {
		mainLoopState = lastRankingMainLoopState;
		resumeAfterRanking = 1;
	}
}

void RankingProc2_1(void) {
	if (init) {
		count = 0;
		flag = 0;
		category = 0;

		RankingCreate(category, 0);
		init = false;
	}

	if (flag == 0) {
		count++;

		if(getPushState(0, APP_BUTTON_A) || getPushState(1, APP_BUTTON_A) || getPushState(0, APP_BUTTON_B) || getPushState(1, APP_BUTTON_B)) {
			flag = -1;
		}


		if(getPushState(0, APP_BUTTON_UP) || getPushState(1, APP_BUTTON_UP) || getPushState(0, APP_BUTTON_DOWN) || getPushState(1, APP_BUTTON_DOWN)) {
			rankingmode = 1 - rankingmode;
			RankingCreate(category, rankingmode);
			count = 0;
		}

		if(getPushState(0, APP_BUTTON_LEFT) || getPushState(1, APP_BUTTON_LEFT)) {
			category = (category - 1);
			if(category < 0) category = 3;
			RankingCreate(category, rankingmode);
			count = 0;
		}
		if(getPushState(0, APP_BUTTON_RIGHT) || getPushState(1, APP_BUTTON_RIGHT)) {
			category = (category + 1) % 4;
			RankingCreate(category, rankingmode);
			count = 0;
		}

		RankingView();
	} else {
		mainLoopState = MAIN_TITLE;
		init = true;
	}
}

void RankingCreate(int32_t cat, int32_t mode) {
	int32_t	i, j;

	for(i = 0; i < 5; i++) {
		SDL_strlcpy(string[3], "TH", STRING_LENGTH);
		if(i == 0) SDL_strlcpy(string[3], "ST", STRING_LENGTH);
		if(i == 1) SDL_strlcpy(string[3], "ND", STRING_LENGTH);
		if(i == 2) SDL_strlcpy(string[3], "RD", STRING_LENGTH);

		j = cat* 10 +mode* 5 + i;
		getTime(rktime[j]);
		SDL_snprintf(string[10 + i], STRING_LENGTH, "%2d%s %7d %3d %4d  %s  %s",
			i + 1, string[3], rksc[j], rklv[j], rkbl[j], string[0], string[30 + j]);
	}
}

int32_t RankingView(void) {
	int32_t		i, xxx, col;

	domirror = 0;	// 鏡像を無効化

	// 背景描画
	if(background == 0) {
		for(i = 0; i <= 4; i++) {
			ExBltRect(PLANE_HEBOFLB1 + (category >= 2) + (category == 3), 96 * i - (count & 63) /2, 0, 0, 0, 96, 240);
		}
	} else if(background == 1) {
		for(i = 0; i <= 4; i++) {
			ExBltRect(PLANE_HEBOFLB1 + (category >= 2) + (category == 3), 96 * i, 0, 0, 0, 96, 240);
		}
	} else {
		ExBlt(PLANE_UNUSED, 0, 0);
	}

	if(category == 0)
		printFont(11, 3, "- BEGINNER MODE -", 4);
	else if(category == 1)
		printFont(11, 3, "-  MASTER MODE  -", 1);
	else if(category == 2)
		printFont(11, 3, "-   20G MODE    -", 7);
	else
		printFont(11, 3, "-  DEVIL MODE   -", 3);

	// 簡易セレクトでは表示しない#1.60c7i6
	if(!mini_select) {
		if(rankingmode)
			printFont(11, 4, "TIME TRIAL RANKING", category);
		else
			printFont(12, 4, "ENDLESS RANKING", category);
	} else {
		rankingmode = 1;
	}

//	printFont(2, 6, "RANK   SCORE  LV LINES TIME     NAME", 2);
	ExBltRect(PLANE_HEBOSPR, 64, 56, 206, 112, 22, 7);
	if(rankingmode) {
		ExBltRect(PLANE_HEBOSPR, 240, 56, 154, 112, 26, 7);
		ExBltRect(PLANE_HEBOSPR, 140, 56, 180, 119, 26, 7);
	} else {
		ExBltRect(PLANE_HEBOSPR, 140, 56, 154, 112, 26, 7);
		ExBltRect(PLANE_HEBOSPR, 240, 56, 180, 119, 26, 7);
	}
	ExBltRect(PLANE_HEBOSPR, 216, 48, 180, 112, 26, 7);
	ExBltRect(PLANE_HEBOSPR, 257, 48, 154, 119, 26, 7);

	for(i = 0; i < 5; i++) {
		xxx = 40 - count + 4 * i;
		if(xxx < 2) xxx = 2;
		if(i % 2 == 1) xxx = 4 - xxx;

		// オレンジに表示されるランキングをとりあえず導入#1.60c7f5
		if(rkfl[category * 10 + rankingmode * 5 + i]) {
			// 完全クリアした場合
			col = 7;
		} else if(((category == 0) && (rklv[category * 10 + rankingmode * 5 + i] == 200)) ||
				  ((category == 3) && (rklv[category * 10 + rankingmode * 5 + i] == 1300)) ||
				  (((category == 1) || (category == 2)) && (rklv[category * 10 + rankingmode * 5 + i] == 999))) {
			// ロール途中で窒息した場合
			if(rankingmode) {
				col = 4;
			} else {
				col = 0;
			}
		} else {
			// 未カンスト
			col = 0;
		}

//		printFont(xxx, 8 + 2 * i, string[10 + i], col);
		SDL_snprintf(string[0], STRING_LENGTH, "%d", i + 1);
		printBIGFont((xxx +3) * 8, (4 * i + 9) * 8, string[0], col);
		printFont(xxx + 6, 4 * i + 10, string[30 + category * 10 + rankingmode* 5 + i], col);
		if(rankingmode) {
			SDL_snprintf(string[0], STRING_LENGTH, "%7d", rksc[category * 10 + rankingmode * 5 + i]);
			printFont(xxx + 25, 4 * i + 10, string[0], col);
			getTime(rktime[category * 10 + rankingmode * 5 + i]);
			printBIGFont((xxx + 11) * 8, (4 * i + 9) * 8, string[0], col);
		} else {
			SDL_snprintf(string[0], STRING_LENGTH, "%7d", rksc[category * 10 + rankingmode * 5 + i]);
			printBIGFont((xxx + 11) * 8, (4 * i + 9) * 8, string[0], col);
			getTime(rktime[category * 10 + rankingmode * 5 + i]);
			printFont(xxx + 25, 4 * i + 10, string[0], col);
		}
		SDL_snprintf(string[0], STRING_LENGTH, "%5d/%3d", rklv[category * 10 + rankingmode * 5 + i], rkbl[category * 10 + rankingmode * 5 + i]);
		printFont(xxx + 24, 4 * i + 9, string[0], col);
	}

	if(getPushState(0, APP_BUTTON_A) || getPushState(0, APP_BUTTON_B) || getPushState(1, APP_BUTTON_A) || getPushState(1, APP_BUTTON_B)) {
		return (-1);
	}

	return (0);
}

int32_t RankingSave(void) {
	int32_t i, temp;

	SDL_memset(saveBuf, 0, 50000 * 4);

	// ファイルフォーマット (4byte単位)
	//   0〜    3 ヘッダ
	// 300〜44299 リプレイデータ

	saveBuf[0] = 0x4F424548;
	saveBuf[1] = 0x20534952;
	saveBuf[2] = 0x48474948;
	saveBuf[3] = 0x32764353;

	for(i = 0; i < 40; i++) {
		temp = (i + 1) << 3;
		saveBuf[temp + 0] = rksc[i];
		saveBuf[temp + 1] = rkbl[i];
		saveBuf[temp + 2] = rklv[i];
		saveBuf[temp + 3] = rktime[i];
		saveBuf[temp + 4] = (int32_t)(
			((uint32_t)string[30 + i][0] <<  0) |
			((uint32_t)string[30 + i][1] <<  8) |
			((uint32_t)string[30 + i][2] << 16) |
			((uint32_t)string[30 + i][3] << 24)
		);
		saveBuf[temp + 5] = rkfl[i];
	}

	APP_WriteFile32("config/data/RANKING.SAV", saveBuf, 328);

	return (0);
}

int32_t RankingLoad(void) {
	int32_t i, temp;

	SDL_memset(saveBuf, 0, 50000 * 4);

	APP_ReadFile32("config/data/RANKING.SAV", saveBuf, 4, 0);

	if(saveBuf[0] != 0x4F424548) return (1);
	if(saveBuf[1] != 0x20534952) return (1);
	if(saveBuf[2] != 0x48474948) return (1);

	if(saveBuf[3] == 0x31764353) return (2);
	else if(saveBuf[3] != 0x32764353) return (1);

	APP_ReadFile32("config/data/RANKING.SAV", saveBuf, 328, 0);

	for(i = 0; i < 40; i++) {
		temp = (i + 1) << 3;
		rksc[i] = saveBuf[temp + 0];
		rkbl[i] = saveBuf[temp + 1];
		rklv[i] = saveBuf[temp + 2];
		rktime[i] = saveBuf[temp + 3];
		string[30 + i][0] = (char)((saveBuf[temp + 4] >>  0) & 0xFF);
		string[30 + i][1] = (char)((saveBuf[temp + 4] >>  8) & 0xFF);
		string[30 + i][2] = (char)((saveBuf[temp + 4] >> 16) & 0xFF);
		string[30 + i][3] = (char)((saveBuf[temp + 4] >> 24) & 0xFF);
		rkfl[i] = saveBuf[temp + 5];
	}

	return (0);
}
