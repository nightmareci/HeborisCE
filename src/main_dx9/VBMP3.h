//
// vbmp3.h
//   VBMP3.DLL 用ヘッダファイル
//

#define INFO_NAME_SIZE		128

typedef void (__stdcall *INPUT_MSG_PROC)(int msg, int param);

struct InputInfo
{
	char	szTrackName[INFO_NAME_SIZE];	//曲名
	char	szArtistName[INFO_NAME_SIZE];	//アーティスト名
	int		channels;						//チャンネル数
	int		bitRate;						//ビットレート
	int		samplingRate;					//サンプリングレート
	int		totalSec;						//総合演奏時間
};

struct TAG_INFO
{
	char	szTrackName[INFO_NAME_SIZE];	//曲名
	char	szArtistName[INFO_NAME_SIZE];	//アーティスト名
	char    szAlbumName[INFO_NAME_SIZE];	//アルバム名
	char	szYear[5];						//リリース年号
	char	szComment[INFO_NAME_SIZE];		//コメント文字列
	int		genre;							//ジャンル
	char    szGanreName[INFO_NAME_SIZE];	//ジャンル名称
};

struct MPEG_INFO
{
	int		version;		//MPEGのバージョン
	int		layer;			//MPEGのレイヤ
	int		crcDisable;		//エラー保護
	int		extension;		//個人情報
	int		mode;			//チャンネルモード
	int		copyright;		//著作権
	int		original;		//オリジナル
	int		emphasis;		//エンフォシス		

	int		channels;		//出力チャンネル数
	int		bitRate;		//ビットレート
	int		samplingRate;	//サンプリングレート
	int		fileSize;		//ファイルサイズ
	int		flames;			//フレーム数
	int		totalSec;		//総合演奏時間
};

struct LYRICS_INFO
{
	int		sec;
	char	LyricsNext2[INFO_NAME_SIZE];
	char	LyricsNext1[INFO_NAME_SIZE];
	char	LyricsCurrent[INFO_NAME_SIZE];
	char	LyricsPrev1[INFO_NAME_SIZE];
	char	LyricsPrev2[INFO_NAME_SIZE];
};

struct DEC_OPTION{
	int		reduction;		//サンプリング 0:1/1 1:1/2 2:1/4 (Default = 0)
	int		convert;		//チャンネル 0:ステレオ 1:モノラル(Default = 0)
	int		freqLimit;		//周波数
};

struct VBMP3_OPTION
{
	int inputBlock;			//入力フレーム数
	int outputBlock;		//WAVEデータブロック数
	int inputSleep;			//入力直後のスリープ時間(ミリ秒)
	int outputSleep;		//出力直後のスリープ時間(ミリ秒)
};

struct LIST_INFO
{
	char	INAM[INFO_NAME_SIZE];	//曲名
	char	IART[INFO_NAME_SIZE];	//アーティスト名
	char    IPRD[INFO_NAME_SIZE];	//製品名
	char	ICMT[INFO_NAME_SIZE];	//コメント文字列
	char	ICRD[INFO_NAME_SIZE];	//リリース年号
	char	IGNR[INFO_NAME_SIZE];	//ジャンル名
	char    ICOP[INFO_NAME_SIZE];	//著作権
	char    IENG[INFO_NAME_SIZE];	//エンジニア
	char    ISRC[INFO_NAME_SIZE];	//ソース
	char    ISFT[INFO_NAME_SIZE];	//ソフトウェア
	char    IKEY[INFO_NAME_SIZE];	//キーワード
	char    ITCH[INFO_NAME_SIZE];	//技術者
	char    ILYC[INFO_NAME_SIZE];	//歌詞
	char    ICMS[INFO_NAME_SIZE];	//コミッション
};

struct WAVE_DATA
{
	int		channels;
	int		bitsPerSample;
	int		left;
	int		right;
};

struct WAVE_FORM
{
	int		channels;			//チャネル数
	int		bitsPerSample;		//ビット数/1サンプル
	int		samplingRate;
	int		dataSize;			//ファイルサイズ
};

//エンコード系
int APIENTRY  vbmp3_getEncodeState(int* readSize, int* encodeSize);
BOOL APIENTRY vbmp3_encodeOpen(const char* pszWaveName, WAVE_FORM* pWaveForm);
BOOL APIENTRY vbmp3_encodeStart(const char* pszMp3Name);
BOOL APIENTRY vbmp3_encodeStop();

//未使用
void APIENTRY vbmp3_startAnalyze();
void APIENTRY vbmp3_stopAnalyze();
BOOL APIENTRY vbmp3_getWaveData(WAVE_DATA* pWaveData);

