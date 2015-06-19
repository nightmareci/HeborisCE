//==============================================================================//
//				DirectX9用汎用ライブラリ Luna									//
//==============================================================================//
//																				//
//	最新版の置いてあるURL														//
//																				//
//		TWINTAIL LOVERS.														//
//			http://www.twin-tail.jp/											//
//																				//
//	質問等発言場所																//
//																				//
//		BBS																		//
//			TWINTAIL LOVERS.内の掲示板											//
//																				//
// DirectX Library "Luna for DirectX 9.0c"										//
// Copyright (C) 2003-2006 葉迩倭												//
// All rights reserved.															//
//																				//
//==============================================================================//

/**
    @brief		内部で使用している各種型定義を行っています
    @author		葉迩倭
*/

#ifndef ___LUNABASE_H___
#define ___LUNABASE_H___


//================================================================================
// WARNING
//================================================================================
#pragma warning ( disable: 4127 )		// 定数の条件式
#pragma warning ( disable: 4100 )		// 未参照の変数
#pragma warning ( disable: 4514 )		// 未参照のインライン関数
#pragma warning ( disable: 4711 )		// 自動インライン展開
#pragma warning ( disable: 4201 )		// 構造体拡張ワーニング

#pragma inline_depth ( 255 )
#pragma inline_recursion ( on )


//=============================================================================
// FOR MSVC6.0
//=============================================================================
#if (_MSC_VER == 1200)
	#define for				if ( false ) ; else for														///< for文スコープ強制準拠マクロ
	typedef long			LONG_PTR;
	typedef unsigned long	DWORD_PTR;
	typedef void (__stdcall *PAPCFUNC)( DWORD_PTR dwParam );
#endif


//================================================================================
// INCLUDE
//================================================================================
#undef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION		(0x0800)			///< DirectInputバージョン定義

#undef _WIN32_WINNT
#define _WIN32_WINNT			(0x0400)			///< Fiber用

#undef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN							///< 不要なヘッダの参照削除

#undef STRICT
#define STRICT										///< 構造体拡張宣言

#define STRSAFE_NO_DEPRECATE
#define __MT__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mbstring.h>
#include <tchar.h>
#include <wchar.h>
#include <windows.h>
#include <winsock2.h>
#include <shellapi.h>
#include <process.h>
#include <commdlg.h>
#include <strsafe.h>
#include <shlobj.h>
#include <imm.h>
#include <vfw.h>
#include <objbase.h>
#include <basetsd.h>

#include <d3dx9xof.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dsound.h>
#include <dinput.h>
#include <dmusicc.h>
#include <dmusici.h>
#include <dshow.h>
#include <mmstream.h>
#include <amstream.h>
#include <ddstream.h>
#include <vmr9.h>

#undef NULL
#define NULL		(0x00000000)


//================================================================================
// TYPEDEF
//================================================================================
typedef char					Sint8;								///< signed char 型の別定義
typedef short					Sint16;								///< signed short 型の別定義
typedef long					Sint32;								///< signed long 型の別定義
typedef __int64					Sint64;								///< signed __int64 型の別定義
typedef unsigned char			Uint8;								///< unsigned char 型の別定義
typedef unsigned short			Uint16;								///< unsigned short 型の別定義
typedef unsigned long			Uint32;								///< unsigned long 型の別定義
typedef unsigned __int64		Uint64;								///< unsigned __int64 型の別定義
typedef float					Float;								///< Float 型の別定義
typedef float					Float32;							///< Float 型の別定義
typedef double					Float64;							///< double 型の別定義
typedef long double				Float80;							///< long double 型の別定義
typedef bool					Bool;								///< Bool 型の別定義


typedef Bool (*PCALLBACKSOCKETPROC)( HWND hWnd, long Id );			///< ソケット処理用コールバック関数
typedef void (WINAPI *PFIBERPROC)( void *pParameter );				///< マイクロスレッド用コールバック関数
typedef Bool (*PMICROTHREADPROC)( void *pParameter );				///< スレッド用コールバック関数
typedef IDirect3D9 * (WINAPI *PDIRECT3DCREATE9)( unsigned int );	///< Direct3Dオブジェクト生成関数用


//================================================================================
// DEFINE
//================================================================================
static const Sint32 OPTION_DIRECTINPUT			= (1 <<  0);						///< DirectInputを使用してキー入力を行う
static const Sint32 OPTION_DIRECTSOUND			= (1 <<  1);						///< DirectSoundをを使用してサウンド再生を行う
static const Sint32 OPTION_DIRECTMUSIC			= (1 <<  2);						///< DirectMusicを使用してサウンド再生を行う
static const Sint32 OPTION_DIRECTSHOW			= (1 <<  3);						///< DirectShowを使用してムービーの再生をする
static const Sint32 OPTION_CDDA					= (1 <<  4);						///< MCIを使用してCDDAを使用する
static const Sint32 OPTION_FIBER				= (1 <<  5);						///< Fiberを使用してタスク処理をする


static const Sint32 FX_SEND_TRANSFORM			= (1 <<  0);
static const Sint32 FX_SEND_WORLDMATRIX			= (1 <<  1);
static const Sint32 FX_SEND_WORLDVIEWMATRIX		= (1 <<  2);
static const Sint32 FX_SEND_BONEDATA			= (1 <<  3);
static const Sint32 FX_SEND_LIGHTDATA			= (1 <<  4);
static const Sint32 FX_SEND_MATERIALDATA		= (1 <<  5);
static const Sint32 FX_SEND_SLIDE_UV			= (1 <<  6);
static const Sint32 FX_SEND_VERTEXEXTRUCT		= (1 <<  7);
static const Sint32 FX_SEND_MORPFWEIGHT			= (1 <<  8);
static const Sint32 FX_SEND_CAMERAPOS			= (1 <<  9);
static const Sint32 FX_SEND_TEXTURE				= (1 << 10);
static const Sint32 FX_SEND_FOGDATA				= (1 << 11);


//================================================================================
// CLASS TYPEDEF
//================================================================================
typedef class CVector2D			LVECTOR2;
typedef class CVector3D			LVECTOR3;
typedef class CMatrix			LMATRIX;
typedef class CQuaternion		LQUATERNION;
typedef class CStyle			LSTYLE;


#include "Class/Vector2D.h"
#include "Class/Vector3D.h"
#include "Class/Matrix.h"
#include "Class/Quaternion.h"
#include "Class/Style.h"


//================================================================================
// DEFINE
//================================================================================
#define ef						else if																		///< else if 簡略用マクロ

#define toF(V)					((Float)(V))																///< Float型へのキャストマクロ
#define toI(V)					((Sint32)(V))																///< Sint32型へのキャストマクロ
#define F(V)					toF(V)
#define L(V)					toI(V)

#define PI						(3.141592653589793238462643383279f)											///< π
#define PI2						(6.283185307179586476925286766559f)											///< 2π
#define REV(V)					toF(1.0f/toF(V))																///< 逆数算出マクロ

#define SAFE_RELEASE(V)			if ( (V) != NULL ) { (V)->Release(); (V) = NULL; }							///< COM安全解放マクロ
#define SAFE_DELETE(V)			if ( (V) != NULL ) { delete (V); (V) = NULL; }								///< newメモリ安全解放
#define SAFE_DELETES(V)			if ( (V) != NULL ) { delete [] (V); (V) = NULL; }							///< new[]メモリ安全解放

#define MIN(N1,N2)				((N1 < N2) ? (N1) : (N2))													///< 最小値取得マクロ
#define MAX(N1,N2)				((N1 > N2) ? (N1) : (N2))													///< 最大値取得マクロ
#define SWAP(N1,N2)				{ N1 = N2 - N1; N2 -= N1; N1 += N2; }										///< 値交換マクロ

#define SIN_TABLE_BIT			(16)																		///< サインテーブルのビット数
#define SIN_TABLE_SIZE			(1 << SIN_TABLE_BIT)														///< サインテーブルのサイズ
#define SIN_TABLE_MASK			(SIN_TABLE_SIZE - 1)														///< サインテーブルのアクセス用マスク
#define ARCTAN_TABLE_SIZE		(SIN_TABLE_SIZE >> 3)														///< アークタンジェントテーブルのサイズ
#define ARCTAN_POS_1			(ARCTAN_TABLE_SIZE << 1)													///< アークタンジェントの第１象限
#define ARCTAN_POS_2			(ARCTAN_POS_1 + ARCTAN_POS_1)												///< アークタンジェントの第２象限
#define ARCTAN_POS_3			(ARCTAN_POS_1 + ARCTAN_POS_2)												///< アークタンジェントの第３象限
#define ARCTAN_POS_4			(ARCTAN_POS_1 + ARCTAN_POS_3)												///< アークタンジェントの第４象限
#define ARCTAN_POS_SHIFT		(SIN_TABLE_BIT - 3)															///< アークタンジェントの位置シフト量
#define ARCTAN_SCALE(V)			(toF(V) * toF(1 << ARCTAN_POS_SHIFT))										///< アークタンジェントの拡大率

#define TEXTURE_STAGE_MAX		(4)																			///< テクスチャステージ最大数
#define BONE_MAX				(32)																		///< ひとつのメッシュで使用可能な最大ボーン数
#define LUNA_THREAD_MAX			(32)																		///< スレッドの最大数

#define NORMALIZE_ANGLE(A)		((A) & SIN_TABLE_MASK)														///< 角度の正規化（0～65535へ）
#define GET_ANGLE(A)			toI(toF(A) * toF(SIN_TABLE_SIZE) / toF(360))								///< 専用角度の算出
#define GET_ANGLE_PI(A)			toI(toF(A) * toF(SIN_TABLE_SIZE) / toF(PI2))								///< ラジアンから専用角度の算出

