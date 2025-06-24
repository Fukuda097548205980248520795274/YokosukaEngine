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

/// <summary>
/// 球面線形補間を行う
/// </summary>
/// <param name="start">始点</param>
/// <param name="end">終点</param>
/// <param name="t">媒介変数</param>
/// <returns></returns>
Vector3 Slerp(const Vector3& start, const Vector3& end, float t)
{
	// ベクトルを正規化する
	Vector3 normalizeStart = Normalize(start);
	Vector3 normalizeEnd = Normalize(end);

	// 長さを求めえる
	float lengthStart = Length(start);
	float lengthEnd = Length(end);

	// 内積を求める（1.0fを越えないようにする）
	float dot = std::clamp(Dot(normalizeStart, normalizeEnd), -1.0f, 1.0f);

	// acosでΘの角度を求める
	float theta = std::clamp(std::acos(dot), -179.0f / 180.0f * std::numbers::pi_v<float>, 179.0f / 180.0f * std::numbers::pi_v<float>);

	// Θの角度からsinΘを求める
	float sinTheta = std::sin(theta);

	// sin(Θ(1 - t))を求める
	float sinThetaFrom = std::sin((1.0f - t) * theta);

	// sin(tΘ)を求める
	float sinThetaTo = std::sin(t * theta);

	// 正規化補間ベクトル
	Vector3 slerp = { 0.0f , 0.0f , 0.0f };

	// ゼロ除算を防ぐ
	if (sinTheta < 1.0e-5)
	{
		slerp = normalizeStart;
	} else
	{
		slerp = (sinThetaFrom * normalizeStart + sinThetaTo * normalizeEnd) / sinTheta;
	}

	// 長さで線形補間を行う
	float length = Lerp(lengthStart, lengthEnd, t);

	return slerp * length;
}