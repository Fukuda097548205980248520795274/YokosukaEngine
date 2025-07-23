#include "Input.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "xinput.lib")

/// <summary>
/// デストラクタ
/// </summary>
Input::~Input()
{
	// マウス
	mouse_->Release();

	// キーボード
	keyboard_->Release();

	// DirectInput
	directInput_->Release();
}

/// <summary>
/// 初期化
/// </summary>
void Input::Initialize(WinApp* winApp)
{
	// nullptrチェック
	assert(winApp);

	// 引数を受け取る
	winApp_ = winApp;

	// DirectInputを初期化する
	HRESULT hr = DirectInput8Create(winApp_->GetWndClass().hInstance,
		DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput_, nullptr);
	assert(SUCCEEDED(hr));


	/*--------------
		キーボード
	--------------*/

	// キーボードデバイスの生成
	hr = directInput_->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	assert(SUCCEEDED(hr));

	// 入力データの形式セット
	hr = keyboard_->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(hr));

	// 排他制御レベルのセット
	hr = keyboard_->SetCooperativeLevel(winApp_->GetHwnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));


	/*-----------
		マウス
	-----------*/

	// マウスデバイスの生成
	hr = directInput_->CreateDevice(GUID_SysMouse, &mouse_, NULL);
	assert(SUCCEEDED(hr));

	// 入力データの形式セット
	hr = mouse_->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(hr));

	// 排他制御レベルのセット
	hr = mouse_->SetCooperativeLevel(winApp->GetHwnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(hr));
}

/// <summary>
/// 全てのキーの入力情報を取得する
/// </summary>
void Input::CheckInputInfo()
{
	/*--------------
		キーボード
	--------------*/

	// キーボード情報の取得開始
	keyboard_->Acquire();

	// キーの入力情報を取得する
	keyboard_->GetDeviceState(sizeof(keys_), keys_);


	/*-----------
		マウス
	-----------*/

	// マウス情報の取得開始
	mouse_->Acquire();

	// マウスの入力情報を取得する
	HRESULT hr = mouse_->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState_);

	// マウスの入力情報を取得できたら、移動量を求める
	mouseVelocity_.x = static_cast<float>(mouseState_.lX);
	mouseVelocity_.y = static_cast<float>(mouseState_.lY);
	mouseVelocity_.z = static_cast<float>(mouseState_.lZ);

	// マウスのボタンが押されたかどうか
	for (uint32_t i = 0; i < 3; i++)
	{
		isPushButton_[i] = mouseState_.rgbButtons[i];
	}


	/*-----------------
		ゲームパッド
	-----------------*/

	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		// 構造体をゼロクリア
		ZeroMemory(&gamepadState_[i], sizeof(XINPUT_STATE));

		// ゲームパッドの入力情報を取得する
		dwResult_[i] = XInputGetState(i, &gamepadState_[i]);
	}
}

/// <summary>
/// 全ての入力情報をコピーする
/// </summary>
void Input::CopyInputInfo()
{
	memcpy(preKeys_, keys_, 256 * sizeof(BYTE));
	memcpy(prevIsPushButton_, isPushButton_, 3 * sizeof(bool));
	memcpy(prevGamepadState_, gamepadState_, sizeof(XINPUT_STATE) * XUSER_MAX_COUNT);
}

/// <summary>
/// キーの入力情報（Press）
/// </summary>
/// <param name="key"></param>
bool Input::GetKeyPress(KeyBord key)
{
	if (keys_[key])
	{
		return true;
	}

	return false;
}

/// <summary>
/// キーの入力情報（Trigger）
/// </summary>
/// <param name="key">キー</param>
/// <returns></returns>
bool Input::GetKeyTrigger(KeyBord key)
{
	if (!preKeys_[key] && keys_[key])
	{
		return true;
	}

	return false;
}