//情報系
BOOL APIENTRY vbmp3_setLyricsFile(const char* pszLyricsName);
BOOL APIENTRY vbmp3_getLyrics(LYRICS_INFO* pLyricsInfo);
void APIENTRY vbmp3_getSpectrum(int* pSpecL, int* pSpecR);
void APIENTRY vbmp3_getWave(int* pWaveL, int* pWaveR);

//コールバック関数
BOOL APIENTRY vbmp3_callback(INPUT_MSG_PROC pProc);
BOOL APIENTRY vbmp3_startCallback();
BOOL APIENTRY vbmp3_stopCallback();

//ファイル出力系
BOOL APIENTRY vbmp3_decodeWave(const char* pszWaveName);
BOOL APIENTRY vbmp3_changeWav(const char* pszName);
BOOL APIENTRY vbmp3_changeMp3(const char* pszName);
BOOL APIENTRY vbmp3_changeRmp(const char* pszName);
BOOL APIENTRY vbmp3_cutMacBinary(const char* pszName);
BOOL APIENTRY vbmp3_setTagInfo(const char* pszName, TAG_INFO* pTagInfo,int tagSet, int tagAdd);
BOOL APIENTRY vbmp3_setListInfo(const char* pszName, LIST_INFO* pListInfo);

//情報取得系
BOOL APIENTRY vbmp3_getFileInfo2(const char* pszName, TAG_INFO* pTagInfo, MPEG_INFO* pMpegInfo, LIST_INFO* pListInfo);
BOOL APIENTRY vbmp3_getFileInfo(const char* pszName, TAG_INFO* pTagInfo, MPEG_INFO* pMpegInfo);
BOOL APIENTRY vbmp3_getFileTagInfo(const char* pszName, TAG_INFO* pTagInfo);
BOOL APIENTRY vbmp3_getListInfo(LIST_INFO* pListInfo);
BOOL APIENTRY vbmp3_getMpegInfo(MPEG_INFO* pMpegInfo);
BOOL APIENTRY vbmp3_getTagInfo(TAG_INFO* pTagInfo);
int APIENTRY vbmp3_getFileType(const char* pszName);
BOOL APIENTRY vbmp3_getGenre(TAG_INFO* pTagInfo);
long APIENTRY vbmp3_getWinampPlayMs();
int APIENTRY vbmp3_getWinampTotalSec();
int APIENTRY vbmp3_getPlayBitRate();
int APIENTRY vbmp3_getLastErrorNo();

//VBMP3 関連
int APIENTRY vbmp3_getVersion();
BOOL APIENTRY vbmp3_setVbmp3Option(VBMP3_OPTION* pVbmp3Option);
void APIENTRY vbmp3_getVbmp3Option(VBMP3_OPTION* pVbmp3Option);
BOOL APIENTRY vbmp3_setDecodeOption(DEC_OPTION* pDecOption);
void APIENTRY vbmp3_getDecodeOption(DEC_OPTION* pDecOption);
int APIENTRY vbmp3_debug();
void APIENTRY vbmp3_setEqualizer(int* pTable);

//基本操作系
BOOL APIENTRY vbmp3_setStepPitch(int pitch, int frames);
int APIENTRY vbmp3_getStepPitch();
BOOL APIENTRY vbmp3_reload();
BOOL APIENTRY vbmp3_setPitch(int pitch);
int APIENTRY vbmp3_getPitch();
BOOL APIENTRY vbmp3_init();
BOOL APIENTRY vbmp3_free();
BOOL APIENTRY vbmp3_open(const char* pszName, InputInfo* pInfo);
BOOL APIENTRY vbmp3_close();
int APIENTRY vbmp3_getState(int* sec);
BOOL APIENTRY vbmp3_play();
BOOL APIENTRY vbmp3_stop();
BOOL APIENTRY vbmp3_pause();
BOOL APIENTRY vbmp3_restart();
BOOL APIENTRY vbmp3_seek(int pos);
int APIENTRY vbmp3_getPlayFlames();
BOOL APIENTRY vbmp3_setPlayFlames(int flames);
int APIENTRY vbmp3_getPlaySamples();
int APIENTRY vbmp3_getTotalSamples();
BOOL APIENTRY vbmp3_setPlaySamples(int sample);
BOOL APIENTRY vbmp3_setVolume(int left, int right);
BOOL APIENTRY vbmp3_getVolume(int* left, int* right);
void APIENTRY vbmp3_setFadeIn(int on);
void APIENTRY vbmp3_setFadeOut(int on);
void APIENTRY vbmp3_fadeOut();
