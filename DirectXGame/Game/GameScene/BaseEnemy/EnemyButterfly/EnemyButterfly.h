#pragma once
#include "../BaseEnemy.h"

class EnemyButterfly : public BaseEnemy
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	/// <param name="position"></param>
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
	/// 本体のワールド座標のGetter
	/// </summary>
	/// <returns></returns>
	Vector3 GetBodyWorldPosition() override;


private:


	// モデル列挙体
	enum ModelEnum
	{
		kBody,
		kWingR,
		kWingL,
		kNumModel
	};

	// モデル構造体
	ModelStruct models_[kNumModel];

	// モデルの初期位置
	const Vector3 kStartPosition[kNumModel] =
	{
		{0.0f,0.0f,0.0f},
		{0.0f,0.25f,0.0f},
		{0.0f,0.25f,0.0f}
	};

	// モデルの初期回転
	const Vector3 kStartRotation[kNumModel] =
	{
		{0.0f , 0.0f , 0.0f},
		{0.0f , 0.0f , 0.0f},
		{0.0f , 0.0f , 0.0f}
	};

	// ポイントライト
	std::unique_ptr<PointLight> pointLight_ = nullptr;


	/*--------------------
	    ギミック : 浮遊
	--------------------*/

	/// <summary>
	/// ギミック : 浮遊 : 初期化
	/// </summary>
	void GimmickFloatingInitialize();

	/// <summary>
	/// ギミック : 浮遊 : 更新処理
	/// </summary>
	void GimmickFloatingUpdate();

	// 浮遊ギミックのパラメータ
	float floatingParameter_ = 0.0f;

	// 浮遊ギミックの最大値
	const float kFloatingParameterMax = std::numbers::pi_v<float> *2.0f;

	// 浮遊ギミックの速度
	float floatingVelocity_ = 0.0f;

	// 浮遊ギミックの振幅
	float floatingAmplitude_ = 0.0f;


	/*-----------------------
	    ギミック : 羽ばたく
	-----------------------*/

	/// <summary>
	/// ギミック : 羽ばたく : 初期化
	/// </summary>
	void GimmickFlappingInitialize();

	/// <summary>
	/// ギミック : 羽ばたく : 更新処理
	/// </summary>
	void GimmickFlappingUpdate();

	// 羽ばたきギミックのパラメータ
	float flappingParameter_ = 0.0f;

	// 羽ばたきギミックの最大値
	const float kFlappingPrameterMax = std::numbers::pi_v<float> *2.0f;

	// 羽ばたきギミックの速度
	float flappingVelocity_ = 0.0f;

	// 幅だきぎいっくの振幅
	float flappingAmplitude_ = 0.0f;
};

