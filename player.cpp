//=============================================================================
//
//	プレイヤークラス [player.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "object.h"
#include "player.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "model.h"
#include "motion.h"

#include "debugproc.h"

#include "object3D.h"

#include "field.h"
#include "wall.h"
#include "item.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SPEED				(0.025f)		// 速度
#define ROTATTENUATION		(0.08f)			// 回転減衰

//=============================================================================
// 生成処理
//=============================================================================
CPlayer* CPlayer::Create(void)
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
	CPlayer* pPlayer = NULL;		// ポインタ保存用

	// プレイヤーのインスタンス生成
	pPlayer = new CPlayer;

	// 初期化処理
	pPlayer->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10.0f, SPEED, ROTATTENUATION);

	// 生成したオブジェクトのポインタを返す
	return pPlayer;
}

//=============================================================================
// 生成処理
//=============================================================================
CPlayer* CPlayer::Create(const D3DXVECTOR3 pos, const float fRadius, const float fSpeed, const float fRotAttenuation)
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
	CPlayer* pPlayer = NULL;		// ポインタ保存用

	// プレイヤーのインスタンス生成
	pPlayer = new CPlayer;

	// 初期化処理
	pPlayer->Init(pos, fRadius, fSpeed, fRotAttenuation);

	// 生成したオブジェクトのポインタを返す
	return pPlayer;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CPlayer::CPlayer(int nPriority) : CObject(nPriority)
{
	// 各メンバ変数クリア
	memset(&m_apModel, NULL, sizeof(CModel*) * MAX_PARTS);
	m_nNumModel = 0;
	m_pMotion = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_moveDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scl = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecAxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecAxisOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRadius = 0.0f;
	m_fCircle = 0.0f;
	m_fMove = 0.0f;
	m_fSpeed = 0.0f;
	m_fValueRot = 0.0f;
	m_fRotAttenuation = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(void)
{
	// デバイスの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 変数初期化
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecAxis = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_fRadius = 10.0f;
	m_fCircle = (D3DX_PI * 2.0f) * m_fRadius;
	m_fMove = 0.0f;
	m_fValueRot = 0.0f;
	m_fRotAttenuation = ROTATTENUATION;
	m_fSpeed = SPEED;
	D3DXMatrixIdentity(&m_mtxRot);

	return S_OK;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(const D3DXVECTOR3 pos, const float fRadius, const float fSpeed, const float fRotAttenuation)
{
	// レンダラーの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 変数初期化
	m_pos = pos;
	m_posOld = pos;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecAxis = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_fRadius = fRadius;
	m_fCircle = (D3DX_PI * 2.0f) * m_fRadius;
	m_fMove = 0.0f;
	m_fValueRot = 0.0f;
	m_fSpeed = fSpeed;
	m_fRotAttenuation = fRotAttenuation;
	D3DXMatrixIdentity(&m_mtxRot);

	SetType(TYPE::TYPE_PLAYER);

	// モデルの生成
	m_apModel[0] = CModel::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		"data//MODEL//spikeball000.x");

	m_nNumModel++;

	//m_apModel[1] = CModel::Create(D3DXVECTOR3(0.0f, 22.5f, 0.0f),
	//	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	//	"data//MODEL//01_snowman_head.x");

	//m_nNumModel++;

	//m_apModel[2] = CModel::Create(D3DXVECTOR3(-8.5f, 17.5f, 0.0f),
	//	D3DXVECTOR3(0.0f, 0.0f, 0.90f),
	//	"data//MODEL//02_snowman_armR.x");

	//m_nNumModel++;

	//m_apModel[3] = CModel::Create(D3DXVECTOR3(8.5f, 17.5f, 0.0f),
	//	D3DXVECTOR3(0.0f, 0.0f, -0.90f),
	//	"data//MODEL//03_snowman_armL.x");

	//m_nNumModel++;

	//m_apModel[4] = CModel::Create(D3DXVECTOR3(5.0f, 15.0f, 0.0f),
	//	D3DXVECTOR3(0.0f, 0.0f, -0.75f),
	//	"data//MODEL//04_snowman_bucket.x");

	//m_nNumModel++;

	//m_apModel[5] = CModel::Create(D3DXVECTOR3(0.0f, 15.0f, 0.0f),
	//	D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI, 0.0f),
	//	"data//MODEL//Arrow000.x");

	//m_nNumModel++;

	//// 親モデルの設定
	//m_apModel[1]->SetParent(m_apModel[0]);

	//m_apModel[2]->SetParent(m_apModel[0]);

	//m_apModel[3]->SetParent(m_apModel[0]);

	//m_apModel[4]->SetParent(m_apModel[1]);

	//m_apModel[5]->SetParent(m_apModel[0]);

	//// モーションのインスタンス生成
	//if (m_pMotion == NULL)
	//{
	//	m_pMotion = new CMotion;
	//}

	//if (m_pMotion != NULL)
	//{

	//}

	//m_pMotion->SetModel(&m_apModel[0], m_nNumModel);
	//LoadMotionInfo("data/SCRIPTS/MOTION/snowman_motion.txt");

	//m_pMotion->Set(0, false);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	// 全パーツの開放
	for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
	{
		if (m_apModel[nCntParts] == NULL)
		{// もしモデルが存在しなければ
			// スキップ
			continue;
		}

		// パーツの終了処理
		m_apModel[nCntParts]->Uninit();
		delete m_apModel[nCntParts];
		m_apModel[nCntParts] = NULL;
	}

	// モーションインスタンスの破棄
	if (m_pMotion != NULL)
	{
		delete m_pMotion;
		m_pMotion = NULL;
	}

	// 開放
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
	// デバイス取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	CInputKeyboard* pInputKeyboard = pManager->GetInputKeyboard();
	CInputJoypad* pInputJoypad = pManager->GetInputJoypad();
	CCamera* pCamera = pManager->GetCamera();

	static float fRotMove = 0.0f, fRotDest = 0.0f, fRotDiff = 0.0f;

	D3DXVECTOR3 KeyboardMove = { 0.0f, 0.0f, 0.0f };
	static D3DXVECTOR3 KeyboardMoveOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = m_pos;
	m_vecAxisOld = m_vecAxis;

	if (pInputKeyboard->GetPress(DIK_A) == true || pInputJoypad->GetPress(CInputJoypad::KEY::KEY_LEFT) == true)
	{// Aキーが押された
		KeyboardMove.x += -1.0f;
	}

	if (pInputKeyboard->GetPress(DIK_D) == true || pInputJoypad->GetPress(CInputJoypad::KEY::KEY_RIGHT) == true)
	{// Dキーが押された
		KeyboardMove.x += 1.0f;
	}

	if (pInputKeyboard->GetPress(DIK_W) == true || pInputJoypad->GetPress(CInputJoypad::KEY::KEY_UP) == true)
	{// Wキーが押された
		KeyboardMove.z += 1.0f;
	}

	if (pInputKeyboard->GetPress(DIK_S) == true || pInputJoypad->GetPress(CInputJoypad::KEY::KEY_DOWN) == true)
	{// Sキーが押された
		KeyboardMove.z += -1.0f;
	}

	// デバッグ機能
#if _DEBUG
	Debug();
#endif

	// スティックの値をもらう(使っていたら優先)
	pInputJoypad->GetStickLeft(&KeyboardMove.x, &KeyboardMove.z);

	// 移動ベクトルを正規化
	D3DXVec3Normalize(&KeyboardMove, &KeyboardMove);

	// 移動総量
	m_fMoveKeyboard = SQRTF(KeyboardMove.x, KeyboardMove.z);

	if (m_fMoveKeyboard != 0.0f)
	{// 移動していたら

		fRotDest = atan2f(KeyboardMove.x, KeyboardMove.z) + pCamera->GetRotation().y;	// 目的地への向き

		// 目的向きに移動方向を合わせる
		KeyboardMove.x = sinf(fRotDest) * m_fMoveKeyboard;
		KeyboardMove.z = cosf(fRotDest) * m_fMoveKeyboard;

		// 移動軸設定
		m_vecAxis.x = -KeyboardMove.z;
		m_vecAxis.y = 0.0f;
		m_vecAxis.z = KeyboardMove.x;

		D3DXVECTOR3 vecAxisDiff = m_vecAxisOld - m_vecAxis;

		if (fabsf(vecAxisDiff.x) > 1.5f || fabsf(vecAxisDiff.z) > 1.5f)
		{
			m_fValueRot *= -1.0f;
		}

		KeyboardMoveOld = KeyboardMove;
	}

	// 移動量
	m_fValueRot += m_fMoveKeyboard * m_fSpeed;
	m_fMove = (m_fValueRot / (D3DX_PI * 2.0f)) * m_fCircle;

	// 移動量の更新
	m_move.x = (m_fMove * -KeyboardMoveOld.x);
	m_move.z = (m_fMove * -KeyboardMoveOld.z);

	CDebugProc::Print("前回の移動方向 = { %.2f %.2f %.2f }\n", KeyboardMoveOld.x, KeyboardMoveOld.y, KeyboardMoveOld.z);

	if ((pInputKeyboard->GetTrigger(DIK_SPACE) == true || pInputJoypad->GetTrigger(CInputJoypad::KEY::KEY_A) == true) && m_bJump == false)
	{// スペースキーが押されたかつジャンプ状態じゃない
		m_move.y = m_fRadius * 1.25f;
		m_bJump = true;
	}

	m_move.y += -0.75f;		// 重力をかける
	m_pos += m_move;		// 位置を更新

	// 当たり判定
	CollisionField();
	CollisionWall();
	CollisionItem();

	// 慣性を更新
	m_fValueRot += (0.0f - m_fValueRot) * m_fRotAttenuation;

	CDebugProc::Print("*** モデル情報 ***\n");
	CDebugProc::Print("位置 = { %.2f %.2f %.2f }\n", m_pos.x, m_pos.y, m_pos.z);
	CDebugProc::Print("移動量 = { %.2f %.2f %.2f }\n", m_move.x, m_move.y, m_move.z);
	CDebugProc::Print("回転軸 = { %.2f %.2f %.2f }\n", m_vecAxis.x, m_vecAxis.y, m_vecAxis.z);
	CDebugProc::Print("回転軸Old = { %.2f %.2f %.2f }\n", m_vecAxisOld.x, m_vecAxisOld.y, m_vecAxisOld.z);
	CDebugProc::Print("回転角 = { %.2f }\n", m_fValueRot);
	CDebugProc::Print("移動量 = { %.2f }\n", m_fMove);
	CDebugProc::Print("\n半径 : [← / →] (%.1f)\n", m_fRadius);
	CDebugProc::Print("回転速度 : [↑ / ↓] (%.3f)\n", m_fSpeed);
	CDebugProc::Print("回転減衰 : [Y / H] (%.2f)\n", m_fRotAttenuation);

	// モーションの更新
	//m_pMotion->Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	// デバイスの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	CTexture* pTexture = pManager->GetTextrue();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;		// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;							// マテリアルデータへのポインタ

	D3DXMATERIAL mat;							// マテリアル保持用

	//// 影の描画用変数
	//D3DXMATRIX mtxShadow;		// シャドウマトリックス
	//D3DLIGHT9 light;			// ライトの情報
	//D3DXVECTOR4 posLight;		// ライトの位置
	//D3DXVECTOR3 pos, normal;	// 平面の点と法線ベクトル
	//D3DXPLANE plane;			// 平面の情報

	//// ライトの位置を設定
	//pDevice->GetLight(0, &light);
	//posLight = D3DXVECTOR4(-light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f);

	//// 平面情報を生成
	//pos = D3DXVECTOR3(0.0f, 0.05f, 0.0f);		// 高さが0ならなんでもいい
	//normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//D3DXPlaneFromPointNormal(&plane, &pos, &normal);

	//// 現在のマテリアルを取得
	//pDevice->GetMaterial(&matDef);

	//// 描画
	//D3DXMATRIX mtxRotOffSetModel, mtxTransOffSetModel;	// 計算用マトリックス
	//D3DXMATRIX mtxParent;								// 親のマトリックス

	//// ワールドマトリックスの初期化(デフォルトの値にする)
	//D3DXMatrixIdentity(&mtxShadow);

	//// シャドウマトリックスの生成
	//D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
	//D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

	//// パーツのワールドマトリックスを設定
	//pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	//// マテリアルデータへのポインタを取得
	//pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	//for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	//{
	//	mat = pMat[nCntMat];

	//	mat.MatD3D.Diffuse.r = 0.0f;
	//	mat.MatD3D.Diffuse.g = 0.0f;
	//	mat.MatD3D.Diffuse.b = 0.0f;

	//	// マテリアルの設定
	//	pDevice->SetMaterial(&mat.MatD3D);

	//	// テクスチャの設定
	//	pDevice->SetTexture(0, nullptr);

	//	// プレイヤー(パーツ)の描画
	//	m_pMesh->DrawSubset(nCntMat);;
	//}

	//// 保存していたマテリアルを戻す
	//pDevice->SetMaterial(&matDef);

	// 頂点法線正規化をオン
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	// ライトをオフにする
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScale, m_scl.x, m_scl.y, m_scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 回転軸における指定の回転角からクォータニオンを作成
	D3DXQuaternionRotationAxis(&m_quat, &m_vecAxis, m_fValueRot);

	// クォータニオンから回転マトリックスを作成
	D3DXMatrixRotationQuaternion(&mtxRot, &m_quat);

	// 向きを反映
	D3DXMatrixMultiply(&m_mtxRot, &m_mtxRot, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// 全パーツの描画
	for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
	{
		if (m_apModel[nCntParts] == NULL)
		{// もしパーツが存在していなければ
			// スキップ
			continue;
		}

		// パーツの描画処理
		m_apModel[nCntParts]->Draw();
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	// ライトをオンにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// 頂点法線正規化をオフ
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, false);

	//// デバイスの取得
	//CManager* pManager = CManager::GetInstance();
	//CRenderer* pRenderer = pManager->GetRenderer();
	//LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス

	//// ワールドマトリックスの初期化
	//D3DXMatrixIdentity(&m_mtxWorld);

	//// 向きを反映
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//// 位置を反映
	//D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//// ワールドマトリックスの設定
	//pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//// 全パーツの描画
	//for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
	//{
	//	if (m_apModel[nCntParts] == NULL)
	//	{// もしパーツが存在していなければ
	//		// スキップ
	//		continue;
	//	}

	//	// パーツの描画処理
	//	m_apModel[nCntParts]->Draw();
	//}
}

//=============================================================================
// 地面との当たり判定
//=============================================================================
bool CPlayer::CollisionField(void)
{
	// 情報取得
	CManager* pManager = CManager::GetInstance();
	CSound* pSound = pManager->GetSound();

	// ローカル変数
	bool isCollision = false;

	// オブジェクト探索
	CObject* pObject = nullptr;
	CObject* pObjectNext = nullptr;
	CField* pField = nullptr;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		// 先頭オブジェクト取得
		pObject = CObject::GetTop(nCntPriority);

		while (pObject != nullptr)
		{// オブジェクトが消えるまでループ
			pObjectNext = pObject->GetNext();

			if (pObject->GetType() != CObject::TYPE_FIELD)
			{// フィールドじゃなかったらスキップ
				pObject = pObjectNext;
				continue;
			}

			// フィールドだったらキャスト
			pField = static_cast<CField*>(pObject);

			// 高さ取得
			float fHeight = pField->GetHeight(m_pos, m_posOld);

			if (m_pos.y - m_fRadius < fHeight)
			{
				m_pos.y = fHeight + m_fRadius;
				m_bJump = false;
			}

			// 次のオブジェクトへ
			pObject = pObjectNext;
		}
	}

	return isCollision;
}

//=============================================================================
// 壁との当たり判定
//=============================================================================
bool CPlayer::CollisionWall(void)
{
	// 情報取得
	CManager* pManager = CManager::GetInstance();
	CSound* pSound = pManager->GetSound();

	// ローカル変数
	bool isCollision = false;
	bool isCollisionTest = false;

	// オブジェクト探索
	CObject* pObject = nullptr;
	CObject* pObjectNext = nullptr;
	CWall* pWall = nullptr;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		// 先頭オブジェクト取得
		pObject = CObject::GetTop(nCntPriority);

		while (pObject != nullptr)
		{// オブジェクトが消えるまでループ
			pObjectNext = pObject->GetNext();

			if (pObject->GetType() != CObject::TYPE_WALL)
			{// フィールドじゃなかったらスキップ
				pObject = pObjectNext;
				continue;
			}

			// フィールドだったらキャスト
			pWall = static_cast<CWall*>(pObject);

			isCollision = pWall->Collision(&m_pos, &m_posOld, m_fRadius, true, true);

			// 次のオブジェクトへ
			pObject = pObjectNext;
		}
	}

	return isCollision;
}

