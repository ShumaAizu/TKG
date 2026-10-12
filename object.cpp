//=============================================================================
//
//	オブジェクトクラス [object.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DRAW_CANVAS			// キャンバスへの描画
#undef DRAW_CANVAS

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CObject* CObject::m_apObject[PRIORITY_MAX][MAX_OBJECT] = {};	// オブジェクトへのポインタ
int CObject::m_nNumAll = 0;										// オブジェクトの総数
CObject* CObject::m_apTop[PRIORITY_MAX] = {};					// 先頭オブジェクトへのポインタ
CObject* CObject::m_apCur[PRIORITY_MAX] = {};					// 最後尾オブジェクトへのポインタ

bool CObject::m_isFreeCamera = false;

//=============================================================================
// コンストラクタ (オーバーロード)
//=============================================================================
CObject::CObject(int nPriority)
{
#ifndef ENABLE_LINKLIST_OBJECT
	// 自分自身のポインタを全体リストに保管
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (m_apObject[nPriority][nCntObject] != NULL)
		{// オブジェクトが存在している
			continue;
		}

		// オブジェクトに自分自身のポインタを代入し、IDと優先順位を保存し抜ける
		m_apObject[nPriority][nCntObject] = this;
		m_nID = nCntObject;
		m_nPriority = nPriority;
		break;
	}

	// 総数を増やす
	m_nNumAll++;
#endif // !ENABLE_LINKLIST_OBJECT

#ifdef ENABLE_LINKLIST_OBJECT
	if (m_apTop[nPriority] == NULL)
	{// 先頭オブジェクトが存在しなければ
		m_apTop[nPriority] = this;
		m_pPrev = NULL;
	}

	if (m_apCur[nPriority] != NULL)
	{// 最後尾オブジェクトが存在していれば
		// 自分自身の前のオブジェクトに
		m_pPrev = m_apCur[nPriority];

		// 最後尾のオブジェクトの次のオブジェクトに自分自身を
		m_apCur[nPriority]->m_pNext = this;
	}

	// 最後尾オブジェクトへのポインタに自分自身を入れる
	m_apCur[nPriority] = this;
	m_pNext = NULL;

	// 自分の優先順位を保存
	m_nPriority = nPriority;
	m_isDeath = false;
#endif // ENABLE_LINKLIST_OBJECT
}

//=============================================================================
// デストラクタ
//=============================================================================
CObject::~CObject()
{
	
}

//=============================================================================
// 全開放処理
//=============================================================================
void CObject::ReleaseAll(void)
{
#ifndef ENABLE_LINKLIST_OBJECT
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
		{
			if (m_apObject[nCntPriority][nCntObject] == NULL)
			{// オブジェクトが存在していない
				// スキップ
				continue;
			}

			// 終了処理
			m_apObject[nCntPriority][nCntObject]->Uninit();
		}
	}
#endif // !ENABLE_LINKLIST_OBJECT

#ifdef ENABLE_LINKLIST_OBJECT
	// ローカル変数
	CObject* pObject = NULL;
	CObject* pNextObject = NULL;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		// 先頭オブジェクトを取得
		pObject = m_apTop[nCntPriority];

		while (pObject != NULL)
		{// オブジェクトへのポインタが存在しなくなるまで
			// 次のオブジェクトへのポインタを保存
			pNextObject = pObject->m_pNext;

			// 終了処理
			pObject->Uninit();

			// 次のオブジェクトへのポインタを代入
			pObject = pNextObject;
		}
	}

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		// 先頭オブジェクトを取得
		pObject = m_apTop[nCntPriority];

		while (pObject != NULL)
		{// オブジェクトへのポインタが存在しなくなるまで
			// 次のオブジェクトへのポインタを保存
			pNextObject = pObject->m_pNext;

			if (pObject->m_isDeath == true)
			{// 死亡フラグが立っていたなら
				// 削除
				pObject->Delete();
			}

			// 次のオブジェクトへのポインタを代入
			pObject = pNextObject;
		}
	}
#endif // ENABLE_LINKLIST_OBJECT
}

//=============================================================================
// 全更新処理
//=============================================================================
void CObject::UpdateAll(void)
{
	// カメラの取得
	CManager* pManager = CManager::GetInstance();
	CCamera* pCamera = pManager->GetCamera();

	// カメラの更新
	pCamera->Update();

#ifndef ENABLE_LINKLIST_OBJECT
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
		{
			if (m_apObject[nCntPriority][nCntObject] == NULL)
			{// オブジェクトが存在していない
				// スキップ
				continue;
			}

			// 更新処理
			m_apObject[nCntPriority][nCntObject]->Update();
		}
	}
#endif // !ENABLE_LINKLIST_OBJECT

#ifdef ENABLE_LINKLIST_OBJECT
	// ローカル変数
	CObject* pObject = NULL;
	CObject* pNextObject = NULL;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		// 先頭オブジェクトを取得
		pObject = m_apTop[nCntPriority];

		while (pObject != NULL)
		{// オブジェクトへのポインタが存在しなくなるまで
			// 次のオブジェクトへのポインタを保存
			if (m_apTop[nCntPriority] == NULL)
			{
				break;
			}

			pNextObject = pObject->m_pNext;

			// 更新処理
			if (pObject->m_isDeath != true)
			{
				pObject->Update();
			}

			// 次のオブジェクトへのポインタを代入
			pObject = pNextObject;
		}
	}

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		// 先頭オブジェクトを取得
		pObject = m_apTop[nCntPriority];

		while (pObject != NULL)
		{// オブジェクトへのポインタが存在しなくなるまで
			// 次のオブジェクトへのポインタを保存
			pNextObject = pObject->m_pNext;

			if (pObject->m_isDeath == true)
			{// 死亡フラグが立っていたなら
				// 削除
				pObject->Delete();
			}

			// 次のオブジェクトへのポインタを代入
			pObject = pNextObject;
		}
	}
