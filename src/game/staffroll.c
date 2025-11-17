//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  スタッフロールデータを初期化
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲

#include "common.h"

void staffInitPl(void) {
//	staffroll_y_ex[player] = 2000;		//無理矢理…
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  スタッフロール文字列を描画
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void staffExecute(int32_t player) {
	if( ending[player] != 2 ) return;

	if(!onRecord[player]) return;

	// 描画する
	if ( getDrawRate() != 1 )
		APP_DrawPlaneRect(70,240 + 384 * player - 192 * maxPlay,80,0,staffroll_y_ex[player],160,320);
	else
		APP_DrawPlaneRect(70,120 + 192 * player - 96 * maxPlay,40,0,staffroll_y_ex[player] / 2,80,160);
}

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//  新しい文字列を下から表示させる
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void staffRoll(int32_t player) {
	if( (ending[player] == 2) && (onRecord[player]) && (!pauseGame[player]) ) {
		if(fastroll[player]){
			endtime[player]=endtime[player]+2; // 経過時間+1 #1.60c7o6
		}else{
			endtime[player]++; // 経過時間+1 #1.60c7o6
		}
		//無限回転対策
		if((repversw >= 55) && (gameMode[player] >= 1) && ((kickc[player] > 10) || (kickc3[player] > 8)) ){
			endtime[player] = endtime[player] - 1 - (fastroll[player]);
		}else{
			// 上に移動
			if( (endtime[player] % 2 == 0) && (endtime[player] < 3100) && (!pauseGame[player]) ){
				staffroll_y_ex[player]++;
			}
		}

	}
}