#define PRIMITIVE_Z_MAX			(0xFFFF)																	///< プリミティブ用Ｚ最大値
#define INV_PRIMITIVE_Z_MAX		REV(PRIMITIVE_Z_MAX)														///< プリミティブ用Ｚ値算出用データ
#define PRIMITIVE_Z_CENTER		toF(PRIMITIVE_Z_MAX / 2)													///< プリミティブＺ中心位置
#define Z_TO_SORT_VALUE(Z)		(toI(Z) >> 3)																///< Ｚ値からＺソート用基準値算出用
#define SORT_Z_MAX				Z_TO_SORT_VALUE(PRIMITIVE_Z_MAX)											///< Ｚソート用Ｚ最大値

#define MemoryClear(PD,S)		::FillMemory((PD),(S),0x00)													///< メモリクリア
#define MemoryFill(PD,PS,S)		::FillMemory((PD),(S),(PS))													///< メモリ塗りつぶし
#define MemoryMove(PD,PS,S)		::MoveMemory((PD),(PS),(S))													///< メモリ移動
#define MemoryCopy(PD,PS,S)		::CopyMemory((PD),(PS),(S))													///< メモリコピー

#define MemGlobalAlloc(size)	::VirtualAlloc( NULL, ((size)+4095)&~4095, MEM_COMMIT, PAGE_READWRITE )						///< VirtuakAllocメモリ確保マクロ
#define MemGlobalFree(pointer)	if ( (pointer) != NULL ) { ::VirtualFree( pointer, 0, MEM_RELEASE ); (pointer) = NULL; }	///< VirtualAllocメモリ解放マクロ

#define MemLocalAlloc(size)		::HeapAlloc( ::GetProcessHeap(), HEAP_ZERO_MEMORY, size )										///< HeapAllocメモリ確保マクロ
#define MemLocalFree(pointer)	if ( (pointer) != NULL ) { ::HeapFree( ::GetProcessHeap(), 0, pointer ); (pointer) = NULL; }	///< HeapAllocメモリ解放マクロ

#define CHECK_GUID(S,D)			(strcmp( (S).Data, (D) ) == 0)												///< システムコールバックのGUIDチェック

#define COLOR_BASE				(D3DCOLOR_XRGB(0xFF,0xFF,0xFF))												///< カラー定義：基本色
#define COLOR_WHITE				(D3DCOLOR_XRGB(0xFF,0xFF,0xFF))												///< カラー定義：白
#define COLOR_BLACK				(D3DCOLOR_XRGB(0x00,0x00,0x00))												///< カラー定義：黒
#define COLOR_RED				(D3DCOLOR_XRGB(0xFF,0x00,0x00))												///< カラー定義：赤
#define COLOR_GREEN				(D3DCOLOR_XRGB(0x00,0xFF,0x00))												///< カラー定義：緑
#define COLOR_BLUE				(D3DCOLOR_XRGB(0x00,0x00,0xFF))												///< カラー定義：青
#define COLOR_ALPHA(A)			(D3DCOLOR_ARGB(A,0xFF,0xFF,0xFF))											///< カラー定義：アルファ
#define COLOR_BRIGHT(B)			(D3DCOLOR_XRGB(B,B,B))														///< カラー定義：明度

#define LZSS_RING_LENGTH		4096																		///< 環状バッファの大きさ
#define LZSS_RING_MASK			(LZSS_RING_LENGTH-1)														///< 環状バッファのマスク
#define LZSS_LONGEST_MATCH		16																			///< 最長一致長
#define NIL						LZSS_RING_LENGTH															///< 木の末端

#define COLORKEY_DISABLE		(D3DCOLOR_ARGB(0x00,0x00,0x00,0x00))										///< テクスチャ読み込み時カラーキー不要
#define COLORKEY_BLACK			(D3DCOLOR_ARGB(0xFF,0x00,0x00,0x00))										///< テクスチャ読み込み時カラーキー黒設定
#define COLORKEY_WHITE			(D3DCOLOR_ARGB(0xFF,0xFF,0xFF,0xFF))										///< テクスチャ読み込み時カラーキー白設定
#define COLORKEY_RED			(D3DCOLOR_ARGB(0xFF,0xFF,0x00,0x00))										///< テクスチャ読み込み時カラーキー赤設定
#define COLORKEY_GREEN			(D3DCOLOR_ARGB(0xFF,0x00,0xFF,0x00))										///< テクスチャ読み込み時カラーキー緑設定
#define COLORKEY_BLUE			(D3DCOLOR_ARGB(0xFF,0x00,0x00,0xFF))										///< テクスチャ読み込み時カラーキー青設定

#define LIGHT_MAX				(4)																			///< ライト最大数

#define PADLAYER_MAX			(256)																		///< パッド用レイヤー最大数
#define JOYSTICK_MAX			(16)																		///< ジョイスティック数最大数
#define KEY_BUFFER_MAX			(32)																		///< キーバッファ用バッファサイズ
#define ANIMATION_MAX			(256)																		///< アニメーション最大数

#define WM_SOCKET				(WM_USER + 1)																///< Socketイベント受信用メッセージ
#define WM_GRAPHNOTIFY			(WM_USER + 2)																///< ムービーイベント受信用メッセージ

#define THREAD_SYSTEM			(0xFFFFFFFF)																///< システム用スレッドを表す定数

#define RENDER_STATE_TBL_MAX	(210)
#define TEXTURE_STATE_TBL_MAX	(33)
#define SAMPLER_STATE_TBL_MAX	(14)

#define FILE_OPEN_READ(name)				::CreateFile( name, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL )
#define FILE_OPEN_WRITE(name)				::CreateFile( name, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL )
#define FILE_OPEN_READ_WRITE(name)			::CreateFile( name, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL )
#define FILE_CLOSE(handle)					::CloseHandle( handle )
#define FILE_GET_SIZE(handle)				::GetFileSize( handle, NULL )
#define FILE_READ(handle,ptr,size)			{Uint32 ReadBytes; ::ReadFile( handle, ptr, size, &ReadBytes, NULL);}
#define FILE_WRITE(handle,ptr,size)			{Uint32 WriteBytes; ::WriteFile( handle, ptr, size, &WriteBytes, NULL);}
#define FILE_SEEK_BEGIN(handle,pos)			::SetFilePointer( handle, pos, NULL, FILE_BEGIN )
#define FILE_SEEK_CURRENT(handle,pos)		::SetFilePointer( handle, pos, NULL, FILE_CURRENT )
#define FILE_SEEK_END(handle,pos)			::SetFilePointer( handle, pos, NULL, FILE_END )
#define FILE_SET_TIME(handle,ft0,ft1,ft2)	::SetFileTime( handle, ft0, ft1, ft2 )
#define FILE_GET_TIME(handle,ft0,ft1,ft2)	::GetFileTime( handle, ft0, ft1, ft2 )


//================================================================================
// OBJECT HANDLE
//================================================================================
typedef enum { INVALID_CAMERA		= -1 } LCAMERA;				///< カメラハンドル
typedef enum { INVALID_MODEL		= -1 } LMODEL;				///< モデルハンドル
typedef enum { INVALID_ANIMATION	= -1 } LANIMATION;			///< アニメーションハンドル
typedef enum { INVALID_SOUND		= -1 } LSOUND;				///< サウンドハンドル
typedef enum { INVALID_MUSIC		= -1 } LMUSIC;				///< ミュージックハンドル
typedef enum { INVALID_FIELD		= -1 } LFIELD;				///< フィールドハンドル
typedef enum { INVALID_MEDIA		= -1 } LMEDIA;				///< メディアハンドル
typedef enum { INVALID_LIST			= -1 } LLIST;				///< リストハンドル
typedef enum { INVALID_SHADERFX		= NULL } LSHADERFX;			///< エフェクトハンドル


typedef struct ___LSURFACE___      { void *pData; } *LSURFACE;
typedef struct ___LTEXTURE___      { void *pData; } *LTEXTURE;
typedef struct ___LSHEET___        { void *pData; } *LSHEET;
typedef struct ___LVMR9___         { void *pData; } *LVMR9;
typedef struct ___LSCENECONTROL___ { void *pData; } *LSCENECONTROL;
typedef struct ___LPOINT___        { void *pData; } *LPOINT;
typedef struct ___LPOINT3D___      { void *pData; } *LPOINT3D;
typedef struct ___LLINE___         { void *pData; } *LLINE;
typedef struct ___LLINE3D___       { void *pData; } *LLINE3D;
typedef struct ___LPRIMITIVE___    { void *pData; } *LPRIMITIVE;
typedef struct ___LPRIMITIVE3D___  { void *pData; } *LPRIMITIVE3D;
typedef struct ___LSPRITE___       { void *pData; } *LSPRITE;
typedef struct ___LSPRITE3D___     { void *pData; } *LSPRITE3D;
typedef struct ___LFONT___         { void *pData; } *LFONT;
typedef struct ___LFONTSPRITE___   { void *pData; } *LFONTSPRITE;
typedef struct ___LFONTSPRITE3D___ { void *pData; } *LFONTSPRITE3D;
typedef struct ___LMODELEX___      { void *pData; } *LMODELEX;


