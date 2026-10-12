//=============================================================================
//
//	カメラクラス [camera.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define INIT_VECU		(D3DXVECTOR3(0.0f, 1.0f, 0.0f))			// ベクトル初期値
#define CAMERAPOSR_DIS			(175.0f)						// 視点と注視点との距離
#define CAMERA_INERTIA			(0.115f)						// カメラ移動の慣性係数
#define INITCAMERA_VECU			(D3DXVECTOR3(0.0f, 1.0f, 0.0f))	// カメラのベクトル初期値
#define CAMERA_SPEED			(0.005f)						// カメラの移動速度
#define CAMERAROT_RESTRICTION	(0.45f)							// カメラの角度制限

//=============================================================================
// コンストラクタ
//=============================================================================
CCamera::CCamera()
{
	// 各メンバ変数のクリア
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	memset(&m_mtxProjection, 0.0f, sizeof(m_mtxProjection));
	memset(&m_mtxView, 0.0f, sizeof(m_mtxView));
}

//=============================================================================
// デストラクタ
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCamera::Init(void)
{
	// メンバ変数初期化
	m_posV = D3DXVECTOR3(0.0f, 50.0f, -100.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = INIT_VECU;
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRadiusVertical = 0.0f;
	m_fRadiusHorizontal = 0.0f;

	return S_OK;
}

//=============================================================================
//	カメラの終了処理
//=============================================================================
void CCamera::Uninit(void)
{

}

//=============================================================================
// カメラの更新処理
//=============================================================================
void CCamera::Update(void)
{
	// 入力デバイス取得
	CManager* pManager = CManager::GetInstance();
	CInputKeyboard* pInputKeyboard = pManager->GetInputKeyboard();
	CInputMouse* pInputMouse = pManager->GetInputMouse();

	// ローカル変数
	D3DXVECTOR3 posV = {}, posR = {};	// 視点・注視点
	D3DXVECTOR3 rot = {};				// カメラの向き
	int nWheelMove = 0;					// ホイールの変動量
	POINT move = {};					// 移動量格納用

	// 向きを取得
	posV = GetPositionV();
	posR = GetPositionR();
	rot = GetRotation();

	if (pInputKeyboard->GetPress(DIK_LCONTROL) == true)
	{
		move = pInputMouse->GetMouseMove();

		CDebugProc::Print("%d, %d\n", move.x, move.y);

		if (pInputMouse->GetMouseButtonFlags(CInputMouse::KEY_RIGHT) == true)
		{// マウスが右クリックされていたら
			posR.x += cosf(rot.y) * (float)move.x;
			posR.y += cosf(rot.x) * (float)move.y;
			posR.z += -sinf(rot.y) * (float)move.x + sinf(rot.x) * (float)move.y;
		}
		else if (pInputMouse->GetMouseButtonFlags(CInputMouse::KEY_LEFT) == true)
		{// マウスが左クリックされていたら
			// 直接入れる
			rot.x += move.y * CAMERA_SPEED;
			rot.y += move.x * CAMERA_SPEED;
		}
		nWheelMove = pInputMouse->GetMouseWheelMove();	// マウスホイール移動量
	}

	// 角度制限
	if (rot.x < -D3DX_PI * CAMERAROT_RESTRICTION)
	{
		rot.x = -D3DX_PI * CAMERAROT_RESTRICTION;
	}

	if (rot.x > D3DX_PI * CAMERAROT_RESTRICTION)
	{
		rot.x = D3DX_PI * CAMERAROT_RESTRICTION;
	}

	// ホイール移動量を距離に加算
	m_fRadiusVertical += nWheelMove;

	// 距離制限
	if (m_fRadiusVertical < WHEEL_DELTA)
	{
		m_fRadiusVertical = WHEEL_DELTA;
	}

	if (m_fRadiusVertical > WHEEL_DELTA * 25)
	{
		m_fRadiusVertical = WHEEL_DELTA * 25;
	}

	// 角度を補正
	rot.x = AngleNormalize(rot.x);
	rot.y = AngleNormalize(rot.y);

	// 目的距離
	D3DXVECTOR3 DestDistance;

	DestDistance = D3DXVECTOR3(sinf(0.0f) * m_fRadiusVertical,
		cosf(rot.x) * m_fRadiusVertical,
		cosf(0.0f) * m_fRadiusVertical);

	// 球の半径を計算
	m_fRadiusHorizontal = SQRTF(DestDistance.x, DestDistance.y);

	// 目的視点を目的注視点から離す
	posV.x = posR.x + sinf(rot.y) * m_fRadiusHorizontal;
	posV.y = posR.y + sinf(rot.x) * m_fRadiusVertical;
	posV.z = posR.z + cosf(rot.y) * m_fRadiusHorizontal;

	CDebugProc::Print("視点 { %.2f %.2f %.2f }\n", posV.x, posV.y, posV.z);
	CDebugProc::Print("注視点 { %.2f %.2f %.2f }\n", posR.x, posR.y, posR.z);
	CDebugProc::Print("カメラの向き { %.2f %.2f %.2f }\n", rot.x, rot.y, rot.z);

	// 変更を反映
	SetPositionV(posV);
	SetPositionR(posR);
	SetRotation(rot);
}

//=============================================================================
//	カメラの設定処理
//=============================================================================
void CCamera::Set(void)
{
	// デバイスの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// プロジェクションマトリックスを初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),							// 視野角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		// アスペクト比
		10.0f,											// Zの最小値
		1500000.0f);										// Zの最大値

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスを初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//=============================================================================
// ビューポート生成処理
//=============================================================================
D3DVIEWPORT9 CCamera::SetViewPort(DWORD dwX, DWORD dwY, DWORD dwWidth, DWORD dwHeight)
{
	D3DVIEWPORT9 ViwePort = {};

	ViwePort.X = dwX;					// 左上X座標
	ViwePort.Y = dwY;					// 左上Y座標
	ViwePort.Width = dwWidth;			// 幅
	ViwePort.Height = dwHeight;			// 高さ
	ViwePort.MinZ = 0.0f;				// 
	ViwePort.MaxZ = 1.0f;				// 

	return ViwePort;
}

//=============================================================================
//	カメラの視点設定処理
//=============================================================================
void CCamera::SetPositionV(D3DXVECTOR3 posV)
{
	m_posV = posV;
}

//=============================================================================
//	カメラの注視点設定処理
//=============================================================================
void CCamera::SetPositionR(D3DXVECTOR3 posR)
{
	m_posR = posR;
}

//=============================================================================
//	カメラの向き設定処理
//=============================================================================
void CCamera::SetRotation(D3DXVECTOR3 rot)
{
	m_rot = rot;
}