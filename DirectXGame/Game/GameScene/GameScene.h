#pragma once
#include "../../YokosukaEngine/Include/YokosukaEngine.h"
#include "Player/Player.h"
#include "Skydome/Skydome.h"
#include "Ground/Ground.h"

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


	// directionalLight
	std::unique_ptr<DirectionalLight> directionalLight_ = nullptr;

	// skydome
	std::unique_ptr<Skydome> skydome_ = nullptr;

	// ground
	std::unique_ptr<Ground> ground_ = nullptr;

	// player
	std::unique_ptr<Player> player_ = nullptr;
};