const LSURFACE			INVALID_SURFACE			= NULL;
const LTEXTURE			INVALID_TEXTURE			= NULL;
const LSHEET			INVALID_SHEET			= NULL;
const LVMR9				INVALID_VMR9			= NULL;
const LSCENECONTROL		INVALID_SCENECONTROL	= NULL;
const LPOINT			INVALID_POINT			= NULL;
const LPOINT3D			INVALID_POINT3D			= NULL;
const LLINE				INVALID_LINE			= NULL;
const LLINE3D			INVALID_LINE3D			= NULL;
const LPRIMITIVE		INVALID_PRIMITIVE		= NULL;
const LPRIMITIVE3D		INVALID_PRIMITIVE3D		= NULL;
const LSPRITE			INVALID_SPRITE			= NULL;
const LSPRITE3D			INVALID_SPRITE3D		= NULL;
const LFONT				INVALID_FONT			= NULL;
const LFONTSPRITE		INVALID_FONTSPRITE		= NULL;
const LFONTSPRITE3D		INVALID_FONTSPRITE3D	= NULL;
const LMODELEX			INVALID_MODELEX			= NULL;


//================================================================================
// ENUM
//================================================================================

/**
	@brief	サーフェイスフォーマット定義

	各種サーフェイス及びテクスチャフォーマットの定義。
*/
enum eSurfaceFormat
{
	FORMAT_FROM_FILE,					///< ファイルから

	FORMAT_DEPTH16,						///< 16Bit深度バッファ
	FORMAT_DEPTH32,						///< 32Bit深度バッファ
	FORMAT_DEPTHSTENCIL,				///< ステンシル深度バッファ

	FORMAT_DXT1,						///< DXT1圧縮フォーマットテクスチャ
	FORMAT_DXT3,						///< DXT3圧縮フォーマットテクスチャ
	FORMAT_DXT5,						///< DXT5圧縮フォーマットテクスチャ

	FORMAT_BACKBUFFER,					///< バックバッファーに準拠
	FORMAT_TARGET16,					///< 16Bitレンダリングターゲット
	FORMAT_TARGET32,					///< 32Bitレンダリングターゲット
	FORMAT_TARGETFloat,					///< 浮動小数点レンダリングターゲット

	FORMAT_ALPHAONLY,					///< アルファ用テクスチャ

	FORMAT_TEXTURE32,					///< 32Bitテクスチャフォーマットに準拠
	FORMAT_TEXTURE16,					///< 16Bitテクスチャフォーマットに準拠

	FORMAT_TEXTURE_2D,					///< 2D用汎用テクスチャフォーマット
	FORMAT_TEXTURE_3D,					///< 3D用汎用テクスチャフォーマット
};

/**
	@brief	頂点フォーマット定義

	主にLunaSpriteクラスで使用される頂点定義。
*/
enum eVertexPrimitiveType
{
	PRIM_VERTEX_UV0,					///< テクスチャステージ数０の頂点データ
	PRIM_VERTEX_UV1,					///< テクスチャステージ数１の頂点データ
	PRIM_VERTEX_UV2,					///< テクスチャステージ数２の頂点データ
	PRIM_VERTEX_UV3,					///< テクスチャステージ数３の頂点データ
	PRIM_VERTEX_UV4,					///< テクスチャステージ数４の頂点データ
};


/**
	@brief	アルファブレンディングモード定義

	各種アルファブレンディングモードの定義。
*/
enum eBlendType
{
	BLEND_NONE,							///< アルファブレンド無効
	BLEND_NORMAL,						///< 通常ブレンディング
	BLEND_ADD,							///< 加算ブレンディング
	BLEND_ADD_NOALPHA,					///< 加算ブレンディング（アルファ影響なし
	BLEND_MUL,							///< 乗算ブレンディング
	BLEND_REVERSE,						///< 反転ブレンディング
};


//================================================================================
// STRUCTURE
//================================================================================
/**
	@brief	サーフェイス用デバイスステート
*/
struct MATERIALSTATE
{
	Uint32		SamplerState[TEXTURE_STAGE_MAX][SAMPLER_STATE_TBL_MAX];
	Uint32		TextureState[TEXTURE_STAGE_MAX][TEXTURE_STATE_TBL_MAX];
	eBlendType	BlendType;
	Bool		IsColorKey;
	Bool		IsBilinear;
	Bool		IsZBuffer;
	Bool		IsZWrite;
};


/**
	@brief	点データ

	Lunaで使用される点表現用の構造体。
*/
typedef struct CLunaPoint
{
	Float Px;					///< Ｘ座標
	Float Py;					///< Ｙ座標

	/**
		@brief 値設定
		値を簡略的に設定するための関数。

		@param px		[in] Ｘ座標
		@param py		[in] Ｙ座標
	*/
	void Set( Float px, Float py )
	{
		Px = px;
		Py = py;
	}
}
LUNAPOINT;

/**
	@brief	矩形データ

	Lunaで使用される矩形表現用の構造体。
*/
typedef struct CLunaRect
{
	Float Px;					///< Ｘ始点座標
	Float Py;					///< Ｙ始点座標
	Float Sx;					///< Ｘサイズ
	Float Sy;					///< Ｙサイズ

	/**
		@brief 値設定
		値を簡略的に設定するための関数。

		@param px		[in] Ｘ始点座標
		@param py		[in] Ｙ始点座標
		@param sx		[in] Ｘサイズ
		@param sy		[in] Ｙサイズ
	*/
	void Set( Float px, Float py, Float sx, Float sy )
	{
		Px = px;
		Py = py;
		Sx = sx;
		Sy = sy;
	}
}
LUNARECT;

/**
	@brief	クリッピング領域

	クリッピング用の矩形データ
*/
struct CClipRect
{
	Sint32 x1;					///< Ｘ位置始点
	Sint32 y1;					///< Ｙ位置始点
	Sint32 x2;					///< Ｘ位置終点
	Sint32 y2;					///< Ｙ位置終点
};

/**
	@brief	フォント描画矩形

	フォント描画用のクリッピング＆描画用矩形データ
*/
struct CFontDrawRect
{
	Sint32 x1;					///< Ｘ始点座標
	Sint32 y1;					///< Ｙ始点座標
	Sint32 x2;					///< Ｘ終点座標
	Sint32 y2;					///< Ｙ終点座標
	Sint32 Pitch;					///< フォント画像のピッチ
	void *pBits;				///< フォントのピクセルデータの先頭ポインタ
};

/**
	@brief	３Ｄデバイス情報

	Luna3Dクラスで使用する、デバイスの情報を全て
	格納しておくための構造体。
*/
struct DEVICEINFORMATION
{
	Bool IsPureDevice;							///< ピュアデバイスをサポートしてるかどうか
	Bool IsHardwareTnL;							///< ハードウェアＴｎＬをサポートしてるかどうか
	Bool IsSoftwareTnL;							///< ソフトウェアＴｎＬをサポートしてるかどうか

	D3DDEVTYPE DeviceType;						///< デバイスタイプ

	Bool IsAnisotropy;							///< 異方性フィルタリングをサポートしてるか否か
	Bool IsDither;								///< ディザをサポートしてるか否か
	Bool IsDepthBias;							///< 深度バイアスをサポートしてるか否か
	Bool IsWBuffer;								///< Ｗバッファをサポートしてるか否か
	Bool IsAutoMipMap;							///< ミップマップ自動生成をサポートしているか否か

	Bool IsZCmpAlways;							///< ALWAYS深度比較をサポートしているか否か
	Bool IsZCmpEqual;							///< EQUAL深度比較をサポートしているか否か
	Bool IsZCmpGreater;							///< GREATER深度比較をサポートしているか否か
	Bool IsZCmpGreaterEqual;					///< GREATEREQUAL深度比較をサポートしているか否か
	Bool IsZCmpLess;							///< LESS深度比較をサポートしているか否か
	Bool IsZCmpLessEqual;						///< LESSEQUAL深度比較をサポートしているか否か
	Bool IsZCmpNever;							///< NEVER深度比較をサポートしているか否か
	Bool IsZCmpNotEqual;						///< NOTEQUAL深度比較をサポートしているか否か

	Bool IsSrcBlendFactor;						///< ソースにブレンドファクターをサポートしているか否か
	Bool IsSrcBothInvSrc;						///< ソースにBOTHINVSRCブレンドをサポートしているか否か
	Bool IsSrcBothSrc;							///< ソースにBOTHSRCブレンドをサポートしているか否か
	Bool IsSrcDestAlpha;						///< ソースにDESTALPHAブレンドをサポートしているか否か
	Bool IsSrcDestColor;						///< ソースにDESTCOLORブレンドをサポートしているか否か
	Bool IsSrcInvDestAlpha;						///< ソースにINVDESTALPHAブレンドをサポートしているか否か
	Bool IsSrcInvDestColor;						///< ソースにINVDESTCOLORブレンドをサポートしているか否か
	Bool IsSrcInvSrcAlpha;						///< ソースにINVSRCALPHAブレンドをサポートしているか否か
	Bool IsSrcInvSrcColor;						///< ソースにINVSRCCOLORブレンドをサポートしているか否か
	Bool IsSrcOne;								///< ソースにONEブレンドをサポートしているか否か
	Bool IsSrcSrcAlpha;							///< ソースにSRCALPHAブレンドをサポートしているか否か
	Bool IsSrcSrcAphaSat;						///< ソースにSRCALPHASATブレンドをサポートしているか否か
	Bool IsSrcSrcColor;							///< ソースにSRCCOLORブレンドをサポートしているか否か
	Bool IsSrcZero;								///< ソースにSEROブレンドをサポートしているか否か

