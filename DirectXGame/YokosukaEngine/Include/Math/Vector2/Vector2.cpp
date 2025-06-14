#include "Vector2.h"

/// <summary>
/// ベクトルの距離を求める
/// </summary>
/// <param name="vector">ベクトル</param>
/// <returns></returns>
float Length(const Vector2& vector)
{
	float length = std::sqrt(std::pow(vector.x, 2.0f) + std::pow(vector.y, 2.0f));
	return length;
}

/// <summary>
/// 正規化する
/// </summary>
/// <param name="vector">ベクトル</param>
/// <returns></returns>
Vector2 Normalize(const Vector2& vector)
{
	float length = Length(vector);
	Vector2 normalize = { 0.0f , 0.0f };
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
float Dot(const Vector2& v1, const Vector2& v2)
{
	// 内積
	float dot = v1.x * v2.x + v1.y * v2.y;
	return dot;
}

/// <summary>
/// 外積を求める
/// </summary>
/// <param name="v1">ベクトル1</param>
/// <param name="v2">ベクトル2</param>
/// <returns></returns>
float Cross(const Vector2& v1, const Vector2& v2)
{
	// 外積
	float cross = v1.x * v2.y + v1.y + v2.x;
	return cross;
}

/// <summary>
/// 線形補間を行う
/// </summary>
/// <param name="start">始点</param>
/// <param name="end">終点</param>
/// <param name="t">媒介変数</param>
/// <returns></returns>
Vector2 Lerp(const Vector2& start, const Vector2& end, float t)
{
	return (1.0f - t) * start + t * end;
}