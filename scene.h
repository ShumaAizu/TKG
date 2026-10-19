//=============================================================================
//
//	シーンクラス [scene.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _SCENE_H_
#define _SCENE_H_

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// シーンクラス
//*****************************************************************************
class CScene : public CObject
{
public:
	//*************************************************************************
	// モードの種類
	//*************************************************************************
	typedef enum MODE
	{
		MODE_NONE = 0,		// なにもない
		MODE_TITLE,			// タイトル
		MODE_GAME,			// ゲーム
		MODE_RESULT,		// リザルト
		MODE_MAX			// 総数
	}MODE;

	//*************************************************************************
	// 終了フラグ
	//*************************************************************************
	enum class ENDFLAG
	{
		ENDFLAG_NONE,		// なにもない
		ENDFLAG_CLEAR,		// クリア
		ENDFLAG_FAILURE,	// 失敗
		ENDFLAG_MAX			// 総数
	};

	static CScene* Create(MODE mode);

	CScene(MODE mode, int nPriority = 0);
	virtual ~CScene();

	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	MODE GetMode(void) { return m_mode; }
	static ENDFLAG GetEndFlag(void) { return m_endFlag; }
	static void SetEndFlag(ENDFLAG endFlag) { m_endFlag = endFlag; }

private:
	MODE m_mode;				// 現在のモード
	static ENDFLAG m_endFlag;	// 終了条件
};

#endif	// !_SCENE_H_