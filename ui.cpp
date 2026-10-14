//=============================================================================
//
//	UIクラス [ui.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "object.h"
#include "ui.h"
#include "renderer.h"
#include "input.h"

#include "debugproc.h"
#include "sound.h"
#include "texture.h"

//=============================================================================
// 生成処理
//=============================================================================
CUI* CUI::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const char* pTexFileName, const int nPriority)
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
	CUI* pUI = NULL;		// ポインタ保存用

	// テクスチャインスタンスへのポインタを取得
	CManager* pManager = CManager::GetInstance();
	CTexture* pTexture = pManager->GetTextrue();

	// オブジェクト2Dのインスタンス生成
	pUI = new CUI(nPriority);

	// 初期化処理
	pUI->Init(pos, rot, fWidth, fHeight);

	// テクスチャの登録
	pUI->SetIdxTexture(pTexture->Register(pTexFileName));

	// 生成したオブジェクトのポインタを返す
	return pUI;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CUI::CUI(int nPriority) : CObject2D(nPriority)
{
	// メンバ変数クリア
	m_isDisp = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CUI::~CUI()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CUI::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight)
{
	// 初期化処理
	CObject2D::Init(pos, rot, fWidth, fHeight);

	// 種類の設定
	SetType(TYPE_UI);

	// 表示状態を有効に
	m_isDisp = true;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CUI::Uninit(void)
{
	// 表示状態無効
	m_isDisp = false;

	// 終了処理
	CObject2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CUI::Update(void)
{
	// 更新処理
	CObject2D::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CUI::Draw(void)
{
	if (m_isDisp == false)
	{// 表示状態が無効なら早期終了
		return;
	}

	// 描画処理
	CObject2D::Draw();
}