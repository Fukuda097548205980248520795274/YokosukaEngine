#pragma once
#include "../../YokosukaEngine/Include/YokosukaEngine.h"

class Fade
{
public:

	enum class Status
	{
		// フェードなし
		kNone,

		// フェードアウト中
		kFadeOut,

		// フェードイン中
		kFadeIn,
	};


public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	void Initialize(const YokosukaEngine* engine);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// フェード開始
	/// </summary>
	/// <param name="status">フェード状態</param>
	/// <param name="duration">フェードする時間</param>
	void Start(Status status, float duration);

	/// <summary>
	/// フェード停止
	/// </summary>
	void Stop();

	/// <summary>
	/// フェード終了判定
	/// </summary>
	/// <returns></returns>
	bool IsFinished();


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;


	// 2Dカメラ
	std::unique_ptr<Camera2D> camera2d_ = nullptr;

	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// UVトランスフォーム
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// 色
	Vector4 color_ = { 0.0f , 0.0f , 0.0f , 0.0f };


	// 現在のフェードの状態
	Status status_ = Status::kNone;

	// フェードの継続時間
	float duration_ = 0.0f;

	// 経過時間カウンター
	float counter_ = 0.0f;
};