/// <summary>
/// キーの入力情報（Release）
/// </summary>
/// <param name="key">キー</param>
/// <returns></returns>
bool Input::GetKeyRelease(KeyBord key)
{
	if (preKeys_[key] && !keys_[key])
	{
		return true;
	}

	return false;
}


/// <summary>
/// マウスボタンの入力情報（Press）
/// </summary>
/// <returns></returns>
bool Input::GetMousePress(uint32_t mouseButtonNumber)
{
	if (isPushButton_[mouseButtonNumber])
	{
		return true;
	}

	return false;
}

/// <summary>
/// マウスボタンの入力情報（Trigger）
/// </summary>
/// <returns></returns>
bool Input::GetMouseTrigger(uint32_t mouseButtonNumber)
{
	if (!prevIsPushButton_[mouseButtonNumber] && isPushButton_[mouseButtonNumber])
	{
		return true;
	}

	return false;
}

/// <summary>
/// マウスボタンの入力情報（Release）
/// </summary>
/// <returns></returns>
bool Input::GetMouseRelease(uint32_t mouseButtonNumber)
{
	if (prevIsPushButton_[mouseButtonNumber] && !isPushButton_[mouseButtonNumber])
	{
		return true;
	}

	return false;
}


/// <summary>
/// マウスホイールが上回転したかどうか
/// </summary>
/// <returns></returns>
bool Input::GetMouseWheelUp()
{
	if (static_cast<float>(mouseState_.lZ) > 0.0f)
	{
		return true;
	}

	return false;
}

/// <summary>
/// マウスホイールが下回転したかどうか
/// </summary>
/// <returns></returns>
bool Input::GetMouseWheelDown()
{
	if (static_cast<float>(mouseState_.lZ) < 0.0f)
	{
		return true;
	}

	return false;
}


/// <summary>
/// ゲームパッドが有効化を判断する
/// </summary>
/// <param name="gamepadNumber"></param>
/// <returns></returns>
bool Input::IsGamepadEnable(DWORD gamepadNumber)
{
	if (dwResult_[gamepadNumber] == ERROR_SUCCESS)
	{
		return true;
	}

	return false;
}

/// <summary>
/// 
/// </summary>
/// <param name="controllerNumber"></param>
/// <param name="wButtons"></param>
/// <returns></returns>
bool Input::GetGamepadButtonPress(DWORD gamepadNumber, Gamepad wButtons)
{
	if (dwResult_[gamepadNumber] == ERROR_SUCCESS)
	{
		if (gamepadState_[gamepadNumber].Gamepad.wButtons & wButtons)
		{
			return true;
		}
	}

	return false;
}

/// <summary>
/// 
/// </summary>
/// <param name="controllerNumber"></param>
/// <param name="wButtons"></param>
/// <returns></returns>
bool Input::GetGamepadButtonTrigger(DWORD gamepadNumber, Gamepad wButtons)
{
	if (dwResult_[gamepadNumber] == ERROR_SUCCESS)
	{
		if (!(prevGamepadState_[gamepadNumber].Gamepad.wButtons & wButtons) &&
			gamepadState_[gamepadNumber].Gamepad.wButtons & wButtons)
		{
			return true;
		}
	}

	return false;
}

/// <summary>
/// 
/// </summary>
/// <param name="controllerNumber"></param>
/// <param name="wButtons"></param>
/// <returns></returns>
bool Input::GetGamepadButtonRelease(DWORD gamepadNumber, Gamepad wButtons)
{
	if (dwResult_[gamepadNumber] == ERROR_SUCCESS)
	{
		if (prevGamepadState_[gamepadNumber].Gamepad.wButtons & wButtons &&
			!(gamepadState_[gamepadNumber].Gamepad.wButtons & wButtons))
		{
			return true;
		}
	}

	return false;
}