#endif // ENABLE_LINKLIST_OBJECT
}

//=============================================================================
// 全描画処理
//=============================================================================
void CObject::DrawAll(void)
{
	// レンダラー, デバイスの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	CCamera* pCamera = NULL;

	// カメラの取得
	pCamera = pManager->GetCamera();

	// カメラの設定
	pCamera->Set();

#ifdef DRAW_CANVAS

	// 設定保存用変数
	LPDIRECT3DSURFACE9 pRenderDef, pZBuffDef;
	D3DVIEWPORT9 viewportDef;
	D3DXMATRIX mtxViewDef, mtxProjectionDef;

	// 現在の各デバイス設定を取得 (一時保存)
	pDevice->GetRenderTarget(0, &pRenderDef);
	pDevice->GetDepthStencilSurface(&pZBuffDef);
	pDevice->GetViewport(&viewportDef);
	pDevice->GetTransform(D3DTS_VIEW, &mtxViewDef);
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProjectionDef);

	// レンダリング対象変更
	pRenderer->ChangeTarget(pCamera->GetPositionV(), pCamera->GetPositionR(), pCamera->GetVectorU());

	// レンダリングターゲット用テクスチャのクリア
	pDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

#endif // DRAW_CANVAS

#ifndef ENABLE_LINKLIST_OBJECT
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
		{
			if (m_apObject[nCntPriority][nCntObject] == NULL)
			{// オブジェクトが存在していない
				// スキップ
				continue;
			}

			// 描画処理
			m_apObject[nCntPriority][nCntObject]->Draw();
		}
	}
#endif // !ENABLE_LINKLIST_OBJECT

#ifdef ENABLE_LINKLIST_OBJECT
	// ローカル変数
	CObject* pObject = NULL;
	CObject* pNextObject = NULL;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		// 先頭オブジェクトを取得
		pObject = m_apTop[nCntPriority];

		while (pObject != NULL)
		{// オブジェクトへのポインタが存在しなくなるまで
			// 次のオブジェクトへのポインタを保存
			pNextObject = pObject->m_pNext;

			// 描画処理
			pObject->Draw();

			// 次のオブジェクトへのポインタを代入
			pObject = pNextObject;
		}
	}
#endif // ENABLE_LINKLIST_OBJECT

#ifdef DRAW_CANVAS

	// 保存していたデバイス設定を元に戻す
	pDevice->SetRenderTarget(0, pRenderDef);
	pDevice->SetDepthStencilSurface(pZBuffDef);
	pDevice->SetViewport(&viewportDef);
	pDevice->SetTransform(D3DTS_VIEW, &mtxViewDef);
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjectionDef);

#endif // DRAW_CANVAS
}

//=============================================================================
// 開放処理
//=============================================================================
void CObject::Release(void)
{
#ifndef ENABLE_LINKLIST_OBJECT
	// 優先順位とIDの保存
	const int nPriority = m_nPriority;
	const int nID = m_nID;

	if (m_apObject[m_nPriority][nID] == NULL)
	{// オブジェクトが存在しない
		// 関数終了
		return;
	}

	// 自分自身の破棄
	delete m_apObject[nPriority][nID];

	// 自分の位置にNULLを
	m_apObject[nPriority][nID] = NULL;
	m_nNumAll--;
#endif // !ENABLE_LINKLIST_OBJECT

#ifdef ENABLE_LINKLIST_OBJECT
	// 死亡フラグを立てる
	m_isDeath = true;
#endif // ENABLE_LINKLIST_OBJECT
}

//=============================================================================
// 消去処理
//=============================================================================
void CObject::Delete(void)
{
	if (m_pPrev != NULL && m_pNext != NULL)
	{// 前のオブジェクトも次のオブジェクトもあるのなら
		// 自分の前のオブジェクトと次のオブジェクトを繋ぎ直す
		m_pPrev->m_pNext = m_pNext;
		m_pNext->m_pPrev = m_pPrev;
	}

	if (m_pPrev == NULL && m_pNext == NULL)
	{// 前のオブジェクトも次のオブジェクトもないなら (自分しかいない)
		// オブジェクトをまっさらにする
		m_apTop[m_nPriority] = NULL;
		m_apCur[m_nPriority] = NULL;
	}
	else
	{
		if (m_pPrev == NULL)
		{// もし自分が先頭だったら (前のオブジェクトが存在しない)
			m_apTop[m_nPriority] = m_pNext;
			m_apTop[m_nPriority]->m_pPrev = NULL;
		}

		if (m_pNext == NULL)
		{// もし自分が最後尾だったら (次のオブジェクトが存在しない)
			m_apCur[m_nPriority] = m_pPrev;
			m_apCur[m_nPriority]->m_pNext = NULL;
		}
	}

	// 自分自身の破棄
	delete this;
}