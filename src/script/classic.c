#include "../script/include.h"

int32_t		blkDataX[7 * 4 * 4] =	// ユニットを構成するブロックの相対Ｘ位置(7種類/ 4角度/ 4ブロック)
{
	0, 1, 2, 3, 		2, 2, 2, 2, 		3, 2, 1, 0, 		2, 2, 2, 2,
	1, 0, 0, 2, 		1, 0, 1, 1, 		1, 2, 2, 0, 		1, 2, 1, 1,
	1, 2, 2, 1, 		2, 2, 1, 1, 		2, 1, 1, 2, 		1, 1, 2, 2,
	1, 0, 1, 2, 		2, 2, 1, 1, 		1, 2, 1, 0, 		1, 1, 2, 2,
	1, 0, 1, 2, 		1, 1, 0, 1, 		1, 2, 1, 0, 		1, 1, 2, 1,
	1, 0, 2, 2, 		1, 1, 1, 0, 		1, 2, 0, 0, 		1, 1, 1, 2,
	1, 2, 1, 0, 		1, 1, 0, 0, 		1, 0, 1, 2, 		0, 0, 1, 1,
};

int32_t		blkDataY[7 * 4 * 4] =	// ユニットを構成するブロックの相対Ｙ位置(7種類/ 4角度/ 4ブロック)
{
	1, 1, 1, 1, 		0, 1, 2, 3, 		1, 1, 1, 1, 		3, 2, 1, 0,
	1, 2, 1, 1, 		1, 0, 0, 2, 		2, 1, 2, 2, 		1, 2, 2, 0,
	1, 1, 2, 2, 		1, 2, 2, 1, 		2, 2, 1, 1, 		2, 1, 1, 2,
	1, 1, 2, 2, 		1, 0, 1, 2, 		2, 2, 1, 1, 		1, 2, 1, 0,
	1, 1, 2, 1, 		1, 0, 1, 2, 		2, 2, 1, 2, 		1, 2, 1, 0,
	1, 1, 1, 2, 		1, 0, 2, 2, 		2, 2, 2, 1, 		1, 2, 0, 0,
	1, 1, 2, 2, 		1, 2, 1, 0, 		2, 2, 1, 1, 		1, 0, 1, 2,
};

