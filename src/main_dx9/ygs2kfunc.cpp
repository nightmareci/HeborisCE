#include "include.h"
#include "ygs2kfunc.h"
#include "prototypes.h"
#include "kbgmplayer.h"
#include "mp3player.h"
#include "bassmod.h"

#define		YGS_TEXTURE_MAX		100
#define		YGS_SOUND_MAX		100
#define		YGS_KEYREPEAT_MAX	512
#define		YGS_JOYREPEAT_MAX	20
#define		YGS_PRIM_BUF_MAX	3200
#define		YGS_JOYPAD_MAX		2
#define		YGS_TEXTLAYER_MAX	16

struct SScreenInfo
{
	int		real_w;
	int		real_h;
	int		win_w;
	int		win_h;
	int		full_screen;
};

struct STextLayer
{
	bool	enable;
	int		x;
	int		y;
	int		r, g, b;
	int		size;
	char	string[256];
};

enum
{
	YGS_SOUNDTYPE_NONE,
	YGS_SOUNDTYPE_SOUND,
	YGS_SOUNDTYPE_MUSIC,
	YGS_SOUNDTYPE_MP3,
	YGS_SOUNDTYPE_MOD,
};

static CKBGMPlayer	s_BGM;
static CMP3Player	s_MP3;

static LTEXTURE		s_pYGSTexture[YGS_TEXTURE_MAX];
static int			s_iYGSSoundType[YGS_SOUND_MAX];
static LSOUND		s_pYGSSound[YGS_SOUND_MAX];
static int			s_iYGSSoundVolume[YGS_SOUND_MAX];
static char			s_szYGSExMusic[YGS_SOUND_MAX][128];
static int			s_iKeyRepeat[YGS_KEYREPEAT_MAX];
static int			s_iJoyRepeat[2][YGS_JOYREPEAT_MAX];
static LPRIMITIVE	s_pPrim;
static LFONT		lFont[3] = { INVALID_FONT };

static int			s_iActivePad = 0;
static LTEXTURE		s_pLastSetTexture = INVALID_TEXTURE;
static D3DCOLOR		m_iPrimColor = D3DCOLOR_RGBA(255, 255, 255, 255);
static float		s_fDrawTextScale = 1.0f;
static eSurfaceFormat s_iSurfaceFormat = FORMAT_DXT3;

static STextLayer	s_TextLayer[YGS_TEXTLAYER_MAX];
extern int			screenMode;

static int			s_iNewOffsetX = 0, s_iNewOffsetY = 0;
static int			s_iOffsetX = 0, s_iOffsetY = 0;

extern int			cpp_texdxt;
extern int			cpp_texfilter;

static SScreenInfo	s_ScreenInfo[] =
{
	{ 320, 240,  320,  240, true },		// 0
	{ 320, 240,  320,  240, false },	// 1
	{ 640, 480,  640,  480, false },	// 2
	{ 640, 480,  640,  480, true },		// 3
	{ 640, 480,  800,  600, false },	// 4
	{ 640, 480, 1024,  768, false },	// 5
	{ 640, 480, 1280,  960, false },	// 6
	{ 320, 240,  640,  480, true },		// 7
	{ 320, 240,  480,  360, false },	// 8
	{ 320, 240,  640,  480, false },	// 9
	{ 320, 240,  800,  600, false },	// 10
	{ 320, 240, 1024,  768, false },	// 11
	{ 320, 240, 1280,  960, false },	// 12
};

