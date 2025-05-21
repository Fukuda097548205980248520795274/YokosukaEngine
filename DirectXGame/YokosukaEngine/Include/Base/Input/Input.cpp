#include "Input.h"

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
		DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(hr));

	// キーボードデバイスの生成
	hr = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(hr));

	// 入力データの形式セット
	hr = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(hr));

	// 排他制御レベルのセット
	hr = keyboard->SetCooperativeLevel(winApp_->GetHwnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));
}

/// <summary>
/// 全てのキーの入力情報を取得する
/// </summary>
void Input::CheckAllKeyInfo()
{
	// キーボード情報の取得開始
	keyboard->Acquire();

	// キーの入力情報を取得する
	keyboard->GetDeviceState(sizeof(keys), keys);
}

/// <summary>
/// 全てのキーの入力情報をコピーする
/// </summary>
void Input::CopyAllKeyInfo()
{
	memcpy(preKeys, keys , 256);
}

/// <summary>
/// キーの入力情報（Press）
/// </summary>
/// <param name="key"></param>
bool Input::GetKeyPress(BYTE key)
{
	if (keys[key])
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
	if (!preKeys[key] && keys[key])
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
	if (preKeys[key] && !keys[key])
	{
		return true;
	}

	return false;
}