//=============================================================================
//
//	フェード処理 [fade.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "fade.h"

#include "manager.h"
#include "renderer.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CFade::CFade()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CFade::~CFade()
{
	// メンバ変数クリア
	m_pVtxBuff = nullptr;
	m_state = STATE::STATE_NONE;
	m_modeNext = CScene::MODE_NONE;
	m_col = COLOR_WHITE;
	m_fInSpeed = 0.0f;
	m_fOutSpeed = 0.0f;
	m_isSet = false;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CFade::Init(const CScene::MODE modeNext, const D3DXCOLOR col)
{
	// デバイスの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	m_state = STATE::STATE_IN;	// フェードイン状態に

	m_modeNext = modeNext;		// 次の画面を指定

	m_col = col;				// 指定した色にする

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 初期化
	m_fInSpeed = 0.025f;
	m_fOutSpeed = 0.025f;
	m_isSet = true;

	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// モードの設定
	pManager->SetMode(m_modeNext);

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CFade::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// 描画
//=============================================================================
void CFade::Draw(void)
{
	// デバイスの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// 更新
//=============================================================================
void CFade::Update(void)
{
	if (m_state != STATE::STATE_NONE)
	{
		if (m_state == STATE::STATE_IN)
		{// フェードイン状態
			m_col.a -= m_fInSpeed;					// ポリゴンを透明にしていく
			if (m_col.a <= 0.0f)
			{// 透明になったなら
				m_col.a = 0.0f;
				m_state = STATE::STATE_NONE;		// 何もしていない状態にする
			}
		}
		else if (m_state == STATE::STATE_OUT)
		{// フェードアウト状態
			m_col.a += m_fOutSpeed;					// ポリゴンを不透明にしていく
			if (m_col.a >= 1.0f)
			{// はっきりしたなら
				m_col.a = 1.0f;
				m_state = STATE::STATE_IN;			// フェードイン状態にする

				if (m_isSet == true)
				{
					// モードの設定
					CManager::GetInstance()->SetMode(m_modeNext);
				}
			}
		}

		VERTEX_2D* pVtx;			// 頂点情報へのポインタ

		// 頂点バッファをロックし,頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラーの設定
		pVtx[0].col = m_col;
		pVtx[1].col = m_col;
		pVtx[2].col = m_col;
		pVtx[3].col = m_col;

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// 設定
//=============================================================================
void CFade::Set(CScene::MODE modeNext, D3DXCOLOR col, float fFadeInSpeed, float fFadeOutSpeed)
{
	if (m_state == STATE::STATE_NONE)
	{
		m_state = STATE::STATE_OUT;			// フェードアウト状態に

		m_isSet = true;						// モード切替

		m_fInSpeed = fFadeInSpeed;			// フェードイン速度

		m_fOutSpeed = fFadeOutSpeed;		// フェードアウト速度

		m_modeNext = modeNext;				// 次の画面(モード)を設定

		m_col = col;						// 指定した色に設定

		m_col.a = 0.0f;						// 透明にしておく
	}
}

//=============================================================================
// 状態の取得
//=============================================================================
CFade::STATE CFade::Get(void)
{
	return m_state;
}