	Bool IsDstBlendFactor;						///< デスティネーションにソースにブレンドファクターをサポートしているか否か
	Bool IsDstBothInvSrc;						///< デスティネーションにBOTHINVSRCブレンドをサポートしているか否か
	Bool IsDstBothSrc;							///< デスティネーションにBOTHSRCブレンドをサポートしているか否か
	Bool IsDstDestAlpha;						///< デスティネーションにDESTALPHAブレンドをサポートしているか否か
	Bool IsDstDestColor;						///< デスティネーションにDESTCOLORブレンドをサポートしているか否か
	Bool IsDstInvDestAlpha;						///< デスティネーションにINVDESTALPHAブレンドをサポートしているか否か
	Bool IsDstInvDestColor;						///< デスティネーションにINVDESTCOLORブレンドをサポートしているか否か
	Bool IsDstInvSrcAlpha;						///< デスティネーションにINVSRCALPHAブレンドをサポートしているか否か
	Bool IsDstInvSrcColor;						///< デスティネーションにINVSRCCOLORブレンドをサポートしているか否か
	Bool IsDstOne;								///< デスティネーションにONEブレンドをサポートしているか否か
	Bool IsDstSrcAlpha;							///< デスティネーションにSRCALPHAブレンドをサポートしているか否か
	Bool IsDstSrcAphaSat;						///< デスティネーションにSRCALPHASATブレンドをサポートしているか否か
	Bool IsDstSrcColor;							///< デスティネーションにSRCCOLORブレンドをサポートしているか否か
	Bool IsDstZero;								///< デスティネーションにZEROブレンドをサポートしているか否か

	Bool IsCubeTexture;							///< キューブテークスチャをサポートしているか否か
	Bool IsMipmapTexture;						///< ミップマップテクスチャをサポートしているか否か
	Bool IsPerspective;							///< パースペクティブコレクトをサポートしているか否か
	Bool IsPow2;								///< ２のべき乗以外のサイズのテクスチャをサポートしているか否か
	Bool IsPow2Conditional;						///< 条件付きで２のべき乗以外のサイズのテクスチャをサポートしているか否か
	Bool IsSquareOnly;							///< 正方形以外のテクスチャをサポートしているか否か

	Bool IsTextureBorder;						///< BORDERテクスチャモードをサポートしてるか否か
	Bool IsTxtureClamp;							///< CLAMPテクスチャモードをサポートしてるか否か
	Bool IsTextureIndependent;					///< INDEPENDENTテクスチャモードをサポートしてるか否か
	Bool IsTextureMirror;						///< MIRRORテクスチャモードをサポートしてるか否か
	Bool IsTextureMirrorRonce;					///< MIRRORRONCEテクスチャモードをサポートしてるか否か
	Bool IsTextureWrap;							///< WRAPテクスチャモードをサポートしてるか否か

	Uint32 MaxAnisotropy;						///< 異方性フィルタ次数
	Uint32 MaxTextureWidth;						///< 最大テクスチャ横幅
	Uint32 MaxTextureHeight;					///< 最大テクスチャ縦幅
	Uint32 MaxTextureRepeat;					///< 最大テクスチャリピート数
	Uint32 MaxTextureAspectRatio;				///< 最大テクスチャアスペクト値

	Bool IsStencilKeep;							///< KEEPステンシル操作をサポートしてるか否か
	Bool IsStencilZero;							///< ZEROステンシル操作をサポートしてるか否か
	Bool IsStencilReplace;						///< REPLACEステンシル操作をサポートしてるか否か
	Bool IsStencilIncRsat;						///< INCRSATステンシル操作をサポートしてるか否か
	Bool IsStencilDecRsat;						///< DECRSATステンシル操作をサポートしてるか否か
	Bool IsStencilInvert;						///< INVERTステンシル操作をサポートしてるか否か
	Bool IsStencilIncR;							///< INCRステンシル操作をサポートしてるか否か
	Bool IsStencilDecR;							///< DECRステンシル操作をサポートしてるか否か
	Bool IsStencilTwoSided;						///< 両面ステンシル処理が可能かどうか

	Float MaxVertexW;							///< 最大頂点Ｗ値
	Float GuardBandLeft;						///< ガードバンドクリッピング左位置
	Float GuardBandTop;							///< ガードバンドクリッピング上位置
	Float GuardBandRight;						///< ガードバンドクリッピング右位置
	Float GuardBandBottom;						///< ガードバンドクリッピング下位置

	Float MaxPointSize;							///< 最大ポイントスプライトサイズ

	Uint32 MaxPrimitiveCount;					///< 最大一括描画可能ポリゴン数
	Uint32 MaxVertexIndex;						///< 最大一括描画可能頂点インデックス数

	Uint32 MaxMaxAnisotropy;					///< 異方性フィルタリング最大次数
	Uint32 MaxTextureBlendStages;				///< 最大テクスチャブレンディングステージ数
	Uint32 MaxSimultaneousTextures;				///< 最大テクスチャステージ数

	Uint32 MaxActiveLights;						///< 最大ハードウェアライト数

	Uint32 MaxVertexBlendMatrices;				///< 頂点ブレンド最大数
	Uint32 MaxVertexBlendMatrixIndex;			///< 頂点ウェイト最大数

	Uint32 VertexShaderVersion;					///< 頂点シェーダーバージョン
	Uint32 PixelShaderVersion;					///< ピクセルシェーダーバージョン
	Uint32 MaxVertexShaderConst;				///< 最大頂点シェーダー定数レジスタ数

	Uint32 DynamicFlowControlDepth;				///< 頂点シェーダーでの動的フロー制御最大ネスト数
	Uint32 StaticFlowControlDepth;				///< 頂点シェーダーでの静的フロー制御最大ネスト数
};

/**
	@brief	圧縮データヘッダ

	LunaCompresクラスで使用する、圧縮データのヘッダ。
*/
struct LZSSHEADER
{
	char Chunk[4];					///< 専用のファイルであることをしめす識別子。常に'L''Z''S''S'
	Uint32 DecodeSize;				///< 展開後のデータサイズ
	Uint32 Reserved[2];				///< 予約領域（常に０）
};

/**
	@brief	16Bitピクセルデータ

	16Bitピクセルフォーマットの構造体。
*/
struct PIXELDATA16
{
	Uint16 a : 4;				///< アルファ成分
	Uint16 r : 4;				///< 赤成分
	Uint16 g : 4;				///< 緑成分
	Uint16 b : 4;				///< 青成分
};

/**
	@brief	32Bitピクセルデータ

	32Bitピクセルフォーマットの構造体。
*/
struct PIXELDATA32
{
	Uint8 b;				///< 青成分
	Uint8 g;				///< 緑成分
	Uint8 r;				///< 赤成分
	Uint8 a;				///< アルファ成分
};

/**
	@brief	64Bitピクセルデータ

	64Bitピクセルフォーマットの構造体。
*/
struct PIXELDATA64
{
	Uint16 b;				///< 青成分
	Uint16 g;				///< 緑成分
	Uint16 r;				///< 赤成分
	Uint16 a;				///< アルファ成分
};

/**
	@brief	Float型ピクセルデータ

	浮動小数点ピクセルフォーマットの構造体。
*/
struct PIXELDATAFloat
{
	Float b;						///< 青成分
	Float g;						///< 緑成分
	Float r;						///< 赤成分
	Float a;						///< アルファ成分
};

/**
	@brief	プリミティブ用点情報

	２Ｄプリミティブのレンダリングキューに
	データを追加する時に使用する汎用頂点データ。
*/
typedef struct CPrimitivePoint2D
{
	Float x;							///< Ｘ位置
	Float y;							///< Ｙ位置
	Float z;							///< Ｚ位置
	Float w;							///< Ｗ位置
	D3DCOLOR c;							///< 頂点色
	Float u1;							///< ＵＶステージ１
	Float v1;							///< ＵＶステージ１
	Float u2;							///< ＵＶステージ２
	Float v2;							///< ＵＶステージ２
	Float u3;							///< ＵＶステージ３
	Float v3;							///< ＵＶステージ３
	Float u4;							///< ＵＶステージ４
	Float v4;							///< ＵＶステージ４
}
PRIMITIVEPOINT, PRIMITIVEPOINT2D;

/**
	@brief	３Ｄプリミティブ用点情報

	３Ｄプリミティブのレンダリングキューに
	データを追加する時に使用する汎用頂点データ。
*/
typedef struct CPrimitivePoint3D
{
	Float x;							///< Ｘ位置
	Float y;							///< Ｙ位置
	Float z;							///< Ｚ位置
	D3DCOLOR c;							///< 頂点色
	Float u1;							///< ＵＶステージ１
	Float v1;							///< ＵＶステージ１
	Float u2;							///< ＵＶステージ２
	Float v2;							///< ＵＶステージ２
	Float u3;							///< ＵＶステージ３
	Float v3;							///< ＵＶステージ３
	Float u4;							///< ＵＶステージ４
	Float v4;							///< ＵＶステージ４
}
PRIMITIVEPOINT3D;

/**
	@brief	２Ｄポリゴン

	通常のシンプルな２Ｄ描画を行う時に使用される頂点データ構造体。
*/
typedef struct CVertex2D
{
	Float x;							///< Ｘ位置
	Float y;							///< Ｙ位置
	Float z;							///< Ｚ位置
	Float w;							///< Ｗ位置
	D3DCOLOR c;							///< 頂点色
	Float u;							///< ＵＶ
	Float v;							///< ＵＶ

	/**
		CVertex2D頂点用ＦＶＦ定義
	*/
	enum {
		/// ＦＶＦフォーマット定義
		FVF = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
	};
}
VERTEX2D;

/**
	@brief	プリミティブ頂点

	テクスチャＵＶを４ステージ分含んだプリミティブ用頂点
*/
struct PRIMVERTEX0
{
	Float x;							///< Ｘ位置
	Float y;							///< Ｙ位置
	Float z;							///< Ｚ位置
	Float w;							///< Ｗ位置
	D3DCOLOR c;							///< 頂点色

	/**
		PRIMVERTEX0頂点用ＦＶＦ定義
	*/
	enum {
		/// ＦＶＦフォーマット定義
		FVF = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
	};
};


