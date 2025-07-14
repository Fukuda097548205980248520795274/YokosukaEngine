#pragma once
#include "../../YokosukaEngine/Include/YokosukaEngine.h"

#include "Player/Player.h"

#include "BasePlayerBullet/BasePlayerBullet.h"
#include "BasePlayerBullet/PlayerBulletWeek/PlayerBulletWeek.h"
#include "BasePlayerBullet/PlayerBulletStrong/PlayerBulletStrong.h"

#include "StageObject/StageObject.h"
#include "StageObject/Tutorial/Ground/TutorialGroundEmpty/TutorialGroundEmpty.h"

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
	/// デストラクタ
	/// </summary>
	~GameScene();

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
	/// ボスの更新処理
	/// </summary>
	void BossUpdate();

	/// <summary>
	/// ステージオブジェクトの更新処理
	/// </summary>
	void StageObjectUpdate();


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


	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;

	// プレイヤーの弾のリスト
	std::list<BasePlayerBullet*> playerBullets_;


	// 敵のリスト
	std::list<BaseEnemy*> enemies_;

	// 敵の弾のリスト
	std::list<BaseEnemyBullet*> enemyBullets_;


	// ボスのリスト
	std::list<BaseBoss*> bosses_;


	// ステージオブジェクトのリスト
	std::list<StageObject*> stageObjects_;




	// グロースクリーン
	uint32_t screenHandleGrow_ = 0;
};
