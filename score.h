//=============================================================================
//
//	スコアクラス [score.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _SCORE_H_
#define _SCORE_H_

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_NUM		(3)		// 数値の桁数

//*****************************************************************************
// スコアクラス
//*****************************************************************************
class CScore : public CObject
{
public:
	CScore(int nPriority = 6);
	~CScore();

	static CScore* Create(const D3DXVECTOR3 pos, const D3DXCOLOR col, const float fWidth, const float fHeight, const int nScore);

	HRESULT Init(void);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXCOLOR col, const float fWidth, const float fHeight, const int nScore);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int Get(void) { return m_nNum; }
	void Add(const int nValue);

private:
	CNumber* m_apNumber[MAX_NUM];		// ナンバーへのポインタ
	int m_nNum;							// 値
	D3DXVECTOR3 m_pos;					// 位置
	D3DXCOLOR m_col;					// 色
	float m_fWidth;						// 幅
	float m_fHeight;					// 高さ
};

#endif	// !_SCORE_H_