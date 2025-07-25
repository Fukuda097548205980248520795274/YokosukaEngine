#pragma once
#include "../../YokosukaEngine/Include/YokosukaEngine.h"

#include "CenterAxis/CenterAxis.h"

#include "Player/Player.h"

#include "BasePlayerBullet/BasePlayerBullet.h"
#include "BasePlayerBullet/PlayerBulletWeek/PlayerBulletWeek.h"
#include "BasePlayerBullet/PlayerBulletStrong/PlayerBulletStrong.h"

#include "BaseBoss/BaseBoss.h"
#include "BaseBoss/BossBenkei/BossBenkei.h"

#include "BaseEnemy/BaseEnemy.h"
#include "BaseEnemy/EnemyButterfly/EnemyButterfly.h"

#include "BaseEnemyBullet/BaseEnemyBullet.h"

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

	/// <summary>
	/// カメラをシェイクする
	/// </summary>
	/// <param name="shakeTime"></param>
	/// <param name="shakeSize"></param>
	void CameraShake(float shakeTime, float shakeSize) { mainCamera_->SetShake(shakeTime, shakeSize); }

	/// <summary>
	/// プレイヤーの弾を発射する
	/// </summary>
	/// <param name="playerBullet"></param>
	void PlayerBulletShot(std::unique_ptr<BasePlayerBullet> playerBullet);

	/// <summary>
	/// 敵の弾を発射する
	/// </summary>
	/// <param name="enemyBullet"></param>
	void EnemyBulletShot(std::unique_ptr<BaseEnemyBullet> enemyBullet);


private:

	/// <summary>
	/// プレイヤーの弾の更新処理
	/// </summary>
	void PlayerBulletUpdate();

	/// <summary>
	/// 敵の更新処理
	/// </summary>
	void EnemyUpdate();

	/// <summary>
	/// 敵の弾の更新処理
	/// </summary>
	void EnemyBulletUpdate();

	/// <summary>
	/// 全ての当たり判定を行う
	/// </summary>
	void AllCheckCollision();


	// サウンドハンドル
	uint32_t soundHandle_ = 0;

	// プレイハンドル
	uint32_t playHandle_ = 0;



	// 平行光源
	std::unique_ptr<DirectionalLight> directionalLight_ = nullptr;

	// 天球
	std::unique_ptr<Skydome> skydome_ = nullptr;


	// 中心軸
	std::unique_ptr<CenterAxis> centerAxis_ = nullptr;


	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;

	// プレイヤーの弾のリスト
	std::list<std::unique_ptr<BasePlayerBullet>> playerBullets_;

	uint32_t playerBulletWeekModelHandle_ = 0;
	uint32_t playerBulletStrongModelHandle_ = 0;


	// 敵のリスト
	std::list<std::unique_ptr<BaseEnemy>> enemies_;

	// 敵の弾のリスト
	std::list<std::unique_ptr<BaseEnemyBullet>> enemyBullets_;


	// グロースクリーン
	uint32_t screenHandleGrow_ = 0;
};
