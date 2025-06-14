#pragma once
#include "../../Math/Vector3/Vector3.h"
#include "../../Math/Vector4/Vector4.h"
#include "../../Math/Matrix4x4/Matrix4x4.h"

class WorldTransform
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="scale"></param>
	/// <param name="rotation"></param>
	/// <param name="translation"></param>
	void Initialize(const Vector3& scale, const Vector3& rotation, const Vector3& translation);

	/// <summary>
	/// 親のワールドトランスフォームのSetter
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(WorldTransform* parent) { parent_ = parent; }

	/// <summary>
	/// ワールド行列を更新する
	/// </summary>
	void UpdateWorldMatrix();

	// 拡縮
	Vector3 scale_;

	// 回転
	Vector3 rotation_;

	// 移動
	Vector3 translation_;

	// ワールド行列
	Matrix4x4 worldMatrix_{};

	// 親ワールドトランスフォーム
	WorldTransform* parent_ = nullptr;
};

