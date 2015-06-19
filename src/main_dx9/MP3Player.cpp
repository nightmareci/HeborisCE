// DirectShowによるBGM再生
// 面倒くさかったのでクラスまみれのゲームプログラミング入門から
// 拝借しました。ありがとうございました。
// http://www.tnksoft.com/reading/classgame/

#include "MP3Player.h"

void CMP3Player::InitializeBGM()
{
	pBuilder = NULL;
	pMediaCtrl = NULL;
	pMediaSeeking = NULL;
	pBasicAudio = NULL;

	bLoop = FALSE;
	bFading = FALSE;
	bPlaying = FALSE;
}

void CMP3Player::Release()
{
	Stop();

	RELEASE(pBasicAudio);
	RELEASE(pMediaSeeking);
	RELEASE(pMediaCtrl);
	RELEASE(pBuilder);
}

CMP3Player::CMP3Player()
{
	// InitializeBGM();
}

CMP3Player::~CMP3Player()
{
	Release();
}

void CMP3Player::Load(LPCTSTR filename)
{
	Release();

	HRESULT hr = CoCreateInstance(
		CLSID_FilterGraph,		// COMの識別子
		NULL,					// 特別に取得する情報はないのでNULL
		CLSCTX_INPROC_SERVER,	// このプログラム内でのみ使うことを指定
		IID_IGraphBuilder,		// 取得するインターフェース
		(LPVOID*)&pBuilder);	// 格納先

	if(FAILED(hr)){
		// DXTRACE_MSG(_T("IGraphBuilderの作成に失敗しました"));
		return;
	}

#ifdef _UNICODE
	pBuilder->RenderFile(filename, NULL);
#else
	wchar_t wfname[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, filename, -1, wfname, MAX_PATH);
	pBuilder->RenderFile(wfname, NULL);
#endif

	pBuilder->QueryInterface(IID_IMediaControl, (LPVOID*)&pMediaCtrl);
	pBuilder->QueryInterface(IID_IMediaSeeking, (LPVOID*)&pMediaSeeking);
	pBuilder->QueryInterface(IID_IBasicAudio, (LPVOID*)&pBasicAudio);
}

void CMP3Player::Play(bool loop)
{
	if(pMediaCtrl){
		bLoop = loop;	// ループ再生を行うかのフラグ
		if(pMediaSeeking){
			// 再生位置を先頭に移動する
			LONGLONG s = 0;
			pMediaSeeking->SetPositions(
				// 再生位置は絶対位置で(s * 100)ナノ秒
				&s, AM_SEEKING_AbsolutePositioning,
				// 終了位置は変更しないのでNoPositioningを指定
				NULL, AM_SEEKING_NoPositioning);
		}
		pMediaCtrl->Run();
		bPlaying = TRUE;
	}
}

void CMP3Player::Stop()
{
	if(pMediaCtrl)
	{
		pMediaCtrl->Stop();
		bPlaying = FALSE;
	}
}

void CMP3Player::SetVolume(LONG volume)
{
	if ( pBasicAudio )
	{
		/* なぜか-3000ぐらいですごい小さい音になるので補正する */
		if ( volume > -8500 )
		{
			/* 0 ～ -8500 → 0 ～ -1500 */
			volume = volume * 1500 / 8500;
		}
		else
		{
			/* -8501 ～ -10000 → -1500 ～ -10000 */
			volume += 8500;						/* 0 ～ -1500に */
			volume = volume * 8500 / 1500;		/* 0 ～ -8500に */
			volume -= 1500;						/* -1500 ～ -10000 に */
		}

		pBasicAudio->put_Volume(volume);
	}
}

void CMP3Player::Exec()
{
	if(bLoop == TRUE && pMediaSeeking){
		LONGLONG s, e;
		pMediaSeeking->GetPositions(&s, &e);	// sは開始位置、eは終了位置
		if(s >= e){
			s = 0;
			pMediaSeeking->SetPositions(
				&s, AM_SEEKING_AbsolutePositioning,
				NULL, AM_SEEKING_NoPositioning);
		}
	}

	if(bFading == TRUE){
		// 差分で経過時間を取得
		float t = (float)timeGetTime() - time_s;
		pBasicAudio->put_Volume((long)(delta * t + vol_s));

		// 経過時間を超えたらフェード処理終了
		if(t >= time_e)
		{
			bFading = FALSE;
			bPlaying = FALSE;
		}
	}
}

void CMP3Player::Fade(DWORD endtime, LONG volume)
{
	if(!pBasicAudio){
		// DXTRACE_MSG(_T("ボリュームの設定ができません。"));
		return;
	}

	bFading = TRUE;

	LONG v;
	pBasicAudio->get_Volume(&v);

	vol_s = (float)v;
	float vol_e = (float)volume;

	time_s = (float)timeGetTime();
	time_e = (float)endtime;

	delta = (vol_e - vol_s) / time_e;
}

bool CMP3Player::IsPlay()
{
	if ( pMediaSeeking && bPlaying )
	{
		LONGLONG s, e;
		pMediaSeeking->GetPositions(&s, &e);	// sは開始位置、eは終了位置
		if ( s < e )
		{
			return true;
		}
	}
	return false;
}
