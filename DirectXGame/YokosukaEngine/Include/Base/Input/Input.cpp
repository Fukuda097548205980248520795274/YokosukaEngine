#include "Input.h"

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
}

/// <summary>
/// 全てのキーの入力情報をコピーする
/// </summary>
void Input::CopyInputInfo()
{
	memcpy(preKeys_, keys_ , 256 * sizeof(BYTE));
	memcpy(prevIsPushButton_, isPushButton_, 3 * sizeof(bool));
}

/// <summary>
/// キーの入力情報（Press）
/// </summary>
/// <param name="key"></param>
bool Input::GetKeyPress(BYTE key)
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
bool Input::GetKeyTrigger(BYTE key)
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
bool Input::GetKeyRelease(BYTE key)
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