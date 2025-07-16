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
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;


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
};