// クラシックルールの色 #1.60c7j1
// 9色だと、ccol[9]が不正な値になっているので10色に修正#1.60c7k6
//int32_t	ccol[20] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
int32_t	ccol[7] = {1,2,3,4,5,6,7};

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  ステータスNo.05 - ブロック移動中
//  ※この辺の操作部分以外を修正する場合は、world.cも修正してください。
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void statCMove(int32_t player) {
	int32_t		i, bak, bottom, move, don, flash, kosa, rolling,k,synchro,lockT, fsmooth;

	fsmooth = 0;
	don = 0;
	synchro = 0;
	hiddenProc(player);

	if(!statusc[player * 10 + 2]) {
		padRepeat(player);

		// Hold
		if((repversw >= 64) && (!statusc[player * 10 + 6])){
			doHold(player, 0);
			// HOLDしてゲームオーバーになった場合はここで止める #1.60c7m2
			if(status[player] == 7) return;
		}

		bs[player] = bs[player] + sp[player];
		lockT = wait3[player];
		if(isboost[player]){
			bs[player] = 1200;
			if((sp[player] >= 1200) && (gameMode[player] != 6))
				lockT = wait3[player] / 2;
		}

		// spawn_y_typeが1の場合の20G #1.60c7s2
		if( (spawn_y_type) && (sp[player] >= 1200) ) bs[player] = 1320;

		// Hold
		if(repversw < 35){
			doHold(player, 0);
			// HOLDしてゲームオーバーになった場合はここで止める #1.60c7m2
			if(status[player] == 7) return;
		}

		// ロールロール処理 #1.60c7j5
		rolling = 0;
		if(isrollroll[player]) {
			// 回るタイミングを「ステージタイムが30フレームで割り切れる」時に変更 #1.60c7m5
			if((ending[player] == 2) && (repversw >= 44)){
				/* エンディング・ロール */
				if( edrec[player] % p_rollroll_timer == 0 )
					rolling = 1;
			} else if( gameMode[player] == 6 ) {
				/* TOMOYO */
				//ステージ20以降（ステージタイム無し）
				if((repversw >= 44) && (stage[player] > 19) && (ltime[player] % p_rollroll_timer == 0))
					rolling = 1;
				//ステージ19まで
				else if( stime[player] % p_rollroll_timer == 0 )
					rolling = 1;
			} else if( (gameMode[player] >= 4) || (item_mode[player]) ) {
				/* VERSUS / PRACTICE / MISSION */
				if( time[player] % p_rollroll_timer  == 0 )
					rolling = 1;
			}
		}

		// 回転!!
		if((statusc[player * 10 + 6] != 0) && (repversw >= 59))
			move = 0;
		else
			move = (getPushState(player, 5) || rolling) - (getPushState(player, 4) || getPushState(player, 6));
		if((move) && (isrotatelock[player] == 0)) {
			bak = (rt[player] + move);
				if(istrance[player]){
					do{
						k = gameRand(7,player);
						setNextBlockColors(player, 1);
					}while(k == blk[player]);
				}else{
					k = blk[player];
				}
			if(bak < 0) bak = 3;
			if(bak > 3) bak = 0;

			if(judgeBlock(player, bx[player], by[player], k, bak) == 0) {
				// 地形に重ならずに回転できる場合
				if(rotate_snd) PlaySE(4);	// 回転音設定#1.60c7f7
				rt[player] = bak;
				blk[player] = k;
				synchro = 1;
				if(istrance[player])
					c_cblk_r[player] = ccol[k];

				// 回転させた回数+1 #1.60c7s6
				statusc[player * 10 + 5]++;

				// T-SPIN権利獲得 #1.60c7n6
				if( judgeBlock(player, bx[player], by[player] + 1, blk[player], rt[player]) != 0 ) {
					if((blk[player] == 4)&&(tspin_type >0) ) tspin_flag[player] = 1;
				}
			} else if( (blk[player] != 0) && (!disable_wallkick) && (heboGB[player]==0) ) {	// ブロックが棒以外のとき
				// 回転できないときは、左右に動かしてみよう
				move = 0;

				// BIGの場合は2マス分調べる #1.60c7o9
				for(i = 0; i <= IsBig[player]; i++) {
					// 重なっている部分が真中の列だけではなければ
					if( judgeBlockRotate(player, bx[player], by[player], k, bak) )
					{
						if(judgeBlock(player, bx[player] - 1 - i, by[player], k, bak) == 0)
							move = -1 -i;
						if(judgeBlock(player, bx[player] + 1 + i, by[player], k, bak) == 0)
							move = 1 + i;

						if(move) {
							if(rotate_snd) PlaySE(4);	// 回転音設定#1.60c7f7
							bx[player] = bx[player] + move;
							rt[player] = bak;
							blk[player] = k;
							synchro = 1;
							if(istrance[player])
								c_cblk_r[player] = ccol[k];

							// 回転させた回数+1 #1.60c7s6
							statusc[player * 10 + 5]++;

							// T-SPIN権利獲得 #1.60c7n6
							if( judgeBlock(player, bx[player], by[player] + 1, blk[player], rt[player]) != 0 ) {
								if((blk[player] == 4)&&(tspin_type >0) ) tspin_flag[player] = 1;
							}
							break;
						// Ti風水色床蹴り 床蹴りは1回だけ可能 #1.60c3
						} else if((blk[player] == 4) && (rots[player] == 1) && (kickc[player] == 0)) {
							if((bak == 2) && (judgeBlock(player, bx[player], by[player] -1 -i, k, bak) == 0)) {
								if(rotate_snd) PlaySE(4);	// 回転音設定#1.60c7f7
								by[player] = by[player] -1 -i;
								rt[player] = bak;
								blk[player] = k;
								synchro = 1;
								if(istrance[player])
									c_cblk_r[player] = wcol[k];

								// 回転させた回数+1 #1.60c7s6
								statusc[player * 10 + 5]++;

								kickc[player]++;

								// T-SPIN権利獲得 #1.60c7n6
								if( judgeBlock(player, bx[player], by[player] + 1, blk[player], rt[player]) != 0 ) {
									if((blk[player] == 4)&&(tspin_type >0) ) tspin_flag[player] = 1;
								}
								break;
							}
						}
					}
				}
			} else if((!disable_wallkick) && (heboGB[player]==0)) {
				// Ti風赤壁･床蹴り (とりあえず床蹴りは縦、壁蹴りは横になる時だけ) #1.60c3
				if(rots[player] == 1) {
					move = 0;
					// BIGの場合は4マス分調べる #1.60c7m6
					for(i = 0; i <= IsBig[player] * 3; i++) {
						if((bak == 0) || (bak == 2)) {	// 横になる時
							if(judgeBlock(player, bx[player] -1 -i, by[player], k, bak) == 0)
								move = -1 -i;
							else if(judgeBlock(player, bx[player] + 1 +i, by[player], k, bak) == 0)
								move = 1 + i;
							else if(judgeBlock(player, bx[player] + 2 +i, by[player], k, bak) == 0)
								move = 2 + i;

							if(move) {
								if(rotate_snd) PlaySE(4);	// 回転音設定#1.60c7f7
								bx[player] = bx[player] + move;
								rt[player] = bak;
								blk[player] = k;
								synchro = 1;
								if(istrance[player])
									c_cblk_r[player] = ccol[k];

								// 回転させた回数+1 #1.60c7s6
								statusc[player * 10 + 5]++;

								break;
							}
						} else if(kickc[player] == 0) {	// 縦になる時
							if(judgeBlock(player, bx[player], by[player] -1 -i, k, bak) == 0)
								move = -1 -i;
							else if(judgeBlock(player, bx[player], by[player] -2 -i, k, bak) == 0)
								move = -2 -i;

							// 接地していない場合は蹴らない #1.60c7k5
							if( (judgeBlock(player,bx[player],by[player]+1+i,k,rt[player]) == 0) && (repversw >= 8) )
								move = 0;

							if(move) {
								if(rotate_snd) PlaySE(4);	// 回転音設定#1.60c7f7
								by[player] = by[player] + move;
								rt[player] = bak;
								blk[player] = k;
								synchro = 1;
								if(istrance[player])
									c_cblk_r[player] = ccol[k];
								// 回転させた回数+1 #1.60c7s6
								statusc[player * 10 + 5]++;

								kickc[player]++;
								break;
							}
						}
					}

				}
			}
		}

		// 左右に移動
		move = getPressState(player, 3) - getPressState(player, 2);
		if(isLRreverse[player]) move = 0 - move;
		if(move && (statusc[player * 10 + 4])) {
			if((block_spectrum) && (heboGB[player]==0))
			{
				/* スムーズ時の表示異常を修正 via C++ Port */
				if ( smooth && (sp[player] < 60) )
				{
					// drawCBlock (player, blk[player] + 1, 5, bs[player] * 8 / 60, 0, 1);
				}
				else
				{
					drawCBlock (player, blk[player] + 1, 5, 0, 0, 1);
				}
			}
			// BIG時、2マス単位モードの場合は移動量を2倍する #1.60c7m6
			if(IsBig[player] && BigMove[player]) move = move * 2;

			if((mpc[player] == 1) || (mpc[player] >= waitt[player]) || ((isremote[player]) && (gameMode[player] != 4)))
				if(judgeBlock(player, bx[player] + move, by[player], blk[player], rt[player]) == 0) {
					bx[player] = bx[player] + move;
					if(movesound) PlaySE(5);
					if(!synchro) tspin_flag[player] = 0;	// T-SPIN権利剥奪 #1.60c7n6
				}
		}
		synchro = 0;
//		statusc[player * 10 + 4] = 1; 				 // 最初の1フレームは移動させないゾ

		if(!nanameallow) move = 0;

		// ↓を離している、またはdowntypeが0の場合、下入れ制限解除#1.60c7f9
		if((!getPressState(player, 1-(1*(isUDreverse[player])))) || (!downtype)) down_flag[player] = 0;
		if((!getPressState(player, 0+(1*(isUDreverse[player])))) || (!downtype) || (repversw < 12)) up_flag[player] = 0;

		// 高速落下!! 下入れ制限改造#1.60c7f9
		if((!move) && getPressState(player, 1-(1*(isUDreverse[player]))) && (sp[player] < 60) && (!down_flag[player]) && ((statusc[player * 10 + 4]) || (repversw < 27))) {
			bs[player] = 61;
			if((gameMode[player]>=9)||((gameMode[player]==6)&&(anothermode[player]==4))||((gameMode[player]==7)&&(anothermode[player]==3))){
				sc[player]++;//直入れ
			}else{
				qdrop[player]++;	//遅い分のドロップボーナス半減は、スコア計算時に
			}
		}
		if((ismagnetblock[player]) && (repversw >= 41)) bs[player] = bs[player] + 30;

		for(bottom = by[player]; judgeBlock(player, bx[player], bottom, blk[player], rt[player]) == 0; bottom++);

		// 地面にくっついた
		if((judgeBlock(player, bx[player], by[player] + 1, blk[player], rt[player]) != 0)) {

			if(ismagnetblock[player] == 1)
				bk[player] = 100;

			bs[player] = 0;
			if((bk[player] == 0) && (smooth)) {
				bs[player] = 0;
				don = 1;
			}

			if(lockT != 99) // 接着99処理 #1.60c7
				bk[player]++;
			else
				bk[player] = 98;

			// 下入れ制限#1.60c7f9
			// 下入れ即接着 #1.60c7k8
			if((getPressState(player, 1-(1*(isUDreverse[player])))) && (!down_flag[player])
						&& (statusc[player * 10 + 4] || (repversw < 27)) && (!move || nanamedown)) {
				if (repversw < 9)
					bk[player] = bk[player] + 8;
				else
					bk[player] = 100;
            }

			// 固定
			if(bk[player] > lockT) {
				if(heboGB[player]==0)
					PlaySE(3);
				else if(heboGB[player] == 1)
					PlaySE(1);
				kickc[player] = 0;	// 永久床蹴り防止用 #1.60c3
				statusc[player * 10 + 2]++;
				drawCBlock(player, 0, 0, 0, 10, 0);
				// 下入れ制限#1.60c7f9
				if((getPressState(player, 1)) && (downtype) && (!move || nanamedown)) down_flag[player] = 1;
				if(repversw >= 57) goto lockflash;	// SUPERなSKIP
				return;
			}
		} else {
			// T.L.S. (Temporary Landing System)  (c)1998 AR●KA(^^;
			// 別関数に独立 #1.60c7k4
			drawTLSBlock(player, bottom);


			// 上キーで一瞬で落下
			// 上入れ制限追加 #1.60c7p1
			if(getPressState(player, 0+(1*(isUDreverse[player]))) && (!move) && (!sonicdrop) && (!up_flag[player]) && (heboGB[player]==0) && ((statusc[player * 10 + 4]) || (repversw < 27))) {
				if((sp[player] <= 240) && (!ending[player])) {
					if(gameMode[player]==10){
						sc[player] = sc[player] + ( bottom - by[player] - 1 ) * 23 / 20;
					}else if((gameMode[player]>=9)||((gameMode[player]==6)&&(anothermode[player]==4))||((gameMode[player]==7)&&(anothermode[player]==3))){
						sc[player] =sc[player]+ (bottom -by[player] -1) *2;//直入れ
					}else{
						sdrop[player] = (bottom -by[player] -1) *2;//一旦sdropへ入れる
					}
				}
				if(!block_spectrum){
					by[player] = bottom - 1;
				}else{	//残像を描画
					/* 残像開始位置をスムーズ地点にする via C++ Port */
					fsmooth = 0;
					if ( smooth && (sp[player] < 60) )
					{
						/* 1フレームだけ */
						fsmooth = bs[player] * 8 / 60;
					}

					while(by[player] != bottom - 1){
						drawCBlock (player, blk[player] + 1, 4, fsmooth, 0, 1);
						by[player]++;
						fsmooth = 0;
					}
				}
				bs[player] = 0;
				bk[player] = 0;
				if(!smooth) don = 1;
				if((downtype) && (repversw < 54)) up_flag[player] = 1;
				statusc[player * 10 + 5] = 0;	// 回転させた数リセット #1.60c7s6
			}
		}

		// ブロックが落ちる
		while(bs[player] > 60) {
			bk[player] = 0;
			tspin_flag[player] = 0;	// T-SPIN権利剥奪 #1.60c7o9

			bs[player] = bs[player] - 60;

			// BIG時、2マス単位モードの場合は落ちる量を2倍する #1.60c7m6
			// 3マス離れていても接地してしまうのを修正 #1.60c7o9
			if( (judgeBlock(player, bx[player], by[player] + 1, blk[player], rt[player]) == 0)
			 || (judgeBlock(player, bx[player], by[player] + 1 + (IsBig[player] && BigMove[player] && !ybigtype), blk[player], rt[player]) == 0) )
			{
				if((block_spectrum) && ((!smooth) || (bs[player] >= 60)) && (heboGB[player]==0))	//残像を描画
					drawCBlock (player, blk[player] + 1, 5, 0 , 0, 1);
				by[player] = by[player] + 1 + (IsBig[player] && BigMove[player] && !ybigtype);
			}

			// 地面にくっついた
			if(judgeBlock(player, bx[player], by[player] + 1, blk[player], rt[player])) {
				by[player] = bottom - 1;
				bs[player] = 0;
				if(!smooth) don = 1;

				// 下キーで即固定 #1.60c7r3
				if((getPressState(player, 1-(1*(isUDreverse[player])))) && (!down_flag[player]) && (repversw >= 15)
							&& (statusc[player * 10 + 4] || (repversw < 27)) && (!move || nanamedown)) {
					PlaySE(1);
					if(heboGB[player]==0)
						PlaySE(3);
					kickc[player] = 0;
					statusc[player * 10 + 2]++;
					drawCBlock(player, 0, 0, 0, 10, 0);
					if(downtype) down_flag[player] = 1;	// 下入れ制限
					if(repversw >= 57) goto lockflash;	// SUPERなSKIP
					return;
				}
			} else {
				// 回転させた数リセット #1.60c7s6
				statusc[player * 10 + 5] = 0;
			}
		}

		statusc[player * 10 + 4] = 1; 				 // 最初の1フレームは移動させないゾ

		if((don) && (heboGB[player]!=1)) PlaySE(1);

		// ブロックを描く
		flash = (count % 2) * (blockflash == 0) + (blockflash == 1);
		kosa = bk[player] * 7 / (lockT + (lockT == 0));
		if(kosa > 6) kosa = 6;
		if(smooth && (sp[player] < 60))
			drawCBlock(player, blk[player] + 1, kosa, bs[player] * 8 / 60, flash, 0);
		else
			drawCBlock(player, blk[player] + 1, kosa, 0, flash, 0);

		// Hold
		if((repversw >= 35) && (repversw < 64) && (!statusc[player * 10 + 2]) && (!statusc[player * 10 + 6])){
			doHold(player, 0);
			// HOLDしてゲームオーバーになった場合はここで止める #1.60c7m2
			if(status[player] == 7) return;
		}

		statusc[player * 10 + 6] = 0;
		if((repversw >= 59) && (statusc[player * 10 + 2] != 0)) goto lockflash;	// SUPERなSKIP
	} else {
		lockflash:
		statusc[player * 10 + 2]++;
		if((c_nblk[0 + player * 6] != 8) && (c_nblk[0 + player * 6] >= 0))drawCBlock (player, 0, 0, 0, 10, 0);

		// ↓を離している場合、下入れ制限解除#1.60c7n6
		if((!getPressState(player, 1-(1*(isUDreverse[player])))) && (repversw >= 10)) down_flag[player] = 0;
		else if((downtype) && (repversw >= 54)) down_flag[player] = 1;
		if(!getPressState(player, 0+(1*(isUDreverse[player])))) up_flag[player] = 0;
		else if((downtype) && (repversw >= 54)) up_flag[player] = 1;

		if(statusc[player * 10 + 2] > 2 - (repversw >= 9)) {	// 接着時の灰色化を1フレ短縮 #1.60c7k8
			setBlock(player, bx[player], by[player], blk[player], rt[player]);
			if(status[player] == 7) return;

			ndelay[player] = 1;

			lc[player]++;

			// T-SPIN獲得 #1.60c7s6
			//すでにtspin_flagは立ってる
			if((tspin_flag[player]) && (blk[player] == 4) && ((tspin_type>=2) || (gameMode[player] == 8)) && (statusc[player * 10 + 5] > 0) && (isTSpin(player) >= 3) ) {
				PlaySE(34);
				tspin_c[player] = 120;
				last_tspin[player] = 0;
				tspin_flag[player] = 2;//3コーナーのT-spin
			} else if((tspin_type==2) || (gameMode[player] == 8)){
				tspin_flag[player] = 0;
			}

			// TGMLV上昇はstatBlockに移転しました#1.60c7i8
			if(ending[player] == 0) {
				// ブロックを着床させた回数 #1.60c7i2
				bdowncnt[player]++;
				st_bdowncnt[player]++;
				if((gameMode[player] == 3) && (devil_nextrise[player] >= 0) && (devil_randrise)) {
					devil_nextrise[player]--;
				}
			}

			// エンディング中の花火発生 #1.60c7s8
			if( (ending[player] == 2) && (gameMode[player] == 0) && (repversw <= 37)) {
				hanabi_total[player]++;
				hanabi_waiting[player]++;
			}
			// ACEカスタム式IRS
			if((ace_irs == 2) && (repversw >= 35)) doIRS2plus(player);
			// ブロック消去判断により1フレ短縮 #1.60c7k8
			if ((blockEraseJudge(player)) || (repversw < 9)) {
				status[player] = 8;
				statusc[player * 10] = 0;
				statusc[player * 10 + 1] = 0;
				statusc[player * 10 + 2] = 0;
				statusc[player * 10 + 3] = 0;
				statusc[player * 10 + 4] = 0;
				statusc[player * 10 + 5] = 0;
				statusc[player * 10 + 6] = 0;
				//ステータス変更と同時に次のステータス関数を実行する事で1フレ短縮　C7V2.1
				if(repversw >= 54) statEraseBlock(player);
			} else {
				status[player] = 6;
				statusc[player * 10] = wait1[player];
				statusc[player * 10 + 1] = 15;
				statusc[player * 10 + 2] = 0;
				statusc[player * 10 + 3] = 2;
				statusc[player * 10 + 4] = 0;
				statusc[player * 10 + 5] = 0;
				statusc[player * 10 + 6] = 0;

				cmbpts[player] = 0;
				combo[player] = 0;
				combo2[player] = 0;
				hanabi_combo[player] = 0;
				hebocombo[player] = 0;
				scrate[player] = 100;//oriモード用のスコアレート
				pinchCheck(player);
				//ステータス変更と同時に次のステータス関数を実行する事で1フレ短縮　C7V2.1
				if(repversw >= 54) statErase(player);
			}

			return;
		}
	}

	effect(player);
}