/**
	@brief	プリミティブ頂点

	テクスチャＵＶを１ステージ分含んだプリミティブ用頂点
*/
struct PRIMVERTEX1
{
	Float x;							///< Ｘ位置
	Float y;							///< Ｙ位置
	Float z;							///< Ｚ位置
	Float w;							///< Ｗ位置
	D3DCOLOR c;							///< 頂点色
	Float u1;							///< ＵＶステージ１
	Float v1;							///< ＵＶステージ１

	/**
		PRIMVERTEX1頂点用ＦＶＦ定義
	*/
	enum {
		/// ＦＶＦフォーマット定義
		FVF = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
	};
};

/**
	@brief	プリミティブ頂点

	テクスチャＵＶを２ステージ分含んだプリミティブ用頂点
*/
struct PRIMVERTEX2
{
	Float x;							///< Ｘ位置
	Float y;							///< Ｙ位置
	Float z;							///< Ｚ位置
	Float w;							///< Ｗ位置
	D3DCOLOR c;							///< 頂点色
	Float u1;							///< ＵＶステージ１
	Float v1;							///< ＵＶステージ１
	Float u2;							///< ＵＶステージ２
	Float v2;							///< ＵＶステージ２

	/**
		PRIMVERTEX2頂点用ＦＶＦ定義
	*/
	enum {
		/// ＦＶＦフォーマット定義
		FVF = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX2)
	};
};

/**
	@brief	プリミティブ頂点

	テクスチャＵＶを３ステージ分含んだプリミティブ用頂点
*/
struct PRIMVERTEX3
{
	Float x;							///< Ｘ位置
	Float y;							///< Ｙ位置
	Float z;							///< Ｚ位置
	Float w;							///< Ｗ位置
	D3DCOLOR c;							///< 頂点色
	Float u1;							///< ＵＶステージ１
	Float v1;							///< ＵＶステージ１
	Float u2;							///< ＵＶステージ２
	Float v2;							///< ＵＶステージ２
	Float u3;							///< ＵＶステージ３
	Float v3;							///< ＵＶステージ３

	/**
		PRIMVERTEX3頂点用ＦＶＦ定義
	*/
	enum {
		/// ＦＶＦフォーマット定義
		FVF = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX3)
	};
};

/**
	@brief	プリミティブ頂点

	テクスチャＵＶを４ステージ分含んだプリミティブ用頂点
*/
struct PRIMVERTEX4
{
	Float x;							///< Ｘ位置
	Float y;							///< Ｙ位置
	Float z;							///< Ｚ位置
	Float w;							///< Ｗ位置
	D3DCOLOR c;							///< 頂点色
	Float u1;							///< ＵＶステージ１
	Float v1;							///< ＵＶステージ１
	Float u2;							///< ＵＶステージ２
	Float v2;							///< ＵＶステージ２
	Float u3;							///< ＵＶステージ３
	Float v3;							///< ＵＶステージ３
	Float u4;							///< ＵＶステージ４
	Float v4;							///< ＵＶステージ４

	/**
		PRIMVERTEX4頂点用ＦＶＦ定義
	*/
	enum {
		/// ＦＶＦフォーマット定義
		FVF = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX4)
	};
};

/**
	@brief	３Ｄプリミティブ頂点

	テクスチャＵＶを含まない３Ｄプリミティブ用頂点
*/
struct PRIMVERTEX3D0
{
	Float x;							///< Ｘ位置
	Float y;							///< Ｙ位置
	Float z;							///< Ｚ位置
	D3DCOLOR c;							///< 頂点色

	/**
		PRIMVERTEX3D0頂点用ＦＶＦ定義
	*/
	enum {
		/// ＦＶＦフォーマット定義
		FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE)
	};
};

/**
	@brief	３Ｄプリミティブ頂点

	テクスチャＵＶを１ステージ分含んだ３Ｄプリミティブ用頂点
*/
struct PRIMVERTEX3D1
{
	Float x;							///< Ｘ位置
	Float y;							///< Ｙ位置
	Float z;							///< Ｚ位置
	D3DCOLOR c;							///< 頂点色
	Float u1;							///< ＵＶステージ１
	Float v1;							///< ＵＶステージ１

	/**
		PRIMVERTEX3D1頂点用ＦＶＦ定義
	*/
	enum {
		/// ＦＶＦフォーマット定義
		FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
	};
};

/**
	@brief	３Ｄプリミティブ頂点

	テクスチャＵＶを２ステージ分含んだ３Ｄプリミティブ用頂点
*/
struct PRIMVERTEX3D2
{
	Float x;							///< Ｘ位置
	Float y;							///< Ｙ位置
	Float z;							///< Ｚ位置
	D3DCOLOR c;							///< 頂点色
	Float u1;							///< ＵＶステージ１
	Float v1;							///< ＵＶステージ１
	Float u2;							///< ＵＶステージ２
	Float v2;							///< ＵＶステージ２

	/**
		PRIMVERTEX3D2頂点用ＦＶＦ定義
	*/
	enum {
		/// ＦＶＦフォーマット定義
		FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX2)
	};
};

/**
	@brief	３Ｄプリミティブ頂点

	テクスチャＵＶを３ステージ分含んだ３Ｄプリミティブ用頂点
*/
struct PRIMVERTEX3D3
{
	Float x;							///< Ｘ位置
	Float y;							///< Ｙ位置
	Float z;							///< Ｚ位置
	D3DCOLOR c;							///< 頂点色
	Float u1;							///< ＵＶステージ１
	Float v1;							///< ＵＶステージ１
	Float u2;							///< ＵＶステージ２
	Float v2;							///< ＵＶステージ２
	Float u3;							///< ＵＶステージ３
	Float v3;							///< ＵＶステージ３

	/**
		PRIMVERTEX3D3頂点用ＦＶＦ定義
	*/
	enum {
		/// ＦＶＦフォーマット定義
		FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX3)
	};
};

/**
	@brief	３Ｄプリミティブ頂点

	テクスチャＵＶを４ステージ分含んだ３Ｄプリミティブ用頂点
*/
struct PRIMVERTEX3D4
{
	Float x;							///< Ｘ位置
	Float y;							///< Ｙ位置
	Float z;							///< Ｚ位置
	D3DCOLOR c;							///< 頂点色
	Float u1;							///< ＵＶステージ１
	Float v1;							///< ＵＶステージ１
	Float u2;							///< ＵＶステージ２
	Float v2;							///< ＵＶステージ２
	Float u3;							///< ＵＶステージ３
	Float v3;							///< ＵＶステージ３
	Float u4;							///< ＵＶステージ４
	Float v4;							///< ＵＶステージ４

	/**
		PRIMVERTEX3D4頂点用ＦＶＦ定義
	*/
	enum {
		/// ＦＶＦフォーマット定義
		FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX4)
	};
};

/**
	@brief	シェーダー自動生成用

	シェーダーを選択するときに使用するデータ構造体。
*/
struct SHADERCREATE
{
	Uint32 BoneWeightCount;		///< ボーンウェイト数
	Uint32 ShaderVersion;		///< シェーダーバージョン
	Bool IsSoftwareVP;					///< ソフトウェア処理か否か
};

/**
	@brief	２次元上の点

	LunaCollisionクラスで使われる点データ構造体。
*/
struct POINT2D
{
	Float Px;							///< Ｘ位置
	Float Py;							///< Ｙ位置
};

/**
	@brief	２次元上の円

	LunaCollisionクラスで使われる円データ構造体。
*/
struct CIRCLE2D
{
	POINT2D Center;						///< 円の中心
	Float Radius;						///< 円の半径
};

/**
	@brief	２次元上の四角

	LunaCollisionクラスで使われる四角形データ構造体。
*/
struct RECT2D
{
	Float Sx;							///< 左上Ｘ座標
	Float Sy;							///< 左上Ｙ座標
	Float Ex;							///< 右下Ｘ座標
	Float Ey;							///< 右下Ｙ座標
};

/**
	@brief	２次元上の多角形

	LunaCollisionクラスで使われる多角形データ構造体。
*/
struct POLYGON2D
{
	Sint32 Count;							///< ポイント配列のポイント数
	POINT2D *pPts;						///< ポリゴンを表すためのポイント配列のポインタ
};

/**
	@brief	２次元上の線

	LunaCollisionクラスで使われる線データ構造体。
*/
struct LINE2D
{
	POINT2D Pt0;						///< 線分始点位置
	POINT2D Pt1;						///< 線分終点位置
};

/**
	@brief	３次元上の点
*/
struct POINT3D
{
	Float Px;							///< Ｘ位置
	Float Py;							///< Ｙ位置
	Float Pz;							///< Ｚ位置

	POINT3D(){}														///< コントラクタ
	POINT3D( Float x, Float y, Float z ){ Px = x; Py = y; Pz = z; }	///< コントラクタ
	POINT3D( POINT3D &pt ) { Px = pt.Px; Py = pt.Py; Pz = pt.Pz; }	///< コントラクタ
};

/**
	@brief	３次元上の円（球

	LunaCollisionクラスで使用される球データ構造体。
*/
struct SPHERE3D
{
	POINT3D Center;						///< 球の中心
	Float Radius;						///< 球の半径

	SPHERE3D(){}															///< コンストラクタ
	SPHERE3D( POINT3D &pt, Float radius ){ Center = pt; Radius = radius; }	///< コンストラクタ
};

/**
	@brief	３次元上の線

	LunaCollisionクラスで使われる線データ構造体。
*/
struct LINE3D
{
	POINT3D Pt0;						///< 線分始点位置
	POINT3D Pt1;						///< 線分終点位置

	LINE3D(){}																///< コンストラクタ
	LINE3D( POINT3D &pt0, POINT3D &pt1 ) { Pt0 = pt0; Pt1 = pt1; }			///< コンストラクタ
};

