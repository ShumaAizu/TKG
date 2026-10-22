//=============================================================================
//
//	テスト画面クラス [testscene.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "testscene.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "debugproc.h"

#include "player.h"
#include "objectloader.h"

#include "object3D.h"
#include "batchingeffect.h"
#include "ui.h"
#include "fade.h"

#include "meshcylinder.h"

#include "number.h"
#include "score.h"
#include "timer.h"
#include "gimmick.h"
#include "rotationgimmick.h"

#include "ball.h"

#include <vector>
#include <string>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SAVE_COOLTIME		(5000)			// 保存クールタイム
#define FIELD_RADIUSIN		(3750.0f)		// フィールドの広さ (内径)
#define FIELD_RADIUSOUT		(4500.0f)		// フィールドの広さ (外径)
#define STARRANGE_RADIUS	(750000.0f)		// 星と中心との距離の半径
#define VIEW_TUTORIAL_TIME	(600)			// チュートリアル表示時間

//=============================================================================
// コンストラクタ (オーバーロード)
//=============================================================================
CTestScene::CTestScene(int nPriority) : CScene(MODE::MODE_GAME, nPriority)
{
	// メンバ変数クリア
	m_pPlayer = nullptr;
	m_pObjectLoader = nullptr;
	m_pTutorial = nullptr;
	m_pScore = nullptr;
	m_nCntTutorial = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CTestScene::~CTestScene()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTestScene::Init(void)
{
	// オブジェクトローダーのインスタンスを生成
	if (m_pObjectLoader == nullptr)
	{
		m_pObjectLoader = new CObjectLoader;
	}

	if (m_pObjectLoader == nullptr)
	{// 生成に失敗していたら
		return -1;
	}

	//CGimmick::Create(INIT_D3DXVEC3, INIT_D3DXVEC3, "data//MODEL//building000.x");

	//CRotationGimmick::Create(D3DXVECTOR3(0.0f, -50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f), 75.0f, 250.0f, 75.0f, "data//MODEL//TestBox000.x");

	m_vpRotGimmick.push_back(CRotationGimmick::Create(D3DXVECTOR3(0.0f, 500.0f, -550.0f), D3DXVECTOR3(-0.75f, 0.0f, 0.0f), 150.0f, 12.5f, 50.0f, "data//MODEL//TestBox001.x"));

	m_vpRotGimmick.push_back(CRotationGimmick::Create(D3DXVECTOR3(175.0f, 300.0f, -250.0f), D3DXVECTOR3(-0.75f, 0.0f, 0.0f), 150.0f, 12.5f, 50.0f, "data//MODEL//TestBox001.x"));

	m_vpRotGimmick.push_back(CRotationGimmick::Create(D3DXVECTOR3(-175.0f, 100.0f, -50.0f), D3DXVECTOR3(-0.75f, 0.0f, 0.0f), 150.0f, 12.5f, 50.0f, "data//MODEL//TestBox001.x"));


	//CRotationGimmick::Create(D3DXVECTOR3(0.0f, -50.0f, -0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 75.0f, 250.0f, 75.0f, "data//MODEL//TestBox000.x");

	CBall::Create(D3DXVECTOR3(0.0f, 1500.0f, -1250.0f), 10.0f, 10.0f, 10.0f);

	m_pObjectLoader->Load("data//SCRIPTS//Test000.txt");

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTestScene::Uninit(void)
{
	// フィードバックエフェクト解除
	CManager* pManager = CManager::GetInstance();
	pManager->GetRenderer()->ResetEnableFeedback();

	if (m_pObjectLoader != nullptr)
	{// オブジェクトローダーを生成していたなら
		// 開放
		m_pObjectLoader->Uninit();
		delete m_pObjectLoader;
		m_pObjectLoader = nullptr;
	}

	m_vpRotGimmick.clear();

	// エフェクトリセット
	pManager->GetBatchingEffect()->Reset();

	// 開放
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTestScene::Update(void)
{
	// 入力デバイス取得
	CManager* pManager = CManager::GetInstance();
	CInputKeyboard* pInputKeyboard = pManager->GetInputKeyboard();
	CInputJoypad* pInputJoypad = pManager->GetInputJoypad();

	static int nSelect = 0;

	if (pInputKeyboard->GetTrigger(DIK_F8) == true)
	{
		if (nSelect > 0)
		{
			nSelect--;
		}
	}

	if (pInputKeyboard->GetTrigger(DIK_F9) == true)
	{
		if (nSelect < m_vpRotGimmick.size() - 1)
		{
			nSelect++;
		}
	}

	m_vpRotGimmick.at(nSelect)->Move();

	// 終了条件
	switch (GetEndFlag())
	{
		// クリアしていれば
	case ENDFLAG::ENDFLAG_CLEAR:
		pManager->GetFade()->Set(MODE_RESULT, COLOR_BLACK);
		break;

		// 失敗していたら
	case ENDFLAG::ENDFLAG_FAILURE:
		pManager->GetFade()->Set(MODE_RESULT, COLOR_BLACK);
		break;
	}

	if ((pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
		pInputJoypad->GetTrigger(CInputJoypad::KEY_START) == true) &&
		pManager->GetFade()->Get() == CFade::STATE::STATE_NONE)
	{
		pManager->GetFade()->Set(MODE_RESULT, COLOR_BLACK);
	}

	if (pInputKeyboard->GetTrigger(DIK_BACK) == true)
	{// バックスペースでリセット
		pManager->SetMode(MODE_TESTSCENE);
	}

	CDebugProc::Print("TEST\n");
}

//=============================================================================
// 描画処理
//=============================================================================
void CTestScene::Draw(void)
{
	
}