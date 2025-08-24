#pragma once
#define NOMINMAX
#include "../../YokosukaEngine/Include/YokosukaEngine.h"

class FadeScreen
{
private:

	// フェード
	enum Fade
	{
		kIn,
		kOut
	};
	
	// 現在のフェード
	Fade fade_ = kIn;


public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera2d"></param>
	void Initialize(const YokosukaEngine* engine , const Camera2D* camera2d);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 色のSetter
	/// </summary>
	/// <param name="color"></param>
	void SetColor(const Vector3 color) { color_.x = color.x; color_.y = color.y; color_.z = color.z; }

	/// <summary>
	/// フェードインのリセット
	/// </summary>
	/// <param name="timer"></param>
	void ResetFadeIn(float timer);

	/// <summary>
	/// フェードアウトのリセット
	/// </summary>
	/// <param name="timer"></param>
	void ResetFadeOut(float timer);


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// 2Dカメラ
	const Camera2D* camera2d_ = nullptr;


	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform2D> worldTransform_ = nullptr;

	// UVトランスフォーム
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;

	// テクスチャ
	uint32_t textureHandle_ = 0;

	// 色
	Vector4 color_ = Vector4(0.0f, 0.0f, 0.0f, 0.0f);



	/// <summary>
	/// フェードインの更新処理
	/// </summary>
	void FadeInUpdate();

	/// <summary>
	/// フェードアウトの更新処理
	/// </summary>
	void FadeOutUpdate();

	// フェードする時間
	float fadeTime_ = 0.0f;

	// フェードタイマー
	float fadeTimer_ = 0.0f;
};

