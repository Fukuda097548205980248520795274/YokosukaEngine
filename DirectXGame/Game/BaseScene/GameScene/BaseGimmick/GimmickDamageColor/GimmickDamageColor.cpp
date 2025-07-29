#include "GimmickDamageColor.h"

/// <summary>
/// 初期化
/// </summary>
void GimmickDamageColor::Initialize(WorldTransform* worldTransform, float parameterVelocity)
{
	// 基底クラスの初期化
	BaseGimmick::Initialize(worldTransform, parameterVelocity);

	// パラメータ最大値を設定する
	parameterMax_ = 1.0f;

	// 既に最大値から始める
	parameter_ = parameterMax_;
}

/// <summary>
/// 更新処理
/// </summary>
void GimmickDamageColor::Update()
{
	// 最大値になったら処理しない
	if (parameter_ >= parameterMax_)
		return;

	// 色パラメータを加算する
	parameter_ += parameterVelocity_;

	// 最大値まで進める
	if (parameter_ >= parameterMax_)
		parameterMax_ = parameterMax_;
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="modelHandle"></param>
/// <param name="color"></param>
void GimmickDamageColor::Draw(const YokosukaEngine* engine, const Camera3D* camera3d, uint32_t modelHandle, const Vector3& color)
{
	// 最大値になったら処理しない
	if (parameter_ >= parameterMax_)
		return;

	engine->DrawModel(worldTransform_, camera3d, modelHandle, Vector4(color.x, color.y, color.z, parameterMax_ - parameter_), false);
}

/// <summary>
/// リセット
/// </summary>
void GimmickDamageColor::Reset()
{
	parameter_ = 0.0f;
}