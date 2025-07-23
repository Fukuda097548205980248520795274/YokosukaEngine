#pragma once
#include "../BaseBoss.h"

class BossBenkei : public BaseBoss
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


private:

	// モデルの列挙体
	enum ModelEnum
	{
		kBody,
		kArmor,
		kHead,
		kHat,
		kShoulderR,
		kShoulderL,
		kElbowR,
		kElbowL,
		kHandR,
		kHandL,
		kWeapon,
		kNumModels
	};

	// モデルの構造体
	struct ModelStruct
	{
		// ワールドトランスフォーム
		std::unique_ptr<WorldTransform> worldTransform = nullptr;

		// モデルハンドル
		uint32_t modelHandle = 0;

		// 色
		Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	};


	// モデル
	ModelStruct models_[kNumModels];

	// モデルの初期位置
	const Vector3 kStartPosition[kNumModels] =
	{
		{0.0f , 0.0f , 0.0f},
		{0.0f , 0.0f , 0.0f},
		{0.0f , 3.8f , 0.0f},
		{0.0f , -0.3f , 0.0f},
		{3.0f , 2.3f , 0.9f},
		{-3.0f , 2.3f , 0.9f},
		{0.45f , -1.0f , 0.2f},
		{-0.45f , -1.0f , 0.2f},
		{0.85f , -0.95f , 0.1f},
		{-0.85f , -0.95f , 0.1f},
		{0.0f , -1.9f , -1.3f},
	};



	/*------------------
	    攻撃ビヘイビア
	------------------*/

	// 攻撃ビヘイビア
	enum AttackBehavior
	{
		// 何もない
		kNothing,

		// 薙ぎ払い
		kSweepAway
	};


	/*   薙ぎ払い   */

	/// <summary>
	/// 攻撃ビヘイビア : 薙ぎ払い : 初期化
	/// </summary>
	void AttackBehaviorSweepAwayInitialize();

	/// <summary>
	/// 攻撃ビヘイビア : 薙ぎ払い : 更新処理
	/// </summary>
	void AttackBehaviorSweppAwayUpdate();

	// 薙ぎ払いパラメータ
	float sweepAwayParamter_ = 0.0f;

	// 薙ぎ払い構え初期位置
	const Vector3 kSSweepAwayStanceRotation[kNumModels] =
	{
		{0.0f , 0.65f , 0.0f},
		{0.0f , 0.0f , 0.0f},
		{0.0f , -0.5f , 0.0f},
		{0.0f , 0.0f , 0.0f},
		{0.0f , 1.09f , 0.96f},
		{0.0f , 0.0f , 0.0f},
		{-0.67f , -0.26f , 0.0f},
		{0.0f , 0.0f , 0.0f},
		{0.0f , 1.77f , -1.05f},
		{0.0f , 0.0f , 0.0f},
		{0.0f , 0.0f , 0.0f},
	};
};