bool YGS2kInit()
{
	int		winWidth, winHeight;

	s_iNewOffsetX = 0;	s_iNewOffsetY = 0;
	s_iOffsetX = 0;		s_iOffsetY = 0;

	/* CONFIG.SAVより設定をロード */
	if ( LoadConfig() )
	{
		readdef::readdef();
		LoadConfig();
	}

	/* 画面モードの変更 */
	if ( screenMode >= 0 && screenMode <= 12 )
	{
		SScreenInfo		*s = &s_ScreenInfo[screenMode];
		Luna::SetVirtualScreenSize(s->win_w, s->win_h, s->real_w, s->real_h);
		Luna::ChangeScreenSize(s->win_w, s->win_h, Luna::IsFullScreenMode() != s->full_screen);
		s_fDrawTextScale = (float)s->win_w / (float)s->real_w;

		switch ( cpp_texdxt )
		{
		case 0:
		default:
			s_iSurfaceFormat = FORMAT_TEXTURE_2D;
			break;
		case 1:
			s_iSurfaceFormat = FORMAT_DXT3;
			break;
		case 2:
			s_iSurfaceFormat = (s->real_w >= 640) ? FORMAT_DXT3 : FORMAT_TEXTURE_2D;
			break;
		}
	}

	/* KBGMの初期化 */
	s_BGM.LibraryInitialize();

	/* DirectShowの初期化 */
	s_MP3.InitializeBGM();

	/* BASSMOD の初期化 */
	BASSMOD_Init(-1, 44100, 0);

	/* Lunaの初期化 */
	Luna3D::Refresh();
	Luna3D::Clear( D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0) );
	Luna3D::BeginScene();

	/* キーリピートバッファ初期化 */
	for ( int i = 0 ; i < YGS_KEYREPEAT_MAX ; i ++ )
	{
		s_iKeyRepeat[i] = 0;
	}

	for ( int pl = 0 ; pl < YGS_JOYPAD_MAX ; pl ++ )
	{
		for ( int i = 0 ; i < YGS_JOYREPEAT_MAX ; i ++ )
		{
			s_iJoyRepeat[pl][i] = 0;
		}
	}

	/* テクスチャ領域の初期化 */
	for ( int i = 0 ; i < YGS_TEXTURE_MAX ; i ++ )
	{
		s_pYGSTexture[i] = INVALID_TEXTURE;
	}

	/* サウンドの初期化 */
	for ( int i = 0 ; i < YGS_SOUND_MAX ; i ++ )
	{
		s_iYGSSoundType[i]   = YGS_SOUNDTYPE_NONE;
		s_iYGSSoundVolume[i] = 0;
		s_pYGSSound[i]       = INVALID_SOUND;
		s_szYGSExMusic[i][0] = '\0';
	}

	/* テキストレイヤーの初期化 */
	for ( int i = 0 ; i < YGS_TEXTLAYER_MAX ; i ++ )
	{
		memset(&s_TextLayer[i], 0, sizeof(STextLayer));
		s_TextLayer[i].r = s_TextLayer[i].g = s_TextLayer[i].b = 255;
		s_TextLayer[i].size = 16;
	}
	
	/* プリミティブバッファの初期化の初期化 */
	s_pLastSetTexture = INVALID_TEXTURE;
	s_pPrim = LunaPrimitive::Create(YGS_PRIM_BUF_MAX, PRIM_VERTEX_UV1, false);

	/* 文字描画の初期化 (フォントの描画先のバックバッファに設定) */
	lFont[0] = LunaFont::Create("ＭＳ ゴシック", (int)(12.0f * s_fDrawTextScale));
	LunaFont::SetDestination(lFont[0], INVALID_TEXTURE);
	lFont[1] = LunaFont::Create("ＭＳ ゴシック", (int)(10.0f * s_fDrawTextScale));
	LunaFont::SetDestination(lFont[0], INVALID_TEXTURE);
	lFont[2] = LunaFont::Create("ＭＳ ゴシック", (int)(16.0f * s_fDrawTextScale));
	LunaFont::SetDestination(lFont[0], INVALID_TEXTURE);

	LunaMath::RandSeed(time(NULL));
	return true;
}

void YGS2kExit()
{
	/* シーン終了 */
	Luna3D::EndScene();

	/* BGMの停止 */
	StopMIDI();
	s_MP3.Release();
	BASSMOD_Free();

	/* テクスチャ領域の解放 */
	for ( int i = 0 ; i < YGS_TEXTURE_MAX ; i ++ )
	{
		if ( s_pYGSTexture[i] )
		{
			LunaTexture::Release(s_pYGSTexture[i]);
			s_pYGSTexture[i] = INVALID_TEXTURE;
		}
	}

	/* サウンドの解放 */
	for ( int i = 0 ; i < YGS_SOUND_MAX ; i ++ )
	{
		s_iYGSSoundType[i] = YGS_SOUNDTYPE_NONE;

		if ( s_pYGSSound[i] != INVALID_SOUND )
		{
			LunaSound::Release(s_pYGSSound[i]);
			s_pYGSSound[i] = INVALID_SOUND;
		}
	}

	s_BGM.Free(0);

	/* その他解放 */
	LunaPrimitive::Release(s_pPrim);
	s_pPrim = INVALID_PRIMITIVE;

	LunaFont::Release(lFont[0]);
	LunaFont::Release(lFont[1]);
	LunaFont::Release(lFont[2]);
	lFont[0] = INVALID_FONT;
	lFont[1] = INVALID_FONT;
	lFont[2] = INVALID_FONT;
}

bool YGS2kHalt()
{
	/* バッファに溜まっている残りプリミティブを描く */
	YGS2kPrimitiveFlush();

	/* テキストレイヤーの描画 */
	for ( int i = 0 ; i < YGS_TEXTLAYER_MAX ; i ++ )
	{
		TextBlt(i);
	}

	/* シーン終了 */
	Luna3D::EndScene();

	/* メッセージループを待つ */
	Luna::WaitForMsgLoop();

	/* バックバッファの内容をウィンドウに転送 */
	Luna3D::Refresh();

	/* 画面クリア */
	Luna3D::Clear( D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0) );

	/* シーン開始 */
	Luna3D::BeginScene();

	/* 画面ずらし量の反映 */
	s_iOffsetX = s_iNewOffsetX;
	s_iOffsetY = s_iNewOffsetY;

	/* MP3の定期処理 */
	s_MP3.Exec();

	return true;
}

