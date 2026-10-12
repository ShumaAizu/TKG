//=============================================================================
//
//	オブジェクト3Dクラス [object3D.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// オブジェクトクラスの定義
//*****************************************************************************
class CObject3D : public CObject
{
public:
	static CObject3D* Create(void);
	static CObject3D* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const float fDepth, const char* pTexFile);

	CObject3D(int nPriority = 3);
	~CObject3D() override;

	HRESULT Init(void) override;
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const float fDepth, const char* pTexFile);
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void SetPosition(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition(void);
	void SetRotation(const D3DXVECTOR3 rot);
	D3DXVECTOR3 GetRotation(void);
	void SetSize(const float fWidth, const float fHeight, const float fDepth);
	D3DXVECTOR3 GetSize(void);
	float GetLength(void) { return m_fLength; }
	void SetColor(const D3DXCOLOR col);

protected:
	int m_nIdxTexture;					// テクスチャのインデックス
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス

private:
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	float m_fWidth;						// 幅
	float m_fHeight;					// 高さ
	float m_fDepth;						// 奥行
	float m_fAngle;						// 角度
	float m_fLength;					// 長さ
};

#endif