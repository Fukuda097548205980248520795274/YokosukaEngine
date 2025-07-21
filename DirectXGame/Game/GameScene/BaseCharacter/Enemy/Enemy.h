#pragma once
#include "../BaseCharacter.h"

class Enemy : public BaseCharacter
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
	/// 中心座標のGetter
	/// </summary>
	/// <returns></returns>
	Vector3 GetCenterPosition() const;


private:


	// モデル
	enum ModelEnum
	{
		// 胴体
		kBody,

		// わっか
		kRing,

		// 刃
		kBlade,

		// モデルの数
		kNumModels
	};

	// モデル
	ModelStruct models_[kNumModels];

	// モデルの初期ポジション
	const Vector3 modelsStartPosition[kNumModels] =
	{
		{0.0f , 0.0f , 0.0f},
		{0.0f , 0.35f , 0.0f},
		{0.0f , 0.0f , 0.0f},
	};


	/*------------
		回転移動
	------------*/

	/// <summary>
	/// 回転移動ギミック初期化
	/// </summary>
	void InitializeRotateMoveGimmick();

	/// <summary>
	/// 回転移動ギミック更新処理
	/// </summary>
	void UpdateRotateMoveGimmick();


	// 回転ギミックの時間
	float rotateMoveTime_ = 3.0f;

	// 回転ギミックのパラメータ
	float rotateMoveParameter_ = 0.0f;

	// 回転ギミックの移動速度
	float rotateMoveSpeed_ = 0.1f;


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
	float floatingParamter_ = 0.0f;

	// 浮遊ギミックの時間
	float floatingTime_ = 2.0f;

	// 浮遊ギミックの振幅
	float floatingAmplitude_ = 0.1f;


	/*-------------------
	    ギミック : 回転
	-------------------*/

	/// <summary>
	/// ギミック : 回転 : 初期化
	/// </summary>
	void GimmickRotateInitialize();

	/// <summary>
	/// ギミック : 回転 : 更新処理
	/// </summary>
	void GimmickRotateUpdate();

	// 回転ギミックのパラメータ
	float rotateParameter_ = 0.0f;

	// 回転ギミックの時間
	float rotateTime_ = 1.0f;
};

