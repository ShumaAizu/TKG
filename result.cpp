//=============================================================================
//
//	リザルト画面クラス [result.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "result.h"
#include "input.h"
#include "ui.h"
#include "fade.h"
#include "batchingeffect.h"
#include "sound.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FIELD_RADIUSIN		(4250.0f)		// フィールドの広さ (内径)
#define FIELD_RADIUSOUT		(5000.0f)		// フィールドの広さ (外径)
#define STARRANGE_RADIUS	(750000.0f)		// 星と中心との距離の半径
#define FADE_COUNTER		(600)			// 画面遷移カウンタ

//=============================================================================
// コンストラクタ (オーバーロード)
//=============================================================================
CResult::CResult(int nPriority) : CScene(MODE::MODE_RESULT, nPriority)
{
	// メンバ変数クリア
	m_nFadeCounter = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CResult::Init(void)
{
	// サウンドのインスタンス取得
	CManager* pManager = CManager::GetInstance();
	CSound* pSound = pManager->GetSound();

	// フラグを元に戻す
	SetEndFlag(CScene::ENDFLAG::ENDFLAG_NONE);

	m_nFadeCounter = 0;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CResult::Uninit(void)
{
	// エフェクトリセット
	CManager::GetInstance()->GetBatchingEffect()->Reset();

	// 開放
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CResult::Update(void)
{
	// 入力デバイス取得
	CManager* pManager = CManager::GetInstance();
	CInputKeyboard* pInputKeyboard = pManager->GetInputKeyboard();
	CInputJoypad* pInputJoypad = pManager->GetInputJoypad();

	if ((pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
		pInputJoypad->GetTrigger(CInputJoypad::KEY_A) == true) &&
		pManager->GetFade()->Get() == CFade::STATE::STATE_NONE)
	{
		pManager->GetFade()->Set(MODE_TITLE, COLOR_BLACK);
	}

	CDebugProc::Print("RESULT\n");
}

//=============================================================================
// 描画処理
//=============================================================================
void CResult::Draw(void)
{
	
}