/* TGM式段位上昇 */

#include "../script/include.h"

void GradeUp(int32_t player){
	if(gameMode[player] >= 4) return;
	if((enable_grade[player] == 1) && (gameMode[player] != 0) && (gameMode[player] != 3)) { // DEVIL以外
		// (S8までなら)段位上昇判定 #1.60c7g5
		if((grade[player] < 17) && (sc[player] >= gscore[grade[player]])) {
			PlayWave(30);	// rankup.wav #1.60c7i2
			grade[player]++;
			gflash[player]=120;
			gup_down[player] = 1;
		}

		// gmflag1成立判定#1.60c7g5
		if((tc[player] >= 300) && (!gmflag1_e[player])) {
			if((grade[player] >= 6) && (time[player] <= 255 * 60)){
					gmflag1[player] = 1;
					objectCreate2(player, 8, Rand(20) + 180 + 192 * player - 96 * maxPlay, 20 + Rand(10), 0, 0, 0, 0);
				}
			gmflag1_e[player] = 1;
		}

		// gmflag2成立判定#1.60c7g5
		if((tc[player] >= 500) && (!gmflag2_e[player])) {
			if((grade[player] >= 9) && (time[player] <= 450 * 60)){
					gmflag2[player] = 1;
					objectCreate2(player, 8, Rand(20) + 180 + 192 * player - 96 * maxPlay, 20 + Rand(10), 0, 0, 0, 0);
				}
			gmflag2_e[player] = 1;
		}
	}
}
/* TAP式段位 #C7T5EX*/
void GradeUp2(int32_t player, int32_t lines){
	int32_t i[2],gbai[2];
	if( (gameMode[player] == 1) || (gameMode[player] == 2) ) {
		if(enable_grade[player] == 2){
			if(grade[player]==0){//9
				if ( lines == 1 ) {
					i[player] = 10;
				} else if ( lines == 2 ) {
					i[player] = 15;
				} else if ( lines == 3 ) {
					i[player] = 30;
				} else {
					i[player] = 55;
				}
			}else if((grade[player]>=1)&&(grade[player]<=2)){//8,7
				if ( lines == 1 ) {
					i[player] = 10;
				} else if ( lines == 2 ) {
					i[player] = 20;
				} else if ( lines == 3 ) {
					i[player] = 30;
				} else {
					i[player] = 40;
				}
			}else if((grade[player]>=3)&&(grade[player]<=5)){//6,5,4
				if ( lines == 1 ) {
					i[player] = 10;
				} else if ( lines == 2 ) {
					i[player] = 15;
				} else if ( lines == 3 ) {
					i[player] = 25;
				} else {
					i[player] = 35;
				}
			}else if((grade[player]>=6)&&(grade[player]<=8)){//3,2,1
				if ( lines == 1 ) {
					i[player] = 5;
				} else if ( lines == 2 ) {
					i[player] = 15;
				} else if ( lines == 3 ) {
					i[player] = 23;
				} else {
					i[player] = 33;
				}
			}else if((grade[player]>=9)&&(grade[player]<=12)){//S1-S4
				if ( lines == 1 ) {
					i[player] = 3;
				} else if ( lines == 2 ) {
					i[player] = 12;
				} else if ( lines == 3 ) {
					i[player] = 15;
				} else {
					i[player] = 30;
				}
			}else if((grade[player]>=13)&&(grade[player]<=15)){//S5-S7
				if ( lines == 1 ) {
					i[player] = 3;
				} else if ( lines == 2 ) {
					i[player] = 15;
				} else if ( lines == 3 ) {
					i[player] = 20;
				} else {
					i[player] = 30;
				}
			}else if((grade[player]>=16)&&(grade[player]<=26)){//S8-m9
				if ( lines == 1 ) {
					i[player] = 4;
				} else if ( lines == 2 ) {
					i[player] = 18;
				} else if ( lines == 3 ) {
					i[player] = 23;
				} else {
					i[player] = 30;
				}
			}else if(grade[player]>=27){//M以上
				if ( lines == 1 ) {
					i[player] = 2;
				} else if ( lines == 2 ) {
					i[player] = 12;
				} else if ( lines == 3 ) {
					i[player] = 15;
				} else {
					i[player] = 20;
				}
			}
		}else if(enable_grade[player] == 3){
			if(grade2[player]==0){//9
				if ( lines == 1 ) {
					i[player] = 10;
				} else if ( lines == 2 ) {
					i[player] = 15;
				} else if ( lines == 3 ) {
					i[player] = 30;
				} else {
					i[player] = 50;
				}
			}else if((grade2[player]>=1)&&(grade2[player]<=2)){//8,7
				if ( lines == 1 ) {
					i[player] = 10;
				} else if ( lines == 2 ) {
					i[player] = 20;
				} else if ( lines == 3 ) {
					i[player] = 30;
				} else {
					i[player] = 40;
				}
			}else if((grade2[player]>=3)&&(grade2[player]<=5)){//6,5,4
				if ( lines == 1 ) {
					i[player] = 10;
				} else if ( lines == 2 ) {
					i[player] = 15;
				} else if ( lines == 3 ) {
					i[player] = 25;
				} else {
					i[player] = 35;
				}
			}else if((grade2[player]>=6)&&(grade2[player]<=8)){//3,2,1
				if ( lines == 1 ) {
					i[player] = 5;
				} else if ( lines == 2 ) {
					i[player] = 15;
				} else if ( lines == 3 ) {
					i[player] = 23;
				} else {
					i[player] = 30;
				}
			}else if((grade2[player]>=9)&&(grade2[player]<=12)){//S1-S4
				if ( lines == 1 ) {
					i[player] = 3;
				} else if ( lines == 2 ) {
					i[player] = 12;
				} else if ( lines == 3 ) {
					i[player] = 20;
				} else {
					i[player] = 30;
				}
			}else if((grade2[player]>=13)&&(grade2[player]<=15)){//S5-S7
				if ( lines == 1 ) {
					i[player] = 3;
				} else if ( lines == 2 ) {
					i[player] = 15;
				} else if ( lines == 3 ) {
					i[player] = 18;
				} else {
					i[player] = 25;
				}
			}else if((grade2[player]>=16)&&(grade2[player]<=26)){//S8-m9
				if ( lines == 1 ) {
					i[player] = 3;
				} else if ( lines == 2 ) {
					i[player] = 10;
				} else if ( lines == 3 ) {
					i[player] = 15;
				} else {
					i[player] = 20;
				}
			}else if(grade2[player]>=27){//M以上
				if ( lines == 1 ) {
					i[player] = 1;
				} else if ( lines == 2 ) {
					i[player] = 7;
				} else if ( lines == 3 ) {
					i[player] = 10;
				} else {
					i[player] = 15;
				}
			}
		}

		if( ( (skillg[player] == 15) && (skillbai[player] == 0) )
			 || ( (skillg[player] == 30) && (skillbai[player] == 1) )
			 || ( (skillg[player] == 42) && (skillbai[player] == 2) )
			 || ( (skillg[player] == 50) && (skillbai[player] == 3) ) )
		{
			skillbai[player]++;
		}

		if(enable_grade[player] == 2){
			gbai[player] = ((tc[player] / 250) + 1) + (skillbai[player + 1]);//倍率(レベルとSK数で決定)
		}else if(enable_grade[player] == 3){
			gbai[player] = ((tc[player] / 250) + 1)*2;//
		}

		if(gbai[player] < 2)gbai[player]= 2;//1以下にはならない

		i[player] = (i[player] * gbai[player]) / 2;

		if(((combo[player]-1)!=0)&&(combo2[player]<3)){//2コンボまで
			if(lines == 2){
		 		i[player] = (i[player] * 6 * (combo2[player] - 1)) / 5;//1.2倍
			}else if(lines == 3){
				i[player] = (i[player] * 13 * (combo2[player] - 1)) / 10;//1.3倍
			}else if(lines == 4){
				i[player] = (i[player] * 3 * (combo2[player] - 1)) / 2;//1.5倍
			}
		}else if(((combo[player]-1)!=0)&&(combo2[player]>=3)&&(combo2[player]<5)){//3コンボ以上
			if(lines == 1){		//コンボ主体では段位ポイントが上がりにくく C7T7.1
				i[player] = (i[player] * 21*(combo[player] - 1)) /20;//1.05倍
		 	}else if(lines == 2){
		 		i[player] = (i[player] * 11 * (combo2[player] - 1)) / 10;//1.1倍
			}else if(lines == 3){
				i[player] = (i[player] * 6 * (combo2[player] - 1)) / 5;//1.2倍
			}else if(lines == 4){
				i[player] = (i[player] * 3 * (combo2[player] - 1)) / 2;//1.5倍
			}
		}

		gpoint[player] = gpoint[player] + i[player];
		if(enable_grade[player] == 2){
			if(medal_sk[player] == 4)gbai[player] = 10;//SKメダルが4になると4倍
			if( (gpoint[player] >= 100) &&(grade[player] <= 28)) {//MV
				gup_down[player] = 1;
				gflash[player] = 120;
				PlaySE(30);//rankup.wav
				gtime[player] = 0;
				grade[player]++;
				gpoint[player] = ((gpoint[player] - 100)/ ((tc[player] / 350) + 1 + (combo2[player] / 2)));//持ち越し
			}
					// gmflag1成立判定#C7T5EX
			if((tc[player] >= 500) && (!gmflag1_e[player])) {
				if((grade[player] >= 11) && (time[player] <= 300 * 60)){
					gmflag1[player] = 1;
					objectCreate2(player, 8, Rand(20) + 180 + 192 * player - 96 * maxPlay, 20 + Rand(10), 0, 0, 0, 0);
				}
				gmflag1_e[player] = 1;
			}

			// gmflag2成立判定#C7T5EX
			if((tc[player] >= 700) && (!gmflag2_e[player])) {
				if((grade[player] >= 19) && (time[player] <= 390*60)){
					gmflag2[player] = 1;
					objectCreate2(player, 8, Rand(20) + 180 + 192 * player - 96 * maxPlay, 20 + Rand(10), 0, 0, 0, 0);
				}
				gmflag2_e[player] = 1;
			}
		}else if(enable_grade[player] == 3){
			if( (gpoint[player] >= 100) &&(grade2[player] <= 29)) {//MM
				gup_down[player] = 1;

				gtime[player] = 0;
				grade2[player]++;
				gpoint[player] = 0;
				if((grade2[player]==0)||(grade2[player]==1)||(grade2[player]==2)||(grade2[player]==3)||
				(grade2[player]==4)||(grade2[player]==5)||(grade2[player]==6)||(grade2[player]==8)||
				(grade2[player]==11)||(grade2[player]==14)||(grade2[player]==17)||(grade2[player]==18)||
				(grade2[player]==19)||(grade2[player]==22)||(grade2[player]==24)||(grade2[player]==26)||
				(grade2[player]==28)||(grade2[player]==30)){
					PlaySE(30);//rankup音
					gflash[player] = 120;
				}else {
					PlaySE(46);//cool音
				}
			}//19段位変換
			if(grade2[player]==0)
				grade[player]=0;//9
			if(grade2[player]==1)
				grade[player]=1;//8}
			if(grade2[player]==2)//7
				grade[player]=2;//7
			if(grade2[player]==3)//6
				grade[player]=3;//6
			if(grade2[player]==4)//
				grade[player]=4;//5
			if(grade2[player]==5)
				grade[player]=5;//4
			if((grade2[player]==6)||(grade2[player]==7))//21
				grade[player]=6;//3
			if((grade2[player]==8)||(grade2[player]==9)||(grade2[player]==10))//S1S2S3
				grade[player]=7;//2
			if((grade2[player]==11)||(grade2[player]==12)||(grade2[player]==13))//S4S5S6
				grade[player]=8;//1

			if((grade2[player]==14)||(grade2[player]==15)||(grade2[player]==16))//S7S8S9
				grade[player]=9;//S1
			if(grade2[player]==17)//m1
				grade[player]=10;//S2
			if(grade2[player]==18)//m2
				grade[player]=11;//S3
			if((grade2[player]==19)||(grade2[player]==20)||(grade2[player]==21))//m3m4
				grade[player]=12;//S4
			if((grade2[player]==22)||(grade2[player]==23))//m5m6
				grade[player]=13;//S5
			if((grade2[player]==24)||(grade2[player]==25))//m7m8
				grade[player]=14;//S6
			if((grade2[player]==26)||(grade2[player]==27))//m9M
				grade[player]=15;//S7
			if((grade2[player]==28)||(grade2[player]==29))//MKMV
				grade[player]=16;//S8
			if(grade2[player]>=30)//MM
				grade[player]=17;//S9

		//grade3つづき

			// gmflag1成立判定#C7T5EX
			if((tc[player] >= 500) && (!gmflag1_e[player])) {
				if((grade2[player] >= 11) && (time[player] <= 300 * 60)){
					gmflag1[player] = 1;
					objectCreate2(player, 8, Rand(20) + 180 + 192 * player - 96 * maxPlay, 20 + Rand(10), 0, 0, 0, 0);
				}
			gmflag1_e[player] = 1;
			}

			// gmflag2成立判定#C7T5EX
			if((tc[player] >= 700) && (!gmflag2_e[player])) {
				if((grade2[player] >= 19) && (time[player] <= 390*60)){
					gmflag2[player] = 1;
					objectCreate2(player, 8, Rand(20) + 180 + 192 * player - 96 * maxPlay, 20 + Rand(10), 0, 0, 0, 0);
				}
				gmflag2_e[player] = 1;
			}
		}
	}
}

