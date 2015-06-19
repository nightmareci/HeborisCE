
/**
    @brief		３Ｄ統括クラス
    @author		葉迩倭
*/

//=============================================================================
// INCLUDE
//=============================================================================
#include "LunaBase.h"

#include "Luna3D.h"
#include "LunaMain.h"
#include "LunaCamera.h"
#include "LunaLight.h"
#include "LunaField.h"
#include "LunaDebug.h"
#include "LunaLine.h"
#include "LunaLine3D.h"
#include "LunaPoint.h"
#include "LunaPoint3D.h"
#include "LunaPrimitive.h"
#include "LunaPrimitive3D.h"
#include "LunaSprite.h"
#include "LunaSprite3D.h"
#include "LunaSurface.h"
#include "LunaTexture.h"
#include "LunaVMR9.h"
#include "LunaModel.h"
#include "LunaModelEx.h"
#include "LunaShaderEffect.h"
#include "LunaSimple.h"


using namespace LibLuna;

//=============================================================================
// CONSTANT
//=============================================================================

//---------------------------------------------------------------
// バックバッファフォーマット
//---------------------------------------------------------------
static const D3DFORMAT BackBufferFormats[] = {
	D3DFMT_X8R8G8B8,
	D3DFMT_X1R5G5B5,
	D3DFMT_R5G6B5,
	D3DFMT_UNKNOWN,
};


//---------------------------------------------------------------
// テクスチャフォーマット
//---------------------------------------------------------------
static D3DFORMAT TextureFormats[] = {
	D3DFMT_R5G6B5,			D3DFMT_X1R5G5B5,		D3DFMT_A1R5G5B5,
	D3DFMT_A4R4G4B4,		D3DFMT_R8G8B8,			D3DFMT_A8R8G8B8,
	D3DFMT_X8R8G8B8,		D3DFMT_R3G3B2,			D3DFMT_A8,
	D3DFMT_A8R3G3B2,		D3DFMT_X4R4G4B4,		D3DFMT_A2B10G10R10,
	D3DFMT_A2R10G10B10,		D3DFMT_A16B16G16R16,	D3DFMT_A8P8,
	D3DFMT_P8,				D3DFMT_L8,				D3DFMT_L16,
	D3DFMT_A8L8,			D3DFMT_A4L4,			D3DFMT_V8U8,
	D3DFMT_Q8W8V8U8,		D3DFMT_V16U16,			D3DFMT_Q16W16V16U16,
	D3DFMT_CxV8U8,			D3DFMT_L6V5U5,			D3DFMT_X8L8V8U8,
	D3DFMT_A2W10V10U10,		D3DFMT_G8R8_G8B8,		D3DFMT_R8G8_B8G8,
	D3DFMT_DXT5,			D3DFMT_DXT4,			D3DFMT_DXT3,
	D3DFMT_DXT2,			D3DFMT_DXT1,			D3DFMT_UYVY,
	D3DFMT_YUY2,			D3DFMT_D16_LOCKABLE,	D3DFMT_D32,
	D3DFMT_D15S1,			D3DFMT_D24S8,			D3DFMT_D24X8,
	D3DFMT_D24X4S4,			D3DFMT_D32F_LOCKABLE,	D3DFMT_D24FS8,
	D3DFMT_D16,				D3DFMT_VERTEXDATA,		D3DFMT_INDEX16,
	D3DFMT_INDEX32,			D3DFMT_R16F,			D3DFMT_G16R16F,
	D3DFMT_A16B16G16R16F,	D3DFMT_R32F,			D3DFMT_G32R32F,
	D3DFMT_A32B32G32R32F,	D3DFMT_G16R16,			D3DFMT_UNKNOWN,
};


//---------------------------------------------------------------
// 深度バッファフォーマット
//---------------------------------------------------------------
static D3DFORMAT DepthFormats[] = {
	D3DFMT_D16,				D3DFMT_D32,				D3DFMT_D24S8,
	D3DFMT_D24X8,			D3DFMT_D24X4S4,			D3DFMT_D24FS8,
	D3DFMT_D15S1,			D3DFMT_UNKNOWN,
};


//---------------------------------------------------------------
// デバイスタイプ
//---------------------------------------------------------------
static D3DDEVTYPE DeviceType[3] = {
	D3DDEVTYPE_HAL,													// 0
	D3DDEVTYPE_HAL,													// 1
	D3DDEVTYPE_REF,													// 2
};


//---------------------------------------------------------------
// 頂点処理
//---------------------------------------------------------------
static Uint32 CreateFlag[3] = {
	D3DCREATE_MIXED_VERTEXPROCESSING,								// 0
	D3DCREATE_SOFTWARE_VERTEXPROCESSING,							// 1
	D3DCREATE_SOFTWARE_VERTEXPROCESSING,							// 2
};

//---------------------------------------------------------------
// レンダーステート一覧
//---------------------------------------------------------------
static D3DRENDERSTATETYPE RenderStateTbl[] =
{
    D3DRS_ZENABLE,
    D3DRS_FILLMODE,
    D3DRS_SHADEMODE,
    D3DRS_ZWRITEENABLE,
    D3DRS_ALPHATESTENABLE,
    D3DRS_LASTPIXEL,
    D3DRS_SRCBLEND,
    D3DRS_DESTBLEND,
    D3DRS_CULLMODE,
    D3DRS_ZFUNC,
    D3DRS_ALPHAREF,
    D3DRS_ALPHAFUNC,
    D3DRS_DITHERENABLE,
    D3DRS_ALPHABLENDENABLE,
    D3DRS_FOGENABLE,
    D3DRS_SPECULARENABLE,
    D3DRS_FOGCOLOR,
    D3DRS_FOGTABLEMODE,
    D3DRS_FOGSTART,
    D3DRS_FOGEND,
    D3DRS_FOGDENSITY,
    D3DRS_RANGEFOGENABLE,
    D3DRS_STENCILENABLE,
    D3DRS_STENCILFAIL,
    D3DRS_STENCILZFAIL,
    D3DRS_STENCILPASS,
    D3DRS_STENCILFUNC,
    D3DRS_STENCILREF,
    D3DRS_STENCILMASK,
    D3DRS_STENCILWRITEMASK,
    D3DRS_TEXTUREFACTOR,
    D3DRS_WRAP0,
    D3DRS_WRAP1,
    D3DRS_WRAP2,
    D3DRS_WRAP3,
    D3DRS_WRAP4,
    D3DRS_WRAP5,
    D3DRS_WRAP6,
    D3DRS_WRAP7,
    D3DRS_CLIPPING,
    D3DRS_LIGHTING,
    D3DRS_AMBIENT,
    D3DRS_FOGVERTEXMODE,
    D3DRS_COLORVERTEX,
    D3DRS_LOCALVIEWER,
    D3DRS_NORMALIZENORMALS,
    D3DRS_DIFFUSEMATERIALSOURCE,
    D3DRS_SPECULARMATERIALSOURCE,
    D3DRS_AMBIENTMATERIALSOURCE,
    D3DRS_EMISSIVEMATERIALSOURCE,
    D3DRS_VERTEXBLEND,
    D3DRS_CLIPPLANEENABLE,
    D3DRS_POINTSIZE,
    D3DRS_POINTSIZE_MIN,
    D3DRS_POINTSPRITEENABLE,
    D3DRS_POINTSCALEENABLE,
    D3DRS_POINTSCALE_A,
    D3DRS_POINTSCALE_B,
    D3DRS_POINTSCALE_C,
    D3DRS_MULTISAMPLEANTIALIAS,
    D3DRS_MULTISAMPLEMASK,
    D3DRS_PATCHEDGESTYLE,
    D3DRS_DEBUGMONITORTOKEN,
    D3DRS_POINTSIZE_MAX,
    D3DRS_INDEXEDVERTEXBLENDENABLE,
    D3DRS_COLORWRITEENABLE,
    D3DRS_TWEENFACTOR,
    D3DRS_BLENDOP,
    D3DRS_POSITIONDEGREE,
    D3DRS_NORMALDEGREE,
    D3DRS_SCISSORTESTENABLE,
    D3DRS_SLOPESCALEDEPTHBIAS,
    D3DRS_ANTIALIASEDLINEENABLE,
    D3DRS_MINTESSELLATIONLEVEL,
    D3DRS_MAXTESSELLATIONLEVEL,
    D3DRS_ADAPTIVETESS_X,
    D3DRS_ADAPTIVETESS_Y,
    D3DRS_ADAPTIVETESS_Z,
    D3DRS_ADAPTIVETESS_W,
    D3DRS_ENABLEADAPTIVETESSELLATION,
    D3DRS_TWOSIDEDSTENCILMODE,
    D3DRS_CCW_STENCILFAIL,
    D3DRS_CCW_STENCILZFAIL,
    D3DRS_CCW_STENCILPASS,
    D3DRS_CCW_STENCILFUNC,
    D3DRS_COLORWRITEENABLE1,
    D3DRS_COLORWRITEENABLE2,
    D3DRS_COLORWRITEENABLE3,
    D3DRS_BLENDFACTOR,
    D3DRS_SRGBWRITEENABLE,
    D3DRS_DEPTHBIAS,
    D3DRS_WRAP8,
    D3DRS_WRAP9,
    D3DRS_WRAP10,
    D3DRS_WRAP11,
    D3DRS_WRAP12,
    D3DRS_WRAP13,
    D3DRS_WRAP14,
    D3DRS_WRAP15,
    D3DRS_SEPARATEALPHABLENDENABLE,
    D3DRS_SRCBLENDALPHA,
    D3DRS_DESTBLENDALPHA,
    D3DRS_BLENDOPALPHA,
};

static Uint32 RenderStateTblCount = sizeof(RenderStateTbl) / sizeof(D3DRENDERSTATETYPE);


//---------------------------------------------------------------
// テクスチャステート一覧
//---------------------------------------------------------------
static D3DTEXTURESTAGESTATETYPE TextureStateTbl[] =
{
    D3DTSS_COLOROP,
    D3DTSS_COLORARG1,
    D3DTSS_COLORARG2,
    D3DTSS_ALPHAOP,
    D3DTSS_ALPHAARG1,
    D3DTSS_ALPHAARG2,
    D3DTSS_BUMPENVMAT00,
    D3DTSS_BUMPENVMAT01,
    D3DTSS_BUMPENVMAT10,
    D3DTSS_BUMPENVMAT11,
    D3DTSS_TEXCOORDINDEX,
    D3DTSS_BUMPENVLSCALE,
    D3DTSS_BUMPENVLOFFSET,
    D3DTSS_TEXTURETRANSFORMFLAGS,
    D3DTSS_COLORARG0,
    D3DTSS_ALPHAARG0,
    D3DTSS_RESULTARG,
    D3DTSS_CONSTANT,
};

static Uint32 TextureStateTblCount = sizeof(TextureStateTbl) / sizeof(D3DTEXTURESTAGESTATETYPE);


//---------------------------------------------------------------
// サンプラーステート一覧
//---------------------------------------------------------------
static D3DSAMPLERSTATETYPE SamplerStateTbl[] =
{
    D3DSAMP_ADDRESSU,
    D3DSAMP_ADDRESSV,
    D3DSAMP_ADDRESSW,
    D3DSAMP_BORDERCOLOR,
    D3DSAMP_MAGFILTER,
    D3DSAMP_MINFILTER,
    D3DSAMP_MIPFILTER,
    D3DSAMP_MIPMAPLODBIAS,
    D3DSAMP_MAXMIPLEVEL,
    D3DSAMP_MAXANISOTROPY,
    D3DSAMP_SRGBTEXTURE,
    D3DSAMP_ELEMENTINDEX,
    D3DSAMP_DMAPOFFSET,
};

static Uint32 SamplerStateTblCount = sizeof(SamplerStateTbl) / sizeof(D3DSAMPLERSTATETYPE);


//=============================================================================
// VARIABLE
//=============================================================================
Sint32					Luna3D::m_SceneCount;
HINSTANCE				Luna3D::m_hDllD3D9;
IDirect3D9				*Luna3D::m_pDirect3D;
IDirect3DDevice9		*Luna3D::m_pDevice;
IDirect3DSurface9		*Luna3D::m_pRenderTarget;
IDirect3DQuery9			*Luna3D::m_pQuery[2];
D3DVIEWPORT9			Luna3D::m_Viewport;
char					Luna3D::m_AdapterName[128];
char					Luna3D::m_AdapterDriver[128];
char					Luna3D::m_AdapterVersion[128];
D3DMULTISAMPLE_TYPE		Luna3D::m_MultiSampleType;
Uint32					Luna3D::m_MultiSampleQuality;
DEVICEINFORMATION		Luna3D::m_DeviceInfo;
D3DDISPLAYMODE			Luna3D::m_WindowMode;
D3DDISPLAYMODE			Luna3D::m_FullScreenMode[2];
ADAPTERDATA				Luna3D::m_AdapterData;
D3DPRESENT_PARAMETERS	Luna3D::m_PresentParams[2];
LCAMERA					Luna3D::m_lCamera;
LTEXTURE				Luna3D::m_lTexTarget;
LSURFACE				Luna3D::m_lSurfTarget;
LSURFACE				Luna3D::m_lSurfDepthStencil;
LTEXTURE				Luna3D::m_lTexActive[TEXTURE_STAGE_MAX];
Uint32					Luna3D::m_RenderState[RENDER_STATE_TBL_MAX];
Uint32					Luna3D::m_SamplerState[TEXTURE_STAGE_MAX][SAMPLER_STATE_TBL_MAX];
Uint32					Luna3D::m_TextureState[TEXTURE_STAGE_MAX][TEXTURE_STATE_TBL_MAX];
Uint32					Luna3D::m_MaxVertexShaderVersion;
HFONT					Luna3D::m_hFont;
HFONT					Luna3D::m_hOldFont;
HDC						Luna3D::m_hDC;
Float					Luna3D::m_FogNear;
Float					Luna3D::m_FogFar;
D3DCOLOR				Luna3D::m_FogColor;
Bool					Luna3D::m_IsWaitBlank;
Bool					Luna3D::m_IsFullColor;
Bool					Luna3D::m_IsEmulationMode;
Bool					Luna3D::m_IsLocked;
Bool					Luna3D::m_IsCheckingFillRate;
Bool					Luna3D::m_IsNVPerfHUD;
void					(*Luna3D::m_pCallbackReset)( void );
void					(*Luna3D::m_pCallbackRestore)( void );
MATERIALSTATE			Luna3D::m_DefaultState;

eSurfaceFormat Luna3D::m_SurfaceFormatTbl[2][16] = {
	{ FORMAT_TEXTURE32,	FORMAT_TEXTURE16,	FORMAT_DXT5,		FORMAT_DXT3 },
	{ FORMAT_DXT5,		FORMAT_DXT3,		FORMAT_TEXTURE32,	FORMAT_TEXTURE16 },
};
Uint32 Luna3D::m_SurfaceFormatCount[2] = { 4, 4 };


//=============================================================================
/**
	３Ｄクラスの初期化をします。

	@retval true		成功
	@retval false		失敗
*/
//=============================================================================
Bool Luna3D::Initialize( void )
{
	Luna::LogoutMainCategory( "Luna3Dクラスの初期化" );
	Luna::LogoutNextLine();

	//----------------------------------------------------------
	// デフォルト値
	//----------------------------------------------------------
	m_MaxVertexShaderVersion = D3DVS_VERSION(2,0);

	//----------------------------------------------------------
	// アダプターの初期化
	//----------------------------------------------------------
	if ( !Luna3D::InitAdapter() )
	{
		Luna::MsgBox( MB_ICONERROR, "Luna3D", "アダプターの初期化に失敗" );
		return false;
	}

	return true;
}

//=============================================================================
/**
	３Ｄクラスの解放をします。

*/
//=============================================================================
void Luna3D::Finalize( void )
{
	Luna::LogoutMainCategory( "Luna3Dクラスの解放" );
	Luna::LogoutNextLine();

	for ( Sint32 i = 0; i < 2; i++ )
	{
		if ( m_pQuery[i] != NULL )
		{
			m_pQuery[i]->Release();
			m_pQuery[i] = NULL;
			Luna::Logout( "IDirect3DQuery9インターフェースを解放" );
			Luna::LogoutNextLine();
		}
	}

	SAFE_RELEASE( m_pRenderTarget );

	if ( m_pDevice != NULL )
	{
		m_pDevice->Release();
		m_pDevice = NULL;
		Luna::Logout( "Direct3Dデバイスを解放" );
		Luna::LogoutNextLine();
	}

	if ( m_pDirect3D != NULL )
	{
		m_pDirect3D->Release();
		m_pDirect3D = NULL;
		Luna::Logout( "Direct3Dオブジェクトを解放" );
		Luna::LogoutNextLine();
	}

	if ( m_hDllD3D9 != NULL )
	{
		::FreeLibrary( m_hDllD3D9 );
		m_hDllD3D9 = NULL;
		Luna::Logout( "DLLを解放" );
		Luna::LogoutNextLine();
	}

	Luna::LogoutNextLine();
}

//=============================================================================
/**
	アダプターの初期化をします。

	@retval true		成功
	@retval false		失敗
*/
//=============================================================================
Bool Luna3D::InitAdapter( void )
{
	//---------------------------------------------------------------
	// Direct3Dオブジェクトの生成
	//---------------------------------------------------------------
	if ( !Luna3D::CreateDirect3DObject() )
	{
		return false;
	}

	//---------------------------------------------------------------
	// アダプター情報のチェック
	//---------------------------------------------------------------
	if ( !Luna3D::CheckAdapterInformation() )
	{
		return false;
	}

	//---------------------------------------------------------------
	// デバイス情報のチェック
	//---------------------------------------------------------------
	if ( !Luna3D::CheckDeviceInformation() )
	{
		return false;
	}

	return true;
}

