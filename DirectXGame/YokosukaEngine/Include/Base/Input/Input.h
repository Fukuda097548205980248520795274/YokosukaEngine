#pragma once
#define DIRECTINPUT_0x0800 // DirectInputのバージョン指定
#define MAX_CONTROLLERS4
#include <dinput.h>
#include <dxgi1_6.h>
#include <Xinput.h>
#include "../WinApp/WinApp.h"
#include "../../Math/Vector3/Vector3.h"
#include "../../Math/Vector2/Vector2.h"

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

	/// <summary>
	/// ゲームパッドが有効化を判断する
	/// </summary>
	/// <param name="gamepadNumber">ゲームパッドの番号</param>
	/// <returns></returns>
	bool IsGamepadEnable(DWORD gamepadNumber);

	/// <summary>
	/// ゲームパッドのボタンの入力情報（Press）
	/// </summary>
	/// <param name="gamepadNumber">ゲームパッドの番号</param>
	/// <param name="wButtons">指定のボタン</param>
	/// <returns></returns>
	bool GetGamepadButtonPress(DWORD gamepadNumber, DWORD wButtons);

	/// <summary>
	/// ゲームパッドのボタンの入力情報（Trigger）
	/// </summary>
	/// <param name="gamepadNumber">ゲームパッドの番号</param>
	/// <param name="wButtons"></param>
	/// <returns></returns>
	bool GetGamepadButtonTrigger(DWORD gamepadNumber, DWORD wButtons);

	/// <summary>
	/// ゲームパッドのボタンの入力情報（Release）
	/// </summary>
	/// <param name="gamepadNumber">ゲームパッドの番号</param>
	/// <param name="wButtons"></param>
	/// <returns></returns>
	bool GetGamepadButtonRelease(DWORD gamepadNumber, DWORD wButtons);

	/// <summary>
	/// ゲームパッドの左スティックの入力情報
	/// </summary>
	/// <param name="gamepadNumber">ゲームパッドの番号</param>
	/// <returns></returns>
	Vector2 GetGamepadLeftStick(DWORD gamepadNumber);

	/// <summary>
	/// ゲームパッドの右スティックの入力情報
	/// </summary>
	/// <param name="gamepadNumber">ゲームパッドの番号</param>
	/// <returns></returns>
	Vector2 GetGamepadRightStick(DWORD gamepadNumber);

	/// <summary>
	/// ゲームパッドの左トリガーの入力情報
	/// </summary>
	/// <param name="gamepadNumber">ゲームパッドの番号</param>
	/// <returns></returns>
	float GetGamepadLeftTrigger(DWORD gamepadNumber);

	/// <summary>
	/// ゲームパッドの右トリガーの入力情報
	/// </summary>
	/// <param name="gamepadNumber">ゲームパッドの番号</param>
	/// <returns></returns>
	float GetGamepadRightTrigger(DWORD gamepadNumber);


private:

	// ウィンドウアプリケーション
	WinApp* winApp_ = nullptr;

	// DirectInput
	IDirectInput8* directInput_ = nullptr;


	/*   キーボード   */

	// キーボードデバイス
	IDirectInputDevice8* keyboard_ = nullptr;

	// マウスデバイス
	LPDIRECTINPUTDEVICE8 mouse_ = nullptr;

	// キー
	BYTE keys_[256];

	// 前のキー
	BYTE preKeys_[256];


	/*   マウス   */

	// マウスの入力情報
	DIMOUSESTATE mouseState_;

	// マウスの移動量
	Vector3 mouseVelocity_ = { 0.0f , 0.0f , 0.0f };

	// マウスボタンが押されたかどうか
	bool isPushButton_[3] = { false };

	// 前回のマウスボタンが押されたかどうか
	bool prevIsPushButton_[3] = { false };


	/*   ゲームパッド   */

	// 入力情報
	XINPUT_STATE gamepadState_[XUSER_MAX_COUNT]{};

	// 前回の入力情報
	XINPUT_STATE prevGamepadState_[XUSER_MAX_COUNT]{};

	// 接続確認
	DWORD dwResult_[XUSER_MAX_COUNT];
};

