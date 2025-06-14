#include "Vector3.h"

/// <summary>
/// ベクトルの長さを求める
/// </summary>
/// <param name="vector"></param>
/// <returns></returns>
float Length(const Vector3& vector)
{
	float length = std::sqrt(std::pow(vector.x, 2.0f) + std::pow(vector.y, 2.0f) + std::pow(vector.z, 2.0f));
	return length;
}

/// <summary>
/// 正規化したベクトルを求める
/// </summary>
/// <param name="vector"></param>
/// <returns></returns>
Vector3 Normalize(const Vector3& vector)
{
	// 正規化したベクトル
	Vector3 normalize = { 0.0f , 0.0f , 0.0f };

	// 長さ
	float length = Length(vector);

	if (length != 0.0f)
	{
		normalize = vector / length;
	}

	return normalize;
}

/// <summary>
/// 内積を求める
/// </summary>
/// <param name="v1">ベクトル1</param>
/// <param name="v2">ベクトル2</param>
/// <returns></returns>
float Dot(const Vector3& v1, const Vector3& v2)
{
	// 内積
	float dot = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	return dot;
}

/// <summary>
/// クロス積を求める
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 Cross(const Vector3& v1, const Vector3& v2)
{
	// クロス積
	Vector3 cross;

	cross.x = v1.y * v2.z - v1.z * v2.y;
	cross.y = v1.z * v2.x - v1.x * v2.z;
	cross.z = v1.x * v2.y - v1.y * v2.x;

	return cross;
}

/// <summary>
/// 線形補間を行う
/// </summary>
/// <param name="start">始点</param>
/// <param name="end">終点</param>
/// <param name="t">媒介変数</param>
/// <returns></returns>
Vector3 Lerp(const Vector3& start, const Vector3& end, float t)
{
	return (1.0f - t) * start + t * end;
}