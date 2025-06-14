#pragma once
#include <cmath>

// 2次元ベクトル
struct Vector2
{
	float x;
	float y;

	// 加算する
	Vector2 operator+=(const Vector2& vector)
	{
		this->x += vector.x;
		this->y += vector.y;

		return *this;
	}

	// 減算する
	Vector2 operator-=(const Vector2& vector)
	{
		this->x -= vector.x;
		this->y -= vector.y;

		return *this;
	}

	// スカラー倍
	Vector2 operator*=(float scalar)
	{
		this->x *= scalar;
		this->y *= scalar;

		return *this;
	}

	// スカラー除算
	Vector2 operator/=(float scalar)
	{
		this->x /= scalar;
		this->y /= scalar;

		return *this;
	}

};


namespace
{

	// 加算する
	Vector2 operator+(const Vector2& v1, const Vector2& v2)
	{
		// 加算
		Vector2 add;

		add.x = v1.x + v2.x;
		add.y = v1.y + v2.y;

		return add;
	}

	// 減算する
	Vector2 operator-(const Vector2& v1, const Vector2& v2)
	{
		// 減算
		Vector2 subtract;

		subtract.x = v1.x - v2.x;
		subtract.y = v1.y - v2.y;

		return subtract;
	}

	// スカラー倍
	Vector2 operator*(const Vector2& vector, float scalar)
	{
		// スカラー倍
		Vector2 scalarMultiply;

		scalarMultiply.x = vector.x * scalar;
		scalarMultiply.y = vector.y * scalar;

		return scalarMultiply;
	}

	// スカラー倍
	Vector2 operator*(float scalar, const Vector2& vector)
	{
		// スカラー倍
		Vector2 scalarMultiply;

		scalarMultiply.x = scalar * vector.x;
		scalarMultiply.y = scalar * vector.y;

		return scalarMultiply;
	}

	// スカラー除算
	Vector2 operator/(const Vector2& vector, float scalar)
	{
		// スカラー除算
		Vector2 scalarDivision;

		scalarDivision.x = vector.x / scalar;
		scalarDivision.y = vector.y / scalar;

		return scalarDivision;
	}

	// スカラー除算
	Vector2 operator/(float scalar, const Vector2& vector)
	{
		// スカラー除算
		Vector2 scalarDivision;

		scalarDivision.x = scalar / vector.x;
		scalarDivision.y = scalar / vector.y;

		return scalarDivision;
	}

}

/// <summary>
/// ベクトルの距離を求める
/// </summary>
/// <param name="vector">ベクトル</param>
/// <returns></returns>
float Length(const Vector2& vector);

/// <summary>
/// 正規化する
/// </summary>
/// <param name="vector">ベクトル</param>
/// <returns></returns>
Vector2 Normalize(const Vector2& vector);

/// <summary>
/// 内積を求める
/// </summary>
/// <param name="v1">ベクトル1</param>
/// <param name="v2">ベクトル2</param>
/// <returns></returns>
float Dot(const Vector2& v1, const Vector2& v2);

/// <summary>
/// 外積を求める
/// </summary>
/// <param name="v1">ベクトル1</param>
/// <param name="v2">ベクトル2</param>
/// <returns></returns>
float Cross(const Vector2& v1, const Vector2& v2);

/// <summary>
/// 線形補間を行う
/// </summary>
/// <param name="start">始点</param>
/// <param name="end">終点</param>
/// <param name="t">媒介変数</param>
/// <returns></returns>
Vector2 Lerp(const Vector2& start, const Vector2& end, float t);