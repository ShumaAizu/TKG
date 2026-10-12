//=============================================================================
//
//	オブジェクト3Dクラス [object3D.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "object.h"
#include "object3D.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"

//=============================================================================
// 生成処理
//=============================================================================
CObject3D* CObject3D::Create(void)
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
	CObject3D* pObject3D = NULL;		// ポインタ保存用

	// オブジェクト2Dのインスタンス生成
	pObject3D = new CObject3D;

	// 初期化処理
	pObject3D->Init();

	// 生成したオブジェクトのポインタを返す
	return pObject3D;
}

//=============================================================================
// 生成処理
//=============================================================================
CObject3D* CObject3D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeigh, const float fDepth, const char* pTexFile)
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
	CObject3D* pObject3D = NULL;		// ポインタ保存用

	// オブジェクト2Dのインスタンス生成
	pObject3D = new CObject3D;

	// 初期化処理
	pObject3D->Init(pos, rot, fWidth, fHeigh, fDepth, pTexFile);

	// 生成したオブジェクトのポインタを返す
	return pObject3D;
}

////=============================================================================
//// コンストラクタ
////=============================================================================
//CObject3D::CObject3D()
//{
//	// 各変数クリア
//	m_pTexture = NULL;
//	m_pVtxBuff = NULL;
//
//	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	m_fWidth = 0.0f;
//	m_fHeight = 0.0f;
//	m_fAngle = 0.0f;
//	m_fLength = 0.0f;
//}

