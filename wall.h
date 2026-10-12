//=============================================================================
//
//	ウォールクラス [wall.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _WALL_H_
#define _WALL_H_

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "object3D.h"

//*****************************************************************************
// ウォールクラス
//*****************************************************************************
class CWall : public CObject3D
{
public:
	static CWall* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const char* pTexFile);

	CWall(int nPriority = 3);
	~CWall() override;

	HRESULT Init(void) override;
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const char* pTexFile);
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	bool Collision(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, float fRadius, bool bRetrun, bool bIn);

private:

};

#endif	// !_WALL_H_