

//=============================================================================
// INCLUDE
//=============================================================================
#include "LunaBase.h"

#include "../LunaMain.h"
#include "../Luna3D.h"
#include "../LunaTexture.h"

#include "VMR9.h"
#include "VMR9Allocator.h"

using namespace LibLuna;


//=============================================================================
/**
	@brief コンストラクタ
*/
//=============================================================================
CVMR9::CVMR9( CBaseMgr *pMgr ) : CDeviceObject( pMgr )
{
	m_pGraph		= NULL;
	m_pFilter		= NULL;
	m_pMediaControl	= NULL;
	m_pAllocator	= NULL;
	m_pAllocatorMgr	= new CBaseMgr( Luna3D::GetDevice() );
}


//=============================================================================
/**
	@brief デストラクタ
*/
//=============================================================================
CVMR9::~CVMR9()
{
	Luna::LogoutError( "CVMR9 Destruct [%s]", m_Name );
	Luna::LogoutNextLine();

	SAFE_RELEASE( m_pAllocatorMgr );
	SAFE_RELEASE( m_pGraph );
	SAFE_RELEASE( m_pFilter );
	SAFE_RELEASE( m_pMediaControl );
	SAFE_RELEASE( m_pAllocator );
}

//=============================================================================
/**
*/
//=============================================================================
void CVMR9::Reset( void )
{
}


//=============================================================================
/**
*/
//=============================================================================
void CVMR9::Restore( void )
{
}

//=============================================================================
/**
*/
//=============================================================================
HRESULT CVMR9::SetAllocatorPresenter( IBaseFilter *pFilter, HWND hWnd )
{
	HRESULT hr;
	IVMRSurfaceAllocatorNotify9 *pIVMRSurfAllocNotify;

	hr = pFilter->QueryInterface( IID_IVMRSurfaceAllocatorNotify9, (void**)&pIVMRSurfAllocNotify );
	if FAILED( hr ) return hr;

	m_pAllocator = new CVMR9Allocator( m_pAllocatorMgr );

	hr = pIVMRSurfAllocNotify->AdviseSurfaceAllocator( 0xACDCACDC, m_pAllocator );
	if FAILED( hr ) return hr;

	hr = m_pAllocator->AdviseNotify( pIVMRSurfAllocNotify );
	if FAILED( hr ) return hr;

	return hr;
}


//=============================================================================
/**
*/
//=============================================================================
Bool CVMR9::LoadFromFile( const char *pFile )
{
	HRESULT hr;
	WCHAR wFile[MAX_PATH] = { 0 };
    IVMRFilterConfig9 *pFilterConfig = NULL;

	Stop();
	SAFE_RELEASE( m_pGraph );
	SAFE_RELEASE( m_pFilter );
	SAFE_RELEASE( m_pMediaControl );
	SAFE_RELEASE( m_pAllocator );

	//--------------------------------------------------------------
	// カレントディレクトリをリセット
	//--------------------------------------------------------------
	Luna::ResetCurrentDirectory();

	hr = ::CoCreateInstance( CLSID_FilterGraph, NULL, CLSCTX_ALL, IID_IGraphBuilder, (void**)&m_pGraph );
	if FAILED( hr ) goto EXIT;

	// hr = ::CoCreateInstance( CLSID_VideoMixingRenderer9, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&m_pFilter );
	// if FAILED( hr ) goto EXIT;

	hr = m_pFilter->QueryInterface( IID_IVMRFilterConfig9, (void**)&pFilterConfig );
	if FAILED( hr ) goto EXIT;

	hr = pFilterConfig->SetRenderingMode( VMR9Mode_Renderless );
	if FAILED( hr ) goto EXIT;

    hr = pFilterConfig->SetNumberOfStreams( 2 );
	if FAILED( hr ) goto EXIT;

	hr = SetAllocatorPresenter( m_pFilter, Luna::GetWindowHandle() );
	if FAILED( hr ) goto EXIT;

    hr = m_pGraph->AddFilter( m_pFilter, L"Video Mixing Renderer 9" );
	if FAILED( hr ) goto EXIT;

	hr = m_pGraph->QueryInterface( IID_IMediaControl, (void**)&m_pMediaControl );
	if FAILED( hr ) goto EXIT;

	MultiByteToWideChar( CP_ACP, 0, pFile, -1, wFile, MAX_PATH );
    hr = m_pGraph->RenderFile( wFile, NULL );
	if FAILED( hr ) goto EXIT;

EXIT:
	SAFE_RELEASE( pFilterConfig );

	return SUCCEEDED( hr );
}

//=============================================================================
/**
*/
//=============================================================================
Bool CVMR9::Play( void )
{
	if ( m_pMediaControl == NULL ) return false;

	return SUCCEEDED( m_pMediaControl->Run() );
}

//=============================================================================
/**
*/
//=============================================================================
void CVMR9::Stop( void )
{
	if ( m_pMediaControl == NULL ) return;

	OAFilterState State;
	do
	{
		m_pMediaControl->Stop();
		m_pMediaControl->GetState( 0, &State );
	}
	while ( State != State_Stopped );
}

//=============================================================================
/**
*/
//=============================================================================
Bool CVMR9::IsPlay( void )
{
	if ( m_pMediaControl == NULL ) return false;

	OAFilterState State;
	m_pMediaControl->GetState( 0, &State );

	return State != State_Stopped;
}

//=============================================================================
/**
*/
//=============================================================================
void CVMR9::Render( CLunaRect *pDst )
{
	CLunaRect Dst = { 0, 0 };
	CLunaRect Src = { 0, 0 };

	CTexture *pTex = (CTexture*)m_pAllocator;

	Src.Sx = toF(m_pAllocator->GetMovieWidth());
	Src.Sy = toF(m_pAllocator->GetMovieHeight());

	if ( pDst == NULL )
	{
		Sint32 Width, Height;
		Luna::GetScreenData( &Width, &Height, NULL );
		Dst.Sx = toF(Width);
		Dst.Sy = toF(Height);
	}
	else
	{
		Dst = *pDst;
	}

	LunaTexture::Draw( (LTEXTURE)(Uint32)pTex, &Dst, &Src, COLOR_BASE );
}

//=============================================================================
/**
*/
//=============================================================================
CTexture *CVMR9::GetTexture( void )
{
	if ( m_pAllocator != NULL )
	{
		m_pAllocator->AddRef();
		return (CTexture*)m_pAllocator;
	}

	return NULL;
}

