//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  コンピュータの思考ルーチン
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲

#include "../script/include.h"

// 思考ルーチン内で一時使用する変数
int32_t		cp_fld[10 * 22 * 2];	// 仮想フィールド x + y * 10
int32_t		cp_erase[22 * 2];

/* ブロックのあたり判定 */
int32_t cpu_judgeBlock(int32_t player, int32_t bx1, int32_t by1, int32_t kind, int32_t rotate) {
	int32_t		i, bx2, by2;

	if(IsBig[player]) {
		// BIGの場合は専用の判定処理を行う
		return cpu_judgeBigBlock(player, bx1, by1, kind, rotate);
	}
	for(i = 0; i < 4; i++) {
		if(rots[player] == 8) {
			bx2 = (bx1 + blkDDataX[kind * 16 + rotate * 4 + i]);
			by2 = (by1 + blkDDataY[kind * 16 + rotate * 4 + i]);
		} else if( isWRule(player) ) {
			bx2 = (bx1 + blkWDataX[kind * 16 + rotate * 4 + i]);
			by2 = (by1 + blkWDataY[kind * 16 + rotate * 4 + i]);
		} else {
			bx2 = (bx1 + blkDataX[kind * 16 + rotate * 4 + i]);
			by2 = (by1 + blkDataY[kind * 16 + rotate * 4 + i]);
		}
		if(((bx2 < 0) || (bx2 >= fldsizew[player]) ||
			(by2 > fldsizeh[player])) || ((cp_fld[bx2 + by2 * fldsizew[player] + player * 220] != 0) && (by2 >= 0))) {
			return -1;
		}
	}
	return 0;
}

/* ブロックを設置する */
int32_t cpu_setBlock(int32_t player, int32_t bx1, int32_t by1, int32_t kind, int32_t rotate) {
	int32_t		i, bx2, by2,puted;
	puted = 0;

	if(IsBig[player]) {
		// BIGの場合は専用の設置処理を行う
		return cpu_setBigBlock(player, bx1, by1, kind, rotate);
	}
	for(i = 0; i < 4; i++) {
		if(rots[player] == 8) {
			bx2 = (bx1 + blkDDataX[kind * 16 + rotate * 4 + i]);
			by2 = (by1 + blkDDataY[kind * 16 + rotate * 4 + i]);
		} else if( isWRule(player) ) {
			bx2 = (bx1 + blkWDataX[kind * 16 + rotate * 4 + i]);
			by2 = (by1 + blkWDataY[kind * 16 + rotate * 4 + i]);
		} else {
			bx2 = (bx1 + blkDataX[kind * 16 + rotate * 4 + i]);
			by2 = (by1 + blkDataY[kind * 16 + rotate * 4 + i]);
		}
		if((bx2 >= 0) && (bx2 < fldsizew[player]) && (by2 >= 0+(1*((repversw <= 27) || (!spawn_y_type)))) && (by2 <= fldsizeh[player])) {
			if(by2 >= 2)puted++;
			cp_fld[bx2 + by2 * fldsizew[player] + player * 220] = c_cblk_r[player] + 1;
		}
	}
	return puted;
}

/* BIG用当たり判定 */
int32_t cpu_judgeBigBlock(int32_t player, int32_t bx1, int32_t by1, int32_t kind, int32_t rotate) {
	int32_t		i, bx2, by2;
	int32_t		k, l, bx3, by3;

	for(i = 0; i < 4; i++) {
		if(rots[player] == 8) {
			bx2 = (bx1 + blkDDataX[kind * 16 + rotate * 4 + i] * 2);
			by2 = (by1 + blkDDataY[kind * 16 + rotate * 4 + i] * 2);
		} else if( isWRule(player) ) {
			bx2 = (bx1 + blkWDataX[kind * 16 + rotate * 4 + i] * 2);
			by2 = (by1 + blkWDataY[kind * 16 + rotate * 4 + i] * 2);
		} else {
			bx2 = (bx1 + blkDataX[kind * 16 + rotate * 4 + i] * 2);
			by2 = (by1 + blkDataY[kind * 16 + rotate * 4 + i] * 2);
		}

		/* 4ブロック分調べる */
		for(k = 0; k < 2; k++)for(l = 0; l < 2; l++) {
			bx3 = bx2 + k;
			by3 = by2 + l;

			if( ((bx3 < 0) || (bx3 >= fldsizew[player]) || (by3 > fldsizeh[player])) ||
			    ((cp_fld[bx3 + by3 * fldsizew[player] + player * 220] != 0) && (by3 >= 0)) ) {
				return -1;
			}
		}
	}
	return 0;
}

