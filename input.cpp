//=============================================================================
//
//	入力処理 [input.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "input.h"
#include <windowsx.h>

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECTINPUT8 CInput::m_pInput = NULL;		// デバイスへのポインタ

//=============================================================================
// コンストラクタ
//=============================================================================
CInput::CInput()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CInput::~CInput()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput != NULL)
	{// すでにオブジェクトが存在する
		return S_OK;
	}

	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CInput::Uninit(void)
{
	// 入力デバイスの破棄
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	// DirectInputオブジェクトの破棄
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//=============================================================================
// キーボードのコンストラクタ
//=============================================================================
CInputKeyboard::CInputKeyboard()
{
	// 各変数クリア
	memset(&m_aKeyState[0], NULL, sizeof(BYTE) * NUM_KEY_MAX);
	memset(&m_aKeyStateTrigger[0], NULL, sizeof(BYTE) * NUM_KEY_MAX);
	memset(&m_aKeyStateRelease[0], NULL, sizeof(BYTE) * NUM_KEY_MAX);
	memset(&m_aKeyStateRepeat[0], NULL, sizeof(BYTE) * NUM_KEY_MAX);
	memset(&m_aKeyCount[0], NULL, sizeof(BYTE) * NUM_KEY_MAX);
}


//=============================================================================
// キーボードのデストラクタ
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{

}

//=============================================================================
// キーボード初期化処理
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトの生成
	if (FAILED(CInput::Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// 入力デバイス(キーボード)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードの設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// デバイスへのアクセス権を獲得
	m_pDevice->Acquire();

	return S_OK;
}

//=============================================================================
// キーボードの終了処理
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
}

//=============================================================================
// キーボードの更新処理
//=============================================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];		// キーボードの入力情報

	// 入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];		// トリガー
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & m_aKeyState[nCntKey];	// リリース
			m_aKeyStateRepeat[nCntKey] = (m_aKeyState[nCntKey] & aKeyState[nCntKey]);							// リピート
			m_aKeyState[nCntKey] = aKeyState[nCntKey];															// キーボードのプレス情報を保存
		}
	}
	else
	{
		m_pDevice->Acquire();
	}
}

//=============================================================================
// キーボードのプレス情報を取得
//=============================================================================
bool CInputKeyboard::GetPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

//=============================================================================
//	キーボードのトリガー情報を取得
//=============================================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// キーボードのリリース情報を取得
//=============================================================================
bool CInputKeyboard::GetRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//=============================================================================
// キーボードのリピート情報を取得
//=============================================================================
bool CInputKeyboard::GetRepeat(int nKey)
{
	// フレームカウンターを宣言
	int static nFrameCounter[NUM_KEY_MAX] = {};

	if (m_aKeyStateTrigger[nKey] & 0x80)
	{// 最初はトリガー
		nFrameCounter[nKey] = 0;		// フレームカウンターをリセット
		return true;					// trueを返して終了
	}

	if (m_aKeyStateRepeat[nKey] & 0x80)
	{// リピートしてるなら入る
		nFrameCounter[nKey]++;		// フレームカウンターを増やす
		if (nFrameCounter[nKey] >= 30)
		{// カウントが一定以上なら
			if (nFrameCounter[nKey] % 5 == 0)
			{// 一定間隔ごとにtrueを返す
				return true;
			}
		}
	}

	return false;
}

//=============================================================================
// キーボードの情報を取得
//=============================================================================
bool CInputKeyboard::GetAny(void)
{
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		if (m_aKeyState[nCntKey] & 0x80)
		{
			return true;
		}
	}

	return false;
}

//=============================================================================
// ジョイパッドのコンストラクタ
//=============================================================================
CInputJoypad::CInputJoypad()
{
	// メンバ変数クリア
	memset(&m_State, 0, sizeof(State));
}

//=============================================================================
// ジョイパッドのデストラクタ
//=============================================================================
CInputJoypad::~CInputJoypad()
{

}

