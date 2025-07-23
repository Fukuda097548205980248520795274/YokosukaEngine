#pragma once
#define DIRECTINPUT_VERSION 0x0800
#define MAX_CONTROLLERS4
#include <dinput.h>
#include <dxgi1_6.h>
#include <Xinput.h>
#include "../WinApp/WinApp.h"
#include "../../Math/Vector3/Vector3.h"
#include "../../Math/Vector2/Vector2.h"

// キーボード
enum KeyBord
{
	Key_A = DIK_A,
	Key_B = DIK_B,
	Key_C = DIK_C,
	Key_D = DIK_D,
	Key_E = DIK_E,
	Key_F = DIK_F,
	Key_G = DIK_G,
	Key_H = DIK_H,
	Key_I = DIK_I,
	Key_J = DIK_J,
	Key_K = DIK_K,
	Key_L = DIK_L,
	Key_M = DIK_M,
	Key_N = DIK_N,
	Key_O = DIK_O,
	Key_P = DIK_P,
	Key_Q = DIK_Q,
	Key_R = DIK_R,
	Key_S = DIK_S,
	Key_T = DIK_T,
	Key_U = DIK_U,
	Key_V = DIK_V,
	Key_W = DIK_W,
	Key_X = DIK_X,
	Key_Y = DIK_Y,
	Key_Z = DIK_Z,

	Key_1 = DIK_1,
	Key_2 = DIK_2,
	Key_3 = DIK_3,
	Key_4 = DIK_4,
	Key_5 = DIK_5,
	Key_6 = DIK_6,
	Key_7 = DIK_7,
	Key_8 = DIK_8,
	Key_9 = DIK_9,
	Key_0 = DIK_0,

	Key_Comma = DIK_COMMA,
	Key_Period = DIK_PERIOD,
	Key_Equals = DIK_EQUALS,
	Key_Slash = DIK_SLASH,
	Key_BackSlash = DIK_BACKSLASH,
	Key_Colon = DIK_COLON,
	Key_SemiColon = DIK_SEMICOLON,
	Key_Apostrophe = DIK_APOSTROPHE,
	Key_Grave = DIK_GRAVE,
	Key_AtMark = DIK_AT,
	Key_UnderLine = DIK_UNDERLINE,


	Key_F1 = DIK_F1,
	Key_F2 = DIK_F2,
	Key_F3 = DIK_F3,
	Key_F4 = DIK_F4,
	Key_F5 = DIK_F5,
	Key_F6 = DIK_F6,
	Key_F7 = DIK_F7,
	Key_F8 = DIK_F8,
	Key_F9 = DIK_F9,
	Key_F10 = DIK_F10,
	Key_F11 = DIK_F11,
	Key_F12 = DIK_F12,
	Key_F13 = DIK_F13,
	Key_F14 = DIK_F14,
	Key_F15 = DIK_F15,

	Key_Enter = DIK_RETURN,
	Key_Space = DIK_SPACE,
	Key_Esc = DIK_ESCAPE,
	Key_BackSpace = DIK_BACK,
	Key_Delete = DIK_DELETE,
	Key_Insert = DIK_INSERT,
	Key_Tab = DIK_TAB,

	Key_Home = DIK_HOME,
	Key_PageUp = DIK_PGUP,
	Key_PageDown = DIK_PGDN,
	Key_End = DIK_END,

	Key_ShitfL = DIK_LSHIFT,
	Key_ShitfR = DIK_RSHIFT,
	Key_CtrlL = DIK_LCONTROL,
	Key_CtrlR = DIK_RCONTROL,
	Key_BracketL = DIK_LBRACKET,
	Key_BracketR = DIK_RBRACKET,
	Key_AltL = DIK_LMENU,
	Key_AltR = DIK_RMENU,
	Key_WindowL = DIK_LWIN,
	Key_WindowR = DIK_RWIN,

	Key_CapsdLock = DIK_CAPITAL,
	Key_NumLock = DIK_NUMLOCK,
	Key_ScrollLock = DIK_SCROLL,

	Key_Up = DIK_UP,
	Key_Down = DIK_DOWN,
	Key_Right = DIK_RIGHT,
	Key_Left = DIK_LEFT,

