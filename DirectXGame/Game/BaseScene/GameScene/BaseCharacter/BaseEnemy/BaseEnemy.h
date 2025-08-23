#pragma once
#include "../BaseCharacter.h"
#include "../../BaseGimmick/GimmickDamageColor/GimmickDamageColor.h"

// 前方宣言
class BasePlayerBullet;
class Player;
class GameScene;
class CenterAxis;

class BaseEnemy : public BaseCharacter
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	/// <param name="position"></param>
	virtual void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const ModelHandleStore* modelHandleStore, const Vector3& position, int32_t hp) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() override = 0;

	/// <summary>
	/// 終了フラグのGetter
	/// </summary>
	/// <returns></returns>
	bool IsFinished() const { return isFinished_; }

	/// <summary>
	/// 強制的に終了させる
	/// </summary>
	void FinisheIt() { isFinished_ = true; }

	/// <summary>
	/// 攻撃力のGetter
	/// </summary>
	/// <returns></returns>
	int32_t GetPower() const { return power_; }

	/// <summary>
	/// 攻撃フラグのGetter
	/// </summary>
	/// <returns></returns>
	bool IsAttack() const { return isAttack_; }

	/// <summary>
	/// ワールド座標のGetter
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() const;

	/// <summary>
	/// 本体のワールド座標のGetter
	/// </summary>
	/// <returns></returns>
	virtual Vector3 GetBodyWorldPosition() const = 0;

	/// <summary>
	/// 本体のワールドトランスフォームのGetter
	/// </summary>
	/// <returns></returns>
	virtual WorldTransform* GetBodyWorldTransform() const override = 0;

	/// <summary>
	/// 当たり判定用のAABBのGetter
	/// </summary>
	/// <returns></returns>
	virtual AABB GetCollisionAABB() const = 0;

	/// <summary>
	/// 衝突判定応答
	/// </summary>
	/// <param name="playerBullet"></param>
	virtual void OnCollision(const BasePlayerBullet* playerBullet);

	/// <summary>
	/// ゲームシーンのSetter
	/// </summary>
	/// <param name="gameScene"></param>
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	/// <summary>
	/// 対象のSetter
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(BaseCharacter* target) { target_ = target; }

	/// <summary>
	/// ターゲットのGetter
	/// </summary>
	/// <returns></returns>
	const BaseCharacter* GetTarget() const { return target_; }

	/// <summary>
	/// ゲームタイマーのSetter
	/// </summary>
	/// <param name="gameTimer"></param>
	void SetGameTimer(const float* gameTimer) { gameTimer_ = gameTimer; }

	/// <summary>
	/// 接近する方向のSetter
	/// </summary>
	/// <param name="direction"></param>
	void SetApproachingDirection(const Vector3& direction) { approachingDirection_ = direction; }

	/// <summary>
	/// 接近する時間のSetter
	/// </summary>
	/// <param name="timer"></param>
	void SetApproachingTimer(float timer) { approachingTimer_ = timer; }

	/// <summary>
	/// 接近する方向のGetter
	/// </summary>
	/// <returns></returns>
	Vector3 GetApproachingDirection() { return approachingDirection_; }

	/// <summary>
	/// 接近する時間のGetter
	/// </summary>
	/// <returns></returns>
	float GetApproachingTimer() { return approachingTimer_; }

	/// <summary>
	/// 離脱する方向のSetter
	/// </summary>
	/// <param name="direction"></param>
	void SetAwayDirection(const Vector3& direction) { awayDirection_ = direction; }

	/// <summary>
	/// 離脱する時間のSetter
	/// </summary>
	/// <param name="timer"></param>
	void SetAwayTimer(float timer) { awayTimer_ = timer; }

	/// <summary>
	/// 離脱する方向のGetter
	/// </summary>
	/// <returns></returns>
	Vector3 GetAwayDirection() { return awayDirection_; }

	/// <summary>
	/// 離脱する時間のGetter
	/// </summary>
	/// <returns></returns>
	float GetAwayTimer() { return awayTimer_; }

	/// <summary>
	/// ゲームタイマーのGetter
	/// </summary>
	/// <returns></returns>
	const float* GetGameTimer() { return gameTimer_; }


protected:

	/// <summary>
	/// ダメージカラー
	/// </summary>
	virtual void DamageColor() = 0;


	// 対象
	const BaseCharacter* target_ = nullptr;

	// ゲームシーン
	GameScene* gameScene_ = nullptr;

	// ゲームタイマー
	const float* gameTimer_ = nullptr;


	// モデル構造体
	struct ModelStruct
	{
		// ワールドトランスフォーム
		std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

		// モデルハンドル
		uint32_t modelHandle_ = 0;

		// 色
		Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

		// ダメージカラーギミック
		std::unique_ptr<GimmickDamageColor> gimmickDamageColor_ = nullptr;
	};


	// 当たり判定の大きさ
	Vector3 hitSize_ = { 0.0f , 0.0f , 0.0f };


	// 攻撃力
	int32_t power_ = 0;

	// 攻撃フラグ
	bool isAttack_ = false;


	// ダメージ音
	uint32_t soundHandleDamage_ = 0;

	// 撃破音
	uint32_t soundHandleDestroy_ = 0;


	// 接近する方向
	Vector3 approachingDirection_ = { 0.0f , 0.0f , 0.0f };

	// 接近する時間
	float approachingTimer_ = 0.0f;


	// 離脱する方向
	Vector3 awayDirection_ = { 0.0f , 0.0f , 0.0f };

	// 離脱する時間
	float awayTimer_ = 0.0f;
};

