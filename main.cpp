//=============================================================================
//
//	TKG [main.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "main.h"
#include "input.h"
#include "manager.h"
#include "debugproc.h"

#ifdef new
#undef new
#endif // new

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME			"WindowClass"					// ウインドウクラスの名前
#define WINDOW_NAME			"TKG"							// ウインドウの名前	

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
RECT g_windowRect;							// ウィンドウサイズ

//=============================================================================
// メイン関数
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);			// メモリリーク検知用のフラグ
#endif // _DEBUG

	WNDCLASSEX wcex =									// ウインドウクラスの構造体
	{
		sizeof(WNDCLASSEX),								// WNDCLASSEXのメモリサイズ
		CS_CLASSDC,										// ウインドウのスタイル
		WindowProc,										// ウインドウプロシージャ
		0,												// 0にする(通常は使用しない)
		0,												// 0にする(通常は使用しない)
		hInstance,										// インスタンスハンドル
		LoadIcon(NULL, IDI_APPLICATION),				// タスクバーのアイコン
		LoadCursor(NULL, IDC_ARROW),					// マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),						// クライアント領域の背景色
		NULL,											// メニューバー
		CLASS_NAME,										// ウインドウクラスの名前
		LoadIcon(NULL, IDI_APPLICATION)					// ファイルのアイコン
	};

	HWND hWnd;				// ウインドウハンドル(識別子)
	MSG msg;				// メッセージを格納する変数
	DWORD dwCurrentTime;	// 現在時刻
	DWORD dwExecLastTime;	// 最後に処理した時刻
	DWORD dwFrameCount;		// フレームカウント
	DWORD dwFPSLastTime;	// 最後にFPSを計測した時刻
	int nCountFPS = 0;		// 今のFPSカウント
	int nFPS = 60;			// FPSの指定

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	// 画面サイズの構造体

	// ウインドウクラスの登録
	RegisterClassEx(&wcex);

	// クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// ウインドウを生成
	hWnd = CreateWindowEx(0,							// 拡張ウインドウスタイル
						  CLASS_NAME,					// ウインドウクラスの名前
						  WINDOW_NAME,					// ウインドウの名前
						  WS_OVERLAPPEDWINDOW,			// ウインドウスタイル
						  CW_USEDEFAULT,				// ウインドウの左上X座標
						  CW_USEDEFAULT,				// ウインドウの左上Y座標
						  (rect.right - rect.left),		// ウインドウの幅
						  (rect.bottom - rect.top),		// ウインドウの高さ
						  NULL,							// 親ウインドウのハンドル
						  NULL,							// メニューハンドルまたは子ウインドウID
						  hInstance,					// インスタンスハンドル
						  NULL);						// ウインドウ作成データ

	// マネージャへのポインタ
	CManager* pManager = CManager::GetInstance();

	// 初期化処理
	if (FAILED(pManager->Init(hInstance, hWnd, TRUE)))
	{// 初期化処理が失敗した場合
		return - 1;
	}

	// 分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;						// 初期化する
	dwExecLastTime = timeGetTime();			// 現在時刻を取得(保存)

	dwFrameCount = 0;						// 初期化する
	dwFPSLastTime = timeGetTime();			// 現在時刻を取得(保存)

	// ウインドウを表示
	ShowWindow(hWnd, nCmdShow);				// ウインドウの表示状態を設定
	UpdateWindow(hWnd);						// クライアント領域を更新

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{// windowsの処理
			if (msg.message == WM_QUIT)
			{// WM_QUITメッセージを受け取ったらループを抜ける
				break;
			}
			else
			{
				// メッセージの設定
				TranslateMessage(&msg);				// 仮想メッセージを文字メッセージへ変換
				DispatchMessage(&msg);				// ウインドウプロシージャへメッセージを送出
			}
		}
		else
		{// DirectXの処理
			dwCurrentTime = timeGetTime();		// 現在時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5秒経過
				// FPSを計測
				nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;	// FPSを計測した時間を保存
				dwFrameCount = 0;				// フレームカウントをクリア
				char alog[256];
				sprintf_s(alog, "%d\n", (nCountFPS));

				OutputDebugStringA(alog);
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / nFPS))
			{// 60分の1秒経過
				dwExecLastTime = dwCurrentTime;	// 処理開始の時刻[現在時刻]を保存

				// 更新処理
				pManager->Update();

				// 描画処理
				pManager->Draw();
				CDebugProc::Print("FPS : %d\n", nCountFPS);

				dwFrameCount++;					// フレームカウントを加算
			}
		}
	}

	if (pManager != NULL)
	{// マネージャを生成していたなら
		// 終了処理
		pManager->Uninit();
	}

	// ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//===============================================================================
