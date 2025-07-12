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
};

