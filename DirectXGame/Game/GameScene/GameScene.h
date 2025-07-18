#pragma once
#include "../../YokosukaEngine/Include/YokosukaEngine.h"
#include "Plane/Plane.h"
#include "Sprite/Sprite.h"

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

	
	// 平面
	std::unique_ptr<Plane> plane_ = nullptr;

	// スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;
};