//=============================================================================
// ジョイパッドの初期化処理
//=============================================================================
HRESULT CInputJoypad::Init(void)
{
	// メモリのクリア
	memset(&m_State, 0, sizeof(State));

	// XInputのステートを設定(有効にする)
	XInputEnable(true);

	return S_OK;
}

//=============================================================================
//	ジョイパッドの終了処理
//=============================================================================
void CInputJoypad::Uninit(void)
{
	// Xinputのステートを設定(無効にする)
	XInputEnable(false);
}

//=============================================================================
//	ジョイパッドの更新処理
//=============================================================================
void CInputJoypad::Update(void)
{
	XINPUT_STATE state;		// ジョイパッドの入力情報

	State* pState = &m_State;

	// ジョイパッドの状態を取得
	XINPUT_STATE* pJoykeyState = &pState->State;
	XINPUT_STATE* pJoykeyStateTrigger = &pState->StateTrigger;
	XINPUT_STATE* pJoykeyStateRelease = &pState->StateRelease;
	XINPUT_STATE* pJoykeyStateRepeat = &pState->StateRepeat;
	XINPUT_VIBRATION* pJoypadVibration = &pState->Vibration;

	if (XInputGetState(0, &state) == ERROR_SUCCESS)
	{// ジョイパッドの情報が取得出来たら
		// トリガー判定
		if (state.Gamepad.bLeftTrigger >= ZLRTRIGGER)
		{
			state.Gamepad.wButtons += LEFTTRIGGER;
		}

		if (state.Gamepad.bRightTrigger >= ZLRTRIGGER)
		{
			state.Gamepad.wButtons += RIGHTTRIGGER;
		}

		pJoykeyStateTrigger->Gamepad.wButtons = (pJoykeyState->Gamepad.wButtons ^ state.Gamepad.wButtons) & state.Gamepad.wButtons;
		pJoykeyStateRelease->Gamepad.wButtons = (pJoykeyState->Gamepad.wButtons ^ state.Gamepad.wButtons) & pJoykeyState->Gamepad.wButtons;
		pJoykeyStateRepeat->Gamepad.wButtons = (pJoykeyState->Gamepad.wButtons & state.Gamepad.wButtons);
		*pJoykeyState = state;		// ジョイパッドのプレス情報を保存

		// スティックの情報

		// 左スティック
		if (state.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{// スティックが上に倒されていたら
			pState->isStickL[STICK_UP] = true;
		}
		else
		{
			pState->isStickL[STICK_UP] = false;
		}

		if (state.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{// スティックが下に倒されていたら
			pState->isStickL[STICK_DOWN] = true;
		}
		else
		{
			pState->isStickL[STICK_DOWN] = false;
		}

		if (state.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{// スティックが左に倒されていたら
			pState->isStickL[STICK_LEFT] = true;
		}
		else
		{
			pState->isStickL[STICK_LEFT] = false;
		}

		if (state.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{// スティックが右に倒されていたら
			pState->isStickL[STICK_RIGHT] = true;
		}
		else
		{
			pState->isStickL[STICK_RIGHT] = false;
		}

		// 右スティック
		if (state.Gamepad.sThumbRY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{// スティックが上に倒されていたら
			pState->isStickR[STICK_UP] = true;
		}
		else
		{
			pState->isStickR[STICK_UP] = false;
		}

		if (state.Gamepad.sThumbRY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{// スティックが下に倒されていたら
			pState->isStickR[STICK_DOWN] = true;
		}
		else
		{
			pState->isStickR[STICK_DOWN] = false;
		}

		if (state.Gamepad.sThumbRX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{// スティックが左に倒されていたら
			pState->isStickR[STICK_LEFT] = true;
		}
		else
		{
			pState->isStickR[STICK_LEFT] = false;
		}

		if (state.Gamepad.sThumbRX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{// スティックが右に倒されていたら
			pState->isStickR[STICK_RIGHT] = true;
		}
		else
		{
			pState->isStickR[STICK_RIGHT] = false;
		}
	}

	// 振動の更新処理
	if (pJoypadVibration->wLeftMotorSpeed != NULL || pJoypadVibration->wRightMotorSpeed != NULL)
	{// バイブレーションが設定されている
		pState->nVibCounter--;		// カウントを減らす
		if (pState->nVibCounter <= 0)
		{// カウントが回り切ったら
			// リセット
			pJoypadVibration->wLeftMotorSpeed = 0;
			pJoypadVibration->wRightMotorSpeed = 0;

			XInputSetState(0, pJoypadVibration);
		}
	}
}

//=============================================================================
//	ジョイパッドのプレス情報を取得
//=============================================================================
bool CInputJoypad::GetPress(KEY key, int nIdx)
{
	return (m_State.State.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=============================================================================
//	ジョイパッドのトリガー情報を取得
//=============================================================================
bool CInputJoypad::GetTrigger(KEY key, int nIdx)
{
	return (m_State.StateTrigger.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=============================================================================
//	ジョイパッドのリリース情報を取得
//=============================================================================
bool CInputJoypad::GetRelease(KEY key, int nIdx)
{
	return (m_State.StateRelease.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=============================================================================
//	ジョイパッドのリピート情報を取得
//=============================================================================
bool CInputJoypad::GetRepeat(KEY key, int nIdx)
{
	State* pState = &m_State;

	if (pState->StateTrigger.Gamepad.wButtons & (0x01 << key))
	{// 最初はトリガー
		pState->nRepeatKeyCounter[key] = 0;		// フレームカウンターをリセット
		return true;									// trueを返して終了
	}

	if (pState->StateRepeat.Gamepad.wButtons & (0x01 << key))
	{// リピートしてるなら入る
		pState->nRepeatKeyCounter[key]++;		// フレームカウンターを増やす
		if (pState->nRepeatKeyCounter[key] >= 30)
		{// カウントが一定以上なら
			if (pState->nRepeatKeyCounter[key] % 5 == 0)
			{// 一定間隔ごとにtrueを返す
				return true;
			}
		}
	}

	return false;
}

//=============================================================================
//	ジョイパッドの左トリガーのプレス情報を取得
//=============================================================================
bool CInputJoypad::GetLeftTriggePress(int nIdx)
{
	if (m_State.State.Gamepad.bLeftTrigger >= ZLRTRIGGER)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//=============================================================================
//	ジョイパッドの右トリガーのプレス情報を取得
//=============================================================================
bool CInputJoypad::GetRightTriggePress(int nIdx)
{
	if (m_State.State.Gamepad.bRightTrigger >= ZLRTRIGGER)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//=============================================================================
//	ジョイスティックのプレス情報を取得 (Lスティック)
//=============================================================================
bool CInputJoypad::GetStickPressL(STICK stick, int nIdx)
{
	return m_State.isStickL[stick];
}

//=============================================================================
//	ジョイスティックのプレス情報を取得 (Rスティック)
//=============================================================================
bool CInputJoypad::GetStickPressR(STICK stick, int nIdx)
{
	return m_State.isStickR[stick];
}

//=============================================================================
//	ジョイスティックのリピート情報を取得 (Lスティック)
//=============================================================================
bool CInputJoypad::GetStickRepeatL(STICK stick, int nIdx)
{
	State* pState = &m_State;

	if (pState->isStickL[stick] == true && pState->nRepeatStickCounterL[stick] == 0)
	{
		pState->nRepeatStickCounterL[stick] = 5;
		return true;
	}

	if (pState->isStickL[stick] == true)
	{
		pState->nRepeatStickCounterL[stick]++;
		if (pState->nRepeatStickCounterL[stick] >= 30)
		{
			if (pState->nRepeatStickCounterL[stick] % 5 == 0)
			{
				return true;
			}
		}

		return false;
	}

	if (pState->isStickL[stick] == false)
	{
		pState->nRepeatStickCounterL[stick] = 0;
		return false;
	}

	return false;

}

//=============================================================================
//	ジョイスティックのリピート情報を取得 (Rスティック)
//=============================================================================
bool CInputJoypad::GetStickRepeatR(STICK stick, int nIdx)
{
	State* pState = &m_State;

	if (pState->isStickR[stick] == true && pState->nRepeatStickCounterR[stick] == 0)
	{
		pState->nRepeatStickCounterR[stick] = 5;
		return true;
	}

	if (pState->isStickR[stick] == true)
	{
		pState->nRepeatStickCounterR[stick]++;
		if (pState->nRepeatStickCounterR[stick] >= 30)
		{
			if (pState->nRepeatStickCounterR[stick] % 5 == 0)
			{
				return true;
			}
		}

		return false;
	}

	if (pState->isStickR[stick] == false)
	{
		pState->nRepeatStickCounterR[stick] = 0;
		return false;
	}

	return false;

}

//=============================================================================
//	ジョイパッドの左スティック取得処理
//=============================================================================
bool CInputJoypad::GetStickLeft(float* pValueH, float* pValueV, int nIdx)
{
	XINPUT_STATE* pState = &m_State.State;

	float fValueH = 0, fValueV = 0;

	fValueH = pState->Gamepad.sThumbLX;
	fValueV = pState->Gamepad.sThumbLY;

	if (SQRTF(fValueH, fValueV) * 0.5f > CUSTOM_DEADZONE)
	{// デッドゾーン外なら
		// 正規化
		fValueH = (fValueH) / JOYSTICKVALUE_MAX;
		fValueV = (fValueV) / JOYSTICKVALUE_MAX;

		// 値を渡す
		*pValueH = fValueH;
		*pValueV = fValueV;
		return true;
	}
	else
	{
		return false;
	}
}

//=============================================================================
//	ジョイパッドの右スティック取得処理
//=============================================================================
bool CInputJoypad::GetStickRight(float* pValueH, float* pValueV, int nIdx)
{
	XINPUT_STATE* pState = &m_State.State;

	float fValueH, fValueV;

	fValueH = pState->Gamepad.sThumbRX;
	fValueV = pState->Gamepad.sThumbRY;

	if (SQRTF(fValueH, fValueV) * 0.5f > CUSTOM_DEADZONE)
	{// デッドゾーン外なら
		// 正規化
		fValueH = (fValueH) / JOYSTICKVALUE_MAX;
		fValueV = (fValueV) / JOYSTICKVALUE_MAX;

		// 値を渡す
		*pValueH = fValueH;
		*pValueV = fValueV;
		return true;
	}
	else
	{
		return false;
	}
}

//=============================================================================
//	ジョイパッドの情報を取得
//=============================================================================
bool CInputJoypad::GetAny(int nIdx)
{
	for (int nCntKey = 0; nCntKey < KEY_MAX; nCntKey++)
	{
		if (m_State.StateTrigger.Gamepad.wButtons & (0x01 << nCntKey))
		{
			return true;
		}
	}

	for (int nCntStick = 0; nCntStick < STICK_MAX; nCntStick++)
	{
		if (m_State.isStickL[nCntStick] == true)
		{
			return true;
		}

		if (m_State.isStickR[nCntStick] == true)
		{
			return true;
		}
	}

	return false;
}

//=============================================================================
//	ジョイパッドの振動設定
//=============================================================================
void CInputJoypad::SetVibration(int nLVibration, int nRVibration, int nVibCounter, int nIdx)
{
	m_State.Vibration.wLeftMotorSpeed = nLVibration;
	m_State.Vibration.wRightMotorSpeed = nRVibration;
	m_State.nVibCounter = nVibCounter;

	XInputSetState(nIdx, &m_State.Vibration);
}

//=============================================================================
//	ジョイパッドの振動取得
//=============================================================================
bool CInputJoypad::GetVibration(int nIdx)
{
	if (m_State.Vibration.wLeftMotorSpeed != NULL || m_State.Vibration.wRightMotorSpeed != NULL)
	{
		return true;
	}

	return false;
}

//=============================================================================
//	ジョイパッドの情報取得
//=============================================================================
CInputJoypad::State* CInputJoypad::GetState(int nIdx)
{
	return &m_State;
}

//=============================================================================
// マウスのコンストラクタ
//=============================================================================
CInputMouse::CInputMouse()
{
	// 各変数クリア
	memset(&m_CurrentMouseState, 0, sizeof(LPDIMOUSESTATE2));
	memset(&m_PrevMouseState, 0, sizeof(LPDIMOUSESTATE2));
	memset(&m_aMouseState, 0, sizeof(LPDIMOUSESTATE2));
	memset(&m_aMouseStateTrigger, 0, sizeof(LPDIMOUSESTATE2));
	memset(&m_aMouseStateRelease, 0, sizeof(LPDIMOUSESTATE2));

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_InputhWnd = NULL;

	memset(&m_MyMouseState, 0, sizeof(MouseState));
}

//=============================================================================
// マウスのデストラクタ
//=============================================================================
CInputMouse::~CInputMouse()
{

}

//=============================================================================
// マウスの初期化処理
//=============================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 使用するクライアント領域のウインドウを設定
	m_InputhWnd = hWnd;

	// DirectInputオブジェクトの生成
	if (FAILED(CInput::Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// 入力デバイス(マウス)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// マウスへのアクセス権を獲得
	 m_pDevice->Acquire();

	return S_OK;
}

//=============================================================================
// マウスの終了処理
//=============================================================================
void CInputMouse::Uninit(void)
{
	CInput::Uninit();
}

//=============================================================================
// マウスの更新処理
//=============================================================================
void CInputMouse::Update(void)
{
	// 更新前に過去の入力情報(マウス)を保存
	m_PrevMouseState = m_CurrentMouseState;

	// 現在のマウスの入力情報を取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_CurrentMouseState)))
	{
		for (int nCntMouse = 0; nCntMouse < NUM_MOUSE_MAX; nCntMouse++)
		{
			m_aMouseStateTrigger.rgbButtons[nCntMouse] = ((m_aMouseState.rgbButtons[nCntMouse] ^ m_CurrentMouseState.rgbButtons[nCntMouse]) & m_CurrentMouseState.rgbButtons[nCntMouse]);
			m_aMouseStateRelease.rgbButtons[nCntMouse] = (m_PrevMouseState.rgbButtons[nCntMouse] & (m_PrevMouseState.rgbButtons[nCntMouse] ^ m_CurrentMouseState.rgbButtons[nCntMouse]));
			m_aMouseState.rgbButtons[nCntMouse] = m_CurrentMouseState.rgbButtons[nCntMouse];
		}
	}
	else
	{
		m_pDevice->Acquire();			// マウスへのアクセス権を取得
	}
}

//=============================================================================
// マウスのプレス情報を取得
//=============================================================================
bool CInputMouse::GetPress(int nButton)
{
	return (m_aMouseState.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
// マウスのトリガー情報を取得
//=============================================================================
bool CInputMouse::GetTrigger(int nButton)
{
	return (m_aMouseStateTrigger.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
// マウスのリリース情報を取得
//=============================================================================
bool CInputMouse::GetRelease(int nButton)
{
	return (m_aMouseStateRelease.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
// マウスの位置情報を取得
//=============================================================================
D3DXVECTOR3 CInputMouse::GetPosition(void)
{
	// スクリーン上のマウスの位置
	POINT mousePos;

	// マウスの位置を取得
	if (GetCursorPos(&mousePos) != TRUE)
	{// 取得失敗
		m_pDevice->Acquire();			// マウスへのアクセス権を取得
	}
	else
	{// 取得成功
		// 取得した座標をウィンドウ内の座標に変換
		ScreenToClient(m_InputhWnd, &mousePos);
		m_pos = D3DXVECTOR3(mousePos.x, mousePos.y, 0.0f);			// 現在の位置を保存
	}

	return m_pos;
}

//================================================================================================================
// マウスの位置情報を取得
//================================================================================================================
void CInputMouse::UpdateMouseState(void)
{
	m_MyMouseState.Move.x = m_MyMouseState.CurrentPos.x - m_MyMouseState.PrevPos.x;
	m_MyMouseState.Move.y = m_MyMouseState.CurrentPos.y - m_MyMouseState.PrevPos.y;

	m_MyMouseState.PrevPos = m_MyMouseState.CurrentPos;
	m_MyMouseState.nOutWheel = m_MyMouseState.nInWheel;

	m_MyMouseState.nInWheel = 0;
}

//================================================================================================================
// マウスの位置情報を取得
//================================================================================================================
void CInputMouse::UpdateCuurentMousePos(LPARAM lParam)
{
	m_MyMouseState.CurrentPos.x = GET_X_LPARAM(lParam);
	m_MyMouseState.CurrentPos.y = GET_Y_LPARAM(lParam);
}

//================================================================================================================
// マウスの位置制限
//================================================================================================================
void CInputMouse::TransisionMousePos(HWND hWnd)
{
	if (!m_MyMouseState.ButtonFlags & (BITMASK(KEY_LEFT) | BITMASK(KEY_RIGHT) | BITMASK(KEY_WHEEL)))
	{// 操作中じゃなければ
		return;
	}

	POINT MousePos = {};				// スクリーン上の位置
	RECT rect = {};						// ウィンドウのサイズ
	bool isTransisiton = false;			// 端についてたか

	// ウィンドウのサイズを受け取る
	rect.right = GetSystemMetrics(SM_CXSCREEN);
	rect.bottom = GetSystemMetrics(SM_CYSCREEN);

	GetCursorPos(&MousePos);			// 今の位置を受け取る

	if (MousePos.x <= 0)
	{// もし左端を超えていたら
		MousePos.x = rect.right - 2;
		isTransisiton = true;
	}

	if (MousePos.x > rect.right - 2)
	{// もし右端を超えていたら
		MousePos.x = 1;
		isTransisiton = true;
	}

	if (MousePos.y <= 0)
	{// もし上端を超えていたら
		MousePos.y = rect.bottom - 2;
		isTransisiton = true;
	}

	if (MousePos.y > rect.bottom - 2)
	{// もし下端を超えていたら
		MousePos.y = 1;
		isTransisiton = true;
	}

	if (isTransisiton == true)
	{// もし移動していたら
		SetCursorPos(MousePos.x, MousePos.y);	// 移動後の位置にマウスカーソルを移動

		ScreenToClient(hWnd, &MousePos);		// クライアント領域の座標に変換

		// 変換した座標をマウス情報に設定
		m_MyMouseState.CurrentPos = MousePos;
		m_MyMouseState.PrevPos = MousePos;
	}
}

//================================================================================================================
// マウスの位置の移動量を取得
//================================================================================================================
POINT CInputMouse::GetMouseMove(void)
{
	return m_MyMouseState.Move;
}

//================================================================================================================
// マウスボタンクリックのフラグを取得する
//================================================================================================================
bool CInputMouse::GetMouseButtonFlags(KEY key)
{
	return m_MyMouseState.ButtonFlags & BITMASK(key) ? true : false;
}

//================================================================================================================
// マウスボタンクリックのフラグを立てる
//================================================================================================================
void CInputMouse::AddMouseButtonFlags(UINT flags)
{
	m_MyMouseState.ButtonFlags |= flags;
}

//================================================================================================================
// マウスボタンクリックのフラグを折る
//================================================================================================================
void CInputMouse::SubMouseButtonFlags(UINT flags)
{
	m_MyMouseState.ButtonFlags &= ~flags;
}

//================================================================================================================
// マウスホイール回転情報を取得
//================================================================================================================
void CInputMouse::UpdateMouseWheel(WPARAM wParam)
{
	m_MyMouseState.nInWheel += GET_WHEEL_DELTA_WPARAM(wParam);
}

//================================================================================================================
// マウスホイール回転量を取得
//================================================================================================================
int CInputMouse::GetMouseWheelMove(void)
{
	return m_MyMouseState.nOutWheel;
}