/* BIG用設置処理 */
int32_t cpu_setBigBlock(int32_t player, int32_t bx1, int32_t by1, int32_t kind, int32_t rotate) {
	int32_t		i, bx2, by2,puted;
	int32_t		k, l, bx3, by3;
	puted = 0;

	for(i = 0; i < 4; i++) {
		if(rots[player] == 8) {
			bx2 = (bx1 + blkDDataX[kind * 16 + rotate * 4 + i] * 2);
			by2 = (by1 + blkDDataY[kind * 16 + rotate * 4 + i] * 2);
		} else if( isWRule(player) ) {
			bx2 = (bx1 + blkWDataX[kind * 16 + rotate * 4 + i] * 2);
			by2 = (by1 + blkWDataY[kind * 16 + rotate * 4 + i] * 2);
		} else {
			bx2 = (bx1 + blkDataX[kind * 16 + rotate * 4 + i] * 2);
			by2 = (by1 + blkDataY[kind * 16 + rotate * 4 + i] * 2);
		}
		/* 4ブロック分設置する */
		for(k = 0; k < 2; k++)for(l = 0; l < 2; l++) {
			bx3 = bx2 + k;
			by3 = by2 + l;

			if( (bx3 >= 0) && (bx3 < fldsizew[player]) && (by3 >= 0) && (by3 <= fldsizeh[player]) ) {
				cp_fld[bx3 + by3 * fldsizew[player]+ player * 220] = c_cblk_r[player] + 1;
				puted++;
			}
		}
	}
	return puted;
}

/* 目的の場所へブロックを移動させる */
void cpuMove(int32_t player) {
	int32_t i, slow;

	// キー入力初期化
	for(i=0;i<10;i++) {
		cp_input[i + player * 10] = 0;
	}

	slow = (((rots[player] == 6) || (rots[player] == 8)) && (sp[player] < 40)) * (gameMode[player] == 4);
	slow = slow + 2 * ((isfmirror[player]) || (isxray[player]) || (iscolor[player]) || (item_monochrome[player]) ||
					   (ishidenext[player]) || (isdark[player]) || (isfakenext[player]) || (ismiss[player]));


	if(cp_rot_c[player] > 0) cp_rot_c[player]--;

	// 移動
	if( (status[player] == 4) || (status[player] == 5) ) {
		if(cp_hold[player]) {
			// HOLD
			if(istrance[player]) cp_input[4 + player * 10] = 1;
			else cp_input[7 + player * 10] = 1;
			cp_hold[player] = 0;
		} else if( (rt[player] != cp_rt[player]) && (blk[player] != 2) && ((!isrollroll[player]) || (sp[player] >= 30))) {
			// 回転
			cp_input[4 + player * 10] = 1;
		} else {
			// 右へ移動
			if(bx[player] < cp_x[player]) {
				if(count%(2+(4*slow)) == 0)
					cp_input[3-(1*((isLRreverse[player]) && (item_timer[player] < 360))) + player * 10] = 1;

				// 右に壁がある
				if(judgeBlock(player, bx[player]+1, by[player], blk[player], rt[player]) != 0) {
					if((isWRule(player)) && (blk[player] != 2)){
						cp_input[5 + player * 10] = 0;
						cp_input[4 + player * 10] = 0;
						if((cp_rot_c[player] == 0) || (isrollroll[player])){
							if((blk[player] == 3) || (blk[player] == 6))
								cp_input[5 -(1*(cp_rot_c[player] > 10))+ player * 10] = 1;
							else
								cp_input[4 +(1*(cp_rot_c[player] > 10))+ player * 10] = 1;
							cp_rot_c[player] = 10;
						}
					// 置くのは無理なので落とす
					}else if((dhold[player] == 0) && (!item[player])){
						cp_hold[player] = 1;
					}else{
						cp_input[3 + player * 10] = 0;
						cpuDrop(player);
					}
				}else{
			//		cp_rot_c[player] = 0;
				}
			}
			// 左へ移動
			else if(bx[player] > cp_x[player]) {
				if(count%(2+(4*slow)) == 0)
					cp_input[2+(1*((isLRreverse[player]) && (item_timer[player] < 360))) + player * 10] = 1;

				// 左に壁がある
				if(judgeBlock(player, bx[player]-1, by[player], blk[player], rt[player]) != 0) {
					if((isWRule(player)) && (blk[player] != 2)){
						cp_input[5 + player * 10] = 0;
						cp_input[4 + player * 10] = 0;
						if((cp_rot_c[player] == 0) || (isrollroll[player])){
							if((blk[player] == 3) || (blk[player] == 6))
								cp_input[4 -(1*(cp_rot_c[player] > 10))+ player * 10] = 1;
							else
								cp_input[5 +(1*(cp_rot_c[player] > 10))+ player * 10] = 1;
							cp_rot_c[player] = 10;
						}
					// 置くのは無理なので落とす
					}else if((dhold[player] == 0) && (!item[player])){
						cp_hold[player] = 1;
					}else{
						cp_input[3 + player * 10] = 0;
						cpuDrop(player);
					}
				}else{
			//		cp_rot_c[player] = 0;
				}
			}
			// 設置
			else if((bx[player] == cp_x[player]) && (!isrotatelock[player]) && ((rt[player] == cp_rt[player]) || (blk[player] == 2))) {
				if(istrance[player])
					cpuCheckBestSpot(player);
				if((bx[player] == cp_x[player]) && ((rt[player] == cp_rt[player]) || (blk[player] == 2))){
					cpuDrop(player);
				}
			}
		}
	}
}

