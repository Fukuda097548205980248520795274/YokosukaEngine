#pragma once
#include <cmath>

// エンジン用の3次元ベクトル
typedef struct Vector3
{
	float x;
	float y;
	float z;

	// 加算する
	Vector3 operator+=(const Vector3& vector)
	{
		this->x += vector.x;
		this->y += vector.y;
		this->z += vector.z;

		return *this;
	}

	// 減算する
	Vector3 operator-=(const Vector3& vector)
	{
		this->x -= vector.x;
		this->y -= vector.y;
		this->z -= vector.z;

		return *this;
	}

	// スカラー倍
	Vector3 operator*=(float scalar)
	{
		this->x *= scalar;
		this->y *= scalar;
		this->z *= scalar;

		return *this;
	}

	// スカラー除算
	Vector3 operator/=(float scalar)
	{
		this->x /= scalar;
		this->y /= scalar;
		this->z /= scalar;

		return *this;
	}

}Vector3;


namespace
{

	// 加算する
	Vector3 operator+(const Vector3& v1, const Vector3& v2)
	{
		// 加算
		Vector3 add;

		add.x = v1.x + v2.x;
		add.y = v1.y + v2.y;
		add.z = v1.z + v2.z;

		return add;
	}

	// 減算する
	Vector3 operator-(const Vector3& v1, const Vector3& v2)
	{
		// 減算
		Vector3 subtract;

		subtract.x = v1.x - v2.x;
		subtract.y = v1.y - v2.y;
		subtract.z = v1.z - v2.z;

		return subtract;
	}

	// スカラー倍
	Vector3 operator*(const Vector3& vector, float scalar)
	{
		// スカラー倍
		Vector3 scalarMultiply;

		scalarMultiply.x = vector.x * scalar;
		scalarMultiply.y = vector.y * scalar;
		scalarMultiply.z = vector.z * scalar;

		return scalarMultiply;
	}

	// スカラー倍
	Vector3 operator*(float scalar, const Vector3& vector)
	{
		// スカラー倍
		Vector3 scalarMultiply;

		scalarMultiply.x = scalar * vector.x;
		scalarMultiply.y = scalar * vector.y;
		scalarMultiply.z = scalar * vector.z;

		return scalarMultiply;
	}

	// スカラー除算
	Vector3 operator/(const Vector3& vector, float scalar)
	{
		// スカラー除算
		Vector3 scalarDivision;

		scalarDivision.x = vector.x / scalar;
		scalarDivision.y = vector.y / scalar;
		scalarDivision.z = vector.z / scalar;

		return scalarDivision;
	}

	// スカラー除算
	Vector3 operator/(float scalar, const Vector3& vector)
	{
		// スカラー除算
		Vector3 scalarDivision;

		scalarDivision.x = scalar / vector.x;
		scalarDivision.y = scalar / vector.y;
		scalarDivision.z = scalar / vector.z;

		return scalarDivision;
	}

}

/// <summary>
/// ベクトルの長さを求める
/// </summary>
/// <param name="vector"></param>
/// <returns></returns>
float Length(const Vector3& vector);

/// <summary>
/// 正規化したベクトルを求める
/// </summary>
/// <param name="vector"></param>
/// <returns></returns>
Vector3 Normalize(const Vector3& vector);

/// <summary>
/// クロス積を求める
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 Cross(const Vector3& v1, const Vector3& v2);