//=============================================================================
/**
	Direct3Dオブジェクトの生成をします。

	@retval true		成功
	@retval false		失敗
*/
//=============================================================================
Bool Luna3D::CreateDirect3DObject( void )
{
	PDIRECT3DCREATE9 pProcDirect3DCreate9 = NULL;

	//---------------------------------------------------------------
	// Load DLL
	//---------------------------------------------------------------
	m_hDllD3D9 = ::LoadLibrary( "d3d9.dll" );
	if ( m_hDllD3D9 == NULL )
	{
		Luna::LogoutError( "　　　エラー : DLLの読み込みに失敗" );
		Luna::LogoutNextLine();
		Luna::LogoutError( "　　　DirectX9cが正しくインストールされているか確認して下さい" );
		Luna::LogoutNextLine();
		Luna::MsgBox( MB_ICONERROR, "Luna3D", "DLLの読み込みに失敗\nDirectX9cが正しくインストールされているか\n確認して下さい" );
		return false;
	}

	//---------------------------------------------------------------
	// Load Direct3DCreate from DLL
	//---------------------------------------------------------------
	pProcDirect3DCreate9 = (PDIRECT3DCREATE9)::GetProcAddress( m_hDllD3D9, "Direct3DCreate9" );
	if ( pProcDirect3DCreate9 == NULL )
	{
		Luna::LogoutError( "　　　エラー : DLLからの関数呼び出しに失敗" );
		Luna::LogoutNextLine();
		Luna::LogoutError( "　　　DirectX9cが正しくインストールされているか確認して下さい" );
		Luna::LogoutNextLine();
		Luna::MsgBox( MB_ICONERROR, "Luna3D", "DLLからの関数呼び出しに失敗\nDirectX9cが正しくインストールされているか\n確認して下さい" );
		return false;
	}

	//---------------------------------------------------------------
	// Create Direct3D Object
	//---------------------------------------------------------------
	m_pDirect3D = pProcDirect3DCreate9( D3D_SDK_VERSION );
	if ( m_pDirect3D == NULL )
	{
		Luna::LogoutError( "　　　エラー : Direct3Dオブジェクトの生成に失敗" );
		Luna::LogoutNextLine();
		Luna::LogoutError( "　　　DirectX9cが正しくインストールされているか確認して下さい" );
		Luna::LogoutNextLine();
		Luna::MsgBox( MB_ICONERROR, "Luna3D", "Direct3Dオブジェクトの生成に失敗\nDirectX9cが正しくインストールされているか\n確認して下さい" );
		return false;
	}

	return true;
}

//=============================================================================
/**
	アダプター情報の取得をします。

	@retval true		成功
	@retval false		失敗
*/
//=============================================================================
Bool Luna3D::CheckAdapterInformation( void )
{
	D3DADAPTER_IDENTIFIER9 Identifier;
	HRESULT hr;

	//---------------------------------------------------------------
	// アダプター情報
	//---------------------------------------------------------------
	Luna::LogoutKeyword( "アダプター情報" );
	Luna::LogoutNextLine();

	hr = m_pDirect3D->GetAdapterIdentifier( D3DADAPTER_DEFAULT, 0, &Identifier );
	if SUCCEEDED( hr )
	{
		strcpy( m_AdapterName, Identifier.Description );
		strcpy( m_AdapterDriver, Identifier.Driver );
		sprintf(
			m_AdapterVersion,
			"%u.%u.%u.%u",
			HIWORD(Identifier.DriverVersion.HighPart),
			LOWORD(Identifier.DriverVersion.HighPart),
			HIWORD(Identifier.DriverVersion.LowPart),
			LOWORD(Identifier.DriverVersion.LowPart) );

		Luna::Logout( "　　　Name [ %s ]", Identifier.Description );
		Luna::LogoutNextLine();
		Luna::Logout( "　　　Driver [ %s ]", Identifier.Driver );
		Luna::LogoutNextLine();
		Luna::Logout( "　　　Product [ %u ]", HIWORD(Identifier.DriverVersion.HighPart) );
		Luna::LogoutNextLine();
		Luna::Logout( "　　　Version [ %u.%u.%u ]",
			LOWORD(Identifier.DriverVersion.HighPart),
			HIWORD(Identifier.DriverVersion.LowPart),
			LOWORD(Identifier.DriverVersion.LowPart) );
		Luna::LogoutNextLine();

		Luna::Logout( "　Windows Hardware Quality Lab (WHQL) 認証レベル" );
		Luna::LogoutNextLine();

		if ( Identifier.WHQLLevel == 0 )
		{
			Luna::Logout( "　　　認証されていない。" );
			Luna::LogoutNextLine();
		}
		ef ( Identifier.WHQLLevel == 1 )
		{
			Luna::Logout( "　　　WHQL の認証は受けているが、利用できる日付情報はない。" );
			Luna::LogoutNextLine();
		}
		else
		{
			Luna::Logout( "　　　%u.%u.%u",
				((Identifier.WHQLLevel>>16)&0xFF), ((Identifier.WHQLLevel>>8)&0x0F), ((Identifier.WHQLLevel>>0)&0x0F) );
			Luna::LogoutNextLine();
		}
	}

	Luna::LogoutNextLine();

	//---------------------------------------------------------------
	// 現在の画面モード取得
	//---------------------------------------------------------------
	Luna::LogoutKeyword( "現在の画面情報" );
	Luna::LogoutNextLine();

	hr = m_pDirect3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &m_WindowMode );
	if SUCCEEDED( hr )
	{
		Luna::Logout( "　　　Width [ %u ]", m_WindowMode.Width );
		Luna::LogoutNextLine();

		Luna::Logout( "　　　Height [ %u ]", m_WindowMode.Height );
		Luna::LogoutNextLine();

		Luna::Logout( "　　　RefreshRate [ %u ]", m_WindowMode.RefreshRate );
		Luna::LogoutNextLine();

		Luna::Logout( "　　　Format [ %s ]", Luna3D::GetPixelFormat(m_WindowMode.Format) );
		Luna::LogoutNextLine();
	}

	Luna::LogoutNextLine();

	//---------------------------------------------------------------
	// フルスクリーン時に使用可能な画面モードを列挙する
	//---------------------------------------------------------------
	Luna::LogoutKeyword( "使用可能な画面モードを列挙" );
	Luna::LogoutNextLine();

	for ( Sint32 i = 0; BackBufferFormats[i] != D3DFMT_UNKNOWN; i++ )
	{
		// ディスプレイモード数の取得
		D3DDISPLAYMODE DisplayMode;
		Uint32 ModeCount = m_pDirect3D->GetAdapterModeCount( D3DADAPTER_DEFAULT, BackBufferFormats[i] );

		if ( ModeCount > 0 )
		{
			Luna::LogoutStrong( "　BackBuffer Format [ %s ]", GetPixelFormat(BackBufferFormats[i]) );
			Luna::LogoutNextLine();

			// モードを列挙
			for ( Uint32 j = 0; j < ModeCount; j++ )
			{
				hr = m_pDirect3D->EnumAdapterModes( D3DADAPTER_DEFAULT, BackBufferFormats[i], j, &DisplayMode );
				if SUCCEEDED( hr )
				{
					Bool IsNew = true;

					// 同一画面サイズの検索
					for ( Sint32 k = 0; k < m_AdapterData.ModeCount; k++ )
					{
						if ( m_AdapterData.Mode[k].Width == DisplayMode.Width )
						{
							if ( m_AdapterData.Mode[k].Height == DisplayMode.Height )
							{
								if ( m_AdapterData.Mode[k].Format == DisplayMode.Format )
								{
									IsNew = false;
								}
							}
						}
					}

					// 同一画面サイズが見つからなかった場合は新しく入れておく
					if ( IsNew )
					{
						m_AdapterData.Mode[m_AdapterData.ModeCount++] = DisplayMode;
						Luna::Logout( "　　　[ %u x %u ] [ %uHz ]", DisplayMode.Width, DisplayMode.Height, DisplayMode.RefreshRate );
						Luna::LogoutNextLine();
					}

				}
			}

			Luna::LogoutNextLine();
		}
	}

	Luna::LogoutNextLine();

	//---------------------------------------------------------------
	// フルスクリーンモード用のディスプレイモード設定
	//---------------------------------------------------------------
	Luna::LogoutKeyword( "フルスクリーン用ディスプレイモードの検索" );
	Luna::LogoutNextLine();

	Sint32 Width, Height;
	Luna::GetScreenData( &Width, &Height, NULL );

	// クリア
	m_FullScreenMode[0].Format = D3DFMT_UNKNOWN;
	m_FullScreenMode[1].Format = D3DFMT_UNKNOWN;

	// 検索
	for ( Sint32 j = 0; j < m_AdapterData.ModeCount; j++ )
	{
		if ( m_AdapterData.Mode[j].Width == (Uint32)Width )
		{
			if ( m_AdapterData.Mode[j].Height == (Uint32)Height )
			{
				switch ( m_AdapterData.Mode[j].Format )
				{
				// 16Bit
				case D3DFMT_X1R5G5B5:
				case D3DFMT_R5G6B5:
					m_FullScreenMode[0] = m_AdapterData.Mode[j];
					break;
				// 32Bit
				case D3DFMT_X8R8G8B8:
					m_FullScreenMode[1] = m_AdapterData.Mode[j];
					break;
				}
			}
		}
	}

	// ログ
	Luna::Logout( "　　　16BitColor [ %s ]", Luna3D::GetPixelFormat(m_FullScreenMode[0].Format) );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　32BitColor [ %s ]", Luna3D::GetPixelFormat(m_FullScreenMode[1].Format) );
	Luna::LogoutNextLine();

	Luna::LogoutNextLine();

	//---------------------------------------------------------------
	// テクスチャフォーマットから使用可能なものを検索
	//---------------------------------------------------------------
	Luna::LogoutKeyword( "使用可能なテクスチャフォーマットを列挙" );
	Luna::LogoutNextLine();

	D3DFORMAT BaseFormat = m_FullScreenMode[1].Format == D3DFMT_UNKNOWN ? m_WindowMode.Format : m_FullScreenMode[1].Format;

	for ( Sint32 i = 0; TextureFormats[i] != D3DFMT_UNKNOWN; i++ )
	{
		hr = m_pDirect3D->CheckDeviceFormat(
						D3DADAPTER_DEFAULT,
						D3DDEVTYPE_HAL,
						BaseFormat,
						D3DUSAGE_AUTOGENMIPMAP,
						D3DRTYPE_TEXTURE,
						TextureFormats[i] );
		if ( SUCCEEDED(hr) && (hr != D3DOK_NOAUTOGEN) )
		{
			m_AdapterData.TextureFormats[ m_AdapterData.TextureCount ] = TextureFormats[i];
			m_AdapterData.TextureMipmap[ m_AdapterData.TextureCount ] = true;
			m_AdapterData.TextureCount++;

			Luna::Logout( "　　　○ [ %s ]", Luna3D::GetPixelFormat(TextureFormats[i]) );
			Luna::LogoutNextLine();
		}
		else
		{
			hr = m_pDirect3D->CheckDeviceFormat(
							D3DADAPTER_DEFAULT,
							D3DDEVTYPE_HAL,
							BaseFormat,
							0,
							D3DRTYPE_TEXTURE,
							TextureFormats[i] );
			if SUCCEEDED( hr )
			{
				m_AdapterData.TextureFormats[ m_AdapterData.TextureCount ] = TextureFormats[i];
				m_AdapterData.TextureMipmap[ m_AdapterData.TextureCount ] = false;
				m_AdapterData.TextureCount++;

				Luna::Logout( "　　　× [ %s ]", Luna3D::GetPixelFormat(TextureFormats[i]) );
				Luna::LogoutNextLine();
			}
		}
	}

	Luna::LogoutNextLine();

	//---------------------------------------------------------------
	// レンダリングターゲットテクスチャフォーマットから使用可能なものを検索
	//---------------------------------------------------------------
	Luna::LogoutKeyword( "使用可能なレンダリングターゲットテクスチャフォーマットを列挙" );
	Luna::LogoutNextLine();

	for ( Sint32 i = 0; TextureFormats[i] != D3DFMT_UNKNOWN; i++ )
	{
		hr = m_pDirect3D->CheckDeviceFormat(
						D3DADAPTER_DEFAULT,
						D3DDEVTYPE_HAL,
						BaseFormat,
						D3DUSAGE_RENDERTARGET,
						D3DRTYPE_TEXTURE,
						TextureFormats[i] );
		if SUCCEEDED( hr )
		{
			m_AdapterData.TargetFormats[ m_AdapterData.TargetCount++ ] = TextureFormats[i];

			Luna::Logout( "　　　[ %s ]", Luna3D::GetPixelFormat(TextureFormats[i]) );
			Luna::LogoutNextLine();
		}
	}

	Luna::LogoutNextLine();

	//---------------------------------------------------------------
	// 深度バッファフォーマットから使用可能なものを検索
	//---------------------------------------------------------------
	Luna::LogoutKeyword( "使用可能な深度バッファフォーマットを列挙" );
	Luna::LogoutNextLine();

	for ( Sint32 i = 0; DepthFormats[i] != D3DFMT_UNKNOWN; i++ )
	{
		hr = m_pDirect3D->CheckDeviceFormat(
								D3DADAPTER_DEFAULT,
								D3DDEVTYPE_HAL,
								BaseFormat,
								D3DUSAGE_DEPTHSTENCIL,
								D3DRTYPE_SURFACE,
								DepthFormats[i] );
		if SUCCEEDED( hr )
		{
			m_AdapterData.DepthFormats[ m_AdapterData.DepthCount++ ] = DepthFormats[i];

			Luna::Logout( "　　　[ %s ]", Luna3D::GetPixelFormat(DepthFormats[i]) );
			Luna::LogoutNextLine();
		}
	}

	Luna::LogoutNextLine();

	return true;
}

