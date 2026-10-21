//=============================================================================
//
//	シーンクラス [scene.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "main.h"
#include "scene.h"

#include "testscene.h"
#include "title.h"
#include "game.h"
#include "result.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CScene::ENDFLAG CScene::m_endFlag = CScene::ENDFLAG::ENDFLAG_NONE;		// 終了フラグ

//=============================================================================
// 生成
//=============================================================================
CScene* CScene::Create(MODE mode)
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
	CScene* pScene = NULL;		// ポインタ保存用

	// シーンのインスタンス生成
	switch (mode)
	{
	case CScene::MODE_NONE:

		break;

		// ゲーム画面
	case CScene::MODE_TESTSCENE:
		pScene = new CTestScene;
		break;

		// タイトル画面
	case CScene::MODE_TITLE:
		pScene = new CTitle;
		break;

		// ゲーム画面
	case CScene::MODE_GAME:
		pScene = new CGame;
		break;

		// リザルト画面
	case CScene::MODE_RESULT:
		pScene = new CResult;
		break;

	case CScene::MODE_MAX:

		break;

	default:

		break;
	}

	// 初期化処理
	pScene->Init();

	// 生成したオブジェクトのポインタを返す
	return pScene;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CScene::CScene(MODE mode, int nPriority) : CObject(nPriority)
{
	// モードの設定
	m_mode = mode;
}

//=============================================================================
// デストラクタ
//=============================================================================
CScene::~CScene()
{

}