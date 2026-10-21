//=============================================================================
//
//	ボールクラス [ball.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "object.h"
#include "ball.h"
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
#include "batchingeffect.h"

#include "gimmick.h"
#include "rotationgimmick.h"
#include "ringeffect.h"

#include "model_particle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SPEED				(0.025f)		// 速度
#define ROTATTENUATION		(0.08f)			// 回転減衰
#define GRAVITY				(-0.25f)		// 重力

//=============================================================================
// 生成処理
//=============================================================================
CBall* CBall::Create(void)
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
	CBall* pBall = nullptr;		// ポインタ保存用

	// ボールのインスタンス生成
	pBall = new CBall;

	// 初期化処理
	pBall->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10.0f, SPEED, ROTATTENUATION);

	// 生成したオブジェクトのポインタを返す
	return pBall;
}

//=============================================================================
// 生成処理
//=============================================================================
CBall* CBall::Create(const D3DXVECTOR3 pos, const float fRadius, const float fSpeed, const float fRotAttenuation)
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
	CBall* pBall = NULL;		// ポインタ保存用

	// プレイヤーのインスタンス生成
	pBall = new CBall;

	// 初期化処理
	pBall->Init(pos, fRadius, fSpeed, fRotAttenuation);

	// 生成したオブジェクトのポインタを返す
	return pBall;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CBall::CBall(int nPriority) : CObject(nPriority)
{
	// 各メンバ変数クリア
	m_pRigid_Dynamic = nullptr;
	m_pSphere_Shape = nullptr;
	m_pModel = nullptr;
	m_pMotion = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_moveDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scl = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXQuaternionIdentity(&m_quat);
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
CBall::~CBall()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBall::Init(void)
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
	D3DXQuaternionIdentity(&m_quat);
	D3DXMatrixIdentity(&m_mtxRot);

	return S_OK;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBall::Init(const D3DXVECTOR3 pos, const float fRadius, const float fSpeed, const float fRotAttenuation)
{
	// レンダラーの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 物理エンジン取得
	physx::PxPhysics* pPhysics = pManager->GetPhysics();
	physx::PxScene* pPxScene = pManager->GetPxScene();

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
	D3DXQuaternionIdentity(&m_quat);
	D3DXMatrixIdentity(&m_mtxRot);

	SetType(TYPE::TYPE_PLAYER);

	// モデルの生成
	m_pModel = CModel::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		"data//MODEL//earth000.x");

	// 動かすことのできる(動的)剛体を作成
	m_pRigid_Dynamic = pPhysics->createRigidDynamic(physx::PxTransform(physx::PxIdentity));

	// 形状(Sphere)を作成
	m_pSphere_Shape = pPhysics->createShape(
			// Sphereの大きさ
			physx::PxSphereGeometry(fRadius),
			// 摩擦係数と反発係数の設定
			*pPhysics->createMaterial(0.025f, 0.025f, 0.025f)
		);

	// 形状を紐づけ
	m_pSphere_Shape->setLocalPose(physx::PxTransform(physx::PxIdentity));
	m_pRigid_Dynamic->attachShape(*m_pSphere_Shape);

	physx::PxVec3 Position = physx::PxVec3(pos.x, pos.y, pos.z);
	physx::PxQuat Quat = physx::PxQuat(m_quat.x, m_quat.y, m_quat.z, m_quat.w);
	physx::PxTransform pose = physx::PxTransform(Position, Quat);

	// キネマティック化
	//m_pRigid_Dynamic->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);
	//m_pRigid_Dynamic->setKinematicTarget(pose);

	m_pRigid_Dynamic->setGlobalPose(pose);

	// 剛体を空間に追加
	pPxScene->addActor(*m_pRigid_Dynamic);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBall::Uninit(void)
{
	// モデルの破棄
	if (m_pModel != nullptr)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = nullptr;
	}

	// モーションインスタンスの破棄
	if (m_pMotion != nullptr)
	{
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	m_pSphere_Shape->release();
	m_pRigid_Dynamic->release();

	// 開放
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBall::Update(void)
{
	// デバイス取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	CInputKeyboard* pInputKeyboard = pManager->GetInputKeyboard();
	CInputJoypad* pInputJoypad = pManager->GetInputJoypad();
	CCamera* pCamera = pManager->GetCamera();
	CBatchingEffect* pBatchiEffect = pManager->GetBatchingEffect();

	D3DXVECTOR3 KeyboardMove = { 0.0f, 0.0f, 0.0f };
	static D3DXVECTOR3 KeyboardMoveOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = m_pos;
	m_vecAxisOld = m_vecAxis;

	// デバッグ機能
#if _DEBUG
	Debug();
#endif

	if (pInputKeyboard->GetTrigger(DIK_TAB) == true)
	{
		D3DXVECTOR3 impulse = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		float fVertical = 0.0f, fHorizontal = 0.0f, fPower = 0.0f;;

		fVertical = ((float)(rand() % 629 - 314) / 100);
		fHorizontal = ((float)(rand() % 629 - 314) / 100);
		fPower = (float)(rand() % 15 + 13);

		impulse.x = cosf(fHorizontal) * fPower;
		impulse.y = cosf(fVertical) * (fPower * 2.0f);
		impulse.z = -sinf(fHorizontal) * fPower + sinf(fVertical) * fPower;

		m_pRigid_Dynamic->addForce(physx::PxVec3(impulse.x, impulse.y, impulse.z), physx::PxForceMode::eIMPULSE);
	}

	physx::PxTransform transform = m_pRigid_Dynamic->getGlobalPose();

	m_pos = D3DXVECTOR3(transform.p.x, transform.p.y, transform.p.z);
	m_quat = D3DXQUATERNION(transform.q.x, transform.q.y, transform.q.z, transform.q.w);

	//// 移動量
	//m_fValueRot += m_fMoveKeyboard * m_fSpeed;
	//m_fMove = (m_fValueRot / (D3DX_PI * 2.0f)) * m_fCircle;
}

//=============================================================================
// 描画処理
//=============================================================================
void CBall::Draw(void)
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

	//// 回転軸における指定の回転角からクォータニオンを作成
	//D3DXQuaternionRotationAxis(&m_quat, &m_vecAxis, m_fValueRot);

	// クォータニオンから回転マトリックスを作成
	D3DXMatrixRotationQuaternion(&mtxRot, &m_quat);

	// 向きを反映
	//D3DXMatrixMultiply(&m_mtxRot, &m_mtxRot, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// 描画
	m_pModel->Draw();

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
// 位置指定処理
//=============================================================================
void CBall::SetPosition(const D3DXVECTOR3 pos)
{
	// 位置変更
	m_pos = pos;
}

//=============================================================================
// 位置取得処理
//=============================================================================
D3DXVECTOR3 CBall::GetPosition(void)
{
	return m_pos;
}

//=============================================================================
// 向き指定処理
//=============================================================================
void CBall::SetRotation(const D3DXVECTOR3 rot)
{
	// 向き変更
	m_rot = rot;

	// 向きの範囲制限
	m_rot.z = AngleNormalize(m_rot.z);
}

//=============================================================================
// 向き取得処理
//=============================================================================
D3DXVECTOR3 CBall::GetRotation(void)
{
	return m_rot;
}