//=============================================================================
//
//	モデルパーティクルクラス [model_particle.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "model_particle.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "debugproc.h"
#include "move_model.h"

//=============================================================================
// 生成処理
//=============================================================================
CModelParticle* CModelParticle::Create(const D3DXVECTOR3 pos, const TYPE type)
{
#ifndef ENABLE_LINKLIST_OBJECT

	// 最大チェック
	if (GetNumAll() >= MAX_OBJECT)
	{// これ以上生成できなければ
		// NULLを返し終了
		return NULL;
	}
#endif // !ENABLE_LINKLIST_OBJECT

	// ローカル変数宣言
	CModelParticle* pModelParticle = nullptr;		// ポインタ保存用

	// バッチ描画エフェクトのインスタンス生成
	pModelParticle = new CModelParticle;

	// 初期化処理
	pModelParticle->Init(pos, type);

	// 生成したオブジェクトのポインタを返す
	return pModelParticle;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CModelParticle::CModelParticle()
{
	// メンバ変数クリア
	D3DXMatrixIdentity(&m_mtxWorld);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_type = CModelParticle::TYPE::TYPE_SNOWFLAKE;

}

//=============================================================================
// デストラクタ
//=============================================================================
CModelParticle::~CModelParticle()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CModelParticle::Init(void)
{
	return S_OK;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CModelParticle::Init(const D3DXVECTOR3 pos, const TYPE type)
{
	// デバイスの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	CTexture* pTexture = pManager->GetTextrue();

	const char* pFileName[] =
	{
		"data//MODEL//snowflake000.x",
		"data//MODEL//star000.x",
		"data//MODEL//earth000.x"
	};


	//m_pos = pos;
	m_nLife = 180;

	for (int nCnt = 0; nCnt < 10; nCnt++)
	{
		D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		float fVertical = 0.0f, fHorizontal = 0.0f, fSpeed = 0.0f;;

		fVertical = ((float)(rand() % 629 - 314) / 100);
		fHorizontal = ((float)(rand() % 629 - 314) / 100);
		fSpeed = (float)(rand() % 5 + 3);

		fVertical = NumCirculation(fVertical, -(D3DX_PI * 0.5f), (D3DX_PI * 0.5f));

		move.x = sinf(fVertical) * sinf(fHorizontal) * fSpeed;
		move.y = 0.0f;
		move.z = sinf(fVertical) * cosf(fHorizontal) * fSpeed;

		move.x = cosf(fHorizontal) * fSpeed;
		move.y = cosf(fVertical) * (fSpeed * 2.0f);
		move.z = -sinf(fHorizontal) * fSpeed + sinf(fVertical) * fSpeed;

		m_vModel.push_back(CMoveModel::Create(pos, INIT_D3DXVEC3, move, pFileName[(int)type]));
	}


	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CModelParticle::Uninit(void)
{
	for (int nCnt = 0; nCnt < m_vModel.size(); nCnt++)
	{
		m_vModel.at(nCnt)->Uninit();
		delete m_vModel.at(nCnt);
		m_vModel.at(nCnt) = nullptr;
	}

	m_vModel.clear();
}

//=============================================================================
// 更新
//=============================================================================
void CModelParticle::Update(void)
{
	for (int nCnt = 0; nCnt < m_vModel.size(); nCnt++)
	{
		m_vModel.at(nCnt)->Update();
	}

	m_nLife--;
	if (m_nLife < 0)
	{
		Uninit();
	}
}

//=============================================================================
// 描画
//=============================================================================
void CModelParticle::Draw(void)
{
	// デバイスの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	CTexture* pTexture = pManager->GetTextrue();

	D3DXMATRIX mtxRot, mtxTrans, mtxParent;		// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;							// マテリアルデータへのポインタ

	for (int nCnt = 0; nCnt < m_vModel.size(); nCnt++)
	{
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

		m_vModel.at(nCnt)->Draw();
	}
}