//=============================================================================
//
//	オブジェクト2Dクラス [object2D.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "object.h"
#include "object2D.h"
#include "renderer.h"
#include "texture.h"

//=============================================================================
// 生成処理
//=============================================================================
CObject2D* CObject2D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeigh)
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
	CObject2D* pObject2D = NULL;		// ポインタ保存用

	// オブジェクト2Dのインスタンス生成
	pObject2D = new CObject2D;

	// 初期化処理
	pObject2D->Init(pos, rot, fWidth, fHeigh);

	// 生成したオブジェクトのポインタを返す
	return pObject2D;
}

//=============================================================================
// 生成処理
//=============================================================================
CObject2D* CObject2D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeigh, const char* pTexFileName)
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
	CObject2D* pObject2D = NULL;		// ポインタ保存用

	// テクスチャインスタンスへのポインタを取得
	CManager* pManager = CManager::GetInstance();
	CTexture* pTexture = pManager->GetTextrue();

	// オブジェクト2Dのインスタンス生成
	pObject2D = new CObject2D;

	// 初期化処理
	pObject2D->Init(pos, rot, fWidth, fHeigh);

	// テクスチャの登録
	pObject2D->m_nIdxTexture = pTexture->Register(pTexFileName);

	// 生成したオブジェクトのポインタを返す
	return pObject2D;
}

//=============================================================================
// 生成処理
//=============================================================================
CObject2D* CObject2D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeigh, const int nPriority)
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
	CObject2D* pObject2D = NULL;		// ポインタ保存用

	// オブジェクト2Dのインスタンス生成
	pObject2D = new CObject2D(nPriority);

	// 初期化処理
	pObject2D->Init(pos, rot, fWidth, fHeigh);

	// 生成したオブジェクトのポインタを返す
	return pObject2D;
}

////=============================================================================
//// コンストラクタ
////=============================================================================
//CObject2D::CObject2D()
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
CObject2D::CObject2D(int nPriority) : CObject(nPriority)
{
	// 各変数クリア
	m_pTexture = NULL;
	m_pVtxBuff = NULL;

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_fAngle = 0.0f;
	m_fLength = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CObject2D::~CObject2D()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CObject2D::Init(void)
{
	// レンダラーの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 変数初期化
	m_pos = INIT_POLYGONPOS;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI);
	m_fWidth = POLYGON_WIDTH;
	m_fHeight = POLYGON_HEIGHT;

	m_fAngle = atan2f(m_fWidth, m_fHeight);
	m_fLength = sqrtf(m_fWidth * m_fWidth + m_fHeight * m_fHeight);

	VERTEX_2D* pVtx;			//頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + D3DX_PI + m_fAngle) * m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + D3DX_PI + m_fAngle) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファのアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CObject2D::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight)
{
	// レンダラーの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 変数初期化
	m_pos = pos;
	m_rot = rot;
	m_col = COLOR_WHITE;
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	m_fAngle = atan2f(m_fWidth, m_fHeight);
	m_fLength = sqrtf(m_fWidth * m_fWidth + m_fHeight * m_fHeight);

	VERTEX_2D* pVtx;			//頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + D3DX_PI + m_fAngle) * m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + D3DX_PI + m_fAngle) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファのアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CObject2D::Uninit(void)
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
void CObject2D::Update(void)
{
	// 今の角度と長さ再設定
	m_fAngle = atan2f(m_fWidth, m_fHeight);
	m_fLength = sqrtf(m_fWidth * m_fWidth + m_fHeight * m_fHeight);

	// 角度の範囲制限
	m_fAngle = AngleNormalize(m_fAngle);
}

//=============================================================================
// 描画処理
//=============================================================================
void CObject2D::Draw(void)
{
	// レンダラーの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの取得
	CTexture* pTexture = pManager->GetTextrue();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	// 描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		2);
}

//=============================================================================
// テクスチャの割り当て
//=============================================================================
void CObject2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
// テクスチャ取得
//=============================================================================
LPDIRECT3DTEXTURE9 CObject2D::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
// 頂点バッファ取得
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CObject2D::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
// 位置指定処理
//=============================================================================
void CObject2D::SetPosition(const D3DXVECTOR3 pos)
{
	// 位置変更
	m_pos = pos;

	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + D3DX_PI + m_fAngle) * m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + D3DX_PI + m_fAngle) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	// 頂点バッファのアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 位置取得処理
//=============================================================================
D3DXVECTOR3 CObject2D::GetPosition(void)
{
	return m_pos;
}

//=============================================================================
// 向き指定処理
//=============================================================================
void CObject2D::SetRotation(const D3DXVECTOR3 rot)
{
	// 向き変更
	m_rot = rot;

	// 向きの範囲制限
	m_rot.z = AngleNormalize(m_rot.z);
}

//=============================================================================
// 向き取得処理
//=============================================================================
D3DXVECTOR3 CObject2D::GetRotation(void)
{
	return m_rot;
}

//=============================================================================
// 大きさ変更処理
//=============================================================================
void CObject2D::SetSize(const float fWidth, const float fHeight)
{
	// 大きさ変更
	m_fWidth = fWidth;
	m_fHeight = fHeight;
}

//=============================================================================
// 大きさ取得処理
//=============================================================================
D3DXVECTOR2 CObject2D::GetSize(void)
{
	D3DXVECTOR2 size = { m_fWidth, m_fHeight };
	return size;
}

//=============================================================================
// 色変更処理
//=============================================================================
void CObject2D::SetColor(const D3DXCOLOR col)
{
	m_col = col;

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