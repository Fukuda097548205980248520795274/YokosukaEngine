#pragma once
#define NOMINMAX
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"
#include "../BasePlayerBullet/BasePlayerBullet.h"
#include "../BasePlayerBullet/PlayerBulletWeek/PlayerBulletWeek.h"
#include "../BasePlayerBullet/PlayerBulletStrong/PlayerBulletStrong.h"

// 前方宣言
class GameScene;
class BaseEnemy;

class Player
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
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

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
	/// ゲームシーンのインスタンスのGetter
	/// </summary>
	/// <param name="gameScene"></param>
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	/// <summary>
	/// 終了フラグのGetter
	/// </summary>
	/// <returns></returns>
	bool IsFinished() const { return isFinished_; }

	/// <summary>
	/// 衝突判定応答
	/// </summary>
	/// <param name="enemy"></param>
	void OnCollision(const BaseEnemy* enemy);


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// カメラ
	const Camera3D* camera3d_ = nullptr;

	// ゲームシーン
	GameScene* gameScene_ = nullptr;


	/*----------
	    中心
	----------*/

	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// 終了フラグ
	bool isFinished_ = false;

	// 体力
	int32_t hp_ = 0;

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

	// 弾のリスト
	std::list<BasePlayerBullet*> bullets_;

	// 第発射に要する時間
	const float kBigShotTime = 2.0f;

	// 発射に要する時間
	const float kShotTime = 1.1f;

	// 発射タイマー
	float shotTimer_ = 0.0f;

	// 小発射音
	uint32_t minShotSoundHandle_ = 0;

	// 大発射音
	uint32_t bigShotSoundHandle_ = 0;



	/*---------
	    本体
	---------*/

	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> bodyWorldTransform_ = nullptr;

	// UVトランスフォーム
	std::unique_ptr<UvTransform> bodyUvTransform_ = nullptr;

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
};

