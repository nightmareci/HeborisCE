//□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□
//
//    HEBORIS [ヘボリス] 2001 Ver0.00 / 2000.11.05 (Sun) 〜 ????.??.?? (????)
//
//                         (c) 1998-2000 Kenji Hoshimoto
//
//■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■

import "string.dll"
int screenMode, colorMode, systemmem;

//▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽▼▽
//   初期設定
//▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲△▲
void main() {
	if(LoadConfig()) {	//CONFIG.SAVより設定をロード
		CallScript("script/readdef.c");	//読み込み失敗したらoption.def・keyconf.defからCONFIG.SAV作成
		halt;
		LoadConfig();
	}

	/* 050825 画面モード拡張改造部分 -- ここから */
	if(screenMode == 0)
		SetWindow (0, 320, 240, colorMode);
	else if(screenMode == 1)
		SetWindow (1, 320, 240, colorMode);
	else if(screenMode == 2)
		SetWindow (1, 640, 480, colorMode);
	else if(screenMode == 3)
		SetWindow (0, 640, 480, colorMode);
	else if(screenMode == 4)
		SetWindow (1, 800, 600, colorMode);
	else if(screenMode == 5)
		SetWindow (1, 1024, 768, colorMode);
	else if(screenMode == 6)
		SetWindow (1, 1280, 960, colorMode);
	else
		SetWindow (1, 640, 480, colorMode);
	/* ここまで */

	// 60fps
	SetFPS (60);

	// YGS2000固有の設定
	SetConstParam("Caption", "Please Wait...");
	SetConstParam("EnablePause", 0);		//初期化が終わるまでポーズしない#1.60c7p9ex
	SetConstParam("USE_XZ_FOR_BUTTON", 1);
	SetConstParam("StringMax", 123);

	SetDebugMode(0);

	// システムメモリを使うか?
	SetSystemMemoryUse(systemmem);
}


int LoadConfig(void) {
	int32_t i, cfgbuf[24];

	FillMemory(&cfgbuf, 24, 0);
	LoadFile("config/data/CONFIG.SAV", &cfgbuf, 24);
	if(cfgbuf[0] != 0x4F424550) return (1);
	if(cfgbuf[1] != 0x20534953) return (1);
	if(cfgbuf[2] != 0x464E4F44) return (1);
	if(cfgbuf[3] != 0x31764750) return (1);

	screenMode = cfgbuf[4] & 0xff;
	colorMode = (cfgbuf[4] >> 8 + 1) * 16;
	systemmem = cfgbuf[5];
	return (0);
}
