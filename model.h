//=============================================================================
//
//	モデルクラス [model.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _MODEL_H_
#define _MODEL_H_

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "object.h"
#include <vector>

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// モデルクラスの定義
//*****************************************************************************
class CModel
{
public:
	static CModel* Create(void);
	static CModel* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const char *pFileName);

	CModel();
	virtual ~CModel();

	virtual HRESULT Init(void);
	virtual HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const char* pFileName);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void SetPosition(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition(void) const;
	D3DXVECTOR3 GetPositionOffset(void) const;
	void SetRotation(const D3DXVECTOR3 rot);
	D3DXVECTOR3 GetRotation(void) const;
	D3DXVECTOR3 GetRotationOffset(void) const;
	void SetParent(CModel* pModel);
	D3DXMATRIX GetMtxWorld(void) const;

private:
	std::vector<int> m_vIdxTexture;		// テクスチャのインデックス
	LPD3DXMESH m_pMesh;					// メッシュへのポインタ
	LPD3DXBUFFER m_pBuffMat;			// マテリアルへのポインタ
	DWORD m_dwNumMat;					// マテリアルの数
	D3DXVECTOR3 m_vtxMin;				// 頂点位置の最小値
	D3DXVECTOR3 m_vtxMax;				// 頂点位置の最大値
	DWORD m_dwSizeFVF;					// 頂点フォーマットのサイズ
	int m_nNumVtx;						// 頂点数
	int m_nNumIdx;						// インデックス数

	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_posOffset;			// オフセット位置
	D3DXVECTOR3 m_rot;					// 向き
	D3DXVECTOR3 m_rotOffset;			// オフセット向き
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	CModel* m_pParent;					// 親モデルへのポインタ
};

#endif