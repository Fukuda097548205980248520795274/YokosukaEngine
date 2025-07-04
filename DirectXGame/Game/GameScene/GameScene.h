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

	// DirectionalLight
	std::unique_ptr<DirectionalLight> directionalLight_;


	// WorldTransform
	std::unique_ptr<WorldTransform> worldTransform_;

	// uvTransform
	std::unique_ptr<UvTransform> uvTransform_;

	// ModelHandle
	uint32_t modelHandle_ = 0;
};

