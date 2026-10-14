//=============================================================================
//
//	タイマークラス [timer.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "texture.h"
#include "object.h"
#include "object2D.h"
#include "timer.h"
#include "renderer.h"
#include "input.h"
#include "number.h"

#include "debugproc.h"

#include <math.h>

//=============================================================================
// 生成処理
//=============================================================================
CTimer* CTimer::Create(const D3DXVECTOR3 pos, const D3DXCOLOR col, const float fWidth, const float fHeight)
{
	// 最大チェック
	if (GetNumAll() >= MAX_OBJECT)
	{// これ以上生成できなければ
		// NULLを返し終了
		return NULL;
	}

	// ローカル変数宣言
	CTimer* pTimer = NULL;		// ポインタ保存用

	// タイマーのインスタンス生成
	pTimer = new CTimer;

	// 初期化処理
	pTimer->Init(pos, col, fWidth, fHeight);

	// 生成したインスタンスのポインタを返す
	return pTimer;
}

//=============================================================================
// コンストラクタ (オーバーロード)
//=============================================================================
CTimer::CTimer(int nPriority) : CObject(nPriority)
{
	// 各メンバ変数のクリア
	memset(&m_apTimerNumberMs, NULL, sizeof(CNumber*) * MAX_NUMMS);
	m_pDot = NULL;
	m_nTimer = 0;
	m_nTimerMs = 0;
	m_dwExecLastTime = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_isUse = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CTimer::~CTimer()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTimer::Init(void)
{
	return S_OK;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTimer::Init(const D3DXVECTOR3 pos, const D3DXCOLOR col, const float fWidth, const float fHeight)
{
	// テクスチャインスタンスへのポインタを取得
	CManager* pManager = CManager::GetInstance();
	CTexture* pTexture = pManager->GetTextrue();

	// ローカル変数宣言
	D3DXVECTOR3 numPos = {};

	// 各値代入
	m_pos = pos;
	m_col = col;
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_nTimer = 0;
	m_isUse = true;
	m_dwExecLastTime = timeGetTime();

	// 桁数
	int nNumDigit = 1;
	int nCntNum = 0;

	for (nCntNum = 0; nCntNum < nNumDigit; nCntNum++)
	{
		// 位置を設定
		numPos = D3DXVECTOR3(m_pos.x + (fWidth * nCntNum), m_pos.y, 0.0f);

		// ナンバーの生成
		m_vpTimerNumber.push_back(CNumber::Create(numPos, col, fWidth, fHeight));
		m_vpTimerNumber.at(nCntNum)->SetNum(m_nTimer % (int)powf(10, (nNumDigit - nCntNum)) / (int)(powf(10, (nNumDigit - nCntNum)) / 10));
	}

	// 位置を設定
	float m_fDotWidth = fWidth * 0.75f;
	numPos = D3DXVECTOR3(m_pos.x + (fWidth * 0.75f), m_pos.y, 0.0f);
	m_pDot = CObject2D::Create(D3DXVECTOR3(numPos.x + 5.0f, numPos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI), fWidth * 0.25f, fHeight * 0.25f, 6);
	m_pDot->SetIdxTexture(pTexture->Register("data/TEXTURE/colon000.png"));

	for (nCntNum = 0; nCntNum < MAX_NUMMS; nCntNum++)
	{
		// 位置を設定
		numPos = D3DXVECTOR3(m_pos.x + ((fWidth * nCntNum) + (fWidth * nNumDigit) + m_fDotWidth), m_pos.y, 0.0f);

		// ナンバーの生成
		m_apTimerNumberMs[nCntNum] = CNumber::Create(numPos, col, fWidth, fHeight);
		m_apTimerNumberMs[nCntNum]->SetNum(m_nTimer % (int)powf(10, (MAX_NUMMS - nCntNum)) / (int)(powf(10, (MAX_NUMMS - nCntNum)) / 10));
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTimer::Uninit(void)
{
	for (int nCntNum = 0; nCntNum < m_vpTimerNumber.size(); nCntNum++)
	{// 秒数の終了処理
		if (m_vpTimerNumber.at(nCntNum) == NULL)
		{// 使っていなければスキップ
			continue;
		}

		// ナンバーの終了処理
		m_vpTimerNumber.at(nCntNum)->Uninit();

		// 開放
		delete m_vpTimerNumber.at(nCntNum);
		m_vpTimerNumber.at(nCntNum) = NULL;
	}

	for (int nCntNum = 0; nCntNum < MAX_NUMMS; nCntNum++)
	{// ミリ秒数の終了処理
		if (m_apTimerNumberMs[nCntNum] == NULL)
		{// 使っていなければスキップ
			continue;
		}

		// ナンバーの終了処理
		m_apTimerNumberMs[nCntNum]->Uninit();

		// 開放
		delete m_apTimerNumberMs[nCntNum];
		m_apTimerNumberMs[nCntNum] = NULL;
	}

	// 開放
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTimer::Update(void)
{
	if (m_isUse == false)
	{// 使用状態でなければ早期終了
		return;
	}

	// ローカル変数
	DWORD dwCurrentTime;

	dwCurrentTime = timeGetTime();	// 現在時刻を取得
	m_nTimerMs += dwCurrentTime - m_dwExecLastTime;		// ミリ秒を計測

	CDebugProc::Print("%d\n", m_nTimerMs);

	if (m_nTimerMs > 1000)
	{ // もしカウントが1000を超えていたら
		m_nTimer++;							// 秒数を増やす
		m_nTimerMs = m_nTimerMs - 1000;		// 差分を減算して戻す

		if (m_nTimer >= powf(10, m_vpTimerNumber.size()))
		{// もし桁が変わっていたら
			AddDigit();
		}
	}

	// 今の時間を最終計測時間に
	m_dwExecLastTime = dwCurrentTime;

	// 桁数
	int nNumDigit = m_vpTimerNumber.size();
	int nCntNum = 0;

	for (nCntNum = 0; nCntNum < nNumDigit; nCntNum++)
	{// 秒数の再設定
		m_vpTimerNumber.at(nCntNum)->SetNum(m_nTimer % (int)powf(10, (nNumDigit - nCntNum)) / (int)(powf(10, (nNumDigit - nCntNum)) / 10));
	}

	for (nCntNum = 0; nCntNum < MAX_NUMMS; nCntNum++)
	{// ミリ秒数の再設定
		m_apTimerNumberMs[nCntNum]->SetNum(m_nTimerMs % (int)powf(10, (MAX_NUMMS - nCntNum)) / (int)(powf(10, (MAX_NUMMS - nCntNum)) / 10));
	}

	for (nCntNum = 0; nCntNum < nNumDigit; nCntNum++)
	{// 秒数の更新
		if (m_vpTimerNumber.at(nCntNum) == NULL)
		{// 使っていなければスキップ
			continue;
		}

		// ナンバーの更新処理
		m_vpTimerNumber.at(nCntNum)->Update();
	}

	for (nCntNum = 0; nCntNum < MAX_NUMMS; nCntNum++)
	{// ミリ秒数の更新
		if (m_apTimerNumberMs[nCntNum] == NULL)
		{// 使っていなければスキップ
			continue;
		}

		// ナンバーの更新処理
		m_apTimerNumberMs[nCntNum]->Update();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTimer::Draw(void)
{
	if (m_isUse == false)
	{// 使用状態がオフなら
		// 早期終了
		return;
	}

	for (int nCntNum = 0; nCntNum < m_vpTimerNumber.size(); nCntNum++)
	{// 秒数の描画
		if (m_vpTimerNumber.at(nCntNum) == NULL)
		{// 使っていなければスキップ
			continue;
		}

		// ナンバーの描画処理
		m_vpTimerNumber.at(nCntNum)->Draw();
	}

	for (int nCntNum = 0; nCntNum < MAX_NUMMS; nCntNum++)
	{// ミリ秒数の描画
		if (m_apTimerNumberMs[nCntNum] == NULL)
		{// 使っていなければスキップ
			continue;
		}

		// ナンバーの描画処理
		m_apTimerNumberMs[nCntNum]->Draw();
	}
}

//=============================================================================
// 使用状態設定処理
//=============================================================================
void CTimer::SetUse(const bool isUse)
{
	m_isUse = isUse;
}

//=============================================================================
// 数値の位置再設定処理
//=============================================================================
void CTimer::AddDigit(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 numPos = {};
	float fWidth = 0.0f;
	int nCntNum = 0;

	for (nCntNum = 0; nCntNum < m_vpTimerNumber.size(); nCntNum++)
	{
		// 位置を設定
		fWidth = m_vpTimerNumber.at(nCntNum)->GetSize().x;
		numPos = D3DXVECTOR3(m_pos.x + (fWidth * nCntNum), m_pos.y, 0.0f);
		m_vpTimerNumber.at(nCntNum)->SetPos(numPos);
	}

	// 位置を設定
	numPos = D3DXVECTOR3(m_pos.x + (m_fWidth * nCntNum), m_pos.y, 0.0f);

	// 桁を追加
	m_vpTimerNumber.push_back(CNumber::Create(numPos, m_col, m_fWidth, m_fHeight));
	nCntNum++;

	// 桁数
	int nNumDigit = m_vpTimerNumber.size();

	// 位置を設定
	float m_fDotWidth = fWidth * 0.75f;
	numPos = D3DXVECTOR3(m_pos.x + (m_fWidth * nCntNum) - m_fWidth * 0.25f, m_pos.y, 0.0f);
	m_pDot->SetPosition(D3DXVECTOR3(numPos.x + 5.0f, numPos.y, 0.0f));

	for (nCntNum = 0; nCntNum < MAX_NUMMS; nCntNum++)
	{
		// 位置を設定
		fWidth = m_apTimerNumberMs[nCntNum]->GetSize().x;
		numPos = D3DXVECTOR3(m_pos.x + ((fWidth * nCntNum) + (fWidth * nNumDigit) + m_fDotWidth), m_pos.y, 0.0f);
		m_apTimerNumberMs[nCntNum]->SetPos(numPos);
	}
}