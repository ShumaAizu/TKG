//=============================================================================
//
//	リングエフェクト処理 [ringeffect.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードガード
//*****************************************************************************
#ifndef _RINGEFFECT_H_
#define _RINGEFFECT_H_

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "meshring.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// リングエフェクトクラス
//*****************************************************************************
class CRingEffect : public CMeshRing
{
public:
	static CRingEffect* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXCOLOR col, const float fInRadius, const float fOutRadius, const int nSplit, const char* pTexFile);

	CRingEffect(int nPriority = 3);
	~CRingEffect() override;

	HRESULT Init(void) override;
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXCOLOR col, const float fInRadius, const float fOutRadius, const int nSplit, const char* pTexFile);
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
	D3DXCOLOR m_col;
};

#endif