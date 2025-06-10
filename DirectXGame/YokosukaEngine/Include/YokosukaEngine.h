#pragma once
#include <stdlib.h>
#include <time.h>
#include "Func/CrashHandler/CrashHandler.h"
#include "Base/WinApp/WinApp.h"
#include "Base/OutputLog/OutputLog.h"
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Base/Input/Input.h"
#include "Audio/AudioStore/AudioStore.h"
#include "Camera/DebugCamera/DebugCamera.h"

// ブレンドモード
enum BlendMode
{
	// ブレンドなし
	kBlendModeNone,

	// 通常αブレンド
	kBlendModeNormal,

	// 加算合成
	kBlendModeAdd,

	// 減算合成
	kBlendModeSubtract,

	// 乗算合成
	kBlendModeMultiply,

	// スクリーン合成
	kBlendModeScreen
};

// マウスボタン
enum MouseButton
{
	// 左ボタン
	kMouseButtonLeft,

	// 右ボタン
	kMouseButtonRight,

	// 中央ボタン
	kMouseButtonCenter
};

class YokosukaEngine
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~YokosukaEngine();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="kWindowWidth">ウィンドウの横幅</param>
	/// <param name="kWindowHeight">ウィンドウの縦幅</param>
	/// <param name="titleName">ウィンドウのタイトル名</param>
	void Initialize(const int32_t kWindowWidth, const int32_t kWindowHeight, const std::string& titleName);

	/// <summary>
	/// メッセージを処理する
	/// </summary>
	bool ProcessMessage() { return windowApplication_->ProcessMessage(); }

	/// <summary>
	/// フレーム開始
	/// </summary>
	void BeginFrame() { directXCommon_->PreDraw(); }

	/// <summary>
	/// フレーム終了
	/// </summary>
	void EndFrame() { directXCommon_->PostDraw(); }

	/// <summary>
	/// テクスチャを読み込む
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns></returns>
	uint32_t LoadTexture(const std::string& filePath) const { return directXCommon_->LoadTexture(filePath); }

	/// <summary>
	/// モデルデータを読み込む
	/// </summary>
	/// <param name="directory"></param>
	/// <param name="fileName"></param>
	/// <returns></returns>
	uint32_t LoadModelData(const std::string& directory, const std::string& fileName) const { return directXCommon_->LoadModelData(directory, fileName); }

	/// <summary>
	/// 音声データを読み込む
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	uint32_t LoadSound(const std::string& filePath) const { return audioStore_->GetSoundHandle(filePath); }

	/// <summary>
	/// 音声データを再生する
	/// </summary>
	/// <param name="soundHandle"></param>
	void PlayerSoundData(uint32_t soundHandle, float soundVolume) const { audioStore_->SelectHandlePlayAudio(soundHandle , soundVolume); }

	/// <summary>
	/// 三角形を描画する
	/// </summary>
	void DrawTriangle(const WorldTransform* worldTransform, const UvTransform* uvTransform,
		const Camera3D* camera, uint32_t textureHandle, Vector4 color) const
	{
		directXCommon_->DrawTriangle(worldTransform, uvTransform, camera, textureHandle, color);
	}

	/// <summary>
	/// 球を描画する
	/// </summary>
	void DrawSphere(const WorldTransform* worldTransform, const UvTransform* uvTransform,
		const Camera3D* camera, uint32_t textureHandle, Vector4 color,
		const DirectionalLight* directionalLight, const PointLight* pointLight , const SpotLight* spotLight) const
	{
		directXCommon_->DrawSphere(worldTransform, uvTransform, camera, textureHandle, color, directionalLight, pointLight, spotLight);
	}

	/// <summary>
	/// モデルを描画する
	/// </summary>
	void DrawModel(const WorldTransform* worldTransform, const UvTransform* uvTransform,
		const Camera3D* camera, uint32_t modelHandle, Vector4 color,
		const DirectionalLight* directionalLight, const PointLight* pointLight, const SpotLight* spotLight) const
	{
		directXCommon_->DrawModel(worldTransform, uvTransform, camera, modelHandle, color, directionalLight, pointLight, spotLight);
	}

	/// <summary>
	/// 線を描画する
	/// </summary>
	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <param name="camera"></param>
	/// <param name="color"></param>
	void DrawLine(const Vector3& start, const Vector3& end, const Camera3D* camera, const Vector4& color) const
	{
		directXCommon_->DrawLine(start, end, camera, color);
	}

	/// <summary>
	/// パーティクルを描画する
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="modelHandle"></param>
	/// <param name="color"></param>
	void DrawParticle(const Camera3D* camera, uint32_t modelHandle, Vector4 color) const { directXCommon_->DrawParticle(camera, modelHandle, color); }

	/// <summary>
	/// スプライトを描画する
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="uvTransform"></param>
	/// <param name="camera"></param>
	/// <param name="textureHandle"></param>
	/// <param name="color"></param>
	void DrawSprite(const WorldTransform* worldTransform, const UvTransform* uvTransform, const Camera2D* camera, uint32_t textureHandle, Vector4 color) const
	{
		directXCommon_->DrawSprite(worldTransform, uvTransform, camera, textureHandle, color);
	}

	/// <summary>
	/// ブレンドモードを設定する
	/// </summary>
	/// <param name="blendMode"></param>
	void SetObject3dBlendMode(BlendMode blendMode) const { directXCommon_->SetObject3dBlendMode(static_cast<uint32_t>(blendMode)); }

	/// <summary>
	/// ブレンドモードを設定する
	/// </summary>
	/// <param name="blendMode"></param>
	void SetParticleBlendMode(BlendMode blendMode) const { directXCommon_->SetParticleBlendMode(static_cast<uint32_t>(blendMode)); }

	/// <summary>
	/// 全てのキーの入力情報を取得する
	/// </summary>
	/// <param name="keys"></param>
	void CheckAllKeyInfo() const { input_->CheckInputInfo(); }

	/// <summary>
	/// 全てのキーの入力情報をコピーする
	/// </summary>
	void CopyAllKeyInfo() const { input_->CopyInputInfo(); }

	/// <summary>
	/// キー入力（Press）
	/// </summary>
	/// <param name="key">キー</param>
	/// <returns></returns>
	bool GetKeyPress(BYTE key) const { return input_->GetKeyPress(key); }

	/// <summary>
	/// キー入力（Trigger）
	/// </summary>
	/// <param name="key">キー</param>
	/// <returns></returns>
	bool GetKeyTrigger(BYTE key) const { return input_->GetKeyTrigger(key); }

	/// <summary>
	/// キー入力（Release）
	/// </summary>
	/// <param name="key">キー</param>
	/// <returns></returns>
	bool GetKeyRelease(BYTE key) const { return input_->GetKeyRelease(key); }

	/// <summary>
	/// マウスボタン（Press）
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool GetMouseButtonPress(MouseButton mouseButton) const { return input_->GetMousePress(static_cast<uint32_t>(mouseButton)); };

	/// <summary>
	/// マウスボタン（Trigger）
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool GetMouseButtonTrigger(MouseButton mouseButton) const { return input_->GetMouseTrigger(static_cast<uint32_t>(mouseButton)); };

	/// <summary>
	/// マウスボタン（Release）
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool GetMouseButtonRelease(MouseButton mouseButton) const { return input_->GetMouseRelease(static_cast<uint32_t>(mouseButton)); };

	/// <summary>
	/// マウスの移動量のGetter
	/// </summary>
	/// <returns></returns>
	Vector2 GetMouseVelocity() const { return input_->GetMouseVelocity(); }

	/// <summary>
	/// マウスホイールが上回転しているかどうか
	/// </summary>
	/// <returns></returns>
	bool GetMouseWheelUp() const { return input_->GetMouseWheelUp(); }

	/// <summary>
	/// マウスホイールが下回転しているかどうか
	/// </summary>
	/// <returns></returns>
	bool GetMouseWheelDown()  const { return input_->GetMouseWheelDown(); }

	/// <summary>
	/// マウスホイールの回転量のGetter
	/// </summary>
	/// <returns></returns>
	float GetMouseWheelVelocity() const { return input_->GetMouseWheelVelocity(); }

	/// <summary>
	/// デバッグカメラを更新する
	/// </summary>
	void DebugCameraUpdate() const { debugCamera_->Update(); }
	
	/// <summary>
	/// デバッグカメラのインスタンスを初期化する
	/// </summary>
	/// <returns></returns>
	DebugCamera* GetDebugCameraInstance() const { return debugCamera_; }


	/// <summary>
	/// 
	/// </summary>
	/// <param name="gamepadNumber"></param>
	/// <returns></returns>
	bool IsGamepadEnable(DWORD gamepadNumber) const { return input_->IsGamepadEnable(gamepadNumber); }

	/// <summary>
	/// ゲームパッドのボタンの入力情報（Press）
	/// </summary>
	/// <param name="gamepadNumber">ゲームパッドの番号</param>
	/// <param name="wButtons">指定のボタン</param>
	/// <returns></returns>
	bool GetGamepadButtonPress(DWORD gamepadNumber, DWORD wButtons) const { return input_->GetGamepadButtonPress(gamepadNumber, wButtons); }

	/// <summary>
	/// ゲームパッドのボタンの入力情報（Press）
	/// </summary>
	/// <param name="gamepadNumber">ゲームパッドの番号</param>
	/// <param name="wButtons">指定のボタン</param>
	/// <returns></returns>
	bool GetGamepadButtonTrigger(DWORD gamepadNumber, DWORD wButtons) const { return input_->GetGamepadButtonTrigger(gamepadNumber, wButtons); }

	/// <summary>
	/// ゲームパッドのボタンの入力情報（Press）
	/// </summary>
	/// <param name="gamepadNumber">ゲームパッドの番号</param>
	/// <param name="wButtons">指定のボタン</param>
	/// <returns></returns>
	bool GetGamepadButtonRelease(DWORD gamepadNumber, DWORD wButtons) const { return input_->GetGamepadButtonRelease(gamepadNumber, wButtons); }

	/// <summary>
	/// 
	/// </summary>
	/// <param name="gamepadNumber"></param>
	/// <returns></returns>
	Vector2 GetGamepadLeftStick(DWORD gamepadNumber) const { return input_->GetGamepadLeftStick(gamepadNumber); }

	/// <summary>
	/// 
	/// </summary>
	/// <param name="gamepadNumber"></param>
	/// <returns></returns>
	Vector2 GetGamepadRightStick(DWORD gamepadNumber) const { return input_->GetGamepadRightStick(gamepadNumber); }

	/// <summary>
	/// 
	/// </summary>
	/// <param name="gamepadNumber"></param>
	/// <returns></returns>
	float GetGamepadLeftTrigger(DWORD gamepadNumber) const { return input_->GetGamepadLeftTrigger(gamepadNumber); }

	/// <summary>
	/// 
	/// </summary>
	/// <param name="gamepadNumber"></param>
	/// <returns></returns>
	float GetGamepadRightTrigger(DWORD gamepadNumber) const { return input_->GetGamepadRightTrigger(gamepadNumber); }


private:

	// 現在の時間
	unsigned int currentTimer_;

	// ウィンドウアプリケーション
	WinApp* windowApplication_ = nullptr;

	// ログ出力
	OutputLog* log_ = nullptr;

	// DirectXCommon
	DirectXCommon* directXCommon_ = nullptr;

	// Input
	Input* input_ = nullptr;

	// オーディオストア
	AudioStore* audioStore_ = nullptr;

	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
};