//=============================================================================
// コンストラクタ (オーバーロード)
//=============================================================================
CObject3D::CObject3D(int nPriority) : CObject(nPriority)
{
	// 各変数クリア
	m_nIdxTexture = 0;
	m_pVtxBuff = nullptr;

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_fDepth = 0.0f;
	m_fAngle = 0.0f;
	m_fLength = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CObject3D::~CObject3D()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CObject3D::Init(void)
{
	// デバイスの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	CTexture* pTexture = pManager->GetTextrue();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの登録
	m_nIdxTexture = pTexture->Register("data\\TEXTURE\\field000.jpg");

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 変数初期化
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fWidth = 250.0f;
	m_fHeight = 0.0f;
	m_fDepth = 250.0f;

	m_fAngle = atan2f(m_fWidth, m_fHeight);
	m_fLength = sqrtf(m_fWidth * m_fWidth + m_fHeight * m_fHeight);

	VERTEX_3D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, m_fHeight, m_fDepth);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, 0.0f, m_fDepth);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, 0.0f, -m_fDepth);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, m_fHeight, -m_fDepth);

	// 法線ベクトルの設定
	D3DXVECTOR3 norA, norB, norC;
	D3DXVECTOR3 vecLineA = pVtx[1].pos - pVtx[0].pos;
	D3DXVECTOR3 vecLineB = pVtx[2].pos - pVtx[0].pos;
	D3DXVec3Cross(&norA, &vecLineA, &vecLineB);

	norA = D3DXVECTOR3((vecLineA.y * vecLineB.z) - (vecLineA.z * vecLineB.y),
		(vecLineA.z * vecLineB.x) - (vecLineA.x * vecLineB.z),
		(vecLineA.x * vecLineB.y) - (vecLineA.y * vecLineB.x));

	vecLineA = pVtx[1].pos - pVtx[3].pos;
	vecLineB = pVtx[2].pos - pVtx[3].pos;

	D3DXVec3Cross(&norB, &vecLineA, &vecLineB);

	D3DXVec3Normalize(&norA, &norA);
	D3DXVec3Normalize(&norB, &norB);

	norC = (norA + -norB) * 0.5f;

	pVtx[0].nor = norA;
	pVtx[1].nor = norC;
	pVtx[2].nor = norC;
	pVtx[3].nor = -norB;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CObject3D::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const float fDepth, const char* pTexFile)
{
	// デバイスの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	CTexture* pTexture = pManager->GetTextrue();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの登録
	m_nIdxTexture = pTexture->Register(pTexFile);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 変数初期化
	m_pos = pos;
	m_rot = rot;
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fDepth = fDepth;

	m_fAngle = atan2f(m_fWidth, m_fHeight);
	m_fLength = sqrtf(m_fWidth * m_fWidth + m_fHeight * m_fHeight);

	VERTEX_3D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, m_fHeight, m_fDepth);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, -m_fHeight, -m_fDepth);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, -m_fHeight, -m_fDepth);

	// 法線ベクトルの設定
	D3DXVECTOR3 norA, norB, norC;
	D3DXVECTOR3 vecLineA = pVtx[1].pos - pVtx[0].pos;
	D3DXVECTOR3 vecLineB = pVtx[2].pos - pVtx[0].pos;
	D3DXVec3Cross(&norA, &vecLineA, &vecLineB);

	norA = D3DXVECTOR3((vecLineA.y * vecLineB.z) - (vecLineA.z * vecLineB.y),
		(vecLineA.z * vecLineB.x) - (vecLineA.x * vecLineB.z),
		(vecLineA.x * vecLineB.y) - (vecLineA.y * vecLineB.x));

	vecLineA = pVtx[1].pos - pVtx[3].pos;
	vecLineB = pVtx[2].pos - pVtx[3].pos;

	D3DXVec3Cross(&norB, &vecLineA, &vecLineB);

	D3DXVec3Normalize(&norA, &norA);
	D3DXVec3Normalize(&norB, &norB);

	norC = (norA + -norB) * 0.5f;

	pVtx[0].nor = norA;
	pVtx[1].nor = norC;
	pVtx[2].nor = norC;
	pVtx[3].nor = -norB;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CObject3D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// 開放
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CObject3D::Update(void)
{
	// 今の角度と長さ再設定
	m_fAngle = atan2f(m_fWidth, m_fHeight);
	m_fLength = sqrtf(m_fWidth * m_fWidth + m_fHeight * m_fHeight);

	// 角度の範囲制限
	m_fAngle = AngleNormalize(m_fAngle);

	//SetRotation(D3DXVECTOR3(m_rot.x, m_rot.y + 0.01f, m_rot.z));

	//static float fHeight = 500.0f;

	//CManager* pManager = CManager::GetInstance();
	//CInputKeyboard* pInputKeyboard = pManager->GetInputKeyboard();

	//if (pInputKeyboard->GetPress(DIK_F1) == true)
	//{
	//	fHeight += -1.0f;
	//}

	//if (pInputKeyboard->GetPress(DIK_F2) == true)
	//{
	//	fHeight += 1.0f;
	//}

	//VERTEX_3D* pVtx;			// 頂点情報へのポインタ

	//// 頂点バッファをロックし,頂点情報へのポインタを取得
	//m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//// 頂点座標の設定
	//pVtx[0].pos = D3DXVECTOR3(-m_fWidth, m_fHeight, m_fDepth);
	//pVtx[1].pos = D3DXVECTOR3(m_fWidth, fHeight, m_fDepth);
	//pVtx[2].pos = D3DXVECTOR3(-m_fWidth, fHeight, -m_fDepth);
	//pVtx[3].pos = D3DXVECTOR3(m_fWidth, m_fHeight, -m_fDepth);

	//// 法線ベクトルの設定
	//D3DXVECTOR3 norA, norB, norC;
	//D3DXVECTOR3 vecLineA = pVtx[1].pos - pVtx[0].pos;
	//D3DXVECTOR3 vecLineB = pVtx[2].pos - pVtx[0].pos;
	//D3DXVec3Cross(&norA, &vecLineA, &vecLineB);

	//vecLineA = pVtx[1].pos - pVtx[3].pos;
	//vecLineB = pVtx[2].pos - pVtx[3].pos;

	//D3DXVec3Cross(&norB, &vecLineA, &vecLineB);

	//D3DXVec3Normalize(&norA, &norA);
	//D3DXVec3Normalize(&norB, &norB);

	//norC = (norA + -norB) * 0.5f;

	//pVtx[0].nor = norA;
	//pVtx[1].nor = norC;
	//pVtx[2].nor = norC;
	//pVtx[3].nor = -norB;

	//// 頂点バッファをアンロックする
	//m_pVtxBuff->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void CObject3D::Draw(void)
{
	// デバイスの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	CTexture* pTexture = pManager->GetTextrue();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// 位置指定処理
//=============================================================================
void CObject3D::SetPosition(const D3DXVECTOR3 pos)
{
	// 位置変更
	m_pos = pos;
}

//=============================================================================
// 位置取得処理
//=============================================================================
D3DXVECTOR3 CObject3D::GetPosition(void)
{
	return m_pos;
}

//=============================================================================
// 向き指定処理
//=============================================================================
void CObject3D::SetRotation(const D3DXVECTOR3 rot)
{
	// 向き変更
	m_rot = rot;

	// 向きの範囲制限
	m_rot.x = AngleNormalize(m_rot.x);
	m_rot.y = AngleNormalize(m_rot.y);
	m_rot.z = AngleNormalize(m_rot.z);
}

//=============================================================================
// 向き取得処理
//=============================================================================
D3DXVECTOR3 CObject3D::GetRotation(void)
{
	return m_rot;
}

//=============================================================================
// 大きさ変更処理
//=============================================================================
void CObject3D::SetSize(const float fWidth, const float fHeight, const float fDepth)
{
	// 大きさ変更
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fDepth = fDepth;

	VERTEX_3D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, m_fHeight, m_fDepth);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, -m_fHeight, -m_fDepth);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, -m_fHeight, -m_fDepth);

	// 法線ベクトルの設定
	D3DXVECTOR3 norA, norB, norC;
	D3DXVECTOR3 vecLineA = pVtx[1].pos - pVtx[0].pos;
	D3DXVECTOR3 vecLineB = pVtx[2].pos - pVtx[0].pos;
	D3DXVec3Cross(&norA, &vecLineA, &vecLineB);

	norA = D3DXVECTOR3((vecLineA.y * vecLineB.z) - (vecLineA.z * vecLineB.y),
		(vecLineA.z * vecLineB.x) - (vecLineA.x * vecLineB.z),
		(vecLineA.x * vecLineB.y) - (vecLineA.y * vecLineB.x));

	vecLineA = pVtx[1].pos - pVtx[3].pos;
	vecLineB = pVtx[2].pos - pVtx[3].pos;

	D3DXVec3Cross(&norB, &vecLineA, &vecLineB);

	D3DXVec3Normalize(&norA, &norA);
	D3DXVec3Normalize(&norB, &norB);

	norC = (norA + -norB) * 0.5f;

	pVtx[0].nor = norA;
	pVtx[1].nor = norC;
	pVtx[2].nor = norC;
	pVtx[3].nor = -norB;

	// 頂点バッファのアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 大きさ取得処理
//=============================================================================
D3DXVECTOR3 CObject3D::GetSize(void)
{
	D3DXVECTOR3 size = { m_fWidth, m_fHeight, m_fDepth};
	return size;
}

//=============================================================================
// 色変更処理
//=============================================================================
void CObject3D::SetColor(const D3DXCOLOR col)
{
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// 頂点バッファのアンロック
	m_pVtxBuff->Unlock();
}