/* ??式段位　*/
void GradeUp3(int32_t player) {//ブロックを置く度に
	int32_t 	lap_timeT[2], isqcool;
	int32_t	gup3sec[2],secinlv[2];

	if( (gameMode[player] == 1) || (gameMode[player] == 2) ) {

	gup3sec[player] = tc[player]/100;//レベルからセクションを決定
	secinlv[player] = tc[player] - ((tc[player]/100)*100);
	lap_timeT[player] = ( timeN[player] / (1 + (((rots[player] == 6) || (rots[player] == 8)) && (repversw >= 30))) ) + (time99[player] * 2);//レベルストップ中は3倍加算
		// DS-WORLDで異様にCOOLが出にくいのでC7U1
	if((secinlv[player] > 80)&&(secinlv[player] < 99)&&(qualityflag[player] != 1)){//quality cool
		ave_laptime3[player] = lap_timeT[player] / secinlv[player];//セクションレベルで割る

		if(repversw < 49) isqcool = (ave_laptime3[player] < border_time48[border_rank[player]]);
		else if(repversw < 60) isqcool = (ave_laptime3[player] < border_time59[border_rank[player]]);
		else isqcool = (ave_laptime3[player] < border_time[border_rank[player]]);

		if(isqcool){//
			gup_down[player] = 1;//coolの文字を出す
			PlaySE(46);
			grade[player]++;	//段位を上げる
			qualityflag[player] = 1;
			gflash[player] = 240;
			gup3sp_adjust[player] = 2;	//速度スキップ
			if(gup3rank[player] <= 1){//0か1なら
				gup3rank[player]++;//ランクを上げる
			}
		}
	}
	if(secinlv[player]<94){//セクションまたぎは無効
		if((skillflag[player] != 1)){
			if(((skillg[player] >= 3)&&(tr2[player]<=10))||((skillg[player] >= 4)&&(tr2[player]>10))){//skill cool
				PlaySE(46);
				gup_down[player] = 1;
				skillg[player] = 0;
				gflash[player] = 240;
				grade[player]++;
				skillflag[player] = 1;
			}
		}
	}
	// gmflag1成立判定（border_ranklが15以上	// gmflag2成立判定はisregret内（regretが一回もでない
	if((tc[player] >= 400) &&(!gmflag1_e[player])) {
		if(tr2[player] >= 80){
			gmflag1[player] = 1;
			objectCreate2(player, 8, Rand(20) + 180 + 192 * player - 96 * maxPlay, 20 + Rand(10), 0, 0, 0, 0);
		}
		gmflag1_e[player] = 1;
	}

	}

}
void isregret(int32_t player){	//100超えるごと呼び出し
	int32_t	border_time2[2],regretgosa[2];
	regretgosa[player] = tr2[player]/40;
	if(repversw >= 49){
	border_time2[player]= border_time[border_rank[player]]+ 6 + regretgosa[player];
	}else{
	border_time2[player]= border_time48[border_rank[player]]+ 6 + regretgosa[player];
	}
	if(ave_laptime3[player]>border_time2[player]){//regret判定
		gup_down[player] = 0;//regretの文字を出す
		gflash[player] = 120;
		gmflag2[player] = 1;	// gmflag2成立剥奪
		if(grade[player]>0){
			grade[player]--;	//段位を上げる
		}
		if((gup3rank[player]==0)&&(gup3sp_adjust[player] == 0)){
			gup3sp_adjust[player] = 1;//速度
		}else{//1か2
			gup3rank[player]=0;//ランクダウン
			gup3sp_adjust[player] = 0;//速度やり直し
		}
	}else if(qualityflag[player] != 1){
		gup3sp_adjust[player] = 1;//速度そのまま
		if(gup3rank[player] == 2){	//2
			gup3rank[player]--;//ランクダウン
		}
	}
	ave_laptime3[player] = 0;
	border_rank[player] = border_rank[player] + gup3rank[player];//次の基準タイムを決める
	grade[player]++;
}

void reset_gradeup3(int32_t player){//セクションが変わるごとにリセット
	skillg[player] = 0;
	time99[player] = 0;
	timeN[player] = 0;
	qualityflag[player] = 0;
	skillflag[player] = 0;
	gup3sp_adjust[player] = 0;	//BGMフェードのためにこれも。
	regretcancel[player] = 0;
}
