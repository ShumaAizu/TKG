//=============================================================================
//
//	メッシュシリンダー処理 [meshcylinder.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードガード
//*****************************************************************************
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// メッシュシリンダークラス
//*****************************************************************************
class CMeshCylinder : public CObject
{
public:
	static CMeshCylinder* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fRadius, const float fHeight, const int nSplitWidth, const int nSplitHeight, const char* pFileName);

	CMeshCylinder(int nPriority = 3);
	~CMeshCylinder() override;

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fRadius, const float fHeight, const int nSplitWidth, const int nSplitHeight, const char* pTexFile);
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

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