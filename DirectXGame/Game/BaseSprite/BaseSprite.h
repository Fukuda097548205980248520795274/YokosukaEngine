#pragma once
#include "../../YokosukaEngine/Include/YokosukaEngine.h"

class Sprite
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera2d"></param>
	/// <param name="textureHandle"></param>
	void Initialize(const YokosukaEngine* engine, const Camera2D* camera2d, const Vector2& anchor, uint32_t textureHandle);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();


	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform2D> worldTransform_ = nullptr;

	// UVトランスフォーム
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;

	// 色
	Vector4 color_ = { 1.0f , 1.0f , 1.0f , 1.0f };



private:


	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// 2Dカメラ
	const Camera2D* camera2d_ = nullptr;


	// テクスチャ
	uint32_t textureHandle_ = 0;
};

