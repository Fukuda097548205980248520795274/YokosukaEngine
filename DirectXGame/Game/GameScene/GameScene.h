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

	// uvトランスフォーム
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;

	// モデルハンドル
	uint32_t modelHandle_ = 0;

	// 平行光源
	std::unique_ptr<DirectionalLight> directionalLight_ = nullptr;


	// 始点
	Vector3 start = { 5.0f , 0.0f , 0.0f };

	// 終点
	Vector3 end = { -5.0f , 0.1f , 0.0f };

	// 媒介変数
	float t = 0.0f;
};

