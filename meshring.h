//=============================================================================
//
//	メッシュリング処理 [meshring.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードガード
//*****************************************************************************
#ifndef _MESHRING_H_
#define _MESHRING_H_

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// メッシュリングクラス
//*****************************************************************************
class CMeshRing : public CObject
{
public:
	static CMeshRing* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fInRadius, const float fOutRadius, const int nSplit, const char* pTexFile);

	CMeshRing(int nPriority = 3);
	virtual ~CMeshRing() override;

	virtual HRESULT Init(void);
	virtual HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fInRadius, const float fOutRadius, const int nSplit, const char* pTexFile);
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;

	D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	void SetPosition(const D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_rot; }
	void SetRotation(const D3DXVECTOR3 rot) { m_rot = rot; }
	float GetInRadius(void) const { return m_fInRadius; }
	void SetInRadius(const float fInRadius) { m_fInRadius = fInRadius; }
	float GetOutRadius(void) const { return m_fOutRadius; }
	void SetOutRadius(const float fOutRadius) { m_fOutRadius = fOutRadius; }

	void SetColor(const D3DXCOLOR col);

protected:
	int m_nIdxTexture;						// テクスチャのインデックス
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		// インデックスバッファへのポインタ
	D3DXMATRIX m_mtxWorld;					// ワールドマトリックス

private:
	D3DXVECTOR3 m_pos;						// 位置
	D3DXVECTOR3 m_rot;						// 向き
	int m_nSplit;							// 分割数
	float m_fInRadius;						// 内側の半径
	float m_fOutRadius;						// 外側の半径
};

#endif