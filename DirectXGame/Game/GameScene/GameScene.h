#pragma once
#include "../../YokosukaEngine/Include/YokosukaEngine.h"
#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "CollisionManager/CollisionManager.h"
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

	/// <summary>
	/// コライダーを登録する
	/// </summary>
	void PushCollider();


	// 平行光源
	std::unique_ptr<DirectionalLight> directionalLight_ = nullptr;

	// 衝突マネージャ
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;

	// 天球
	std::unique_ptr<Skydome> skydome_ = nullptr;

	// 地面
	std::unique_ptr<Ground> ground_ = nullptr;

	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;

	//　敵
	std::unique_ptr<Enemy> enemy_ = nullptr;
};

