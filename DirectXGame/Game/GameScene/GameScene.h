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


	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;
	uint32_t modelHandle_ = 0;
	std::unique_ptr<PointLight> pointLight_ = nullptr;
	std::unique_ptr<SpotLight> spotLight_ = nullptr;


	std::unique_ptr<DirectionalLight> directionalLight0_ = nullptr;
	std::unique_ptr<DirectionalLight> directionalLight1_ = nullptr;
	std::unique_ptr<DirectionalLight> directionalLight2_ = nullptr;
	std::unique_ptr<DirectionalLight> directionalLight3_ = nullptr;
};

