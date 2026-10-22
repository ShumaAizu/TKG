//=============================================================================
//
//	ウォールクラス [wall.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "wall.h"
#include "manager.h"
#include "texture.h"
#include "object.h"

//=============================================================================
// 生成
//=============================================================================
CWall* CWall::Create(const D3DXVECTOR3 pos,
	const D3DXVECTOR3 rot,
	const float fWidth,
	const float fHeight,
	const char* pFileName)
{
	// 最大チェック
	if (GetNumAll() >= MAX_OBJECT)
	{// これ以上生成できなければ
		// NULLを返し終了
		return NULL;
	}

	// ローカル変数宣言
	CWall* pField = NULL;		// ポインタ保存用

	// 惑星のインスタンス生成
	pField = new CWall;

	// 初期化処理
	pField->Init(pos, rot, fWidth, fHeight, pFileName);

	// 生成したオブジェクトのポインタを返す
	return pField;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CWall::CWall(int nPriority) : CObject3D(nPriority)
{
	// メンバ変数クリア
	m_pRigid_Dynamic = nullptr;
	m_pBox_Shape = nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
CWall::~CWall()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CWall::Init(void)
{
	return S_OK;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CWall::Init(const D3DXVECTOR3 pos,
	const D3DXVECTOR3 rot,
	const float fWidth,
	const float fHeight,
	const char* pTexFile)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// 物理エンジン取得
	physx::PxPhysics* pPhysics = pManager->GetPhysics();
	physx::PxScene* pPxScene = pManager->GetPxScene();

	m_pRigid_Dynamic = pPhysics->createRigidDynamic(physx::PxTransform(physx::PxIdentity));
	m_pBox_Shape = pPhysics->createShape(physx::PxBoxGeometry(fWidth, fHeight, 25.0f),
		*pPhysics->createMaterial(0.5f, 0.5f, 0.5f));

	physx::PxTransform Localpose = physx::PxTransform(physx::PxIdentity);
	Localpose.p.z = 25.0f;

	// 形状を紐づけ
	m_pBox_Shape->setLocalPose(Localpose);
	m_pRigid_Dynamic->attachShape(*m_pBox_Shape);

	physx::PxVec3 Position = physx::PxVec3(pos.x, pos.y, pos.z);

	D3DXQUATERNION quat;
	D3DXQuaternionRotationYawPitchRoll(&quat, rot.y, rot.x, rot.z);

	physx::PxQuat Quat = physx::PxQuat(quat.x, quat.y, quat.z, quat.w);
	physx::PxTransform pose = physx::PxTransform(Position, Quat);

	// キネマティック化
	m_pRigid_Dynamic->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);
	m_pRigid_Dynamic->setKinematicTarget(pose);

	m_pRigid_Dynamic->setGlobalPose(pose);

	// 剛体を空間に追加
	pPxScene->addActor(*m_pRigid_Dynamic);

	// 各種値設定
	CObject3D::Init(pos, rot, fWidth, fHeight, 0.0f, pTexFile);

	SetType(TYPE::TYPE_WALL);

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CWall::Uninit(void)
{
	// 終了処理
	CObject3D::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CWall::Update(void)
{
	// 更新処理
	CObject3D::Update();
}

//=============================================================================
// 描画
//=============================================================================
void CWall::Draw(void)
{
	// 描画処理
	CObject3D::Draw();
}

//=============================================================================
// 当たり判定
//=============================================================================
bool CWall::Collision(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, float fRadius, bool bRetrun, bool bIn)
{
	D3DXVECTOR3 vecLine, vecToPos, vecToPosOld, vecMove, vecNor;	// 各種ベクトル
	D3DXVECTOR3 Intersection = {};									// 交点
	float fRate;													// 比率
	float fAngleVecLine;											// 境界線ベクトルの角度
	float fLenght = 0.0f;											// 壁ずりの長さ
	float fPosOldCross = 0.0f;
	bool bCollision = false;

	D3DXVECTOR3 pos = GetPosition();				// 位置
	D3DXVECTOR3 rot = GetRotation();				// 向き
	D3DXVECTOR3 size = GetSize();					// 大きさ

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;			// 計算用マトリックス
	D3DXVECTOR3 posA, posB;							// 各頂点情報

	posA = D3DXVECTOR3(size.x, 0.0f, size.z);
	posB = D3DXVECTOR3(-size.x, 0.0f, -size.z);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 位置と向きを反映した頂点座標を入れる
	D3DXVec3TransformCoord(&posA, &posA, &m_mtxWorld);
	D3DXVec3TransformCoord(&posB, &posB, &m_mtxWorld);

	vecMove = *pPos - *pPosOld;			// 移動ベクトル

	if (bIn == true)
	{
		vecLine = posA - posB;			// 境界線ベクトル
		vecToPos = *pPos - posB;		// 
		vecToPosOld = *pPosOld - posB;	// 
	}
	else
	{
		vecLine = posB - posA;			// 境界線ベクトル
		vecToPos = *pPos - posA;		// 
		vecToPosOld = *pPosOld - posA;	// 
	}

	fAngleVecLine = atan2f(vecLine.x, vecLine.z);	// 境界線ベクトルの角度

	// 法線ベクトルを算出
	vecNor = D3DXVECTOR3(sinf((0.5f * D3DX_PI) + fAngleVecLine), 0.0f, cosf((0.5f * D3DX_PI) + fAngleVecLine));

	// 比率を算出
	fRate = ((vecToPos.z * vecMove.x) - (vecToPos.x * vecMove.z)) / ((vecLine.z * vecMove.x) - (vecLine.x * vecMove.z));

	// 壁から過去の位置へのベクトル
	fPosOldCross = (vecLine.z * vecToPosOld.x) - (vecLine.x * vecToPosOld.z);

	if (fPosOldCross < -0.0000001f && fPosOldCross > -1.0f)
	{// 誤差は切り捨て
		fPosOldCross = ceilf(fPosOldCross);		// 負の数は切り上げ
	}

	if ((vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z) < 0.0f &&
		fPosOldCross >= 0.0f &&
		fRate <= 1.0f && fRate >= 0.0f)
	{
		if (bRetrun == false)
		{
			bCollision = true;
			return bCollision;
		}

		Intersection = D3DXVECTOR3(vecLine.x * fRate, vecLine.y * fRate, vecLine.z * fRate);		// 交点

		D3DXVec3Normalize(&vecLine, &vecLine);		// 正規化

		// 交点を代入
		if (bIn == true)
		{
			pPos->x = posB.x + Intersection.x;
			pPos->z = posB.z + Intersection.z;
		}
		else
		{
			pPos->x = posA.x + Intersection.x;
			pPos->z = posA.z + Intersection.z;
		}

		fLenght = (vecNor.z * vecMove.x) - (vecNor.x * vecMove.z);		// 壁ずりの長さ算出

#if 0
		fLenght = (vecLine.x * vecMove.x) + (vecLine.z * vecMove.z);	// これ結果同じらしい(上が外積で下が内積)
#endif

		fLenght = fabsf(fLenght);		// 絶対値化

		if ((vecNor.z * vecMove.x) - (vecNor.x * vecMove.z) < 0)
		{// 外側なら加算
			pPos->x += vecLine.x * fLenght;
			pPos->z += vecLine.z * fLenght;
		}
		else if ((vecNor.z * vecMove.x) - (vecNor.x * vecMove.z) > 0)
		{// 内側なら減算
			pPos->x += -vecLine.x * fLenght;
			pPos->z += -vecLine.z * fLenght;
		}

		bCollision = true;
		return bCollision;
	}

	return bCollision;
}