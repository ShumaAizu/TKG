//=============================================================================
//
// サウンド処理 [sound.h]
// Author : SHUMA AIZU
//          AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンドクラス
//*****************************************************************************
class CSound
{
public:
	//*************************************************************************
	// サウンド一覧
	//*************************************************************************
	typedef enum
	{
		SOUND_LABEL_TITLE_BGM_000 = 0,				// タイトルBGM
		SOUND_LABEL_GAME_BGM_000,					// ゲームBGM
		SOUND_LABEL_RESULT_BGM_000,					// リザルトBGM
		SOUND_LABEL_SELECT_SE_000,					// 選択SE
		SOUND_LABEL_SELECT_SE_001,					// 選択SE2
		SOUND_LABEL_ABSORPTION_SE_000,				// 収集SE
		SOUND_LABEL_EXPLOSION_SE_000,				// 爆発SE
		SOUND_LABEL_ACCELERATION_SE_000,			// 加速SE
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	//*************************************************************************
	// サウンド情報の構造体定義
	//*************************************************************************
	typedef struct
	{
		const char* pFilename;	// ファイル名
		int nCntLoop;			// ループカウント
	} SOUNDINFO;

	CSound();
	~CSound();

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT Play(SOUND_LABEL label);
	void Stop(SOUND_LABEL label);
	void Stop(void);
	bool Check(SOUND_LABEL label);

private:
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
	IXAudio2* m_pXAudio2;									// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice* m_pMasteringVoice;				// マスターボイス
	IXAudio2SourceVoice* m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	BYTE* m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ
	SOUNDINFO m_aSoundInfo[SOUND_LABEL_MAX];				// サウンドの情報
};

#endif