//=============================================================================
/**
	３Ｄデバイスの性能チェックをします。

	@retval true		成功
	@retval false		失敗
*/
//=============================================================================
Bool Luna3D::CheckDeviceInformation( void )
{
	Luna::LogoutKeyword( "デバイス能力の取得" );
	Luna::LogoutNextLine();

	D3DCAPS9 Caps;

	//-----------------------------------------------------------
	// ハードウェアデバイスの性能を取得
	//-----------------------------------------------------------
	m_pDirect3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &Caps );
	GetDeviceCaps( &Caps, &m_DeviceInfo );

	//-----------------------------------------------------------
	// ハードウェアデバイスの性能を列挙
	//-----------------------------------------------------------
	Luna::LogoutStrong( "　デバイス能力" );
	Luna::LogoutNextLine();

	Luna::Logout( "　　　[ %s ] ピュアデバイス", m_DeviceInfo.IsPureDevice ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] ハードウェアＴｎＬ", m_DeviceInfo.IsHardwareTnL ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] ハードウェアラスタライズ", m_DeviceInfo.IsSoftwareTnL ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] 異方性フィルタリング", m_DeviceInfo.IsAnisotropy ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] ディザリング", m_DeviceInfo.IsDither ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] 深度バイアス", m_DeviceInfo.IsDepthBias ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] Ｗバッファ", m_DeviceInfo.IsWBuffer ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] ミップマップ自動生成", m_DeviceInfo.IsAutoMipMap ? "○" : "×" );
	Luna::LogoutNextLine();

	Luna::LogoutStrong( "　深度テスト能力" );
	Luna::LogoutNextLine();

	Luna::Logout( "　　　[ %s ] 深度テスト ALWAYS", m_DeviceInfo.IsZCmpAlways ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] 深度テスト EQUAL", m_DeviceInfo.IsZCmpEqual ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] 深度テスト GREATER", m_DeviceInfo.IsZCmpGreater ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] 深度テスト GREATER EQUA", m_DeviceInfo.IsZCmpGreaterEqual ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] 深度テスト LESS", m_DeviceInfo.IsZCmpLess ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] 深度テスト LESS EQUAL", m_DeviceInfo.IsZCmpLessEqual ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] 深度テスト NEVER", m_DeviceInfo.IsZCmpNever ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] 深度テスト NOT EQUAL", m_DeviceInfo.IsZCmpNotEqual ? "○" : "×" );
	Luna::LogoutNextLine();

	Luna::LogoutStrong( "　ソースブレンド能力" );
	Luna::LogoutNextLine();

	Luna::Logout( "　　　[ %s ] ソースブレンド BLEND FACTOR", m_DeviceInfo.IsSrcBlendFactor ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] ソースブレンド BOTH INVSRC", m_DeviceInfo.IsSrcBothInvSrc ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] ソースブレンド BOTH SRC", m_DeviceInfo.IsSrcBothSrc ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] ソースブレンド DEST ALPHA", m_DeviceInfo.IsSrcDestAlpha ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] ソースブレンド DEST COLOR", m_DeviceInfo.IsSrcDestColor ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] ソースブレンド INV DEST ALPHA", m_DeviceInfo.IsSrcInvDestAlpha ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] ソースブレンド INV DEST COLOR", m_DeviceInfo.IsSrcInvDestColor ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] ソースブレンド INV SRC ALPHA", m_DeviceInfo.IsSrcInvSrcAlpha ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] ソースブレンド INV SRC COLOR", m_DeviceInfo.IsSrcInvSrcColor ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] ソースブレンド ONE", m_DeviceInfo.IsSrcOne ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] ソースブレンド SRC ALPHA", m_DeviceInfo.IsSrcSrcAlpha ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] ソースブレンド SRC ALPHA SAT", m_DeviceInfo.IsSrcSrcAphaSat ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] ソースブレンド SRC COLOR", m_DeviceInfo.IsSrcSrcColor ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] ソースブレンド ZERO", m_DeviceInfo.IsSrcZero ? "○" : "×" );
	Luna::LogoutNextLine();

	Luna::LogoutStrong( "　デストブレンド能力" );
	Luna::LogoutNextLine();

	Luna::Logout( "　　　[ %s ] デストブレンド BLEND FACTOR", m_DeviceInfo.IsDstBlendFactor ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] デストブレンド BOTH INVSRC", m_DeviceInfo.IsDstBothInvSrc ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] デストブレンド BOTH SRC", m_DeviceInfo.IsDstBothSrc ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] デストブレンド DEST ALPHA", m_DeviceInfo.IsDstDestAlpha ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] デストブレンド DEST COLOR", m_DeviceInfo.IsDstDestColor ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] デストブレンド INV DEST ALPHA", m_DeviceInfo.IsDstInvDestAlpha ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] デストブレンド INV DEST COLOR", m_DeviceInfo.IsDstInvDestColor ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] デストブレンド INV SRC ALPHA", m_DeviceInfo.IsDstInvSrcAlpha ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] デストブレンド INV SRC COLOR", m_DeviceInfo.IsDstInvSrcColor ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] デストブレンド ONE", m_DeviceInfo.IsDstOne ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] デストブレンド SRC ALPHA", m_DeviceInfo.IsDstSrcAlpha ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] デストブレンド SRC ALPHA SAT", m_DeviceInfo.IsDstSrcAphaSat ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] デストブレンド SRC COLOR", m_DeviceInfo.IsDstSrcColor ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] デストブレンド ZERO", m_DeviceInfo.IsDstZero ? "○" : "×" );
	Luna::LogoutNextLine();

	Luna::LogoutStrong( "　テクスチャ能力" );
	Luna::LogoutNextLine();

	Luna::Logout( "　　　[ %s ] キューブマップ", m_DeviceInfo.IsCubeTexture ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] ミップマップ", m_DeviceInfo.IsMipmapTexture ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] パースペクティブコレクト", m_DeviceInfo.IsPerspective ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] ２のべき乗以外のテクスチャ", m_DeviceInfo.IsPow2 ? "×" : "○" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] 正方形以外のテクスチャ", m_DeviceInfo.IsSquareOnly ? "×" : "○" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] テクスチャ BORDER", m_DeviceInfo.IsTextureBorder ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] テクスチャ CLAMP", m_DeviceInfo.IsTxtureClamp ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] テクスチャ INDEPENDENT", m_DeviceInfo.IsTextureIndependent ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] テクスチャ MIRROR", m_DeviceInfo.IsTextureMirror ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] テクスチャ MIRROR RONCE", m_DeviceInfo.IsTextureMirrorRonce ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] テクスチャ WRAP", m_DeviceInfo.IsTextureWrap ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　テクスチャ最大横幅 [ %u ]", m_DeviceInfo.MaxTextureWidth );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　テクスチャ最大縦幅 [ %u ]", m_DeviceInfo.MaxTextureHeight );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　テクスチャ最大リピート数 [ %u ]", m_DeviceInfo.MaxTextureRepeat );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　テクスチャ最大アスペクト [ %u ]", m_DeviceInfo.MaxTextureAspectRatio );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　テクスチャ異方性の次数 [ %u ]", m_DeviceInfo.MaxAnisotropy );
	Luna::LogoutNextLine();

	Luna::LogoutStrong( "　ステンシル能力" );
	Luna::LogoutNextLine();

	Luna::Logout( "　　　[ %s ] ステンシル KEEP", m_DeviceInfo.IsStencilKeep ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] ステンシル ZERO", m_DeviceInfo.IsStencilZero ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] ステンシル REPLACE", m_DeviceInfo.IsStencilReplace ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] ステンシル INCRSAT", m_DeviceInfo.IsStencilIncRsat ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] ステンシル DECRSAT", m_DeviceInfo.IsStencilDecRsat ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] ステンシル INVERT", m_DeviceInfo.IsStencilInvert ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] ステンシル INCR", m_DeviceInfo.IsStencilIncR ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] ステンシル DECR", m_DeviceInfo.IsStencilDecR ? "○" : "×" );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　[ %s ] ステンシル TWO SIDED", m_DeviceInfo.IsStencilTwoSided ? "○" : "×" );
	Luna::LogoutNextLine();

	Luna::LogoutStrong( "　その他" );
	Luna::LogoutNextLine();

	Luna::Logout( "　　　最大ｗ値 [ %lf ]", m_DeviceInfo.MaxVertexW );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　ガードバンド　左 [ %.3lf ]", m_DeviceInfo.GuardBandLeft );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　ガードバンド　上 [ %.3lf ]", m_DeviceInfo.GuardBandTop );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　ガードバンド　右 [ %.3lf ]", m_DeviceInfo.GuardBandRight );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　ガードバンド　下 [ %.3lf ]", m_DeviceInfo.GuardBandBottom );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　ポイントサイズ [ %.3lf ]", m_DeviceInfo.MaxPointSize );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　最大プリミティブ数 [ %u ]", m_DeviceInfo.MaxPrimitiveCount );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　最大インデックス数 [ %u ]", m_DeviceInfo.MaxVertexIndex );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　最大テクスチャステージ数 [ %u ]", m_DeviceInfo.MaxTextureBlendStages );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　最大マルチテクスチャ数 [ %u ]", m_DeviceInfo.MaxSimultaneousTextures );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　最大ハードウェアライト数 [ %u ]", m_DeviceInfo.MaxActiveLights );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　最大ブレンドマトリックス数 [ %u ]", m_DeviceInfo.MaxVertexBlendMatrices );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　最大ブレンドインデックス数 [ %u ]", m_DeviceInfo.MaxVertexBlendMatrixIndex );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　最大頂点シェーダー定数数 [ %u ]", m_DeviceInfo.MaxVertexShaderConst );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　頂点シェーダーバージョン [ %u.%u ]", D3DSHADER_VERSION_MAJOR(m_DeviceInfo.VertexShaderVersion), D3DSHADER_VERSION_MINOR(m_DeviceInfo.VertexShaderVersion) );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　ピクセルシェーダーバージョン[ %u.%u ]", D3DSHADER_VERSION_MAJOR(m_DeviceInfo.PixelShaderVersion), D3DSHADER_VERSION_MINOR(m_DeviceInfo.PixelShaderVersion) );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　動的フロー制御命令のネスティング [ %u ]", m_DeviceInfo.DynamicFlowControlDepth );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　静的フロー制御命令のネスティング [ %u ]", m_DeviceInfo.StaticFlowControlDepth );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　異方性フィルタリング最大次数 [ %u ]", m_DeviceInfo.MaxMaxAnisotropy );
	Luna::LogoutNextLine();

	Luna::LogoutNextLine();

	return true;
}

//=============================================================================
/**
	３Ｄデバイスの性能を取得します。

	@param pCaps		[in] デバイスの性能が格納されたキャパシティ構造体
	@param pDevInfo		[out] デバイス情報格納用構造体アドレス

*/
//=============================================================================
void Luna3D::GetDeviceCaps( D3DCAPS9 *pCaps, DEVICEINFORMATION *pDevInfo )
{
	//-----------------------------------------------------------
	// デバイス情報
	//-----------------------------------------------------------

	// デバイスタイプ
	pDevInfo->DeviceType				= pCaps->DeviceType;
	// ピュアデバイス
	pDevInfo->IsPureDevice				= ((pCaps->DevCaps & D3DDEVCAPS_PUREDEVICE) != 0);
	// ハードウェアTnL
	pDevInfo->IsHardwareTnL				= ((pCaps->DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) != 0);
	// ハードウェアRaster
	pDevInfo->IsSoftwareTnL				= ((pCaps->DevCaps & D3DDEVCAPS_HWRASTERIZATION) != 0);
	// ディザリング
	pDevInfo->IsDither					= ((pCaps->RasterCaps & D3DPRASTERCAPS_DITHER) != 0);
	// 深度バイアス
	pDevInfo->IsDepthBias				= ((pCaps->RasterCaps & D3DPRASTERCAPS_DEPTHBIAS) != 0);
	// Ｗバッファ
	pDevInfo->IsWBuffer					= ((pCaps->RasterCaps & D3DPRASTERCAPS_WBUFFER) != 0);
	// ミップマップ
	pDevInfo->IsAutoMipMap				= ((pCaps->Caps2 & D3DCAPS2_CANAUTOGENMIPMAP) != 0);
	// Ｚ比較
	pDevInfo->IsZCmpAlways				= ((pCaps->ZCmpCaps & D3DPCMPCAPS_ALWAYS) != 0);
	pDevInfo->IsZCmpEqual				= ((pCaps->ZCmpCaps & D3DPCMPCAPS_EQUAL) != 0);
	pDevInfo->IsZCmpGreater				= ((pCaps->ZCmpCaps & D3DPCMPCAPS_GREATER) != 0);
	pDevInfo->IsZCmpGreaterEqual		= ((pCaps->ZCmpCaps & D3DPCMPCAPS_GREATEREQUAL) != 0);
	pDevInfo->IsZCmpLess				= ((pCaps->ZCmpCaps & D3DPCMPCAPS_LESS) != 0);
	pDevInfo->IsZCmpLessEqual			= ((pCaps->ZCmpCaps & D3DPCMPCAPS_LESSEQUAL) != 0);
	pDevInfo->IsZCmpNever				= ((pCaps->ZCmpCaps & D3DPCMPCAPS_NEVER) != 0);
	pDevInfo->IsZCmpNotEqual			= ((pCaps->ZCmpCaps & D3DPCMPCAPS_NOTEQUAL) != 0);
	// アルファブレンドタイプ
	pDevInfo->IsSrcBlendFactor			= ((pCaps->SrcBlendCaps & D3DPBLENDCAPS_BLENDFACTOR) != 0);
	pDevInfo->IsSrcBothInvSrc			= ((pCaps->SrcBlendCaps & D3DPBLENDCAPS_BOTHINVSRCALPHA) != 0);
	pDevInfo->IsSrcBothSrc				= ((pCaps->SrcBlendCaps & D3DPBLENDCAPS_BOTHSRCALPHA) != 0);
	pDevInfo->IsSrcDestAlpha			= ((pCaps->SrcBlendCaps & D3DPBLENDCAPS_DESTALPHA) != 0);
	pDevInfo->IsSrcDestColor			= ((pCaps->SrcBlendCaps & D3DPBLENDCAPS_DESTCOLOR) != 0);
	pDevInfo->IsSrcInvDestAlpha			= ((pCaps->SrcBlendCaps & D3DPBLENDCAPS_INVDESTALPHA) != 0);
	pDevInfo->IsSrcInvDestColor			= ((pCaps->SrcBlendCaps & D3DPBLENDCAPS_INVDESTCOLOR) != 0);
	pDevInfo->IsSrcInvSrcAlpha			= ((pCaps->SrcBlendCaps & D3DPBLENDCAPS_INVSRCALPHA) != 0);
	pDevInfo->IsSrcInvSrcColor			= ((pCaps->SrcBlendCaps & D3DPBLENDCAPS_INVSRCCOLOR) != 0);
	pDevInfo->IsSrcOne					= ((pCaps->SrcBlendCaps & D3DPBLENDCAPS_ONE) != 0);
	pDevInfo->IsSrcSrcAlpha				= ((pCaps->SrcBlendCaps & D3DPBLENDCAPS_SRCALPHA) != 0);
	pDevInfo->IsSrcSrcAphaSat			= ((pCaps->SrcBlendCaps & D3DPBLENDCAPS_SRCALPHASAT) != 0);
	pDevInfo->IsSrcSrcColor				= ((pCaps->SrcBlendCaps & D3DPBLENDCAPS_SRCCOLOR) != 0);
	pDevInfo->IsSrcZero					= ((pCaps->SrcBlendCaps & D3DPBLENDCAPS_ZERO) != 0);
	// アルファブレンドタイ	プ
	pDevInfo->IsDstBlendFactor			= ((pCaps->DestBlendCaps & D3DPBLENDCAPS_BLENDFACTOR) != 0);
	pDevInfo->IsDstBothInvSrc			= ((pCaps->DestBlendCaps & D3DPBLENDCAPS_BOTHINVSRCALPHA) != 0);
	pDevInfo->IsDstBothSrc				= ((pCaps->DestBlendCaps & D3DPBLENDCAPS_BOTHSRCALPHA) != 0);
	pDevInfo->IsDstDestAlpha			= ((pCaps->DestBlendCaps & D3DPBLENDCAPS_DESTALPHA) != 0);
	pDevInfo->IsDstDestColor			= ((pCaps->DestBlendCaps & D3DPBLENDCAPS_DESTCOLOR) != 0);
	pDevInfo->IsDstInvDestAlpha			= ((pCaps->DestBlendCaps & D3DPBLENDCAPS_INVDESTALPHA) != 0);
	pDevInfo->IsDstInvDestColor			= ((pCaps->DestBlendCaps & D3DPBLENDCAPS_INVDESTCOLOR) != 0);
	pDevInfo->IsDstInvSrcAlpha			= ((pCaps->DestBlendCaps & D3DPBLENDCAPS_INVSRCALPHA) != 0);
	pDevInfo->IsDstInvSrcColor			= ((pCaps->DestBlendCaps & D3DPBLENDCAPS_INVSRCCOLOR) != 0);
	pDevInfo->IsDstOne					= ((pCaps->DestBlendCaps & D3DPBLENDCAPS_ONE) != 0);
	pDevInfo->IsDstSrcAlpha				= ((pCaps->DestBlendCaps & D3DPBLENDCAPS_SRCALPHA) != 0);
	pDevInfo->IsDstSrcAphaSat			= ((pCaps->DestBlendCaps & D3DPBLENDCAPS_SRCALPHASAT) != 0);
	pDevInfo->IsDstSrcColor				= ((pCaps->DestBlendCaps & D3DPBLENDCAPS_SRCCOLOR) != 0);
	pDevInfo->IsDstZero					= ((pCaps->DestBlendCaps & D3DPBLENDCAPS_ZERO) != 0);
	// テクスチャ
	pDevInfo->IsCubeTexture				= ((pCaps->TextureCaps & D3DPTEXTURECAPS_CUBEMAP) != 0);
	pDevInfo->IsMipmapTexture			= ((pCaps->TextureCaps & D3DPTEXTURECAPS_MIPMAP) != 0);
	pDevInfo->IsPerspective				= ((pCaps->TextureCaps & D3DPTEXTURECAPS_PERSPECTIVE) != 0);
	pDevInfo->IsPow2					= ((pCaps->TextureCaps & D3DPTEXTURECAPS_POW2) != 0);
	pDevInfo->IsPow2Conditional			= ((pCaps->TextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL) != 0);
	pDevInfo->IsSquareOnly				= ((pCaps->TextureCaps & D3DPTEXTURECAPS_SQUAREONLY) != 0);
	// テクスチャアドレシング
	pDevInfo->IsTextureBorder			= ((pCaps->TextureAddressCaps & D3DPTADDRESSCAPS_BORDER) != 0);
	pDevInfo->IsTxtureClamp				= ((pCaps->TextureAddressCaps & D3DPTADDRESSCAPS_CLAMP) != 0);
	pDevInfo->IsTextureIndependent		= ((pCaps->TextureAddressCaps & D3DPTADDRESSCAPS_INDEPENDENTUV) != 0);
	pDevInfo->IsTextureMirror			= ((pCaps->TextureAddressCaps & D3DPTADDRESSCAPS_MIRROR) != 0);
	pDevInfo->IsTextureMirrorRonce		= ((pCaps->TextureAddressCaps & D3DPTADDRESSCAPS_MIRRORONCE) != 0);
	pDevInfo->IsTextureWrap				= ((pCaps->TextureAddressCaps & D3DPTADDRESSCAPS_WRAP) != 0);
	// テクスチャ情報
	pDevInfo->MaxTextureWidth			= pCaps->MaxTextureWidth;
	pDevInfo->MaxTextureHeight			= pCaps->MaxTextureHeight;
	pDevInfo->MaxTextureRepeat			= pCaps->MaxTextureRepeat;
	pDevInfo->MaxTextureAspectRatio		= pCaps->MaxTextureAspectRatio;
	pDevInfo->MaxAnisotropy				= pCaps->MaxAnisotropy;
	// ステンシル
	pDevInfo->IsStencilKeep				= ((pCaps->StencilCaps & D3DSTENCILCAPS_KEEP) != 0);
	pDevInfo->IsStencilZero				= ((pCaps->StencilCaps & D3DSTENCILCAPS_ZERO) != 0);
	pDevInfo->IsStencilReplace			= ((pCaps->StencilCaps & D3DSTENCILCAPS_REPLACE) != 0);
	pDevInfo->IsStencilIncRsat			= ((pCaps->StencilCaps & D3DSTENCILCAPS_INCRSAT) != 0); 
	pDevInfo->IsStencilDecRsat			= ((pCaps->StencilCaps & D3DSTENCILCAPS_DECRSAT) != 0); 
	pDevInfo->IsStencilInvert			= ((pCaps->StencilCaps & D3DSTENCILCAPS_INVERT) != 0);
	pDevInfo->IsStencilIncR				= ((pCaps->StencilCaps & D3DSTENCILCAPS_INCR) != 0);
	pDevInfo->IsStencilDecR				= ((pCaps->StencilCaps & D3DSTENCILCAPS_DECR) != 0);
	pDevInfo->IsStencilTwoSided			= ((pCaps->StencilCaps & D3DSTENCILCAPS_TWOSIDED) != 0);
	// 他
	pDevInfo->MaxVertexW				= pCaps->MaxVertexW;
	pDevInfo->GuardBandLeft				= pCaps->GuardBandLeft;
	pDevInfo->GuardBandTop				= pCaps->GuardBandTop;
	pDevInfo->GuardBandRight			= pCaps->GuardBandRight;
	pDevInfo->GuardBandBottom			= pCaps->GuardBandBottom;
	pDevInfo->MaxPointSize				= pCaps->MaxPointSize;
	pDevInfo->MaxPrimitiveCount			= pCaps->MaxPrimitiveCount;
	pDevInfo->MaxVertexIndex			= pCaps->MaxVertexIndex;
	pDevInfo->MaxMaxAnisotropy			= pCaps->MaxAnisotropy;
	// テクスチャ
	pDevInfo->MaxTextureBlendStages		= pCaps->MaxTextureBlendStages;
	pDevInfo->MaxSimultaneousTextures	= pCaps->MaxSimultaneousTextures;
	// ライト
	pDevInfo->MaxActiveLights			= pCaps->MaxActiveLights;
	// ボーン処理
	pDevInfo->MaxVertexBlendMatrices	= pCaps->MaxVertexBlendMatrices;
	pDevInfo->MaxVertexBlendMatrixIndex	= pCaps->MaxVertexBlendMatrixIndex;
	// シェーダー
	pDevInfo->VertexShaderVersion		= pCaps->VertexShaderVersion;
	pDevInfo->PixelShaderVersion		= pCaps->PixelShaderVersion;
	pDevInfo->MaxVertexShaderConst		= pCaps->MaxVertexShaderConst;
	pDevInfo->DynamicFlowControlDepth	= pCaps->VS20Caps.DynamicFlowControlDepth;
	pDevInfo->StaticFlowControlDepth	= pCaps->VS20Caps.StaticFlowControlDepth;
}

