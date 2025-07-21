#pragma once
#include "../../YokosukaEngine/Include/YokosukaEngine.h"
#include "BaseCharacter/Player/Player.h"
#include "BaseCharacter/Enemy/Enemy.h"
#include "Skydome/Skydome.h"
#include "Ground/Ground.h"
#include "LockOn/LockOn.h"
#include "CollisionManager/CollisionManager.h"

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
	/// 全ての衝突判定を行う
	/// </summary>
	void CheckAllCollision();


	// directionalLight
	std::unique_ptr<DirectionalLight> directionalLight_ = nullptr;

	// skydome
	std::unique_ptr<Skydome> skydome_ = nullptr;

	// ground
	std::unique_ptr<Ground> ground_ = nullptr;

	// 衝突マネージャ
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;

	// player
	std::unique_ptr<Player> player_ = nullptr;

	// 敵
	std::list<std::unique_ptr<Enemy>> enemies_;

	// ロックオン
	std::unique_ptr<LockOn> lockOn_ = nullptr;
};

