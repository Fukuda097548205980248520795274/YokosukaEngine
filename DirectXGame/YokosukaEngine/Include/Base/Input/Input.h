#pragma once
#define DIRECTINPUT_0x0800 // DirectInputのバージョン指定
#include <dinput.h>
#include <dxgi1_6.h>
#include "../WinApp/WinApp.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class Input
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* winApp);

	/// <summary>
	/// 全てのキーの入力情報を取得する
	/// </summary>
	void CheckAllKeyInfo();

	/// <summary>
	/// 全てのキーの入力情報をコピーする
	/// </summary>
	void CopyAllKeyInfo();

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


private:

	// ウィンドウアプリケーション
	WinApp* winApp_ = nullptr;

	// DirectInput
	IDirectInput8* directInput = nullptr;

	// キーボードデバイス
	IDirectInputDevice8* keyboard = nullptr;

	// キー
	BYTE keys[256];

	// 前のキー
	BYTE preKeys[256];
};

