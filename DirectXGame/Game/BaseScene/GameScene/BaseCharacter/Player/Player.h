#pragma once
#define NOMINMAX
#include "../BaseCharacter.h"
#include "../../BaseBullet/BasePlayerBullet/BasePlayerBullet.h"
#include "../../BaseBullet/BasePlayerBullet/PlayerBulletWeek/PlayerBulletWeek.h"
#include "../../BaseBullet/BasePlayerBullet/PlayerBulletStrong/PlayerBulletStrong.h"

// 前方宣言
class GameScene;
class BaseEnemy;
class BaseEnemyBullet;
class CenterAxis;

class Player : public BaseCharacter
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const ModelHandleStore* modelHandleStore, int32_t hp) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// ワールド座標のGetter
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() const;

	/// <summary>
	/// 本体のワールド座標のGetter
	/// </summary>
	/// <returns></returns>
	Vector3 GetBodyWorldPosition() const;

	/// <summary>
	/// 本体のワールドトランスフォームのGetter
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetBodyWorldTransform() const override { return bodyWorldTransform_.get(); };

	/// <summary>
	/// ゲームシーンのインスタンスのGetter
	/// </summary>
	/// <param name="gameScene"></param>
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	/// <summary>
	/// 衝突判定応答
	/// </summary>
	/// <param name="enemy"></param>
	void OnCollision(const BaseEnemy* enemy);

	/// <summary>
	/// 衝突判定応答
	/// </summary>
	/// <param name="bullet"></param>
	void OnCollision(const BaseEnemyBullet* enemyBullet);

	/// <summary>
	/// 当たり判定用のAABBを取得する
	/// </summary>
	/// <returns></returns>
	AABB GetCollisionAABB()const;


private:


	// ゲームシーン
	GameScene* gameScene_ = nullptr;


	/*----------
	    中心
	----------*/

	// 当たり判定の大きさ
	Vector3 hitSize_ = { 1.0f , 0.5f , 0.8f };

	// ダメージ音
	uint32_t soundHandleDamage1_ = 0;
	uint32_t soundHandleDamage2_ = 0;

	/// <summary>
	/// 入力操作
	/// </summary>
	void Input();


	/*---------
	    移動
	---------*/

	/// <summary>
	/// 移動操作
	/// </summary>
	void Move();

	/// <summary>
	/// ゲームパッドでの移動操作
	/// </summary>
	void MoveGamepad();

	/// <summary>
	/// キーボードでの移動操作
	/// </summary>
	void MoveKeyboard();

	// 移動速度
	const float speed = 0.4f;


	/*------------
	    弾の発射
	------------*/

	/// <summary>
	/// 弾の発射操作
	/// </summary>
	void BulletShot();

	/// <summary>
	/// ゲームパッドでの弾の発射操作
	/// </summary>
	void BulletShotGamepad();

	/// <summary>
	/// キーボードでの弾の発射操作
	/// </summary>
	void BulletShotKeyboard();

	// 大発射に要する時間
	const float kBigShotTime = 2.0f;

	// 発射に要する時間
	const float kShotTime = 1.1f;

	// 発射タイマー
	float shotTimer_ = 1.0f;

	// 小発射音
	uint32_t minShotSoundHandle_ = 0;

	// 大発射音
	uint32_t bigShotSoundHandle_ = 0;



	/*---------
	    本体
	---------*/

	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> bodyWorldTransform_ = nullptr;

	// モデルハンドル
	uint32_t bodyModelHandle_ = 0;

	// ポイントライト
	std::unique_ptr<PointLight> bodyPointLight_ = nullptr;



	/*-------------
	    ギミック
	-------------*/

	/*   浮き   */

	/// <summary>
	/// ギミック : 浮き : 初期化
	/// </summary>
	void GimmickFloatInitialize();

	/// <summary>
	/// ギミック : 浮き : 更新処理
	/// </summary>
	void GimmickFloatUpdate();

	// 浮きパラメータ
	float floatParameter_ = 0.0f;

	// 浮きパラメータ速度
	float kFloatPrameterVelocity = 0.05f;

	// 浮きパラメータ上限
	const float kMaxFloatParameter = std::numbers::pi_v<float> * 2.0f;

	// 浮き振れ幅
	float kFloatAmplitude = 0.3f;


	/*   傾き   */

	/// <summary>
	/// ギミック : 傾き : 更新処理
	/// </summary>
	void GimmickTiltUpdate();

	// ギミック : 傾き
	enum GimmickTilt
	{
		kStraight,
		kLeft,
		kRight,
		kNumTileGimmick
	};

	// 現在の傾きギミック
	GimmickTilt gimmickTilt_ = kStraight;

	// 目標角度
	const float kGimmickTiltGoalRadian[kNumTileGimmick] = { 0.0f , std::numbers::pi_v<float> / 4.0f , -std::numbers::pi_v<float> / 4.0f };


	/*   発射   */

	/// <summary>
	/// ギミック : 発射 : 初期化
	/// </summary>
	void GimmickShotInitialize();

	/// <summary>
	/// ギミック : 発射 : 更新処理
	/// </summary>
	void GimmickShotUpdate();

	// 発射パラメータ
	float shotParameter_ = 0.1f;

	// 発射パラメータの最大値
	const float kShotParameterMax = 0.1f;

	// パラメータの速度
	const float kShotParameterVelocity = 1.0f / 60.0f;

	// 発射したときの移動位置
	const Vector3 shotMove_ = Vector3(0.0f, 0.0f, -1.0f);

	// 発射したときのゴール地点
	const Vector3 shotGoal_ = Vector3(0.0f, 0.0f, 0.0f);
};

