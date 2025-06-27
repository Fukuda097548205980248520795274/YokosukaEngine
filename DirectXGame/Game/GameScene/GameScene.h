#pragma once
#include "../../YokosukaEngine/Include/YokosukaEngine.h"
#include "Player/Player.h"
#include "Planet/Planet.h"
#include "GravitationalField/GravitationalField.h"

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

	/// <summary>
	/// 全ての当たり判定を行う
	/// </summary>
	void CheckAllCollisions();


	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;

	// 惑星
	std::unique_ptr<Planet> planet_ = nullptr;

	// 重力場
	std::unique_ptr<GravitationalField> gravitationalField_ = nullptr;
};

