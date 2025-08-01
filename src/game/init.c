#include "init.h"

/*
 ■目次

 ゲーム設定
 +-ゲーム全般
 +-段位認定
 +-足きりタイム
 +-連続スナップ
 +-捨てゲー
 +-アイテム出現率-

 サウンド設定
 +-BGMフェードアウト・切り替えタイミング

 表示設定
 +-背景アニメ設定

 せり上がり設定
 +-せり上がり全般
 +-PATTERN せり上がりパターン

 PRACTICEモード設定
 +-PRACTICE全般
 +-READ NEXT (ツモ固定)
 +-FAVORITES (スピード設定)

 デバッグ
*/

void gameInit() {
	//=============================================================
	// ゲーム設定
	//=============================================================

	//**********************************************************
	// ゲーム全般
	//**********************************************************
	// ランキングタイプ
	// 0=old　1=new 2=new
	last_ranking_type=2;
	ranking_type=2;

	//試験時のランキング登録可否(1で登録する)
	exam_ranking = 0;

	// 赤のIRSの種類
	// 0=ヘボリス型（床蹴りあり） 1=TGM型（壁蹴りなし）
	r_irs = 1;

	// ワールドルールのI型ブロックの壁蹴り
	// 0=左右対称タイプ 1=非左右対称タイプ
	world_i_rot = 1;

	// MASTERモード&タイムトライアルの時、一定条件を満たしてLV999を超えるとシャドウロールに#1.60c7h8
	// 0=ならない 1=なる
	shadow_heboris = 1;

	// bigginer〜devil、originalにおいて15分以上プレイすると超高速に#1.60c7h3
	// 0=ならない 1=なる
	limit15 = 2;

	// BIGモードの縦移動単位
	// 0=2マス 1=1マス
	ybigtype = 1;

	// READY GO の種類
	// 0=HEBORIS 1=LITE
	ready_go_style = 1;

	// READY GO のタイミング（HEBORIS） #1.60c7j5
	ready_start = 0;			// ready開始
	ready_end = 15;				// ready終了
	go_start = 45;				// go開始
	go_end = 60;				// go終了

	// TOMOYOモードでのREADY GO のタイミング（HEBORIS） #1.60c7l7
	s_ready_start = 45;			// ready開始
	s_ready_end = 60;			// ready終了
	s_go_start = 195;			// go開始
	s_go_end = 210;				// go終了

	// READY GO のタイミング（LITE） #1.60c7j5
	ti_ready_start = 20;		// 通常 ready開始
	ti_ready_end = 75;		// 通常 ready終了
	ti_go_start = 80;		// 通常 go開始
	ti_go_end = 140;		// 通常 go終了

	ti_s_ready_start = 45;		// TOMOYO ready開始
	ti_s_ready_end = 155;		// TOMOYO ready終了
	ti_s_go_start = 160;		// TOMOYO go開始
	ti_s_go_end = 225;		// TOMOYO go終了

#ifdef APP_ENABLE_KEYBOARD
	// TOMOYOモードでのステージスキップキー #1.60c7l8
	skipKey = SDL_SCANCODE_F5;
#endif

	// VERSUSモードの対戦ルール
	// 0=NORMAL	通常
	// 1=NOITEM	アイテムなし
	// 2=CEMENT	アイテムもせり上がりもなし
	// 3=NORISE	アイテムのみ
	vsmodesl = 0;

	// VERSUSモードの勝利条件
	// 1だと一戦で勝敗が決まります。
	// それ以上だと指定したポイントを先に取ったほうが勝ちになります
	winpoint = 2;

	// VERSUSモードのゴールレベル初期値
	// 50の倍数でない場合は200になります
	// 0=なし
	def_vs_goal = 200;

	// VERSUSモードのデフォルトせり上がりタイプ
	// 0=最下段コピー
	// 1=パターン
	// 2=ランダム
	// 3=消去した形を反転して送る
	def_vs_uplinetype = 3;

	// BEGINNERモードでの花火の有無
	// 0=あり 1=なし
	no_hanabi = 0;

	// ナビゲーションシステムの有効／無効
	// 0=無効 1=BEGINNERのみ有効
	navigation = 1;

	// ナビゲーション表示終了レベル
	navigation_limitLv = 100;

	// ナビゲーション1回の表示時間
	navigation_limittime = 60;

	//TOMOYOのステージ設定「SPEED」を有効にするか(0=しない　1=する)
	enable_minspeed = 0;

	//斜め入力での固定を有効にするか(0=しない　1=する) #C7T9.6EX
	nanamedown = 1;

	//**********************************************************
	// 段位認定
	//**********************************************************
	// マスター、20G開始時に低確率で段位認定試験（のような物）が
	// 0=発生しない 1=発生する 2=発生しかつ認定を使う
	enable_randexam = 2;
	//（HOLDボタンを押したまま開始すると強制的に発生させることが出来ます。）

	//試験段位の範囲
	exam_range[0] = 9;	// 段位1設定時の最低段位（0〜）
	exam_range[1] = 18;	// 　　〃　　 の最高段位（〜18）
	exam_range[2] = 9;	// 段位2設定時の最低段位（0〜）
	exam_range[3] = 28;	// 　　〃　　 の最高段位（〜32）
	exam_range[4] = 6;	// 段位3設定時の最低段位（0〜）
	exam_range[5] = 15;	// 　　〃　　 の最高段位（〜18）
	exam_range[6] = 9;	// 段位4設定時の最低段位（0〜）
	exam_range[7] = 27;	// 　　〃　　 の最高段位（〜32）

	// 0=9 1=8 2=7 3=6 4=5 5=4 6=3 7=2 8=1
	// 9=S1 10=S2 11=S3 12=S4 13=S5 14=S6 15=S7 16=S8 17=S9
	// 18=m1 19=m2 20=m3 21=m4 22=m5 23=m6 24=m7 25=m8 26=m9
	// 27=M 28=MK 29=MV 30=MO 31=MM 32=Gm


	//**********************************************************
	// 足きりタイム
	//**********************************************************
	// タイムトライアルLV500のとりカンの足きりタイムです。(フレーム単位)#1.60c7g7
	// とりカンなしにしたい人は 0 にしてください。
	//master
	timelimit_master = 420*60; // 7分
	//20G
	timelimit_20G = 360*60;    // 6分
	//DEVIL-DOOM 500(1000足切りはこれの2倍)
	timelimit_doom_E = 240*60;	//Easy 　初心者向け回転法則（ACE-SRS、ACE-ARS、DS-World） 4分
	timelimit_doom_N = 205*60;	//Normal 中級者向け回転法則（Heboris、Ti-World、ACE-ARS2）3分25秒
	timelimit_doom_H = 183*60;	//Hard　 上級・超上級者向け（Ti-ARS、SRS-X、D.R.S）	　3分3秒
	//DEVIL-MINUS
	timelimitm_devil  = 205*60;	//ARS 3分25秒
	timelimitmw_devil = 235*60;	//SRS 3分55秒

	//**********************************************************
	// 連続スナップ
	//**********************************************************
	// 注意：HEBORIS.EXEがあるフォルダ以下にcapフォルダを作らないと動作しません。
	capKey = SDL_SCANCODE_PRINTSCREEN;	//開始/終了キー
	capi = 2;	//取得間隔(フレーム単位)
	capx = 0;	//取得領域の左上X座標
	capy = 0;	//左上Y座標
	capw = 320;	//幅
	caph = 240;	//高さ

	//**********************************************************
	// 捨てゲー
	//**********************************************************
	// 捨てゲーした時の動作#1.60c7i6
	// 0=参加待ち 1=モードセレクト 2=タイトル
	giveup_func = 1;

	// 捨てゲー安全装置の有効/無効#1.60c7i6
	// 0=どこでも可能 1=ポーズ中だけ可能
	giveup_safety = 0;

	//**********************************************************
	// アイテム出現率
	//**********************************************************
	//！注意！　この値を変更すると、アイテム使用時の
	//	　　リプレイ互換性は一切保障されません。

	//0:全く出ない
	//1:ほとんど出ない　〜　9:よく出る
	item_pro[0] =5;		//1:MIRROR
	item_pro[1] =8;		//2:ROLL ROLL
	item_pro[2] =5;		//3:DEATH
	item_pro[3] =9;		//4:X-RAY
	item_pro[4] =6;		//5:COLOR
	item_pro[5] =3;		//6:ROTATELOCK
	item_pro[6] =5;		//7:HIDENEXT
	item_pro[7] =4;		//8:MAGNET
	item_pro[8] =6;		//9:TIMESTOP
	item_pro[9] =8;		//10:HOLDLOCK

	item_pro[10] =4;	//11:→←REV
	item_pro[11] =7;	//12:BOOST
	item_pro[12] =5;	//13:FEVER
	item_pro[13] =6;	//14:↑↓REV
	item_pro[14] =5;	//15:REMOTE CONTROL
	item_pro[15] =3;	//16:DARK
	item_pro[16] =6;	//17:↑DEL
	item_pro[17] =6;	//18:↓DEL
	item_pro[18] =5;	//19:DELEVEN
	item_pro[19] =4;	//20:TRANSFORM

	item_pro[20] =9;	//21:LASER
	item_pro[21] =1;	//22:NEGA
	item_pro[22] =5;	//23:SHOTGUN
	item_pro[23] =1;	//24:EXCHG
	item_pro[24] =7;	//25:HARD
	item_pro[25] =7;	//26:SHUFFLE
	item_pro[26] =8;	//27:RANDOM
	item_pro[27] =3;	//28:FREEFALL
	item_pro[28] =2;	//29:←MOV
	item_pro[29] =5;	//30:→MOV

	item_pro[30] =7;	//31:180°
	item_pro[31] =4;	//32:16t
	item_pro[32] =3;	//33:REFLECT
	item_pro[33] =3;	//34:DOUBLE
	item_pro[34] =1;	//35:ALLCLEAR
	item_pro[35] =5;	//36:MISS
	item_pro[36] =3;	//37:COPYFLD
	item_pro[37] =2;	//38:FAKENEXT
	item_pro[38] =7;	//39:[]BLOCK

	//=============================================================
	// サウンド設定
	//=============================================================
	// HOLD使用時のサウンドの有無（0=なし 1=IHS時のみ 2=HOLDを使ったら必ず）
	hold_snd = 1;

	// 回転音の有無#1.60c7f7 (0=なし 1=あり)
	rotate_snd = 0;

	// ワールドルールでの接地音の有無（0=なし 1=あり 2=即落下以外あり 3=設置状態での移動と回転以外はあり）#1.60c7m9
	world_drop_snd = 2;

	//BEGINNER以外のモードのスタッフロール曲をループさせる(1で有効)
	nmlroll_bgmloop = 1;

	//BEGINNERモードのスタッフロール曲をループさせる(1で有効)
	bgnroll_bgmloop = 1;

	//BEGINNERロールでのBGM(0=ending_b.wav 1=bgm02.wav 2=ending.wav)
	beginner_rollbgm = 0;

	//**********************************************************
	// BGMフェードアウト・切り替えタイミング
	//**********************************************************
	//BGMフェードアウト速度
	//数値が大きいほど早く、0にするとフェードアウトしません。
	fade_seed = 100;

	//BGMのフェードアウトが開始するレベルを指定します。#1.60c7i5
	//フェードアウト後、レベルアップ音が再生されるタイミングになると次のBGMが再生されます。
	//BGMは BGM01.wav → BGM02.wav → BGM03.wav → BGM04.wav → BGM05.wav → BGM06.wav の順に再生されます。

	//特殊な値=
	//-1    最初のフェードアウトレベルをこれにすると、いきなりBGM02.wavを再生
	//      その次のフェードアウトレベルもこれにすると、いきなりBGM03.wavを再生
	//10000 これ以上は切り替えを行わないずに初めに10000を指定した部分の曲がカンストまでそのまま流れ続ける
	//BGMをかえない→すべて10000
	//500でしかかえない→485のみ

	// BEGINNER(tamaya)
	bgmfadestart[0] = 185;//BGM01.wav
	bgmfadestart[1] = 10000;//BGM02.wav
	bgmfadestart[2] = 10000;//BGM03.wav
	bgmfadestart[3] = 10000;//BGM04.wav
	bgmfadestart[4] = 10000;//BGM05.wav
	bgmfadestart[5] = 10000;//BGM06.wav
	// BEGINNER(score)
	bgmfadestart[6] = 285;
	bgmfadestart[7] = 10000;
	bgmfadestart[8] = 10000;
	bgmfadestart[9] = 10000;
	bgmfadestart[10] = 10000;
	bgmfadestart[11] = 10000;
	// MASTER(grade1)
	bgmfadestart[12] = 485;
	bgmfadestart[13] = 10000;
	bgmfadestart[14] = 10000;
	bgmfadestart[15] = 10000;
	bgmfadestart[16] = 10000;
	bgmfadestart[17] = 10000;
	// MASTER(grade2,3)
	bgmfadestart[18] = 485;
	bgmfadestart[19] = 685;
	bgmfadestart[20] = 895;
	bgmfadestart[21] = 10000;
	bgmfadestart[22] = 10000;
	bgmfadestart[23] = 10000;
	// MASTER(grade4)
	// 今のところ機能しません
	bgmfadestart[24] = 490;
	bgmfadestart[25] = 690;
	bgmfadestart[26] = 10000;
	bgmfadestart[27] = 10000;
	bgmfadestart[28] = 10000;
	bgmfadestart[29] = 10000;
	// DEVIL-DOOM
	bgmfadestart[30] = -1;//スキップ
	bgmfadestart[31] = -1;//スキップ
	bgmfadestart[32] = 485;//これから流す
	bgmfadestart[33] = 685;
	bgmfadestart[34] = 985;
	bgmfadestart[35] = 10000;
	// DEVIL-MINUS
	bgmfadestart[36] = -1;
	bgmfadestart[37] = 285;
	bgmfadestart[38] = 485;
	bgmfadestart[39] = 10000;
	bgmfadestart[40] = 10000;
	bgmfadestart[41] = 10000;

	// ACE (NORMAL)　値はレベルではなく消去ライン数
	ace_bgmfadestart[0] = 45;
	ace_bgmfadestart[1] = 95;
	ace_bgmfadestart[2] = 145;

	// ACE (ANOTHER)　値はレベルではなく消去ライン数
	ace_bgmfadestart[3] = 35;
	ace_bgmfadestart[4] = 95;
	ace_bgmfadestart[5] = 145;

	// ACE (HELL)　値はレベルではなく消去ライン数
	ace_bgmfadestart[6] = 45;
	ace_bgmfadestart[7] = 145;
	ace_bgmfadestart[8]= 195;

	// OLD-STYLE　値はレベルではなく消去ライン数
	ace_bgmfadestart[9] = 45;
	ace_bgmfadestart[10] = 145;
	ace_bgmfadestart[11]= 195;
	//ここまで 複雑でｺﾞﾒｿorz

	// 以下の2モードは使用する曲の順番が指定できます。
	// (0=bgm01.wav 1=bgm02.wav 2=bgm03.wav 3=bgm04.wav 4=bgm05.wav 5=bgm06.wav)

	// ACE (NORMAL) 曲構成
	ace_bgmlist[0] = 0;
	ace_bgmlist[1] = 1;
	ace_bgmlist[2] = 2;

	// ACE (ANOTHER) 曲構成
	ace_bgmlist[3] = 5;
	ace_bgmlist[4] = 3;
	ace_bgmlist[5] = 4;

	// ACE (HELL) 曲構成
	ace_bgmlist[6] = 19;
	ace_bgmlist[7] = 18;
	ace_bgmlist[8] = 16;

	// ACE (OLD) 曲構成
	ace_bgmlist[9]  = 8;
	ace_bgmlist[10] = 9;
	ace_bgmlist[11] = 17;
	//=============================================================
	// 表示設定
	//=============================================================
	//テキストガイド
	//0=off 1=on
	textguide = 1;

	//上部フレーム C7T4 (0=TI風 1=ACE風)
	//※ゲーム内のSETTING > DESIGNに移動しました。

	//対戦モード専用の背景を使用するか（0=1番目を使用　1=専用の物を使用）
	vsbg = 1;

	//ミラーのフィールド回転演出を高速化
	//ただし、フィールド反転後の待ち時間がその分長くなるので、
	//実質的なミラー時間は同じ (0=なし 1=あり)
	fastmirror = 1;

	//ブロックを立体っぽく表示する機能の無効レベル
	//0=全て表示　1=BIGのTLSのみ無効　2=TLSのみ無効　3=フィールド上のブロックのみ有効　4=全て無効
	disable_giji3D = 0;

	//TLS（ゴーストピース）の描画タイプ
	//0=全て暗い不透明　1=BIGのみ暗い不透明、他は半透明　2=全て半透明
	alpha_tls = 0;

	// 窒息後の結果表示の有無#1.60c7f3 (0=なし 1=あり)
	show_result = 1;

	// 死亡アニメの種類#1.60c7h3 (0=下からブロックが消えていく 1=上から灰色化)
	deadtype = 0;

	// 破壊エフェクト#1.60c7h8 (0=HEBORIS/TGM 1=TAP/TI 2=旧TAP/TI 3=BEGINNERモードのみHEBORIS/TGM、それ以外TAP/TI)
	breaktype = 3;

	// 破壊エフェクトの画像を微妙に変更（breaktype = 1またはbreaktype = 3のときのみ）
	// 0=無効 1=有効
	breakti = 1;

	// 全てのブロックで破壊エフェクトを表示するか #1.60c7m9
	// 0の場合、ラインを消しても5ブロックしか飛び散らず、あとのブロックは一瞬で消えます。
	// 1の場合、全てのブロックで破壊エフェクトを表示します。ただし重くなります。
	// 2の場合、TGM破壊エフェクトの場合だけ全部表示します。
	super_breakeffect = 2;

	// 全消しした時にブロックが飛び散るエフェクトの有無#1.60c7f6 (0=なし 1=あり)
	// (ただしシングル台だとズレまくり)
	bravo = 0;

	// HOLDした時の黄色の枠 (0=なし 1=あり)#1.60c7i1
	enable_hold_frame = 1;

	// スプリットタイムとラップタイム表示の有無#1.60c7f7 (0=あり 1=なし)
	hide_st_record = 1;

	// セクションタイムを何レベル毎に記録をとるか
	// HeboLvは1Lv毎、TGMLvは10Lv毎に設定可能
	// 最小値は2、それ未満はバグになるかも
	st_record_interval_tgm = 10; // 100TGM    Level毎にタイム更新→st_record_interval_tgmの10倍毎にタイム更新
	st_record_interval_heb =  6; //   6HeborisLevel毎にタイム更新

	// シングル台の時、スプリットタイムとラップタイムをどこに表示するか #1.60c7j8
	// 0=左と右 1=右だけ
	st_record_force_viewright = 1;

	// この段から裏段位を表示
	// 19以上で非表示です
	min_sgrade = 4;

	// ブロックが見えなくなるスタッフロールで設置したブロックの
	// 周囲枠を表示 C7U4.3
	// 0=しない 1=する
	m_roll_blockframe = 1;

	// 死んだときフィールド上のブロックの枠を全て消す #1.60c7k9
	// 0=消さない 1=消す
	dead_blockframe = 1;

	// スコアとライン数表示の有無#1.60c7f3 (0=あり 1=なし)
	hide_score = 0;

	// メダルに関する情報を表示 (4段消し、全消し、コンボ、レスキュー、STメダル個数)
	// 0=しない 1=する
	medal_info = 0;

	// HIDDEN LV表示の有無#1.60c7i9 (0=あり 1=なし)
	hide_hidden = 1;

	// FPS表示の有無#1.60c7e (0=あり 1=なし)
	hide_fps = 1;

	// スコア・タイムなどを全て表示しない(上の設定全てを無視)#1.60c7i9
	// 0=無効 1=有効
	skip_viewscore = 0;

	// 背景の有無#1.60c7i9
	// 0=あり 1=なし
	skip_viewbg = 0;

	// 全ての演出を無効に#1.60c7i9
	// 0=しない 1=する
	skip_obj = 0;

	// X-RAY時に背景を真っ黒にする#1.60c7p9ex
	// 0=あり 1=なし
	xray_bgblack = 1;

	// X-RAY時にブロックの枠線を描く#1.60c7p9ex
	// 0=あり 1=なし
	xray_drawblockframe = 0;

	// COLOR時に背景を真っ黒にする#1.60c7p9ex
	// 0=あり 1=なし
	color_bgblack = 0;

	// HEBOルールとTIルールのブロックの色 #1.60c7p1
	// 0=黒 1=赤 2=橙 3=黄 4=緑 5=水 6=青 7=紫 8=[] 9=[]
	ccol[0] = 1;	//棒
	ccol[1] = 2;	//L
	ccol[2] = 3;	//■
	ccol[3] = 4;	//Z
	ccol[4] = 5;	//凸
	ccol[5] = 6;	//J
	ccol[6] = 7;	//逆Z

	// SRS系ルールのブロックの色 #1.60c7p1
	// 0=黒 1=赤 2=橙 3=黄 4=緑 5=水 6=青 7=紫 8=[] 9=[]
	wcol[0] = 5;	//棒
	wcol[1] = 2;	//L
	wcol[2] = 3;	//■
	wcol[3] = 1;	//Z
	wcol[4] = 7;	//凸
	wcol[5] = 6;	//J
	wcol[6] = 4;	//逆Z

	// ACE-ARS系ルールのブロックの色 #1.60c7q2ex
	// 0=黒 1=赤 2=橙 3=黄 4=緑 5=水 6=青 7=紫 8=[] 9=[]
	acol[0] = 5;	//棒
	acol[1] = 2;	//L
	acol[2] = 3;	//■
	acol[3] = 1;	//Z
	acol[4] = 7;	//凸
	acol[5] = 6;	//J
	acol[6] = 4;	//逆Z

	// 鏡像の設定
	// 0=無効 1=有効
	mirror = 0;

	// TOMOYOモードでステージクリア時にTOTAL TIMEを表示するか
	// 0=あり 1=なし
	hide_tomoyo_totaltime = 1;

	// 背景を切り替える回数(0-11まで)
	bg_max = 11;

	// TLS（ゴーストピース）
	// 0=自動 1=常に有効 2=常に無効
	tls_view = 0;

	// []ブロックの開始レベル
	// デフォルトは1000
	over1000_start = 1000;

	//TOMOYO-EHeart最終面の仕掛け（２つのフィールドが明るくなったり暗くなったり）を消す
	//0=消さない　1=消す
	disable_ehlast_gimmick = 0; 
	
	//**********************************************************
	// 背景アニメ設定
	//**********************************************************
	// 背景をアニメーションさせるかどうかの設定です。
	//（0=静止画　1〜 =指定したコマ数のアニメーション）
	// アニメーションさせる場合は、メモリ容量にご注意ください。
	title_mov_f = 0;			// タイトル
	back_mov_f[0]  = 0;			// back01.png
	back_mov_f[1]  = 0;			// back02.png
	back_mov_f[2]  = 0;			// back03.png
	back_mov_f[3]  = 0;			// back04.png
	back_mov_f[4]  = 0;			// back05.png
	back_mov_f[5]  = 0;			// back06.png
	back_mov_f[6]  = 0;			// back07.png
	back_mov_f[7]  = 0;			// back08.png
	back_mov_f[8]  = 0;			// back09.png
	back_mov_f[9]  = 0;			// back10.png
	back_mov_f[10] = 0;			// back11.png
	back_mov_f[11] = 0;			// back12.png
	back_mov_f[12] = 0;			// back_vs.png

	// 背景をアニメーションさせる場合の、1コマを表示するフレーム数
	// 注意：0にするとクラッシュします
	mov_inter = 2;				// タイトル
	back_mov_inter[0]  = 2;			// back01.png
	back_mov_inter[1]  = 2;			// back02.png
	back_mov_inter[2]  = 2;			// back03.png
	back_mov_inter[3]  = 2;			// back04.png
	back_mov_inter[4]  = 2;			// back05.png
	back_mov_inter[5]  = 2;			// back06.png
	back_mov_inter[6]  = 2;			// back07.png
	back_mov_inter[7]  = 2;			// back08.png
	back_mov_inter[8]  = 2;			// back09.png
	back_mov_inter[9]  = 2;			// back10.png
	back_mov_inter[10] = 2;			// back11.png
	back_mov_inter[11] = 2;			// back12.png
	back_mov_inter[12] = 2;			// back_vs.png

	//=============================================================
	// せり上がり設定
	//=============================================================

	//**********************************************************
	// せり上がり全般
	//**********************************************************
	// DEVILモードのせり上がり間隔をランダムにする（1=ON　2=OFF）
	devil_randrise = 1;

	// 何ラインせり上がるか#1.60c6.2f(旧デフォルトは2 実物は1)
	p_shirase_line = 1;

	// 何個ブロックを置くごとにせり上がるか#1.60c6.2f(デフォルトは20)
	p_shirase_interval = 20;

	// BIG時のせり上がりライン数 #1.60c7r2
	// 0=1倍 1=2倍
	big_rise = 1;

	//**********************************************************
	// PATTERN せり上がりパターン
	//**********************************************************
	upLineNoMax = 58; //読み込むライン総数(デフォルトは32、多めに設定しておいても良いが上限は128)

	// フィールドは、左右に分けて指定。(もちろんhogeパッチからのパクリ)
	// 0=ブロックなし,1=ブロックあり、全て0や全て1のラインは無視されます。
	upFld[ 0]= 1111;upFld[ 1]=11111;//１番目にせり上がるライン。偶数が左側、奇数が右側。
	upFld[ 2]= 1111;upFld[ 3]=11111;//２番目にせり上がるライン。
	upFld[ 4]= 1111;upFld[ 5]=11111;
	upFld[ 6]= 1111;upFld[ 7]=11111;
	upFld[ 8]=11111;upFld[ 9]=11110;
	upFld[10]=11111;upFld[11]=11110;
	upFld[12]=11111;upFld[13]=11110;
	upFld[14]=11111;upFld[15]=11110;
	upFld[16]=  111;upFld[17]=11111;
	upFld[18]= 1111;upFld[19]=11111;
	upFld[20]= 1111;upFld[21]=11111;
	upFld[22]=11111;upFld[23]=11100;
	upFld[24]=11111;upFld[25]=11110;
	upFld[26]=11111;upFld[27]=11110;
	upFld[28]=11011;upFld[29]=11111;
	upFld[30]=10011;upFld[31]=11111;
	upFld[32]=10111;upFld[33]=11111;
	upFld[34]=11111;upFld[35]=11011;
	upFld[36]=11111;upFld[37]=11001;
	upFld[38]=11111;upFld[39]=11101;
	upFld[40]=11110;upFld[41]= 1111;
	upFld[42]=11110;upFld[43]= 1111;
	upFld[44]=11110;upFld[45]=11111;
	upFld[46]=11100;upFld[47]= 1111;
	upFld[48]=11111;upFld[49]=11110;
	upFld[50]=11111;upFld[51]=11110;
	upFld[52]=10110;upFld[53]=11111;
	upFld[54]=10110;upFld[55]=11111;
	upFld[56]=10000;upFld[57]=11111;
	upFld[58]=10110;upFld[59]=11111;
	upFld[60]=10110;upFld[61]=11111;
	upFld[62]=11111;upFld[63]=10001;
	upFld[64]=11111;upFld[65]=10111;
	upFld[66]=11111;upFld[67]=10011;
	upFld[68]=11111;upFld[69]=10111;
	upFld[70]=11111;upFld[71]=10001;
	upFld[72]=10011;upFld[73]=11111;
	upFld[74]=10101;upFld[75]=11111;
	upFld[76]=10001;upFld[77]=11111;
	upFld[78]=10110;upFld[79]=11111;
	upFld[80]=10001;upFld[81]=11111;
	upFld[82]=11111;upFld[83]=10001;
	upFld[84]=11111;upFld[85]= 1110;
	upFld[86]=11111;upFld[87]= 1110;
	upFld[88]=11111;upFld[89]=10001;
	upFld[90]=10001;upFld[91]=11111;
	upFld[92]=10110;upFld[93]=11111;
	upFld[94]=10001;upFld[95]=11111;
	upFld[96]=10110;upFld[97]=11111;
	upFld[98]=11111;upFld[99]=10001;
	upFld[100]=11111;upFld[101]=11011;
	upFld[102]=11111;upFld[103]=11011;
	upFld[104]=11111;upFld[105]=10001;
	upFld[106]=11100;upFld[107]=  111;
	upFld[108]=11011;upFld[109]=11111;
	upFld[110]=11100;upFld[111]= 1111;
	upFld[112]=11111;upFld[113]=10111;
	upFld[114]=11000;upFld[115]= 1111;

	//=============================================================
	// PRACTICEモード設定
	//=============================================================

	//**********************************************************
	// PRACTICE全般
	//**********************************************************
	//PRACTICEの[] BLOCKのデフォルト値#1.60c7e 0=ノーマル 1=モノクロ　2=[]
	p_monochrome_mode = 0;

	// PRACTICEで流すデフォルトのBGM番号#1.60c6.2d
	// （0=Lv500↓　1=Lv500↑　2=Lv700-900　3=Lv900-）
	p_bgmlv = 1;

	// 背景(0〜11まで デフォルトは9)#1.60c7c
	p_backno = 11;

	// TLSの範囲
	// 0=OFF 1=LV100までON 2=常にON
	tlsrange = 1;

	// ツモ送り有無 (0=OFF 1=ON) #1.60c7j5
	p_nextpass = 0;

	// 初期HOLDブロック (-1=なし 0=赤,1=橙,2=黄,3=緑,4=水,5=青,6=紫) #1.60c7j5
	p_hold = -1;

	// 読み込むステージのデフォルト
	// -1=なし
	//0-19=TOMOYO通常
	//20-26=EXステージ
	//27-44=E-Heartステージ
	//47-67=ACEターゲットステージ
	//100-200=F-Pointステージ
	p_stage = -1;

	// フィールドミラー間隔 (0=OFF) #1.60c7j5
	p_fmirror_interval = 3;

	// フィールドミラータイマー (フィールド反転中にかかる時間) #1.60c7j5
	p_fmirror_timer = 10;

	// ロールロール間隔 (0=OFF) #1.60c7j5
	p_rollroll_interval = 4;

	// ロールロールタイマー (回転するまでにかかる時間) #1.60c7j5
	p_rollroll_timer = 30;

	// シャドウタイマー (普通ロールなどでブロックを固定してから消えるまでの時間 0=常に消えない) #1.60c7j5
	p_shadow_timer = 300;

	// シャドウタイマーの種類(0=パッと消える 1=徐々に消える)
	shadow_timer_type = 1;

	//**********************************************************
	// READ NEXT (ツモ固定)
	//**********************************************************
	// PRACTICEでツモのリストを読み込めます。#1.60c7g3
	// 文字列なので" "（ダブルクォーテーション）で括ります。複数行に渡るときは+記号でつなげて下さい。
	// 0=赤,1=橙,2=黄,3=緑,4=水,5=青,6=紫
	static const char *s_nextb_list =
		"1052463015240653120563402534162340621456034251036420314526014362045136455062150461320365204631546310"
		"6451324023650143620435621456302513025430312603452013625026345012660132450346213462054360143260534215"
		"0621543621435624013542130562345123641230462134502613542";
	strcpy(nextb_list, s_nextb_list);

	// 電源パターンツモのリスト
	static const char *s_nextdengen_list =
	"4040050165233516506133350555213560141520224542633206134255165200333560031332022463366645230432611435"
	"5335503262512313515002442203656664131543211220146344201325061401134610644005663441101532234006340505"
	"4621441004021465010225623313311635326311133504346120621126223156523530636115044065300222245330252325"
	"5563545455656660124120450663502223206465164461126135621055103645066644052535021110020361422122352566"
	"1564343513043465103636404534525056551422631026052022163516150316500504641606133253660234134530365424"
	"4124644510156225214120146050543513004022131140054341604166064441010614144404145451160041314635320626"
	"0246251556635262420616451361336106153451563316660054255631510320566516465265421144640513424316315421"
	"6644140264401653410103024436251016522052305506020020331200443440341001604426324366453255122653512056"
	"4234334231212152312006153023444306242003331046140330636540231321265610510125435251421621035523001404"
	"0335464640401464125332132315552404146634264364245513600336065666305002023203545052006445544450440460";
	strcpy(nextdengen_list, s_nextdengen_list);

	static const char *s_nextfp_list =
	"6554550456120244500603524436302425326633362036436052202055321521645455643510264146640306114515244450"
	"4621006053256333034663614136164125646162203014235603434111014421443420136305543632604463351003005300"
	"3113034240614442346145603411201240043660365400542355605502540164151322136554135162536362611511544650"
	"0436003524541031324041121462562345432154451606022103263352550161231140341211033450330633020655146403"
	"4111312042106260052442623214250206561233065501645366322224531632336325105565036321260443364135060462"
	"6424413305662001514114115255244163505645360625360520123310461362403656351553004320234104442443443126"
	"2520622500465633022456043005425350040633021141556064304663022201515200124624202600431500352266062024"
	"0136151422012430242213242602553531464030104514303214051343025324445132610201464236045620365643346206"
	"2141403232202632664016405435063632253420601016604132246604110232541206463226626641062454431434333226"
	"3012131102643663452403012405552261460461540462345656555355534405144012655553302364654530404653013224";
	strcpy(nextfp_list, s_nextfp_list);

	//**********************************************************
	// FAVORITES (スピード設定)
	//**********************************************************
	// デフォルトの設定の番号（0〜skip_fwait）
	def_p_sp = 0;

	// これから先のFAVORITESの値は表示しない
	//(FAVORITESの項目数より大きい値を指定すると確実に落ちるので注意)
	skip_fwait = 19;

	//設定名。
	strcpy(waitname[0], "BEGINNER");

	//ブロックが固定されてから、次のブロックが出現するまでの時間
	//ARE　
	syutugen[0] = 26;

	//ブロックが一列揃ったときの消滅の待ち時間
	//(消滅のエフェクトの速さは、syutugenも影響します)
	//C DELAY
	syoukyo[0]  = 40;

	//ブロックが地面に到達してから、接着されるまでの時間。
	//(この数値を99にすると、接着までの時間が無限になります)
	//L DELAY
	settyaku[0] = 28;

	//横溜めに必要な時間
	//長いとブロックが重い感じになり、逆に短くするとブロックが滑るような感じになります
	//DAS
	yokotame[0] = 15;

	//ブロックの落下スピード。1200で20Gになります。#1.60c6.1c追加
	speed[0] = 2;

	//最大FPS
	fps[0] = 60;

	//hidden (1-7,8=M,9=UM)
	hiddenlv[0] = -1;

	//BLINDの有無 変更する場合は0か1を指定してください
	blind[0]    = 0;

	//BGMの設定　0~19
	p_bgmload[0]    = 0;

	//設定された数値が-1だと、その設定を読み込んでも-1の部分は変更がされません。
	//設定名には、多分「!#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_」が使えます。
	//「"」と「\」も使えますが、"は「\"」、\は「\\」と表記しないといけません。
	//また「`abcdefghifklmnopq」を使うと記号が使えます。

	strcpy(waitname[1], "2G");//1
	syutugen[1] = 26;
	syoukyo[1]  = 40;
	settyaku[1] = 28;
	yokotame[1] = 15;
	fps[1]      = 60;
	hiddenlv[1] = 0;
	speed[1]    = 120;
	blind[1]    = 0;
	p_bgmload[1]= 0;

	strcpy(waitname[2], "5G");//2
	syutugen[2] = 26;
	syoukyo[2]  = 40;
	settyaku[2] = 28;
	yokotame[2] = 15;
	fps[2]      = 60;
	hiddenlv[2] = 0;
	speed[2]    = 300;
	blind[2]    = 0;
	p_bgmload[2]= 0;

	strcpy(waitname[3], "20G");//3
	syutugen[3] = 25;
	syoukyo[3]  = 29;
	settyaku[3] = 27;
	yokotame[3] = 11;
	fps[3]      = 60;
	hiddenlv[3] = 0;
	speed[3]    = 1200;
	blind[3]    = 0;
	p_bgmload[3]= 1;

	strcpy(waitname[4], "MASTER900");//4
	syutugen[4] = 14;
	syoukyo[4]  = 6;
	settyaku[4] = 18;
	yokotame[4] = 9;
	fps[4]      = 60;
	hiddenlv[4] = 0;
	speed[4]    = 1200;
	blind[4]    = 0;

	strcpy(waitname[5], "MASTER #G4");//5
	syutugen[5] = 7;
	syoukyo[5]  = 6;
	settyaku[5] = 15;
	yokotame[5] = 8;
	fps[5]      = 60;
	hiddenlv[5] = 0;
	speed[5]    = 1200;
	blind[5]    = 0;
	p_bgmload[5]= 2;

	strcpy(waitname[6], "MASTER ROLL");//6
	syutugen[6] = 10;
	syoukyo[6]  = 3;
	settyaku[6] = 15;
	yokotame[6] = 8;
	fps[6]      = 60;
	hiddenlv[6] = 0;
	speed[6]    = 1200;
	blind[6]    = 0;

	strcpy(waitname[7], "MM ROLL#G4");//7
	syutugen[7] = 3;
	syoukyo[7]  = 4;
	settyaku[7] = 15;
	yokotame[7] = 7;
	fps[7]      = 60;
	hiddenlv[7] = 0;
	speed[7]    = 1200;
	blind[7]    = 0;

	strcpy(waitname[8], "DEVIL-");//8
	syutugen[8] = 4;
	syoukyo[8]  = 0;
	settyaku[8] = 22;
	yokotame[8] = 11;
	fps[8]      = 60;
	hiddenlv[8] = 0;
	speed[8]    = 1200;

	strcpy(waitname[9], "DEVIL");//9
	syutugen[9] = 9;
	syoukyo[9]  = 5;
	settyaku[9] = 16;
	yokotame[9] = 7;
	fps[9]      = 60;
	hiddenlv[9] = 0;
	speed[9]    = 1200;
	blind[9]    = 0;

	strcpy(waitname[10], "DOOM");//10
	syutugen[10] = 4;
	syoukyo[10]  = 3;
	settyaku[10] = 11;
	yokotame[10] = 5;
	hiddenlv[10] = 0;
	fps[10]      = 60;
	speed[10]    = 1200;
	blind[10]    = 0;
	p_bgmload[10]= 3;

	strcpy(waitname[11], "DOOM 1200");//11
	syutugen[11] = 3;
	syoukyo[11]  = 2;
	settyaku[11] = 9;
	yokotame[11] = 5;
	hiddenlv[11] = 0;
	fps[11]      = 60;
	speed[11]    = 1200;
	blind[11]    = 0;
	p_bgmload[11]= 5;

	strcpy(waitname[12], "DEVIL ROLL");//12
	syutugen[12] = 2;
	syoukyo[12]  = 6;
	settyaku[12] = 14;
	yokotame[12] = 7;
	fps[12]      = 60;
	hiddenlv[12] = 0;
	speed[12]    = 1200;
	blind[12]    = 0;

	strcpy(waitname[13], "OOBAKA");//13
	syutugen[13] = 2;
	syoukyo[13]  = 0;
	settyaku[13] = 5;
	yokotame[13] = 6;
	fps[13]      = 60;
	hiddenlv[13] = 8;
	speed[13]    = 1200;
	blind[13]    = 1;

	strcpy(waitname[14], "DEATH300");//14
	syutugen[14] = 17;
	syoukyo[14]  = 5;
	settyaku[14] = 17;
	yokotame[14] = 10;
	fps[14]      = 60;
	hiddenlv[14] = 0;
	speed[14]    = 1200;
	blind[14]    = 0;

	strcpy(waitname[15], "DEATH400");//15
	syutugen[15] = 6;
	syoukyo[15]  = 4;
	settyaku[15] = 14;
	yokotame[15] = 8;
	fps[15]      = 60;
	hiddenlv[15] = 0;
	speed[15]    = 1200;
	blind[15]    = 0;

	strcpy(waitname[16], "DEATH500");//16
	syutugen[16] = 5;
	syoukyo[16]  = 3;
	settyaku[16] = 14;
	yokotame[16] = 8;
	fps[16]      = 60;
	hiddenlv[16] = 0;
	speed[16]    = 1200;
	blind[16]    = 0;

	//FURTHE(ry
	strcpy(waitname[17], "..!!!!!???");//17
	syutugen[17] = 8;
	syoukyo[17]  = 0;
	settyaku[17] = 6;
	yokotame[17] = 2;
	hiddenlv[17] = 0;
	fps[17]      = 60;
	speed[17]    = 1200;
	blind[17]    = 1;

	//　　　　　。
	strcpy(waitname[18], "VOID(LITE)");//18
	syutugen[18] = 0;
	syoukyo[18]  = 0;
	settyaku[18] = 8;
	yokotame[18] = 2;
	fps[18]      = 60;
	hiddenlv[18] = 0;
	speed[18]    = 1200;
	blind[18]    = 1;

	//最果て
	strcpy(waitname[19], "FINAL(LITE)");//19
	syutugen[19] = 0;
	syoukyo[19]  = 0;
	settyaku[19] = 2;
	yokotame[19] = 1;
	fps[19]      = 60;
	hiddenlv[19] = 0;
	speed[19]    = 1200;
	blind[19]    = 1;

	strcpy(waitname[20], "-----");//20
	strcpy(waitname[21], "-----");//21
	strcpy(waitname[22], "-----");//22
	strcpy(waitname[23], "-----");//23
	strcpy(waitname[24], "-----");//24

	//以下は追加用で全部で25個まで増やすことが出来ます
	//追加するときはコメントの/*と*/を外し、[]で括られた配列番号を変え、上のskip_fwaitの値を変えてください。
	/*
	strcpy(waitname[20], "-----");//20
	syutugen[20] = -1;
	syoukyo[20]  = -1;
	settyaku[20] = -1;
	yokotame[20] = -1;
	fps[20]      = -1;
	speed[20]    = -1;
	*/
	//=============================================================
	// デバッグ
	//=============================================================
	// 最大FPS。デフォルトは60
	// debugが1かどうかに関係なく反映されます。
	max_fps_2 = 60;
}