	Numpad_1 = DIK_NUMPAD1,
	Numpad_2 = DIK_NUMPAD2,
	Numpad_3 = DIK_NUMPAD3,
	Numpad_4 = DIK_NUMPAD4,
	Numpad_5 = DIK_NUMPAD5,
	Numpad_6 = DIK_NUMPAD6,
	Numpad_7 = DIK_NUMPAD7,
	Numpad_8 = DIK_NUMPAD8,
	Numpad_9 = DIK_NUMPAD9,
	Numpad_0 = DIK_NUMPAD0,

	Numpad_Add = DIK_ADD,
	Numpad_Subtract = DIK_SUBTRACT,
	Numpad_Multiply = DIK_MULTIPLY,
	Numpad_Divid = DIK_DIVIDE,
	Numpad_Equals = DIK_NUMPADEQUALS,

	Numpad_Comma = DIK_NUMPADCOMMA,
	Numpad_Decimal = DIK_DECIMAL,
	Numpad_Enter = DIK_NUMPADENTER,

	JapanKey_Yen = DIK_YEN,
	JapanKey_Convert = DIK_CONVERT,
	JapanKey_NoConvert = DIK_NOCONVERT,
	JapanKey_Kana = DIK_KANA,
	JapanKey_Kanji = DIK_KANJI,
	JapanKey_Circumflex = DIK_CIRCUMFLEX,

};

// ゲームパッド
enum Gamepad
{
	Gamepad_A = XINPUT_GAMEPAD_A,
	Gamepad_B = XINPUT_GAMEPAD_B,
	Gamepad_X = XINPUT_GAMEPAD_X,
	Gamepad_Y = XINPUT_GAMEPAD_Y,

	Gamepad_ShoulderL = XINPUT_GAMEPAD_LEFT_SHOULDER,
	Gamepad_ShoulderR = XINPUT_GAMEPAD_RIGHT_SHOULDER,
	Gamepad_ThumbL = XINPUT_GAMEPAD_LEFT_THUMB,
	Gamepad_ThumbR = XINPUT_GAMEPAD_RIGHT_THUMB,

	Gamepad_Dpad_Up = XINPUT_GAMEPAD_DPAD_UP,
	Gamepad_Dpad_Down = XINPUT_GAMEPAD_DPAD_DOWN,
	Gamepad_Dpad_Left = XINPUT_GAMEPAD_DPAD_LEFT,
	GamePad_Dpad_Right = XINPUT_GAMEPAD_DPAD_RIGHT,

	Gamepad_Start = XINPUT_GAMEPAD_START,
	Gamepad_Back = XINPUT_GAMEPAD_BACK,
};

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
	bool GetKeyPress(KeyBord key);

	/// <summary>
	/// キーの入力情報（Trigger）
	/// </summary>
	/// <param name="key">キー</param>
	/// <returns></returns>
	bool GetKeyTrigger(KeyBord key);

	/// <summary>
	/// キーの入力情報（Release）
	/// </summary>
	/// <param name="key">キー</param>
	/// <returns></returns>
	bool GetKeyRelease(KeyBord key);

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
	Vector2 GetMouseVelocity() { return Vector2{ mouseVelocity_.x , mouseVelocity_.y }; }

	/// <summary>
	/// マウスホイールが上回転したかどうか
	/// </summary>
	/// <returns></returns>
	bool GetMouseWheelUp();

	/// <summary>
	/// マウスホイールが下回転したかどうか
	/// </summary>
	/// <returns></returns>
	bool GetMouseWheelDown();

	/// <summary>
	/// マウスホイールの移動量のGetter
	/// </summary>
	/// <returns></returns>
	float GetMouseWheelVelocity() { return mouseVelocity_.z; }

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
	bool GetGamepadButtonPress(DWORD gamepadNumber, Gamepad wButtons);

	/// <summary>
	/// ゲームパッドのボタンの入力情報（Trigger）
	/// </summary>
	/// <param name="gamepadNumber">ゲームパッドの番号</param>
	/// <param name="wButtons"></param>
	/// <returns></returns>
	bool GetGamepadButtonTrigger(DWORD gamepadNumber, Gamepad wButtons);

	/// <summary>
	/// ゲームパッドのボタンの入力情報（Release）
	/// </summary>
	/// <param name="gamepadNumber">ゲームパッドの番号</param>
	/// <param name="wButtons"></param>
	/// <returns></returns>
	bool GetGamepadButtonRelease(DWORD gamepadNumber, Gamepad wButtons);

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

