//=============================================================================
//
//	入力処理 [input.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_KEY_MAX			(256)				// キーの最大数
#define NUM_MOUSE_MAX		(3)					// ボタンの最大数
#define MAX_JOYPAD			(4)					// ジョイパッドの最大数
#define JOYSTICKVALUE_MAX	(32767)				// スティックの値の最大
#define JOYSTICKVALUE_MIN	(-32768)			// スティックの値の最小
#define CUSTOM_DEADZONE		(7849)				// デッドゾーン
#define ZLRTRIGGER			(255)				// ZLZRトリガーの値
#define LEFTTRIGGER			((WORD)0x0400)		// ZLトリガーのビット
#define RIGHTTRIGGER		((WORD)0x0800)		// ZRトリガーのビット

//*****************************************************************************
// ジョイパッドクラス
//*****************************************************************************
class CInputJoypad
{
public:
	//*****************************************************************************
	// キーの種類
	//*****************************************************************************
	typedef enum KEY
	{
		KEY_UP = 0,				// 十字キー上
		KEY_DOWN,				// 十字キー下
		KEY_LEFT,				// 十字キー左
		KEY_RIGHT,				// 十字キー右
		KEY_START,				// スタートボタン
		KEY_BACK,				// バックボタン
		KEY_LEFT_THUMB,			// 左スティック押し込み
		KEY_RIGHT_THUMB,			// 右スティック押し込み
		KEY_LEFT_SHOULDER,		// Lボタン
		KEY_RIGHT_SHOULDER,		// Rボタン
		KEY_LEFT_TRIGGER,		// ZLトリガー
		KEY_RIGHT_TRIGGER,		// ZRトリガー
		KEY_A,					// Aボタン
		KEY_B,					// Bボタン
		KEY_X,					// Xボタン
		KEY_Y,					// Yボタン
		KEY_MAX					// 総数
	}KEY;

	//*****************************************************************************
	// スティックの種類
	//*****************************************************************************
	typedef enum STICK
	{
		STICK_UP = 0,			// 上
		STICK_DOWN,				// 下
		STICK_LEFT,				// 左
		STICK_RIGHT,			// 右
		STICK_MAX				// 総数
	}STICK;

	//*****************************************************************************
	// ジョイパッド情報構造体定義
	//*****************************************************************************
	typedef struct State
	{
		XINPUT_STATE State;							// プレス情報
		XINPUT_STATE StateTrigger;					// トリガー情報
		XINPUT_STATE StateRelease;					// リリース情報
		XINPUT_STATE StateRepeat;					// リピート情報
		XINPUT_VIBRATION Vibration;					// 振動情報
		int nVibCounter;							// 振動カウンター
		bool isStickL[STICK_MAX];					// スティックの情報
		bool isStickR[STICK_MAX];					// スティックの情報
		int nRepeatKeyCounter[KEY_MAX];				// リピートカウント (ボタン)
		int nRepeatStickCounterL[STICK_MAX];		// リピートカウント (Lスティック)
		int nRepeatStickCounterR[STICK_MAX];		// リピートカウント (Rスティック)
	}State;

	CInputJoypad();
	~CInputJoypad();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	bool GetPress(KEY key, int nIdx = 0);
	bool GetTrigger(KEY key, int nIdx = 0);
	bool GetRelease(KEY key, int nIdx = 0);
	bool GetRepeat(KEY key, int nIdx = 0);
	bool GetAny(int nIdx = 0);
	bool GetLeftTriggePress(int nIdx = 0);
	bool GetRightTriggePress(int nIdx = 0);
	bool GetStickPressL(STICK stick, int nIdx = 0);
	bool GetStickPressR(STICK stick, int nIdx = 0);
	bool GetStickRepeatL(STICK stick, int nIdx = 0);
	bool GetStickRepeatR(STICK stick, int nIdx = 0);
	bool GetStickLeft(float* pValueH, float* pValueV, int nIdx = 0);
	bool GetStickRight(float* pValueH, float* pValueV, int nIdx = 0);
	bool GetVibration(int nIdx = 0);
	void SetVibration(int nLVibration, int nRVibration, int nVibCounter, int nIdx = 0);
	State* GetState(int nIdx = 0);

private:

	State m_State;			// ジョイパッドの情報
};

//*****************************************************************************
// 入力クラス
//*****************************************************************************
class CInput
{
public:
	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:
	static LPDIRECTINPUT8 m_pInput;		// DirectInputオブジェクト
	LPDIRECTINPUTDEVICE8 m_pDevice;		// 入力デバイス
};

//*****************************************************************************
// キーボードクラス
//*****************************************************************************
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	bool GetRepeat(int nKey);
	bool GetAny(void);

private:
	BYTE m_aKeyState[NUM_KEY_MAX];			// 入力情報
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];	// トリガー情報
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];	// リリース情報
	BYTE m_aKeyStateRepeat[NUM_KEY_MAX];	// リピート情報
	BYTE m_aKeyCount[NUM_KEY_MAX];			// カウント
};

//*****************************************************************************
// マウスクラス
//*****************************************************************************
class CInputMouse : public CInput
{
public:
	//**********************************************************************************
	// ボタンの種類
	//**********************************************************************************
	typedef enum KEY
	{
		KEY_LEFT = 0,		// 左クリック
		KEY_RIGHT,			// 右クリック
		KEY_WHEEL,			// 中クリック
		KEY_MAX
	}KEY;

	//*****************************************************************************
	// マウス情報構造体定義
	//*****************************************************************************
	typedef struct MouseState
	{
		POINT CurrentPos;			// 今のマウスカーソルの位置
		POINT PrevPos;				// 過去のマウスカーソルの位置
		POINT Move;					// マウスカーソルの移動量
		UINT  ButtonFlags;			// なんのボタンが押されているか
		UINT  PrevButtonFlags;		// 過去のフラグ
		int	  nInWheel;				// ホイールの回転量(保存)
		int   nOutWheel;			// ホイールの回転量(出力)
	}State;

	CInputMouse();
	~CInputMouse();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetPress(int nButton);
	bool GetTrigger(int nButton);
	bool GetRelease(int nButton);
	bool GetRepeat(int nButton);
	bool GetAny(void);
	D3DXVECTOR3 GetPosition(void);

	void UpdateMouseState(void);
	void UpdateCuurentMousePos(LPARAM lParam);
	void TransisionMousePos(HWND hWnd);
	POINT GetMouseMove(void);
	bool GetMouseButtonFlags(KEY key);
	void AddMouseButtonFlags(UINT flags);
	void SubMouseButtonFlags(UINT flags);
	void UpdateMouseWheel(WPARAM wParam);
	int GetMouseWheelMove(void);

private:
	DIMOUSESTATE2 m_CurrentMouseState;				// マウスの入力情報
	DIMOUSESTATE2 m_PrevMouseState;					// マウスの過去の入力情報
	DIMOUSESTATE2 m_aMouseState;					// マウスのプレス情報
	DIMOUSESTATE2 m_aMouseStateTrigger;				// マウスのトリガー情報
	DIMOUSESTATE2 m_aMouseStateRelease;				// マウスのリリース情報

	D3DXVECTOR3 m_pos;								// マウスの位置
	HWND m_InputhWnd;								// 座標変換に使用するウィンドウのハンドル

	MouseState m_MyMouseState;						// マウスの入力情報(自作)
};

#endif
