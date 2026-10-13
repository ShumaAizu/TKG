//=============================================================================
//
//	マネージャクラス [manager.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "texture.h"
#include "debugproc.h"
#include "object.h"
#include "object3D.h"
#include "objectX.h"
#include "camera.h"
#include "light.h"

#include "objectloader.h"
#include "player.h"
#include "field.h"
#include "wall.h"
#include "item.h"
#include "score.h"

#include "batchingeffect.h"

#include "tcp_client.h"
#include "protocol_ranking.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PORTID				(22333)		// 接続を受け付けるポート番号
#define MAX_RECVDATA		(5)			// 送るデータの最大
#define MAX_RANK			(10)		// 最大ランク数

//=============================================================================
// コンストラクタ
//=============================================================================
CManager::CManager()
{
	// 各変数クリア
	m_pRenderer = nullptr;
	m_pInputKeyboard = nullptr;
	m_pInputMouse = nullptr;
	m_pInputJoypad = nullptr;
	m_pSound = nullptr;
	m_pTexture = nullptr;
	m_pCamera = nullptr;
	m_pLight = nullptr;
	m_pScore = nullptr;
	m_pTimer = nullptr;

	m_pObject3D = nullptr;

	m_pObjectLoader = nullptr;

	m_pImGui = nullptr;
	m_pCanvas = nullptr;

	m_pBatchingEffect = nullptr;

	m_pFade = nullptr;
	m_pPause = nullptr;

	m_pTcpClient = nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
#ifndef _DEBUG
	// マウスカーソルを非表示
	ShowCursor(false);
#endif

	// Winsock初期化
	WSADATA wsaData;
	int nErr = WSAStartup(WINSOCK_VERSION, &wsaData);

	if (nErr != 0)
	{// 初期化に失敗したなら
		return -1;
	}

	// レンダラーのインスタンスを生成
	if (m_pRenderer == nullptr)
	{
		m_pRenderer = new CRenderer;
	}

	if (m_pRenderer == nullptr)
	{// 生成に失敗していたら
		return -1;
	}

	// 初期化処理
	if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
	{// 失敗した場合
		return -1;
	}

	// キーボードのインスタンスを生成
	if (m_pInputKeyboard == nullptr)
	{
		m_pInputKeyboard = new CInputKeyboard;
	}

	if (m_pInputKeyboard == nullptr)
	{// 生成に失敗していたら
		return -1;
	}

	// 初期化処理
	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
	{// 失敗した場合
		return -1;
	}

	// マウスのインスタンスを生成
	if (m_pInputMouse == nullptr)
	{
		m_pInputMouse = new CInputMouse;
	}

	if (m_pInputMouse == nullptr)
	{// 生成に失敗していたら
		return -1;
	}

	// 初期化処理
	if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))
	{// 初期化処理が失敗した場合
		return -1;
	}

	// ジョイパッドのインスタンスを生成
	if (m_pInputJoypad == nullptr)
	{
		m_pInputJoypad = new CInputJoypad;
	}

	if (m_pInputJoypad == nullptr)
	{// 生成に失敗していたら
		return -1;
	}

	// 初期化処理
	if (FAILED(m_pInputJoypad->Init()))
	{// 初期化処理が失敗した場合
		return -1;
	}

	// サウンドのインスタンスを生成
	if (m_pSound == nullptr)
	{
		m_pSound = new CSound;
	}

	if (m_pSound == nullptr)
	{// 生成に失敗していたら
		return -1;
	}

	// 初期化処理
	if (FAILED(m_pSound->Init(hWnd)))
	{// 失敗した場合
		return -1;
	}

	// テクスチャのインスタンスを生成
	if (m_pTexture == nullptr)
	{
		m_pTexture = new CTexture;
	}

	if (m_pTexture == nullptr)
	{// 生成に失敗していたら
		return -1;
	}

	// 初期化処理
	if (FAILED(m_pTexture->Load()))
	{// 失敗した場合
		return -1;
	}

	// ライトのインスタンスを生成
	if (m_pLight == nullptr)
	{
		m_pLight = new CLight;
	}

	if (m_pLight == nullptr)
	{// 生成に失敗していたら
		return -1;
	}

	// 初期化処理
	if (FAILED(m_pLight->Init()))
	{// 失敗した場合
		return -1;
	}

	// カメラのインスタンスを生成
	if (m_pCamera == nullptr)
	{
		m_pCamera = new CCamera;
	}

	if (m_pCamera == nullptr)
	{// 生成に失敗していたら
		return -1;
	}

	// 初期化処理
	if (FAILED(m_pCamera->Init()))
	{// 失敗した場合
		return -1;
	}

	// オブジェクトローダーのインスタンスを生成
	if (m_pObjectLoader == nullptr)
	{
		m_pObjectLoader = new CObjectLoader;
	}

	if (m_pObjectLoader == nullptr)
	{// 生成に失敗していたら
		return -1;
	}

	// バッチ描画エフェクトのインスタンスを生成
	if (m_pBatchingEffect == nullptr)
	{
		m_pBatchingEffect = new CBatchingEffect;
	}

	if (m_pBatchingEffect == nullptr)
	{// 生成に失敗していたら
		return -1;
	}

	// 初期化処理
	if (FAILED(m_pBatchingEffect->Init(10000, "data//TEXTURE//effect001.jpg")))
	{// 失敗した場合
		return -1;
	}

	// クライアントのインスタンスを生成
	if (m_pTcpClient == nullptr)
	{
		m_pTcpClient = new CTcpClient;
	}

	if (m_pTcpClient == nullptr)
	{// 生成に失敗していたら
		return -1;
	}

	m_pScore = CScore::Create(D3DXVECTOR3(17.5f, 650.0f, 0.0f), COLOR_WHITE, 35.0f, 70.0f, 75);

	//CObject3D::Create();

	//CObjectX::Create();

	//CField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 250.0f, 250.0f, "data//TEXTURE//field000.jpg");

	//CWall::Create(D3DXVECTOR3(0.0f, 250.0f, 250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 250.0f, 250.0f, "data//TEXTURE//wall000.jpg");

	m_pObjectLoader->Load("data//SCRIPTS//Test000.txt");

	// デバッグ表示の初期化
	CDebugProc::Init();

	// 正常終了
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CManager::Uninit(void)
{
	// 全てのオブジェクトの開放
	CObject::ReleaseAll();

	// デバッグ表示の終了
	CDebugProc::Uninit();

	// サウンドストップ
	m_pSound->Stop();

	if (m_pTcpClient != nullptr)
	{// クライアントを生成していたなら
		// 開放
		m_pTcpClient->Uninit();
		delete m_pTcpClient;
		m_pTcpClient = nullptr;
	}

	if (m_pBatchingEffect != nullptr)
	{// オブジェクトローダーを生成していたなら
		// 開放
		m_pBatchingEffect->Uninit();
		delete m_pBatchingEffect;
		m_pBatchingEffect = nullptr;
	}

	if (m_pObjectLoader != nullptr)
	{// オブジェクトローダーを生成していたなら
		// 開放
		m_pObjectLoader->Uninit();
		delete m_pObjectLoader;
		m_pObjectLoader = nullptr;
	}

	if (m_pCamera != nullptr)
	{// カメラを生成していたなら

		// 終了処理
		m_pCamera->Uninit();

		// 開放
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	if (m_pLight != nullptr)
	{// ライトを生成していたなら

		// 終了処理
		m_pLight->Uninit();

		// 開放
		delete m_pLight;
		m_pLight = nullptr;
	}

	if (m_pTexture != nullptr)
	{// テクスチャを生成していたなら

		// 終了処理
		m_pTexture->Unload();

		// 開放
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	if (m_pSound != nullptr)
	{// サウンドを生成していたなら

		// 終了処理
		m_pSound->Uninit();

		// 開放
		delete m_pSound;
		m_pSound = nullptr;
	}

	if (m_pInputKeyboard != nullptr)
	{// キーボードを生成していたなら

		// 終了処理
		m_pInputKeyboard->Uninit();

		// 開放
		delete m_pInputKeyboard;
		m_pInputKeyboard = nullptr;
	}

	if (m_pInputMouse != nullptr)
	{// マウスを生成していたなら

		// 終了処理
		m_pInputMouse->Uninit();

		// 開放
		delete m_pInputMouse;
		m_pInputMouse = nullptr;
	}

	if (m_pInputJoypad != nullptr)
	{// ジョイパッドを生成していたなら

		// 終了処理
		m_pInputJoypad->Uninit();

		// 開放
		delete m_pInputJoypad;
		m_pInputJoypad = nullptr;
	}

	if (m_pRenderer != nullptr)
	{// レンダラーを生成していたなら

		// 終了処理
		m_pRenderer->Uninit();

		// 開放
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	// Winsockの終了処理
	WSACleanup();
}

//=============================================================================
// 更新処理
//=============================================================================
void CManager::Update(void)
{
	// キーボードの更新処理
	if (m_pInputKeyboard != nullptr)
	{
		m_pInputKeyboard->Update();
	}

	// マウスの更新処理
	if (m_pInputMouse != nullptr)
	{
		m_pInputMouse->Update();
		m_pInputMouse->UpdateMouseState();
	}

	// ジョイパッドの更新処理
	if (m_pInputJoypad != nullptr)
	{
		m_pInputJoypad->Update();
	}

	// レンダラーの更新処理
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Update();
	}

	// バッチ描画エフェクトの更新処理
	if (m_pBatchingEffect != nullptr)
	{
		m_pBatchingEffect->Update();
	}

	if (m_pInputKeyboard->GetTrigger(DIK_BACK) == true)
	{// バックスペースでリセット
		CObject::ReleaseAll();

		CObject3D::Create();

		//CObjectX::Create();

		m_pScore = CScore::Create(D3DXVECTOR3(17.5f, 650.0f, 0.0f), COLOR_WHITE, 35.0f, 70.0f, 75);

		m_pObjectLoader->Load("data//SCRIPTS//Test000.txt");
	}

	if (m_pInputKeyboard->GetRepeat(DIK_F1) == true)
	{
		m_pScore->Add(-1);
	}

	if (m_pInputKeyboard->GetRepeat(DIK_F2) == true)
	{
		m_pScore->Add(1);
	}

	// ランキング機能の仮置き
	RankingTest();
}

//=============================================================================
// 描画処理
//=============================================================================
void CManager::Draw(void)
{
	// レンダラーの描画処理
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Draw();
	}
}

//=============================================================================
// マネージャーの取得
//=============================================================================
CManager* CManager::GetInstance(void)
{
	// 最初に呼び出されたら実体が生まれる
	static CManager manager;
	return &manager;
}

////=============================================================================
//// モードの設定
////=============================================================================
//void CManager::SetMode(CScene::MODE mode)
//{
//	// サウンドの停止
//	m_pSound->Stop();
//
//	// 現在のモードの破棄
//	if (m_pScene != nullptr)
//	{
//		// 終了処理
//		m_pScene->Uninit();
//		m_pScene = nullptr;
//	}
//
//	// すべてのオブジェクトの破棄
//	CObject::ReleaseAll();
//
//	// 新しいモードの生成
//	m_pScene = CScene::Create(mode);
//}
//
////=============================================================================
//// モードの取得
////=============================================================================
//CScene::MODE CManager::GetMode(void)
//{
//	return m_pScene->GetMode();
//}

//=============================================================================
// ランキング実験用
//=============================================================================
void CManager::RankingTest(void)
{
	int nRecvByte = 0;

	if (m_pInputKeyboard->GetTrigger(DIK_RETURN) == true && m_pInputKeyboard->GetPress(DIK_LCONTROL) == true)
	{// 登録
		m_pTcpClient->Init();

		// サーバーへの接続
		if (m_pTcpClient->Connect("127.0.0.1", PORTID) == false)
		{
			return;
		}

		unsigned char* pRecvData = nullptr;
		pRecvData = new unsigned char[MAX_RECVDATA];
		int nRank = -1;

		int nScore = m_pScore->Get();

		nScore = htonl(nScore);

		memset(pRecvData, (unsigned char)COMMAND_TYPE_SET_RANKING, sizeof(char));
		memcpy(&pRecvData[sizeof(char)], &nScore, sizeof(int));

		// データ送信
		nRecvByte = m_pTcpClient->Send(pRecvData, MAX_RECVDATA);

		// データ受信
		m_pTcpClient->Recv(&nRank);

		nRank = ntohl(nRank);

		if (pRecvData != nullptr)
		{// 送信用変数を使用していたら開放
			delete[] pRecvData;
			pRecvData = nullptr;
		}

		m_pTcpClient->Uninit();
	}

	if (m_pInputKeyboard->GetTrigger(DIK_RETURN) == true && m_pInputKeyboard->GetPress(DIK_LSHIFT) == true)
	{// 取得
		m_pTcpClient->Init();

		// サーバーへの接続
		if (m_pTcpClient->Connect("127.0.0.1", PORTID) == false)
		{
			return;
		}

		unsigned char cRecvData = COMMAND_TYPE_GET_RANKING;

		// データ送信
		nRecvByte = m_pTcpClient->Send(&cRecvData);

		int aRankingData[MAX_RANK];

		// データ受信
		m_pTcpClient->Recv(&aRankingData);

		for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{
			aRankingData[nCntRank] = ntohl(aRankingData[nCntRank]);
			CScore::Create(D3DXVECTOR3(1180.0f, 35.0f * (nCntRank + 1), 0.0f), COLOR_WHITE, 35.0f, 70.0f, aRankingData[nCntRank]);
		}

		m_pTcpClient->Uninit();
	}
}