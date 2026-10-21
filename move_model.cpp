//=============================================================================
//
//	移動モデル処理 [move_model.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "move_model.h"
#include "input.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define GRAVITY				(-0.25f)		// 重力

//=============================================================================
// 生成
//=============================================================================
CMoveModel* CMoveModel::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 move, const char* pModelFile)
{
	// ローカル変数宣言
	CMoveModel* pMoveModel = nullptr;		// ポインタ保存用

	// 惑星のインスタンス生成
	pMoveModel = new CMoveModel;

	// 初期化処理
	pMoveModel->Init(pos, rot, move, pModelFile);

	// 生成したオブジェクトのポインタを返す
	return pMoveModel;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CMoveModel::CMoveModel()
{
	// メンバ変数クリア
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CMoveModel::~CMoveModel()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CMoveModel::Init(void)
{
	return S_OK;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CMoveModel::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 move, const char* pModelFile)
{
	// デバイスの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャインスタンスへのポインタを取得
	CTexture* pTexture = pManager->GetTextrue();

	// 初期化
	m_move = move;

	CModel::Init(pos, rot, pModelFile);

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CMoveModel::Uninit(void)
{
	// 終了
	CModel::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CMoveModel::Update(void)
{
	D3DXVECTOR3 pos = GetPosition();

	m_move.y += GRAVITY;	// 重力をかける

	pos += m_move;

	if (pos.y <-50.0f)
	{
		m_move.y = m_move.y * -0.9f;
	}

	SetPosition(pos);

	// 更新
	CModel::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CMoveModel::Draw(void)
{
	// デバイスの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 描画
	CModel::Draw();
}