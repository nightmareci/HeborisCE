//=============================================================================
//  KBGM32.DLL かんたんラッパークラスだYO
//=============================================================================

#include <windows.h>
#include "KBGMPlayer.h"

// ============================================================================
//  CKBGMPlayer : KBGM32.DLL をお手軽に使おう!!
// ============================================================================

// ----------------------------------------------------------------------------
//	CKBGMPlayer クラスのコンストラクタ
// ----------------------------------------------------------------------------
CKBGMPlayer::CKBGMPlayer(bool initialize)
{
	if ( initialize )
		LibraryInitialize();
}

// ----------------------------------------------------------------------------
//	CKBGMPlayer クラスのデストラクタ
// ----------------------------------------------------------------------------
CKBGMPlayer::~CKBGMPlayer()
{
	if ( loaded )
	{
		// 読み込んでいるハンドルがある場合は開放する
		for ( int i = 0 ; i < KBGMPLAYER_HDATA_QTY ; i ++ )
		{
			if ( bgmloaded[i] ) Free(i);
		}

		(*funcs.lpStop)();
		(*funcs.lpClose)();
		FreeLibrary(funcs.hInst);
	}
}

// ----------------------------------------------------------------------------
//	KBGM32.DLL の初期化
// ----------------------------------------------------------------------------
bool CKBGMPlayer::LibraryInitialize()
{
	if ( !loaded )
	{
		funcs.hInst = LoadLibrary("KBGM32.DLL");

		if ( funcs.hInst < (HINSTANCE)HINSTANCE_ERROR )
		{
			return false;
		}

		// 関数のエントリポイントを取得
		funcs.lpOpen		= (KBGM_FUNC_KBGMOPEN)	GetProcAddress(funcs.hInst, "KBGMOpen");
		funcs.lpClose		= (KBGM_FUNC_CLOSE)		GetProcAddress(funcs.hInst, "KBGMClose");
		funcs.lpPlay		= (KBGM_FUNC_PLAY)		GetProcAddress(funcs.hInst, "KBGMPlay");
		funcs.lpResume		= (KBGM_FUNC_RESUME)	GetProcAddress(funcs.hInst, "KBGMResume");
		funcs.lpStop		= (KBGM_FUNC_STOP)		GetProcAddress(funcs.hInst, "KBGMStop");
		funcs.lpPause		= (KBGM_FUNC_PAUSE)		GetProcAddress(funcs.hInst, "KBGMPause");
		funcs.lpFree		= (KBGM_FUNC_FREE)		GetProcAddress(funcs.hInst, "KBGMFree");
		funcs.lpLoadRes		= (KBGM_FUNC_LOADRES)	GetProcAddress(funcs.hInst, "KBGMLoadRes");
		funcs.lpLoadFile	= (KBGM_FUNC_LOADFILE)	GetProcAddress(funcs.hInst, "KBGMLoadFile");
		funcs.lpInit		= (KBGM_FUNC_INIT)		GetProcAddress(funcs.hInst, "KBGMInit");
		funcs.lpGetInfo		= (KBGM_FUNC_GETINFO)	GetProcAddress(funcs.hInst, "KBGMGetInfo");
		funcs.lpSetVolume	= (KBGM_FUNC_SETVOLUME)	GetProcAddress(funcs.hInst, "KBGMSetVolume");
		funcs.lpSendSysx	= (KBGM_FUNC_SENDSYSX)	GetProcAddress(funcs.hInst, "KBGMSendSysx");
		funcs.lpFadeIn		= (KBGM_FUNC_FADEIN)	GetProcAddress(funcs.hInst, "KBGMFadeIn");
		funcs.lpFadeOut		= (KBGM_FUNC_FADEOUT)	GetProcAddress(funcs.hInst, "KBGMFadeOut");
		funcs.lpSetTempo	= (KBGM_FUNC_SETTEMPO)	GetProcAddress(funcs.hInst, "KBGMSetTempo");
		funcs.lpSeek		= (KBGM_FUNC_SEEK)		GetProcAddress(funcs.hInst, "KBGMSeek");
		funcs.lpSetRepeat	= (KBGM_FUNC_SETREPEAT)	GetProcAddress(funcs.hInst, "KBGMSetRepeat");
		funcs.lpNoteOn		= (KBGM_FUNC_NOTEON)	GetProcAddress(funcs.hInst, "KBGMNoteOn");
		funcs.lpSetCtrl		= (KBGM_FUNC_SETCTRL)	GetProcAddress(funcs.hInst, "KBGMSetCtrl");
		funcs.lpSetPrg		= (KBGM_FUNC_SETPRG)	GetProcAddress(funcs.hInst, "KBGMSetPrg");

		for ( int i = 0 ; i < KBGMPLAYER_HDATA_QTY ; i ++ )
		{
			bgmloaded[i] = false;
		}

		(*funcs.lpOpen)(10, MIDI_MAPPER);

		loaded = true;
	}

	return loaded;
}