void YGS2kTextOut(int x, int y, char* text, int r, int g, int b, int size)
{
	YGS2kPrimitiveFlush();

	switch ( size )
	{
	case 12:
	default:
		LunaFont::Begin( lFont[0] );
		break;
	case 10:
		LunaFont::Begin( lFont[1] );
		break;
	case 16:
		LunaFont::Begin( lFont[2] );
		break;
	}

	LunaFont::DrawText( (int)((float)(x + s_iOffsetX) * s_fDrawTextScale), (int)((float)(y + s_iOffsetY) * s_fDrawTextScale), D3DCOLOR_XRGB(r, g, b), text );
	LunaFont::End();
}

void YGS2kPrimitiveFlush()
{
	if ( LunaPrimitive::GetRequestedVertexCount(s_pPrim) <= 0 ) return;

	Luna3D::SetBilinearEnable( cpp_texfilter == 1 );

	if ( s_pLastSetTexture != INVALID_TEXTURE )
	{
		Luna3D::SetTexture(0, s_pLastSetTexture);
		LunaPrimitive::UpdateBuffer(s_pPrim);
		LunaPrimitive::Rendering(s_pPrim);
		s_pLastSetTexture = INVALID_TEXTURE;
	}

	LunaPrimitive::ResetBuffer(s_pPrim);
	Luna3D::SetBilinearEnable( false );
}

void YGS2kSetExtraBltColor(int r, int g, int b, int a)
{
	m_iPrimColor = D3DCOLOR_RGBA(r, g, b, a);
}


