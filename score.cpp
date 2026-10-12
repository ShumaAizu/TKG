//=============================================================================
//
//	スコアクラス [score.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "object.h"
#include "score.h"
#include "renderer.h"
#include "input.h"
#include "number.h"

#include "debugproc.h"

#include <math.h>

//=============================================================================
// 生成処理
//=============================================================================
CScore* CScore::Create(const D3DXVECTOR3 pos, const D3DXCOLOR col, const float fWidth, const float fHeight, const int nScore)
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
	CScore* pScore = NULL;		// ポインタ保存用

	// オブジェクト2Dのインスタンス生成
	pScore = new CScore;

	// 初期化処理
	pScore->Init(pos, col, fWidth, fHeight, nScore);

	// 生成したオブジェクトのポインタを返す
	return pScore;
}

//=============================================================================
// コンストラクタ (オーバーロード)
//=============================================================================
CScore::CScore(int nPriority) : CObject(nPriority)
{
	// 各メンバ変数のクリア
	memset(&m_apNumber[0], NULL, sizeof(CNumber*) * MAX_NUM);
	m_nNum = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CScore::~CScore()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScore::Init(void)
{
	

	return S_OK;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScore::Init(const D3DXVECTOR3 pos, const D3DXCOLOR col, const float fWidth, const float fHeight, const int nScore)
{
	// ローカル変数宣言
	D3DXVECTOR3 numPos = {};

	// 各値代入
	m_pos = pos;
	m_col = col;
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_nNum = nScore;

	for (int nCntNum = 0; nCntNum < MAX_NUM; nCntNum++)
	{
		// 位置を設定
		numPos = D3DXVECTOR3(m_pos.x + (m_fWidth * nCntNum), m_pos.y, 0.0f);

		// ナンバーの生成
		m_apNumber[nCntNum] = CNumber::Create(numPos, m_col, m_fWidth, m_fHeight);
		m_apNumber[nCntNum]->SetNum(m_nNum % (int)powf(10, (MAX_NUM - nCntNum)) / (int)(powf(10, (MAX_NUM - nCntNum)) / 10));
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScore::Uninit(void)
{
	// 終了処理
	for (int nCntNum = 0; nCntNum < MAX_NUM; nCntNum++)
	{
		if (m_apNumber[nCntNum] == NULL)
		{// 使っていなければスキップ
			continue;
		}

		// ナンバーの終了処理
		m_apNumber[nCntNum]->Uninit();

		// 開放
		delete m_apNumber[nCntNum];
		m_apNumber[nCntNum] = NULL;
	}

	// 開放
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CScore::Update(void)
{
	for (int nCntNum = 0; nCntNum < MAX_NUM; nCntNum++)
	{
		m_apNumber[nCntNum]->SetNum(m_nNum % (int)powf(10, (MAX_NUM - nCntNum)) / (int)(powf(10, (MAX_NUM - nCntNum)) / 10));
	}

	// 更新処理
	for (int nCntNum = 0; nCntNum < MAX_NUM; nCntNum++)
	{
		if (m_apNumber[nCntNum] == NULL)
		{// 使っていなければスキップ
			continue;
		}

		// ナンバーの更新処理
		m_apNumber[nCntNum]->Update();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CScore::Draw(void)
{
	// 描画処理
	for (int nCntNum = 0; nCntNum < MAX_NUM; nCntNum++)
	{
		if (m_apNumber[nCntNum] == NULL)
		{// 使っていなければスキップ
			continue;
		}

		// ナンバーの描画処理
		m_apNumber[nCntNum]->Draw();
	}
}

//=============================================================================
// スコア加算処理
//=============================================================================
void CScore::Add(const int nValue)
{
	m_nNum += nValue;
}