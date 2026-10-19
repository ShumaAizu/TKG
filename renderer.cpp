//=============================================================================
//
//	レンダラークラス [renderer.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "object.h"
#include "debugproc.h"
#include "fade.h"
#include "batchingeffect.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_MTPOLYGON_MUL		(0.525f)	// ポリゴンサイズの倍率の最大
#define DEFAULT_MTPOLYGON_MUL	(0.5f)		// ポリゴンサイズの倍率のデフォルト

//=============================================================================
// コンストラクタ
//=============================================================================
CRenderer::CRenderer()
{
	// 各変数クリア
	m_pD3D = nullptr;
	m_pD3DDevice = nullptr;

	memset(&m_apTextureMT, NULL, sizeof(LPDIRECT3DTEXTURE9) * NUM_MULTITARGET);
	memset(&m_apRenderMT, NULL, sizeof(LPDIRECT3DSURFACE9) * NUM_MULTITARGET);
	m_pZBuffMT = nullptr;
	memset(&m_viewportMT, 0, sizeof(D3DVIEWPORT9));

	m_pVtxBuffMT = nullptr;
	m_fPolygonMul = 0.0f;

	m_isEnableFeedback = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CRenderer::~CRenderer()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	// プレゼンテーションパラメータ

	// DirectXオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{// 生成に失敗したら
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// パラメータのゼロクリア
	d3dpp.BackBufferWidth = SCREEN_WIDTH;							// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;							// バックバッファの形成
	d3dpp.BackBufferCount = 1;										// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						// ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;							// デブスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						// デブスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;										// ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル

	// Direct3Dデバイスの生成
	// 描画処理と頂点処理をハードウェアで行う
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{
		// 頂点処理のみCPUで行う
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			// 両方CPUで行う
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// カリングの設定
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				//
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// アルファブレンドの設定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャの拡縮補間の設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャの繰り返し設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//// 霧の設定
	//m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);

	//m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//m_pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);

	//m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, 10.0f);
	//m_pD3DDevice->SetRenderState(D3DRS_FOGEND, 10000.0f);

	// 乱数シードセット
	srand((unsigned)timeGetTime());

	// ローカル変数
	LPDIRECT3DSURFACE9 pRenderDef, pZBuffDef;

	// 現在のレンダリングターゲットを取得 (保存)
	m_pD3DDevice->GetRenderTarget(0, &pRenderDef);

	// 現在のZバッファを取得 (保存)
	m_pD3DDevice->GetDepthStencilSurface(&pZBuffDef);

	// テクスチャレンダリング用Zバッファ生成
	m_pD3DDevice->CreateDepthStencilSurface(SCREEN_WIDTH,
		SCREEN_HEIGHT,
		D3DFMT_D16,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&m_pZBuffMT,
		nullptr);

	// Zバッファを生成したバッファに設定
	m_pD3DDevice->SetDepthStencilSurface(m_pZBuffMT);

	for (int nCntMT = 0; nCntMT < NUM_MULTITARGET; nCntMT++)
	{
		// レンダリングターゲット用テクスチャの生成
		m_pD3DDevice->CreateTexture(SCREEN_WIDTH,
			SCREEN_HEIGHT,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT,
			&m_apTextureMT[nCntMT],
			NULL);

		// テクスチャレンダリング用インターフェースの生成
		m_apTextureMT[nCntMT]->GetSurfaceLevel(0, &m_apRenderMT[nCntMT]);

		// レンダリングターゲットを生成したテクスチャに設定
		m_pD3DDevice->SetRenderTarget(0, m_apRenderMT[nCntMT]);

		// レンダリングターゲット用テクスチャのクリア
		m_pD3DDevice->Clear(0, NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	}

	// レンダリングターゲット Zバッファを元に戻す
	m_pD3DDevice->SetRenderTarget(0, pRenderDef);
	m_pD3DDevice->SetDepthStencilSurface(pZBuffDef);

	// テクスチャレンダリング用ビューポートの設定
	m_viewportMT.X = 0;
	m_viewportMT.Y = 0;
	m_viewportMT.Width = SCREEN_WIDTH;
	m_viewportMT.Height = SCREEN_HEIGHT;
	m_viewportMT.MinZ = 0.0f;
	m_viewportMT.MaxZ = 1.0f;

	m_fPolygonMul = DEFAULT_MTPOLYGON_MUL;

	// 頂点バッファの生成
	m_pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffMT,
		nullptr);

	VERTEX_2D* pVtx;			//頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuffMT->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(pos.x - SCREEN_WIDTH * m_fPolygonMul, pos.y - SCREEN_HEIGHT * m_fPolygonMul, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + SCREEN_WIDTH * m_fPolygonMul, pos.y - SCREEN_HEIGHT * m_fPolygonMul, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - SCREEN_WIDTH * m_fPolygonMul, pos.y + SCREEN_HEIGHT * m_fPolygonMul, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + SCREEN_WIDTH * m_fPolygonMul, pos.y + SCREEN_HEIGHT * m_fPolygonMul, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファのアンロック
	m_pVtxBuffMT->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CRenderer::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuffMT != nullptr)
	{
		m_pVtxBuffMT->Release();
		m_pVtxBuffMT = nullptr;
	}

	// レンダリングターゲット用テクスチャの破棄
	for (int nCntTexMT = 0; nCntTexMT < NUM_MULTITARGET; nCntTexMT++)
	{
		if (m_apTextureMT[nCntTexMT] == nullptr)
		{// 使用していなければスキップ
			continue;
		}

		m_apTextureMT[nCntTexMT]->Release();
		m_apTextureMT[nCntTexMT] = nullptr;
	}

	// テクスチャレンダリング用インターフェースの破棄
	for (int nCntRenderMT = 0; nCntRenderMT < NUM_MULTITARGET; nCntRenderMT++)
	{
		if (m_apRenderMT[nCntRenderMT] == nullptr)
		{// 使用していなければスキップ
			continue;
		}

		m_apRenderMT[nCntRenderMT]->Release();
		m_apRenderMT[nCntRenderMT] = nullptr;
	}

	// テクスチャレンダリング用Zバッファの破棄
	if (m_pZBuffMT != nullptr)
	{
		m_pZBuffMT->Release();
		m_pZBuffMT = nullptr;
	}

	// Direct3Dデバイスの破棄
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CRenderer::Update(void)
{
	// フィードバックエフェクトの更新処理
	UpdateFeedback();

	// オブジェクトの更新
	CObject::UpdateAll();

	// デバッグ表示の更新処理
	CDebugProc::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CRenderer::Draw(void)
{
	// 独立描画オブジェクトの取得
	CManager* pManager = CManager::GetInstance();
	CMyImGui* pImGui = pManager->GetImGui();
	CFade* pFade = pManager->GetFade();

	// 画面クリア(バックバッファとZバッファのクリア)
	m_pD3DDevice->Clear(0, nullptr,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// 描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{// 描画開始が成功した場合

		// フィードバックエフェクトの描画
		SetFeedback();

		// デバッグ表示の描画
		CDebugProc::Draw();

		// 描画終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

//=============================================================================
// フィードバックエフェクトを設定
//=============================================================================
void CRenderer::SetFeedback(void)
{
	// 独立描画オブジェクトの取得
	CManager* pManager = CManager::GetInstance();
	CFade* pFade = pManager->GetFade();
	CBatchingEffect* pBatchingEffect = pManager->GetBatchingEffect();

	// 保存用変数
	LPDIRECT3DSURFACE9 pRenderWk;
	LPDIRECT3DTEXTURE9 pTextureWk;
	LPDIRECT3DSURFACE9 pRenderDef, pZBuffDef;

	// 現在のレンダリングターゲットを取得 (保存)
	m_pD3DDevice->GetRenderTarget(0, &pRenderDef);

	// 現在のZバッファを取得 (保存)
	m_pD3DDevice->GetDepthStencilSurface(&pZBuffDef);

	// レンダリングターゲット変更
	ChangeTarget();

	// レンダリングターゲット用テクスチャのクリア
	m_pD3DDevice->Clear(0, nullptr,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(32, 0, 32, 255), 1.0f, 0);

	// オブジェクトの描画
	CObject::DrawAll();

	// バッチ描画エフェクトの描画
	if (pBatchingEffect != nullptr)
	{
		pBatchingEffect->Draw();
	}

	// フェードの描画
	if (pFade != nullptr)
	{
		pFade->Draw();
	}

	if (m_isEnableFeedback == true)
	{// フィードバックエフェクトがオンなら前回の画面も描画
		DrawMT(1);
	}

	// レンダリングターゲット Zバッファを元に戻す
	m_pD3DDevice->SetRenderTarget(0, pRenderDef);
	m_pD3DDevice->SetDepthStencilSurface(pZBuffDef);

	// アルファブレンドをオフにして不透明で描画
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	// 今回の描画結果テクスチャをポリゴンに貼り付けて描画
	DrawMT(0);

	// アルファブレンドを戻す
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	// テクスチャ0と1を入れ替える
	pTextureWk = m_apTextureMT[0];
	m_apTextureMT[0] = m_apTextureMT[1];
	m_apTextureMT[1] = pTextureWk;

	pRenderWk = m_apRenderMT[0];
	m_apRenderMT[0] = m_apRenderMT[1];
	m_apRenderMT[1] = pRenderWk;
}

//=============================================================================
// フィードバック用ポリゴンのリセット
//=============================================================================
void CRenderer::ResetEnableFeedback(void)
{
	VERTEX_2D* pVtx;			//頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuffMT->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * DEFAULT_MTPOLYGON_MUL, SCREEN_HEIGHT * DEFAULT_MTPOLYGON_MUL, 0.0f);

	m_fPolygonMul = DEFAULT_MTPOLYGON_MUL;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(pos.x - SCREEN_WIDTH * m_fPolygonMul, pos.y - SCREEN_HEIGHT * m_fPolygonMul, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + SCREEN_WIDTH * m_fPolygonMul, pos.y - SCREEN_HEIGHT * m_fPolygonMul, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - SCREEN_WIDTH * m_fPolygonMul, pos.y + SCREEN_HEIGHT * m_fPolygonMul, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + SCREEN_WIDTH * m_fPolygonMul, pos.y + SCREEN_HEIGHT * m_fPolygonMul, 0.0f);

	// 頂点バッファのアンロック
	m_pVtxBuffMT->Unlock();

	m_isEnableFeedback = false;
}

//=============================================================================
// フィードバック用ポリゴンの更新
//=============================================================================
void CRenderer::UpdateFeedback(void)
{
	VERTEX_2D* pVtx;			//頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuffMT->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * DEFAULT_MTPOLYGON_MUL, SCREEN_HEIGHT * DEFAULT_MTPOLYGON_MUL, 0.0f);

	if (m_isEnableFeedback == true && m_fPolygonMul <= MAX_MTPOLYGON_MUL)
	{// フィードバックエフェクト適用中に今の倍率が最大より低かったら
		m_fPolygonMul += (MAX_MTPOLYGON_MUL - m_fPolygonMul) * 0.05f;
	}

	if (m_isEnableFeedback == false && m_fPolygonMul >= DEFAULT_MTPOLYGON_MUL)
	{// フィードバックエフェクト適用外に今の倍率がデフォルトよりより高かったら
		m_fPolygonMul += (DEFAULT_MTPOLYGON_MUL - m_fPolygonMul) * 0.05f;
	}

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(pos.x - SCREEN_WIDTH * m_fPolygonMul, pos.y - SCREEN_HEIGHT * m_fPolygonMul, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + SCREEN_WIDTH * m_fPolygonMul, pos.y - SCREEN_HEIGHT * m_fPolygonMul, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - SCREEN_WIDTH * m_fPolygonMul, pos.y + SCREEN_HEIGHT * m_fPolygonMul, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + SCREEN_WIDTH * m_fPolygonMul, pos.y + SCREEN_HEIGHT * m_fPolygonMul, 0.0f);

	// 頂点バッファのアンロック
	m_pVtxBuffMT->Unlock();
}

//=============================================================================
// フィードバック用ポリゴンへの描画
//=============================================================================
void CRenderer::DrawMT(int nTexIdx)
{
	// 頂点フォーマットの設定
	m_pD3DDevice->SetFVF(FVF_VERTEX_2D);

	// 頂点バッファをデータストリームに設定
	m_pD3DDevice->SetStreamSource(0, m_pVtxBuffMT, 0, sizeof(VERTEX_2D));

	// テクスチャの設定
	m_pD3DDevice->SetTexture(0, m_apTextureMT[nTexIdx]);

	// 描画
	m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		2);
}

//=============================================================================
// レンダリング対象変更処理
//=============================================================================
void CRenderer::ChangeTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU)
{
	// ローカル変数
	D3DXMATRIX mtxView, mtxProjection;
	float fAspect;

	// レンダリングターゲットを生成したテクスチャに設定
	m_pD3DDevice->SetRenderTarget(0, m_apRenderMT[0]);

	// Zバッファを生成したZバッファに設定
	m_pD3DDevice->SetDepthStencilSurface(m_pZBuffMT);

	// テクスチャレンダリング用のビューポートを設定
	m_pD3DDevice->SetViewport(&m_viewportMT);

	// ビューマトリックスを初期化
	D3DXMatrixIdentity(&mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&mtxView,
		&posV,
		&posR,
		&vecU);

	// ビューマトリックスの設定
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&mtxProjection);

	// プロジェクションマトリックスを作成
	fAspect = (float)m_viewportMT.Width / (float)m_viewportMT.Height;
	D3DXMatrixPerspectiveFovLH(&mtxProjection,
		D3DXToRadian(45.0f),
		fAspect,
		10.0f,
		10000.0f);

	// プロジェクションマトリックスの設定
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);
}