/// <summary>
/// ゲームパッドのスティックの入力情報
/// </summary>
/// <param name="gamepadNumber"></param>
/// <returns></returns>
Vector2 Input::GetGamepadLeftStick(DWORD gamepadNumber)
{
	Vector2 thumbL = { 0.0f , 0.0f };

	if (dwResult_[gamepadNumber] == ERROR_SUCCESS)
	{
		float length = std::sqrtf(static_cast<float>(std::pow(gamepadState_[gamepadNumber].Gamepad.sThumbLX, 2)) +
			static_cast<float>(std::pow(gamepadState_[gamepadNumber].Gamepad.sThumbLY, 2)));

		if (length > static_cast<float>(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
		{
			if (gamepadState_[gamepadNumber].Gamepad.sThumbLX > 0)
			{
				thumbL.x = static_cast<float>(gamepadState_[gamepadNumber].Gamepad.sThumbLX) / SHRT_MAX;
			} else
			{
				thumbL.x = static_cast<float>(gamepadState_[gamepadNumber].Gamepad.sThumbLX) / -SHRT_MIN;
			}

			if (gamepadState_[gamepadNumber].Gamepad.sThumbLY > 0)
			{
				thumbL.y = static_cast<float>(gamepadState_[gamepadNumber].Gamepad.sThumbLY) / SHRT_MAX;
			} else
			{
				thumbL.y = static_cast<float>(gamepadState_[gamepadNumber].Gamepad.sThumbLY) / -SHRT_MIN;
			}
		}
	}

	return thumbL;
}

/// <summary>
/// ゲームパッドの右スティックの入力情報
/// </summary>
/// <param name="gamepadNumber"></param>
/// <returns></returns>
Vector2 Input::GetGamepadRightStick(DWORD gamepadNumber)
{
	Vector2 thumbR = { 0.0f , 0.0f };

	if (dwResult_[gamepadNumber] == ERROR_SUCCESS)
	{
		float length = std::sqrtf(static_cast<float>(std::pow(gamepadState_[gamepadNumber].Gamepad.sThumbRX, 2)) +
			static_cast<float>(std::pow(gamepadState_[gamepadNumber].Gamepad.sThumbRY, 2)));

		if (length > static_cast<float>(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
		{
			if (gamepadState_[gamepadNumber].Gamepad.sThumbRX > 0)
			{
				thumbR.x = static_cast<float>(gamepadState_[gamepadNumber].Gamepad.sThumbRX) / SHRT_MAX;
			} else
			{
				thumbR.x = static_cast<float>(gamepadState_[gamepadNumber].Gamepad.sThumbRX) / -SHRT_MIN;
			}

			if (gamepadState_[gamepadNumber].Gamepad.sThumbRY > 0)
			{
				thumbR.y = static_cast<float>(gamepadState_[gamepadNumber].Gamepad.sThumbRY) / SHRT_MAX;
			} else
			{
				thumbR.y = static_cast<float>(gamepadState_[gamepadNumber].Gamepad.sThumbRY) / -SHRT_MIN;
			}
		}
	}

	return thumbR;
}

/// <summary>
/// ゲームパッドの左トリガーの入力情報
/// </summary>
/// <param name="gamepadNumber"></param>
/// <returns></returns>
float Input::GetGamepadLeftTrigger(DWORD gamepadNumber)
{
	float triggerL = 0.0f;

	if (dwResult_[gamepadNumber] == ERROR_SUCCESS)
	{
		triggerL = static_cast<float>(gamepadState_[gamepadNumber].Gamepad.bLeftTrigger) / 255.0f;
	}

	return triggerL;
}

/// <summary>
/// ゲームパッドの右トリガーの入力情報
/// </summary>
/// <param name="gamepadNumber"></param>
/// <returns></returns>
float Input::GetGamepadRightTrigger(DWORD gamepadNumber)
{
	float triggerR = 0.0f;

	if (dwResult_[gamepadNumber] == ERROR_SUCCESS)
	{
		triggerR = static_cast<float>(gamepadState_[gamepadNumber].Gamepad.bRightTrigger) / 255.0f;
	}

	return triggerR;
}