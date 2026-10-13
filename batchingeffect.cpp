//=============================================================================
//
//	バッチ描画エフェクトクラス [batchingeffect.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "batchingeffect.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "debugproc.h"

//=============================================================================
// 生成処理
//=============================================================================
CBatchingEffect* CBatchingEffect::Create(const int nMax, const char* pTexFileName)
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
	CBatchingEffect* pBatchingEffect = NULL;		// ポインタ保存用

	// バッチ描画エフェクトのインスタンス生成
	pBatchingEffect = new CBatchingEffect;

	// 初期化処理
	pBatchingEffect->Init(nMax, pTexFileName);

	// 生成したオブジェクトのポインタを返す
	return pBatchingEffect;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CBatchingEffect::CBatchingEffect()
{
	// メンバ変数クリア
	m_pVtxBuff = NULL;
	m_pIdxBuff = NULL;
	m_nIdxTexture = 0;
	m_nMax = 0;
	m_nNum = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CBatchingEffect::~CBatchingEffect()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CBatchingEffect::Init(void)
{
	return S_OK;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CBatchingEffect::Init(const int nMax, const char* pTexFileName)
{
	// デバイスの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	CTexture* pTexture = pManager->GetTextrue();

	// 引数の値をメンバ変数に代入
	m_nMax = nMax;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * nMax * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff, 
		NULL);

	// テクスチャの登録
	m_nIdxTexture = pTexture->Register(pTexFileName);

	VERTEX_3D* pVtx;    // 頂点情報の設定

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < m_nMax; nCntEffect++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラーの設定
		pVtx[0].col = COLOR_WHITE;
		pVtx[1].col = COLOR_WHITE;
		pVtx[2].col = COLOR_WHITE;
		pVtx[3].col = COLOR_WHITE;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	// インデックスバッファの設定
	pDevice->CreateIndexBuffer(sizeof(WORD) * (nMax * (4 + 2)),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	WORD* pIdx;		// インデックス情報へのポインタ

	// インデックスバッファをロックし、頂点情報へのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// 超雑式で6つのインデックスを四角形に指定
	for (int nCntIdx = 0; nCntIdx < nMax * 4; nCntIdx += 4)
	{
		pIdx[0] = nCntIdx + 2;
		pIdx[1] = nCntIdx;
		pIdx[2] = nCntIdx + 1;
		pIdx[3] = nCntIdx + 2;
		pIdx[4] = nCntIdx + 1;
		pIdx[5] = nCntIdx + 3;

		pIdx += 6;
	}

	// インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CBatchingEffect::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// インデックスバッファの破棄
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}
}

//=============================================================================
// 更新
//=============================================================================
void CBatchingEffect::Update(void)
{
	// ここで種類ごとの更新とかするかも
	for (int nCntEffect = 0; nCntEffect < m_nNum; nCntEffect++)
	{
		if (m_vType.at(nCntEffect) == TYPE::TYPE_TRAJECTORY)
		{
			m_vRadius.at(nCntEffect) += -1.0f;	// 半径を小さくする
			m_vCol.at(nCntEffect).a += -0.025f;

			if (m_vRadius.at(nCntEffect) < 0.0f || m_vCol.at(nCntEffect).a < 0.0f)
			{// 見えなくなったら
				// 解放処理
				SwapAndPop(nCntEffect);
				nCntEffect--;
				continue;
			}
		}
	}

	CDebugProc::Print("エフェクト数 : %d\n", m_nNum);
}

//=============================================================================
// 描画
//=============================================================================
void CBatchingEffect::Draw(void)
{
	// 必要リソース取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	CTexture* pTexture = pManager->GetTextrue();

	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DXMATRIX mtx;					// 単位行列
	D3DXMATRIX mtxView;				// ビューマトリックス

	// ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	VERTEX_3D* pVtx;    // 頂点情報の設定

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < m_nNum; nCntEffect++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_vMtxWorld.at(nCntEffect));

		// カメラに対して正面に向ける
		D3DXMatrixInverse(&m_vMtxWorld.at(nCntEffect), NULL, &mtxView);	// 逆行列を求める

		m_vMtxWorld.at(nCntEffect)._41 = 0.0f;		// マトリックス(行列)の内容
		m_vMtxWorld.at(nCntEffect)._42 = 0.0f;
		m_vMtxWorld.at(nCntEffect)._43 = 0.0f;

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_vPos.at(nCntEffect).x, m_vPos.at(nCntEffect).y, m_vPos.at(nCntEffect).z);
		D3DXMatrixMultiply(&m_vMtxWorld.at(nCntEffect), &m_vMtxWorld.at(nCntEffect), &mtxTrans);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-m_vRadius.at(nCntEffect), m_vRadius.at(nCntEffect), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_vRadius.at(nCntEffect), m_vRadius.at(nCntEffect), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-m_vRadius.at(nCntEffect), -m_vRadius.at(nCntEffect), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_vRadius.at(nCntEffect), -m_vRadius.at(nCntEffect), 0.0f);

		//頂点座標の設定
		pVtx[0].pos = *D3DXVec3TransformCoord(&pVtx[0].pos, &pVtx[0].pos, &m_vMtxWorld.at(nCntEffect));
		pVtx[1].pos = *D3DXVec3TransformCoord(&pVtx[1].pos, &pVtx[1].pos, &m_vMtxWorld.at(nCntEffect));
		pVtx[2].pos = *D3DXVec3TransformCoord(&pVtx[2].pos, &pVtx[2].pos, &m_vMtxWorld.at(nCntEffect));
		pVtx[3].pos = *D3DXVec3TransformCoord(&pVtx[3].pos, &pVtx[3].pos, &m_vMtxWorld.at(nCntEffect));

		// 頂点カラーの設定
		pVtx[0].col = m_vCol.at(nCntEffect);
		pVtx[1].col = m_vCol.at(nCntEffect);
		pVtx[2].col = m_vCol.at(nCntEffect);
		pVtx[3].col = m_vCol.at(nCntEffect);

		// ポインタを進める
		pVtx += 4;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);		// ライトを無効にする

	// αブレンディングを加算合成して設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// Zバッファ更新の有効/無効の設定

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtx);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	// エフェクトの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,		// STRIPではなくLISTで指定
		0,
		0,
		m_nNum * 4,											// 頂点数
		0,
		m_nNum * 2);										// 一枚のビルボードに三角形を2つ

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// αブレンディングを戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);		// ライトを有効にする
}