bool CPlayer::CollisionItem(void)
{
	// 情報取得
	CManager* pManager = CManager::GetInstance();
	CSound* pSound = pManager->GetSound();

	// ローカル変数
	bool isCollision = false;

	// オブジェクト探索
	CObject* pObject = nullptr;
	CObject* pObjectNext = nullptr;
	CItem* pItem = nullptr;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		// 先頭オブジェクト取得
		pObject = CObject::GetTop(nCntPriority);

		while (pObject != nullptr)
		{// オブジェクトが消えるまでループ
			pObjectNext = pObject->GetNext();

			if (pObject->GetType() != CObject::TYPE_ITEM)
			{// アイテムじゃなかったらスキップ
				pObject = pObjectNext;
				continue;
			}

			// アイテムだったらキャスト
			pItem = static_cast<CItem*>(pObject);

			// 両方の位置を取得
			D3DXVECTOR3 posPlanet = pItem->GetPosition();
			float fRadius = pItem->GetRadius();

			// 距離と長さ
			float fDiff = powf(m_pos.x - posPlanet.x, 2) + powf(m_pos.y - posPlanet.y, 2) + powf(m_pos.z - posPlanet.z, 2);
			fRadius = powf(fRadius, 2);						// 惑星の半径

			if (fDiff <= fRadius)
			{// 衝突していたら
				pItem->Get();
			}

			// 次のオブジェクトへ
			pObject = pObjectNext;
		}
	}

	return isCollision;
}

