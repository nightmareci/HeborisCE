//¤¥¤¥¤¥¤¥¤¥¤¥¤¥¤¥¤¥¤¥¤¥¤¥¤¥¤¥¤¥¤¥¤¥¤¥¤¥¤
//  ƒZƒNƒVƒ‡ƒ“ƒ^ƒCƒ€ƒ‰ƒ“ƒLƒ“ƒOŠÖ˜A
//£¢£¢£¢£¢£¢£¢£¢£¢£¢£¢£¢£¢£¢£¢£¢£¢£¢£¢£¢£
int		st_version = 2;	// ƒo[ƒWƒ‡ƒ“
						// 0:–³Œø 1:c7p6/
//30*3@(ƒrƒM+ƒ}ƒX123+20G), ƒfƒr+ƒ}ƒX4,ƒgƒ‚ ‚ª30‚Ã‚Â(‚½‚¾‚µ•\¦”‚Íˆá‚¤)
int		st_time[90];	// ƒ^ƒCƒ€
int		st_end[90];		// 0:–¢ƒJƒ“ƒXƒg 1:ƒ[ƒ‹“r’†’‚‘§ 2:Š®‘SƒNƒŠƒA
int		st_lvstop[90];		//ƒŒƒxƒ‹ƒXƒgƒbƒv‚µ‚½ŠÔ
int		st_others[90];		//—‚Æ‚µ‚½ƒuƒƒbƒN”

//str*		st_rkname[135];			// –¼‘O
// ‰Šú‰»
void ST_RankingInit(void) {
	int	i, j;

	for(i=0; i<90; i++) {
		st_time[i] = 5400;
		st_end[i] = 0;
		st_lvstop[i] = 0;
		st_others[i] = 0;
	}

}

// STƒƒ_ƒ‹‚ÌF‚ğ”»’è
int ST_RankingCheck(int player, int rmode, int section,int enable_grade) {
	int	tmp;

	if((rmode >= 4) || (rmode == 0)) return 0;

	tmp = ST_rankingGet(player,rmode,enable_grade);

	//-5•bXV
	if(lap_time[section + player * 100] <= st_time[section + tmp] - 300) {
		return 4;
	}
	// ‹L˜^XV
	if(lap_time[section + player * 100] <= st_time[section + tmp]) {
		return 3;
	}
	// 5•bˆÈ“à
	if(lap_time[section + player * 100] <= st_time[section + tmp] + 300) {
		return 2;
	}
	// 10•bˆÈ“à
	if(lap_time[section + player * 100] <= st_time[section + tmp] + 600) {
		return 1;
	}

	return 0;
}