// ----------------------------------------------------------------------------
//	SMF ファイルを読み込む
// ----------------------------------------------------------------------------
UINT CKBGMPlayer::LoadFile(UINT num, LPSTR filename)
{
	UINT	result;

	// 初期化ができていないときはやめる
	if ( !loaded ) return KBGM_NOTLOADEDDLL;

	// hData にファイルを読み込む
	result = (*funcs.lpLoadFile)(&hData[num], filename);
	if ( result == KBGM_NOERROR ) bgmloaded[num] = true;

	return result;
}

// ----------------------------------------------------------------------------
//	リソースから SMF を読み込む
// ----------------------------------------------------------------------------
UINT CKBGMPlayer::LoadRes(UINT num, HINSTANCE hInstApp, LPCSTR resname, LPCSTR restype)
{
	UINT	result;

	// 初期化ができていないときはやめる
	if ( !loaded ) return KBGM_NOTLOADEDDLL;

	// 既に読み込み済みのときは一旦開放する
	if ( bgmloaded[num] ) Free( num );

	// hData にリソースを読み込む
	result = (*funcs.lpLoadRes)(&hData[num], hInstApp, resname, restype);
	if ( result == KBGM_NOERROR ) bgmloaded[num] = true;

	return result;
}

// ----------------------------------------------------------------------------
//	初期化を行う
// ----------------------------------------------------------------------------
UINT CKBGMPlayer::Init(UINT num)
{
	UINT	result;

	// 初期化ができていないときはやめる
	if ( !loaded ) return KBGM_NOTLOADEDDLL;
	
	// 初期化
	result = (*funcs.lpInit)(hData[num]);

	return result;
}

// ----------------------------------------------------------------------------
//	再生する
// ----------------------------------------------------------------------------
UINT CKBGMPlayer::Play(UINT num, UINT type)
{
	UINT	result;

	// 初期化ができていないときはやめる
	if ( !loaded ) return KBGM_NOTLOADEDDLL;

	// BGM を再生
	result = (*funcs.lpPlay)(hData[num], type);

	return result;
}

// ----------------------------------------------------------------------------
//	停止する
// ----------------------------------------------------------------------------
UINT CKBGMPlayer::Stop(void)
{
	UINT	result;

	// 初期化ができていないときはやめる
	if ( !loaded ) return KBGM_NOTLOADEDDLL;

	// BGM を停止
	result = (*funcs.lpStop)();

	return result;
}

// ----------------------------------------------------------------------------
//	一時停止する
// ----------------------------------------------------------------------------
UINT CKBGMPlayer::Pause(void)
{
	UINT	result;

	// 初期化ができていないときはやめる
	if ( !loaded ) return KBGM_NOTLOADEDDLL;

	// BGM を一時停止
	result = (*funcs.lpPause)();

	return result;
}

// ----------------------------------------------------------------------------
//	読み込んだ MIDI を開放
// ----------------------------------------------------------------------------
UINT CKBGMPlayer::Free(UINT num)
{
	UINT	result;

	// 初期化ができていないときはやめる
	if ( !loaded ) return KBGM_NOTLOADEDDLL;

	// MIDI を開放
	result = (*funcs.lpFree)(hData[num]);
	if ( result == KBGM_NOERROR ) bgmloaded[num] = false;

	return result;
}

// ----------------------------------------------------------------------------
//	一時停止を再開する
// ----------------------------------------------------------------------------
UINT CKBGMPlayer::Resume(UINT num)
{
	UINT	result;

	// 初期化ができていないときはやめる
	if ( !loaded ) return KBGM_NOTLOADEDDLL;

	// 一時停止した BGM を再開
	result = (*funcs.lpResume)(hData[num]);

	return result;
}

// ----------------------------------------------------------------------------
//	ボリュームを変更する
// ----------------------------------------------------------------------------
UINT CKBGMPlayer::SetVolume(UINT num, UINT volume)
{
	UINT	result;

	// 初期化ができていないときはやめる
	if ( !loaded ) return KBGM_NOTLOADEDDLL;

	// ボリュームを設定
	result = (*funcs.lpSetVolume)(hData[num], volume);

	return result;
}