/**
	@brief	３次元上の面

	LunaCollisionクラスで使われる面データ構造体。
*/
struct PLANE3D
{
	Float a;							///< 一般平面方程式におけるクリップ面の a 係数 
	Float b;							///< 一般平面方程式におけるクリップ面の b 係数 
	Float c;							///< 一般平面方程式におけるクリップ面の c 係数 
	Float d;							///< 一般平面方程式におけるクリップ面の d 係数 
};

/**
	@brief	３次元上の箱

	LunaCollisionクラスで使われる箱データ構造体。
*/
struct BOX3D
{
	POINT3D Points[8];					///< 箱の各角を表す頂点データ
	PLANE3D Planes[6];					///< 箱の各面を表す面データ
};

/**
	@brief	シェーダー用ライトデータ

    シェーダー用のライティングデータを
	格納するための構造体。
*/
struct SHADERLIGHTDATA
{
	BOOL IsActive[LIGHT_MAX];			///< ライトを使用するか否か
	BOOL IsDirection[LIGHT_MAX];		///< ディレクショナルライトか否か
	Float Attenuation[LIGHT_MAX];		///< ライト減衰率
	D3DXVECTOR4 Position[LIGHT_MAX];	///< ライト位置
	D3DXVECTOR4 Direction[LIGHT_MAX];	///< ライト方向
	D3DCOLORVALUE Color[LIGHT_MAX];		///< ライト色
	D3DCOLORVALUE Specular;				///< 反射光
	D3DCOLORVALUE Ambient;				///< 環境光
};

/**
	@brief	モデル用頂点

	ボーン無し頂点ベース頂点構造体。<BR>
	固定機能で処理可能なボーン無し頂点形式の構造体。
*/
struct BASEVERTEX
{
	D3DXVECTOR3 Pos;					///< 位置
	D3DXVECTOR3 Norm;					///< 法線
	D3DCOLOR Color;						///< 頂点色
	D3DXVECTOR2 Tex;					///< テクスチャＵＶ

	/**
		BASEVERTEX頂点用ＦＶＦ定義
	*/
	enum {
		/// ＦＶＦフォーマット定義
		FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)
	};
};

/**
	@brief	ボーンモデル用頂点

	ボーン有り頂点ベース頂点構造体。<BR>
	固定機能で処理可能なボーン有り頂点形式の構造体。
*/
struct BASEBONEVERTEX
{
	D3DXVECTOR3 Pos;					///< 位置
	Float Weight[3];					///< ウェイト
	Uint8 Index[4];				///< ボーンインデックス
	D3DXVECTOR3 Norm;					///< 法線
	D3DCOLOR Color;						///< 頂点色
	D3DXVECTOR2 Tex;					///< テクスチャＵＶ

	/**
		BASEBONEVERTEX頂点用ＦＶＦ定義
	*/
	enum {
		/// ＦＶＦフォーマット定義
		FVF = (D3DFVF_XYZB4 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)
	};
};

/**
	@brief	全データ保有頂点

	全頂点データ保有用頂点データ構造体。
	読み込まれたモデルーデータは必ずこの形式の
	頂点データを保有しています。
*/
struct VERTEXALL
{
	D3DXVECTOR3 Pos;					///< 位置
	D3DXVECTOR3 Norm;					///< 法線
	D3DXVECTOR3 PosMorph;				///< ターゲット頂点
	D3DXVECTOR3 NormMorph;				///< ターゲット法線
	Float Weight[3];					///< ウェイト
	Uint8 Index[4];				///< ボーンインデックス
	D3DCOLOR Color;						///< 頂点色
	D3DXVECTOR2 Tex;					///< テクスチャＵＶ
};

/**
	@brief	通常頂点

	通常の透視変換用の頂点構造体。
*/
struct VERTEXNORMAL
{
	D3DXVECTOR3 Pos;					///< 位置
	D3DXVECTOR3 Norm;					///< 法線
	D3DCOLOR Color;						///< 頂点色
	D3DXVECTOR2 Tex;					///< テクスチャＵＶ
};

/**
	@brief	モーフィング用頂点

	モーフィング処理が可能な頂点の構造体。
*/
struct VERTEXMORPH
{
	D3DXVECTOR3 Pos;					///< 位置
	D3DXVECTOR3 Norm;					///< 法線
	D3DXVECTOR3 PosMorph;				///< ターゲット位置
	D3DXVECTOR3 NormMorph;				///< ターゲット法線
	D3DCOLOR Color;						///< 頂点色
	D3DXVECTOR2 Tex;					///< テクスチャＵＶ
};

/**
	@brief	ボーン２ウェイト頂点

	最大２本のボーンの影響を受ける頂点の構造体。
*/
struct VERTEXBONE2
{
	D3DXVECTOR3 Pos;					///< 位置
	D3DXVECTOR3 Norm;					///< 法線
	Float Weight[1];					///< ウェイト
	Uint8 Index[4];				///< ボーンインデックス
	D3DCOLOR Color;						///< 頂点色
	D3DXVECTOR2 Tex;					///< テクスチャＵＶ
};

/**
	@brief	ボーン３ウェイト頂点

	最大３本のボーンの影響を受ける頂点の構造体。
*/
struct VERTEXBONE3
{
	D3DXVECTOR3 Pos;					///< 位置
	D3DXVECTOR3 Norm;					///< 法線
	Float Weight[2];					///< ウェイト
	Uint8 Index[4];				///< ボーンインデックス
	D3DCOLOR Color;						///< 頂点色
	D3DXVECTOR2 Tex;					///< テクスチャＵＶ 
};

/**
	@brief	ボーン４ウェイト頂点

	最大４本のボーンの影響を受ける頂点の構造体。
*/
struct VERTEXBONE4
{
	D3DXVECTOR3 Pos;					///< 位置
	D3DXVECTOR3 Norm;					///< 法線
	Float Weight[3];					///< ウェイト
	Uint8 Index[4];				///< ボーンインデックス
	D3DCOLOR Color;						///< 頂点色
	D3DXVECTOR2 Tex;					///< テクスチャＵＶ
};

/**
	@brief	ボーン２ウェイトモーフィング頂点

	最大２本のボーンの影響を受けるモーフィング有り頂点の構造体。
*/
struct VERTEXBONE2MORPH
{
	D3DXVECTOR3 Pos;					///< 位置
	D3DXVECTOR3 Norm;					///< 法線
	D3DXVECTOR3 PosMorph;				///< ターゲット頂点
	D3DXVECTOR3 NormMorph;				///< ターゲット法線
	Float Weight[1];					///< ウェイト
	Uint8 Index[4];				///< ボーンインデックス
	D3DCOLOR Color;						///< 頂点色
	D3DXVECTOR2 Tex;					///< テクスチャＵＶ
};

/**
	@brief	ボーン３ウェイトモーフィング頂点

	最大３本のボーンの影響を受けるモーフィング有り頂点の構造体。
*/
struct VERTEXBONE3MORPH
{
	D3DXVECTOR3 Pos;					///< 位置
	D3DXVECTOR3 Norm;					///< 法線
	D3DXVECTOR3 PosMorph;				///< ターゲット頂点
	D3DXVECTOR3 NormMorph;				///< ターゲット法線
	Float Weight[2];					///< ウェイト
	Uint8 Index[4];				///< ボーンインデックス
	D3DCOLOR Color;						///< 頂点色
	D3DXVECTOR2 Tex;					///< テクスチャＵＶ
};

/**
	@brief	ボーン４ウェイトモーフィング頂点

	最大４本のボーンの影響を受けるモーフィング有り頂点の構造体。
*/
struct VERTEXBONE4MORPH
{
	D3DXVECTOR3 Pos;					///< 位置
	D3DXVECTOR3 Norm;					///< 法線
	D3DXVECTOR3 PosMorph;				///< ターゲット頂点
	D3DXVECTOR3 NormMorph;				///< ターゲット法線
	Float Weight[3];					///< ウェイト
	Uint8 Index[4];				///< ボーンインデックス
	D3DCOLOR Color;						///< 頂点色
	D3DXVECTOR2 Tex;					///< テクスチャＵＶ
};

/**
	@brief	バンプマップ用頂点

	バンプマップありの透視変換用の頂点構造体。
*/
struct VERTEXBUMPMAP
{
	D3DXVECTOR3 Pos;					///< 位置
	D3DXVECTOR3 Norm;					///< 法線
	D3DXVECTOR3 Tangent;				///< 接線
	D3DXVECTOR3 BiNorm;					///< 従法線
	D3DCOLOR Color;						///< 頂点色
	D3DXVECTOR2 Tex;					///< テクスチャＵＶ
};

/**
	@brief	ホスト情報

	ソケット使用時にホスト情報を格納するために使う構造体。
*/
struct HOSTINFO
{
	char Name[256];						///< ホスト名
	char Alias[16][256];				///< ホスト別名テーブル
	char Address[16][64];				///< IPアドレステーブル
	Sint32 Type;						///< データタイプ
	Sint32 Length;						///< データサイズ
	Sint32 AliasCount;					///< エイリアス数
	Sint32 AddressCount;				///< IPアドレス数
};

/**
	@brief	ソケット用送信データ定義

	ソケット使用時にデータを送信する際に使用されるデータ構造体。
*/
struct SOCKETSENDDATA
{
	Uint32 Send;					///< 送信済みデータサイズ
	Uint32 Size;					///< 送信合計サイズ
	Uint8 *pData;					///< 送信データ
};

