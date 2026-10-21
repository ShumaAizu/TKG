//=============================================================================
//
//	マネージャクラス [manager.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードガード
//*****************************************************************************
#ifndef _MANAGER_H_
#define _MANAGER_H_

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "object.h"
#include "scene.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CRenderer;			// レンダラークラス
class CInputKeyboard;		// キーボードクラス
class CInputMouse;			// マウスクラス
class CInputJoypad;			// ジョイパッドクラス
class CSound;				// サウンドクラス
class CTexture;				// テクスチャクラス
class CCamera;				// カメラクラス
class CFreeCamera;			// フリーカメラクラス
class CLight;				// ライトクラス
class CScore;				// スコアクラス
class CTimer;				// タイマークラス

class CObject3D;			// オブジェクト3Dクラス

class CObjectLoader;		// オブジェクトローダークラス

class CMyImGui;				// ImGuiクラス
class CCanvas;				// キャンバスクラス

class CBatchingEffect;		// バッチ描画エフェクト

class CFade;				// フェードクラス
class CPause;				// ポーズクラス

class CTcpClient;			// クライアントクラス

//*****************************************************************************
// マネージャクラスの定義
//*****************************************************************************
class CManager
{
public:
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CManager* GetInstance(void);

	CRenderer* GetRenderer(void) const { return m_pRenderer; }
	CInputKeyboard* GetInputKeyboard(void) const { return m_pInputKeyboard; }
	CInputMouse* GetInputMouse(void) const { return m_pInputMouse; }
	CInputJoypad* GetInputJoypad(void) const { return m_pInputJoypad; }
	CSound* GetSound(void) const { return m_pSound; }
	CTexture* GetTextrue(void) const { return m_pTexture; }
	CCamera* GetCamera(void) const { return m_pCamera; }
	CLight* GetLight(void) const { return m_pLight; }
	CScore* GetScore(void) const { return m_pScore; }
	CObject3D* GetObject3D(void) const { return m_pObject3D; }

	CMyImGui* GetImGui(void) const { return m_pImGui; }
	CCanvas* GetCanvas(void) const { return m_pCanvas; }
	CBatchingEffect* GetBatchingEffect(void) const { return m_pBatchingEffect; }

	CFade* GetFade(void) const { return m_pFade; }
	CPause* GetPause(void) const { return m_pPause; }

	void SetMode(CScene::MODE mode);
	CScene::MODE GetMode(void);

	CTcpClient* GetTcpClient(void) { return m_pTcpClient; }

	physx::PxPhysics* GetPhysics(void) { return m_pPhysics; }
	physx::PxScene* GetPxScene(void) { return m_pPxScene; }

private:
	CManager();
	~CManager();

	void RankingTest(void);

	CRenderer* m_pRenderer;				// レンダラーへのポインタ
	CInputKeyboard* m_pInputKeyboard;	// キーボードへのポインタ
	CInputMouse* m_pInputMouse;			// マウスへのポインタ
	CInputJoypad* m_pInputJoypad;		// ジョイパッドへのポインタ
	CSound* m_pSound;					// サウンドへのポインタ
	CTexture* m_pTexture;				// テクスチャへのポインタ
	CCamera* m_pCamera;					// カメラへのポインタ
	CLight* m_pLight;					// ライトへのポインタ
	CScore* m_pScore;					// スコアへのポインタ
	CTimer* m_pTimer;					// タイマーへのポインタ
	CObject3D* m_pObject3D;				// オブジェクト3Dへのポインタ

	CObjectLoader* m_pObjectLoader;		// オブジェクトローダーへのポインタ

	CScene* m_pScene;					// シーンへのポインタ
	CMyImGui* m_pImGui;					// ImGuiへのポインタ
	CCanvas* m_pCanvas;					// キャンバスへのポインタ

	CBatchingEffect* m_pBatchingEffect;	// バッチ描画エフェクトへのポインタ

	CFade* m_pFade;						// フェードへのポインタ
	CPause* m_pPause;					// ポーズへのポインタ

	CTcpClient* m_pTcpClient;			// クライアントへのポインタ

	// PhysX内で利用するアロケーター
	physx::PxDefaultAllocator m_defaultAllocator;

	// エラー時用のコールバックでエラー内容が入ってる
	physx::PxDefaultErrorCallback m_defaultErrorCallback;

	// 上位レベルのSDK(PxPhysicsなど)をインスタンス化する際に必要
	physx::PxFoundation* m_pFoundation;

	// 実際に物理演算を行う
	physx::PxPhysics* m_pPhysics;

	// シミュレーションをどう処理するかの設定でマルチスレッドの設定もできる
	physx::PxDefaultCpuDispatcher* m_pDispatcher;

	// シミュレーションする空間の単位でActorの追加などもここで行う
	physx::PxScene* m_pPxScene;

	// PVDと通信する際に必要
	physx::PxPvd* m_pPvd;
};

#endif