// ウインドウプロシージャ
//===============================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 入力デバイス取得
	CManager* pManager = CManager::GetInstance();
	CInputMouse* pInputMouse = pManager->GetInputMouse();

	int nID;

	switch (uMsg)
	{
	case WM_DESTROY:
		// WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:				// キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:				// [ESC]が押された
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				// ウインドウを破棄する
				DestroyWindow(hWnd);
			}
			break;

		case VK_F11:
			ToggleFullscreen(hWnd);// F11でフルスクリーン
			break;
		}
		break;

	case WM_CLOSE:								// 閉じるボタン押下のメッセージ
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
		if (nID == IDYES)
		{
			// ウインドウを破棄する
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;							// 0を返さないと終了してしまう
		}
		break;

		// マウス移動
	case WM_MOUSEMOVE:
		pInputMouse->UpdateCuurentMousePos(lParam);
		pInputMouse->TransisionMousePos(hWnd);
		break;

		// 左クリック
	case WM_LBUTTONDOWN:
		SetCapture(hWnd);
		pInputMouse->AddMouseButtonFlags(BITMASK(CInputMouse::KEY_LEFT));
		break;

		// 左クリック解除
	case WM_LBUTTONUP:
		ReleaseCapture();
		pInputMouse->SubMouseButtonFlags(BITMASK(CInputMouse::KEY_LEFT));
		break;

		// 右クリック
	case WM_RBUTTONDOWN:
		SetCapture(hWnd);
		pInputMouse->AddMouseButtonFlags(BITMASK(CInputMouse::KEY_RIGHT));
		break;

		// 右クリック解除
	case WM_RBUTTONUP:
		ReleaseCapture();
		pInputMouse->SubMouseButtonFlags(BITMASK(CInputMouse::KEY_RIGHT));
		break;

		// 中央クリック
	case WM_MBUTTONDOWN:
		SetCapture(hWnd);
		pInputMouse->AddMouseButtonFlags(BITMASK(CInputMouse::KEY_WHEEL));
		break;

		// 中央クリック解除
	case WM_MBUTTONUP:
		ReleaseCapture();
		pInputMouse->SubMouseButtonFlags(BITMASK(CInputMouse::KEY_WHEEL));
		break;

		// マウスホイール回転
	case WM_MOUSEWHEEL:
		pInputMouse->UpdateMouseWheel(wParam);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//==================================================================================
//	角度の正規化
//==================================================================================
float AngleNormalize(float fAngle)
{
	if (fAngle < -D3DX_PI)
	{
		fAngle += D3DX_PI * 2;
	}
	else if (fAngle > D3DX_PI)
	{
		fAngle -= D3DX_PI * 2;
	}

	return fAngle;
}

//==================================================================================
//	度数法から弧度法への変換
//==================================================================================
float DegreeToRadian(float fDegree)
{
	return fDegree * D3DX_PI / 180;
}

//==================================================================================
//	弧度法から度数法への変換
//==================================================================================
int RadianToDegree(float fRadian)
{
	return fRadian * 180 / D3DX_PI;
}

//==================================================================================
// 数値の最小値と最大値の間を循環させる
//==================================================================================
float NumCirculation(float fNum, float fMin, float fMax)
{
	if (fNum < fMin)
	{// 数値が最小値より小さい
		return fMax + (fNum - fMin);
	}

	if (fNum > fMax)
	{// 数値が最大値より大きい
		return fMin + (fNum - fMax);
	}

	// どちらでもなければそのままを返す
	return fNum;
}

//================================================
// ウィンドウフルスクリーン処理
//================================================
void ToggleFullscreen(HWND hWnd)
{
	// フルスクリーン状態
	static bool isFullScreen = false;

	// 現在のウィンドウスタイルを取得
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	if (isFullScreen == true)
	{
		// ウィンドウモードに切り替え
		SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, g_windowRect.left, g_windowRect.top,
			g_windowRect.right - g_windowRect.left, g_windowRect.bottom - g_windowRect.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_NORMAL);
	}
	else
	{
		// フルスクリーンモードに切り替え
		GetWindowRect(hWnd, &g_windowRect);
		SetWindowLong(hWnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_MAXIMIZE);
	}

	isFullScreen = !isFullScreen;
}