//=============================================================================
// 生成
//=============================================================================
void CBatchingEffect::Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, TYPE type)
{
	if (m_nNum >= m_nMax)
	{// 最大数を超えるなら早期終了
		return;
	}

	// 各配列に追加
	m_vPos.push_back(pos);
	m_vRot.push_back(rot);
	m_vCol.push_back(col);
	m_vRadius.push_back(fRadius);
	m_vType.push_back(type);

	D3DXMATRIX mtx;		// 単位行列
	m_vMtxWorld.push_back(mtx);

	// 総数を増やす
	m_nNum++;
}

//=============================================================================
// 全消去
//=============================================================================
void CBatchingEffect::Reset(void)
{
	// 全部クリアする
	m_vMtxWorld.clear();
	m_vPos.clear();
	m_vRot.clear();
	m_vCol.clear();
	m_vRadius.clear();
	m_vType.clear();
	m_nNum = 0;
}

//=============================================================================
// 交換
//=============================================================================
void CBatchingEffect::SwapAndPop(int nIdx)
{
	if (nIdx == m_nNum - 1)
	{
		// 各要素の最後尾を消去
		m_vMtxWorld.pop_back();
		m_vPos.pop_back();
		m_vRot.pop_back();
		m_vCol.pop_back();
		m_vRadius.pop_back();
		m_vType.pop_back();
	}
	else
	{
		// 各要素を最後尾から上書き&最後尾を消去
		m_vMtxWorld.at(nIdx) = m_vMtxWorld.back();
		m_vMtxWorld.pop_back();

		m_vPos.at(nIdx) = m_vPos.back();
		m_vPos.pop_back();

		m_vRot.at(nIdx) = m_vRot.back();
		m_vRot.pop_back();

		m_vCol.at(nIdx) = m_vCol.back();
		m_vCol.pop_back();

		m_vRadius.at(nIdx) = m_vRadius.back();
		m_vRadius.pop_back();

		m_vType.at(nIdx) = m_vType.back();
		m_vType.pop_back();
	}

	// 使用数を減らす
	m_nNum--;
}