// ----------------------------------------------------------------------------
//	リピートの設定を変更する
// ----------------------------------------------------------------------------
UINT CKBGMPlayer::SetRepeat(UINT num, DWORD times, DWORD seek)
{
	UINT	result;

	// 初期化ができていないときはやめる
	if ( !loaded ) return KBGM_NOTLOADEDDLL;

	// リピートを設定
	result = (*funcs.lpSetRepeat)(hData[num], times, seek);

	return result;
}

// ----------------------------------------------------------------------------
//	テンポを設定する
// ----------------------------------------------------------------------------
UINT CKBGMPlayer::SetTempo(UINT num, UINT beat)
{
	UINT	result;

	// 初期化ができていないときはやめる
	if ( !loaded ) return KBGM_NOTLOADEDDLL;

	// テンポを設定
	result = (*funcs.lpSetTempo)(hData[num], beat);

	return result;
}

// ----------------------------------------------------------------------------
//	SMF の情報を取得
// ----------------------------------------------------------------------------
UINT CKBGMPlayer::GetInfo(UINT num, LPSMFINFO lpSmfInfo)
{
	UINT	result;

	// 初期化ができていないときはやめる
	if ( !loaded ) return KBGM_NOTLOADEDDLL;

	// SMF情報を取得
	result = (*funcs.lpGetInfo)(hData[num], lpSmfInfo);

	return result;
}

// ----------------------------------------------------------------------------
//	システムエクスクルージブメッセージを送信
// ----------------------------------------------------------------------------
UINT CKBGMPlayer::SendSysx(UINT type)
{
	UINT	result;

	// 初期化ができていないときはやめる
	if ( !loaded ) return KBGM_NOTLOADEDDLL;

	// システムエクスクルージブメッセージを送信
	result = (*funcs.lpSendSysx)(type);

	return result;
}

// ----------------------------------------------------------------------------
//	フェードイン
// ----------------------------------------------------------------------------
UINT CKBGMPlayer::FadeIn(UINT num, UINT Sec)
{
	UINT	result;

	// 初期化ができていないときはやめる
	if ( !loaded ) return KBGM_NOTLOADEDDLL;

	// フェードイン
	result = (*funcs.lpFadeIn)(hData[num], Sec);

	return result;
}

// ----------------------------------------------------------------------------
//	フェードアウト
// ----------------------------------------------------------------------------
UINT CKBGMPlayer::FadeOut(UINT num, UINT Sec)
{
	UINT	result;

	// 初期化ができていないときはやめる
	if ( !loaded ) return KBGM_NOTLOADEDDLL;

	// フェードアウト
	result = (*funcs.lpFadeOut)(hData[num], Sec);

	return result;
}

// ----------------------------------------------------------------------------
//	シークする
// ----------------------------------------------------------------------------
UINT CKBGMPlayer::Seek(UINT num, LONG Tick)
{
	UINT	result;

	// 初期化ができていないときはやめる
	if ( !loaded ) return KBGM_NOTLOADEDDLL;

	// シーク
	result = (*funcs.lpSeek)(hData[num], Tick);

	return result;
}

// ----------------------------------------------------------------------------
//	ノートオン
// ----------------------------------------------------------------------------
UINT CKBGMPlayer::NoteOn(BYTE ChNum, BYTE Note, BYTE Vel)
{
	UINT	result;

	// 初期化ができていないときはやめる
	if ( !loaded ) return KBGM_NOTLOADEDDLL;

	// 音を鳴らす
	result = (*funcs.lpNoteOn)(ChNum, Note, Vel);

	return result;
}

// ----------------------------------------------------------------------------
//	コントロールメッセージを送る
// ----------------------------------------------------------------------------
UINT CKBGMPlayer::SetCtrl(BYTE ChNum, BYTE Data1, BYTE Data2)
{
	UINT	result;

	// 初期化ができていないときはやめる
	if ( !loaded ) return KBGM_NOTLOADEDDLL;

	// コントロールメッセージを送る
	result = (*funcs.lpSetCtrl)(ChNum, Data1, Data2);

	return result;
}

// ----------------------------------------------------------------------------
//	音色を指定する
// ----------------------------------------------------------------------------
UINT CKBGMPlayer::SetPrg(BYTE ChNum, BYTE Pnum)
{
	UINT	result;

	// 初期化ができていないときはやめる
	if ( !loaded ) return KBGM_NOTLOADEDDLL;

	// 音色を指定
	result = (*funcs.lpSetPrg)(ChNum, Pnum);

	return result;
}

//=============================================================================
//	end of file
//=============================================================================
