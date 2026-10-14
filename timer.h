//=============================================================================
//
//	タイマークラス [timer.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _TIMER_H_
#define _TIMER_H_

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;				// 数字クラス
class CObject2D;			// オブジェクト2Dクラス

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_NUMMS	(3)		// 数値の桁数 (ミリ秒)

//*****************************************************************************
// スコアクラス
//*****************************************************************************
class CTimer : public CObject
{
public:
	CTimer(int nPriority = 6);
	~CTimer();

	static CTimer* Create(const D3DXVECTOR3 pos, const D3DXCOLOR col, const float fWidth, const float fHeight);

	HRESULT Init(void);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXCOLOR col, const float fWidth, const float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetUse(const bool isUse);
	void AddDigit(void);

private:
	std::vector<CNumber*> m_vpTimerNumber;		// 経過時間数値 (秒) へのポインタ
	CNumber* m_apTimerNumberMs[MAX_NUMMS];		// 経過時間数値 (ミリ秒) へのポインタ
	CObject2D* m_pDot;							// ドットへのポインタ
	int m_nTimer;								// 経過時間 (秒)
	int m_nTimerMs;								// 経過時間 (ミリ秒)
	DWORD m_dwExecLastTime;						// 最後の計測時間
	D3DXVECTOR3 m_pos;							// 基準位置
	D3DXCOLOR m_col;							// 基準色
	float m_fWidth;								// 基準幅
	float m_fHeight;							// 基準高さ
	bool m_isUse;								// 使用しているか
};

#endif	// !_TIMER_H_