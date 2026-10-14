//=============================================================================
//
//	ギミッククラス [gimmick.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "gimmick.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "object.h"
#include "model.h"

//=============================================================================
// 生成
//=============================================================================
CGimmick* CGimmick::Create(const D3DXVECTOR3 pos,
	const D3DXVECTOR3 rot,
	const char* pModelFile)
{
	// 最大チェック
	if (GetNumAll() >= MAX_OBJECT)
	{// これ以上生成できなければ
		// nullptrを返し終了
		return nullptr;
	}

	// ローカル変数宣言
	CGimmick* pGimmick = nullptr;		// ポインタ保存用

	// ギミックのインスタンス生成
	pGimmick = new CGimmick;

	// 初期化処理
	pGimmick->Init(pos, rot, pModelFile);

	// 生成したオブジェクトのポインタを返す
	return pGimmick;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CGimmick::CGimmick(int nPriority) : CObject(nPriority)
{
	// 各メンバ変数クリア
	m_pModel = nullptr;
	m_nNumModel = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type = TYPE_NONE;
}

//=============================================================================
// デストラクタ
//=============================================================================
CGimmick::~CGimmick()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CGimmick::Init(void)
{
	return S_OK;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CGimmick::Init(const D3DXVECTOR3 pos,
	const D3DXVECTOR3 rot,
	const char* pModelFile)
{
	// 各変数初期化
	m_pos = pos;
	m_rot = rot;

	// モデルの生成
	m_pModel = CModel::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		pModelFile);

	m_nNumModel++;

	SetType(TYPE_GIMMICK);

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CGimmick::Uninit(void)
{
	// モデルの破棄
	if (m_pModel != nullptr)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = nullptr;
	}

	// 開放
	Release();
}

//=============================================================================
// 更新
//=============================================================================
void CGimmick::Update(void)
{
	
}

//=============================================================================
// 描画
//=============================================================================
void CGimmick::Draw(void)
{
	// デバイスの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス

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

	if (m_pModel != nullptr)
	{
		// 描画処理
		m_pModel->Draw();
	}
}