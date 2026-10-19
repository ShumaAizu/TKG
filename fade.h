//=============================================================================
//
//	フェードクラス [fade.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードガード
//*****************************************************************************
#ifndef _FADE_H_
#define _FADE_H_

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// フェードクラス
//*****************************************************************************
class CFade
{
public:
	//*************************************************************************
	// フェードの状態
	//*************************************************************************
	enum class STATE
	{
		STATE_NONE = 0,		// 何もしていない状態
		STATE_IN,			// フェードイン状態
		STATE_OUT,			// フェードアウト状態
		STATE_MAX			// 総数
	};

	CFade();
	~CFade();

	HRESULT Init(const CScene::MODE modeNext, const D3DXCOLOR col);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Set(const CScene::MODE modeNext, const D3DXCOLOR col, const float fFadeInSpeed = 0.025f, const float fFadeOutSpeed = 0.025f);
	STATE Get(void);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファへのポインタ
	STATE m_state;								// 状態
	CScene::MODE m_modeNext;					// 次のシーン
	D3DXCOLOR m_col;							// ポリゴン(フェード)の色
	float m_fInSpeed;							// フェードイン速度
	float m_fOutSpeed;							// フェードアウト速度
	bool m_isSet;
};

#endif