void cpuDrop(int32_t player){

	if(isUDreverse[player]){	//上下逆転時
		if((sonicdrop) || (heboGB[player] == 0) &&
			((rots[player] <= 1) || (rots[player] == 5) || (rots[player] == 7))){
			if(!up_flag[player]){
				cp_input[0 + player * 10] = 0;
				cp_input[1 + player * 10] = 1;
			}
		}else{
			if(!down_flag[player]){
				cp_input[0 + player * 10] = 1;
				cp_input[1 + player * 10] = 0;
			}
		}
	}else{	// 通常時
		if((!up_flag[player]) && (!sonicdrop) && (heboGB[player] == 0) &&
			((rots[player] == 2) || (rots[player] == 3) || (rots[player] == 4) || (rots[player] == 6) || (rots[player] == 8))){
			cp_input[0 + player * 10] = 1;
			cp_input[1 + player * 10] = 0;
		}else{
			if((sonicdrop) || (heboGB[player] != 0) ||
				(judgeBlock(player, bx[player], by[player] + 1, blk[player], rt[player]) != 0)){	// 地面にくっついた
				if(!down_flag[player]){
					cp_input[0 + player * 10] = 0;
					cp_input[1 + player * 10] = 1;
				}
			}else{
				if(!up_flag[player]){
					cp_input[0 + player * 10] = 1;
					cp_input[1 + player * 10] = 0;
				}
			}
		}
	}
}

/* ブロックの総数を調べる */
int32_t cpuBlockHowManyFilled(int32_t player, int32_t y) {
	int32_t		j, ret;
	ret = 0;

	for(j = 0; j < fldsizew[player]; j++)
		if(cp_fld[j + y * fldsizew[player] + player * 220]) {
			ret++;
		}
	return ret;
}

// 指定した行で左端から連続しているブロックの数を求める
int32_t blockHowManyFilledFromLeft(int32_t player, int32_t y) {
	int32_t		j, ret;
	ret = 0;

	for(j = 0; j < fldsizew[player]; j++)
		if(!cp_fld[j + y * fldsizew[player] + player * 220]) {
			break;
		} else {
			ret++;
		}

	return ret;
}