//=============================================================================
/**
	３Ｄ関係のデバイスを起動します。

	@param hWindow		[in] ウィンドウハンドル
	@param IsWindow		[in] ウィンドウモードの時は true を指定

	@retval true		成功
	@retval false		失敗
*/
//=============================================================================
Bool Luna3D::Start( HWND hWindow, Bool IsWindow )
{
	HRESULT hr = E_FAIL;

	Luna::LogoutMainCategory( "Luna3Dクラスの起動" );
	Luna::LogoutNextLine();

	//-----------------------------------------------------------
	// 生成するデバイスタイプの設定
	//-----------------------------------------------------------
	Sint32 DeviceNo = Luna3D::GetDeviceTypeNo();

	//-----------------------------------------------------------
	// デバイス生成用パラメーターの設定
	//-----------------------------------------------------------
	m_PresentParams[0] = MakePresentParameters( hWindow, true, false );
	m_PresentParams[1] = MakePresentParameters( hWindow, false, m_IsFullColor );

	D3DPRESENT_PARAMETERS PresentParam = m_PresentParams[IsWindow ? 0 : 1];

	//-----------------------------------------------------------
	// ３Ｄデバイスの生成
	//-----------------------------------------------------------
	Luna::LogoutStrong( "　Direct3Dデバイスの生成" );
	Luna::LogoutNextLine();

	if ( !CreateDeviceByNVPerfHUD( hWindow, &PresentParam ) )
	{
		do
		{
			// デバイス生成
			hr = m_pDirect3D->CreateDevice(
								D3DADAPTER_DEFAULT,
								DeviceType[DeviceNo],
								hWindow,
								CreateFlag[DeviceNo] | D3DCREATE_MULTITHREADED,
								&PresentParam,
								&m_pDevice );
			if SUCCEEDED( hr ) break;
		}
		while ( ++DeviceNo < 5 );

		// デバイス生成終了
		switch ( DeviceNo )
		{
		case 0:
			Luna::Logout( "　　　Direct3Dデバイスを [ D3DDEVTYPE_HAL ] [ D3DCREATE_MIXED_VERTEXPROCESSING ] で生成" );
			Luna::LogoutNextLine();
			break;
		case 1:
			Luna::Logout( "　　　Direct3Dデバイスを [ D3DDEVTYPE_HAL ] [ D3DCREATE_SOFTWARE_VERTEXPROCESSING ] で生成" );
			Luna::LogoutNextLine();
			break;
		case 2:
			Luna::Logout( "　　　Direct3Dデバイスを [ D3DDEVTYPE_REF ] [ D3DCREATE_SOFTWARE_VERTEXPROCESSING ] で生成" );
			Luna::LogoutNextLine();
			break;
		default:
			Luna::LogoutError( "　　　エラー：デバイスの作成に失敗 [ %s ]", Luna3D::GetError(hr) );
			Luna::LogoutNextLine();
			Luna::LogoutError( "　　　　　　：DirectX側のエラーで無い場合は他のソフトがビデオカードを占有してる可能性があります。" );
			Luna::LogoutNextLine();
			return false;
		}
	}

	//-----------------------------------------------------------
	// VRAM
	//-----------------------------------------------------------
	Luna::LogoutStrong( "　推定残テクスチャメモリ容量 <%4dMB>", Luna3D::GetRestVRAM() );
	Luna::LogoutNextLine();

	//-----------------------------------------------------------
	// IDirect3DQuery9インターフェースを取得
	//-----------------------------------------------------------
	// イベント取得用
	hr = m_pDevice->CreateQuery( D3DQUERYTYPE_EVENT, &m_pQuery[0] );
	if FAILED( hr )
	{
		Luna::LogoutError( "　　　エラー：IDirect3DQuery9インターフェースの取得に失敗 [ %s ]", Luna3D::GetError(hr) );
		Luna::LogoutNextLine();
	}

	// ピクセルフィルレート取得用
	hr = m_pDevice->CreateQuery( D3DQUERYTYPE_OCCLUSION, &m_pQuery[1] );
	if FAILED( hr )
	{
		Luna::LogoutError( "　　　エラー：IDirect3DQuery9インターフェースの取得に失敗 [ %s ]", Luna3D::GetError(hr) );
		Luna::LogoutNextLine();
	}

	//-----------------------------------------------------------
	// レンダリングターゲット取得
	//-----------------------------------------------------------
	m_pDevice->GetRenderTarget( 0, &m_pRenderTarget );

	//-----------------------------------------------------------
	// デバイス設定
	//-----------------------------------------------------------
	InitializeDeviceSetteing();
	ResetDeviceSetteing();

	//-----------------------------------------------------------
	// 現在のデバイスの性能を取得
	//-----------------------------------------------------------
	D3DCAPS9 Caps;
	m_pDevice->GetDeviceCaps( &Caps );
	GetDeviceCaps( &Caps, &m_DeviceInfo );

	Luna::LogoutStrong( "　現在のデバイス情報を取得" );
	Luna::LogoutNextLine();

	Luna::Logout( "　　　ポイントサイズ [ %.3lf ]", m_DeviceInfo.MaxPointSize );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　最大プリミティブ数 [ %u ]", m_DeviceInfo.MaxPrimitiveCount );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　最大インデックス数 [ %u ]", m_DeviceInfo.MaxVertexIndex );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　最大テクスチャステージ数 [ %u ]", m_DeviceInfo.MaxTextureBlendStages );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　最大マルチテクスチャ数 [ %u ]", m_DeviceInfo.MaxSimultaneousTextures );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　最大ハードウェアライト数 [ %u ]", m_DeviceInfo.MaxActiveLights );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　最大ブレンドマトリックス数 [ %u ]", m_DeviceInfo.MaxVertexBlendMatrices );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　最大ブレンドインデックス数 [ %u ]", m_DeviceInfo.MaxVertexBlendMatrixIndex );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　最大頂点シェーダー定数数 [ %u ]", m_DeviceInfo.MaxVertexShaderConst );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　頂点シェーダーバージョン [ %u.%u ]", D3DSHADER_VERSION_MAJOR(m_DeviceInfo.VertexShaderVersion), D3DSHADER_VERSION_MINOR(m_DeviceInfo.VertexShaderVersion) );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　ピクセルシェーダーバージョン[ %u.%u ]", D3DSHADER_VERSION_MAJOR(m_DeviceInfo.PixelShaderVersion), D3DSHADER_VERSION_MINOR(m_DeviceInfo.PixelShaderVersion) );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　動的フロー制御命令のネスティング [ %u ]", m_DeviceInfo.DynamicFlowControlDepth );
	Luna::LogoutNextLine();
	Luna::Logout( "　　　静的フロー制御命令のネスティング [ %u ]", m_DeviceInfo.StaticFlowControlDepth );
	Luna::LogoutNextLine();

	Luna::LogoutNextLine();

	return true;
}

//=============================================================================
/**
	デバイス設定の初期化をします

*/
//=============================================================================
Bool Luna3D::CreateDeviceByNVPerfHUD( HWND hWindow, D3DPRESENT_PARAMETERS *pPresentParam )
{
	if ( m_IsNVPerfHUD )
	{
		for ( Uint32 Adapter = 0; Adapter < m_pDirect3D->GetAdapterCount(); Adapter++ )
		{
			D3DADAPTER_IDENTIFIER9 Identifier;
			
			m_pDirect3D->GetAdapterIdentifier( Adapter, 0, &Identifier );
			if ( !strcmp( Identifier.Description, "NVIDIA NVPerfHUD" ) )
			{
				HRESULT hr = m_pDirect3D->CreateDevice(
									Adapter,
									D3DDEVTYPE_REF,
									hWindow,
									D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
									pPresentParam,
									&m_pDevice );
				if SUCCEEDED( hr )
				{
					Luna::Logout( "　　　Direct3Dデバイスを [ NVPerfHUD用 ] で生成" );
					Luna::LogoutNextLine();

					return true;
				}
			}
		}
	}

	return false;
}

