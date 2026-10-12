//=============================================================================
//
//	フィールドクラス [field.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "field.h"
#include "manager.h"
#include "texture.h"
#include "object.h"

//=============================================================================
// 生成
//=============================================================================
CField* CField::Create(const D3DXVECTOR3 pos,
	const D3DXVECTOR3 rot,
	const float fWidth,
	const float fDepth,
	const char* pFileName)
{
	// 最大チェック
	if (GetNumAll() >= MAX_OBJECT)
	{// これ以上生成できなければ
		// NULLを返し終了
		return NULL;
	}

	// ローカル変数宣言
	CField* pField = NULL;		// ポインタ保存用

	// 惑星のインスタンス生成
	pField = new CField;

	// 初期化処理
	pField->Init(pos, rot, fWidth, fDepth, pFileName);

	// 生成したオブジェクトのポインタを返す
	return pField;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CField::CField(int nPriority) : CObject3D(nPriority)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CField::~CField()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CField::Init(void)
{
	return S_OK;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CField::Init(const D3DXVECTOR3 pos,
	const D3DXVECTOR3 rot,
	const float fWidth,
	const float fDepth,
	const char* pTexFile)
{

	// 各種値設定
	CObject3D::Init(pos, rot, fWidth, 0.0f, fDepth, pTexFile);

	SetType(TYPE::TYPE_FIELD);

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CField::Uninit(void)
{
	// 終了処理
	CObject3D::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CField::Update(void)
{
	// 更新処理
	CObject3D::Update();
}

//=============================================================================
// 描画
//=============================================================================
void CField::Draw(void)
{
	// 描画処理
	CObject3D::Draw();
}

//=============================================================================
// フィールドの高さ取得処理
//=============================================================================
float CField::GetHeight(const D3DXVECTOR3 pos, const D3DXVECTOR3 posOld)
{
	// ローカル変数
	D3DXVECTOR3 myPos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();
	D3DXVECTOR3 vec0, vec1, vec2, vec3, nor0, nor1;
	D3DXVECTOR3 vtx0, vtx1, vtx2, vtx3;
	float fHeight = 0.0f;

	VERTEX_3D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を抜き取る
	vtx0 = pVtx[0].pos;
	vtx1 = pVtx[1].pos;
	vtx2 = pVtx[2].pos;
	vtx3 = pVtx[3].pos;

	// 頂点バッファのアンロック
	m_pVtxBuff->Unlock();

	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, myPos.x, myPos.y, myPos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 位置を設定
	D3DXVec3TransformCoord(&vtx0, &vtx0, &m_mtxWorld);
	D3DXVec3TransformCoord(&vtx1, &vtx1, &m_mtxWorld);
	D3DXVec3TransformCoord(&vtx2, &vtx2, &m_mtxWorld);
	D3DXVec3TransformCoord(&vtx3, &vtx3, &m_mtxWorld);

	// それぞれの始点から位置へのベクトルを算出
	D3DXVECTOR3 vecToPos0 = pos - vtx0;
	D3DXVECTOR3 vecToPos1 = pos - vtx1;
	D3DXVECTOR3 vecToPos2 = pos - vtx2;
	D3DXVECTOR3 vecToPos3 = pos - vtx3;

	// 2つのベクトルの法線を求める
	vec0 = vtx1 - vtx0;
	vec1 = vtx2 - vtx0;
	vec2 = vtx1 - vtx3;
	vec3 = vtx2 - vtx3;

	D3DXVec3Cross(&nor0, &vec0, &vec1);
	D3DXVec3Cross(&nor1, &vec2, &vec3);

	// 法線を正規化
	D3DXVec3Normalize(&nor0, &nor0);
	D3DXVec3Normalize(&nor1, &nor1);

	// 3辺の境界線ベクトルを算出
	D3DXVECTOR3 vecLine0 = vtx2 - vtx0;
	D3DXVECTOR3 vecLine1 = vtx0 - vtx1;
	D3DXVECTOR3 vecLine2 = vtx1 - vtx2;

	if ((vecLine0.z * vecToPos0.x) - (vecLine0.x * vecToPos0.z) < 0 &&
		(vecLine1.z * vecToPos1.x) - (vecLine1.x * vecToPos1.z) < 0 &&
		(vecLine2.z * vecToPos2.x) - (vecLine2.x * vecToPos2.z) < 0)
	{
		if (nor0.y != 0.0f)
		{
			// 前提 : 0.0f = (pos.x - vtx0.x) * nor.x + (pos.y - vtx0.y) * nor.y + (pos.z - vtx0.z) * nor.z;

			fHeight = vtx0.y - ((pos.x - vtx0.x) * nor0.x + (pos.z - vtx0.z) * nor0.z) / nor0.y;

			return fHeight;
		}
	}

	// もう一つの境界線ベクトル
	vecLine0 = vtx1 - vtx3;
	vecLine1 = vtx2 - vtx1;
	vecLine2 = vtx3 - vtx2;

	if ((vecLine0.z * vecToPos3.x) - (vecLine0.x * vecToPos3.z) < 0 &&
		(vecLine1.z * vecToPos1.x) - (vecLine1.x * vecToPos1.z) < 0 &&
		(vecLine2.z * vecToPos2.x) - (vecLine2.x * vecToPos2.z) < 0)
	{
		if (nor0.y != 0.0f)
		{
			fHeight = vtx3.y - ((pos.x - vtx3.x) * nor1.x + (pos.z - vtx3.z) * nor1.z) / nor1.y;

			return fHeight;
		}
	}

	return fHeight;
}