/* 下に隙間があるか調べる */
int32_t cpuCheckFloating(int32_t player, int32_t x, int32_t y) {
	int32_t y2, y3;

	y2 = y + 1;
	if(y2 > fldsizeh[player]) y2 = fldsizeh[player];
	y3 = y + 2;
	if(y3 > fldsizeh[player]) y3 = fldsizeh[player];

	if( (!cp_fld[x + y2 * fldsizew[player] + player * 220]) || (!cp_fld[x + y3 * fldsizew[player] + player * 220]) ) {
		return -1;
	}

	return 0;
}
int32_t cpuCheckFloatingE(int32_t player, int32_t x, int32_t y) {
	int32_t y2;

	//そのブロックが消えるならスルー
//	if(cp_erase[y+22*player]) return 0;

	y2 = y + 1;
	if(y2 > fldsizeh[player]) y2 = fldsizeh[player];

	if(!cp_fld[x + y2 * fldsizew[player] + player * 220]) {
		return -1;
	}

	return 0;
}

/* 横にブロックが隣接しているか調べる */
int32_t cpuCheckLeftRight(int32_t player, int32_t x, int32_t y) {
	int32_t ret, x2, x3;

	ret = 0;
	x2 = x + 1;
	x3 = x - 1;

	if( (x2 >= fldsizew[player]) || (cp_fld[x2 + y * fldsizew[player] + player * 220]) ) ret++;
	if( (x3 >= fldsizew[player]) || (cp_fld[x3 + y * fldsizew[player] + player * 220]) ) ret++;

	return ret;
}

/* ラインが消えるか調べる */
int32_t cpu_blockEraseJudge(int32_t player) {
	int32_t		i, j, sr,ret;

	ret = 0;
	for(i = 0; i <= fldsizeh[player]; i++) {
		sr = 1;
		for(j = 0; j < fldsizew[player]; j++)
			if(cp_fld[j + i * fldsizew[player] + player * 220] == 0) {
				sr = 0;
				break;
			}
		ret = ret + sr;
		if(sr) cp_erase[i+22*player] = 1;
	}

	return ret;
}

// ヘボリス棒が必要な穴がいくつあるのか調べる
//■　■
//■　■こんな状態なら必要と判断
//■　■
int32_t cpu_HowManyNeedIblock(int32_t player) {
	int32_t i,j, count_left, count_right,total,by,by2;

	count_left = 0;
	count_right = 0;
	total = 0;

	for(j = 0;j < fldsizew[player];j++) {
		by = cpu_checkHoleStart(player,j);
		by2 = cpu_checkFieldTop(player,j);
		for(i = by; i < by2; i++) {
			// 左側にブロックがある（または端）
			if((j == 0) || (fld[(j - 1) + i * 10 + player * 220] != 0))
				count_left++;

			// 右側にブロックがある
			if((j == fldsizew[player] - 1 ) || (fld[(j + 1) + i * 10 + player * 220] != 0))
				count_right++;
		}
		if((count_left >= 3) && (count_right >= 3)) total++;	//棒が要ると判断
		count_left=0;
		count_right=0;
	}


	return total;
}

/* 現在のフィールドを仮想フィールドにコピー */
void cpuCopyField(int32_t player) {
	int32_t i;

	for(i=0; i<220; i++) {
		cp_fld[i + player * 220] = fld[i + player * 220];
	}
}

int32_t cpu_checkFieldTop(int32_t player,int32_t x) {
	int32_t i;

	for(i = 0; i <= fldsizeh[player]; i++)
		if(fld[x + i * fldsizew[player] + player * 220])
			return i;

	// 空っぽの場合
	return 22;
}
//穴が始まる位置を探す
//■　■
int32_t cpu_checkHoleStart(int32_t player,int32_t x) {
	int32_t i,top;
	top = cpu_checkFieldTop(player,x);

	for(i = 0; i < top; i++)
		if( ((x == 0) || (fld[(x - 1) + i * 10 + player * 220] != 0)) && ((x == fldsizew[player] - 1 ) || (fld[(x + 1) + i * 10 + player * 220] != 0)) )
			return i;

	return top;
}

