#pragma once
#include "../../YokosukaEngine/Include/YokosukaEngine.h"

class GameScene : public Scene
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine">エンジン</param>
	void Initialize(const YokosukaEngine* engine) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw()override;


private:

	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;
	float theta_ = 0.0f;
	float phi_ = 0.0f;

	// UVトランスフォーム
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;

	// モデルハンドル
	uint32_t modelHandle_ = 0;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// サウンドハンドル
	uint32_t soundHandle_ = 0;

	// プレイハンドル
	uint32_t playHandle_ = 0;
};