//=============================================================================
// レンダリング対象変更処理 (カメラ指定なし)
//=============================================================================
void CRenderer::ChangeTarget(void)
{
	// レンダリングターゲットを生成したテクスチャに設定
	m_pD3DDevice->SetRenderTarget(0, m_apRenderMT[0]);

	// Zバッファを生成したZバッファに設定
	m_pD3DDevice->SetDepthStencilSurface(m_pZBuffMT);

	// テクスチャレンダリング用のビューポートを設定
	m_pD3DDevice->SetViewport(&m_viewportMT);
}

//==================================================================================
//	テクスチャステージステートのアルファの設定
//==================================================================================
void CRenderer::SetTextureStageStateAlpha(const int nIdx, const DWORD Stage, const D3DTEXTUREOP Type, const DWORD Value)
{
	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(nIdx, D3DTSS_ALPHAARG1, Stage);
	m_pD3DDevice->SetTextureStageState(nIdx, D3DTSS_ALPHAOP, Type);
	m_pD3DDevice->SetTextureStageState(nIdx, D3DTSS_ALPHAARG2, Value);
}

//==================================================================================
//	テクスチャステージステートのアルファのリセット
//==================================================================================
void CRenderer::ResetTextureStageStateAlpha(const int nNumIdx)
{
	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	for (int nCntIdx = 1; nCntIdx < nNumIdx; nCntIdx++)
	{
		m_pD3DDevice->SetTextureStageState(nCntIdx, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pD3DDevice->SetTextureStageState(nCntIdx, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		m_pD3DDevice->SetTextureStageState(nCntIdx, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	}
}

//==================================================================================
//	テクスチャステージステートのカラーの設定
//  nIdx  : 設定するテクスチャステージのインデックス
//  Stage : 合成に使用する色 (何で)
//  Type  : どういう合成方法か
//  Value : 合成に使用する色 (何に)
//==================================================================================
void CRenderer::SetTextureStageStateColor(const int nIdx, const DWORD Stage, const D3DTEXTUREOP Type, const DWORD Value)
{
	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(nIdx, D3DTSS_COLORARG1, Stage);
	m_pD3DDevice->SetTextureStageState(nIdx, D3DTSS_COLOROP, Type);
	m_pD3DDevice->SetTextureStageState(nIdx, D3DTSS_COLORARG2, Value);
}

//==================================================================================
//	テクスチャステージステートのカラーのリセット
//==================================================================================
void CRenderer::ResetTextureStageStateColor(const int nNumIdx)
{
	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);

	for (int nCntIdx = 1; nCntIdx < nNumIdx; nCntIdx++)
	{
		m_pD3DDevice->SetTextureStageState(nCntIdx, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pD3DDevice->SetTextureStageState(nCntIdx, D3DTSS_COLOROP, D3DTOP_DISABLE);
		m_pD3DDevice->SetTextureStageState(nCntIdx, D3DTSS_COLORARG2, D3DTA_CURRENT);
	}
}