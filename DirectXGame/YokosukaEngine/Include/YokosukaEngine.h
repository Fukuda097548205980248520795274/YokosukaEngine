#pragma once
#include <stdlib.h>
#include <time.h>
#include "Func/CrashHandler/CrashHandler.h"
#include "Base/WinApp/WinApp.h"
#include "Base/OutputLog/OutputLog.h"
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Base/Input/Input.h"
#include "Audio/AudioStore/AudioStore.h"

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
	void Initialize(const int32_t kWindowWidth , const int32_t kWindowHeight , const std::string& titleName);

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
	uint32_t LoadTexture(const std::string& filePath) { return directXCommon_->LoadTexture(filePath); }

	/// <summary>
	/// モデルデータを読み込む
	/// </summary>
	/// <param name="directory"></param>
	/// <param name="fileName"></param>
	/// <returns></returns>
	uint32_t LoadModelData(const std::string& directory, const std::string& fileName) { return directXCommon_->LoadModelData(directory, fileName); }

	/// <summary>
	/// 音声データを読み込む
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	uint32_t LoadSound(const std::string& filePath) { return audioStore_->GetSoundHandle(filePath); }

	/// <summary>
	/// 音声データを再生する
	/// </summary>
	/// <param name="soundHandle"></param>
	void PlayerSoundData(uint32_t soundHandle) { audioStore_->SelectHandlePlayAudio(soundHandle); }

	/// <summary>
	/// 三角形を描画する
	/// </summary>
	void DrawTriangle(const WorldTransform* worldTransform, const WorldTransform* uvTransform, const Camera3D* camera , uint32_t textureHandle, Vector4 color)
	{directXCommon_->DrawTriangle(worldTransform,uvTransform, camera, textureHandle,color);}

	/// <summary>
	/// 球を描画する
	/// </summary>
	void DrawSphere(const WorldTransform* worldTransform, const WorldTransform* uvTransform, const Camera3D* camera, uint32_t textureHandle, Vector4 color)
	{
		directXCommon_->DrawSphere(worldTransform, uvTransform, camera, textureHandle, color);
	}

	/// <summary>
	/// モデルを描画する
	/// </summary>
	void DrawModel(const WorldTransform* worldTransform, const WorldTransform* uvTransform, const Camera3D* camera, uint32_t modelHandle, Vector4 color)
	{
		directXCommon_->DrawModel(worldTransform, uvTransform, camera, modelHandle, color);
	}

	/// <summary>
	/// スプライトを描画する
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="uvTransform"></param>
	/// <param name="camera"></param>
	/// <param name="textureHandle"></param>
	/// <param name="color"></param>
	void DrawSprite(const WorldTransform* worldTransform, const WorldTransform* uvTransform, const Camera2D* camera, uint32_t textureHandle, Vector4 color)
	{
		directXCommon_->DrawSprite(worldTransform, uvTransform, camera, textureHandle, color);
	}

	/// <summary>
	/// ブレンドモードを設定する
	/// </summary>
	/// <param name="blendMode"></param>
	void SetBlendModel(BlendMode blendMode) { directXCommon_->SetBlendMode(static_cast<uint32_t>(blendMode)); }
	
	/// <summary>
	/// 全てのキーの入力情報を取得する
	/// </summary>
	/// <param name="keys"></param>
	void CheckAllKeyInfo() { input_->CheckAllKeyInfo(); }

	/// <summary>
	/// 全てのキーの入力情報をコピーする
	/// </summary>
	void CopyAllKeyInfo() { input_->CopyAllKeyInfo(); }

	/// <summary>
	/// キー入力（Press）
	/// </summary>
	/// <param name="key">キー</param>
	/// <returns></returns>
	bool GetKeyPress(BYTE key) { return input_->GetKeyPress(key); }

	/// <summary>
	/// キー入力（Trigger）
	/// </summary>
	/// <param name="key">キー</param>
	/// <returns></returns>
	bool GetKeyTrigger(BYTE key) { return input_->GetKeyTrigger(key); }

	/// <summary>
	/// キー入力（Release）
	/// </summary>
	/// <param name="key">キー</param>
	/// <returns></returns>
	bool GetKeyRelease(BYTE key) { return input_->GetKeyRelease(key); }


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
};

