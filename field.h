//=============================================================================
//
//	フィールドクラス [field.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードガード
//*****************************************************************************
#ifndef _FIELD_H_
#define _FIELD_H_

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "object3D.h"

//*****************************************************************************
// フィールドクラス
//*****************************************************************************
class CField : public CObject3D
{
public:
	static CField* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fDepth, const char* pTexFile);

	CField(int nPriority = 3);
	~CField() override;

	HRESULT Init(void) override;
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fDepth, const char* pTexFile);
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	float GetHeight(const D3DXVECTOR3 pos, const D3DXVECTOR3 posOld);

private:

};

#endif	// !_FIELD_H_