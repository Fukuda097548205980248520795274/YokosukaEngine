#pragma once
#include "../YokosukaEngine/Include/YokosukaEngine.h"
#include "GameScene/GameScene.h"

class Game
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine">エンジン</param>
	void Initialize(const YokosukaEngine* engine);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// ゲームシーン
	std::unique_ptr<GameScene> gameScene_ = nullptr;


	// 平行光源
	std::unique_ptr<DirectionalLight> directionalLight_ = nullptr;
};