//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  READY後のSTART!演出
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲

#include "../script/include.h"

void effect(int32_t player) {
	if(ready_go_style) return;
	statusc[player * 10]++;

	if(statusc[player * 10 + 1] == 1) {
		if(statusc[player * 10] < 30)
			//                           ↓が+になっていたので修正#1.60c7f6
			ExBltRect(3, 117 + player * 192 - 96 * maxPlay, 119, 0, 17, 86, 17);
		else
			statusc[player * 10 + 1] = 0;
	}
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  オブジェクト関連
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void objectCreate(int32_t p, int32_t s, int32_t x, int32_t y, int32_t v, int32_t w, int32_t j, int32_t c) {
	int32_t		i;
	for(i = 0; i < 100; i++)
		if(obj[i] == 0) {
			obj[i] = s;
			objx[i] = x * 100;
			objy[i] = y * 100;
			objv[i] = v;
			obja[i] = 0;	// アニメーションの初期化#1.60c7h8
			objw[i] = w;
			objj[i] = j - 1;
			objc[i] = c;
			objp[i] = p;	// #1.60c7l1

			break;
		}
}

// 全消しだけで使われている
void objectCreate2(int32_t p, int32_t s, int32_t x, int32_t y, int32_t v, int32_t w, int32_t j, int32_t c) {
	int32_t		i;
	for(i = 0; i < 100; i++)
		if(obj[i] == 0) {
			obj[i] = s;
			objx[i] = x;
			objy[i] = y;
			objv[i] = v;
			obja[i] = 0;	// アニメーションの初期化#1.60c7h8
			objw[i] = w;
			objj[i] = j - 1;
			objc[i] = c;
			objp[i] = p;	// #1.60c7l1

			break;
		}
}

void objectExecute(void) {
	int32_t		i;
	for(i = 0; i < 100; i++) {
		if(obj[i]) {
			// jump(obj[i] - 1, o00, o01, o02, o03, o04, o05, o06, o07,o08,o09,o10,o11,o12,o13);

			switch (obj[i] - 1) {
				case 0: goto o00;
				case 1: goto o01;
				case 2: goto o02;
				case 3: goto o03;
				case 4: goto o04;
				case 5: goto o05;
				case 6: goto o06;
				case 7: goto o07;
				case 8: goto o08;
				case 9: goto o09;
				case 10: goto o10;
				case 11: goto o11;
				case 12: goto o12;
				case 13: goto o13;
			}

			o00: objectStatBlock(i);			// 01 -> 飛び散るブロック
					goto next;
			o01: objectStatLine(i);				// 02 -> 何列消したか表示
					goto next;
			o02: objectStatBlock(i);			// 03 -> 飛び散るブロック (速いバージョン)
					goto next;
			o03: objectAllCrear(i);				// 04 -> 全けし
					goto next;
			o04: objectStatBlock2(i);			// 05 -> 飛び散るブロック (遅いバージョン)
					goto next;
			o05: objectPopBlock(i);				// 06 -> 弾け飛ぶブロック
					goto next;
			o06: objectHanabi(i);				// 07 -> 花火
					goto next;
			o07: objectNagareboshiP(i);			// 08 -> 流れ星（親）
					goto next;
			o08: objectNagareboshiC(i);			// 09 -> 流れ星（子）
					goto next;
			o09: objectItemWarning(i);			// 10 -> アイテム発動警告
					goto next;
			o10: objectGetmedal(i);				// 11 -> メダル獲得演出
					goto next;
			o11: objectitem_erase(i);			// 12 -> アイテムブロック消去
					goto next;
			o12: objectdelfield(i);				// 13 -> ライン強制消去エフェクト
					goto next;
			o13: objectplatina_erase(i);		// 14 -> プラチナブロック消去エフェクト
					goto next;
			next:	;
		}
	}
}

// objectStatBlock / objectStatBlock2
// 破壊エフェクトの変更に伴い全面変更 #1.60c7h8
void objectStatBlock(int32_t no) {
	int32_t		zoom, offset, rate ,k, l;

	if((objj[no] < 0) && (rots[objp[no]] == 6)){
		ExBltFastRect(74,objx[no] / 100, objy[no] / 100,0,0,8,8);
		obj[no] = 0;
		return;
	}

	// breakeffectが動作していない問題を修正#1.60c7h9
	if( (!breakeffect) || (((objj[no] == 8) || (objj[no] == 9)) && (heboGB[objp[no]]==0)) || (objj[no] < 0)) {
		obj[no] = 0;
		return;
	}

	rate = (obj[no] == 3) + 1;
	if(heboGB[objp[no]]==1){
		if(obja[no] % 30 < 15)
			ExBltFastRect(74,objx[no] / 100, objy[no] / 100,0,0,8,8);
		else if(obja[no] < 40)
			ExBltFastRect(74,objx[no] / 100, objy[no] / 100,objj[no] * 8,0,8,8);
		if(obja[no] >= 59) obj[no] = 0;
		obja[no] = obja[no] + 1;
	}else if(heboGB[objp[no]]==2){
		ExBltFastRect(64,objx[no] / 100, objy[no] / 100,0,56 + (8*(obja[no] / 2)),8,8);
		if(obja[no] >= 20) obj[no] = 0;
		obja[no] = obja[no] + 1;
	//breaktypeを3にするとBEGINNERのみ旧エフェクト、それ以外新エフェクト #1.60c7l1
	}else if( (gameMode[objp[no]] == 10) || (breaktype == 0) || ((breaktype == 3) && (gameMode[objp[no]] == 0) && (!novice_mode[objp[no]])) ) {
		if(!istimestop[objp[no]]){
			objx[no] = objx[no] + objv[no] * rate;
			objy[no] = objy[no] + objw[no] * rate;

			objw[no] = objw[no] + 250 * rate;
			objc[no] = objc[no] + 15 * rate;
		}

		offset = (objc[no] - 100) * 8 / 200;

		if(objj[no] >= 10){
			zoom = objc[no] * 65536 / 100;

			// 色がバグっていたのを修正 #1.60c7l1
			ExBltFastRectR(getBlockPlaneNo(objp[no], objj[no]), objx[no] / 100 - offset, objy[no] / 100 - offset, objj[no] * 8 - (objj[no] >= 10) * 80, 0, 8, 8, zoom, zoom);
		} else {
			k = getBigBlock_add(objp[no]);
			l = 0;
			if(k >= 4){
				k = k-4;
				l = 1;
			}
			zoom = objc[no] * 65536 / 200;
			ExBltFastRectR(76, objx[no] / 100 - offset, objy[no] / 100 - offset, objj[no] * 16 + (l * 160), k * 112, 16, 16, zoom, zoom);
		}

		if((objy[no] + offset * 100) > 24000) obj[no] = 0;

	} else {

		// TAP/TIのようにブロックが砕けるアニメーションを表示する #1.60c8
		// hebospr.bmpで用意したアニメーションは6コマあるが、
		// 1フレームごとに1コマだと速すぎて見えないので
		// 通常消去時は1/3倍速、高速消去時は1/2倍速としている

		// ExBltRect(3, dposx, dposy, sposx, sposy, width, height);

		if( objj[no] >= 9 ) {
			// プラチナブロック #1.60c7m9
			objectplatina_erase(no);
			if((obja[no] < 32) && (gameMode[objp[no]] == 6) && (stage[objp[no]] < 100))
				ExBltRect(3, objx[no] / 100, (objy[no]-(obja[no]*30)) / 100, 212+(12*(objj[no]-10)), 64, 12, 7);
			//objy[no] = objy[no] - 20;
			if(obja[no] >= 59) obj[no] = 0;
		} else {
			// iniの設定で旧式の砕けエフェクトを使用できるようにした#1.60c7i4
			if(breaktype == 2) {
				if(obja[no] >= 15)
					obj[no] = 0;
				else
					ExBltRect(3, objx[no] / 100 - 12, objy[no] / 100 - 12, (obja[no] / 3) * 32, 162 + (objj[no] * 32), 32, 32);
			} else {
				if(obja[no] >= 36)
					obj[no] = 0;
				else
				ExBltRect(32 +objj[no], objx[no] / 100 - 44, objy[no] / 100 - 20 - ( (breakti == 0) * 24 ),
					((obja[no]) % (8 - (2 * (breakti))) ) * 96, ((obja[no]) / (8 - (2 * (breakti)))) * 96, 96, 96);
			}
		}
		if(!istimestop[objp[no]]){
			obja[no] = obja[no] + 1;
			if((objj[no] < 9) && ((wait1[objp[no]] / 2) + wait2[objp[no]] <= 10) && ((timeOn[objp[no]]) || (ending[objp[no]] == 2)))
				obja[no] = obja[no] + 1;
		}
	}
}

void objectStatBlock2(int32_t no) {
	int32_t		zoom, offset, rate, k, l;

	if((objj[no] < 0) && (rots[objp[no]] == 6)){
		ExBltFastRect(74,objx[no] / 100, objy[no] / 100,0,0,8,8);
		obj[no] = 0;
		return;
	}
	// breakeffectが動作していない問題を修正#1.60c7h9
	if( (!breakeffect) || (((objj[no] == 8) || (objj[no] == 9)) && (heboGB[objp[no]]==0)) || (objj[no] < 0)) {
		obj[no] = 0;
		return;
	}
	if(heboGB[objp[no]]==1){
		if(obja[no] % 30 < 15)
			ExBltFastRect(74,objx[no] / 100, objy[no] / 100,0,0,8,8);
		else if(obja[no] < 40)
			ExBltFastRect(74,objx[no] / 100, objy[no] / 100,objj[no] * 8,0,8,8);
		if(obja[no] >= 59) obj[no] = 0;
		obja[no] = obja[no] + 1;
	}else if(heboGB[objp[no]]==2){
		ExBltFastRect(64,objx[no] / 100, objy[no] / 100,0,56 + (8*(obja[no] / 2)),8,8);
		if(obja[no] >= 20) obj[no] = 0;
		obja[no] = obja[no] + 1;
	//breaktypeを3にするとBEGINNERのみ旧エフェクト、それ以外新エフェクト #1.60c7l1
	} else if((gameMode[objp[no]] == 10) || (breaktype == 0) || ((breaktype == 3) && (gameMode[objp[no]] == 0) && (!novice_mode[objp[no]])) ) {
		if(!istimestop[objp[no]]){
			objx[no] = objx[no] + objv[no] / 2;
			objy[no] = objy[no] + objw[no] / 2;

			objw[no] = objw[no] + 250 / 2;
			objc[no] = objc[no] + 15 / 2;
		}
		offset = (objc[no] - 100) * 8 / 200;

		if(objj[no] >= 10){
			zoom = objc[no] * 65536 / 100;

			// 色がバグっていたのを修正 #1.60c7l1
			ExBltFastRectR(getBlockPlaneNo(objp[no], objj[no]), objx[no] / 100 - offset, objy[no] / 100 - offset, objj[no] * 8 - (objj[no] >= 10) * 80, 0, 8, 8, zoom, zoom);
		} else {
			k = getBigBlock_add(objp[no]);
			l = 0;
			if(k >= 4){
				k = k-4;
				l = 1;
			}
			zoom = objc[no] * 65536 / 200;
			ExBltFastRectR(76, objx[no] / 100 - offset, objy[no] / 100 - offset, objj[no] * 16 + (l * 160), k * 112, 16, 16, zoom, zoom);
		}

		if((objy[no] + offset * 100) > 24000) obj[no] = 0;

	} else {
		// ExBltRect(3, dposx, dposy, sposx, sposy, width, height);

		if( objj[no] >= 9 ) {
			// プラチナブロック #1.60c7m9
			objectplatina_erase(no);
			if((obja[no] < 32) && (gameMode[objp[no]] == 6) && (stage[objp[no]] < 100))
				ExBltRect(3, objx[no] / 100, (objy[no]-(obja[no]*30)) / 100, 212+(12*(objj[no]-10)), 64, 12, 7);
			objy[no] = objy[no] - 20;
			if(obja[no] >= 59) obj[no] = 0;
		} else {
			// iniの設定で旧式の砕けエフェクトを使用できるようにした#1.60c7i4
			if(breaktype == 2) {
				ExBltRect(3, objx[no] / 100 - 12, objy[no] / 100 - 12, (obja[no] / 4) * 32, 162 + (objj[no] * 32), 32, 32);
				if(obja[no] >= 20) obj[no] = 0;
			} else {
				ExBltRect(32 +objj[no], objx[no] / 100 - 44, objy[no] / 100 - 20 - ( (breakti == 0) * 24 ),
					((obja[no]) % (8 - (2 * (breakti))) ) * 96, ((obja[no]) / (8 - (2 * (breakti)))) * 96, 96, 96);
			}
		}
		if(!istimestop[objp[no]]){
			obja[no] = obja[no] + 1;
			if((objj[no] < 9) && ((wait1[objp[no]] / 2) + wait2[objp[no]] <= 10) && ((timeOn[objp[no]]) || (ending[objp[no]] == 2)))
				obja[no] = obja[no] + 1;
		}
		if(obja[no] >= 36) obj[no] = 0;
	}
}
// objp[no]:BtoB や T-Spin の判定に使用
void objectStatLine(int32_t no) {
	int32_t scale;
	if(!istimestop[(objy[no] / 100 == 132)]){
		objw[no] = objw[no] + 50;
		obja[no]++;



		if(objw[no] < 0) {
			objv[no] = objv[no] + 75 * (1 - (objv[no] > 0) * 2);

			objx[no] = objx[no] + objv[no];
//			if(objy[no] + objw[no] > 9600)
//				objy[no] = objy[no] + objw[no];
		}
	}
	if(objw[no] <= 3600) scale = 65536;
	else scale = 65536 - ((objw[no] - 3600) * 64);

	if(scale < 0){
		obj[no] = 0;
		return;
	}
/*
	if(objw[no] > 3600) {
		objy[no] = objy[no] - objw[no] + 3600;
		if(objy[no] < -3200) obj[no] = 0;
	} else {
		if(objy[no] < 9600)
		objy[no] = objy[no] + 400;
	}
*/

	ExBltRectR(81, objx[no] / 100 - 3, objy[no] / 100 , 90 * objp[no], 320 + (objj[no] * 32), 90, 32, 65536, scale);

	if((objc[no] > 1) && (objw[no] <= 3600)) {
		ExBltRect(3, objx[no] / 100 +  8 + (objv[no] < 0) * 32, objy[no] / 100 + 32 , (objc[no] - 1) * 8, 112, 8, 8);
		ExBltRect(3, objx[no] / 100 + 16 + (objv[no] < 0) * 32, objy[no] / 100 + 32 , 0, 120, 24, 8);
	}
}

void objectAllCrear(int32_t no) {
	objw[no]++;
	if(objy[no] > 13) objy[no]--;

	ExBltRect(81, 110+192 * objp[no] -96 * maxPlay , (objy[no] - 1) * 8 , 100, 150 + 25 * (count % 4 / 2), 100, 25);
	if( (gameMode[objp[no]] <= 3) || (gameMode[objp[no]] == 5) )
		printFont(18 +objx[no] * 24 - 12 * maxPlay, objy[no] + 4, "e 4", 5);	//#1.60c7j9

	if(objw[no] > 80)
		obj[no] = 0;
}

// 弾け飛ぶブロック #1.60c7o6
void objectPopBlock(int32_t no) {
	int32_t		zoom, offset, rate, k, l;

	if( (!breakeffect) || (objj[no] == 8) || (objj[no] == 9) || (objj[no] < 0)) {
		obj[no] = 0;
		return;
	}

	rate = (obj[no] == 3) + 1;
	objx[no] = objx[no] + objv[no] * rate;
	objy[no] = objy[no] + objw[no] * rate;
	objw[no] = objw[no] + 250 * rate;
	objc[no] = objc[no] + 15 * rate;

	offset = (objc[no] - 100) * 8 / 200;

		if(objj[no] >= 10){
			zoom = objc[no] * 65536 / 100;

			// 色がバグっていたのを修正 #1.60c7l1
			ExBltFastRectR(getBlockPlaneNo(objp[no], objj[no]), objx[no] / 100 - offset, objy[no] / 100 - offset, objj[no] * 8 - (objj[no] >= 10) * 80, 0, 8, 8, zoom, zoom);
		} else {
			k = getBigBlock_add(objp[no]);
			l = 0;
			if(k >= 4){
				k = k-4;
				l = 1;
			}
			zoom = objc[no] * 65536 / 200;
			ExBltFastRectR(76, objx[no] / 100 - offset, objy[no] / 100 - offset, objj[no] * 16 + (l * 160), k * 112, 16, 16, zoom, zoom);
		}

	if((objy[no] + offset * 100) > 24000) obj[no] = 0;
}

// 花火
void objectHanabi(int32_t no) {

	if( (!breakeffect) || (objj[no] < 0) || (objj[no] > 6) ) {
		obj[no] = 0;
		return;
	}

	// 96x96
	// 48frames

	ExBltRect(47 + objj[no], objx[no], objy[no], (obja[no]%6) * 96, (obja[no]/6) * 96, 96, 96);

	obja[no]++;
	if(obja[no] >= 48) obj[no] = 0;
}
//プラチナブロック消去エフェクトC7T2.8
void objectplatina_erase(int32_t no) {
	// 32x32
	// 60frames

	ExBltRect(57+objj[no]-10, objx[no] / 100 - 12, objy[no] / 100 - 12,(obja[no]%10) * 32, (obja[no]/10) * 32, 32, 32);

	if(obj[no] == 14) obja[no]++;
	if(obja[no] >= 60) obj[no] = 0;
}
//流れ星（座標計算）
void objectNagareboshiP(int32_t no) {
	objx[no]=objx[no]-4;
	objy[no]=objy[no]+6;

	objectCreate2(objp[no], 9, objx[no], objy[no], 0, 0, 0, 0);
		obja[no]++;
		if(obja[no] >= 30) obj[no] = 0;
}
//流れ星（描画）
void objectNagareboshiC(int32_t no) {
		// 32x32
		// 35frames

		ExBltRect(65, objx[no], objy[no], (obja[no]%6) * 32, (obja[no]/6) * 32, 32, 32);

		obja[no]++;
		if(obja[no] >= 35) obj[no] = 0;
}
//アイテム発動警告
//objw[no]：REFLECTで跳ね返された
void objectItemWarning(int32_t no){

	if((objc[no] == 0) && ((status[objp[no]] == 5) || (fmirror_cnt[objp[no]] >= 0))){
		objc[no] = 1;
		objy[no] = 1;
	}
	if((objc[no] == 1) && (abs_YGS2K(GetRealFPS() - GetFPS()) >= 10)) objc[no] = 2;

	if(((objj[no]==2) && (IsBigStart[objp[no]])) || ((objj[no]==36) && (gameMode[objp[no]] != 4)))
		printFont(15 + 24 * objp[no] - 12 * maxPlay, 15, "NO EFFECT", fontc[rots[objp[no]]]);
	else{
		if((!objx[no]) && ((!objy[no]) || (obja[no] % 2 == 0)))
			ExBltRect(81, 110+192 * objp[no] -96 * maxPlay , 54 , 0, 150 + 25 * (count % 4 / 2), 100, 25);

		if(objj[no] >= 0){
			if(objc[no] == 1)
				BlendExBltRect(89, 120+192 * objp[no] -96 * maxPlay , 86 , 80 * (objj[no] / 10), 36 * (objj[no] % 10), 80, 36,87,87,87,170,170,170);
			else if((objc[no] == 0) || ((objc[no] == 2) && (obja[no] % 2 == 0)))
				ExBltFastRect(89, 120+192 * objp[no] -96 * maxPlay , 86 , 80 * (objj[no] / 10), 36 * (objj[no] % 10), 80, 36);

			if(objw[no] == 1)
				printFont(15 + 24 * objp[no] - 12 * maxPlay, 16,"REFLECTED!", 2 * (count % 4 / 2));
			else if(objw[no] == 2)
				printFont(15 + 24 * objp[no] - 12 * maxPlay, 16,"  GUARD!", 6 * (count % 4 / 2));
			else{
				ExBltFastRect(46,(4 + 24 * objp[no] + 12 * (!maxPlay))*8, 128,(8*(objj[no]))+56,0,8,8);
				ExBltRect(28,(5 + 24 * objp[no] + 12 * (!maxPlay))*8, 124, 0, (objj[no]) * 16, 48, 16);
				ExBltFastRect(46,(11 + 24 * objp[no] + 12 * (!maxPlay))*8, 128,(8*(objj[no]))+56,0,8,8);
			}
		}
	}
	obja[no]++;

	if(obja[no] > 50) obj[no] = 0;

}
//メダル獲得
//objv メダルの種類  0:AC 1:ST 2:SK 3:RE 4:RO 5:CO
//objc メダルの段階　0:銅 1:銀 2:金 3:プラ
void objectGetmedal(int32_t no){
	int32_t i,j,k;
	if(medaltype >= 2){
		obj[no] = 0;
		return;
	}

	if(medaltype){	//TIメダル（拡大→縮小）
		if(obja[no] <= 8){
			k = obja[no] * (8 / 5);
			i = (16 * k / 2 - 16) / 2;
			j = (12 * k / 2 - 12) / 2;
			k = 65536 * k / 2;
		} else {
			k = (16 - obja[no]) * (8 / 5);
			i = (16 * k / 3 - 16) / 2;
			j = (12 * k / 3 - 12) / 2;
			k = 65536 * k / 3;
			if(k < 65536){
				k = 65536;
				i=0;
				j=0;
			}
		}
		ExBltRectR(56, objx[no] - i, objy[no] - j, objv[no] * 16, (3 - objc[no]) * 12, 16, 12, k, k);
	} else{	//TAPメダル（拡大した状態から縮小）
		k = 16 - obja[no];
		i = (15 * k / 3 - 15) / 2;
		j = (12 * k / 3 - 12) / 2;
		k = 65536 * k / 3;
		if(k < 65536){
			k = 65536;
			i=0;
			j=0;
		}
		ExBltRectR( 0, objx[no] - i, objy[no] - j, objv[no] * 16, (3 - objc[no]) * 13, 15, 12, k, k);
	}
	obja[no]++;
	if(obja[no] >= 16) obj[no] = 0;
}
//アイテムブロック消去エフェクト
void objectitem_erase(int32_t no) {
	// 80x80
	// 30frames

	ExBltRect(82, objx[no] / 100 - 36, objy[no] / 100 - 36,(obja[no]%6) * 80, (obja[no]/6) * 80, 80, 80);

	if(!istimestop[objp[no]]) obja[no]++;
	if(obja[no] >= 30) obj[no] = 0;
}
//ライン消去エフェクト
void objectdelfield(int32_t no) {

	ExBltRect(45, objx[no] / 100, objy[no] / 100,0, obja[no] * 8 + (128 * (objc[no])), 80, 8);

	if(!istimestop[objp[no]]){
		obja[no]++;
		if((wait2[objp[no]] <= 12) && ((timeOn[objp[no]]) || (ending[objp[no]] == 2) || ((objc[no]) && (repversw >= 65))))
			obja[no] = obja[no] + 1;
	}
	if(obja[no] >= 15) obj[no] = 0;
}
//エフェクト全消去
void objectClear(void) {
	int32_t		i;

	for(i = 0; i < 100; i++) {
		obj[i] = 0;
	}
}
//指定したプレイヤーのエフェクトのみ全消去
void objectClearPl(int32_t player) {
	int32_t		i;

	for(i = 0; i < 100; i++) {
		if(objp[i] == player)
			obj[i] = 0;
	}
}

void objectComboClearPl(int32_t player) {
	int32_t		i;

	for(i = 0; i < 100; i++) {
		if((obj[i] == 2) && (objy[i] / 100 == 100 + 32 * player))
			obj[i] = 0;
	}
}
