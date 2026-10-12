//=============================================================================
//
//	モデルクラス [model.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "object.h"
#include "model.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "texture.h"

//=============================================================================
// 生成処理
//=============================================================================
CModel* CModel::Create(void)
{
	// ローカル変数宣言
	CModel* pModel = NULL;		// ポインタ保存用

	// モデルのインスタンス生成
	pModel = new CModel;

	// 初期化処理
	pModel->Init();

	// 生成したオブジェクトのポインタを返す
	return pModel;
}

//=============================================================================
// 生成処理
//=============================================================================
CModel* CModel::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const char* pFileName)
{
	// ローカル変数宣言
	CModel* pModel = NULL;		// ポインタ保存用

	// オブジェクト2Dのインスタンス生成
	pModel = new CModel;

	// 初期化処理
	pModel->Init(pos, rot, pFileName);

	// 生成したオブジェクトのポインタを返す
	return pModel;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CModel::CModel()
{
	// 各変数クリア
	m_vIdxTexture.clear();
	m_pMesh = NULL;
	m_pBuffMat = NULL;
	m_dwNumMat = 0;
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_dwSizeFVF = 0;
	m_nNumVtx = 0;
	m_nNumIdx = 0;

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pParent = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CModel::~CModel()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CModel::Init(void)
{
	// デバイスの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	CTexture* pTexture = pManager->GetTextrue();

	// 変数初期化
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	BYTE* pVtxBuff;								// 頂点バッファへのポインタ

	// Xファイルの読み込み
	D3DXLoadMeshFromX("data//MODEL//player000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMat,
		&m_pMesh);

	if (m_pBuffMat == NULL)
	{
		return E_FAIL;
	}

	D3DXMATERIAL* pMat;

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{// テクスチャファイルが存在する
			// テクスチャの登録
			m_vIdxTexture.push_back(pTexture->Register(pMat[nCntMat].pTextureFilename));
			continue;
		}

		// 存在しなければ無効の番号を入れておく
		m_vIdxTexture.push_back(-1);
	}

	// 頂点数を取得
	m_nNumVtx = m_pMesh->GetNumVertices();

	// インデックス数を取得
	m_nNumIdx = m_pMesh->GetNumFaces();

	// 頂点フォーマットのサイズを取得
	m_dwSizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	// 頂点バッファをロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点情報の代入

		if (vtx.x > m_vtxMax.x)
		{
			m_vtxMax.x = vtx.x;
		}

		if (vtx.y > m_vtxMax.y)
		{
			m_vtxMax.y = vtx.y;
		}

		if (vtx.z > m_vtxMax.z)
		{
			m_vtxMax.z = vtx.z;
		}

		if (vtx.x < m_vtxMin.x)
		{
			m_vtxMin.x = vtx.x;
		}

		if (vtx.y < m_vtxMin.y)
		{
			m_vtxMin.y = vtx.y;
		}

		if (vtx.z < m_vtxMin.z)
		{
			m_vtxMin.z = vtx.z;
		}

		pVtxBuff += m_dwSizeFVF;		// 頂点フォーマットのサイズ分ポインタを進める
	}

	// 頂点バッファのアンロック
	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CModel::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const char* pFileName)
{
	// デバイスの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	CTexture* pTexture = pManager->GetTextrue();

	// 変数初期化
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_posOffset = pos;
	m_rotOffset = rot;

	// Xファイルの読み込み
	D3DXLoadMeshFromX(pFileName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMat,
		&m_pMesh);

	if (m_pBuffMat == NULL)
	{
		return E_FAIL;
	}

	D3DXMATERIAL* pMat;

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{// テクスチャファイルが存在する
			// テクスチャの登録
			m_vIdxTexture.push_back(pTexture->Register(pMat[nCntMat].pTextureFilename));
			continue;
		}

		// 存在しなければ無効の番号を入れておく
		m_vIdxTexture.push_back(-1);
	}

	// 頂点数を取得
	m_nNumVtx = m_pMesh->GetNumVertices();

	// インデックス数を取得
	m_nNumIdx = m_pMesh->GetNumFaces();

	// 頂点フォーマットのサイズを取得
	m_dwSizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	// 頂点バッファへのポインタ
	BYTE* pVtxBuff;

	// 頂点バッファをロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点情報の代入

		if (vtx.x > m_vtxMax.x)
		{
			m_vtxMax.x = vtx.x;
		}

		if (vtx.y > m_vtxMax.y)
		{
			m_vtxMax.y = vtx.y;
		}

		if (vtx.z > m_vtxMax.z)
		{
			m_vtxMax.z = vtx.z;
		}

		if (vtx.x < m_vtxMin.x)
		{
			m_vtxMin.x = vtx.x;
		}

		if (vtx.y < m_vtxMin.y)
		{
			m_vtxMin.y = vtx.y;
		}

		if (vtx.z < m_vtxMin.z)
		{
			m_vtxMin.z = vtx.z;
		}

		pVtxBuff += m_dwSizeFVF;		// 頂点フォーマットのサイズ分ポインタを進める
	}

	// 頂点バッファのアンロック
	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CModel::Uninit(void)
{
	// メッシュの破棄
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// マテリアルの破棄
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CModel::Update(void)
{
	
}

//=============================================================================
// 描画処理
//=============================================================================
void CModel::Draw(void)
{
	// デバイスの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	CTexture* pTexture = pManager->GetTextrue();

	D3DXMATRIX mtxRot, mtxTrans, mtxParent;		// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;							// マテリアルデータへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_pParent != NULL)
	{// 親モデルが存在していれば
		// 親モデルのワールドマトリックスを取得
		mtxParent = m_pParent->GetMtxWorld();
	}
	else
	{// 親モデルが存在しなければ
		// 現在(最新)のワールドマトリックスを取得
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	// 親モデルのワールドマトリックスと掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, pTexture->GetAddress(m_vIdxTexture.at(nCntMat)));

		// モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// 位置指定処理
//=============================================================================
void CModel::SetPosition(const D3DXVECTOR3 pos)
{
	// 位置変更
	m_pos = pos;
}

//=============================================================================
// 位置取得処理
//=============================================================================
D3DXVECTOR3 CModel::GetPosition(void) const
{
	return m_pos;
}

//=============================================================================
// オフセット位置取得処理
//=============================================================================
D3DXVECTOR3 CModel::GetPositionOffset(void) const
{
	return m_posOffset;
}

//=============================================================================
// 向き指定処理
//=============================================================================
void CModel::SetRotation(const D3DXVECTOR3 rot)
{
	// 向き変更
	m_rot = rot;

	// 向きの範囲制限
	m_rot.x = AngleNormalize(m_rot.x);
	m_rot.y = AngleNormalize(m_rot.y);
	m_rot.z = AngleNormalize(m_rot.z);
}

//=============================================================================
// 向き取得処理
//=============================================================================
D3DXVECTOR3 CModel::GetRotation(void) const
{
	return m_rot;
}

//=============================================================================
// オフセット向き取得処理
//=============================================================================
D3DXVECTOR3 CModel::GetRotationOffset(void) const
{
	return m_rotOffset;
}

//=============================================================================
// 親モデル設定処理
//=============================================================================
void CModel::SetParent(CModel* pModel)
{
	m_pParent = pModel;
}

//=============================================================================
// ワールドマトリックス取得処理
//=============================================================================
D3DXMATRIX CModel::GetMtxWorld(void) const
{
	return m_mtxWorld;
}