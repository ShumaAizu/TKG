//=============================================================================
//
//	UIクラス [ui.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _UI_H_
#define _UI_H_

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "object2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// UIクラスの定義
//*****************************************************************************
class CUI : public CObject2D
{
public:
	CUI(int nPriority = 7);
	~CUI();

	static CUI* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const char* pTexFileName, const int nPriority = 7);

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	const bool GetDisp(void) { return m_isDisp; }
	void SetDisp(bool isDisp) { m_isDisp = isDisp; }

private:
	bool m_isDisp;		// 表示状態
};

#endif	// !_UI_H_