void DrawPrim(LTEXTURE tex, LUNARECT *pDstRect, LUNARECT *pSrcRect, DWORD Color, float fPosZ = 0.0f)
{
	if ( tex != s_pLastSetTexture || LunaPrimitive::GetRequestedVertexCount(s_pPrim) + 6 >= YGS_PRIM_BUF_MAX )
	{
		YGS2kPrimitiveFlush();
	}

	Float fOffsetUV = LunaTexture::GetOffsetUV();

	Float x1 = pDstRect->Px + s_iOffsetX;
	Float y1 = pDstRect->Py + s_iOffsetY;
	Float x2 = pDstRect->Px + pDstRect->Sx + s_iOffsetX;
	Float y2 = pDstRect->Py + pDstRect->Sy + s_iOffsetY;

	Float u1, v1, u2, v2;

	s_pLastSetTexture = tex;

	// UV変換
	if ( tex != INVALID_TEXTURE )
	{
		// u1 = ((CTexture*)tex)->PixelToTexelU(pSrcRect->Px + fOffsetUV );
		// v1 = ((CTexture*)tex)->PixelToTexelV(pSrcRect->Py + fOffsetUV );
		// u2 = ((CTexture*)tex)->PixelToTexelU(pSrcRect->Px + pSrcRect->Sx - fOffsetUV );
		// v2 = ((CTexture*)tex)->PixelToTexelV(pSrcRect->Py + pSrcRect->Sy - fOffsetUV );

		u1 = (Float)(pSrcRect->Px) / LunaTexture::GetWidth(tex);
		v1 = (Float)(pSrcRect->Py) / LunaTexture::GetHeight(tex);
		u2 = (Float)(pSrcRect->Px + pSrcRect->Sx) / LunaTexture::GetWidth(tex);
		v2 = (Float)(pSrcRect->Py + pSrcRect->Sy) / LunaTexture::GetHeight(tex);
	}
	else
	{
		return;
	}

	CPrimitivePoint2D PrimPt[] = {
		{ x1, y1, fPosZ, 1.0f, Color, u1, v1, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
		{ x2, y1, fPosZ, 1.0f, Color, u2, v1, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
		{ x2, y2, fPosZ, 1.0f, Color, u2, v2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },

		{ x2, y2, fPosZ, 1.0f, Color, u2, v2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
		{ x1, y2, fPosZ, 1.0f, Color, u1, v2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
		{ x1, y1, fPosZ, 1.0f, Color, u1, v1, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
	};

	// プリミティブに追加
	LunaPrimitive::PushRenderBuffer( s_pPrim, PrimPt, 2 );
}

void DrawPrimRotate(LTEXTURE tex, LUNARECT *pDstRect, LUNARECT *pSrcRect, DWORD Color, Uint32 Angle, float cx = 0.0f, float cy = 0.0f, float fPosZ = 0.0f)
{
	if ( tex != s_pLastSetTexture || LunaPrimitive::GetRequestedVertexCount(s_pPrim) + 6 >= YGS_PRIM_BUF_MAX )
	{
		YGS2kPrimitiveFlush();
	}

	Float fOffsetUV = LunaTexture::GetOffsetUV();

	Float x1 = pDstRect->Px;
	Float y1 = pDstRect->Py;
	Float x2 = pDstRect->Px + pDstRect->Sx;
	Float y2 = pDstRect->Py + pDstRect->Sy;

	Float u1, v1, u2, v2;

	Float cx2 = (x1 + x2) * 0.5f + cx;
	Float cy2 = (y1 + y2) * 0.5f + cx;

	// 回転
	CLunaPoint Pt[4] = {
		{ x1, y1 }, { x2, y1 },
		{ x1, y2 }, { x2, y2 },
	};

	Pt[0].Px -= cx2;		Pt[0].Py -= cy2;
	Pt[1].Px -= cx2;		Pt[1].Py -= cy2;
	Pt[2].Px -= cx2;		Pt[2].Py -= cy2;
	Pt[3].Px -= cx2;		Pt[3].Py -= cy2;

	LunaMath::RotationPoint( &Pt[0], Angle );
	LunaMath::RotationPoint( &Pt[1], Angle );
	LunaMath::RotationPoint( &Pt[2], Angle );
	LunaMath::RotationPoint( &Pt[3], Angle );

	Pt[0].Px += cx2;		Pt[0].Py += cy2;
	Pt[1].Px += cx2;		Pt[1].Py += cy2;
	Pt[2].Px += cx2;		Pt[2].Py += cy2;
	Pt[3].Px += cx2;		Pt[3].Py += cy2;

	s_pLastSetTexture = tex;

	// UV変換
	if ( tex != INVALID_TEXTURE )
	{
		u1 = ((CTexture*)tex)->PixelToTexelU(pSrcRect->Px + fOffsetUV );
		v1 = ((CTexture*)tex)->PixelToTexelV(pSrcRect->Py + fOffsetUV );
		u2 = ((CTexture*)tex)->PixelToTexelU(pSrcRect->Px + pSrcRect->Sx - fOffsetUV );
		v2 = ((CTexture*)tex)->PixelToTexelV(pSrcRect->Py + pSrcRect->Sy - fOffsetUV );
	}
	else
	{
		return;
	}

	CPrimitivePoint2D PrimPt[] = {
		{ Pt[0].Px, Pt[0].Py, fPosZ, 1.0f, Color, u1, v1, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
		{ Pt[1].Px, Pt[1].Py, fPosZ, 1.0f, Color, u2, v1, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
		{ Pt[3].Px, Pt[3].Py, fPosZ, 1.0f, Color, u2, v2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
												  
		{ Pt[3].Px, Pt[3].Py, fPosZ, 1.0f, Color, u2, v2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
		{ Pt[2].Px, Pt[2].Py, fPosZ, 1.0f, Color, u1, v2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
		{ Pt[0].Px, Pt[0].Py, fPosZ, 1.0f, Color, u1, v1, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
	};

	// プリミティブに追加
	LunaPrimitive::PushRenderBuffer( s_pPrim, PrimPt, 2 );
}

int IsPlayMIDI()
{
	SMFINFO		info;
	if ( s_BGM.GetInfo(0, &info) == KBGM_NOERROR )
	{
		return info.Playing ? 1 : 0;
	}
	return 0;
}

void SelectJoyStick( int pl )
{
	s_iActivePad = pl;
}

int IsPushKey ( int key )
{
	return s_iKeyRepeat[key] == 1 ? 1 : 0;
}

int IsPressKey ( int key )
{
	if ( key >= 0 && key <= 255 )
	{
		return LunaInput::GetKeyData( key ) ? 1 : 0;
	}
	else
	{
		return 0;
	}
}

int IsPushJoyKey ( int key )
{
	return s_iJoyRepeat[s_iActivePad][key] == 1 ? 1 : 0;
}

int IsPressJoyKey ( int key )
{
	JOYSTICKDATA Joy;

	if ( LunaInput::GetJoystickData( s_iActivePad, &Joy ) )
	{
		switch ( key )
		{
		case 0:		// 上
			if ( Joy.Py < -65536 / 4 ) return 1;
			break;

		case 1:		// 下
			if ( Joy.Py > +65536 / 4 ) return 1;
			break;

		case 2:		// 左
			if ( Joy.Px < -65536 / 4 ) return 1;
			break;

		case 3:		// 右
			if ( Joy.Px > +65536 / 4 ) return 1;
			break;

		default:
			int		key2 = key - 4;
			if ( key2 >= 0 && key2 < 16 && Joy.Button[key2] ) return 1;
			break;
		}
	}

	return 0;
}

int IsPushReturnKey()
{
	return IsPushKey(DIK_RETURN);
}

int IsPushDeleteKey()
{
	return IsPushKey(DIK_DELETE);
}

int IsPushBSKey()
{
	return IsPushKey(DIK_BACKSPACE);
}

int IsPushEscKey()
{
	return IsPushKey(DIK_ESCAPE);
}

int IsPushEndKey()
{
	return IsPushKey(DIK_END);
}

int getMaxKey()
{
	return YGS_KEYREPEAT_MAX;
}

void SetJoyButtonMax ( int max )
{

}

void SetConstParam ( char *param, int value )
{

}

void KeyInput()
{
	int		padbak = s_iActivePad;

	for ( int i = 0 ; i < YGS_KEYREPEAT_MAX ; i ++ )
	{
		if ( IsPressKey(i) )
		{
			s_iKeyRepeat[i] ++;
		}
		else
		{
			s_iKeyRepeat[i] = 0;
		}
	}

	
	for ( int pl = 0 ; pl < 2 ; pl ++ )
	{
		s_iActivePad = pl;

		for ( int i = 0 ; i < YGS_JOYREPEAT_MAX ; i ++ )
		{
			if ( IsPressJoyKey(i) )
			{
				s_iJoyRepeat[pl][i] ++;
			}
			else
			{
				s_iJoyRepeat[pl][i] = 0;
			}
		}
	}

	s_iActivePad = padbak;
}

int Rand ( int max )
{
	return LunaMath::Rand(0, max - 1);
}

void PauseMIDI()
{
	s_BGM.Pause();
}

void ReplayMIDI()
{
	s_BGM.Resume(0);
}

void PlayWave ( int no )
{
	switch ( s_iYGSSoundType[no] )
	{
	case YGS_SOUNDTYPE_SOUND:
		LunaSound::Stop(s_pYGSSound[no]);
		LunaSound::Play(s_pYGSSound[no]);
		LunaSound::SetVolume(s_pYGSSound[no], (float)s_iYGSSoundVolume[no]);
		break;

	case YGS_SOUNDTYPE_MUSIC:
		s_MP3.Stop();
		BASSMOD_MusicStop();

		LoadMIDI(s_szYGSExMusic[no]);
		PlayMIDI();
		s_BGM.SetVolume(0, s_iYGSSoundVolume[no]);
		break;

	case YGS_SOUNDTYPE_MP3:
		StopMIDI();
		BASSMOD_MusicStop();

		s_MP3.Load(s_szYGSExMusic[no]);
		s_MP3.Play(true);
		s_MP3.SetVolume(s_iYGSSoundVolume[no]);
		break;

	case YGS_SOUNDTYPE_MOD:
		s_MP3.Stop();
		StopMIDI();

		BASSMOD_MusicFree();
		BASSMOD_MusicLoad(false, s_szYGSExMusic[no], 0, 0, BASS_MUSIC_LOOP | BASS_MUSIC_RAMP | BASS_MUSIC_POSRESET);

		/* すべてのチャンネルのボリュームを変更 */
		{
			int		channel = 0, instruments;
			while ( BASSMOD_MusicSetVolume(channel, s_iYGSSoundVolume[no]) )
			{
				instruments = 0;
				while ( BASSMOD_MusicSetVolume(MAKELONG(instruments, channel), s_iYGSSoundVolume[no]) )
				{
					instruments ++;
				}
				channel ++;
			}
		}

		BASSMOD_MusicPlay();
		break;
	}
}

void ReplayWave ( int no )
{
	PlayWave(no);
}

void StopWave ( int no )
{
	switch ( s_iYGSSoundType[no] )
	{
	case YGS_SOUNDTYPE_SOUND:
		LunaSound::Stop(s_pYGSSound[no]);
		break;

	case YGS_SOUNDTYPE_MUSIC:
		StopMIDI();
		break;

	case YGS_SOUNDTYPE_MP3:
		s_MP3.Stop();

	case YGS_SOUNDTYPE_MOD:
		BASSMOD_MusicStop();
		BASSMOD_MusicFree();
		break;
	}
}

void PauseWave ( int no )
{
	switch ( s_iYGSSoundType[no] )
	{
	case YGS_SOUNDTYPE_SOUND:
		LunaSound::Pause(s_pYGSSound[no]);
		break;

	case YGS_SOUNDTYPE_MUSIC:
		PauseMIDI();
		break;

	case YGS_SOUNDTYPE_MP3:
		s_MP3.Stop();
		break;

	case YGS_SOUNDTYPE_MOD:
		BASSMOD_MusicPause();
		break;
	}
}

void SetVolumeWave( int no, int vol )
{
	float	volume = float(vol + 10000) / 100;

	switch ( s_iYGSSoundType[no] )
	{
	case YGS_SOUNDTYPE_SOUND:
		s_iYGSSoundVolume[no] = (int)volume;		/* 小数切り捨てでいいや */
		LunaSound::SetVolume(s_pYGSSound[no], volume);
		break;

	case YGS_SOUNDTYPE_MUSIC:
		s_iYGSSoundVolume[no] = (int)volume;
		s_BGM.SetVolume(0, (int)volume);
		break;

	case YGS_SOUNDTYPE_MP3:
		s_iYGSSoundVolume[no] = vol;
		s_MP3.SetVolume(vol);
		break;

	case YGS_SOUNDTYPE_MOD:
		s_iYGSSoundVolume[no] = (int)volume;

		int		channel = 0, instruments;
		while ( BASSMOD_MusicSetVolume(channel, volume) )
		{
			instruments = 0;
			while ( BASSMOD_MusicSetVolume(MAKELONG(instruments, channel), volume) )
			{
				instruments ++;
			}
			channel ++;
		}
		break;
	}
}

int IsPlayWave( int no )
{
	switch ( s_iYGSSoundType[no] )
	{
	case YGS_SOUNDTYPE_SOUND:
		return LunaSound::IsPlay(s_pYGSSound[no]) ? 1 : 0;
		break;

	case YGS_SOUNDTYPE_MUSIC:
		return IsPlayMIDI();
		break;

	case YGS_SOUNDTYPE_MP3:
		return s_MP3.IsPlay() ? 1 : 0;
		break;

	case YGS_SOUNDTYPE_MOD:
		return BASSMOD_MusicIsActive() == BASS_ACTIVE_PLAYING ? 1 : 0;
		break;
	}

	return 0;
}

void LoadWave( char* filename, int no )
{
	int		len = strlen(filename);
	if ( len < 4 ) { return; }

	if ( s_pYGSSound[no] != INVALID_SOUND )
	{
		LunaSound::Release(s_pYGSSound[no]);
		s_pYGSSound[no] = INVALID_SOUND;
	}

	s_iYGSSoundType[no] = YGS_SOUNDTYPE_NONE;

	// midファイルはファイル名を覚えておく
	if ( !strcmpi(&filename[len - 4], ".mid") )
	{
		// ファイルの存在チェック
		if ( GetFileAttributes(filename) != 0xffffffff )
		{
			strcpy(s_szYGSExMusic[no], filename);
			s_iYGSSoundType[no] = YGS_SOUNDTYPE_MUSIC;
			s_iYGSSoundVolume[no] = 100;
		}
	}
	// modファイルもファイル名を覚えておく
	if ( !strcmpi(&filename[len - 4], ".mod") || !strcmpi(&filename[len - 3], ".it") || !strcmpi(&filename[len - 3], ".xm") )
	{
		// ファイルの存在チェック
		if ( GetFileAttributes(filename) != 0xffffffff )
		{
			strcpy(s_szYGSExMusic[no], filename);
			s_iYGSSoundType[no] = YGS_SOUNDTYPE_MOD;
			s_iYGSSoundVolume[no] = 100;
		}
	}
	// mp3ファイル、もしくはBGM扱い(no.50以上のwav)もファイル名を覚えておく
	else if ( !strcmpi(&filename[len - 4], ".mp3") || (no >= 50 && !strcmpi(&filename[len - 4], ".wav")) )
	{
		// ファイルの存在チェック
		if ( GetFileAttributes(filename) != 0xffffffff )
		{
			strcpy(s_szYGSExMusic[no], filename);
			s_iYGSSoundType[no] = YGS_SOUNDTYPE_MP3;
			s_iYGSSoundVolume[no] = 0;
		}
	}
	// wavとogg
	else
	{
		s_pYGSSound[no] = LunaSound::CreateFromFile(filename, false);

		if ( s_pYGSSound[no] != INVALID_SOUND )
		{
			s_iYGSSoundType[no] = YGS_SOUNDTYPE_SOUND;
			s_iYGSSoundVolume[no] = 100;
		}
	}
}

void SetLoopModeWave( int no, int mode )
{

}

void LoadMIDI( char* filename )
{
	s_BGM.LoadFile(0, filename);
	// s_BGM.Init(0);
}

void LoadBitmap( char* filename, int plane, int val )
{
	if ( s_pYGSTexture[plane] != INVALID_TEXTURE )
	{
		LunaTexture::Release(s_pYGSTexture[plane]);
		s_pYGSTexture[plane] = INVALID_TEXTURE;
	}

	s_pYGSTexture[plane] = LunaTexture::CreateFromFile(filename, s_iSurfaceFormat, COLORKEY_DISABLE);
}

void PlayMIDI()
{
	s_BGM.Play(0);
}

void StopMIDI()
{
	s_BGM.Stop();
}

void SetColorKeyPos(int plane, int x, int y)
{

}

void EnableBlendColorKey(int plane, int key)
{

}

void CreateSurface(int surf, int w, int h)
{

}

void ClearSecondary()
{

}

void SetFillColor(int col)
{

}

void LoadFile( char* filename, void* buf, int size )
{
	FILE	*file;
	file = fopen(filename, "rb");
	if ( file )
	{
		fread(buf, 1, size, file);
		fclose(file);
	}
}

void SaveFile( char* filename, void* buf, int size )
{
	FILE	*file;
	file = fopen(filename, "wb");
	if ( file )
	{
		fwrite(buf, 1, size, file);
		fclose(file);
	}
}

void TextLayerOn ( int layer, int x, int y )
{
	s_TextLayer[layer].enable = true;
	s_TextLayer[layer].x = x;
	s_TextLayer[layer].y = y;
}

void TextMove ( int layer, int x, int y )
{
	s_TextLayer[layer].x = x;
	s_TextLayer[layer].y = y;
}

void TextColor ( int layer, int r, int g, int b )
{
	s_TextLayer[layer].r = r;
	s_TextLayer[layer].g = g;
	s_TextLayer[layer].b = b;
}

void TextBackColorDisable ( int layer )
{

}

void TextSize ( int layer, int size )
{
	s_TextLayer[layer].size = size;
}

void TextHeight ( int layer, int height )
{

}

void TextOut ( int layer, char* text )
{
	strcpy(s_TextLayer[layer].string, text);
}

void TextBlt ( int layer )
{
	YGS2kPrimitiveFlush();

	if ( s_TextLayer[layer].enable )
	{
		YGS2kTextOut(s_TextLayer[layer].x, s_TextLayer[layer].y, s_TextLayer[layer].string, s_TextLayer[layer].r, s_TextLayer[layer].g, s_TextLayer[layer].b, s_TextLayer[layer].size);
	}
}

void TextLayerOff ( int layer )
{
	s_TextLayer[layer].enable = false;
}

void Blt(int pno, int dx, int dy)
{
	LUNARECT Dst, Src;
	Dst.Set( dx, dy, LunaTexture::GetOriginalWidth(s_pYGSTexture[pno]), LunaTexture::GetOriginalHeight(s_pYGSTexture[pno]) );
	Src.Set(  0,  0, LunaTexture::GetOriginalWidth(s_pYGSTexture[pno]), LunaTexture::GetOriginalHeight(s_pYGSTexture[pno]) );
	DrawPrim( s_pYGSTexture[pno], &Dst, &Src, COLOR_BASE );
}

void BltRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy)
{
	LUNARECT Dst, Src;
	Dst.Set( dx, dy, hx, hy );
	Src.Set( sx, sy, hx, hy );
	DrawPrim( s_pYGSTexture[pno], &Dst, &Src, COLOR_BASE );
}

void BltFast(int pno, int dx, int dy)
{
	Blt(pno, dx, dy);
}

void BltFastRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy)
{
	BltRect(pno, dx, dy, sx, sy, hx, hy);
}


void BlendBlt(int pno, int dx, int dy, int ar, int ag, int ab, int br, int bg, int bb)
{
	LUNARECT Dst, Src;
	Dst.Set( dx, dy, LunaTexture::GetOriginalWidth(s_pYGSTexture[pno]), LunaTexture::GetOriginalHeight(s_pYGSTexture[pno]) );
	Src.Set(  0,  0, LunaTexture::GetOriginalWidth(s_pYGSTexture[pno]), LunaTexture::GetOriginalHeight(s_pYGSTexture[pno]) );
	DrawPrim( s_pYGSTexture[pno], &Dst, &Src, D3DCOLOR_ARGB(ar, 255, 255, 255) );
}

void BlendBltRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int ar, int ag, int ab, int br, int bg, int bb)
{
	LUNARECT Dst, Src;
	Dst.Set( dx, dy, hx, hy );
	Src.Set( sx, sy, hx, hy );
	DrawPrim( s_pYGSTexture[pno], &Dst, &Src, D3DCOLOR_ARGB(ar, 255, 255, 255) );
}

void BltR(int pno, int dx, int dy, int scx, int scy)
{
	LUNARECT Dst, Src;
	Dst.Set( dx, dy, (float)LunaTexture::GetOriginalWidth(s_pYGSTexture[pno]) * ((float)scx / 65536.0f), (float)LunaTexture::GetOriginalHeight(s_pYGSTexture[pno]) * ((float)scy / 65536.0f) );
	Src.Set(  0,  0, (float)LunaTexture::GetOriginalWidth(s_pYGSTexture[pno]), (float)LunaTexture::GetOriginalHeight(s_pYGSTexture[pno]) );
	DrawPrim( s_pYGSTexture[pno], &Dst, &Src, COLOR_BASE );
}

void BltRectR(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int scx, int scy)
{
	LUNARECT Dst, Src;
	Dst.Set( dx, dy, (float)hx * ((float)scx / 65536.0f), (float)hy * ((float)scy / 65536.0f) );
	Src.Set( sx, sy, hx, hy );
	DrawPrim( s_pYGSTexture[pno], &Dst, &Src, COLOR_BASE );
}

void BltFastR(int pno, int dx, int dy, int scx, int scy)
{
	BltR(pno, dx, dy, scx, scy);
}

void BltFastRectR(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int scx, int scy)
{
	BltRectR(pno, dx, dy, sx, sy, hx, hy, scx, scy);
}

void BltTrans(int pno, int dx, int dy)
{
	LUNARECT Dst, Src;
	Dst.Set( dx, dy, LunaTexture::GetOriginalWidth(s_pYGSTexture[pno]), LunaTexture::GetOriginalHeight(s_pYGSTexture[pno]) );
	Src.Set(  0,  0, LunaTexture::GetOriginalWidth(s_pYGSTexture[pno]), LunaTexture::GetOriginalHeight(s_pYGSTexture[pno]) );
	DrawPrim( s_pYGSTexture[pno], &Dst, &Src, D3DCOLOR_ARGB(128, 255, 255, 255) );
}

void BlendBltR(int pno, int dx, int dy, int ar, int ag, int ab, int br, int bg, int bb, int scx, int scy)
{
	LUNARECT Dst, Src;
	Dst.Set( dx, dy, (float)LunaTexture::GetOriginalWidth(s_pYGSTexture[pno]) * ((float)scx / 65536.0f), (float)LunaTexture::GetOriginalHeight(s_pYGSTexture[pno]) * ((float)scy / 65536.0f) );
	Src.Set(  0,  0, (float)LunaTexture::GetOriginalWidth(s_pYGSTexture[pno]), (float)LunaTexture::GetOriginalHeight(s_pYGSTexture[pno]) );
	DrawPrim( s_pYGSTexture[pno], &Dst, &Src, D3DCOLOR_ARGB(ar, 255, 255, 255) );
}

void BlendBltRectR(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int ar, int ag, int ab, int br, int bg, int bb, int scx, int scy)
{
	LUNARECT Dst, Src;
	Dst.Set( dx, dy, (float)hx * ((float)scx / 65536.0f), (float)hy * ((float)scy / 65536.0f) );
	Src.Set( sx, sy, hx, hy );
	DrawPrim( s_pYGSTexture[pno], &Dst, &Src, D3DCOLOR_ARGB(ar, 255, 255, 255) );
}

void YGS2kExtraBlt(int pno, int dx, int dy, int rotate, int scx, int scy, int rotcx, int rotcy)
{
	YGS2kPrimitiveFlush();
	// Luna3D::SetBilinearEnable( true );

	LUNARECT Dst, Src;
	Dst.Set( dx, dy, (float)LunaTexture::GetOriginalWidth(s_pYGSTexture[pno]) * ((float)scx / 65536.0f), (float)LunaTexture::GetOriginalHeight(s_pYGSTexture[pno]) * ((float)scy / 65536.0f) );
	Src.Set(  0,  0, (float)LunaTexture::GetOriginalWidth(s_pYGSTexture[pno]), (float)LunaTexture::GetOriginalHeight(s_pYGSTexture[pno]) );
	Dst.Px -= Dst.Sx / 2 + rotcx;
	Dst.Py -= Dst.Sy / 2 + rotcy;
	DrawPrimRotate( s_pYGSTexture[pno], &Dst, &Src, m_iPrimColor, rotate, rotcx, rotcy );

	YGS2kPrimitiveFlush();
	// Luna3D::SetBilinearEnable( false );
}

void YGS2kExtraBltRect(int pno, int dx, int dy, int sx, int sy, int hx, int hy, int rotate, int scx, int scy, int rotcx, int rotcy)
{
	LUNARECT Dst, Src;
	Dst.Set( dx, dy, (float)hx * ((float)scx / 65536.0f), (float)hy * ((float)scy / 65536.0f) );
	Src.Set( sx, sy, hx, hy );
	Dst.Px -= Dst.Sx / 2 + rotcx;
	Dst.Py -= Dst.Sy / 2 + rotcy;
	DrawPrimRotate( s_pYGSTexture[pno], &Dst, &Src, m_iPrimColor, rotate, rotcx, rotcy );
}

void SetSecondaryOffset(int x, int y)
{
	s_iNewOffsetX = x;
	s_iNewOffsetY = y;
}

void SetColorKeyRGB(int pno, int r, int g, int b)
{

}

void SwapToSecondary(int pno)
{

}

void SetFPS(int fps)
{
	Luna::SetFrameRate( fps );
}

int GetFPS()
{
	return Luna::GetFrameRate();
}

int GetRealFPS()
{
	return Luna::GetFPS();
}

void StrCpy(char *dest, char *src)
{
	strcpy(dest, src);
}

void StrCpy(void *dest, char *src)
{
	strcpy((char*)dest, src);
}

void StrCpy(char *dest, void *src)
{
	strcpy(dest, (char*)src);
}

void StrCat(char *str1, char *str2)
{
	strcat(str1, str2);
}

int StrLen(char *stri)
{
	return strlen(stri);
}

void MidStr(char *src, int start, int len, char *dest)
{
	int		i;
	for ( i = 0 ; i < len ; i ++ )
	{
		dest[i] = src[start - 1 + i];
	}
	dest[len] = '\0';
}

void LeftStr(char *src, int len, char *dest)
{
	MidStr(src, 1, len, dest);
}

char CharAt(char *stri, int pos)
{
	return stri[pos];
}

int ValLong(char *stri)
{
	return atoi(stri);
}
