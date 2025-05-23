#pragma once
#define DIRECTINPUT_0x0800 // DirectInputのバージョン指定
#include <dinput.h>
#include <dxgi1_6.h>
#include "../WinApp/WinApp.h"
#include "../../Math/Vector3/Vector3.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class Input
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Input();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* winApp);

	/// <summary>
	/// 入力情報を取得する
	/// </summary>
	void CheckInputInfo();

	/// <summary>
	/// 全ての入力情報をコピーする
	/// </summary>
	void CopyInputInfo();

	/// <summary>
	/// キーの入力情報（Press）
	/// </summary>
	/// <param name="key"></param>
	bool GetKeyPress(BYTE key);

	/// <summary>
	/// キーの入力情報（Trigger）
	/// </summary>
	/// <param name="key">キー</param>
	/// <returns></returns>
	bool GetKeyTrigger(BYTE key);

	/// <summary>
	/// キーの入力情報（Release）
	/// </summary>
	/// <param name="key">キー</param>
	/// <returns></returns>
	bool GetKeyRelease(BYTE key);

	/// <summary>
	/// マウスボタンの入力情報（Press）
	/// </summary>
	/// <returns></returns>
	bool GetMousePress(uint32_t mouseButtonNumber);

	/// <summary>
	/// マウスボタンの入力情報（Trigger）
	/// </summary>
	/// <returns></returns>
	bool GetMouseTrigger(uint32_t mouseButtonNumber);

	/// <summary>
	/// マウスボタンの入力情報（Release）
	/// </summary>
	/// <returns></returns>
	bool GetMouseRelease(uint32_t mouseButtonNumber);

	/// <summary>
	/// マウスの移動量のGetter
	/// </summary>
	/// <returns></returns>
	Vector3 GetMouseVelocity() { return mouseVelocity_; }


private:

	// ウィンドウアプリケーション
	WinApp* winApp_ = nullptr;

	// DirectInput
	IDirectInput8* directInput_ = nullptr;

	// キーボードデバイス
	IDirectInputDevice8* keyboard_ = nullptr;

	// マウスデバイス
	LPDIRECTINPUTDEVICE8 mouse_ = nullptr;

	// キー
	BYTE keys_[256];

	// 前のキー
	BYTE preKeys_[256];

	// マウスの入力情報
	DIMOUSESTATE mouseState_;

	// マウスの移動量
	Vector3 mouseVelocity_ = { 0.0f , 0.0f , 0.0f };

	// マウスボタンが押されたかどうか
	bool isPushButton_[3] = { false };

	// 前回のマウスボタンが押されたかどうか
	bool prevIsPushButton_[3] = { false };
};

