//=============================================================================
//
//	回転ギミッククラス [rotationgimmick.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードガード
//*****************************************************************************
#ifndef _ROTATIONGIMMICK_H_
#define _ROTATIONGIMMICK_H_

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "gimmick.h"

//*****************************************************************************
// 回転ギミッククラス
//*****************************************************************************
class CRotationGimmick : public CGimmick
{
public:
	static CRotationGimmick* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const float fDepth, const char* pModelFile);

	CRotationGimmick(int nPriority = 3);
	~CRotationGimmick() override;

	HRESULT Init(void) override;
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const float fDepth, const char* pModelFile);
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	bool Collision(D3DXVECTOR3& pos, float fRadius);

private:
	float m_fWidth;			// 幅
	float m_fHeight;		// 高さ
	float m_fDepth;			// 奥行き
};

#endif	// !_ROTATIONGIMMICK_H_