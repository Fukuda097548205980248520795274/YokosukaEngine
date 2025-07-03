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


	// 平行光源
	std::unique_ptr<DirectionalLight> directionalLight_ = nullptr;

	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// UVトランスフォーム
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// 角度
	float theta_ = std::numbers::pi_v<float> / 2.0f;
	float phi_ = 0.0f;
};