// ‹L˜^XV‚µ‚½‚©ƒ`ƒFƒbƒN
int ST_RankingCheckAll(int player, int rmode, int enable_grade) {
	int i,tmp2;

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
// ƒXƒe[ƒW‚²‚Æ‚Ì‹L˜^XV‚µ‚½‚©ƒ`ƒFƒbƒN
int Stage_RankingCheck(int player, int rmode) {
	int i,tmp3;
	if(stage[player] >= 27) return 0;

	tmp3 = ST_rankingGet(player,rmode,0);

	if((stage_time[stage[player] + player * 30] <= st_time[stage[player] + tmp3]) && (stage_time[stage[player] + player * 30] != 0)) {
		return 1;
	}
	return 0;
}
// ƒZƒNƒVƒ‡ƒ“ƒ^ƒCƒ€ƒ‰ƒ“ƒLƒ“ƒOXV
void ST_RankingUpdate(int player, int rmode, int end,int enable_grade) {
	int i, temp,tmp4;

	tmp4 = ST_rankingGet(player,rmode,enable_grade);

	if(rmode == 6){//TOMOYO
		for(i=0; i<stage[player] + (stage[player] == laststage[player]); i++) {
			if((stage_time[i + player * 30] <= st_time[i + tmp4]) && (stage_time[i + player * 30] != 0)) {
				st_time[i + tmp4] = stage_time[i + player * 30];	// ƒ^ƒCƒ€
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
			st_time[i + tmp4] = lap_time[i + player * 100];	// ƒ^ƒCƒ€
			st_end[i + tmp4] = end;		// ƒGƒ“ƒfƒBƒ“ƒO“’B
			st_lvstop[i + tmp4] = lvstop_time[i + player * 20];
			st_others[i + tmp4] = st_other[i + player * 30];
		}
	}
}

// ƒtƒ@ƒCƒ‹ƒtƒH[ƒ}ƒbƒg (4byte’PˆÊ)
//   0      ƒo[ƒWƒ‡ƒ“
//   1`  9 –¢g—pi©—R‚Ég—p‰Â”
//  10` 61 ƒ^ƒCƒ€			10-90
//  62`113 ƒGƒ“ƒfƒBƒ“ƒO“’B		91-171
// 114`165 –¼‘O			172-252
//					253-333

// ƒZ[ƒu
void ST_RankingSave(void) {//12345 6789
	int i,j, temp2[3];

	FillMemory(&saveBuf, 5000 * 4, 0);

	saveBuf[0] = st_version;

	for(i=0;i<90;i++) {
		saveBuf[i + 10] = st_time[i];
		saveBuf[i + 99] = st_end[i];
		saveBuf[i + 188] = st_lvstop[i];
		saveBuf[i + 277] = st_others[i];
	}

	SaveFile("STRANKING.SAV", &saveBuf, 400 * 4);
}

// ƒ[ƒh
int ST_RankingLoad(void) {
	int i,j, temp2[3];

	FillMemory(&saveBuf, 5000 * 4, 0);

	// ƒo[ƒWƒ‡ƒ“‚ğŒ©‚é
	LoadFile("STRANKING.SAV", &saveBuf, 4);

	if(saveBuf[0] != st_version) {
		return 1;
	}

	// ‘S‘Ì‚ğ“Ç‚İ‚Ş
	LoadFile("STRANKING.SAV", &saveBuf, 400 * 4);

	for(i=0;i<90;i++) {
		st_time[i] = saveBuf[i + 10];
		st_end[i] = saveBuf[i + 99];
		st_lvstop[i] = saveBuf[i + 188];
		st_others[i] = saveBuf[i + 277];
	}
	return 0;
}
//ƒxƒXƒgƒ^ƒCƒ€(ACE‚Ì‚Íranking2.c)
void viewbestSTtime(int player){
	int	tempbest,color;

	if(Isbesttime==0)return;

	color = (count % 4 / 2) * (sp[player] >= 1200) * digitc[rots[player]];
	if((gameMode[player]>=0)&&(gameMode[player]<=3)){//ƒZƒNƒVƒ‡ƒ“ƒ^ƒCƒ€
	}else if((gameMode[player]==6) && (stage[player] < 27)){//ƒxƒXƒgƒ^ƒCƒ€
	//	if( (gameMode[player] == 6) && (!maxPlay) ) {
			printFont(26+2*((hnext[player] >= 4) && (!player)) + 7 * player - 12 * maxPlay, 11, "BEST TIME", fontc[rots[player]]);

			tempbest = ST_rankingGet(player,6,0);
			getTime(st_time[stage[player] + tempbest]);
			printFont(26+2*((hnext[player] >= 4) && (!player)) + 7 * player - 12 * maxPlay, 12, string[0], color);
	}
}
//ƒxƒXƒgƒ^ƒCƒ€ƒXƒ‚[ƒ‹ƒtƒHƒ“ƒg
void viewbestSTtimes(int player){
	int	tempbest,color;

	if(Isbesttime==0)return;

	color = (count % 4 / 2) * (sp[player] >= 1200) * digitc[rots[player]];
	if((gameMode[player]>=0)&&(gameMode[player]<=3)){//ƒZƒNƒVƒ‡ƒ“ƒ^ƒCƒ€

	}else if((gameMode[player]==6) && (stage[player] < 27)){//ƒxƒXƒgƒ^ƒCƒ€
		if( (gameMode[player] == 6) && ((!maxPlay) || (stat[1 - player] == 0)) ) {
			//best‚Ì•¶š
			ExBltRect(3, 208+20*((hnext[player] >= 4) && (!player)) + 70 * player - 96 * maxPlay, 95, 251, 91, 21, 7);
			//time‚Ì•¶š
			ExBltRect(3, 230+20*((hnext[player] >= 4) && (!player)) + 70 * player - 96 * maxPlay, 95, 180, 119, 19, 7);

			tempbest = ST_rankingGet(player,6,0);
			getTime(st_time[stage[player] + tempbest]);
			printSMALLFont((26 + 8 * player - 12 * maxPlay)*8+20*((hnext[player] >= 4) && (!player)), 103, string[0], color);
		}
	}
}
//STƒ‰ƒ“ƒLƒ“ƒO‚ÌƒAƒhƒŒƒXw’è
int ST_rankingGet(int player,int rmode,int enable_grade){
	int	temp;
	if(rmode == 0){//ƒrƒMƒi[
		temp = 0;
	}else if(rmode == 1){//ƒ}ƒXƒ^[
		if(enable_grade !=4){
		temp = 4;
		}else if(enable_grade==4){
		temp = 15;
		}
	}else if(rmode == 2){//20G
		temp = 30;
	}else if(rmode == 3){//ƒfƒrƒ‹
		temp = 40;
	}else if(rmode == 6){//TOMOYO
		temp =  60;
	}
	return temp;
}

// ƒZƒNƒVƒ‡ƒ“ƒ^ƒCƒ€ƒ‰ƒ“ƒLƒ“ƒO•\¦
void ST_RankingView() {
	int i, max, mode, tmp, tmp5, bps, bps1,bps2,s;

	mode = 1;

	while(1) {
		// ”wŒi•`‰æ
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

		//ƒAƒhƒŒƒXw’è
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

		// ƒ‚[ƒh–¼•\¦
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

		// •\¦”‚ğŒˆ‚ß‚é
		if(mode == 0)
			max = 2;
		else if( (mode == 1) || (mode == 2)|| (mode == 3) )
			max = 10;
		else if(mode == 4)
			max = 13;
		else if(mode == 5)
			max =27;

		// ƒ‰ƒ“ƒLƒ“ƒO•\¦
		for(i=0; i<max; i++) {
			if(st_end[i + tmp5] == 1)
				tmp = 4;
			else if(st_end[i + tmp5] == 2)
				tmp = 7;
			else
				tmp = 0;

			if(mode != 5){//‚»‚Ì‘¼
				if( ((mode == 1) || (mode == 2)|| (mode == 3)) && (i == 9) ){
					sprintf(string[0], " 900-999");
				} else if(mode <= 4){
					sprintf(string[0], "%4d-%3d", i*100, (i+1)*100);
				}

				printFont(1, 4+i, string[0], tmp);

				//ƒ^ƒCƒ€
				getTime(st_time[i + tmp5]);
				printFont(11, 4+i, string[0], tmp);

				//ƒŒƒxƒ‹ƒXƒgƒbƒv
				getSTime(st_lvstop[i + tmp5]);
				printFont(21, 4+i, string[0], tmp);

				//USEƒuƒƒbƒN”
				sprintf(string[0],"%d",st_others[i + tmp5]);
				printFont(29, 4+i, string[0], tmp);

				//BPS
				bps = (st_others[i + tmp5] * 1000) / (st_time[i + tmp5] / 60);
				bps1 = bps / 1000;//®”
				bps2 = bps % 1000;//‰ºOŒ…
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

					//ƒ^ƒCƒ€
					getTime(st_time[i + tmp5]);
					printFont(6, 4+i, string[0], tmp);

					//g—pƒuƒƒbƒN”
					sprintf(string[0],"%d",st_others[i + tmp5]);
					printFont(16, 4+i, string[0], tmp);

				}else if(i > 19){
					sprintf(string[0], "EX%d",i - 19);
					printFont(20, i - 16, string[0], tmp);

					//ƒ^ƒCƒ€
					getTime(st_time[i + tmp5]);
					printFont(26, i - 16, string[0], tmp);

					//g—pƒuƒƒbƒN”
					sprintf(string[0],"%d",st_others[i + tmp5]);
					printFont(36, i-16, string[0], tmp);
				}
			}
		}

		// ‡Œv”•\¦
		tmp = 0;
		for(i=0; i<max; i++) {
			tmp = tmp + st_time[i + tmp5];
		}
		printFont(1, 25, "TOTAL TIME", 1);
		getTime(tmp);
		printFont(12, 25, string[0], 2);

		KeyInput();

		if(getPushState(0, 4) || getPushState(0, 5)) {
			// A‚©B‚Å–ß‚é
			return;
		}

		if(getPushState(0, 2)) {
			// ©
			PlaySE(5);
			mode--;
			if(mode < 1) mode = 5;
		}
		if(getPushState(0, 3)) {
			// ¨
			PlaySE(5);
			mode++;
			if(mode > 5) mode = 1;
		}

		spriteTime();//halt
	}
}


// ƒtƒ@ƒCƒ‹ƒtƒH[ƒ}ƒbƒg (4byte’PˆÊ)
//   0      ƒo[ƒWƒ‡ƒ“
//   1` 10 ’iˆÊƒqƒXƒgƒŠ
//  10` 11 ”F’è’iˆÊ
//  12` 13

// ƒvƒŒƒCƒ„[ƒf[ƒ^ƒZ[ƒu
void PlayerdataSave(void) {//12345 6789
	int i,j, temp2[3];

	FillMemory(&saveBuf, 500 * 4, 0);

	saveBuf[0] = 0x4F424503;

	for( j = 0 ; j < 2 ; j++){
		for(i=0;i<5;i++) {
			saveBuf[1 + i + 5 * j] =grade_his[i+j*5];
		}
		saveBuf[1 + 10 + j]=admit_grade[j];
		saveBuf[1 + 12 + j]=grade_pasttime[j];
	}

	SaveFile("PLAYERDATA.SAV", &saveBuf, 100 * 4);
}

// ƒvƒŒƒCƒ„[ƒf[ƒ^ƒ[ƒh
int PlayerdataLoad(void) {
	int i,j, temp2[3];

	FillMemory(&saveBuf, 500 * 4, 0);

	// ƒo[ƒWƒ‡ƒ“‚ğŒ©‚é
	LoadFile("PLAYERDATA.SAV", &saveBuf, 4);

	if(saveBuf[0] != 0x4F424503) {
		return 1;
	}

	// ‘S‘Ì‚ğ“Ç‚İ‚Ş
	LoadFile("PLAYERDATA.SAV", &saveBuf, 100 * 4);

	for( j = 0 ; j < 2 ; j++){
		for(i=0;i<5;i++) {
			grade_his[i+j*5]=saveBuf[1 + i + 5 * j];
		}
		admit_grade[j] = saveBuf[1 + 10 + j];
		grade_pasttime[j]=saveBuf[1 + 12 + j];
	}
	return 0;
}
