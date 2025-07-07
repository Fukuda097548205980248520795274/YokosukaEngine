#pragma once
#include "../../YokosukaEngine/Include/YokosukaEngine.h"
#include "Player/Player.h"
#include "Skydome/Skydome.h"

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


	// サウンドハンドル
	uint32_t soundHandle_ = 0;

	// プレイハンドル
	uint32_t playHandle_ = 0;


	// 平行光源
	std::unique_ptr<DirectionalLight> directionalLight_ = nullptr;

	// 天球
	std::unique_ptr<Skydome> skydome_ = nullptr;

	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;
};

