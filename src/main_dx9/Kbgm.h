#ifndef __KBGM_H
#define __KBGM_H

#include <windows.h>
#include <mmsystem.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __WINDOWS__
#define WINFARPROC WINAPI __loadds
#else
#define WINFARPROC WINAPI
#endif

#define KBGMVER 0x008

#define HDATA HGLOBAL
typedef HDATA FAR * LPHDATA;


#define ONCE      0
#define REPEAT    1
#define ENDLESS   0

#define GM_RESET  0
#define GS_RESET  1
#define VL_RESET1 2
#define VL_RESET2 3
#define XG_RESET  4

#define KBGM_ERROR_BASE 0x1000
#define KBGM_NOERROR    0

#define KBGM_WRONGID        KBGM_ERROR_BASE+0
#define KBGM_NOMEM          KBGM_ERROR_BASE+1
#define KBGM_UNKNOWNERROR   KBGM_ERROR_BASE+2
#define KBGM_STILLPLAYING   KBGM_ERROR_BASE+3
#define KBGM_WAIT           KBGM_ERROR_BASE+4
#define KBGM_NOTIMERRES     KBGM_ERROR_BASE+5
#define KBGM_TIMERERROR     KBGM_ERROR_BASE+6
#define KBGM_NOFILE         KBGM_ERROR_BASE+7
#define KBGM_READERROR      KBGM_ERROR_BASE+8
#define KBGM_WRONGFILE      KBGM_ERROR_BASE+9
#define KBGM_PAGELOCKFAILED KBGM_ERROR_BASE+10
#define KBGM_NOBUFFER       KBGM_ERROR_BASE+11
#define KBGM_NOTIME         KBGM_ERROR_BASE+12
#define KBGM_NOTPLAYING     KBGM_ERROR_BASE+13
#define KBGM_NORESOURCE     KBGM_ERROR_BASE+14
#define KBGM_WRONGCH        KBGM_ERROR_BASE+15

//KBGM.DLL

typedef struct tagSmfInfo{
	
	UINT	Tempo;		//テンポ
	BOOL	Playing;	//演奏フラグ
	DWORD	Timebase;	//タイムベース
	UINT	Volume;		//マスターボリューム
	HGLOBAL	T_Global;
	LPBYTE	Title;		//タイトル格納用
	DWORD	T_Length;	//タイトルの長さ
	DWORD	SongPtr;	//ソングポインタ
	DWORD	Times;		//何回演奏したか
	BOOL	Note[16][128];	//音が鳴っているか否か
	
}SMFINFO;

typedef SMFINFO FAR * LPSMFINFO;

extern	UINT WINFARPROC KBGMOpen( UINT nTimerRes, UINT IDDevice );
extern	UINT WINFARPROC KBGMClose( void );
extern	UINT WINFARPROC KBGMPlay( HDATA hData, UINT Type );
extern	UINT WINFARPROC KBGMResume( HDATA hData );
extern	UINT WINFARPROC KBGMStop( void );
extern	UINT WINFARPROC KBGMPause( void );
extern	UINT WINFARPROC KBGMFree( HDATA hData );
extern	UINT WINFARPROC KBGMLoadRes( LPHDATA lphData, HINSTANCE hInstApp, LPCSTR resname, LPCSTR restype );
//extern	UINT WINFARPROC KBGMLoadResVB( LPHDATA lphData, LPBYTE data);
extern	UINT WINFARPROC KBGMLoadFile( LPHDATA hData, LPSTR filename );
extern	UINT WINFARPROC KBGMInit( HDATA hData );
extern	UINT WINFARPROC KBGMGetInfo( HDATA hData, LPSMFINFO lpSmfInfo );
extern	UINT WINFARPROC KBGMSetVolume( HDATA hData, UINT Volume );
extern	UINT WINFARPROC KBGMSendSysx( UINT Type );
extern	UINT WINFARPROC KBGMFadeIn( HDATA hData, UINT Sec );
extern	UINT WINFARPROC KBGMFadeOut( HDATA hData, UINT Sec );
extern	UINT WINFARPROC KBGMSetTempo( HDATA hData, UINT Beat );
extern	UINT WINFARPROC KBGMSeek( HDATA hData, LONG Tick );
extern	UINT WINFARPROC KBGMSetRepeat( HDATA hData, DWORD Times, DWORD Seek );
extern	UINT WINFARPROC KBGMNoteOn( BYTE ChNum, BYTE Note, BYTE Vel );
extern	UINT WINFARPROC KBGMSetCtrl( BYTE ChNum, BYTE Data1, BYTE Data2 );
extern	UINT WINFARPROC KBGMSetPrg( BYTE ChNum, BYTE Pnum );

#ifdef __cplusplus
}
#endif

#endif