/**
	@brief	ソケット接続情報

	ソケットでの接続時の接続情報を<BR>
	格納しておくための構造体です
*/
struct CONNECTCLIENTDATA
{
	SOCKET Socket;							///< ソケットデータ
	SOCKADDR_IN SockAddr;					///< ソケットアドレス
	HOSTINFO HostInfo;						///< ホスト情報
	char IPAddress[64];						///< ＩＰアドレス
	char Buffer[1024];						///< バッファ
	Sint32 Length;							///< バッファ長さ

	Bool IsWriteEnable;						///< 書き込みＯＫフラグ
	Bool IsReadEnable;						///< 読み込みＯＫフラグ

	SOCKETSENDDATA SendData;				///< 送信データ

	CONNECTCLIENTDATA *pNext;				///< 次の接続情報へのポインタ
};

/**
	@brief	非同期IO用オーバーラップ構造体

	非同期IOを用いた処理をする時に<BR>
	使う情報を格納するための構造体です。
*/
struct OVERLAPEDDATA
{
	Bool			IsRequest;				///< 送受信リクエスト中か否か
	Sint32			RequestCode;			///< リクエスト結果コード
	WSAOVERLAPPED	Overlap;				///< 送受信リクエスト用データ
	WSABUF			Buffer;					///< 送信バッファ
};

/**
	@brief	ファイル管理用データ定義

	LunaFileクラスで用いられるファイル管理関係の構造体。
*/
struct FILEDATA
{
	HANDLE hFile;						///< ファイルハンドル
	BOOL IsPack;						///< パックファイルか否か
	Uint32 Start;						///< ファイル開始位置
	Uint32 Size;						///< ファイル終了位置
	FILETIME CreationTime;				///< 作成日時
	FILETIME LastAccessTime;			///< 最終アクセス日時
	FILETIME LastWriteTime;				///< 最終更新日時
	char FileName[MAX_PATH];					///< ファイル名
};

//=============================================================================
// DEFINE
//=============================================================================
#define SOCKET_LOG_LINE_MAX				(256)


//=============================================================================
// WORK ENUM
//=============================================================================

/**
	@brief	漢字コード定義

	LunaFontSpriteクラスで使用される<BR>
	S-JIS漢字コード表のインデックス算出用定義です
*/
enum
{
	CODE_ASCII_START		= 0x0000,
	CODE_ASCII_END			= 0x00FF,

	CODE_SJIS_A_START		= 0x8140,
	CODE_SJIS_A_END			= 0x9FFF,

	CODE_SJIS_B_START		= 0xE040,
	CODE_SJIS_B_END			= 0xEEFF,

	CODE_SJIS_C_START		= 0xF8A0,
	CODE_SJIS_C_END			= 0xFC4F,

	CODE_TABLE_SIZE			= (CODE_ASCII_END - CODE_ASCII_START) + (CODE_SJIS_A_END - CODE_SJIS_A_START) + (CODE_SJIS_B_END - CODE_SJIS_B_START) + (CODE_SJIS_C_END - CODE_SJIS_C_START),	///< 最終的なテーブルサイズ

	SHEET_MAX				= 32,		///< LunaFontSpriteクラスで使用するシート最大数
};

/**
	@brief	シートサイズ定義

	CSheetクラスで使用されるシートの各種定義です
*/
enum {
	SHEET_X_MAX		= 16,			///< Ｘ方向のシート最大数
	SHEET_Y_MAX		= 16,			///< Ｙ方向のシート最大数

	SHEET_SIZE_X	= 256,			///< シートのＸ方向最大サイズ
	SHEET_SIZE_Y	= 256,			///< シートのＹ方向最大サイズ
};

/**
	@brief	エフェクトパラメーターハンドル定義

	LunaModelExクラスで使用されるエフェクトハンドルの各種定義です
*/
enum {
	FX_HANDLE_mTransform,			///< 透視変換用行列
	FX_HANDLE_mWorld,				///< ワールド変換行列
	FX_HANDLE_mWorldView,			///< カメラ空間への変換行列

	FX_HANDLE_mBoneMatrixArray,		///< ボーン用行列
	FX_HANDLE_BoneWeightCount,		///< ボーンウェイト数

	FX_HANDLE_LightIsUse,			///< ライト：使用有無フラグ
	FX_HANDLE_LightIsDirectional,	///< ライト：平行光源フラグ（true:平行光源/false:点光源）
	FX_HANDLE_LightAttenuation,		///< ライト：スポットライト減衰率
	FX_HANDLE_LightColor,			///< ライト：色
	FX_HANDLE_LightParameter,		///< ライト：方向or位置（方向か位置かはLightIsDirectionalに依存）

	FX_HANDLE_MaterialDiffuse,		///< マテリアル：ディフーズ
	FX_HANDLE_MaterialAmbient,		///< マテリアル：アンビエント

	FX_HANDLE_vCameraPos,			///< カメラ位置
	FX_HANDLE_SlideTextureU,		///< テクスチャＵスライド値
	FX_HANDLE_SlideTextureV,		///< テクスチャＶスライド値
	FX_HANDLE_VertexExtrusion,		///< 頂点押し出し値
	FX_HANDLE_MorphWeight,			///< モーフィングウェイト

	FX_HANDLE_FogParameter,			///< フォグ：計算用
	FX_HANDLE_FogTransform,			///< フォグ：Ｚ値算出用

	FX_HANDLE_TextureStage0,		///< テクスチャ：ステージ０
	FX_HANDLE_TextureStage1,		///< テクスチャ：ステージ１
	FX_HANDLE_TextureStage2,		///< テクスチャ：ステージ２
	FX_HANDLE_TextureStage3,		///< テクスチャ：ステージ３

	FX_HANDLE_MAX,
};

/**
	@brief	頂点タイプ定義

	LunaModelExで使用される頂点フォーマット定義。
*/
enum eVertexType
{
	VX_NORMAL,						///< 通常の透視変換用
	VX_MORPH,						///< モーフィング有り透視変換用
	VX_BONE2,						///< ２ボーン影響ボーン変換
	VX_BONE3,						///< ３ボーン影響ボーン変換
	VX_BONE4,						///< ４ボーン影響ボーン変換
	VX_BONE2MORPH,					///< ２ボーン影響ボーン変換＋モーフィング
	VX_BONE3MORPH,					///< ３ボーン影響ボーン変換＋モーフィング
	VX_BONE4MORPH,					///< ４ボーン影響ボーン変換＋モーフィング

	VX_UNKNOW,
};


//=============================================================================
// WORK STRUCT
//=============================================================================

/**
	@brief	LFDファイルヘッダ

	LFDファイル用のファイルヘッダ構造体。
*/
struct LFDHEADER
{
	char Guid[4];						///< ファイル識別情報（常に 'L' 'F' 'D' '\0'
	Uint8 FontSize;						///< フォントサイズ
	Uint8 SheetMax;						///< シート最大数
	Uint16 SheetWidth;					///< シート横幅
	Uint16 SheetHeight;					///< シート縦幅
	Uint16 FontMax;						///< フォント最大数
	Uint16 IndexTbl[CODE_TABLE_SIZE];	///< インデックステーブル
};

/**
	@brief	基本オブジェクト

	各オブジェクトの基本となる構造体です
*/
struct OBJECTBASE
{
	Bool IsUse;							///< 使用フラグ
	Uint32 RefCount;					///< 参照カウンタ
};

/**
	@brief	基本オブジェクトファイル

	各ファイルオブジェクトの基本となる構造体です
*/
struct OBJECTFILE : public OBJECTBASE
{
	char FileName[MAX_PATH];				///< ファイル名
};

/**
	@brief	アダプター情報

	DirectXGraphicsで使用されるアダプターの<BR>
	情報を格納しておくための構造体です
*/
struct ADAPTERDATA
{
	Sint32 ModeCount;						///< 画面モード数
	D3DDISPLAYMODE Mode[128];				///< 画面モード

	Sint32 DepthCount;						///< 深度フォーマット数
	D3DFORMAT DepthFormats[16];				///< 深度フォーマット

	Sint32 TextureCount;					///< テクスチャフォーマット数
	D3DFORMAT TextureFormats[128];			///< テクスチャフォーマット
	Bool TextureMipmap[128];				///< 自動ミップマップ生成

	Sint32 TargetCount;						///< レンダリングターゲットフォーマット数
	D3DFORMAT TargetFormats[32];			///< レンダリングターゲットフォーマット
};

/**
	@brief	サウンドデータ

	LunaSoundクラスで使用される、サウンドデータを<BR>
	管理するための構造体です
*/
struct SOUNDDATA : public OBJECTFILE
{
	CRITICAL_SECTION CriticalSection;		///< クリティカルセクション
	FILEDATA *pFile;						///< ファイルハンドル
	class CAyame *pAyame;					///< ドライバー

	// 制御用
	struct {
		Float fStart;
		Float fEnd;
		Sint32 Time;
		Sint32 TimeAll;
		Bool IsCtrl;
	} Volume, Pan;
};

/**
	@brief	メディアオブジェクト

	LunaMediaクラスで使用される、メディア関係の情報を<BR>
	管理するための構造体です
*/
struct MEDIADATA : public OBJECTFILE
{
	IGraphBuilder	*pGB;			///< DirectShowインターフェース
	IMediaSeeking	*pMS;			///< DirectShowインターフェース
	IMediaEventEx	*pME;			///< DirectShowインターフェース
	IVideoWindow	*pVW;			///< DirectShowインターフェース
	IMediaControl	*pMC;			///< DirectShowインターフェース
	IBasicAudio		*pBA;			///< DirectShowインターフェース
	Sint64			StartPos;		///< 開始位置
	Sint64			EndPos;			///< 終了位置
	Bool			IsPlay;			///< 再生フラグ
	Bool			IsLoop;			///< ループフラグ
	Bool			IsPause;		///< 一時停止フラグ
	Bool			IsTemp;			///< 一時ファイルからの読み込みか否か
};

