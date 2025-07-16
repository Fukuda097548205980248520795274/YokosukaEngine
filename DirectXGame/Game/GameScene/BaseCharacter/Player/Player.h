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
		{0.25f , 1.1f , 0.0f}
	};


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

