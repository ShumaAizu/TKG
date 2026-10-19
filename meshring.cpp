//=============================================================================
//
//	メッシュリング処理 [meshring.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "meshring.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=============================================================================
// 生成
//=============================================================================
CMeshRing* CMeshRing::Create(const D3DXVECTOR3 pos,
	const D3DXVECTOR3 rot,
	const float fInRadius,
	const float fOutRadius,
	const int nSplit,
	const char* pTexFile)
{
	// 最大チェック
	if (GetNumAll() >= MAX_OBJECT)
	{// これ以上生成できなければ
		// NULLを返し終了
		return NULL;
	}

	// ローカル変数宣言
	CMeshRing* pMeshRing = NULL;		// ポインタ保存用

	// 惑星のインスタンス生成
	pMeshRing = new CMeshRing;

	// 初期化処理
	pMeshRing->Init(pos, rot, fInRadius, fOutRadius, nSplit, pTexFile);

	// 生成したオブジェクトのポインタを返す
	return pMeshRing;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CMeshRing::CMeshRing(int nPriority) : CObject(nPriority)
{
	// メンバ変数クリア
	m_nIdxTexture = 0;
	m_pVtxBuff = nullptr;
	m_pIdxBuff = nullptr;
	
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nSplit = 0;
	m_fInRadius = 0.0f;
	m_fOutRadius = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CMeshRing::~CMeshRing()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CMeshRing::Init(void)
{
	return S_OK;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CMeshRing::Init(const D3DXVECTOR3 pos,
	const D3DXVECTOR3 rot,
	const float fInRadius,
	const float fOutRadius,
	const int nSplit,
	const char* pTexFile)
{
	// デバイスの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャインスタンスへのポインタを取得
	CTexture* pTexture = pManager->GetTextrue();

	// 初期化
	m_pos = pos;
	m_rot = rot;
	m_fInRadius = fInRadius;
	m_fOutRadius = fOutRadius;
	m_nSplit = nSplit + 1;

	// ポリゴン一枚当たりの頂点の向き
	float fAngleLap = D3DX_PI / nSplit;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * nSplit * nSplit,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	// 初期化
	VERTEX_3D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fAngle = D3DX_PI;

	for (int nCntVtx = 0; nCntVtx < m_nSplit; nCntVtx++)
	{
		pVtx[0].pos = D3DXVECTOR3(sinf(fAngle) * m_fOutRadius, 0.0f, cosf(fAngle) * m_fOutRadius);
		pVtx[1].pos = D3DXVECTOR3(sinf(fAngle) * m_fInRadius, 0.0f, cosf(fAngle) * m_fInRadius);

		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);

		pVtx[0].tex = D3DXVECTOR2(1.0f * nCntVtx, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f * nCntVtx, 0.0f);

		fAngle += -fAngleLap * 2;
		pVtx += 2;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// インデックスバッファの設定
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nSplit * m_nSplit,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		nullptr);

	WORD* pIdx;		// インデックス情報へのポインタ

	// インデックスバッファをロックし、頂点情報へのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntVtx = 0; nCntVtx < m_nSplit * m_nSplit; nCntVtx++)
	{
		pIdx[nCntVtx] = nCntVtx;
	}

	// インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();

	// テクスチャの登録
	m_nIdxTexture = pTexture->Register(pTexFile);

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CMeshRing::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// インデックスバッファの破棄
	if (m_pIdxBuff != nullptr)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}

	// 解放
	Release();
}

//=============================================================================
// 更新
//=============================================================================
void CMeshRing::Update(void)
{
	// ポリゴン一枚当たりの頂点の向き
	float fAngleLap = D3DX_PI / (m_nSplit - 1);

	// 初期化
	VERTEX_3D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fAngle = D3DX_PI;

	for (int nCntVtx = 0; nCntVtx < m_nSplit; nCntVtx++)
	{
		pVtx[0].pos = D3DXVECTOR3(sinf(fAngle) * m_fOutRadius, 0.0f, cosf(fAngle) * m_fOutRadius);
		pVtx[1].pos = D3DXVECTOR3(sinf(fAngle) * m_fInRadius, 0.0f, cosf(fAngle) * m_fInRadius);

		fAngle += -fAngleLap * 2;
		pVtx += 2;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void CMeshRing::Draw(void)
{
	// デバイスの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャインスタンスへのポインタを取得
	CTexture* pTexture = pManager->GetTextrue();

	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	// メッシュリングの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		m_nSplit * m_nSplit,
		0,
		(m_nSplit - 1) + (m_nSplit - 1));
}

//=============================================================================
// 色変更処理
//=============================================================================
void CMeshRing::SetColor(const D3DXCOLOR col)
{
	// ポリゴン一枚当たりの頂点の向き
	float fAngleLap = D3DX_PI / m_nSplit;

	VERTEX_3D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fAngle = D3DX_PI;

	for (int nCntVtx = 0; nCntVtx < m_nSplit; nCntVtx++)
	{
		pVtx[0].col = col;
		pVtx[1].col = col;

		fAngle += -fAngleLap * 2;
		pVtx += 2;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}