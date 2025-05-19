#include "Vector4.h"

/// <summary>
/// ベクトルの長さを求める
/// </summary>
/// <param name="vector"></param>
/// <returns></returns>
float Length(const Vector4& vector)
{
	float length = std::sqrt(std::pow(vector.x, 2.0f) + std::pow(vector.y, 2.0f) + std::pow(vector.z, 2.0f));
	return length;
}

/// <summary>
/// 正規化したベクトルを求める
/// </summary>
/// <param name="vector"></param>
/// <returns></returns>
Vector4 NormalizeVector4(const Vector4& vector)
{
	// 正規化したベクトル
	Vector4 normalize = { 0.0f , 0.0f , 0.0f ,vector.w};

	// 長さ
	float length = Length(vector);

	if (length != 0.0f)
	{
		normalize.x = vector.x / length;
		normalize.y = vector.y / length;
		normalize.z = vector.z / length;
	}

	return normalize;
}