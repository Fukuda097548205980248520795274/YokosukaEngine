#pragma once
#include "../../YokosukaEngine/Include/YokosukaEngine.h"
#include "Player/Player.h"
#include "Skydome/Skydome.h"
#include "Blocks/Blocks.h"
#include "MapChipField/MapChipField.h"
#include "Enemy/Enemy.h"
#include "DeathParticle/DeathParticle.h"

class GameScene
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	void Initialize(const YokosukaEngine* engine , const Camera3D* camera3d);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// プレイヤーのインスタンスのGetter
	/// </summary>
	/// <returns></returns>
	Player* GetPlayerInstance() { return player_.get(); }

	/// <summary>
	/// 全ての当たり判定を行う
	/// </summary>
	void CheckAllCollisions();


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// 3Dカメラ
	const Camera3D* camera3d_ = nullptr;

	// 平行光源
	std::unique_ptr<DirectionalLight> directionalLight_ = nullptr;

	// マップチップフィールド
	std::unique_ptr<MapChipField> mapChipField_ = nullptr;


	// 天球
	std::unique_ptr<Skydome> skydome_ = nullptr;

	// ブロック
	std::unique_ptr<Blocks> blocks_ = nullptr;

	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;

	// デスパーティクル
	std::unique_ptr<DeathParticle> deathParticle_ = nullptr;


	// 敵の数
	const uint32_t kNumEnemy = 3;

	// 敵のリスト
	std::list<Enemy*> enemies_ = {};


	/// <summary>
	/// プレイヤーと敵の当たり判定
	/// </summary>
	void CheckPlayerAndEnemyCollision();
};

