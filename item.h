//=============================================================================
//
//	アイテムクラス [item.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードガード
//*****************************************************************************
#ifndef _ITEM_H_
#define _ITEM_H_

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "objectX.h"

//*****************************************************************************
// フィールドクラス
//*****************************************************************************
class CItem : public CObjectX
{
public:
	static CItem* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fRadius, const char* pModelFile);

	CItem(int nPriority = 3);
	~CItem() override;

	HRESULT Init(void) override;
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fRadius, const char* pModelFile);
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	float GetRadius(void) { return m_fRadius; }

	bool Get(void);

private:
	float m_fRadius;			// 半径
};

#endif	// !_FIELD_H_