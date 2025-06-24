#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"
#include "EnemyBullet/EnemyBullet.h"
#include "BaseEnemyPhase/BaseEnemyPhase.h"
#include "../TimedCall/TimedCall.h"

// 前方宣言
class Player;

class Enemy
{
public:

	// 行動フェーズ
	enum class Phase
	{
		// 接近
		kApproach,

		// 離脱
		kLeave
	};


public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	/// <param name="directionalLight"></param>
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const DirectionalLight* directionalLight,
		const Vector3& position);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 弾を発射する
	/// </summary>
	void BulletShot();

	/// <summary>
	/// 弾を発射する前にタイマーをリセットする
	/// </summary>
	void BulletShotPrevReset();

	/// <summary>
	/// 弾を発射して、タイマーをリセットする
	/// </summary>
	void BulletShotedReset();

	/// <summary>
	/// 弾を発射する時間を消す
	/// </summary>
	void BulletShotTimerDelete();

	/// <summary>
	/// フェーズを切り替える
	/// </summary>
	/// <param name="phaseState"></param>
	void ChangePhase(std::unique_ptr<BaseEnemyPhase> phase);

	/// <summary>
	/// ワールド座標のGetter
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() const;

	/// <summary>
	/// プレイヤーのインスタンスのGetter
	/// </summary>
	/// <param name="player"></param>
	void SetPlayerInstance(Player* player) { player_ = player; }

	/// <summary>
	/// ワールドトランスフォームの移動のGetter
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldTransformTranslation() { return worldTransform_->translation_; }

	/// <summary>
	/// ワールドトランスフォームの移動のSetter
	/// </summary>
	/// <param name="translation"></param>
	void SetWorldTransformTranslation(const Vector3& translation) { worldTransform_->translation_ = translation; }

	/// <summary>
	/// 発射間隔のGetter
	/// </summary>
	/// <returns></returns>
	float GetKShotInterval() { return kShotInterval; }

	/// <summary>
	/// 弾のリストのGetter
	/// </summary>
	/// <returns></returns>
	const std::list<EnemyBullet*> GetBulletsInstance() { return bullets_; }

	/// <summary>
	/// 衝突コールバック関数
	/// </summary>
	void OnCollision();


private:


	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// 3Dカメラ
	const Camera3D* camera3d_ = nullptr;

	// 平行光源
	const DirectionalLight* directionalLight_ = nullptr;

	// プレイヤーのポインタ
	Player* player_ = nullptr;


	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// UVトランスフォーム
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;

	// モデルハンドル
	uint32_t modelHandle_ = 0;

	// ポイントライト
	std::unique_ptr<PointLight> pointLight_ = nullptr;

	// スポットライト
	std::unique_ptr<SpotLight> spotLight_ = nullptr;


	// フェーズの状態
	std::unique_ptr<BaseEnemyPhase> phase_;


	// フェーズの初期化テーブル
	static void(Enemy::*phaseInitializeTable[])();

	// フェーズの更新処理テーブル
	static void (Enemy::*phaseUpdateTable[])();


	// 時限発動のリスト
	std::list<TimedCall*> timedCalls_;

	// 弾のリスト
	std::list<EnemyBullet*> bullets_;

	// 発射間隔
	const float kShotInterval = 3.0f;
};

