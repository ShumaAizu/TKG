//=============================================================================
//
//	リングエフェクト処理 [ringeffect.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "ringeffect.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=============================================================================
// 生成
//=============================================================================
CRingEffect* CRingEffect::Create(const D3DXVECTOR3 pos,
	const D3DXVECTOR3 rot,
	const D3DXCOLOR col,
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
	CRingEffect* pRingEffect = NULL;		// ポインタ保存用

	// 惑星のインスタンス生成
	pRingEffect = new CRingEffect;

	// 初期化処理
	pRingEffect->Init(pos, rot, col, fInRadius, fOutRadius, nSplit, pTexFile);

	// 生成したオブジェクトのポインタを返す
	return pRingEffect;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CRingEffect::CRingEffect(int nPriority) : CMeshRing(nPriority)
{
	// メンバ変数クリア
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CRingEffect::~CRingEffect()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CRingEffect::Init(void)
{
	return S_OK;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CRingEffect::Init(const D3DXVECTOR3 pos,
	const D3DXVECTOR3 rot,
	const D3DXCOLOR col,
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
	m_col = col;

	CMeshRing::Init(pos, rot, fInRadius, fOutRadius, nSplit, pTexFile);

	SetColor(col);

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CRingEffect::Uninit(void)
{
	// 終了
	CMeshRing::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CRingEffect::Update(void)
{
	// ローカル変数
	float fInRadius = 0.0f;
	float fOutRadius = 0.0f;

	fInRadius = GetInRadius();
	fOutRadius = GetOutRadius();

	fInRadius += 5.0f;
	fOutRadius += 2.5f;
	m_col.a += -0.025f;

	SetInRadius(fInRadius);
	SetOutRadius(fOutRadius);
	SetColor(m_col);

	// 更新
	CMeshRing::Update();

	if (fInRadius > fOutRadius)
	{
		Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CRingEffect::Draw(void)
{
	// デバイスの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// αブレンディングを加算合成して設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 描画
	CMeshRing::Draw();

	// αブレンディングを戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}