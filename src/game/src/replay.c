#include "common.h"

static int32_t chunkBuf[REPLAY_PLAYER_CHUNK / 2];

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  キーが押されてたらリプレイを保存する#1.60c7i9
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void ReplaySaveCheck(int32_t player, int32_t statnumber) {
	int32_t i;

	//速度制限（テスト）
	if(SDL_abs(MAIN_GetRealFPS() - max_fps_2) >= 10) return;

	if((time2[player] <= REPLAY_TIME_MAX) && !playback && replay_save[player]) {
		for(i = 0; i < 10; i++) {
			// TODO: Change how replay saving is selected, away
			// from hard-coded to the keyboard number keys to
			// something using the configured inputs.
			#ifdef INPUT_ENABLE_KEYBOARD
			if(INPUT_IsPushKey(2 + i + player * 14)) {
				saveReplayData(player, i + player * 10 + 1);
				freeReplayData();
				statusc[player * 10 + statnumber] = i + player * 10 + 1;
			}
			#endif
		}
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  リプレイデータをセーブ
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void saveReplayData(int32_t pl, int32_t number) {
	int32_t i, j, temp1, max;

	if(gameMode[pl] == 4){	// VSはフォーマットが一部異なる
		saveReplay_VS(number);
		return;
	}

	SDL_memset(saveBuf, 0, 300 * sizeof(int32_t));

	// ファイルフォーマット (4byte単位)
	//   0〜    3 ヘッダ
	//   4        ファイルサイズ
	//   5〜  179 ネクストブロック
	// 200        タイム
	// 201        ゲームモード
	// 202        スコア
	// 203        レベル
	// 204        ライン
	// 206        下入れ使いまわし設定#1.60c7f9
	// 207		　レベルアップボーナス
	// 208　　　　足きりタイム
	// 209　　　　15分モード

	// 234〜289   空き
	// 290        スムーズモード
	// 291        ナナメ移動許可
	// 292        高速落下モード
	// 293        横方向先行入力
	// 294        赤棒床蹴り
	// 300...     リプレイデータ

	saveBuf[0] = 0x4F424548;
	saveBuf[1] = 0x20534952;
	saveBuf[2] = 0x4C504552;
	saveBuf[3] = 0x31765941;

	for(i = 0; i < 175; i++) {
		temp1 = 0;
		for(j = 0; j < 8; j++) {
			temp1 = temp1 | (nextb[(i << 3) + j + pl * 1400] << (j * 4));
		}
		saveBuf[i + 5] = temp1;
	}

	saveBuf[200] = gametime[pl];
	saveBuf[201] = gameMode[pl];
	saveBuf[202] = sc[pl];
	saveBuf[203] = lv[pl];
	saveBuf[204] = li[pl];
	saveBuf[205] = start[pl];	// 開始レベルの保存 #1.60c3
	saveBuf[206] = downtype;	// 下入れ#1.60c7f9
	saveBuf[207] = lvupbonus;	// レベルアップボーナス #1.60c7g3
	saveBuf[208] = timelimit[pl];	// 足きりタイム #1.60c7g7
	saveBuf[209] = limit15;		// 15分モード#1.60c7h3
	saveBuf[210] = mps[pl * 2    ];
	saveBuf[211] = mps[pl * 2 + 1];
	saveBuf[212] = spawn_y_type;		// 出現位置タイプ #1.60c7s2
	saveBuf[213] = BigMove[pl];			// BIGモードの移動単位 #1.60c7m6
	saveBuf[214] = disable_hold;		// HOLD有効 #1.60c7n7
	saveBuf[215] = disable_irs;			// IRS有効 #1.60c7n7
	saveBuf[216] = start_stage[pl];		// スタート時のステージ（TOMOYO用）#1.60c7n8
	saveBuf[217] = start_nextc[pl];		// スタート時のnextc（TOMOYO用）#1.60c7n8
	saveBuf[218] = t_noborder[pl];		// 足切りなしモード（TOMOYO） #1.60c7n8
	saveBuf[219] = clearnum[pl];		// スキップせずにクリアした数（TOMOYO） #1.60c7o3
	saveBuf[220] = clearp[pl];			// クリア率（TOMOYO） #1.60c7o3
	saveBuf[221] = t_training[pl];		// トレーニングモード（TOMOYO）#1.60c7o7
	saveBuf[222] = disable_wallkick;	// 壁蹴り禁止 #1.60c7q2ex
	saveBuf[223] = r_irs;				// IRS基準 #1.60c7q6
	saveBuf[224] = big_rise;			// BIG時のせり上がり2倍 #1.60c7r2
	saveBuf[225] = anothermode[pl];		// アナザーモード
	saveBuf[226] = randommode[pl];		// ランダムかどうか
	saveBuf[227] = timelimit2[pl];		// LEVEL1000足切りタイム
	saveBuf[228] = timelimitw[pl];		// ワールド用足切りタイム
	saveBuf[229] = world_i_rot;			// ワールド時のI型回転法則 #1.60c7r7
	saveBuf[230] = ybigtype;			// BIG時の縦移動タイプ
	saveBuf[231] = upLineT[pl];			// せり上がりタイプ #1.60c7s6
	saveBuf[232] = enable_grade[pl];	// 段位システムが2種類になったため追加 C7T6.4
	saveBuf[233] = tspin_type;			// スコアに影響 C7T6.4
	saveBuf[234] = mission_file;		// ミッションファイル番号
	saveBuf[235] = start_mission;		// 開始時のミッション番号
	saveBuf[236] = first_seed[pl];		// 乱数シード　超重要
	saveBuf[237] = max_hnext[pl];		// NEXT最大表示数
	saveBuf[238] = death_plus[pl];
	saveBuf[239] = max_fps_2;			// FPS
	saveBuf[240] = enable_minspeed;		// TOMOYO最低速度設定の有無
	saveBuf[241] = nanamedown;
	saveBuf[242] = item_mode[pl];		// アイテムモード
	saveBuf[243] = hebo_plus[pl];		// HEBO+
	saveBuf[244] = item_num;			// 収録時点でのアイテム数
	saveBuf[245] = examination[pl];		// 段位認定試験
	saveBuf[246] = exam_grade[pl];		// 試験段位
	saveBuf[247] = ace_irs;				// IRSがACE式か
	saveBuf[248] = ready_go_style;		// READY→GOの種類
	saveBuf[249] = r_start[pl];
	saveBuf[250] = r_end[pl];
	saveBuf[251] = g_start[pl];
	saveBuf[252] = g_end[pl];
	saveBuf[253] = item_interval;
	saveBuf[254] = devil_minus[pl];		// DEVIL-
	saveBuf[255] = devil_randrise;
		// 256-258番目はSHIRASE関係(容量の節約のため、2つの値を詰め込んでいる) #1.60c7f4
	// 256 どのバージョンで記録されたか？
	// 257 HI = SHIRASEの有効/無効
	// 257 LO = SHIRASEの開始レベル
	// 258 HI = せり上がりの段数
	// 258 LO = せり上がりの間隔
	// これらの値は設定ファイルまたは開始前のメニューから設定される
	saveBuf[256] = repversw;
	saveBuf[257] = (p_shirase[pl] << 16) | level_shirase_start;
	saveBuf[258] = (raise_shirase_lines << 16) | raise_shirase_interval;
	saveBuf[259] = novice_mode[pl];
	saveBuf[260] = std_opt[pl];//262までSTANDARD用の設定
	saveBuf[261] = sp[pl];
	saveBuf[262] = wait1[pl] + wait2[pl] * 0x100 + wait3[pl] * 0x10000 + waitt[pl] * 0x1000000;
	saveBuf[263] = timelimitm[pl];			// devilminus用足切りタイム
	saveBuf[264] = timelimitmw[pl];			// devilminus用足切りタイム
	saveBuf[265] = b2bcheck;		// Back to Back
	saveBuf[266] = p_bgmlv;
	saveBuf[267] = relaymode[pl];
	saveBuf[268] = segacheat;
	saveBuf[269] = SavedSeed[0];
	saveBuf[270] = SavedSeed[1];

	saveBuf[290] = smooth;
	saveBuf[291] = nanameallow;
	saveBuf[292] = sonicdrop;
	saveBuf[293] = fastlrmove;
	if(relaymode[pl])
		saveBuf[294] = first_rot[pl];			// 回転方式のセーブ #1.60c
	else
		saveBuf[294] = rotspl[pl];
	saveBuf[296] = ori_opt[pl];		// のセーブ #1.60c6
	saveBuf[297] = IsBigStart[pl];
	//saveBuf[298] = startnextc[pl];
	saveBuf[299] = w_reverse;			// ワールド時回転方向逆転#1.60c7f8



	// TOMOYOモードではスコアに到達ステージを入れる（手抜き＆暫定的）#1.60c7n8
	if((gameMode[pl] == 6) && (stage[pl] < 100)) {
		saveBuf[202] = stage[pl] + 1;
	}

	max = time2[pl];

	if(max > REPLAY_TIME_MAX) max = REPLAY_TIME_MAX;
	saveBuf[4] = REPLAY_SIZE_1P(max);

	if(number <= 40)
		SDL_snprintf(string[0], STRING_LENGTH, "replay/REPLAY%02d.SAV", number);
	else
		SDL_snprintf(string[0], STRING_LENGTH, "demo/DEMO%02d.SAV", number - 40);

	FILESYSTEM_WriteFile32(string[0], saveBuf, 300);

	temp1 = pl * REPLAY_PLAYER_CHUNK;
	for (j = 0; j < replayChunkCnt; j++) {
		SDL_memset(saveBuf, 0, (REPLAY_PLAYER_CHUNK / 2) * sizeof(int32_t));
		for (i = 0; i < REPLAY_PLAYER_CHUNK / 2 && j * REPLAY_PLAYER_CHUNK + (i << 1) < max; i++) {
			saveBuf[i] =
				(replayData[j][(i << 1) + 0 + temp1] <<  0) |
				(replayData[j][(i << 1) + 1 + temp1] << 16);
		}
		FILESYSTEM_AppendFile32(string[0], saveBuf, i);
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  VSモードのリプレイデータをセーブ
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void saveReplay_VS(int32_t number) {
	int32_t i, j, temp1, temp2, max, pl;
	pl = 0;

	SDL_memset(saveBuf, 0, 300 * sizeof(int32_t));

	// 300... 　1P&2Pリプレイデータ

	saveBuf[0] = 0x4F424548;
	saveBuf[1] = 0x20534952;
	saveBuf[2] = 0x4C504552;
	saveBuf[3] = 0x31765941;

	for(i = 0; i < 175; i++) {
		temp1 = 0;
		for(j = 0; j < 8; j++) {
			temp1 = temp1 | (nextb[(i << 3) + j] << (j * 4));
		}
		saveBuf[i + 5] = temp1;
	}

	saveBuf[200] = gametime[0];
	saveBuf[201] = gameMode[0];
	saveBuf[204] = time2[0];	// リプレイ時間　プレイヤー毎
	saveBuf[205] = time2[1];	//
	saveBuf[206] = downtype;	// 下入れ#1.60c7f9
	saveBuf[207] = lvupbonus;	// レベルアップボーナス #1.60c7g3
//	saveBuf[208]
	saveBuf[209] = limit15;		// 15分モード#1.60c7h3

	saveBuf[210] = mps[0 * 2    ];
	saveBuf[211] = mps[0 * 2 + 1];
	saveBuf[202] = mps[1 * 2    ];
	saveBuf[203] = mps[1 * 2 + 1];

	saveBuf[212] = spawn_y_type;		// 出現位置タイプ #1.60c7s2

	saveBuf[213] = BigMove[0];			// BIGモードの移動単位 #1.60c7m6
	saveBuf[234] = BigMove[1];

	saveBuf[214] = disable_hold;		// HOLD有効 #1.60c7n7
	saveBuf[215] = disable_irs;			// IRS有効 #1.60c7n7
	saveBuf[222] = disable_wallkick;	// 壁蹴り禁止 #1.60c7q2ex
	saveBuf[223] = r_irs;				// IRS基準 #1.60c7q6
	saveBuf[224] = big_rise;			// BIG時のせり上がり2倍 #1.60c7r2
	saveBuf[229] = world_i_rot;			// ワールド時のI型回転法則 #1.60c7r7
	saveBuf[230] = ybigtype;			// BIG時の縦移動タイプ
	saveBuf[231] = upLineT[0];			// せり上がりタイプ #1.60c7s6
	saveBuf[232] = upLineT[1];
	saveBuf[233] = tspin_type;			// スコアに影響 C7T6.4

	saveBuf[235] = first_seed[0];
	saveBuf[236] = first_seed[1];		// 乱数シード　超重要
	saveBuf[237] = max_hnext[0];		// NEXT最大表示数
	saveBuf[238] = max_hnext[1];
	saveBuf[239] = max_fps_2;			// FPS
//	saveBuf[240]
	saveBuf[241] = nanamedown;
//	saveBuf[242]
//	saveBuf[243]
	saveBuf[244] = item_num;			// 収録時点でのアイテム数
//	saveBuf[245]
//	saveBuf[246]
	saveBuf[247] = ace_irs;				// IRSがACE式か
	saveBuf[248] = ready_go_style;		// READY→GOの種類
	saveBuf[249] = r_start[0];
	saveBuf[250] = r_end[0];
	saveBuf[251] = g_start[0];
	saveBuf[252] = g_end[0];
	saveBuf[253] = item_interval;
//	saveBuf[254]
//	saveBuf[255]
	saveBuf[256] = repversw;
	saveBuf[257] = (p_shirase[pl] << 16) | level_shirase_start;
	saveBuf[258] = (raise_shirase_lines << 16) | raise_shirase_interval;
//	saveBuf[259]
	saveBuf[260] = sp[0];
	saveBuf[261] = sp[1];
	saveBuf[262] = wait1[0] + wait2[0] * 0x100 + wait3[0] * 0x10000 + waitt[0] * 0x1000000;
	saveBuf[263] = wait1[1] + wait2[1] * 0x100 + wait3[1] * 0x10000 + waitt[1] * 0x1000000;
//	saveBuf[264]
	saveBuf[265] = b2bcheck;		// Back to Back
//	saveBuf[266]
//	saveBuf[267]
	saveBuf[269] = SavedSeed[0];
	saveBuf[270] = SavedSeed[1];

	saveBuf[290] = smooth;
	saveBuf[291] = nanameallow;
	saveBuf[292] = sonicdrop;
	saveBuf[293] = fastlrmove;
	saveBuf[294] = first_rot[0];			// 回転方式のセーブ #1.60c
	saveBuf[295] = first_rot[1];
//	saveBuf[296]
	saveBuf[297] = IsBigStart[0];
	saveBuf[298] = IsBigStart[1];
	saveBuf[299] = w_reverse;			// ワールド時回転方向逆転#1.60c7f8

	// VS専用項目
	saveBuf[216] = use_item[0];
	saveBuf[217] = use_item[1];
	saveBuf[218] = versus_rot[0];
	saveBuf[219] = versus_rot[1];
	saveBuf[220] = vs_time;
	saveBuf[221] = wintype;
	saveBuf[225] = vs_goal;
	saveBuf[226] = noitem;
	saveBuf[227] = disrise;
	saveBuf[228] = winpoint;

	max = time2[0] > time2[1] ?
		time2[0] :
		time2[1];
	if(max > REPLAY_TIME_MAX) max = REPLAY_TIME_MAX;

	saveBuf[4] = REPLAY_SIZE_2P(max);

	if(number <= 40)
		SDL_snprintf(string[0], STRING_LENGTH, "replay/REPLAY%02d.SAV", number);
	else
		SDL_snprintf(string[0], STRING_LENGTH, "demo/DEMO%02d.SAV", number - 40);

	FILESYSTEM_WriteFile32(string[0], saveBuf, 300);

	if (replayData) {
		for (j = 0; j < (max - 1) / 2 / SAVEBUF_2P_CHUNK + 1; j++) {
			SDL_memset(saveBuf, 0, SAVEBUF_2P_CHUNK * 2 * sizeof(int32_t));
			for (pl = 0; pl < 2; pl++) {
				temp1 = pl * REPLAY_PLAYER_CHUNK;
				temp2 = pl * SAVEBUF_2P_CHUNK;

				int32_t replayIndex = j * SAVEBUF_2P_CHUNK * 2;

				for (i = 0; i < SAVEBUF_2P_CHUNK && (j * SAVEBUF_2P_CHUNK + i) * 2 < max; i++, replayIndex += 2) {
					saveBuf[i + temp2] =
						(replayData[(replayIndex + 0) / REPLAY_PLAYER_CHUNK][(replayIndex + 0) % REPLAY_PLAYER_CHUNK + temp1] <<  0) |
						(
						((j * SAVEBUF_2P_CHUNK + i) * 2 + 1 < max) ?
						(replayData[(replayIndex + 1) / REPLAY_PLAYER_CHUNK][(replayIndex + 1) % REPLAY_PLAYER_CHUNK + temp1] << 16) :
						0
						);
				}
			}
			FILESYSTEM_AppendFile32(string[0], saveBuf, SAVEBUF_2P_CHUNK + i);
		}
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  リプレイデータをロード
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
int32_t loadReplayData(int32_t pl, int32_t number) {
	int32_t temp1, sptemp[4];

	SDL_memset(saveBuf, 0, 300 * sizeof(int32_t));

	if(number <= 40)
		SDL_snprintf(string[0], STRING_LENGTH, "replay/REPLAY%02d.SAV", number);
	else
		SDL_snprintf(string[0], STRING_LENGTH, "demo/DEMO%02d.SAV", number - 40);

	FILESYSTEM_ReadFile32(string[0], saveBuf, 300, 0);

	if(saveBuf[201] == 4)	// VSはフォーマットが一部異なる
		return(loadReplay_VS(number));

	if((size_t)saveBuf[4] > REPLAY_SIZE_1P(REPLAY_TIME_MAX)) saveBuf[4] = REPLAY_SIZE_1P(REPLAY_TIME_MAX);
	if((size_t)saveBuf[4] < 300 * sizeof(int32_t)) return (1);


	if(saveBuf[0] != 0x4F424548) return (1);
	if(saveBuf[1] != 0x20534952) return (1);
	if(saveBuf[2] != 0x4C504552) return (1);
	if(saveBuf[3] != 0x31765941) return (1);

	for(int32_t i = 0; i < 175; i++) {
		for(int32_t j = 0; j < 8; j++) {
			nextb[(i << 3) + j] = ((saveBuf[5 + i]) >> (j * 4)) & 15;
		}
	}

	gameMode[pl] = saveBuf[201];
	start[pl] = saveBuf[205];		// 開始レベルのロード #1.60c3

	// 背景設定#1.60c7h6
	if(start[pl] > 1100)
		backno = 11;
	else
		backno = start[pl] / 100;

	downtype  = saveBuf[206];		// 下入れ#1.60c7f9
	lvupbonus = saveBuf[207];		// レベルアップボーナス #1.60c7g3
	timelimit[pl] = saveBuf[208];		// 足きりタイム #1.60c7g7
	limit15   = saveBuf[209];		// 15分モード#1.60c7h3

	mps[pl * 2    ] = saveBuf[210];
	mps[pl * 2 + 1] = saveBuf[211];
	spawn_y_type = saveBuf[212];		// 出現位置タイプ #1.60c7s2
	BigMove[pl] = saveBuf[213];			// BIGモードの移動単位 #1.60c7m6
	disable_hold = saveBuf[214];		// HOLD有効 #1.60c7n7
	disable_irs = saveBuf[215];			// IRS有効 #1.60c7n7
	start_stage[pl] = saveBuf[216];		// スタート時のステージ（TOMOYO用）#1.60c7n8
	start_nextc[pl] = saveBuf[217];		// スタート時のnextc（TOMOYO用）#1.60c7n8
	t_noborder[pl] = saveBuf[218];		// 足切りなしモード（TOMOYO） #1.60c7n8
	t_training[pl] = saveBuf[221];		// トレーニングモード（TOMOYO）#1.60c7o7
	disable_wallkick = saveBuf[222];	// 壁蹴り禁止 #1.60c7q2ex
	r_irs = saveBuf[223];				// IRS基準 #1.60c7q6
	big_rise = saveBuf[224];			// BIG時のせり上がり2倍 #1.60c7r2
	anothermode[pl] = saveBuf[225];		// アナザーモード
	randommode[pl] = saveBuf[226];  	// ランダムかどうか
	timelimit2[pl] = saveBuf[227];			// LEVEL1000足切りタイム
	timelimitw[pl] = saveBuf[228];			// ワールド用足切りタイム
	world_i_rot = saveBuf[229];			// ワールド時のI型回転法則 #1.60c7r7
	ybigtype = saveBuf[230];			// BIG時の縦移動タイプ
	upLineT[pl] = saveBuf[231];			// せり上がりタイプ #1.60c7s6
	timelimitm[pl] = saveBuf[263];			// devilm用足切りタイム
	timelimitmw[pl] = saveBuf[264];			// devilm用足切りタイム
	smooth = saveBuf[290];
	nanameallow = saveBuf[291];
	sonicdrop = saveBuf[292];
	fastlrmove = saveBuf[293];
	rotspl[pl] = saveBuf[294];			// 回転方式のロード #1.60c
	ori_opt[pl] = saveBuf[296];		// ロード #1.60c6
	IsBigStart[pl] = saveBuf[297];
	w_reverse = saveBuf[299];			// ワールド時回転方向逆転#1.60c7f8

	// スタート時のnextc
	if(gameMode[pl] == 6) {
		nextc[pl] = saveBuf[217];	// TOMOYO用 #1.60c7n8
	} else {
		nextc[pl] = saveBuf[298];	// 通常
	}

	// 256-257番目はSHIRASE関係(容量の節約のため、2つの値を1つの変数に詰め込んでいる) #1.60c7f4
	if(saveBuf[256] == 0) {
		p_shirase[pl] = (saveBuf[201] ==3);
		level_shirase_start = p_shirase_start_level;
		raise_shirase_lines = p_shirase_line;
		raise_shirase_interval = p_shirase_interval;
	} else {
		p_shirase[pl] = saveBuf[257] >> 16;
		level_shirase_start = saveBuf[257] & 0xffff;
		raise_shirase_lines = saveBuf[258] >> 16;
		raise_shirase_interval = saveBuf[258] & 0xffff;
	}
	repversw = saveBuf[256];		// 旧バージョンのリプレイデータを再生するためのスイッチ #1.60c7h2

	if(repversw >= 19){	//このバージョンから新たに追加
		enable_grade[pl] = saveBuf[232];		// 段位システムが2種類になったため追加 C7T6.4
		if(repversw<52){
			tspin_type = saveBuf[233]+1;			// スコアに影響 C7T6.4
		}else{
			tspin_type = saveBuf[233];
		}
	}
	if(repversw >= 21){
		 mission_file = saveBuf[234];		// ミッションファイル番号
		 start_mission = saveBuf[235];		// 開始時のミッション番号
		 first_seed[pl] = saveBuf[236];		// 乱数シード　超重要
		 randseed[pl] = first_seed[pl];
	}
	if(repversw >= 22){
		max_hnext[pl] = saveBuf[237];		// NEXT最大表示数
		death_plus[pl] = saveBuf[238];
	} else {
		death_plus[pl] = 0;
		max_hnext[pl] = dispnext;
	}
	if(repversw >= 26){
		enable_minspeed = saveBuf[240];		// TOMOYO最低速度値を有効にするか
	} else if(repversw == 25){
		enable_minspeed = 1;
	}
	if(repversw >= 28) {
		nanamedown = saveBuf[241];	//斜め入力での固定を有効にするか #C7T9.6EX
		item_mode[pl] = saveBuf[242];
		hebo_plus[pl] = saveBuf[243];
		item_num = saveBuf[244];
	} else if(repversw == 27) {
		nanamedown = 0;
		item_mode[pl] = 0;
		hebo_plus[pl] = 0;
	} else {
		nanamedown = 1;
		item_mode[pl] = 0;
		hebo_plus[pl] = 0;
	}
	if(repversw >= 32){
		examination[pl] = saveBuf[245];
		exam_grade[pl] = saveBuf[246];
		ace_irs = saveBuf[247];
	} else {
		examination[pl] = 0;
		ace_irs = 0;
	}
	if(repversw >= 37){
		ready_go_style = saveBuf[248];
		r_start[pl] = saveBuf[249];
		r_end[pl] = saveBuf[250];
		g_start[pl] = saveBuf[251];
		g_end[pl] = saveBuf[252];
	} else {
		ready_go_style = 0;
	}
	if(repversw >= 38)
		item_interval = saveBuf[253];
	else item_interval = 20;

	if(repversw >= 42)
		devil_minus[pl] = saveBuf[254];
	else devil_minus[pl] = 0;

	if(repversw >= 44)
		devil_randrise = saveBuf[255];
	else devil_randrise = 0;

	if(repversw >= 45)
		novice_mode[pl] = saveBuf[259];
	else novice_mode[pl] = 0;

	std_opt[pl] = saveBuf[260];

	if(repversw >= 54)
		b2bcheck = saveBuf[265];
	else
		b2bcheck = (tspin_type == 2);

	setStartLevel(pl);		// start[pl]から各種速度設定

	if(saveBuf[201] ==9){
		if(std_opt[pl] <= 1){
			sp[pl] = saveBuf[261];
			for(int32_t k = 0; k <= 3; k++) {
				sptemp[k] = (saveBuf[262] >> (k * 8)) & 0xff;
			}
			wait1[pl] = sptemp[0];
			wait2[pl] = sptemp[1];
			wait3[pl] = sptemp[2];
			waitt[pl] = sptemp[3];
			p_bgmlv = saveBuf[266];
			relaymode[pl] = saveBuf[267];
		}
	}
	if (repversw > 65)
	{
		segacheat = saveBuf[268];
		SavedSeed[0]= saveBuf[269];
		SavedSeed[1]= saveBuf[270];
	}
	if(IsBigStart[pl]) {
		IsBig[pl] = 1;
	}

	temp1 = pl * 60 * 60 * 20;

	replayChunkCnt = ((saveBuf[4] / sizeof(int32_t) - 300) * 2 / REPLAY_PLAYER_CHUNK) + 1;
	if (!(replayData = SDL_malloc(replayChunkCnt * sizeof(int32_t*)))) {
		ERROR_Set("Failed allocating replay data");
		MAIN_Exit(SDL_APP_FAILURE);
	}
	for (int32_t j = 0; j < replayChunkCnt; j++) {
		size_t count;
		if (j < replayChunkCnt - 1) {
			count = REPLAY_PLAYER_CHUNK / 2;
		}
		else {
			count = (saveBuf[4] / sizeof(int32_t) - 300) % ((REPLAY_PLAYER_CHUNK / 2));
		}
		FILESYSTEM_ReadFile32(
			string[0],
			chunkBuf,
			count,
			(300 + j * (REPLAY_PLAYER_CHUNK / 2)) * 1
		);
		if (!(replayData[j] = SDL_calloc(REPLAY_CHUNK_SIZE, 1u))) {
			ERROR_Set("Failed allocating replay data");
			MAIN_Exit(SDL_APP_FAILURE);
		}

		for(size_t i = 0; i < count; i++) {
			replayData[j][(i << 1) + temp1    ] =  chunkBuf[i] & 0xFFFF;
			replayData[j][(i << 1) + temp1 + 1] = (chunkBuf[i] & 0xFFFF0000) >> 16;
		}
	}

	next[pl] = nextb[nextc[pl] + pl * 1400];	// #1.60c7n7
	setNextBlockColors(pl, 1);	// #1.60c7n2

	stage[pl] = start_stage[pl];	// 開始ステージ #1.60c7n8

	return (0);
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  VSのリプレイデータをロード
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
int32_t loadReplay_VS(int32_t number) {
	int32_t i, j, length, temp1, max,k,sptemp[4],pl;
	pl = 0;

	SDL_memset(saveBuf, 0, 300 * sizeof(int32_t));

	if(number <= 40)
		SDL_snprintf(string[0], STRING_LENGTH, "replay/REPLAY%02d.SAV", number);
	else
		SDL_snprintf(string[0], STRING_LENGTH, "demo/DEMO%02d.SAV", number - 40);

	FILESYSTEM_ReadFile32(string[0], saveBuf, 300, 0);

	if((size_t)saveBuf[4] > REPLAY_SIZE_2P(REPLAY_TIME_MAX)) saveBuf[4] = REPLAY_SIZE_2P(REPLAY_TIME_MAX);
	if((size_t)saveBuf[4] < 300 * sizeof(int32_t)) return (1);


	FILESYSTEM_ReadFile32(string[0], saveBuf, saveBuf[4] / sizeof(int32_t), 0);

	if(saveBuf[0] != 0x4F424548) return (1);
	if(saveBuf[1] != 0x20534952) return (1);
	if(saveBuf[2] != 0x4C504552) return (1);
	if(saveBuf[3] != 0x31765941) return (1);

	for(i = 0; i < 175; i++) {
		for(j = 0; j < 8; j++) {
			nextb[(i << 3) + j] = ((saveBuf[5 + i]) >> (j * 4)) & 15;
		}
	}
	// 1PのNEXTを2Pにコピー #1.60c7m1
	for(i=0;i<1400;i++) {
		nextb[i + 1400] = nextb[i];
	}

//	max = (saveBuf[200] + 3730) / 2 + 1;
	gameMode[0] = saveBuf[201];
	gameMode[1] = saveBuf[201];
	start[0] = 0;
	start[1] = 0;

	backno = 61;

	downtype  = saveBuf[206];		// 下入れ#1.60c7f9
	lvupbonus = saveBuf[207];		// レベルアップボーナス #1.60c7g3
//	timelimit[pl] = saveBuf[208];		// 足きりタイム #1.60c7g7
	limit15   = saveBuf[209];		// 15分モード#1.60c7h3

	mps[0 * 2    ] = saveBuf[210];
	mps[0 * 2 + 1] = saveBuf[211];
	mps[1 * 2    ] = saveBuf[202];
	mps[1 * 2 + 1] = saveBuf[203];
	spawn_y_type = saveBuf[212];		// 出現位置タイプ #1.60c7s2
	BigMove[0] = saveBuf[213];			// BIGモードの移動単位 #1.60c7m6
	BigMove[1] = saveBuf[234];
	disable_hold = saveBuf[214];		// HOLD有効 #1.60c7n7
	disable_irs = saveBuf[215];			// IRS有効 #1.60c7n7
	disable_wallkick = saveBuf[222];	// 壁蹴り禁止 #1.60c7q2ex
	r_irs = saveBuf[223];				// IRS基準 #1.60c7q6
	big_rise = saveBuf[224];			// BIG時のせり上がり2倍 #1.60c7r2
	world_i_rot = saveBuf[229];			// ワールド時のI型回転法則 #1.60c7r7
	ybigtype = saveBuf[230];			// BIG時の縦移動タイプ
	upLineT[0] = saveBuf[231];			// せり上がりタイプ #1.60c7s6
	upLineT[1] = saveBuf[232];

	smooth = saveBuf[290];
	nanameallow = saveBuf[291];
	sonicdrop = saveBuf[292];
	fastlrmove = saveBuf[293];

	rotspl[0] = saveBuf[294];			// 回転方式のロード #1.60c
	rotspl[1] = saveBuf[295];

	IsBigStart[0] = saveBuf[297];
	IsBigStart[1] = saveBuf[298];
	w_reverse = saveBuf[299];			// ワールド時回転方向逆転#1.60c7f8


	nextc[0] = 0;
	nextc[1] = 0;

	// 256-257番目はSHIRASE関係(容量の節約のため、2つの値を1つの変数に詰め込んでいる) #1.60c7f4
	p_shirase[pl] = saveBuf[257] >> 16;
	level_shirase_start = saveBuf[257] & 0xffff;
	raise_shirase_lines = saveBuf[258] >> 16;
	raise_shirase_interval = saveBuf[258] & 0xffff;

	repversw = saveBuf[256];		// 旧バージョンのリプレイデータを再生するためのスイッチ #1.60c7h2

	tspin_type = saveBuf[233];

	max_hnext[0] = saveBuf[237];		// NEXT最大表示数
	max_hnext[1] = saveBuf[238];

	first_seed[0] = saveBuf[235];		// 乱数シード　超重要
	randseed[0] = first_seed[0];

	first_seed[1] = saveBuf[236];
	randseed[1] = first_seed[1];

	nanamedown = saveBuf[241];	//斜め入力での固定を有効にするか #C7T9.6EX
	item_num = saveBuf[244];

	ace_irs = saveBuf[247];

	ready_go_style = saveBuf[248];
	r_start[0] = saveBuf[249];
	r_end[0] = saveBuf[250];
	g_start[0] = saveBuf[251];
	g_end[0] = saveBuf[252];

	r_start[1] = saveBuf[249];
	r_end[1] = saveBuf[250];
	g_start[1] = saveBuf[251];
	g_end[1] = saveBuf[252];

	use_item[0] = saveBuf[216];
	use_item[1] = saveBuf[217];
	versus_rot[0] = saveBuf[218];
	versus_rot[1] = saveBuf[219];
	vs_time  = saveBuf[220];
	wintype = saveBuf[221];
	vs_goal = saveBuf[225];
	noitem = saveBuf[226];
	disrise = saveBuf[227];
	winpoint = saveBuf[228];

	item_interval = saveBuf[253];

	b2bcheck = saveBuf[265];

//	setStartLevel(pl);		// start[pl]から各種速度設定

	sp[0] = saveBuf[260];
	for(k = 0; k <= 3; k++) {
		sptemp[k] = (saveBuf[262] >> (k * 8)) & 0xff;
	}
	wait1[0] = sptemp[0];
	wait2[0] = sptemp[1];
	wait3[0] = sptemp[2];
	waitt[0] = sptemp[3];

	sp[1] = saveBuf[261];
	for(k = 0; k <= 3; k++) {
		sptemp[k] = (saveBuf[263] >> (k * 8)) & 0xff;
	}
	wait1[1] = sptemp[0];
	wait2[1] = sptemp[1];
	wait3[1] = sptemp[2];
	waitt[1] = sptemp[3];

	if(IsBigStart[0]) {
		IsBig[0] = 1;
	}

	if(IsBigStart[1]) {
		IsBig[1] = 1;
	}
	#if 0

	for(pl = 0; pl <= 1; pl++){
		temp1 = pl * 60 * 60 * 20;

		max[pl] = 60 * 60 * 10 / 2;

		if(max[pl] > 60 * 60 * 10 / 2) max[pl] = 60 * 60 * 10 / 2;
		for(i = 0; i < max[pl]; i++) {
			replayData[(i << 1) + temp1    ] =  saveBuf[i + 300 + (SAVEBUF_2P_CHUNK * pl)] & 0xFFFF;
			replayData[(i << 1) + temp1 + 1] = (saveBuf[i + 300 + (SAVEBUF_2P_CHUNK * pl)] & 0xFFFF0000) >> 16;
		}

		next[pl] = nextb[nextc[pl] + pl * 1400];	// #1.60c7n7
		setNextBlockColors(pl, 1);	// #1.60c7n2
	}

	#else

	length = (saveBuf[4] / sizeof(int32_t) - 300);
	max = ((length % SAVEBUF_2P_CHUNK) + (length / (SAVEBUF_2P_CHUNK * 2)) * SAVEBUF_2P_CHUNK) * 2;

	replayChunkCnt = ((max - 1) / REPLAY_PLAYER_CHUNK) + 1;
	if (!(replayData = SDL_malloc(replayChunkCnt * sizeof(int32_t*)))) {
		ERROR_Set("Failed allocating replay data");
		MAIN_Exit(SDL_APP_FAILURE);
	}
	for (i = 0; i < replayChunkCnt; i++) {
		if (!(replayData[i] = SDL_calloc(REPLAY_CHUNK_SIZE, 1u))) {
			ERROR_Set("Failed allocating replay data");
			MAIN_Exit(SDL_APP_FAILURE);
		}
	}

	for (j = 0; j < (max - 1) / 2 / SAVEBUF_2P_CHUNK + 1; j++) {
		for(pl = 0; pl <= 1; pl++){
			temp1 = pl * REPLAY_PLAYER_CHUNK;

			int32_t replayIndex = j * SAVEBUF_2P_CHUNK * 2;

			for(i = 0; i < SAVEBUF_2P_CHUNK && (j * SAVEBUF_2P_CHUNK + i) * 2 < max; i++, replayIndex += 2) {
				replayData[(replayIndex + 0) / REPLAY_PLAYER_CHUNK][(replayIndex + 0) % REPLAY_PLAYER_CHUNK + temp1] =  saveBuf[i + 300 + (SAVEBUF_2P_CHUNK * pl)] & 0xFFFF;
				if ((j * SAVEBUF_2P_CHUNK + i) * 2 + 1 < max) {
					replayData[(replayIndex + 1) / REPLAY_PLAYER_CHUNK][(replayIndex + 1) % REPLAY_PLAYER_CHUNK + temp1] = (saveBuf[i + 300 + (SAVEBUF_2P_CHUNK * pl)] & 0xFFFF0000) >> 16;
				}
			}

			next[pl] = nextb[nextc[pl] + pl * 1400];	// #1.60c7n7
			setNextBlockColors(pl, 1);	// #1.60c7n2
		}
	}

	#endif
	SavedSeed[0]= saveBuf[269]; // harmless if it's zero anyway
	SavedSeed[1]= saveBuf[270]; // harmless if it's zero anyway


	return (0);
}

void freeReplayData() {
	if (replayData) {
		for (int32_t i = 0u; i < replayChunkCnt; i++) {
			SDL_free(replayData[i]);
		}
		SDL_free(replayData);
		replayData = NULL;
		replayChunkCnt = 0;
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  リプレイ関連
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void ReplaySelectProc(void) {
	if (init) {
		ReplaySelectInitial();
		flag = 0;
		init = false;
	}

	if (flag == 0) {
		count++;
		ReplaySelect();
	} else if(flag > 0) {
		demo = 0;
		playback = 1;
		domirror = 1;	// 鏡像を有効

		gameAllInit();
		playerInitial(0);
		playerInitial(1);
		loadReplayData(0, flag);

		timeOn[0] = 0;
		onRecord[0] = 0;
		status[0] = 1;					// ブロックシャッター実行
		statusc[0] = 0;					// ステータスカウンタを0に
		statusc[1] = 3;					// シャッター後はステータスNo.3
		if(examination[0]) statusc[1] = 30;
		//next[0] = nextb[nextc[0]];		// #1.60c7
		if(gameMode[0] == 6){
			if((start_stage[0] >= 27) && (start_stage[0] <= 44))
				ltime[0] = 600 * 60;
			else if(start_stage[0] >= 100)
				ltime[0] = 1080 * 60;
		}
		if((gameMode[0] == 9)&&(std_opt[0] == 1)){
			ltime[0] = 120 * 60;
		}
		// 段位設定が記録されていないリプレイを
		// 4設定で再生すると確実にずれるので。
		if((repversw < 19) && (enable_grade[0] == 4))
			enable_grade[0] = 2;
		bgmlv = setstartBGM(gameMode[0], 0);
		dispLinfo = 1;
		hnext[0] = max_hnext[0];
		if(gameMode[0] == 8){
			c_mission = start_mission;
			loadMissionData(mission_file);
			if(mission_type[c_mission] == 19)
				backno = 10;
			else if((mission_type[c_mission] == 20) || (mission_type[c_mission] == 22))
				backno = 11;
			else {
			backno = ((c_mission+1) % 10)-1;
			if(backno < 0)
				backno = 9;
			}
			if(backno >= bg_max) backno = bg_max;
			bgmlv = mission_bgm[c_mission];
			if(repversw < 54) missionSetStatus();
			if(((mission_file == 6) || (mission_file == 18) || (mission_file == 24)) && (start_mission == 0)) PlaySE(WAVE_SE_CHEER);
		}
		if(gameMode[0] == 4){
			timeOn[1] = 0;
			onRecord[1] = 0;
			hnext[1] = max_hnext[1];
			gameMode[1] = 4;		// 2P 設定
			status[1] = 1;			// ブロックシャッター実行
			statusc[0 + 10] = 0;		// ステータスカウンタを0に
			statusc[1 + 10] = 3;		// シャッター後はステータスNo.3
			if(!tmp_maxPlay)
				maxPlay = 1;
			domirror = 0;
		}
		if(((gameMode[0] == 3) && (!devil_minus[0])) || ((gameMode[0] == 7) && (anothermode[0] == 2))){
			PlaySE(WAVE_SE_CHEER);//歓声
			gflash[0]=120;
		}
		mainLoopState = MAIN_GAME_EXECUTE;
		init = true;
	} else {
		mainLoopState = MAIN_TITLE;
		init = true;
	}
}

//BGMを決定
int32_t ReplayBgmModeDecide(int32_t mode,int32_t nv,int32_t dm,int32_t eg){
	if(mode==0){
		if(!nv){//HANABI
			return 0;
		}else{
			return 1;//SCORE
		}
	}else if(mode==3){
		if(!dm){
			return 5;
		}else{
			return 6;
		}
	}else{
		if(eg==1){
			return 2;
		}else if(eg==4){
			return 4;
		}else{
			return 3;
		}
	}
}
int32_t getModeAddRep(void){
	if(saveBuf[201] == 0) return saveBuf[259];
	else if((saveBuf[201] == 1) || (saveBuf[201] == 2)) return saveBuf[232];
	else if(saveBuf[201] == 3) return saveBuf[254];
	else if(saveBuf[201] == 6) return (saveBuf[216] >= 27)+(saveBuf[216] >= 45)+(saveBuf[216] >= 69)+(saveBuf[216] >= 100);
	else if(saveBuf[201] == 7) return saveBuf[225];
	else if(saveBuf[201] == 9){
		if(saveBuf[267]) return 4;
		else return saveBuf[260];
	}else if(saveBuf[201] == 10) return saveBuf[296];
	else return 0;
}
void ReplaySelectInitial(void) {
	int32_t		i;

	cnt = 0;
	csr = 0;

	for(i = 0; i < 40; i++) {

		if(!loadReplayData(0, i + 1)) {
			enable[i] = saveBuf[201];
			enable[i + 40] = getModeAddRep();
			cnt++;
			getTime(saveBuf[200]);
			// ファイル番号表示追加 #1.60c7i5
			if(saveBuf[201] == 6) {
				// TOMOYO
				if(saveBuf[216] <= 26)
					SDL_snprintf(string[1], STRING_LENGTH, "%3d>", saveBuf[220]);	// クリア率
				else
					SDL_snprintf(string[1], STRING_LENGTH, "    ");
				SDL_snprintf(string[10 + i], STRING_LENGTH, "%2d %7d  %s        %s", i + 1, saveBuf[202], string[1], string[0]);
			} else if(saveBuf[201] == 7) {
				// ACE
				SDL_snprintf(string[10 + i], STRING_LENGTH, "%2d          %4d  %4d  %s", i + 1, saveBuf[203], saveBuf[204], string[0]);
			} else if(saveBuf[201] == 9){
				if(saveBuf[260] == 0)
					SDL_snprintf(string[10 + i], STRING_LENGTH, "%2d                      %s", i + 1, string[0]);
				else if(saveBuf[260] == 1)
					SDL_snprintf(string[10 + i], STRING_LENGTH, "%2d                %4d  ", i + 1, saveBuf[204]);
				else
					SDL_snprintf(string[10 + i], STRING_LENGTH, "%2d %7d  %4d  %4d  %s", i + 1, saveBuf[202], saveBuf[203], saveBuf[204], string[0]);
			} else if((saveBuf[201] != 8) && (saveBuf[201] != 4)){
				// 通常
				SDL_snprintf(string[10 + i], STRING_LENGTH, "%2d %7d  %4d  %4d  %s", i + 1, saveBuf[202], saveBuf[203], saveBuf[204], string[0]);
			} else
				SDL_snprintf(string[10 + i], STRING_LENGTH,"%2d                      %s ", i + 1,string[0]);
		} else {
			enable[i] = -1;
			SDL_strlcpy(string[10 + i], "", STRING_LENGTH);
		}
		freeReplayData();
	}

	if(cnt) {
		while(enable[csr] == -1) {
			csr++;
			if(csr >= 40) csr = 0;
		}
	}
}

void ReplaySelect(void) {
	int32_t		i,startNo,endNo;

	// 背景描画
	if(background == 0) {
		for(i = 0; i <= 4; i++) {
			if(getDrawRate() == 1)
				VIDEO_DrawPlaneRect(PLANE_HEBOFLB1, 96 * i - (count % 96) / 3, 0, 0, 0, 96, 240);
			else
				VIDEO_DrawPlaneRect(PLANE_HEBOFLB1, 192 * i - (count % 32), 0, 0, 0, 192, 480);
		}
	} else if(background == 1) {
		for(i = 0; i <= 4; i++) {
			ExBltRect(PLANE_HEBOFLB1, 96 * i, 0, 0, 0, 96, 240);
		}
	} else {
		ExBlt(PLANE_UNUSED, 0, 0);
	}

	// Bで戻る
	if(getPushState(0, INPUT_BUTTON_B)) {
		restoreSetups();
		if(gameMode[0] == 8) gameMode[0] = 0;
		if(gameMode[0] == 4){
			gameMode[0] = 0;
			gameMode[1] = 0;
		}
		flag = -1;
	}

	if(!cnt) {
		printFont(13, 14, "NO REPLAY DATA", 2);
		return;
	}

	ExBltRect(PLANE_LINE, 0, 232,  count % 320, 20, 320 - (count % 320), 8);
	ExBltRect(PLANE_LINE, 320 - (count % 320), 232,  0, 20, count % 320, 8);

	ExBltRect(PLANE_LINE, count % 320, 0,  0, 28, 320 - (count % 320), 8);
	ExBltRect(PLANE_LINE, 0, 0, 320 - (count % 320), 28, count % 320, 8);

	printFont(9, 2, "- SELECT REPLAY DATA -", 4);
	printMenuButton(12, 3, INPUT_BUTTON_C, -1);
	printFont(6, 3, "PRESS   BUTTON TO VIEW DETAIL", 5);

	// ↑↓カーソルリピード #1.60c7o6
	padRepeat2(0);

	// ↑
	if( (mpc2[0] == 1) || ((mpc2[0] > tame3) && (mpc2[0] % tame4 == 0)) ) {
		if(getPressState(0, INPUT_BUTTON_UP)) {
			PlaySE(WAVE_SE_MOVE);
			do {
				csr--;
				if(csr < 0) csr = 39;
			} while(enable[csr] == -1);
		}
	}

	// ↓
	if( (mpc2[0] == 1) || ((mpc2[0] > tame3) && (mpc2[0] % tame4 == 0)) ) {
		if(getPressState(0, INPUT_BUTTON_DOWN)) {
			PlaySE(WAVE_SE_MOVE);
			do {
				csr++;
				if(csr > 39) csr = 0;
			} while(enable[csr] == -1);
		}
	}

	// Aで開始
	if(getPushState(0, INPUT_BUTTON_A)) {
		PlaySE(WAVE_SE_KETTEI);
		flag = csr + 1;
	}

//	printFont(1, 5, "FILE   SCORE  LV LINES TIME     MODE", 2);
	ExBltRect(PLANE_HEBOSPR,  48, 40, 154, 112, 26, 7);//SCOREの文字
	ExBltRect(PLANE_HEBOSPR,  95, 40, 180, 112, 26, 7);
	ExBltRect(PLANE_HEBOSPR, 134, 40, 154, 119, 26, 7);
	ExBltRect(PLANE_HEBOSPR, 180, 40, 180, 119, 26, 7);
	ExBltRect(PLANE_HEBOSPR, 232, 40, 206, 119, 22, 7);//Mode

	for(i = 0; i < 40; i++) ExBltRect(PLANE_HEBOSPR, 8 * i, (csr - (20 * (csr >= 20))) * 9 + 53, 20, 34, 8, 4);

	if(csr >= 20) {
		printFont(7, 29, "PAGE 2/2 (NO.21 - NO.40)", 7);
		startNo = 20;
		endNo = 40;
	} else {
		printFont(7, 29, "PAGE 1/2 (NO.01 - NO.20)", 7);
		startNo = 0;
		endNo = 20;
	}
	for(i = startNo; i < endNo; i++) {
		printSMALLFont(25, 48 + 9 * (i - 20 * (i >= 20)), string[10 + i], 0); // 番号表示に従い変更 #1.60c7i5
		if(enable[i] > -1){
			ExBltRect(PLANE_GAMEMODEFONT, 232, 48 + 9 * (i - 20 * (i >= 20)), 72 * enable[i + 40], enable[i] * 9, 72, 9);
		}
	}

	// Cで詳細
	if(getPushState(0, INPUT_BUTTON_C)) {
		PlaySE(WAVE_SE_KETTEI);
		mainLoopState = MAIN_REPLAY_DETAIL;
		init = true;
	}
}

// リプレイ詳細 #1.60c7p5
void ReplayDetail() {
	if (init) {
		// リプレイデータ読み込み
		loadReplayData(0, csr + 1);
		init = false;
	}

	// 背景描画
	count++;
	if(background == 0) {
		for(int32_t i = 0; i <= 4; i++) {
			if(getDrawRate() == 1)
				VIDEO_DrawPlaneRect(PLANE_HEBOFLB1, 96 * i - (count % 96) / 3, 0, 0, 0, 96, 240);
			else
				VIDEO_DrawPlaneRect(PLANE_HEBOFLB1, 192 * i - (count % 32), 0, 0, 0, 192, 480);
		}
	} else if(background == 1) {
		for(int32_t i = 0; i <= 4; i++) {
			ExBltRect(PLANE_HEBOFLB1, 96 * i, 0, 0, 0, 96, 240);
		}
	} else {
		ExBlt(PLANE_UNUSED, 0, 0);
	}
	ExBltRect(PLANE_LINE, 0, 232,  count % 320, 20, 320 - (count % 320), 8);
	ExBltRect(PLANE_LINE, 320 - (count % 320), 232,  0, 20, count % 320, 8);

	ExBltRect(PLANE_LINE, count % 320, 0,  0, 28, 320 - (count % 320), 8);
	ExBltRect(PLANE_LINE, 0, 0, 320 - (count % 320), 28, count % 320, 8);
	// 詳細を表示
	printFont(1, 1,  "REPLAY DETAIL", 5);

	/* 基本情報 */
	printFont(1, 3,  "NUMBER      :", 0);
	SDL_snprintf(string[0], STRING_LENGTH,"%d",csr + 1);
	printFont(15, 3, string[0], 0);

	printFont(1, 4,  "GAME MODE   :", 0);
	if(gameMode[0] == 0) printFont(15, 4, "BEGINNER", 4);
	else if(gameMode[0] == 1) printFont(15, 4, "MASTER", 1);
	else if(gameMode[0] == 2) printFont(15, 4, "20G", 5);
	else if(gameMode[0] == 3) printFont(15, 4, "DEVIL", 2);
	else if(gameMode[0] == 4) printFont(15, 4, "VERSUS", 7);
	else if(gameMode[0] == 6) printFont(15, 4, "TOMOYO", 3);
	else if(gameMode[0] == 7) printFont(15, 4, "ACE", 0);
	else if(gameMode[0] == 8) printFont(15, 4, "MISSION", 8);
	else if(gameMode[0] == 9) printFont(15, 4, "SIMPLE", 9);
	else if(gameMode[0] == 10) printFont(15, 4, "ORIGINAL", 0);
	else printFont(15, 4, "INVALID MODE", 0);

	if(death_plus[0]) printFont(21, 4, "DEATH+", 2);
	if(devil_minus[0]) printFont(28-(7*(!death_plus[0])), 4, "MINUS", 2);
	if(item_mode[0]) printFont(22, 4, "ITEM MODE", 5);
	if(hebo_plus[0]) printFont(22, 4, "HEBO+", 9);
	if(examination[0]) printFont(22, 4, "EXAMINATION", 7);
	if(gameMode[0] == 7){
		if(anothermode[0] == 1) printFont(18, 4, " ANOTHER", 0);
		if(anothermode[0] == 2) printFont(18, 4, " HELL", 2);
		else printFont(18, 4, " OLD STYLE", 9);
	}
	if(gameMode[0] == 0){
		if(novice_mode[0]) printFont(24, 4, "SCORE TRIAL", 4);
		else printFont(24, 4, "TAMAYA TRIAL", 4);
	}
	if(gameMode[0] == 9){
		if(relaymode[0]){
			if(std_opt[0]==0)printFont(24, 4, "ROT.RELAY 40L", 9);
			else printFont(24, 4, "ROT.RELAY 2MIN", 9);
		}else{
			if(std_opt[0]==0) printFont(24, 4, "40LINES", 9);
			else if(std_opt[0]==1) printFont(24, 4, "ULTRA2MIN", 9);
			else if(std_opt[0]==2) printFont(24, 4, "SQUARE", 9);
			else printFont(24, 4, "MARATHON", 4);
		}
	}

	if(gameMode[0] != 4){
		printFont(1, 5,  "ROTATE RULE :", 0);
		if(rotspl[0] == 0) printFont(15, 5, "HEBORIS", fontc[0]);
		else if(rotspl[0] == 1) printFont(15, 5, "TI-ARS", fontc[1]);
		else if(rotspl[0] == 2) printFont(15, 5, "TI-WORLD", fontc[2]);
		else if(rotspl[0] == 3) printFont(15, 5, "ACE-SRS", fontc[3]);
		else if(rotspl[0] == 4) printFont(15, 5, "ACE-ARS", fontc[4]);
		else if(rotspl[0] == 5) printFont(15, 5, "ACE-ARS2", fontc[5]);
		else if(rotspl[0] == 6) printFont(15, 5, "DS-WORLD", fontc[6]);
		else if(rotspl[0] == 7) printFont(15, 5, "SRS-X", fontc[7]);
		else if(rotspl[0] == 8) printFont(15, 5, "D.R.S", fontc[8]);
	}

	/* 設定 */
	if((gameMode[0] != 4) && (gameMode[0] != 8) && (gameMode[0] != 9)){
		printFont(1, 6,  "LVUP RULE   :", 0);
		if((gameMode[0] <= 3) || (gameMode[0] == 6))
			printFont(15, 6, "BLOCK+ERASE", 0);
		else
			printFont(15, 6, "ERASE LINES", 0);
	}


	printFont(1, 8,  "8WAY INPUT  :", 0);
	if(nanameallow) printFont(15, 8, "e", 0);
	else printFont(15, 8, "c", 0);

	printFont(1, 9,  "SONIC DROP  :", 0);
	if(sonicdrop) printFont(15, 9, "e", 0);
	else printFont(15, 9, "c", 0);

	printFont(1, 10, "INIT LR MOVE:", 0);
	if(fastlrmove) printFont(15, 10, "e", 0);
	else printFont(15, 10, "c", 0);

	printFont(1, 11, "WORLDREVERSE:", 0);
	if(w_reverse) printFont(15, 11, "c", 0);
	else printFont(15, 11, "e", 0);

	printFont(1, 12, "DOWN RESET  :", 0);
	if(downtype) printFont(15, 12, "c", 0);
	else printFont(15, 12, "e", 0);

	printFont(1, 13, "LVUP BONUS  :", 0);
	if(lvupbonus==2) printFont(15, 13, "AUTO", 0);
	else if(lvupbonus==1) printFont(15, 13, "e", 0);
	else printFont(15, 13, "c", 0);

	/* スコア類 */
	if(gameMode[0] == 6) {
		// TOMOYO
		printFont(1, 15, "STAGE       :", 0);
		SDL_snprintf(string[0], STRING_LENGTH,"%d",saveBuf[202]);
		printFont(15, 15, string[0], 0);

		printFont(1, 16, "START STAGE :", 0);
		SDL_snprintf(string[0], STRING_LENGTH,"%d",start_stage[0] + 1);
		printFont(15, 16, string[0], 0);

		printFont(1, 17, "START NEXTC :", 0);
		SDL_snprintf(string[0], STRING_LENGTH,"%d",start_nextc[0]);
		printFont(15, 17, string[0], 0);

		printFont(1, 18, "BORDER      :", 0);
		if(t_noborder[0]) printFont(15, 18, "e", 0);
		else printFont(15, 18, "c", 0);

		printFont(1, 19, "TRAINING    :", 0);
		if(t_training[0] == 2) printFont(15, 19, "B", 0);
		else if(t_training[0] == 1) printFont(15, 19, "A", 0);
		else printFont(15, 19, "e", 0);

		printFont(1, 20, "CLEAR STAGE :", 0);
		SDL_snprintf(string[0], STRING_LENGTH,"%d",saveBuf[219]);
		printFont(15, 20, string[0], 0);
		if(start_stage[0] <= 26){
			printFont(1, 21, "CLEAR PER.  :", 0);
			SDL_snprintf(string[0], STRING_LENGTH,"%d%%",saveBuf[220]);
			printFont(15, 21, string[0], 0);
		}

		printFont(1, 22, "RANDOM      :", 0);
		if(saveBuf[226]) printFont(15, 22, "c", 0);
		else printFont(15, 22, "e", 0);

		printFont(1, 23, "ENABLE SPEED:", 0);
		if((saveBuf[240]) || (repversw == 25)) printFont(15, 23, "c", 0);
		else printFont(15, 23, "e", 0);
	} else if(gameMode[0] == 4){
		//VERSUS
		printFont(1, 15,  "ROTATE RULE", 0);
		printFont(3, 16,  "1P :", 0);
		if(versus_rot[0] == 0) printFont(8, 16, "HEBORIS", fontc[0]);
		else if(versus_rot[0] == 1) printFont(8, 16, "TI-ARS", fontc[1]);
		else if(versus_rot[0] == 2) printFont(8, 16, "TI-WORLD", fontc[2]);
		else if(versus_rot[0] == 3) printFont(8, 16, "ACE-SRS", fontc[3]);
		else if(versus_rot[0] == 4) printFont(8, 16, "ACE-ARS", fontc[4]);
		else if(versus_rot[0] == 5) printFont(8, 16, "ACE-ARS2", fontc[5]);
		else if(versus_rot[0] == 6) printFont(8, 16, "DS-WORLD", fontc[6]);
		else if(versus_rot[0] == 7) printFont(8, 16, "SRS-X", fontc[7]);
		else if(versus_rot[0] == 8) printFont(8, 16, "D.R.S", fontc[8]);
		else printFont(8, 16, "RANDOM", 0);

		printFont(3, 17,  "2P :", 0);
		if(versus_rot[1] == 0) printFont(8, 17, "HEBORIS", fontc[0]);
		else if(versus_rot[1] == 1) printFont(8, 17, "TI-ARS", fontc[1]);
		else if(versus_rot[1] == 2) printFont(8, 17, "TI-WORLD", fontc[2]);
		else if(versus_rot[1] == 3) printFont(8, 17, "ACE-SRS", fontc[3]);
		else if(versus_rot[1] == 4) printFont(8, 17, "ACE-ARS", fontc[4]);
		else if(versus_rot[1] == 5) printFont(8, 17, "ACE-ARS2", fontc[5]);
		else if(versus_rot[1] == 6) printFont(8, 17, "DS-WORLD", fontc[6]);
		else if(versus_rot[1] == 7) printFont(8, 17, "SRS-X", fontc[7]);
		else if(versus_rot[1] == 8) printFont(8, 17, "D.R.S", fontc[8]);
		else printFont(8, 17, "RANDOM", 0);

		printFont(1, 18, "MODE        :", 0);
		if((disrise) && (noitem)) printFont(15, 18, "CEMENT", 0);
		else if((!disrise) && (noitem)) printFont(15, 18, "NO ITEM", 0);
		else if((disrise) && (!noitem)) printFont(15, 18, "ITEM ONLY", 0);
		else printFont(15, 18, "NORMAL", 0);

		printFont(1, 19, "GOAL TYPE   :", 0);
		if(vs_goal == 0) SDL_snprintf(string[0], STRING_LENGTH, "SURVIVAL");
		else if(wintype == 0) SDL_snprintf(string[0], STRING_LENGTH, "LV %d", vs_goal);
		else if(wintype == 1) SDL_snprintf(string[0], STRING_LENGTH, "%d LINES", vs_goal / 10);
		else SDL_snprintf(string[0], STRING_LENGTH, "SURVIVAL");
		printFont(15, 19, string[0], 0);

		if(!noitem){
			printFont(1, 21, "ITEMS", 0);

			printFont(3, 22, "1P :", 0);
			if(use_item[0] == 0) SDL_snprintf(string[0], STRING_LENGTH, "ALL");
			else if(use_item[0] == item_num + 1) SDL_snprintf(string[0], STRING_LENGTH, "FEW");
			else if(use_item[0] == item_num + 2) SDL_snprintf(string[0], STRING_LENGTH, "DS");
			else if(use_item[0] == item_num + 3) SDL_snprintf(string[0], STRING_LENGTH, "TGM");
			else SDL_snprintf(string[0], STRING_LENGTH, "%d", use_item[0]);
			printFont(8, 22, string[0], 0);

			printFont(3, 23, "2P :", 0);
			if(use_item[1] == 0) SDL_snprintf(string[0], STRING_LENGTH, "ALL");
			else if(use_item[1] == item_num + 1) SDL_snprintf(string[0], STRING_LENGTH, "FEW");
			else if(use_item[1] == item_num + 2) SDL_snprintf(string[0], STRING_LENGTH, "DS");
			else if(use_item[1] == item_num + 3) SDL_snprintf(string[0], STRING_LENGTH, "TGM");
			else SDL_snprintf(string[0], STRING_LENGTH, "%d", use_item[1]);
			printFont(8, 23, string[0], 0);
		}

	} else if((gameMode[0] != 8)&&(gameMode[0] != 9)) {
		// NORMAL
		if(gameMode[0] != 7) {
			printFont(1, 15, "SCORE       :", 0);
			SDL_snprintf(string[0], STRING_LENGTH,"%d",saveBuf[202]);
			printFont(15, 15, string[0], 0);
		}

		printFont(1, 16, "LEVEL       :", 0);
		SDL_snprintf(string[0], STRING_LENGTH,"%d",saveBuf[203]);
		printFont(15, 16, string[0], 0);

		printFont(1, 17, "LINES       :", 0);
		SDL_snprintf(string[0], STRING_LENGTH,"%d",saveBuf[204]);
		printFont(15, 17, string[0], 0);

		printFont(1, 18, "BIG         :", 0);
		if(IsBigStart[0]) printFont(15, 18, "c", 0);
		else printFont(15, 18, "e", 0);

		printFont(1, 19, "START LEVEL :", 0);
		SDL_snprintf(string[0], STRING_LENGTH,"%d",start[0]);
		printFont(15, 19, string[0], 0);

		printFont(1, 20, "RISE ON/OFF :", 0);
		if((p_shirase[0]) || ((devil_randrise) && (gameMode[0] == 3) && (!devil_minus[0]))) printFont(15, 20, "c", 0);
		else printFont(15, 20, "e", 0);

		printFont(1, 21, "RISE START  :", 0);
		if((devil_randrise) && (gameMode[0] == 3) && (!devil_minus[0]))
			SDL_snprintf(string[0], STRING_LENGTH,"500");
		else SDL_snprintf(string[0], STRING_LENGTH,"%d",level_shirase_start);
		printFont(15, 21, string[0], 0);

		printFont(1, 22, "RISE LINES  :", 0);
		SDL_snprintf(string[0], STRING_LENGTH,"%d",raise_shirase_lines);
		printFont(15, 22, string[0], 0);

		printFont(1, 23, "RISE INTER  :", 0);
		if((devil_randrise) && (gameMode[0] == 3) && (!devil_minus[0]))
			SDL_snprintf(string[0], STRING_LENGTH,"RANDOM BY SECTION");
		else
			SDL_snprintf(string[0], STRING_LENGTH,"%d",raise_shirase_interval);
		printFont(15, 23, string[0], 0);
	} else if(gameMode[0] == 8){	// MISSION
		printFont(1, 15, "FILE NAME.  :", 0);
		if(mission_file == 0)
			SDL_snprintf(string[0], STRING_LENGTH, "BIG ROAD");
		else if(mission_file == 1)
			SDL_snprintf(string[0], STRING_LENGTH, "TRICKY ROAD");
		else if(mission_file == 2)
			SDL_snprintf(string[0], STRING_LENGTH, "GRAND ROAD");
		else if(mission_file == 3)
			SDL_snprintf(string[0], STRING_LENGTH, "STAR ROAD");
		else if(mission_file == 4)
			SDL_snprintf(string[0], STRING_LENGTH, "ANOTHER ROAD");
		else if(mission_file == 5)
			SDL_snprintf(string[0], STRING_LENGTH, "DS ROAD");
		else if(mission_file == 6)
			SDL_snprintf(string[0], STRING_LENGTH, "DEVIL ROAD");
		else if(mission_file <= 16)
			SDL_snprintf(string[0], STRING_LENGTH, "TRIAL S%d", mission_file - 6);
		else if(mission_file == 17)
			SDL_snprintf(string[0], STRING_LENGTH, "TRIAL HM");
		else if(mission_file == 18)
			SDL_snprintf(string[0], STRING_LENGTH, "TRIAL GOD");
		else if(mission_file == 19)
			SDL_snprintf(string[0], STRING_LENGTH, "HEBO AMATEUR");
		else if(mission_file == 20)
			SDL_snprintf(string[0], STRING_LENGTH, "HEBO PRO");
		else if(mission_file == 21)
			SDL_snprintf(string[0], STRING_LENGTH, "HEBO BRONZE");
		else if(mission_file == 22)
			SDL_snprintf(string[0], STRING_LENGTH, "HEBO SILVER ");
		else if(mission_file == 23)
			SDL_snprintf(string[0], STRING_LENGTH, "HEBO GOLD");
		else if(mission_file == 24)
			SDL_snprintf(string[0], STRING_LENGTH, "HEBO PLATINUM");
		else
			SDL_snprintf(string[0], STRING_LENGTH, "NO.%02d", mission_file);
		printFont(15, 15, string[0], 0);

		printFont(1, 16, "START MISSION:", 0);
		SDL_snprintf(string[0], STRING_LENGTH,"%d",start_mission);
		printFont(15, 16, string[0], 0);
	}else if((gameMode[0] == 9) && (saveBuf[260] < 2)){	// STANDARD


		printFont(1, 16, "SPEED     :", 0);
		SDL_snprintf(string[0], STRING_LENGTH,"%d",saveBuf[261]);
		printFont(15, 16, string[0], 0);

		printFont(1, 17, "ARE       :", 0);

		printFont(1, 18, "LINE CLEAR:", 0);

		printFont(1, 19, "LOCK DELAY:", 0);

		printFont(1, 20, "DAS       :", 0);
		for(int32_t k = 0; k <= 3; k++) {
			SDL_snprintf(string[0], STRING_LENGTH,"%4d",(saveBuf[262] >> (k * 8)) & 0xff);
			printFont(15, 17+k, string[0], 0);
		}
		SDL_snprintf(string[0], STRING_LENGTH, "BGM       :%2d", saveBuf[266]);
		printFont(1, 22, string[0], 0);
	}
	/* タイム類 */
	printFont(1, 25, "TIME        :", 0);
	getTime(saveBuf[200]);
	printFont(15, 25, string[0], 0);

	printFont(1, 26, "LENGTH      :", 0);
	SDL_snprintf(string[0], STRING_LENGTH, "%zu", saveBuf[4] / sizeof(int16_t) + 1);
	printFont(15, 26, string[0], 0);

	/* バージョン */
	printFont(1, 28, "VERSION     :", 0);
	SDL_snprintf(string[0], STRING_LENGTH,"%d",repversw);
	printFont(15, 28, string[0], 0);

	// 段位
	if((gameMode[0] <= 2) && (gameMode[0] != 0)){
		printFont(20, 8, "GRADE TYPE :", 0);
		if(repversw < 19) printFont(32, 8, "N/A", 0);
		else if(enable_grade[0] == 0) printFont(33, 8, "NO USE", 0);
		else if(enable_grade[0] == 1) printFont(33, 8, "1", 0);
		else if(enable_grade[0] == 2) printFont(33, 8, "2+", 0);
		else if(enable_grade[0] == 3) printFont(33, 8, "2", 0);
		else if(enable_grade[0] == 4) printFont(33, 8, "3", 0);
	}
	// T-SPIN
	printFont(20, 9, "T-SPIN TYPE:", 0);
	if(repversw < 19) printFont(33, 9, "N/A", 0);
	else if(tspin_type == 0) printFont(32, 9, "NO CHECK", 0);
	else if(tspin_type == 1) printFont(33, 9, "SLIDE", 0);
	else if(tspin_type == 2) printFont(32, 9, "3-CORNER", 0);
	else if(tspin_type == 3) printFont(33, 9, "BOTH", 0);

	if(gameMode[0] != 4){
		printFont(20, 10, "NEXT NUM   :", 0);
		if(repversw < 22) printFont(33, 10, "N/A", 0);
		else if(((death_plus[0]) || (hebo_plus[0])) && (max_hnext[0] >= 1))
			printFont(33, 10, "1", 0);
		else{
			SDL_snprintf(string[0], STRING_LENGTH,"%d",max_hnext[0]);
			printFont(33, 10, string[0], 0);
		}
	}

	printFont(20, 11, "FRAME RATE :", 0);
	if(repversw < 25) printFont(33, 11, "N/A", 0);
	else{
		SDL_snprintf(string[0], STRING_LENGTH,"%2dFPS",saveBuf[239]);
		printFont(33, 11, string[0], 0);
	}
	// IRS
	if(disable_irs){
		printFont(20, 12, "IRS DISABLED", 0);
	}else{
		printFont(20, 12, "IRS STYLE  :", 0);
		if((repversw < 32) || (ace_irs == 0)) printFont(33, 12, "HEBORIS", 0);
		else if(ace_irs == 1) printFont(33, 12, "ACE", 0);
		else if(ace_irs == 2) printFont(33, 12, "ACE+", 0);
	}

	// Back to Back
	printFont(20, 13, "BACK TO BACK:", 0);
	if(b2bcheck) printFont(33, 13, "c", 0);
	else printFont(33, 13, "e", 0);

	// SPAWN Y TYPE
	printFont(20, 14, "SPAWN Y POS:", 0);
	if(spawn_y_type) printFont(33, 14, "21/22", 0);
	else printFont(33, 14, "19/20", 0);

	if(flag < 0 || getPushState(0, INPUT_BUTTON_A) || getPushState(0, INPUT_BUTTON_B)) {
		PlaySE(WAVE_SE_MOVE);
		freeReplayData();
		mainLoopState = MAIN_REPLAY_SELECT;
		init = true;
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  リプレイデータをロード（保存メニュー用）
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
int32_t loadReplayData2(int32_t pl, int32_t number) {
	int32_t tmpBuf[300];

	SDL_memset(tmpBuf, 0, 300 * 4);

	if(number <= 40)
		SDL_snprintf(string[0], STRING_LENGTH, "replay/REPLAY%02d.SAV", number);
	else
		SDL_snprintf(string[0], STRING_LENGTH, "demo/DEMO%02d.SAV", number - 20);

	FILESYSTEM_ReadFile32(string[0], tmpBuf, 300, 0);

	if(tmpBuf[0] != 0x4F424548) return (1);
	if(tmpBuf[1] != 0x20534952) return (1);
	if(tmpBuf[2] != 0x4C504552) return (1);
	if(tmpBuf[3] != 0x31765941) return (1);
//共通
	repdata[0 + pl * 20] = tmpBuf[201];	// gameMode
	repdata[1 + pl * 20] = getModeAddRep2(tmpBuf);	// (modeAdd)
	repdata[2 + pl * 20] = tmpBuf[294];	// rots
	repdata[3 + pl * 20] = tmpBuf[256];	// repversw
	repdata[9 + pl * 20] = tmpBuf[297];	// BIG

//TOMOYO
	if(repdata[0 + pl * 20] == 6){
		repdata[4 + pl * 20] = tmpBuf[202];// stage
		repdata[5 + pl * 20] = tmpBuf[200];// gametime
//VERSUS
	}else if(repdata[0 + pl * 20] == 4){
		repdata[2 + pl * 20] = tmpBuf[218];// 1P_rots
		repdata[4 + pl * 20] = tmpBuf[219];// 2P_rots
		repdata[5 + pl * 20] = tmpBuf[228];// winpoint
		repdata[6 + pl * 20] = tmpBuf[200];// gametime
		repdata[9 + pl * 20] = 0;
//ACE
	}else if(repdata[0 + pl * 20] == 7){
		repdata[4 + pl * 20] = tmpBuf[203];// level
		repdata[5 + pl * 20] = tmpBuf[204];// lines
		repdata[6 + pl * 20] = tmpBuf[200];// gametime
//MISSION
	}else if(repdata[0 + pl * 20] == 8){
		repdata[4 + pl * 20] = tmpBuf[234];// mission_file
		repdata[5 + pl * 20] = tmpBuf[200];// gametime
//SIMPLE
	}else if(repdata[0 + pl * 20] == 9){
		repdata[4 + pl * 20] = tmpBuf[260];// std_opt
		repdata[5 + pl * 20] = tmpBuf[202];// score
		repdata[6 + pl * 20] = tmpBuf[204];// lines
		repdata[7 + pl * 20] = tmpBuf[200];// gametime
//通常
	}else{
		repdata[4 + pl * 20] = tmpBuf[203];// level
		repdata[5 + pl * 20] = tmpBuf[200];// gametime
		repdata[6 + pl * 20] = tmpBuf[202];// score
		repdata[7 + pl * 20] = tmpBuf[204];// lines
	}
	return (0);
}

int32_t getModeAddRep2(int32_t *tmpBuf){
	if(tmpBuf[201] == 0) return tmpBuf[259];
	else if((tmpBuf[201] == 1) || (tmpBuf[201] == 2)) return tmpBuf[232];
	else if(tmpBuf[201] == 3) return tmpBuf[254];
	else if(tmpBuf[201] == 6) return (tmpBuf[216] >= 27)+(tmpBuf[216] >= 45)+(tmpBuf[216] >= 69)+(tmpBuf[216] >= 100);
	else if(tmpBuf[201] == 7) return tmpBuf[225];
	else if(tmpBuf[201] == 9){
		if(tmpBuf[267]) return 4;
		else return tmpBuf[260];
	}else if(tmpBuf[201] == 10) return tmpBuf[296];
	else return 0;
}
