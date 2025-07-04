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

	// モデル
	uint32_t modelHandle_ = 0;

	// 半径
	const float radius_ = 8.0f;

	// 向き
	enum Direction
	{
		kLeft,
		kRight
	};

	// 現在の向き
	Direction direction_ = kRight;

	// Y軸のラジアン
	float radianY_ = std::numbers::pi_v<float> / 3.0f;
};

