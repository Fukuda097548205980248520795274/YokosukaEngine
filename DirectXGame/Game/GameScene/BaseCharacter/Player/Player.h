#pragma once
#include "../BaseCharacter.h"
#include "../../GlobalVariables/GlobalVariables.h"

class Player : public BaseCharacter
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Vector3& position) override;

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


	// 攻撃用定数
	struct ConstAttack
	{
		// 振りかぶりにかかる時間
		float anticipationTime;

		// 溜めの時間
		float chargeTime;

		// 攻撃振りの時間
		float swingTime;

		// 硬直時間
		float recoveryTime;

		// 振りかぶりの移動の速さ
		float anticipationSpeed;

		// 溜めの移動の速さ
		float chargeSpeed;

		// 攻撃振りの移動の速さ
		float swingSpeed;
	};

	// 攻撃コンボ数
	static const int ComboNum = 3;

	// コンボ定数表
	static const std::array<ConstAttack, ComboNum> kConstAttacks;


	// 攻撃用ワーク
	struct WorkAttack
	{
		// パラメータ
		float parameter_ = 0.0f;
		int32_t comboIndex = 0;
		int32_t inComboPhase = 0;
		bool comboNext = false;
	};


private:

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void ApplyGlobalVaribles();

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

	/// <summary>
	/// ジャンプ操作
	/// </summary>
	void Jump();



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
		{0.0f,1.0f,0.0f}
	};

	// モデルの初期回転
	const Vector3 modelsStartRotation[kNumModels] =
	{
		{0.0f , 0.0f , 0.0f},
		{0.0f , 0.0f , 0.0f},
		{0.0f , 0.0f , 0.0f},
		{0.0f , 0.0f , 0.0f},
		{0.0f , 0.0f , 0.0f}
	};



	/*-------------
	    ふるまい
	-------------*/

	// ふるまい
	enum class Behavior
	{
		kRoot,
		kAttack,
		kDash,
		kJump,
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

	// 攻撃用ワーク
	WorkAttack workAttack_{};


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


	/*-----------------------
	    ふるまい : ジャンプ
	-----------------------*/

	/// <summary>
	/// ふるまい : ジャンプ : 初期化
	/// </summary>
	void BehaviorJumpInitialize();

	/// <summary>
	/// ふるまい : ジャンプ : 更新処理
	/// </summary>
	void BehaviorJumpUpdate();

	/// <summary>
	/// ふるまい : ジャンプ : 描画処理
	/// </summary>
	void BehaviorJumpDraw();

	// 速度
	Vector3 velocity_ = { 0.0f , 0.0f , 0.0f };



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

