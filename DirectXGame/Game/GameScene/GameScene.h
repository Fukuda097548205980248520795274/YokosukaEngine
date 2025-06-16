#pragma once
#include "../../YokosukaEngine/Include/YokosukaEngine.h"
#include "Player/Player.h"
#include "Enemy/Enemy.h"

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

	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;

	//　敵
	std::unique_ptr<Enemy> enemy_ = nullptr;
};