/* 最善手の場所を決める */
void cpuCheckBestSpot(int32_t player) {
	int32_t i, j, k;
	int32_t bx2, by2;
	int32_t bottom;
	int32_t pts;
	int32_t tmp;
	int32_t mfilled;	// 最も埋まってる数
	int32_t mrot,mrot_s;		// 調べる回転方向数

	for(i=0;i<=21;i++) cp_erase[i+22*player] = 0;

	mfilled = 0;
	cp_hold[player] = 0;

	// 調べる回転方向数を決める
	mrot = 4;
	mrot_s = 0;

	// 赤、緑、紫は2パターンのみ
	if(!isWRule(player)) {
		if( (blk[player] == 0) || (blk[player] == 3) || (blk[player] == 6) ) mrot = 2;
	}

	// 黄色は回らない
	if( (blk[player] == 2) && (!istrance[player]) ) mrot = 1;
	// 回せない
	else if(isrotatelock[player] ){
		mrot = rt[player] + 1;
		mrot_s = rt[player];
	}

	for(j=mrot_s; j<mrot; j++) { /* 回転方向 */
		for(i=-3; i<fldsizew[player]; i++) { /* X座標 */
			// 底を調べる
			for(bottom = 0; judgeBlock(player, i, bottom, blk[player], j) == 0; bottom++);
			bottom = bottom - 1;

			if( judgeBlock(player, i, bottom, blk[player], j) == 0 ) {
				// 現在のフィールドを仮想フィールドにコピー
				cpuCopyField(player);

				// 仮想フィールドにブロックを設置する
				if(cpu_setBlock(player, i, bottom, blk[player], j)==0){
					pts = 0;
					break;	// 画面外死亡を避ける
				}

				pts = 0;

				for(k=0; k<4; k++) {
					// 座標を決める
					if(rots[player] == 8) {
						bx2 = (i + blkDDataX[blk[player] * 16 + j * 4 + k] * (IsBig[player] + 1));
						by2 = (bottom + blkDDataY[blk[player] * 16 + j * 4 + k] * (IsBig[player] + 1));
					} else if( isWRule(player) ) {
						bx2 = (i + blkWDataX[blk[player] * 16 + j * 4 + k] * (IsBig[player] + 1));
						by2 = (bottom + blkWDataY[blk[player] * 16 + j * 4 + k] * (IsBig[player] + 1));
					} else {
						bx2 = (i + blkDataX[blk[player] * 16 + j * 4 + k] * (IsBig[player] + 1));
						by2 = (bottom + blkDataY[blk[player] * 16 + j * 4 + k] * (IsBig[player] + 1));
					}

					// 2ライン以上消えればボーナス
					tmp = cpu_blockEraseJudge(player);

					// 下1マスに隙間があったら終了
					if( cpuCheckFloatingE(player, bx2, by2) == -1 ) {
						pts = 0;
						break;
					}

					// 総数を調べる
					//pts = pts + cpuBlockHowManyFilled(player, by2);
					pts = pts + blockHowManyFilledFromLeft(player, by2);

					// 壁と隣接させる
					pts = pts + cpuCheckLeftRight(player, bx2, by2);


					//操作ブロックが↑DEL FIELD、MOV FIELD、FREE FALLなら消える形が最優先
					if((item[player] == 17) || (item[player] == 29) || (item[player] == 30) || (item[player] == 35) || (item[player] == 28))
						tmp = tmp * 1000;
					//ピンチならDEL EVEN、↓DEL FIELDも狙う
					if((pinch[player]) && ((item[player] == 18) || (item[player] == 19)))
						tmp = tmp * 1000;
					if(tmp >= 2) {
						pts = pts + tmp * 2;
					} else if( (tmp == 1) && (!pinch[player]) && (cp_type) && (((item[player] == 17) || (item[player] == 29) || (item[player] == 30)) && (tmp != 0)) ) {
						// 1ラインしか消えない場合はこの手を捨てる
						pts = 0;
						break;
					}
				}
				// 最善手の場合
				if(pts >= mfilled) {
					mfilled = pts;
					cp_x[player] = i;
					cp_rt[player] = j;
				}
			}
		}/* for(i=-3; i<fldsizew[player]; i++) */
	}/* for(j=0; j<mrot; j++) */

	// 棒をHOLDする
	if( (blk[player] == 0) && (hold[player] != 0) && (!dhold[player]) && (mfilled <= 40) && (!pinch[player]) && (!item[player])) {
		// 次のブロックがO、Z、Sでない場合はHOLDする
		if( (next[player] != 2) && (next[player] != 3) && (next[player] != 6) && (cpu_HowManyNeedIblock(player) < 2 - (1 * (pinch[player]))) ) {
			cp_hold[player] = 1;
		}
	}

	// 棒をHOLD枠から呼び出す
	if( (hold[player] == 0) && (!dhold[player]) && (cpu_HowManyNeedIblock(player) >= 2 - (1 * (pinch[player]))) )
		cp_hold[player] = 1;

	// 有効な手が無い場合
	if(mfilled == 0) {
		// 棒をHOLD枠から呼び出す
		if( (hold[player] == 0) && (!dhold[player]) && (cpu_HowManyNeedIblock(player) >= 1 ) )
			cp_hold[player] = 1;
		else if( (dhold[player] == 0) && (!item[player]) ) {
			// HOLDできるならHOLDする
			// ただしアイテムがある場合はしない #1.60c7o9
			cp_hold[player] = 1;
		} else {
			// HOLDできないなら妥協
			for(j=0; j<mrot; j++) { /* 回転方向 */
				for(i=-3; i<fldsizew[player]; i++) { /* X座標 */
					// 底を調べる
					for(bottom = 0; judgeBlock(player, i, bottom, blk[player], j) == 0; bottom++);
					bottom = bottom - 1;

					if( judgeBlock(player, i, bottom, blk[player], j) == 0 ) {
						// 現在のフィールドを仮想フィールドにコピー
						cpuCopyField(player);

						// 仮想フィールドにブロックを設置する
						cpu_setBlock(player, i, bottom, blk[player], j);

						pts = 0;

						for(k=0; k<4; k++) {
							// 座標を決める
							if(rots[player] == 8) {
								bx2 = (i + blkDDataX[blk[player] * 16 + j * 4 + k] * (IsBig[player] + 1));
								by2 = (bottom + blkDDataY[blk[player] * 16 + j * 4 + k] * (IsBig[player] + 1));
							} else if( isWRule(player) ) {
								bx2 = (i + blkWDataX[blk[player] * 16 + j * 4 + k] * (IsBig[player] + 1));
								by2 = (bottom + blkWDataY[blk[player] * 16 + j * 4 + k] * (IsBig[player] + 1));
							} else {
								bx2 = (i + blkDataX[blk[player] * 16 + j * 4 + k] * (IsBig[player] + 1));
								by2 = (bottom + blkDataY[blk[player] * 16 + j * 4 + k] * (IsBig[player] + 1));
							}

							// ラインが消えればボーナス
							tmp = cpu_blockEraseJudge(player) * 10;

							// 下1マスに隙間
							if( cpuCheckFloatingE(player, bx2, by2) == -1 )
								pts = pts - 10;
							// 総数を調べる
							pts = pts + cpuBlockHowManyFilled(player, by2);

							// 壁と隣接させる
							pts = pts + cpuCheckLeftRight(player, bx2, by2);

							if((tmp >= 2) || (pinch[player])) {
								pts = pts + (tmp * 10) + 10;	// ボーナス
							}
							//操作ブロックが↑DEL FIELDまたはMOV FIELDなら消える形が最優先
							if((item[player] == 17) || (item[player] == 29) || (item[player] == 30))
								tmp = tmp * 1000;
							//ピンチならDEL EVEN、↓DEL FIELD、FREE FALLも狙う
							if((pinch[player]) && ((item[player] == 18) || (item[player] == 19) || (item[player] == 28)))
								tmp = tmp * 1000;
						}

						// 最善手の場合
						if(pts >= mfilled) {
							mfilled = pts;
							cp_x[player] = i;
							cp_rt[player] = j;
						}
					}
				}/* for(i=-3; i<fldsizew[player]; i++) */
			}/* for(j=0; j<mrot; j++) */
		}
	}/* if(mfilled == 0) */
	// BEGINNERナビゲーション用に、最善手の場所へ、仮想フィールドにブロックを設置
	for(bottom = 0; judgeBlock(player, cp_x[player], bottom, blk[player], cp_rt[player]) == 0; bottom++) {}
		bottom = bottom - 1;
	cpuCopyField(player);
	cpu_setBlock(player, cp_x[player], bottom, blk[player], cp_rt[player]);
}
