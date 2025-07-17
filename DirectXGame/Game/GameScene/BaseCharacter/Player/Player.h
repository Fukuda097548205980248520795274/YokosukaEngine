#pragma once
#include "../BaseCharacter.h"

class Player : public BaseCharacter
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// ワールドトランスフォームのGetter
	/// </summary>
	/// <returns></returns>
	const WorldTransform* GetWorldTransform() { return worldTransform_.get(); }

	/// <summary>
	/// メインカメラのSetter
	/// </summary>
	/// <param name="mainCamera"></param>
	void SetMainCamera(const MainCamera* mainCamera) { mainCamera_ = mainCamera; }


private:


	/// <summary>
	/// 入力操作
	/// </summary>
	void Input();

	/// <summary>
	/// 移動操作
	/// </summary>
	void Move();

	/// <summary>
	/// 攻撃操作
	/// </summary>
	void Attack();

	/// <summary>
	/// ダッシュ操作
	/// </summary>
	void Dash();



	// メインカメラ
	const MainCamera* mainCamera_ = nullptr;


	// 目標角度
	float toRotationY_ = 0.0f;



	// モデル
	enum ModelEnum
	{
		// 胴体
		kBody,

		// 頭
		kHead,

		// 左腕
		kLArm,

		// 右腕
		kRArm,

		// 武器
		kWeapon,

		// モデル数
		kNumModels
	};

	// モデル
	ModelStruct models_[kNumModels];

	// モデルの初期ポジション
	const Vector3 modelsStartPosition[kNumModels] =
	{
		{0.0f , 0.0f , 0.0f},
		{0.0f , 1.15f , 0.0f},
		{-0.25f , 1.1f , 0.0f},
		{0.25f , 1.1f , 0.0f},
		{0.0f,0.0f,0.0f}
	};



	/*-------------
	    ふるまい
	-------------*/

	// ふるまい
	enum class Behavior
	{
		kRoot,
		kAttack,
		kDash
	};

	// 現在のふるまい
	Behavior behavior_ = Behavior::kRoot;

	// 次のふるまいリクエスト
	std::optional<Behavior>  behaviorRequest_ = std::nullopt;


	/*--------------------
		ふるまい : 通常
	--------------------*/

	/// <summary>
	/// ふるまい : 通常 : 初期化
	/// </summary>
	void BehaviorRootInitialize();

	/// <summary>
	/// ふるまい : 通常 : 更新処理
	/// </summary>
	void BehaviorRootUpdate();

	/// <summary>
	/// ふるまい : 通常 : 描画処理
	/// </summary>
	void BehaviorRootDraw();


	/*--------------------
	    ふるまい : 攻撃
	--------------------*/

	/// <summary>
	/// ふるまい : 攻撃 : 初期化
	/// </summary>
	void BehaviorAttackInitialize();

	/// <summary>
	/// ふるまい : 攻撃 : 更新処理
	/// </summary>
	void BehaviorAttackUpdate();

	/// <summary>
	/// ふるまい : 攻撃 : 描画処理
	/// </summary>
	void BehaviorAttackDraw();

	// 攻撃パラメータ
	float attackParameter_ = 0.0f;

	// 攻撃パラメータ最大値
	const float kAttackParameterMax = 0.8f;

	// 振り上げフレーム
	const float kAttackSwingUpFrame[2] = { 0.0f , 0.2f };
	const float kAttackSwingUpRadianX[kNumModels][2] = 
	{
		{0.0f , 0.0f},
		{0.0f , 0.0f},
		{-1.6f , -3.1f},
		{-1.6f , -3.1f},
		{1.6f , 0.0f} 
	};

	// 振る下ろしフレーム
	const float kAttackSwingDownFrame[2] = { 0.4f , 0.6f };
	const float kAttackSwingDownRadianX[kNumModels][2] =
	{
		{0.0f , 0.0f},
		{0.0f , 0.0f},
		{-3.1f , -1.6f},
		{-3.1f , -1.6f},
		{0.0f , 1.6f}
	};
	const float kAttackSwingDownMove = 5.0f;

	// 攻撃の移動
	const float kAttackMoveSpeed = 5.0f;
	Vector3 attackPrevPosition_ = { 0.0f , 0.0f , 0.0f };
	Vector3 attackGoalPosition_ = { 0.0f , 0.0f , 0.0f };


	/*-----------------------
	    ふるまい : ダッシュ
	-----------------------*/

	/// <summary>
	/// ふるまい : ダッシュ : 初期化
	/// </summary>
	void BehaviorDashInitialize();

	/// <summary>
	/// ふるまい : ダッシュ : 更新処理
	/// </summary>
	void BehaviorDashUpdate();

	/// <summary>
	/// ふるまい : ダッシュ : 描画処理
	/// </summary>
	void BehaviorDashDraw();

	// ダッシュ用ワーク
	struct WorkDash
	{
		// パラメータ
		float parameter = 0.0f;

		// 速度
		float speed = 0.0f;

		// パラメータ最大値
		const float kPrameterMax = 1.0f;
	};

	WorkDash workDash_;



	/*----------------
		浮遊ギミック
	----------------*/

	/// <summary>
	/// 浮遊ギミック初期化
	/// </summary>
	void InitializeFloatingGimmick();

	/// <summary>
	/// 浮遊ギミック更新処理
	/// </summary>
	void UpdateFloatingGimmick();

	// 浮遊ギミックの時間
	float floatingTime_ = 1.0f;

	// 浮遊のギミックのパラメータ
	float floatingParameter_ = 0.0f;

	// 浮遊のギミックの揺れ幅
	float floatingAmplitude_ = 0.1f;


	/*-----------------
		手揺れギミック
	-----------------*/

	/// <summary>
	/// 手揺れギミック初期化
	/// </summary>
	void InitializeHandSwingGimmick();

	/// <summary>
	/// 手揺れギミック更新処理
	/// </summary>
	void UpdateHandSwingGimmick();

	// 手揺れギミックの時間
	float handSwingTime_ = 1.0f;

	// 手揺れギミックのパラメータ
	float handSwingParameter_ = 0.0f;

	// 手揺れギミックの揺れ幅
	float handSwingAmplitude_ = 0.12f;
};

