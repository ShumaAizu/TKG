//=============================================================================
//
//	回転ギミッククラス [rotationgimmick.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "rotationgimmick.h"
#include "manager.h"
#include "texture.h"
#include "object.h"
#include "input.h"

//=============================================================================
// 生成
//=============================================================================
CRotationGimmick* CRotationGimmick::Create(const D3DXVECTOR3 pos,
	const D3DXVECTOR3 rot,
	const float fWidth,
	const float fHeight,
	const float fDepth,
	const char* pModelFile)
{
	// 最大チェック
	if (GetNumAll() >= MAX_OBJECT)
	{// これ以上生成できなければ
		// nullptrを返し終了
		return nullptr;
	}

	// ローカル変数宣言
	CRotationGimmick* pRotationGimmick = nullptr;		// ポインタ保存用

	// アイテムのインスタンス生成
	pRotationGimmick = new CRotationGimmick;

	// 初期化処理
	pRotationGimmick->Init(pos, rot, fWidth, fHeight, fDepth, pModelFile);

	// 生成したオブジェクトのポインタを返す
	return pRotationGimmick;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CRotationGimmick::CRotationGimmick(int nPriority) : CGimmick(nPriority)
{
	// メンバ変数クリア
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_fDepth = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CRotationGimmick::~CRotationGimmick()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CRotationGimmick::Init(void)
{
	return S_OK;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CRotationGimmick::Init(const D3DXVECTOR3 pos,
	const D3DXVECTOR3 rot,
	const float fWidth,
	const float fHeight,
	const float fDepth,
	const char* pModelFile)
{
	// 各種値初期化
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fDepth = fDepth;

	m_type = TYPE_ROTATION;

	CGimmick::Init(pos, rot, pModelFile);

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CRotationGimmick::Uninit(void)
{
	// 親クラスの終了処理
	CGimmick::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CRotationGimmick::Update(void)
{
	// デバイス取得
	CManager* pManager = CManager::GetInstance();
	CInputKeyboard* pInputKeyboard = pManager->GetInputKeyboard();

	D3DXVECTOR3 rot = GetRotation();

	if (pInputKeyboard->GetPress(DIK_F3) == true)
	{
		rot.y += 0.01f;
		rot.y = NumCirculation(rot.y, -D3DX_PI, D3DX_PI);
	}

	SetRotation(rot);

	// 親クラスの更新処理
	CGimmick::Update();
}

//=============================================================================
// 描画
//=============================================================================
void CRotationGimmick::Draw(void)
{
	// 親クラスの描画処理
	CGimmick::Draw();
}

bool CRotationGimmick::Collision(D3DXVECTOR3& pos, float fRadius)
{
	// ローカル変数
	D3DXVECTOR3 myPos = GetPosition();
	D3DXVECTOR3 myRot = GetRotation();
	bool isCollision = false;

	// 計算用マトリックス
	D3DXMATRIX TargetmtxRot;

	// マトリックスの初期化
	D3DXMatrixIdentity(&TargetmtxRot);

	// 向きを算出
	D3DXMatrixRotationYawPitchRoll(&TargetmtxRot, myRot.y, myRot.x, myRot.z);

	// 対象の矩形の各軸ベクトル
	D3DXVECTOR3 TargetAxisXNor = { TargetmtxRot._11, TargetmtxRot._12, TargetmtxRot._13 },	// X軸
		TargetAxisX = TargetAxisXNor * m_fWidth;
	D3DXVECTOR3 TargetAxisYNor = { TargetmtxRot._21, TargetmtxRot._22, TargetmtxRot._23 },	// Y軸
		TargetAxisY = TargetAxisYNor * m_fHeight;
	D3DXVECTOR3 TargetAxisZNor = { TargetmtxRot._31, TargetmtxRot._32, TargetmtxRot._33 },	// Z軸
		TargetAxisZ = TargetAxisZNor * m_fDepth;

	// 自分と対象の中心点とのベクトル
	D3DXVECTOR3 Distance = pos - myPos;

	// ローカル座標
	D3DXVECTOR3 MyCenter = D3DXVECTOR3(D3DXVec3Dot(&Distance, &TargetAxisXNor),
		D3DXVec3Dot(&Distance, &TargetAxisYNor),
		D3DXVec3Dot(&Distance, &TargetAxisZNor));

	// 交点
	D3DXVECTOR3 Intersection;

	// クランプ処理
	Intersection.x = max(-m_fWidth, min(m_fWidth, MyCenter.x));
	Intersection.y = max(-m_fHeight, min(m_fHeight, MyCenter.y));
	Intersection.z = max(-m_fDepth, min(m_fDepth, MyCenter.z));

	// 距離
	D3DXVECTOR3 diff = MyCenter - Intersection;
	float fDiff = D3DXVec3LengthSq(&diff);

	if (fDiff <= 0.0f)
	{// セーフティ
		fDiff += 0.0001f;
	}

	if (fDiff < (fRadius * fRadius))
	{
		float fDistance = sqrtf(fDiff);

		// 押し出しベクトル (ローカル)
		D3DXVECTOR3 vecPushNor = diff / fDistance;

		// 押し出し距離	これが深さにあたるはず
		float fPush = fRadius - fDistance;

		// 押し出しベクトル (ワールド)	これが衝突面の法線？
		D3DXVECTOR3 vecPush = (vecPushNor.x * TargetAxisXNor) + (vecPushNor.y * TargetAxisYNor) + (vecPushNor.z * TargetAxisZNor);

		// 距離を含んだ押し出しベクトル
		vecPush = vecPush * fPush;

		// 入り込んだ位置に押し出しベクトルを加算
		pos = pos + vecPush;

		if (vecPush.y > 0.0f)
		{// 上方向への押し出しベクトルがあれば
			// 着地判定
			isCollision = true;
		}
	}

	// 衝突判定情報を返す
	return isCollision;
}