//=============================================================================
// 位置指定処理
//=============================================================================
void CPlayer::SetPosition(const D3DXVECTOR3 pos)
{
	// 位置変更
	m_pos = pos;
}

//=============================================================================
// 位置取得処理
//=============================================================================
D3DXVECTOR3 CPlayer::GetPosition(void)
{
	return m_pos;
}

//=============================================================================
// 向き指定処理
//=============================================================================
void CPlayer::SetRotation(const D3DXVECTOR3 rot)
{
	// 向き変更
	m_rot = rot;

	// 向きの範囲制限
	m_rot.z = AngleNormalize(m_rot.z);
}

//=============================================================================
// 向き取得処理
//=============================================================================
D3DXVECTOR3 CPlayer::GetRotation(void)
{
	return m_rot;
}

//=============================================================================
// デバッグ処理
//=============================================================================
void CPlayer::Debug(void)
{
	// 入力デバイス取得
	CManager* pManager = CManager::GetInstance();
	CInputKeyboard* pInputKeyboard = pManager->GetInputKeyboard();
	CInputJoypad* pInputJoypad = pManager->GetInputJoypad();

	// 大きさ変更
	if (pInputKeyboard->GetPress(DIK_LEFT) == true && m_fRadius < 100.0f)
	{// 右入力で大きく
		m_scl = m_scl * 1.1f;
		m_fRadius = m_fRadius * 1.1f;
		m_fCircle = (D3DX_PI * 2) * m_fRadius;
	}

	if (pInputKeyboard->GetPress(DIK_RIGHT) == true && m_fRadius > 10.0f)
	{// 左入力で小さく
		m_scl = m_scl * 0.9f;
		m_fRadius = m_fRadius * 0.9f;
		m_fCircle = (D3DX_PI * 2) * m_fRadius;
	}

	// 回転速度変更
	if (pInputKeyboard->GetRepeat(DIK_UP) == true && m_fSpeed < 1.0f)
	{// 上入力で大きく
		m_fSpeed += 0.001f;
	}

	if (pInputKeyboard->GetRepeat(DIK_DOWN) == true && m_fSpeed > 0.001f)
	{// 下入力で小さく
		m_fSpeed += -0.001f;
	}

	// 回転減衰変更
	if (pInputKeyboard->GetRepeat(DIK_Y) == true && m_fRotAttenuation < 1.0f)
	{// Y入力で大きく
		m_fRotAttenuation += 0.01f;
	}

	if (pInputKeyboard->GetRepeat(DIK_H) == true && m_fRotAttenuation > 0.01f)
	{// H入力で小さく
		m_fRotAttenuation += -0.01f;
	}
}