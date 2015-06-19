//=============================================================================
//  KBGM32.DLL かんたんラッパークラスだYO
//=============================================================================

#ifndef		_KBGMPLAYER_H_
#define		_KBGMPLAYER_H_

#include <windows.h>
#include "kbgm.h"

#define		KBGMPLAYER_HDATA_QTY		50
#define		KBGM_NOTLOADEDDLL			-1

// ============================================================================
//  KBGM 用定義
// ============================================================================

typedef UINT (WINFARPROC *KBGM_FUNC_KBGMOPEN)		(UINT, UINT);
typedef UINT (WINFARPROC *KBGM_FUNC_CLOSE)			(void);
typedef UINT (WINFARPROC *KBGM_FUNC_PLAY)			(HDATA, UINT);
typedef UINT (WINFARPROC *KBGM_FUNC_RESUME)			(HDATA);
typedef UINT (WINFARPROC *KBGM_FUNC_STOP)			(void);
typedef UINT (WINFARPROC *KBGM_FUNC_PAUSE)			(void);
typedef UINT (WINFARPROC *KBGM_FUNC_FREE)			(HDATA);
typedef UINT (WINFARPROC *KBGM_FUNC_LOADRES)		(LPHDATA, HINSTANCE, LPCSTR, LPCSTR);
typedef UINT (WINFARPROC *KBGM_FUNC_LOADFILE)		(LPHDATA, LPSTR);
typedef UINT (WINFARPROC *KBGM_FUNC_INIT)			(HDATA);
typedef UINT (WINFARPROC *KBGM_FUNC_GETINFO)		(HDATA, LPSMFINFO);
typedef UINT (WINFARPROC *KBGM_FUNC_SETVOLUME)		(HDATA, UINT);
typedef UINT (WINFARPROC *KBGM_FUNC_SENDSYSX)		(UINT);
typedef UINT (WINFARPROC *KBGM_FUNC_FADEIN)			(HDATA, UINT);
typedef UINT (WINFARPROC *KBGM_FUNC_FADEOUT)		(HDATA, UINT);
typedef UINT (WINFARPROC *KBGM_FUNC_SETTEMPO)		(HDATA, UINT);
typedef UINT (WINFARPROC *KBGM_FUNC_SEEK)			(HDATA, LONG);
typedef UINT (WINFARPROC *KBGM_FUNC_SETREPEAT)		(HDATA, DWORD, DWORD);
typedef UINT (WINFARPROC *KBGM_FUNC_NOTEON)			(BYTE, BYTE, BYTE);
typedef UINT (WINFARPROC *KBGM_FUNC_SETCTRL)		(BYTE, BYTE, BYTE);
typedef UINT (WINFARPROC *KBGM_FUNC_SETPRG)			(BYTE, BYTE);

struct KBGM_FUNCS
{
	HINSTANCE				hInst;
	KBGM_FUNC_KBGMOPEN		lpOpen;
	KBGM_FUNC_CLOSE			lpClose;
	KBGM_FUNC_PLAY			lpPlay;
	KBGM_FUNC_RESUME		lpResume;
	KBGM_FUNC_STOP			lpStop;
	KBGM_FUNC_PAUSE			lpPause;
	KBGM_FUNC_FREE			lpFree;
	KBGM_FUNC_LOADRES		lpLoadRes;
	KBGM_FUNC_LOADFILE		lpLoadFile;
	KBGM_FUNC_INIT			lpInit;
	KBGM_FUNC_GETINFO		lpGetInfo;
	KBGM_FUNC_SETVOLUME		lpSetVolume;
	KBGM_FUNC_SENDSYSX		lpSendSysx;
	KBGM_FUNC_FADEIN		lpFadeIn;
	KBGM_FUNC_FADEOUT		lpFadeOut;
	KBGM_FUNC_SETTEMPO		lpSetTempo;
	KBGM_FUNC_SEEK			lpSeek;
	KBGM_FUNC_SETREPEAT		lpSetRepeat;
	KBGM_FUNC_NOTEON		lpNoteOn;
	KBGM_FUNC_SETCTRL		lpSetCtrl;
	KBGM_FUNC_SETPRG		lpSetPrg;
};

// ============================================================================
//  CKBGMPlayer : KBGM32.DLL を簡単に扱うクラス
// ============================================================================

class CKBGMPlayer
{
public:
	CKBGMPlayer(bool initialize = true);
	~CKBGMPlayer();
	bool LibraryInitialize();
	UINT LoadFile(UINT num, LPSTR filename);
	UINT LoadRes(UINT num, HINSTANCE hInstApp, LPCSTR resname, LPCSTR restype);
	UINT Init(UINT num);
	UINT Play(UINT num, UINT type = REPEAT);
	UINT Stop(void);
	UINT Pause(void);
	UINT Free(UINT num);
	UINT Resume(UINT num);
	UINT SetVolume(UINT num, UINT volume);
	UINT SetRepeat(UINT num, DWORD times, DWORD seek);
	UINT SetTempo(UINT num, UINT beat);
	UINT GetInfo(UINT num, LPSMFINFO lpSmfInfo);
	UINT SendSysx(UINT type = GM_RESET);
	UINT FadeIn(UINT num, UINT Sec);
	UINT FadeOut(UINT num, UINT Sec);
	UINT Seek(UINT num, LONG Tick);
	UINT NoteOn(BYTE ChNum, BYTE Note, BYTE Vel);
	UINT SetCtrl(BYTE ChNum, BYTE Data1, BYTE Data2);
	UINT SetPrg(BYTE ChNum, BYTE Pnum);

private:
	bool				loaded;
	KBGM_FUNCS			funcs;
	bool				bgmloaded[KBGMPLAYER_HDATA_QTY];
	HDATA				hData[KBGMPLAYER_HDATA_QTY];
};

#endif
//=============================================================================
//	end of file
//=============================================================================
