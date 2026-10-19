//=============================================================================
//
//	メッシュシリンダー処理 [meshcylinder.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "meshcylinder.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=============================================================================
// 生成
//=============================================================================
CMeshCylinder* CMeshCylinder::Create(const D3DXVECTOR3 pos,
	const D3DXVECTOR3 rot,
	const float fRadius,
	const float fHeight,
	const int nSplitWidth,
	const int nSplitDepth,
	const char* pFileName)
{
	// 最大チェック
	if (GetNumAll() >= MAX_OBJECT)
	{// これ以上生成できなければ
		// NULLを返し終了
		return NULL;
	}

	// ローカル変数宣言
	CMeshCylinder* pMeshCylinder = NULL;		// ポインタ保存用

	// 惑星のインスタンス生成
	pMeshCylinder = new CMeshCylinder;

	// 初期化処理
	pMeshCylinder->Init(pos, rot, fRadius, fHeight, nSplitWidth, nSplitDepth, pFileName);

	// 生成したオブジェクトのポインタを返す
	return pMeshCylinder;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CMeshCylinder::CMeshCylinder(int nPriority) : CMesh(nPriority)
{
	// メンバ変数クリア
	m_nSplitWidth = 0;
	m_nSplitHeight = 0;
	m_fRadius = 0.0f;
	m_fHeight = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CMeshCylinder::~CMeshCylinder()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CMeshCylinder::Init(const D3DXVECTOR3 pos,
	const D3DXVECTOR3 rot,
	const float fRadius,
	const float fHeight,
	const int nSplitWidth,
	const int nSplitHeight,
	const char* pTexFile)
{
	// ポリゴン一枚当たりの頂点の向き
	float fAngleLap = D3DX_PI / nSplitWidth;

	// 分割数を頂点数にそろえる
	m_nSplitWidth = nSplitWidth + 1;
	m_nSplitHeight = nSplitHeight + 1;

	// 親の初期化処理
	CMesh::Init(pos, rot,
		m_nSplitWidth * m_nSplitHeight,
		(m_nSplitWidth * 2 * (m_nSplitHeight - 1) + ((m_nSplitHeight - 2) * 2)),
		pTexFile);

	// 各種値設定
	m_fRadius = fRadius;
	m_fHeight = fHeight;

	// 初期化
	VERTEX_3D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntHeight = 0; nCntHeight < m_nSplitHeight; nCntHeight++)
	{
		float fRot = -D3DX_PI;
		D3DXVECTOR3 vecDir;

		for (int nCntVtxWidht = 0; nCntVtxWidht < m_nSplitWidth; nCntVtxWidht++)
		{
			// 頂点座標の設定
			pVtx[nCntVtxWidht].pos = D3DXVECTOR3(sinf(fRot) * m_fRadius, -MESHCYLINDER_HEIGHT * nCntHeight, cosf(fRot) * m_fRadius);

			// 法線ベクトルの設定
			vecDir = D3DXVECTOR3(pVtx[nCntVtxWidht].pos.x, 0.0f, pVtx[nCntVtxWidht].pos.z);
			D3DXVec3Normalize(&vecDir, &vecDir);
			pVtx[nCntVtxWidht].nor = vecDir;

			// 頂点カラーの設定
			pVtx[nCntVtxWidht].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[nCntVtxWidht].tex = D3DXVECTOR2(0.25f * nCntVtxWidht, -0.49f * nCntHeight);

			// 角度を加算
			fRot += fAngleLap * 2;
		}

		// ポインタを進める
		pVtx += m_nSplitWidth;
	}
		
	// 頂点バッファをアンロックする
	GetVtxBuff()->Unlock();

	WORD* pIdx;		// インデックス情報へのポインタ

	// インデックスバッファをロックし、インデックス情報へのポインタを取得
	GetIdxBuff()->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdxHeigth = 0; nCntIdxHeigth < m_nSplitHeight - 1; nCntIdxHeigth++)
	{
		for (int nCntIdxWidht = 0; nCntIdxWidht < m_nSplitWidth; nCntIdxWidht++)
		{
			if (nCntIdxHeigth != 0 && nCntIdxWidht == 0)
			{// 高さが最初の行以外かつ幅が最初の行
				pIdx[0] = (m_nSplitWidth * (nCntIdxHeigth + 1)) + nCntIdxWidht;

				pIdx++;
			}

			pIdx[0] = (m_nSplitWidth * (nCntIdxHeigth + 1)) + nCntIdxWidht;
			pIdx[1] = m_nSplitWidth * nCntIdxHeigth + nCntIdxWidht;

			pIdx += 2;

			if (nCntIdxHeigth != m_nSplitHeight - 2 && nCntIdxWidht == m_nSplitWidth - 1)
			{// 高さが最後の行以外かつ幅が最後の行
				pIdx[0] = m_nSplitWidth * nCntIdxHeigth + nCntIdxWidht;

				pIdx++;
			}
		}
	}

	// インデックスバッファをアンロックする
	GetIdxBuff()->Unlock();

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CMeshCylinder::Uninit(void)
{

}

//=============================================================================
// 更新
//=============================================================================
void CMeshCylinder::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CMeshCylinder::Draw(void)
{

}