/* 壁蹴りできるか判定（中央だけ重なる場合は回転不可） */
// 戻り値が-1のときは蹴れる
int32_t judgeBlockRotate(int32_t player, int32_t bx1, int32_t by1, int32_t kind, int32_t rotate) {
	int32_t		i, xx, bx2, by2;

	if(IsBig[player]) {
		// BIGの場合は専用の判定処理を行う #1.60c7m6
		return judgeBigBlockRotate(player, bx1, by1, kind, rotate);
	}

	for(i = 0; i < 4; i++) {
		xx = blkDataX[kind * 16 + rotate * 4 + i];
		bx2 = (bx1 + blkDataX[kind * 16 + rotate * 4 + i]);
		by2 = (by1 + blkDataY[kind * 16 + rotate * 4 + i]);

		// 中央でない場合は重なっているか判定
		if(xx != 1) {
			if( ((bx2 < 0) || (bx2 >= fldsizew[player]) || (by2 > fldsizeh[player]))
			 || ((fld[bx2 + by2 * fldsizew[player] + player * 210] != 0) && (by2 >= 0)) ) {
				return -1;
			}
		}
	}

	return 0;
}

/* BIG用壁蹴り判定 #1.60c7m6 */
// 戻り値が-1のときは蹴れる
int32_t judgeBigBlockRotate(int32_t player, int32_t bx1, int32_t by1, int32_t kind, int32_t rotate) {
	int32_t		i, bx2, by2, xx;
	int32_t		k, l, bx3, by3;

	for(i = 0; i < 4; i++) {
		xx = blkDataX[kind * 16 + rotate * 4 + i];
		bx2 = (bx1 + xx * 2);
		by2 = (by1 + blkDataY[kind * 16 + rotate * 4 + i] * 2);

		// 中央でない場合
		if( xx != 1 ) {
			/* 4ブロック分調べる */
			for(k = 0; k < 2; k++)for(l = 0; l < 2; l++) {
				bx3 = bx2 + k;
				by3 = by2 + l;

				if( ((bx3 < 0) || (bx3 >= fldsizew[player]) || (by3 > fldsizeh[player])) ||
				    ((fld[bx3 + by3 * fldsizew[player] + player * 210] != 0) && (by3 >= 0)) ) {
					return -1;
				}
			}
		}
	}

	return 0;
}