//=============================================================================
/**
	デバイス設定の初期化をします

*/
//=============================================================================
void Luna3D::InitializeDeviceSetteing( void )
{
	Sint32 Width, Height;
	Luna::GetScreenData( &Width, &Height, NULL );
	// ビューポート初期値
	m_Viewport.X			= 0;
	m_Viewport.Y			= 0;
	m_Viewport.Width		= Width;
	m_Viewport.Height		= Height;
    m_Viewport.MinZ			= 0.0000000000000000000000001f;
	m_Viewport.MaxZ			= 1.0000000000000000000000000f;

	// 各サーフェイス
	m_lTexTarget			= INVALID_TEXTURE;
	m_lSurfTarget			= INVALID_SURFACE;
	m_lSurfDepthStencil		= INVALID_SURFACE;
	m_lCamera				= INVALID_CAMERA;

	// レンダーステート
	MemoryFill( m_RenderState, 0xFF, sizeof(m_RenderState) );
	for ( Uint32 i = 0; i < RenderStateTblCount; i++ )
	{
		m_pDevice->GetRenderState( RenderStateTbl[i], &m_RenderState[RenderStateTbl[i]] );
	}

	// テクスチャステート
	MemoryFill( m_TextureState, 0xFF, sizeof(m_TextureState) );
	for ( Uint32 i = 0; i < TEXTURE_STAGE_MAX; i++ )
	{
		for ( Uint32 j = 0; j < TextureStateTblCount; j++ )
		{
			m_pDevice->GetTextureStageState( i, TextureStateTbl[j], &m_TextureState[i][TextureStateTbl[j]] );
		}
	}

	// サンプラーステート
	MemoryFill( m_SamplerState, 0xFF, sizeof(m_SamplerState) );
	for ( Uint32 i = 0; i < TEXTURE_STAGE_MAX; i++ )
	{
		for ( Uint32 j = 0; j < SamplerStateTblCount; j++ )
		{
			m_pDevice->GetSamplerState( i, SamplerStateTbl[j], &m_SamplerState[i][SamplerStateTbl[j]] );
		}
	}

	//　レンダリングステート
	Luna3D::SetRenderState( D3DRS_LIGHTING, false );
	Luna3D::SetRenderState( D3DRS_ZENABLE, false );
	Luna3D::SetRenderState( D3DRS_ZWRITEENABLE, false );
	Luna3D::SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	Luna3D::SetRenderState( D3DRS_ALPHATESTENABLE, false );
	Luna3D::SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	for ( Sint32 i = 0; i < TEXTURE_STAGE_MAX; i++ )
	{
		// サンプラーステート
		Luna3D::SetSamplerState( i, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
		Luna3D::SetSamplerState( i, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
		Luna3D::SetSamplerState( i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		Luna3D::SetSamplerState( i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		Luna3D::SetSamplerState( i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
		Luna3D::SetSamplerState( i, D3DSAMP_MAXANISOTROPY, m_DeviceInfo.MaxAnisotropy );

		// テクスチャステート
		Luna3D::SetTextureState( i, D3DTSS_COLOROP, (i==0)?(D3DTOP_MODULATE):(D3DTOP_DISABLE) );
		Luna3D::SetTextureState( i, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		Luna3D::SetTextureState( i, D3DTSS_COLORARG2, D3DTA_CURRENT );

		Luna3D::SetTextureState( i, D3DTSS_ALPHAOP, (i==0)?(D3DTOP_MODULATE):(D3DTOP_DISABLE) );
		Luna3D::SetTextureState( i, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		Luna3D::SetTextureState( i, D3DTSS_ALPHAARG2, D3DTA_CURRENT );
	}

	// マテリアル用デフォルト生成
	for ( Uint32 i = 0; i < TEXTURE_STAGE_MAX; i++ )
	{
		for ( Uint32 j = 0; j < TextureStateTblCount; j++ )
		{
			m_DefaultState.TextureState[i][TextureStateTbl[j]] = m_TextureState[i][TextureStateTbl[j]];
		}

		for ( Uint32 j = 0; j < SamplerStateTblCount; j++ )
		{
			m_DefaultState.SamplerState[i][SamplerStateTbl[j]] = m_SamplerState[i][SamplerStateTbl[j]];
		}
	}

	m_DefaultState.BlendType	= BLEND_NONE;
	m_DefaultState.IsColorKey	= true;
	m_DefaultState.IsBilinear	= true;
	m_DefaultState.IsZBuffer	= true;
	m_DefaultState.IsZWrite		= true;
}

//=============================================================================
/**
	デバイス設定の初期化をします

*/
//=============================================================================
void Luna3D::ResetDeviceSetteing( void )
{
	//------------------------------------------------------------------
	// ビューポートを設定
	//------------------------------------------------------------------
	m_pDevice->SetViewport( &m_Viewport );

	//------------------------------------------------------------------
	// レンダリングターゲットテクスチャ
	//------------------------------------------------------------------
	if ( m_lTexTarget != NULL )
	{
		((CTexture*)m_lTexTarget)->SetRenderTarget();
	}

	//------------------------------------------------------------------
	// レンダリングターゲットサーフェイス
	//------------------------------------------------------------------
	if ( m_lSurfTarget != NULL )
	{
		((CSurface*)m_lSurfTarget)->SetRenderTarget();
	}

	//------------------------------------------------------------------
	// 深度ステンシルサーフェイス
	//------------------------------------------------------------------
	if ( m_lSurfDepthStencil != NULL )
	{
		((CSurface*)m_lSurfDepthStencil)->SetDepthStencil();
	}

	//------------------------------------------------------------------
	// カメラ
	//------------------------------------------------------------------
	if ( m_lCamera != INVALID_CAMERA )
	{
		LunaCamera::SetDevice( m_lCamera );
	}

	//------------------------------------------------------------------
	// レンダリングステート
	//------------------------------------------------------------------
	for ( Uint32 i = 0; i < RenderStateTblCount; i++ )
	{
		m_pDevice->SetRenderState( RenderStateTbl[i], m_RenderState[RenderStateTbl[i]] );
	}

	//------------------------------------------------------------------
	// テクスチャーステート
	//------------------------------------------------------------------
	for ( Uint32 i = 0; i < TEXTURE_STAGE_MAX; i++ )
	{
		for ( Uint32 j = 0; j < TextureStateTblCount; j++ )
		{
			m_pDevice->SetTextureStageState( i, TextureStateTbl[j], m_TextureState[i][TextureStateTbl[j]] );
		}
	}

	//------------------------------------------------------------------
	// サンプラーステート
	//------------------------------------------------------------------
	for ( Uint32 i = 0; i < TEXTURE_STAGE_MAX; i++ )
	{
		for ( Uint32 j = 0; j < SamplerStateTblCount; j++ )
		{
			m_pDevice->SetSamplerState( i, SamplerStateTbl[j], m_SamplerState[i][SamplerStateTbl[j]] );
		}
	}

	//------------------------------------------------------------------
	// バックバッファクリア
	//------------------------------------------------------------------
	Luna3D::Clear( D3DCLEAR_TARGET );
}

//=============================================================================
/**
	エミュレーションモードで起動するようにします。

*/
//=============================================================================
void Luna3D::EnableEmulationMode( void )
{
	m_IsEmulationMode = true;
}

//=============================================================================
/**
	エミュレーションモードを使用するようにします。

*/
//=============================================================================
void Luna3D::EnableFullColorMode( void )
{
	m_IsFullColor = true;
}

//=============================================================================
/**
	画面の更新時に垂直同期街をするようになります。

*/
//=============================================================================
void Luna3D::EnableWaitVBlank( void )
{
	m_IsWaitBlank = true;
}

//=============================================================================
/**
	画面の更新時に垂直同期街をするようになります。

*/
//=============================================================================
void Luna3D::EnableNVPerfHUD( void )
{
	m_IsNVPerfHUD = true;
}

//=============================================================================
/**
	生成するデバイスのタイプ番号を取得します。

	@retval 0		PureDevice
	@retval 1		Hardware & Spftware TnL
	@retval 2		Hardware & Spftware TnL
	@retval 3		Software TnL
	@retval 4		Reference Rasterizer
*/
//=============================================================================
Sint32 Luna3D::GetDeviceTypeNo( void )
{
	Sint32 No = 2;		// REF

	// 強制エミュレーションモード？
	if ( m_IsEmulationMode ) return No;

	// ハードウェアラスタライズサポート？
	if ( !m_DeviceInfo.IsSoftwareTnL ) return No;

	No--;

	// ハードウェアＴｎＬサポート？
	if ( !m_DeviceInfo.IsHardwareTnL ) return No;

	No--;

	return No;
}


//=============================================================================
/**
	使用するプレゼンテーションパラメーターを取得します。

	@param hWnd				[in] ウィンドウハンドル
	@param IsWindow			[in] true の時 ウィンドウモード / false の時 フルスクリーンモード
	@param IsFullColor		[in] true の時 32Bitモード / false の時 16Bitモード

	@return			生成されたプレゼンテーションパラメーター構造体
*/
//=============================================================================
D3DPRESENT_PARAMETERS Luna3D::MakePresentParameters( HWND hWnd, Bool IsWindow, Bool IsFullColor )
{
	Sint32 Width, Height;
	D3DPRESENT_PARAMETERS PresentParam;
	D3DFORMAT DepthFormat = D3DFMT_UNKNOWN;
	D3DFORMAT ScreenFormat = IsWindow ? m_WindowMode.Format : m_FullScreenMode[IsFullColor ? 1 : 0].Format;

	//-----------------------------------------------------------
	// 画面サイズ取得
	//-----------------------------------------------------------
	Luna::GetScreenData( &Width, &Height, NULL );

	//-----------------------------------------------------------
	// 有効なフォーマット取得
	//-----------------------------------------------------------
	for ( Sint32 i = 0; i < 8; i++ )
	{
		if ( BackBufferFormats[i] == ScreenFormat )
		{
			DepthFormat = m_AdapterData.DepthFormats[0];
		}
	}

	//-----------------------------------------------------------
	// パラメーター生成
	//-----------------------------------------------------------
	PresentParam.BackBufferWidth			= Width;
	PresentParam.BackBufferHeight			= Height;
	PresentParam.BackBufferFormat			= ScreenFormat;
	PresentParam.BackBufferCount			= 1;
	PresentParam.MultiSampleType			= m_MultiSampleType;
	PresentParam.MultiSampleQuality			= m_MultiSampleQuality;
	PresentParam.SwapEffect					= D3DSWAPEFFECT_DISCARD;
	PresentParam.hDeviceWindow				= hWnd;
	PresentParam.Windowed					= IsWindow;
	PresentParam.EnableAutoDepthStencil		= false;
	PresentParam.AutoDepthStencilFormat		= DepthFormat;
	PresentParam.Flags						= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	PresentParam.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
	PresentParam.PresentationInterval		= m_IsWaitBlank ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;

	return PresentParam;
}

//=============================================================================
/**
	３Ｄデバイスのリセットをします。

	@param IsChange		[in] true の時、画面モードを切り替えます
*/
//=============================================================================
void Luna3D::ResetDevice( Bool IsChange )
{
	HRESULT hr;

	//-------------------------------------------------------
	// デバイスがまだ生成されていない場合は終了
	//-------------------------------------------------------
	if ( m_pDevice == NULL ) return;

	//-------------------------------------------------------
	// スクリーンモードの変更でない時、デバイスが問題ないなら終了
	//-------------------------------------------------------
	if ( !IsChange && (m_pDevice->TestCooperativeLevel() == D3D_OK) ) return;

	//-------------------------------------------------------
	// プレゼンテーションパラメータ取得
	//-------------------------------------------------------
	Sint32 Width, Height;
	Bool IsWindow;
	Luna::GetScreenData( &Width, &Height, &IsWindow );
	m_PresentParams[0].BackBufferWidth		= Width;
	m_PresentParams[0].BackBufferHeight	= Height;
	m_PresentParams[1].BackBufferWidth		= Width;
	m_PresentParams[1].BackBufferHeight	= Height;

	D3DPRESENT_PARAMETERS PresentParam = m_PresentParams[IsWindow ? 0 : 1];

	//-------------------------------------------------------
	// ビデオメモリ内のリソース全解放
	//-------------------------------------------------------
	LunaTexture::Reset();
	LunaVMR9::Reset();
	LunaSurface::Reset();
	LunaSprite::Reset();
	LunaSprite3D::Reset();
	LunaPrimitive::Reset();
	LunaPrimitive3D::Reset();
	LunaLine::Reset();
	LunaLine3D::Reset();
	LunaPoint::Reset();
	LunaPoint3D::Reset();
	LunaModel::Reset();
	LunaModelEx::Reset();
	LunaField::Reset();
	LunaSimple::Reset();
	LunaShaderFx::Reset();

	SAFE_RELEASE( m_pQuery[0] );
	SAFE_RELEASE( m_pQuery[1] );
	SAFE_RELEASE( m_pRenderTarget );

	if ( m_pCallbackReset != NULL )
	{
		m_pCallbackReset();
	}

	//-------------------------------------------------------
	// デバイスのリセット
	//-------------------------------------------------------
	if FAILED( m_pDevice->Reset( &PresentParam ) )
	{
		Luna::MsgBox( MB_ICONERROR, "ERROR", "デバイスの復帰に失敗" );
	}

	m_pDevice->GetRenderTarget( 0, &m_pRenderTarget );

	//-------------------------------------------------------
	// IDirect3DQuery9インターフェースを取得
	//-------------------------------------------------------
	// イベント取得用
	hr = m_pDevice->CreateQuery( D3DQUERYTYPE_EVENT, &m_pQuery[0] );
	if FAILED( hr )
	{
		Luna::LogoutError( "　　　エラー：IDirect3DQuery9インターフェースの取得に失敗 [ %s ]", Luna3D::GetError(hr) );
		Luna::LogoutNextLine();
	}

	// ピクセルフィルレート取得用
	hr = m_pDevice->CreateQuery( D3DQUERYTYPE_OCCLUSION, &m_pQuery[1] );
	if FAILED( hr )
	{
		Luna::LogoutError( "　　　エラー：IDirect3DQuery9インターフェースの取得に失敗 [ %s ]", Luna3D::GetError(hr) );
		Luna::LogoutNextLine();
	}

	m_IsCheckingFillRate = false;

	//-------------------------------------------------------
	// ビデオメモリ内のリソース全復旧
	//-------------------------------------------------------
	LunaTexture::Restore();
	LunaVMR9::Restore();
	LunaSurface::Restore();
	LunaSprite::Restore();
	LunaSprite3D::Restore();
	LunaPrimitive::Restore();
	LunaPrimitive3D::Restore();
	LunaLine::Restore();
	LunaLine3D::Restore();
	LunaPoint::Restore();
	LunaPoint3D::Restore();
	LunaModel::Restore();
	LunaModelEx::Restore();
	LunaField::Restore();
	LunaSimple::Restore();
	LunaShaderFx::Restore();

	if ( m_pCallbackRestore != NULL )
	{
		m_pCallbackRestore();
	}

	//-------------------------------------------------------
	// デバイスの設定を復帰
	//-------------------------------------------------------
	Luna3D::ResetDeviceSetteing();
}

//=============================================================================
/*
	デバイスロスト時のデータリセット用コールバック関数設定

	@param pCallback		[in] コールバック関数

*/
//=============================================================================
void Luna3D::SetCallbackDeviceReset( void (*pCallback)( void ) )
{
	m_pCallbackReset = pCallback;
}

//=============================================================================
/*
	デバイスロスト時のデータリストア用コールバック関数設定

	@param pCallback		[in] コールバック関数

*/
//=============================================================================
void Luna3D::SetCallbackDeviceRestore( void (*pCallback)( void ) )
{
	m_pCallbackRestore = pCallback;
}

//=============================================================================
/*
	使用する３Ｄデバイスのデバイス情報を取得します。

	@param pInfo		[in] デバイス情報格納先アドレス

*/
//=============================================================================
void Luna3D::GetDeviceInformation( DEVICEINFORMATION *pInfo )
{
	MemoryCopy( pInfo, &m_DeviceInfo, sizeof(DEVICEINFORMATION) );
}

//=============================================================================
/**
	使用している３Ｄデバイスを取得します。

	@return		３Ｄデバイスオブジェクト
*/
//=============================================================================
HMONITOR Luna3D::GetAdapterMonitor( void )
{
	return m_pDirect3D->GetAdapterMonitor( D3DADAPTER_DEFAULT );
}

//=============================================================================
/**
	使用している３Ｄデバイスを取得します。

	@return		３Ｄデバイスオブジェクト
*/
//=============================================================================
IDirect3DDevice9 *Luna3D::GetDevice( void )
{
	return m_pDevice;
}

//=============================================================================
/**
	ピクセルシェーダーのサポートチェックをします

	@param Major		[in] メジャーバージョン
	@param Minor		[in] マイナーバージョン

	@retval true		サポートしている
	@retval false		サポートしていない
*/
//=============================================================================
Bool Luna3D::CheckPixelShaderSupport( Uint16 Major, Uint16 Minor )
{
	return (D3DPS_VERSION(Major,Minor) <= m_DeviceInfo.PixelShaderVersion);
}

//=============================================================================
/**
	使用する頂点シェーダーの最高バージョンを設定

	@param Major		[in] メジャーバージョン
	@param Minor		[in] マイナーバージョン

*/
//=============================================================================
void Luna3D::SetMaxVertexShaderViersion( Uint16 Major, Uint16 Minor )
{
	m_MaxVertexShaderVersion = D3DVS_VERSION(Major,Minor);
}

//=============================================================================
/**
	頂点シェーダーのサポートチェックをします

	@param Major		[in] メジャーバージョン
	@param Minor		[in] マイナーバージョン

	@retval true		サポートしている
	@retval false		サポートしていない
*/
//=============================================================================
void Luna3D::SetVertexProccessing( Uint16 Major, Uint16 Minor )
{
	if ( m_IsEmulationMode )
	{
		m_pDevice->SetSoftwareVertexProcessing( true );
	}
	else
	{
		if ( GetVertexProccessing( Major, Minor ) )
		{
			m_pDevice->SetSoftwareVertexProcessing( true );
		}
		else
		{
			m_pDevice->SetSoftwareVertexProcessing( false );
		}
	}
}

//=============================================================================
/**
	頂点処理タイプを取得します

	@param Major		[in] 頂点シェーダーのメジャーバージョン
	@param Minor		[in] 頂点シェーダーのマイナーバージョン

	@retval true		ハードウェアＴｎＬで処理される
	@retval false		ソフトウェアＴｎＬで処理される
*/
//=============================================================================
Bool Luna3D::GetVertexProccessing( Uint16 Major, Uint16 Minor )
{
	if ( m_DeviceInfo.IsHardwareTnL )
	{
		if ( D3DVS_VERSION(Major,Minor) <= m_DeviceInfo.VertexShaderVersion )
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return true;
	}
}


//=============================================================================
/**
	自動ミップマップ生成をサポートしているかを調べます。

	@param Format	[in] 調べるサーフェイスフォーマット
	@param IsMipmap	[in] ミップマップを使用するか否か

	@return サポート時はtrue
*/
//=============================================================================
Bool Luna3D::GetAutoGenerateMipmapEnable( D3DFORMAT Format, Bool IsMipmap )
{
	if ( IsMipmap )
	{
		for ( Sint32 i = 0; i < m_AdapterData.TextureCount; i++ )
		{
			if ( m_AdapterData.TextureFormats[i] == Format )
			{
				if ( m_AdapterData.TextureMipmap[i] )
				{
					return true;
				}
			}
		}
	}

	return false;
}


//=============================================================================
/**
	レンダリングに使用するビューポートを設定します

	@param pRect	[in] ビューポート矩形構造体へのポインタ
*/
//=============================================================================
void Luna3D::SetViewport( RECT *pRect )
{
	RECT Rect = { 0, 0, 0, 0 };
	if ( pRect == NULL )
	{
		pRect = &Rect;
		Luna::GetScreenData( &Rect.right, &Rect.bottom, NULL );
	}

	m_Viewport.X		= pRect->left;
	m_Viewport.Y		= pRect->top;
	m_Viewport.Width	= pRect->right - pRect->left;
	m_Viewport.Height	= pRect->bottom - pRect->top;
	m_Viewport.MinZ		= 0.0f;
	m_Viewport.MaxZ		= 1.0f;

	m_pDevice->SetViewport( &m_Viewport );
}

//=============================================================================
/**
	レンダリングに使用いているビューポートを取得します

	@param pRect	[in] ビューポート矩形構造体へのポインタ
*/
//=============================================================================
void Luna3D::GetViewport( RECT *pRect )
{
	pRect->left   = m_Viewport.X;
	pRect->top    = m_Viewport.Y;
	pRect->right  = m_Viewport.X + m_Viewport.Width;
	pRect->bottom = m_Viewport.Y + m_Viewport.Height;
}

//=============================================================================
/**
	レンダリングステートをデバイスに設定します。<BR>
	パラメーター関係はDirectX9のヘルプを参照してください。

	@param State	[in] 設定するステートタイプ
	@param Param	[in] パラメータ
*/
//=============================================================================
void Luna3D::SetRenderState( D3DRENDERSTATETYPE State, Uint32 Param )
{
	if ( m_RenderState[State] != Param )
	{
		m_RenderState[State] = Param;
		m_pDevice->SetRenderState( State, Param );
	}
}

//=============================================================================
/**
	テクスチャステートをデバイスに設定します<BR>
	パラメーター関係はDirectX9のヘルプを参照してください。

	@param Stage	[in] 設定するステージ
	@param State	[in] 設定するステートタイプ
	@param Param	[in] パラメータ
*/
//=============================================================================
void Luna3D::SetTextureState( Uint32 Stage, D3DTEXTURESTAGESTATETYPE State, Uint32 Param )
{
	if ( (0 <= Stage) && (Stage < TEXTURE_STAGE_MAX) )
	{
		if ( m_TextureState[Stage][State] != Param )
		{
			m_TextureState[Stage][State] = Param;
			m_pDevice->SetTextureStageState( Stage, State, Param );
		}
	}
}

//=============================================================================
/**
	サンプラーステートをデバイスに設定します<BR>
	パラメーター関係はDirectX9のヘルプを参照してください

	@param Stage	[in] 設定するステージ
	@param State	[in] 設定するステートタイプ
	@param Param	[in] パラメータ
*/
//=============================================================================
void Luna3D::SetSamplerState( Uint32 Stage, D3DSAMPLERSTATETYPE State, Uint32 Param )
{
	if ( (0 <= Stage) && (Stage < TEXTURE_STAGE_MAX) )
	{
		if ( m_SamplerState[Stage][State] != Param )
		{
			m_SamplerState[Stage][State] = Param;
			m_pDevice->SetSamplerState( Stage, State, Param );
		}
	}
}

//=============================================================================
/**
	アルファブレンディングのタイプを設定します

	@param BlendType		[in] 設定するブレンドタイプ

*/
//=============================================================================
void Luna3D::SetBlendingType( eBlendType BlendType )
{
	switch ( BlendType )
	{
	case BLEND_NONE:
		Luna3D::SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
		Luna3D::SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );
		break;

	case BLEND_NORMAL:
		Luna3D::SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
		Luna3D::SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		break;

	case BLEND_ADD:
		Luna3D::SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
		Luna3D::SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
		break;

	case BLEND_ADD_NOALPHA:
		Luna3D::SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
		Luna3D::SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
		break;

	case BLEND_MUL:
		Luna3D::SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ZERO );
		Luna3D::SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR );
		break;

	case BLEND_REVERSE:
		Luna3D::SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_INVDESTCOLOR );
		Luna3D::SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );
		break;
	}
}

//=============================================================================
/**
	バイリニアフィルタのＯＮ／ＯＦＦをします

	@param Flag		[in] バイリニアフィルタをＯＮにするなら true / ＯＦＦにするなら false

*/
//=============================================================================
void Luna3D::SetBilinearEnable( Bool Flag )
{
	if ( Flag )
	{
		LunaTexture::SetOffsetUV( 0.5f );
		for ( Sint32 i = 0; i < TEXTURE_STAGE_MAX; i++ )
		{
			if ( m_DeviceInfo.MaxMaxAnisotropy > 0 )
			{
				Luna3D::SetSamplerState( i, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC );
				Luna3D::SetSamplerState( i, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC );
				Luna3D::SetSamplerState( i, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC );
				Luna3D::SetSamplerState( i, D3DSAMP_MAXANISOTROPY, m_DeviceInfo.MaxMaxAnisotropy );
			}
			else
			{
				Luna3D::SetSamplerState( i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
				Luna3D::SetSamplerState( i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
				Luna3D::SetSamplerState( i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
			}
		}
	}
	else
	{
		LunaTexture::SetOffsetUV( 0.0f );
		for ( Sint32 i = 0; i < TEXTURE_STAGE_MAX; i++ )
		{
			Luna3D::SetSamplerState( i, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
			Luna3D::SetSamplerState( i, D3DSAMP_MINFILTER, D3DTEXF_POINT );
			Luna3D::SetSamplerState( i, D3DSAMP_MIPFILTER, D3DTEXF_POINT );
		}
	}
}

//=============================================================================
/**
	カラーキー処理のＯＮ／ＯＦＦをします

	@param Flag		[in] カラーキーをＯＮにするなら true / ＯＦＦにするなら false

*/
//=============================================================================
void Luna3D::SetColorkeyEnable( Bool Flag )
{
	if ( Flag )
	{
		Luna3D::SetRenderState( D3DRS_ALPHABLENDENABLE, true );
		Luna3D::SetRenderState( D3DRS_ALPHATESTENABLE, true );
		Luna3D::SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER );
		Luna3D::SetRenderState( D3DRS_ALPHAREF, 8 );
	}
	else
	{
		Luna3D::SetRenderState( D3DRS_ALPHABLENDENABLE, true );
		Luna3D::SetRenderState( D3DRS_ALPHATESTENABLE, false );
	}
}

//=============================================================================
/**
	カラーキー処理のＯＮ／ＯＦＦをします

	@param Flag		[in] カラーキーをＯＮにするなら true / ＯＦＦにするなら false

*/
//=============================================================================
void Luna3D::SetZBufferEnable( Bool Flag )
{
	if ( Flag )
	{
		Luna3D::SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
	}
	else
	{
		Luna3D::SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
	}
}

//=============================================================================
/**
	カラーキー処理のＯＮ／ＯＦＦをします

	@param Flag		[in] カラーキーをＯＮにするなら true / ＯＦＦにするなら false

*/
//=============================================================================
void Luna3D::SetZWriteEnable( Bool Flag )
{
	if ( Flag )
	{
		Luna3D::SetRenderState( D3DRS_ZWRITEENABLE, true );
	}
	else
	{
		Luna3D::SetRenderState( D3DRS_ZWRITEENABLE, false );
	}
}

//=============================================================================
/**
	カラーキー処理のＯＮ／ＯＦＦをします

	@param Flag		[in] カラーキーをＯＮにするなら true / ＯＦＦにするなら false

*/
//=============================================================================
void Luna3D::SetWireFrameEnable( Bool Flag )
{
	if ( Flag )
	{
		Luna3D::SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
		Luna3D::SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
	}
	else
	{
		Luna3D::SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
		Luna3D::SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
	}
}

//=============================================================================
/**
	フォグ用データ設定

	@param fNear	[in] フォグ開始点
	@param fFar		[in] フォグ終了点
	@param Color	[in] フォグ色

*/
//=============================================================================
void Luna3D::SetFogParameter( Float fNear, Float fFar, D3DCOLOR Color )
{
	Float start_z = 0.0f;
	Float end_z   = 1.0f;

	Luna3D::SetRenderState( D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR );
	Luna3D::SetRenderState( D3DRS_FOGCOLOR, Color & 0x00FFFFFF );
	Luna3D::SetRenderState( D3DRS_FOGSTART, *((Uint32 *)(&start_z)) );
	Luna3D::SetRenderState( D3DRS_FOGEND,   *((Uint32 *)(&end_z)) );

	m_FogNear  = fNear;
	m_FogFar   = fFar;
	m_FogColor = Color;
}

//=============================================================================
/**
	フォグ用データ取得

	@param pfNear	[in] フォグ開始点格納先
	@param pfFar	[in] フォグ終了点格納先
	@param pColor	[in] フォグ色格納先

*/
//=============================================================================
void Luna3D::GetFogParameter( Float *pfNear, Float *pfFar, D3DCOLOR *pColor )
{
	if ( pfNear != NULL ) *pfNear = m_FogNear;
	if ( pfFar  != NULL ) *pfFar  = m_FogFar;
	if ( pColor != NULL ) *pColor = m_FogColor;
}

//=============================================================================
/**
	各バッファ内容をクリアします。

	@param ClearFlags	[in] クリアフラグ<BR>
								D3DCLEAR_TARGET		レンダリング ターゲットをクリアして、Color パラメータの色にする<BR>
								D3DCLEAR_ZBUFFER	深度バッファをクリアして、Depth パラメータの値にする<BR>
								D3DCLEAR_STENCIL	ステンシル バッファをクリアして、Stencil パラメータの値にする<BR>
	@param Color		[in] D3DCLEAR_TARGET 指定時の塗りつぶし色
	@param Depth		[in] D3DCLEAR_ZBUFFER 指定時の深度値
	@param Stencil		[in] D3DCLEAR_STENCIL 指定時のステンシル値
	@param pDst			[in] 適用先矩形アドレス

	@retval true		成功
	@retval false		失敗
*/
//=============================================================================
Bool Luna3D::Clear( Uint32 ClearFlags, D3DCOLOR Color, Float Depth, Uint32 Stencil, RECT *pDst )
{
	return SUCCEEDED( m_pDevice->Clear( (pDst==NULL)?(0):(1), (D3DRECT*)pDst, ClearFlags, Color, Depth, Stencil ) );
}

//=============================================================================
/**
	画面の塗りつぶしをします

	@param pDst		[in] 塗りつぶし転送先矩形のポインタ
	@param Color	[in] 塗りつぶし色
*/
//=============================================================================
void Luna3D::ColorFill( RECT *pDst, D3DCOLOR Color )
{
	m_pDevice->Clear( (pDst==NULL)?(0):(1), (D3DRECT*)pDst, D3DCLEAR_TARGET, Color, 1, 0 );
}

//=============================================================================
/**
	３Ｄシーンの開始をデバイスに通知します。

	@retval true		成功
	@retval false		失敗
*/
//=============================================================================
Bool Luna3D::BeginScene( void )
{
	HRESULT hr = D3D_OK;

	if ( m_SceneCount == 0 )
	{
		hr = m_pDevice->BeginScene();
	}

	m_SceneCount++;

	return SUCCEEDED( hr );
}

//=============================================================================
/**
	３Ｄシーンの終了をデバイスに通知します。

*/
//=============================================================================
void Luna3D::EndScene( void )
{
	m_SceneCount--;

	if ( m_SceneCount == 0 )
	{
		m_pDevice->EndScene();
	}
}

//=============================================================================
/**
	ポリゴンをシザリングするための領域を設定します

	@param IsUse		[in] 使用するなら true / 使用しないなら false
	@param pRect		[in] シザリング矩形のポインタ
*/
//=============================================================================
void Luna3D::SetScissoring( Bool IsUse, RECT *pRect )
{
	//------------------------------------------------
	// 仮想画面から実画面へ
	//------------------------------------------------
	if ( IsUse )
	{
		pRect->left   = toI(Luna::Virtual2RealX( toF(pRect->left)   ) - 0.5f);
		pRect->right  = toI(Luna::Virtual2RealX( toF(pRect->right)  ) - 0.5f);
		pRect->top    = toI(Luna::Virtual2RealY( toF(pRect->top)    ) - 0.5f);
		pRect->bottom = toI(Luna::Virtual2RealY( toF(pRect->bottom) ) - 0.5f);
	}

	//------------------------------------------------
	// 各クラスに伝達
	//------------------------------------------------
	LunaPrimitive::SetScissoring( IsUse, pRect );
	LunaSprite::SetScissoring( IsUse, pRect );
	LunaLine::SetScissoring( IsUse, pRect );
	LunaPoint::SetScissoring( IsUse, pRect );
}

//=============================================================================
/**
	２Ｄプリミティブ関連の透視変換描画処理の設定をします

	@param IsUse		[in] 使用するなら true / 使用しないなら false
	@param LookX		[in] 注視点Ｘ座標
	@param LookY		[in] 注視点Ｘ座標
	@param Scale		[in] パースのきつさ
*/
//=============================================================================
void Luna3D::SetPerspective( Bool IsUse, Float LookX, Float LookY, Float Scale )
{
	LunaPrimitive::SetPerspective( IsUse, LookX, LookY, Scale );
	LunaSprite::SetPerspective( IsUse, LookX, LookY, Scale );
	LunaLine::SetPerspective( IsUse, LookX, LookY, Scale );
	LunaPoint::SetPerspective( IsUse, LookX, LookY, Scale );
}

//=============================================================================
/*
	２Ｄプリミティブ関連の拡大縮小描画処理の設定をします

	@param IsUse		[in] 使用するなら true / 使用しないなら false
	@param CenterX		[in] 中心Ｘ座標
	@param CenterY		[in] 中心Ｙ座標
	@param Zoom			[in] 拡大縮小率
*/
//=============================================================================
void Luna3D::SetZoom( Bool IsUse, Float CenterX, Float CenterY, Float Zoom )
{
	LunaPrimitive::SetZoom( IsUse, CenterX, CenterY, Zoom );
	LunaSprite::SetZoom( IsUse, CenterX, CenterY, Zoom );
	LunaLine::SetZoom( IsUse, CenterX, CenterY, Zoom );
	LunaPoint::SetZoom( IsUse, CenterX, CenterY, Zoom );
}

//=============================================================================
/**
	バックバッファの内容をフロントバッファに転送します。

	@param hWnd		[in] 転送先ウインドウハンドル
*/
//=============================================================================
void Luna3D::Refresh( HWND hWnd )
{
	HRESULT hr;
	//------------------------------------------------------------------
	// 画面更新
	//------------------------------------------------------------------
#ifdef __TOOL_MODE__
	if ( hWnd == NULL )
	{
		hr = m_pDevice->Present( NULL, NULL, NULL, NULL );
	}
	else
	{
		RECT Rect;
		::GetClientRect( hWnd, &Rect );
		hr = m_pDevice->Present( &Rect, &Rect, hWnd, NULL );
	}
#else
	hr = m_pDevice->Present( NULL, NULL, hWnd, NULL );
#endif
	switch ( hr )
	{
	case D3DERR_DRIVERINTERNALERROR:
#ifndef __TOOL_MODE__
		Luna::Exit();
#else
		::SendMessage( ::GetActiveWindow(), WM_CLOSE, 0, 0 );
#endif
		break;
	case D3DERR_DEVICELOST:
		switch ( m_pDevice->TestCooperativeLevel() )
		{
		case D3DERR_DEVICELOST:
			::SleepEx( 10, true );
			break;
		case D3DERR_DEVICENOTRESET:
			Luna3D::ResetDevice( false );
			break;
		}
		break;
	}
}

//=============================================================================
/**
	テクスチャをデバイスに設定します

	@param Stage	[in] テクスチャステージ
	@param lTex		[in] テクスチャハンドル

*/
//=============================================================================
void Luna3D::SetTexture( Uint32 Stage, LTEXTURE lTex )
{
	if ( (0 <= Stage) && (Stage < TEXTURE_STAGE_MAX) )
	{
		if ( lTex == NULL )
		{
			m_pDevice->SetTexture( Stage, NULL );
		}
		else
		{
			IDirect3DTexture9 *pTex;
			((CTexture*)lTex)->GetInstance( &pTex );
			m_pDevice->SetTexture( Stage, pTex );
			SAFE_RELEASE( pTex );
		}
	}
}

//=============================================================================
/**
	サーフェイス用のフォーマットの取得を行います。

	@param Format	[in] サーフェイスフォーマット
	@param OrgFmt	[in] オリジナルのファイルフォーマット

	@return		実際のサーフェイスに適用されるフォーマット
*/
//=============================================================================
D3DFORMAT Luna3D::GetFormat( eSurfaceFormat Format, D3DFORMAT OrgFmt )
{
	Bool IsWindow;
	D3DFORMAT ScreenFormat;

	//------------------------------------------------------------
	// ウィンドウ情報取得
	//------------------------------------------------------------
	Luna::GetScreenData( NULL, NULL, &IsWindow );
	ScreenFormat = IsWindow ? m_WindowMode.Format : m_FullScreenMode[m_IsFullColor ? 1 : 0].Format;

	//------------------------------------------------------------
	// フォーマット情報検索
	//------------------------------------------------------------
	switch ( Format )
	{
		// ファイルから
	case FORMAT_FROM_FILE:
		return OrgFmt;

		// 16Bit深度バッファ
	case FORMAT_DEPTH16:
		{
			D3DFORMAT FmtTbl[] = {
				D3DFMT_D16_LOCKABLE, D3DFMT_D16, D3DFMT_D15S1, D3DFMT_UNKNOWN,
			};

			for ( Sint32 i = 0; FmtTbl[i] != D3DFMT_UNKNOWN; i++ )
			{
				for ( Sint32 j = 0; j < m_AdapterData.DepthCount; j++ )
				{
					if ( FmtTbl[i] == m_AdapterData.DepthFormats[j] )
					{
						return FmtTbl[i];
					}
				}
			}
		}
		break;

		// 32Bit深度バッファ
	case FORMAT_DEPTH32:
		{
			D3DFORMAT FmtTbl[] = {
				D3DFMT_D32, D3DFMT_D24S8, D3DFMT_D24X8, D3DFMT_D24X4S4, D3DFMT_D24FS8, D3DFMT_UNKNOWN,
			};

			for ( Sint32 i = 0; FmtTbl[i] != D3DFMT_UNKNOWN; i++ )
			{
				for ( Sint32 j = 0; j < m_AdapterData.DepthCount; j++ )
				{
					if ( FmtTbl[i] == m_AdapterData.DepthFormats[j] )
					{
						return FmtTbl[i];
					}
				}
			}
		}
		break;

		// ステンシル深度バッファ
	case FORMAT_DEPTHSTENCIL:
		{
			D3DFORMAT FmtTbl[] = {
				D3DFMT_D24S8, D3DFMT_D24X4S4, D3DFMT_D24FS8, D3DFMT_D15S1, D3DFMT_UNKNOWN,
			};

			for ( Sint32 i = 0; FmtTbl[i] != D3DFMT_UNKNOWN; i++ )
			{
				for ( Sint32 j = 0; j < m_AdapterData.DepthCount; j++ )
				{
					if ( FmtTbl[i] == m_AdapterData.DepthFormats[j] )
					{
						return FmtTbl[i];
					}
				}
			}
		}
		break;

		// バックバッファーに準拠
	case FORMAT_BACKBUFFER:
		{
			return ScreenFormat;
		}
		break;

		// 16Bitレンダリングターゲット
	case FORMAT_TARGET16:
		{
			D3DFORMAT FmtTbl[] = {
				D3DFMT_A4R4G4B4, D3DFMT_A1R5G5B5, D3DFMT_X1R5G5B5, D3DFMT_UNKNOWN,
			};

			for ( Sint32 i = 0; FmtTbl[i] != D3DFMT_UNKNOWN; i++ )
			{
				for ( Sint32 j = 0; j < m_AdapterData.TargetCount; j++ )
				{
					if ( FmtTbl[i] == m_AdapterData.TargetFormats[j] )
					{
						return FmtTbl[i];
					}
				}
			}
		}
		break;

		// 32Bitレンダリングターゲット
	case FORMAT_TARGET32:
		{
			D3DFORMAT FmtTbl[] = {
				D3DFMT_A8R8G8B8, D3DFMT_X8R8G8B8, D3DFMT_UNKNOWN,
			};

			for ( Sint32 i = 0; FmtTbl[i] != D3DFMT_UNKNOWN; i++ )
			{
				for ( Sint32 j = 0; j < m_AdapterData.TargetCount; j++ )
				{
					if ( FmtTbl[i] == m_AdapterData.TargetFormats[j] )
					{
						return FmtTbl[i];
					}
				}
			}
		}
		break;

		// 浮動小数点レンダリングターゲット
	case FORMAT_TARGETFloat:
		{
			D3DFORMAT FmtTbl[] = {
				D3DFMT_A32B32G32R32F, D3DFMT_A16B16G16R16F, D3DFMT_UNKNOWN,
			};

			for ( Sint32 i = 0; FmtTbl[i] != D3DFMT_UNKNOWN; i++ )
			{
				for ( Sint32 j = 0; j < m_AdapterData.TargetCount; j++ )
				{
					if ( FmtTbl[i] == m_AdapterData.TargetFormats[j] )
					{
						return FmtTbl[i];
					}
				}
			}
		}
		break;

		// アルファ用テクスチャ
	case FORMAT_ALPHAONLY:
		{
			D3DFORMAT FmtTbl[] = {
				D3DFMT_A8, D3DFMT_A8R3G3B2, D3DFMT_A8R8G8B8, D3DFMT_A4R4G4B4, D3DFMT_UNKNOWN,
			};

			for ( Sint32 i = 0; FmtTbl[i] != D3DFMT_UNKNOWN; i++ )
			{
				for ( Sint32 j = 0; j < m_AdapterData.TextureCount; j++ )
				{
					if ( FmtTbl[i] == m_AdapterData.TextureFormats[j] )
					{
						return FmtTbl[i];
					}
				}
			}
		}
		break;

		// 32Bitテクスチャフォーマットに準拠
	case FORMAT_TEXTURE32:
		{
			D3DFORMAT FmtTbl[] = {
				D3DFMT_A8R8G8B8, D3DFMT_UNKNOWN,
			};

			for ( Sint32 i = 0; FmtTbl[i] != D3DFMT_UNKNOWN; i++ )
			{
				for ( Sint32 j = 0; j < m_AdapterData.TextureCount; j++ )
				{
					if ( FmtTbl[i] == m_AdapterData.TextureFormats[j] )
					{
						return FmtTbl[i];
					}
				}
			}
		}
		break;

		// 16Bitテクスチャフォーマットに準拠
	case FORMAT_TEXTURE16:
		{
			D3DFORMAT FmtTbl[] = {
				D3DFMT_A4R4G4B4, D3DFMT_A1R5G5B5, D3DFMT_A8R3G3B2, D3DFMT_UNKNOWN,
			};

			for ( Sint32 i = 0; FmtTbl[i] != D3DFMT_UNKNOWN; i++ )
			{
				for ( Sint32 j = 0; j < m_AdapterData.TextureCount; j++ )
				{
					if ( FmtTbl[i] == m_AdapterData.TextureFormats[j] )
					{
						return FmtTbl[i];
					}
				}
			}
		}
		break;

		// DXT1圧縮フォーマットテクスチャ
	case FORMAT_DXT1:
		{
			D3DFORMAT FmtTbl[] = {
				D3DFMT_DXT1, D3DFMT_UNKNOWN,
			};

			for ( Sint32 i = 0; FmtTbl[i] != D3DFMT_UNKNOWN; i++ )
			{
				for ( Sint32 j = 0; j < m_AdapterData.TextureCount; j++ )
				{
					if ( FmtTbl[i] == m_AdapterData.TextureFormats[j] )
					{
						return FmtTbl[i];
					}
				}
			}
		}
		break;

		// DXT3圧縮フォーマットテクスチャ
	case FORMAT_DXT3:
		{
			D3DFORMAT FmtTbl[] = {
				D3DFMT_DXT3, D3DFMT_UNKNOWN,
			};

			for ( Sint32 i = 0; FmtTbl[i] != D3DFMT_UNKNOWN; i++ )
			{
				for ( Sint32 j = 0; j < m_AdapterData.TextureCount; j++ )
				{
					if ( FmtTbl[i] == m_AdapterData.TextureFormats[j] )
					{
						return FmtTbl[i];
					}
				}
			}
		}
		break;

		// DXT5圧縮フォーマットテクスチャ
	case FORMAT_DXT5:
		{
			D3DFORMAT FmtTbl[] = {
				D3DFMT_DXT5, D3DFMT_UNKNOWN,
			};

			for ( Sint32 i = 0; FmtTbl[i] != D3DFMT_UNKNOWN; i++ )
			{
				for ( Sint32 j = 0; j < m_AdapterData.TextureCount; j++ )
				{
					if ( FmtTbl[i] == m_AdapterData.TextureFormats[j] )
					{
						return FmtTbl[i];
					}
				}
			}
		}
		break;

		// ユーザー定義2Dテクスチャ
	case FORMAT_TEXTURE_2D:
		{
			for ( Uint32 i = 0; i < m_SurfaceFormatCount[0]; i++ )
			{
				D3DFORMAT Fmt = GetFormat( m_SurfaceFormatTbl[0][i], D3DFMT_UNKNOWN );
				if ( Fmt != D3DFMT_UNKNOWN )
				{
					return Fmt;
				}
			}
		}
		break;

		// ユーザー定義3Dテクスチャ
	case FORMAT_TEXTURE_3D:
		{
			for ( Uint32 i = 0; i < m_SurfaceFormatCount[1]; i++ )
			{
				D3DFORMAT Fmt = GetFormat( m_SurfaceFormatTbl[1][i], D3DFMT_UNKNOWN );
				if ( Fmt != D3DFMT_UNKNOWN )
				{
					return Fmt;
				}
			}
		}
		break;
	}

	return D3DFMT_UNKNOWN;
}

//=============================================================================
/**
	2D用サーフェイスフォーマット検索テーブルを設定します

	@param FmtTbl	[in] サーフェイスフォーマットテーブル
	@param Count	[in] サーフェイスフォーマットテーブル数
*/
//=============================================================================
void Luna3D::SetSurfaceFormatTblFor2D( eSurfaceFormat FmtTbl[], Uint32 Count )
{
	MemoryCopy( m_SurfaceFormatTbl[0], FmtTbl, sizeof(eSurfaceFormat) * Count );
	m_SurfaceFormatCount[0] = Count;
}

//=============================================================================
/**
	3D用サーフェイスフォーマット検索テーブルを設定します

	@param FmtTbl	[in] サーフェイスフォーマットテーブル
	@param Count	[in] サーフェイスフォーマットテーブル数
*/
//=============================================================================
void Luna3D::SetSurfaceFormatTblFor3D( eSurfaceFormat FmtTbl[], Uint32 Count )
{
	MemoryCopy( m_SurfaceFormatTbl[1], FmtTbl, sizeof(eSurfaceFormat) * Count );
	m_SurfaceFormatCount[1] = Count;
}

//=============================================================================
/**
	深度バッファに使用するサーフェイスを設定します

	@param lSurf	[in] サーフェイスハンドル

	@retval true		成功
	@retval false		失敗
*/
//=============================================================================
Bool Luna3D::SetDepthStencilSurface( LSURFACE lSurf )
{
	if ( lSurf == NULL )
	{
		m_lSurfDepthStencil = NULL;
		Luna3D::GetDevice()->SetDepthStencilSurface( NULL );

		return true;
	}
	else
	{
		m_lSurfDepthStencil = lSurf;
		((CSurface*)lSurf)->SetDepthStencil();

		return true;
	}
}

//=============================================================================
/**
	レンダリングに使用するカメラを設定します

	@param lCam		[in] カメラハンドル

*/
//=============================================================================
void Luna3D::SetCamera( LCAMERA lCam )
{
	if ( lCam == INVALID_CAMERA ) return;

	m_lCamera = lCam;
	LunaCamera::SetDevice( m_lCamera );
}

//=============================================================================
/**
	レンダリングターゲットに使うサーフェイスを設定します

	@param lSurf		[in] サーフェイスハンドル

	@retval true		成功
	@retval false		失敗
*/
//=============================================================================
Bool Luna3D::SetRenderTargetSurface( LSURFACE lSurf )
{
	if ( lSurf != NULL )
	{
		m_lTexTarget = NULL;
		m_lSurfTarget = lSurf;

		if ( ((CSurface*)m_lSurfTarget)->SetRenderTarget() )
		{
			ResetDepthStencil();
		
			return true;
		}
	}
	else
	{
		m_lTexTarget = NULL;
		m_lSurfTarget = NULL;

		RECT View = { 0, 0, 0, 0 };
		Luna::GetScreenData( &View.right, &View.bottom, NULL );

		if ( ResetRenderTarget() )
		{
			SetViewport( &View );
	
			ResetDepthStencil();

			return true;
		}
	}

	return false;
}

//=============================================================================
/**
	レンダリングターゲットに使うテクスチャを設定します

	@retval true		成功
	@retval false		失敗
*/
//=============================================================================
Bool Luna3D::ResetRenderTarget( void )
{
	if SUCCEEDED( m_pDevice->SetRenderTarget( 0, m_pRenderTarget ) )
	{
		ResetDepthStencil();

		return true;
	}

	return false;
}

//=============================================================================
/**
	レンダリングターゲットに使うテクスチャを設定します

	@param pDstRect		[in] 転送先矩形(NULLで全体
	@param lSurf		[in] 転送元サーフェイス
	@param pSrcRect		[in] 転送元矩形(NULLで全体

	@retval true		成功
	@retval false		失敗
*/
//=============================================================================
Bool Luna3D::CopyToRenderTarget( RECT *pDstRect, LSURFACE lSurf, RECT *pSrcRect )
{
	CSurface *pSrc = (CSurface*)lSurf;
	IDirect3DSurface9 *pSrcSurf;
	HRESULT hr;

	pSrc->GetInstance( &pSrcSurf );

	hr = GetDevice()->StretchRect(
						pSrcSurf,
						pSrcRect,
						m_pRenderTarget,
						pDstRect,
						D3DTEXF_NONE );

	pSrcSurf->Release();

	return SUCCEEDED( hr );
}


//=============================================================================
/**
	レンダリングターゲットに使うテクスチャを設定します

	@retval true		成功
	@retval false		失敗
*/
//=============================================================================
Bool Luna3D::ResetDepthStencil( void )
{
	if ( m_lSurfDepthStencil != NULL )
	{
		if ( ((CSurface*)m_lSurfDepthStencil)->SetDepthStencil() )
		{
			SetRenderState( D3DRS_ZENABLE, true );
			SetRenderState( D3DRS_ZWRITEENABLE, true );

			return true;
		}

		SetRenderState( D3DRS_ZENABLE, false );
		SetRenderState( D3DRS_ZWRITEENABLE, false );

		return false;
	}

	SetRenderState( D3DRS_ZENABLE, false );
	SetRenderState( D3DRS_ZWRITEENABLE, false );

	return true;
}

//=============================================================================
/**
	レンダリングターゲットに使うテクスチャを設定します

	@param lTex		[in] テクスチャハンドル

	@retval true		成功
	@retval false		失敗
*/
//=============================================================================
Bool Luna3D::SetRenderTargetTexture( LTEXTURE lTex )
{
	if ( lTex != NULL )
	{
		m_lTexTarget = lTex;
		m_lSurfTarget = NULL;

		if ( ((CTexture*)m_lTexTarget)->SetRenderTarget() )
		{
			RECT View = { 0, 0, ((CTexture*)m_lTexTarget)->GetWidth(), ((CTexture*)m_lTexTarget)->GetHeight() };
			SetViewport( &View );

			ResetDepthStencil();

			return true;
		}
	}
	else
	{
		m_lTexTarget = NULL;
		m_lSurfTarget = NULL;

		RECT View = { 0, 0, 0, 0 };
		Luna::GetScreenData( &View.right, &View.bottom, NULL );

		if ( ResetRenderTarget() )
		{
			SetViewport( &View );
			ResetDepthStencil();

			return true;
		}
	}

	return false;
}

//=============================================================================
/**
	ライトのＯＮ／ＯＦＦを設定します

	@param No			[in] ライト番号
	@param IsActive		[in] true 使用 / false 未使用
*/
//=============================================================================
void Luna3D::SetLight( Sint32 No, Bool IsActive )
{
	LunaLight::SetDevice( No, IsActive );
}

//=============================================================================
/**
	ＤＣ経由のフォント描画を開始します

	@param pFontFace	[in] フォント名
	@param FontSize		[in] フォントサイズ

	@retval true		成功
	@retval false		失敗
*/
//=============================================================================
Bool Luna3D::FontBegin( const char *pFontFace, Sint32 FontSize )
{
	HRESULT hr;

	//----------------------------------------------
	// ＤＣ取得
	//----------------------------------------------
	hr = m_pRenderTarget->GetDC( &m_hDC );
	if FAILED( hr )
	{
		return false;
	}

	//----------------------------------------------
	// フォント生成
	//----------------------------------------------
	m_hFont = ::CreateFont(
						FontSize,					// フォント高さ
						0,							// 文字幅
						0,							// テキストの角度	
						0,							// ベースラインとｘ軸との角度
						0,							// フォントの重さ（太さ）
						false,						// イタリック体
						false,						// アンダーライン
						false,						// 打ち消し線
						SHIFTJIS_CHARSET,			// 文字セット
						OUT_TT_PRECIS,				// 出力精度
						CLIP_DEFAULT_PRECIS,		// クリッピング精度
						PROOF_QUALITY,				// 出力品質
						FIXED_PITCH | FF_MODERN,	// ピッチとファミリー
						pFontFace );				// 書体名

	// フォント設定
	m_hOldFont = (HFONT)::SelectObject( m_hDC, m_hFont );

	return true;
}

//=============================================================================
/**
	ＤＣ経由のフォント描画をします

	@param Px			[] 描画Ｘ位置
	@param Py			[] 描画Ｙ位置
	@param FontColor	[] フォントカラー
	@param BackColor	[] 背景カラー
	@param pStr,...		[] 描画文字列（printf()と同様の書式）
*/
//=============================================================================
void Luna3D::FontDraw( Sint32 Px, Sint32 Py, COLORREF FontColor, COLORREF BackColor, const char *pStr,... )
{
	char Temp[1024] = "";
	vsprintf( Temp, pStr, (char*)(&pStr + 1) );

	// 拝啓色
	::SetBkColor( m_hDC, BackColor );
	::SetBkMode( m_hDC, OPAQUE );

	// フォント色
	::SetTextColor( m_hDC, FontColor );

	// フォント描画
	::TextOut( m_hDC, Px, Py, Temp, (Sint32)strlen(Temp) );
}

//=============================================================================
/**
	ＤＣ経由のフォント描画を終了します

*/
//=============================================================================
void Luna3D::FontEnd( void )
{
	::SelectObject( m_hDC, m_hOldFont );
	::DeleteObject( m_hFont );

	m_pRenderTarget->ReleaseDC( m_hDC );
}

//=============================================================================
/**
	フィルレートのチェックを開始する

*/
//=============================================================================
void Luna3D::FillRateCheckStart( void )
{
	if ( m_pQuery[1] != NULL )
	{
		if ( !m_IsCheckingFillRate )
		{
			m_IsCheckingFillRate = true;
			m_pQuery[1]->Issue( D3DISSUE_BEGIN );
		}
	}
}

//=============================================================================
/**
	フィルレートのチェックを終了する

	@return フィルピクセル数
*/
//=============================================================================
Uint32 Luna3D::FillRateCheckEnd( void )
{
	if ( m_pQuery[1] != NULL )
	{
		if ( m_IsCheckingFillRate )
		{
			m_IsCheckingFillRate = false;
			m_pQuery[1]->Issue( D3DISSUE_END );

			Uint32 Data = 0;
			HRESULT hr = m_pQuery[1]->GetData( &Data, sizeof(Uint32), D3DGETDATA_FLUSH );
			if FAILED( hr )
			{
				LunaDebug::OutputString( Luna3D::GetError( hr ) );
				return 0xFFFFFFFF;
			}
			return Data;
		}
	}

	return 0;
}

//=============================================================================
/**
	レンダリングの終了を待つ

*/
//=============================================================================
Bool Luna3D::BlockForEndRendering( void )
{
	if ( m_pQuery[0] != NULL )
	{
		m_pQuery[0]->Issue( D3DISSUE_END );

		BOOL Data = false;
		return (S_OK == m_pQuery[0]->GetData( &Data, sizeof(BOOL), D3DGETDATA_FLUSH ));
	}
	return false;
}

//=============================================================================
/**
	バックバッファのロックをします

	@param ppBits		[out] バッファのピクセルデータの先頭アドレス格納先
	@param pPitch		[out] サーフェイスのピッチ格納先アドレス
	@param pFormat		[out] サーフェイスのフォーマット格納先アドレス
	@param pWidth		[out] サーフェイスの横幅格納先アドレス
	@param pHeight		[out] サーフェイスの縦幅格納先アドレス

	@retval true		成功
	@retval false		失敗
*/
//=============================================================================
Bool Luna3D::LockBackBuffer( void **ppBits, Sint32 *pPitch, D3DFORMAT *pFormat, Sint32 *pWidth, Sint32 *pHeight )
{
	if ( !m_IsLocked )
	{
		HRESULT hr;
		IDirect3DSurface9 *pBackBuffer = NULL;

		// バックバッファ取得
		hr = m_pDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
		if SUCCEEDED( hr )
		{
			D3DSURFACE_DESC Desc;
			pBackBuffer->GetDesc( &Desc );
			*pFormat = Desc.Format;
			*pWidth = Desc.Width;
			*pHeight = Desc.Height;

			D3DLOCKED_RECT rect;
			HRESULT hr = pBackBuffer->LockRect( &rect, NULL, 0 );
			*pPitch = rect.Pitch;
			*ppBits = rect.pBits;

			m_IsLocked = SUCCEEDED( hr );

			SAFE_RELEASE( pBackBuffer );
		}
	}

	return m_IsLocked;
}

//=============================================================================
/**
	バックバッファのロックを解除します

*/
//=============================================================================
void Luna3D::UnlockBackBuffer( void )
{
	if ( m_IsLocked )
	{
		HRESULT hr;
		IDirect3DSurface9 *pBackBuffer = NULL;

		// バックバッファ取得
		hr = m_pDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
		if SUCCEEDED( hr )
		{
			pBackBuffer->UnlockRect();
			SAFE_RELEASE( pBackBuffer );
			m_IsLocked = false;
		}
	}
}

//=============================================================================
/**
	ワールド座標からスクリーン空間への変換をします

	@param pWorldPos	[in] ワールド空間の座標
	@param pScreenPos	[out] スクリーン上の座標
*/
//=============================================================================
void Luna3D::WorldToScreen( CVector3D *pWorldPos, CVector3D *pScreenPos )
{
	CMatrix mCamera;
	CVector3D vWorld;

	//----------------------------------------------
	// カメラ行列取得
	//----------------------------------------------
	LunaCamera::GetMatrixDirect( NULL, NULL, &mCamera );

	//----------------------------------------------
	// カメラ空間に変換
	//----------------------------------------------
	vWorld.TransformProjection( pWorldPos, &mCamera );

	//----------------------------------------------
	// プロジェクション空間に変換
	//----------------------------------------------
	Float w = toF(m_Viewport.Width  / 2);
	Float h = toF(m_Viewport.Height / 2);
	Float x = toF(m_Viewport.X + w);
	Float y = toF(m_Viewport.Y + h);

	pScreenPos->x = x + (+vWorld.x * w);
	pScreenPos->y = y + (-vWorld.y * h);
}

//=============================================================================
/**
	スクリーン座標からワールド空間への変換をします

	@param pWorldPos	[out] ワールド空間の座標
	@param pScreenPos	[in] スクリーン上の座標
*/
//=============================================================================
void Luna3D::ScreenToWorld( CVector3D *pWorldPos, CVector3D *pScreenPos )
{
	CMatrix mCameraInv;

	//----------------------------------------------
	// カメラ行列取得
	//----------------------------------------------
	LunaCamera::GetMatrixDirect( NULL, NULL, NULL, &mCameraInv );

	//----------------------------------------------
	// プロジェクション空間に変換
	//----------------------------------------------
	Float w = toF(m_Viewport.Width  / 2);
	Float h = toF(m_Viewport.Height / 2);
	Float x = toF(m_Viewport.X + w);
	Float y = toF(m_Viewport.Y + h);
	pWorldPos->x = +toF(pScreenPos->x - x) / w;
	pWorldPos->y = -toF(pScreenPos->y - y) / h;
	pWorldPos->z = pScreenPos->z;

	//----------------------------------------------
	// カメラ空間に変換
	//----------------------------------------------
	pWorldPos->TransformProjection( &mCameraInv );
}

//=============================================================================
/**
	スクリーン座標からビュー空間への変換をします

	@param pViewPos		[out] ビュー空間の座標
	@param pScreenPos	[in] スクリーン上の座標
*/
//=============================================================================
void Luna3D::ScreenToView( CVector3D *pViewPos, CVector3D *pScreenPos )
{
	CMatrix mCamera;

	//----------------------------------------------
	// カメラ行列取得
	//----------------------------------------------
	LunaCamera::GetMatrixDirect( &mCamera, NULL, NULL );

	//----------------------------------------------
	// プロジェクション空間に変換
	//----------------------------------------------
	Float w = toF(m_Viewport.Width  / 2);
	Float h = toF(m_Viewport.Height / 2);
	Float x = toF(m_Viewport.X + w);
	Float y = toF(m_Viewport.Y + h);
	pViewPos->x = +toF(pScreenPos->x - x) / w;
	pViewPos->y = -toF(pScreenPos->y - y) / h;
	pViewPos->z = pScreenPos->z;

	//----------------------------------------------
	// カメラ空間に変換
	//----------------------------------------------
	mCamera.Inverse();
	pViewPos->TransformProjection( &mCamera );
}

//=============================================================================
/**
	指定矩形をクリッピングします

	@param pDst		[in/out] 転送先矩形
	@param pSrc		[in/out] 転送元矩形

	@retval true	描画可能
	@retval false	描画不要
*/
//=============================================================================
Bool Luna3D::Clip( RECT *pDst, RECT *pSrc )
{
	// ビューポート
	Sint32 x1 = m_Viewport.X;
	Sint32 y1 = m_Viewport.Y;
	Sint32 x2 = x1 + m_Viewport.Width;
	Sint32 y2 = y1 + m_Viewport.Height;

	// 転送先矩形
	Sint32 dx1 = pDst->left;
	Sint32 dy1 = pDst->top;
	Sint32 dx2 = pDst->right;
	Sint32 dy2 = pDst->bottom;

	// 転送元矩形
	Sint32 sx1 = pSrc->left;
	Sint32 sy1 = pSrc->top;
	Sint32 sx2 = pSrc->right;
	Sint32 sy2 = pSrc->bottom;

	// サイズ比率
	Float fx = toF(sx2 - sx1) / toF(dx2 - dx1);
	Float fy = toF(sy2 - sy1) / toF(dy2 - dy1);

	// 左チェック
	if ( dx1 < x1 )
	{
		sx1 = sx1 + toI(toF(x1 - dx1) * fx);
		dx1 = x1;
	}
	// 上チェック
	if ( dy1 < y1 )
	{
		sy1 = sy1 + toI(toF(y1 - dy1) * fy);
		dy1 = y1;
	}
	// 右チェック
	if ( dx2 > x2 )
	{
		sx2 = sx2 - toI(toF(dx2 - x2) * fx);
		dx2 = x2;
	}
	// 下チェック
	if ( dy2 > y2 )
	{
		sy2 = sy2 - toI(toF(dy2 - y2) * fy);
		dy2 = y2;
	}

	// 転送先矩形
	pDst->left		= dx1;
	pDst->top		= dy1;
	pDst->right		= dx2;
	pDst->bottom	= dy2;

	// 転送元矩形
	pSrc->left		= sx1;
	pSrc->top		= sy1;
	pSrc->right		= sx2;
	pSrc->bottom	= sy2;

	return ((dx1 < dx2) && (dy1 < dy2));
}

//=============================================================================
/**
	使用可能な推定テクスチャメモリ容量を取得します。

	@return		メモリ容量(MByte)
*/
//=============================================================================
Uint32 Luna3D::GetRestVRAM( void )
{
	return m_pDevice->GetAvailableTextureMem() / 1024 / 1024;
}

//=============================================================================
/**
	D3DFORMAT型の示すピクセルフォーマットを文字列として取得します

	@param Format	[in] ピクセルフォーマット識別用定数
	@param pStr		[out] 文字列格納先

	@return		ピクセルフォーマット文字列のポインタ
*/
//=============================================================================
char *Luna3D::GetPixelFormat( D3DFORMAT Format, char *pStr )
{
	static char String[MAX_PATH];

	switch ( Format )
	{
	case D3DFMT_UNKNOWN:			strcpy( String, "UNKNOWN" );		break;
	case D3DFMT_R8G8B8:				strcpy( String, "R8G8B8" );		break;
	case D3DFMT_A8R8G8B8:			strcpy( String, "A8R8G8B8" );		break;
	case D3DFMT_X8R8G8B8:			strcpy( String, "X8R8G8B8" );		break;
	case D3DFMT_R5G6B5:				strcpy( String, "R5G6B5" );		break;
	case D3DFMT_X1R5G5B5:			strcpy( String, "X1R5G5B5" );		break;
	case D3DFMT_A1R5G5B5:			strcpy( String, "A1R5G5B5" );		break;
	case D3DFMT_A4R4G4B4:			strcpy( String, "A4R4G4B4" );		break;
	case D3DFMT_R3G3B2:				strcpy( String, "R3G3B2" );		break;
	case D3DFMT_A8:					strcpy( String, "A8" );			break;
	case D3DFMT_A8R3G3B2:			strcpy( String, "A8R3G3B2" );		break;
	case D3DFMT_X4R4G4B4:			strcpy( String, "X4R4G4B4" );		break;
	case D3DFMT_A2B10G10R10:		strcpy( String, "A2B10G10R10" );	break;
	case D3DFMT_A8B8G8R8:			strcpy( String, "A8B8G8R8" );		break;
	case D3DFMT_X8B8G8R8:			strcpy( String, "X8B8G8R8" );		break;
	case D3DFMT_G16R16:				strcpy( String, "G16R16" );		break;
	case D3DFMT_A2R10G10B10:		strcpy( String, "A2R10G10B10" );	break;
	case D3DFMT_A16B16G16R16:		strcpy( String, "A16B16G16R16" );	break;
	case D3DFMT_A8P8:				strcpy( String, "A8P8" );			break;
	case D3DFMT_P8:					strcpy( String, "P8" );			break;
	case D3DFMT_L8:					strcpy( String, "L8" );			break;
	case D3DFMT_L16:				strcpy( String, "L16" );			break;
	case D3DFMT_A8L8:				strcpy( String, "A8L8" );			break;
	case D3DFMT_A4L4:				strcpy( String, "A4L4" );			break;
	case D3DFMT_V8U8:				strcpy( String, "V8U8" );			break;
	case D3DFMT_Q8W8V8U8:			strcpy( String, "Q8W8V8U8" );		break;
	case D3DFMT_V16U16:				strcpy( String, "V16U16" );		break;
	case D3DFMT_Q16W16V16U16:		strcpy( String, "Q16W16V16U16" );	break;
	case D3DFMT_CxV8U8:				strcpy( String, "CxV8U8" );		break;
	case D3DFMT_L6V5U5:				strcpy( String, "L6V5U5" );		break;
	case D3DFMT_X8L8V8U8:			strcpy( String, "X8L8V8U8" );		break;
	case D3DFMT_A2W10V10U10:		strcpy( String, "A2W10V10U10" );	break;
	case D3DFMT_G8R8_G8B8:			strcpy( String, "G8R8_G8B8" );		break;
	case D3DFMT_R8G8_B8G8:			strcpy( String, "R8G8_B8G8" );		break;
	case D3DFMT_DXT1:				strcpy( String, "DXT1" );			break;
	case D3DFMT_DXT2:				strcpy( String, "DXT2" );			break;
	case D3DFMT_DXT3:				strcpy( String, "DXT3" );			break;
	case D3DFMT_DXT4:				strcpy( String, "DXT4" );			break;
	case D3DFMT_DXT5:				strcpy( String, "DXT5" );			break;
	case D3DFMT_UYVY:				strcpy( String, "UYVY" );			break;
	case D3DFMT_YUY2:				strcpy( String, "YUY2" );			break;
	case D3DFMT_D16_LOCKABLE:		strcpy( String, "D16_LOCKABLE" );	break;
	case D3DFMT_D32:				strcpy( String, "D32" );			break;
	case D3DFMT_D15S1:				strcpy( String, "D15S1" );			break;
	case D3DFMT_D24S8:				strcpy( String, "D24S8" );			break;
	case D3DFMT_D24X8:				strcpy( String, "D24X8" );			break;
	case D3DFMT_D24X4S4:			strcpy( String, "D24X4S4" );		break;
	case D3DFMT_D32F_LOCKABLE:		strcpy( String, "D32F_LOCKABLE" );	break;
	case D3DFMT_D24FS8:				strcpy( String, "D24FS8" );		break;
	case D3DFMT_D16:				strcpy( String, "D16" );			break;
	case D3DFMT_VERTEXDATA:			strcpy( String, "VERTEXDATA" );	break;
	case D3DFMT_INDEX16:			strcpy( String, "INDEX16" );		break;
	case D3DFMT_INDEX32:			strcpy( String, "INDEX32" );		break;
	case D3DFMT_R16F:				strcpy( String, "R16F" );			break;
	case D3DFMT_G16R16F:			strcpy( String, "G16R16F" );		break;
	case D3DFMT_A16B16G16R16F:		strcpy( String, "A16B16G16R16F" );	break;
	case D3DFMT_R32F:				strcpy( String, "R32F" );			break;
	case D3DFMT_G32R32F:			strcpy( String, "G32R32F" );		break;
	case D3DFMT_A32B32G32R32F:		strcpy( String, "A32B32G32R32F" );	break;
	}

	if ( pStr != NULL ) strcpy( pStr, String );

	return String;
}

//=============================================================================
/**
	HRESULT型の示すDirect3D関連のエラーを文字列として取得します

	@param hr	[in] エラー識別用定数

	@return		エラー文字列のポインタ
*/
//=============================================================================
char *Luna3D::GetError( HRESULT hr )
{
	static char String[MAX_PATH] = "";

	switch ( hr )
	{
	case D3D_OK:
		strcpy( String, "エラーは発生していない。" );
		break;
	case D3DERR_CONFLICTINGRENDERSTATE:
		strcpy( String, "現在設定されているレンダリング ステートは一緒に使用できない。" );
		break;
	case D3DERR_CONFLICTINGTEXTUREFILTER:
		strcpy( String, "現在のテクスチャ フィルタは一緒に使用できない。" );
		break;
	case D3DERR_CONFLICTINGTEXTUREPALETTE:
		strcpy( String, "現在のテクスチャは同時に使用できない。一般にマルチテクスチャ デバイスにおいて、同時に有効にされたパレット化テクスチャで同じパレットを共有する必要がある場合に発生する。" );
		break;
	case D3DERR_DEVICELOST:
		strcpy( String, "デバイスが失われていて、現時点では復元できないため、レンダリングは不可能である。" );
		break;
	case D3DERR_DEVICENOTRESET:
		strcpy( String, "デバイスはリセットできない。" );
		break;
	case D3DERR_DRIVERINTERNALERROR:
		strcpy( String, "内部ドライバ エラー。" );
		break;
	case D3DERR_INVALIDCALL:
		strcpy( String, "メソッドの呼び出しが無効である。たとえば、メソッドのパラメータに無効な値が設定されている場合など。" );
		break;
	case D3DERR_INVALIDDEVICE:
		strcpy( String, "要求されたデバイスの種類が有効でない。" );
		break;
	case D3DERR_MOREDATA:
		strcpy( String, "指定されたバッファ サイズに保持できる以上のデータが存在する。" );
		break;
	case D3DERR_NOTAVAILABLE:
		strcpy( String, "このデバイスは、照会されたテクニックをサポートしていない。" );
		break;
	case D3DERR_NOTFOUND:
		strcpy( String, "要求された項目が見つからなかった。" );
		break;
	case D3DERR_OUTOFVIDEOMEMORY:
		strcpy( String, "Direct3D が処理を行うのに十分なディスプレイ メモリがない。" );
		break;
	case D3DERR_TOOMANYOPERATIONS:
		strcpy( String, "デバイスがサポートしている以上のテクスチャ フィルタリング処理を、アプリケーションが要求している。" );
		break;
	case D3DERR_UNSUPPORTEDALPHAARG:
		strcpy( String, "アルファ チャネルに対して指定されているテクスチャ ブレンディング引数を、デバイスがサポートしていない。" );
		break;
	case D3DERR_UNSUPPORTEDALPHAOPERATION:
		strcpy( String, "アルファ チャネルに対して指定されているテクスチャ ブレンディング処理を、デバイスがサポートしていない。" );
		break;
	case D3DERR_UNSUPPORTEDCOLORARG:
		strcpy( String, "カラー値に対して指定されているテクスチャ ブレンディング引数を、デバイスがサポートしていない。" );
		break;
	case D3DERR_UNSUPPORTEDCOLOROPERATION:
		strcpy( String, "カラー値に対して指定されているテクスチャ ブレンディング処理を、デバイスがサポートしていない。" );
		break;
	case D3DERR_UNSUPPORTEDFACTORVALUE:
		strcpy( String, "デバイスが指定されたテクスチャ係数値をサポートしていない。" );
		break;
	case D3DERR_UNSUPPORTEDTEXTUREFILTER:
		strcpy( String, "デバイスが指定されたテクスチャ フィルタをサポートしていない。" );
		break;
	case D3DERR_WRONGTEXTUREFORMAT:
		strcpy( String, "テクスチャ サーフェスのピクセル フォーマットが有効でない。" );
		break;
	case E_FAIL:
		strcpy( String, "Direct3D サブシステム内で原因不明のエラーが発生した。" );
		break;
	case E_INVALIDARG:
		strcpy( String, "無効なパラメータが戻ってくる関数に渡された。" );
		break;
	case E_OUTOFMEMORY:
		strcpy( String, "Direct3D が呼び出しを完了するための十分なメモリを割り当てることができなかった。" );
		break;
	case D3DXERR_CANNOTATTRSORT:
		strcpy( String, "最適化テクニックとして属性のソート (D3DXMESHOPT_ATTRSORT) はサポートされていない。 " );
		break;
	case D3DXERR_CANNOTMODIFYINDEXBUFFER:
		strcpy( String, "インデックス バッファを変更できない。 " );
		break;
	case D3DXERR_INVALIDMESH:
		strcpy( String, "メッシュが無効である。 " );
		break;
	case D3DXERR_SKINNINGNOTSUPPORTED:
		strcpy( String, "スキニングはサポートされていない。 " );
		break;
	case D3DXERR_TOOMANYINFLUENCES:
		strcpy( String, "指定された影響が多すぎる。 " );
		break;
	case D3DXERR_INVALIDDATA:
		strcpy( String, "データが無効である。" );
		break;
	default:
		strcpy( String, "未知のエラー。" );
		break;
	}

	return String;
}

//=============================================================================
/**
	画面モード数の取得をします

	@param IsFullColor	[in] フルカラー画面かどうか

	@return		モード数
*/
//=============================================================================
Sint32 Luna3D::GetScreenModeCount( Bool IsFullColor )
{
	Sint32 Count = 0;

	if ( IsFullColor )
	{
		for ( Sint32 i = 0; i < m_AdapterData.ModeCount; i++ )
		{
			if ( m_AdapterData.Mode[i].Format == m_FullScreenMode[1].Format )
			{
				Count++;
			}
		}
	}
	else
	{
		for ( Sint32 i = 0; i < m_AdapterData.ModeCount; i++ )
		{
			if ( m_AdapterData.Mode[i].Format == m_FullScreenMode[0].Format )
			{
				Count++;
			}
		}
	}

	return Count;
}

//=============================================================================
/**
	画面モードの取得をします

	@param IsFullColor	[in] フルカラー画面かどうか
	@param No			[in] 画面サイズ番号
	@param pWidth		[out] 画面横幅
	@param pHeight		[out] 画面縦幅

*/
//=============================================================================
void Luna3D::GetScreenMode( Bool IsFullColor, Sint32 No, Sint32 *pWidth, Sint32 *pHeight )
{
	if ( IsFullColor )
	{
		for ( Sint32 i = 0; i < m_AdapterData.ModeCount; i++ )
		{
			if ( m_AdapterData.Mode[i].Format == m_FullScreenMode[1].Format )
			{
				if ( No-- == 0 )
				{
					*pWidth = m_AdapterData.Mode[i].Width;
					*pHeight = m_AdapterData.Mode[i].Height;
					return;
				}
			}
		}
	}
	else
	{
		for ( Sint32 i = 0; i < m_AdapterData.ModeCount; i++ )
		{
			if ( m_AdapterData.Mode[i].Format == m_FullScreenMode[0].Format )
			{
				if ( No-- == 0 )
				{
					*pWidth = m_AdapterData.Mode[i].Width;
					*pHeight = m_AdapterData.Mode[i].Height;
					return;
				}
			}
		}
	}

	*pWidth = 0;
	*pHeight = 0;
}

//=============================================================================
/**
	レンダリングターゲットテクスチャサイズ数の取得をします

    @return		テクスチャサイズ数
*/
//=============================================================================
Sint32 Luna3D::GetRenderTargetTextureSizeCount( void )
{
	Sint32 Count = 0;

	if ( m_DeviceInfo.MaxTextureWidth <  m_DeviceInfo.MaxTextureHeight )
	{
		for ( Uint32 i = 32; i <= m_DeviceInfo.MaxTextureWidth; i <<= 1 ) Count++;
	}
	else
	{
		for ( Uint32 i = 32; i <= m_DeviceInfo.MaxTextureHeight; i <<= 1 ) Count++;
	}

	return Count;
}

//=============================================================================
/**
	レンダリングターゲットテクスチャサイズの取得をします

	@param No			[in] 画面サイズ番号
	@param pWidth		[out] 画面横幅
	@param pHeight		[out] 画面縦幅

*/
//=============================================================================
void Luna3D::GetRenderTargetTextureSize( Sint32 No, Sint32 *pWidth, Sint32 *pHeight )
{
	Sint32 Count = 0;

	if ( m_DeviceInfo.MaxTextureWidth <  m_DeviceInfo.MaxTextureHeight )
	{
		for ( Uint32 i = 32; i <= m_DeviceInfo.MaxTextureWidth; i <<= 1 )
		{
			if ( Count++ == No )
			{
				*pWidth = *pHeight = i;
				return;
			}
		}
	}
	else
	{
		for ( Uint32 i = 32; i <= m_DeviceInfo.MaxTextureHeight; i <<= 1 )
		{
			if ( Count++ == No )
			{
				*pWidth = *pHeight = i;
				return;
			}
		}
	}

	*pWidth = *pHeight = 0;
}

//=============================================================================
/**
	アダプター名の取得をします

	@param pName	[out] 取得した文字列の格納先

*/
//=============================================================================
void Luna3D::GetAdapterName( char *pName )
{
	strcpy( pName, m_AdapterName );
}

//=============================================================================
/**
	アダプタードライバーの取得をします

	@param pName	[out] 取得した文字列の格納先

*/
//=============================================================================
void Luna3D::GetAdapterDriver( char *pName )
{
	strcpy( pName, m_AdapterDriver );
}

//=============================================================================
/**
	アダプタードライバーバージョンの取得をします

	@param pName	[out] 取得した文字列の格納先

*/
//=============================================================================
void Luna3D::GetAdapterVersion( char *pName )
{
	strcpy( pName, m_AdapterVersion );
}

//=============================================================================
/**
	ダイアログボックスモードを使用するかどうか

	@param IsMode	[in] 使用するか否か

*/
//=============================================================================
void Luna3D::SetDialogMode( Bool IsMode )
{
	if ( m_pDevice != NULL )
	{
		m_pDevice->SetDialogBoxMode( IsMode );
	}
}

//=============================================================================
/**
	デフォルトのモデルマテリアルステートを取得

	@param pState	[in] ステート

*/
//=============================================================================
void Luna3D::GetDefaultMateriaState( MATERIALSTATE *pState )
{
	*pState = m_DefaultState;
}

//=============================================================================
/**
	デフォルトのモデルマテリアルステートを取得

	@param pState	[in] ステート

*/
//=============================================================================
void Luna3D::SetMateriaRenderState( MATERIALSTATE *pState )
{
	Luna3D::SetBlendingType( pState->BlendType );
	Luna3D::SetBilinearEnable( pState->IsBilinear );
	Luna3D::SetColorkeyEnable( pState->IsColorKey );
	Luna3D::SetZBufferEnable( pState->IsZBuffer );
	Luna3D::SetZWriteEnable( pState->IsZWrite );

	for ( Uint32 i = 0; i < TEXTURE_STAGE_MAX; i++ )
	{
		for ( Uint32 j = 0; j < TextureStateTblCount; j++ )
		{
			Luna3D::SetTextureState( i, TextureStateTbl[j], pState->TextureState[i][TextureStateTbl[j]] );
		}

		for ( Uint32 j = 0; j < SamplerStateTblCount; j++ )
		{
			Luna3D::SetSamplerState( i, SamplerStateTbl[j], pState->SamplerState[i][SamplerStateTbl[j]] );
		}
	}
}