/**
	@brief	ミュージックデータ

	LunaMusicクラスで使用される、シーケンスデータを<BR>
	管理するための構造体です
*/
struct MUSICDATA : public OBJECTFILE
{
	IDirectMusicPerformance8 *pPerformance;	///< パフォーマンスオブジェクト
	IDirectMusicSegment8 *pSegment;			///< セグメントオブジェクト
	IDirectMusicSegmentState *pState;		///< セグメントステータスオブジェクト

	void *pMusicData;						///< パックファイルからのデータ

	Bool IsPlay;							///< 再生中か否か
	Bool IsPause;							///< 一時停止中か否か
};

/**
	@brief	キーフレームデータ

	LunaAnimationクラスで使用される、アニメーションの<BR>
	キーフレームの情報を格納するための構造体です
*/
struct KEYFRAMEDATA
{
	Float fTime;							///< キー時間
	Float fParam;							///< キー値
	Bool IsIn;								///< スプライン時のIN係数を定数にするかどうか
	Bool IsOut;								///< スプライン時のOUT係数を定数にするかどうか
	Float fIn;								///< IN係数の定数
	Float fOut;								///< OUT係数の定数

	KEYFRAMEDATA *pPrev;					///< 前方キーフレームデータポインタ
	KEYFRAMEDATA *pNext;					///< 後方キーフレームデータポインタ
};

/**
	@brief	サウンドデータ

	LunaAnimationクラスで使用される、アニメーションの<BR>
	データを管理するための構造体です
*/
struct ANIMATIONDATA : public OBJECTBASE
{
	Float *pNewtonWork0;					///< ニュートン補間用ワーク
	Float *pNewtonWork1;					///< ニュートン補間用ワーク
	KEYFRAMEDATA *pKeys;					///< キーフレームデータポインタ
	Uint32 KeyMax;					///< キーフレーム数
	Float fTimeFirst;						///< 開始時間
	Float fTimeLast;						///< 終了時間
};

/**
	@brief	シェーダー転送用データ

	LunaModelExクラスで使用される、レンダリング時の<BR>
	シェーダーへの転送データを管理するための構造体です
*/
struct SHADERDATA
{
	CMatrix mWorld;							///< ワールド行列
	CMatrix mCamera;						///< カメラ行列
	CMatrix mTransform;						///< トランスオーム行列
	CMatrix mWorldView;						///< ワールドビュー行列
	CMatrix mInvRotateWorld;				///< 回転逆行列
	CMatrix mInvWorld;						///< ワールド逆行列

	Float fVertexExtrusion;					///< 頂点押し出し量
	Float fMorphWeight;						///< モーフィングウェイト
	Float fSlideTextureU;					///< Ｕマップスライド量
	Float fSlideTextureV;					///< Ｖマップスライド量
};

/**
	@brief	カメラデータ

	LunaCameraクラスで使用される、カメラデータを<BR>
	管理するための構造体です
*/
struct LUNACAMERADATA : public OBJECTBASE
{
	CMatrix mProjection;					///< プロジェクション行列
	CMatrix mView;							///< ビュー行列
	CMatrix mCamera;						///< カメラ行列
	CVector3D vPosition;						///< 位置

	CStyle Style;							///< 姿勢制御情報

	Float fFov;								///< カメラＦＯＶ
	Float fAspect;							///< カメラアスペクト
	Float fNearZ;							///< 最近平面
	Float fFarZ;							///< 最遠平面
};

/**
	@brief	マイクロスレッドデータ

	LunaMicroThreadクラスで使用される、<BR>
	スレッドデータを管理するための構造体です
*/
struct MICROTHREADDATA
{
	PMICROTHREADPROC pProcMain;							///< スレッドメイン関数
	PMICROTHREADPROC pProcTerminate;						///< スレッド終了関数

	Sint32 StackSize;									///< スタックサイズ

	unsigned int NextThreadNo;						///< 次のスレッドの番号
	unsigned int PrevThreadNo;						///< 前のスレッドの番号
	unsigned int ThreadId;							///< スレッドのＩＤ
	HANDLE hThread;									///< スレッドのハンドル
	HANDLE hSemaphore;								///< スレッドの管理用セマフォ

	Sint64 StartTime;								///< 計測開始時間
	Sint64 EndTime;								///< 計測終了時間
	Float CPUTime;									///< ＣＰＵ時間

	Bool IsActive;									///< アクティブか否か
};

/**
	@brief	ファイバーデータ

	LunaFiberクラスで使用される、<BR>
	ファイバーデータを管理するための構造体です
*/
struct FIBERDATA
{
	PFIBERPROC pProcMain;					///< スレッド用コールバック関数のポインタ
	unsigned long StackSize;				///< スタックサイズ
	Bool IsActive;							///< アクティブか否か
	Bool IsBoot;							///< 起動中か否か
	Bool IsSuspend;							///< サスペンド中か否か

	__int64 StartTime;						///< 開始時間
	__int64 EndTime;						///< 終了時間
	Float CPUTime;							///< ＣＰＵ使用時間（％）

	void *pFiber;							///< ファイバーのポインタ
	void *pNextFiber;						///< 次のファイバーのポインタ
};

/**
	@brief	エフェクトデータ

	LunaShaderFxクラスで使用される、エフェクト情報データを<BR>
	管理するための構造体です
*/
struct SHADERFXDATA : public OBJECTFILE
{
	ID3DXEffect *pEffect;
	Uint16 Major;
	Uint16 Minor;
};

/**
	@brief	モデルデータ

	LunaModelクラスで使用される、モデル情報データを<BR>
	管理するための構造体です
*/
struct MODELDATA : public OBJECTFILE
{
	ID3DXAnimationController *pAnimController;	///< アニメーションコントローラー
	D3DXFRAME *pFrameRoot;						///< フレームデータへのポインタ

	Sint32 MeshCount;								///< メッシュ数
	D3DXMESHCONTAINER **ppMeshTbl;				///< メッシュアクセステーブル

	Float fObjectRadius;						///< バウンディングスフィアの半径
	CVector3D vObjectCenter;						///< バウンディングスフィアの中心
	CVector3D vBBMin;							///< バウンディングボックス最小値
	CVector3D vBBMax;							///< バウンディングボックス最大値

	Float fSx;									///< Ｘ方向拡大縮小
	Float fSy;									///< Ｙ方向拡大縮小
	Float fSz;									///< Ｚ方向拡大縮小

	CStyle Style;								///< 姿勢制御情報
	CMatrix mWorld;								///< ワールド行列
};

/**
	@brief	マテリアルデータ

	LunaModelExクラスで使用される、マテリアルデータを<BR>
	管理するための構造体です
*/
struct LMATERIAL
{
	D3DMATERIAL9 Mat;						///< マテリアルデータ
	LTEXTURE lTex[4];						///< テクスチャハンドル
	Uint32 VertexStart;						///< 頂点開始位置
	Uint32 VertexCount;						///< 頂点数
	Uint32 IndexStart;						///< インデックス開始位置
	Uint32 FaceCount;						///< インデックス数
};


/**
	@brief	フィールドマテリアルデータ

	クオードツリー構築用マテリアルデータ
*/
struct LFIELDMATERIAL
{
	Sint32 IndexStart;
	Sint32 IndexCount;
};

/**
	@brief	フィールドメッシュデータ

	クオードツリー構築用メッシュデータ
*/
struct LFIELDMESH
{
	CVector3D BBMin;							///< バウンディングボックス最小点
	CVector3D BBMax;							///< バウンディングボックス最大点

	IDirect3DVertexDeclaration9 *pVD;		///< 頂点定義
	IDirect3DVertexBuffer9 *pVB;			///< 頂点バッファ
	IDirect3DIndexBuffer9 *pIB;				///< インデックスバッファ

	void *pVertexData;						///< 頂点データ
	void *pIndexData;						///< インデックスデータ
	Uint32 VertexCount;						///< 頂点数
	Uint32 IndexCount;						///< インデックス数

	CVector3D *pNormals;						///< 各面の法線

	LFIELDMATERIAL *pMaterial;				///< マテリアルデータ
};

/**
	@brief	フィールドフレームデータ

	クオードツリー構築用フレームデータ
*/
struct LFIELDFRAME
{
	LFIELDFRAME *pChild[4];					///< 子供フレームへのポインタ
	Sint32 MeshNo;							///< 関連付けられているメッシュ番号

	CVector3D BBMin;							///< バウンディングボックス最小点
	CVector3D BBMax;							///< バウンディングボックス最大点

	BOX3D Box;								///< バウンディングボックス
};

/**
	@brief	フィールドデータ

	LunaFieldクラスで使用される、フィールドデータを<BR>
	管理するための構造体です
*/
struct LUNAFIELDDATA : public OBJECTFILE
{
	Sint32 AreaCount;							///< エリア数
	LFIELDMESH *pMeshData;					///< メッシュデータ
	Bool *pIsDrawList;						///< 描画データリスト

	Bool IsBumpMap;							///< バンプマップの有無

	LFIELDFRAME *pFrameRoot;				///< フレームの有無

	LMATERIAL *pMaterial;					///< マテリアルデータポインタ
	Uint32 MaterialCount;			///< マテリアル数

	Uint16 Major;					///< シェーダーメジャーバージョン
	Uint16 Minor;					///< シェーダーマイナーバージョン

	D3DXHANDLE FxHandle[FX_HANDLE_MAX];		///< エフェクトハンドル
	LSHADERFX lShaderFx;					///< エフェクトハンドル
	Uint32 EffectSendData;			///< エフェクト送信データ
};

#endif // ___LUNABASE_H___

