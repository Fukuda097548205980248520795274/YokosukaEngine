#pragma once
#include <cmath>

// エンジン用の4次元ベクトル
typedef struct Vector4
{
	float x;
	float y;
	float z;
	float w;

	// 加算する
	Vector4 operator+=(const Vector4& vector)
	{
		this->x += vector.x;
		this->y += vector.y;
		this->z += vector.z;
		this->w += vector.w;

		return *this;
	}

	// 減算する
	Vector4 operator-=(const Vector4& vector)
	{
		this->x -= vector.x;
		this->y -= vector.y;
		this->z -= vector.z;
		this->w -= vector.w;

		return *this;
	}

	// スカラー倍
	Vector4 operator*=(float scalar)
	{
		this->x *= scalar;
		this->y *= scalar;
		this->z *= scalar;
		this->w *= scalar;

		return *this;
	}

	// スカラー除算
	Vector4 operator/=(float scalar)
	{
		this->x /= scalar;
		this->y /= scalar;
		this->z /= scalar;
		this->w /= scalar;

		return *this;
	}

}Vector4;


namespace
{

	// 加算する
	Vector4 operator+(const Vector4& v1, const Vector4& v2)
	{
		// 加算
		Vector4 add;

		add.x = v1.x + v2.x;
		add.y = v1.y + v2.y;
		add.z = v1.z + v2.z;
		add.w = v1.w + v2.w;

		return add;
	}

	// 減算する
	Vector4 operator-(const Vector4& v1, const Vector4& v2)
	{
		// 減算
		Vector4 subtract;

		subtract.x = v1.x - v2.x;
		subtract.y = v1.y - v2.y;
		subtract.z = v1.z - v2.z;
		subtract.w = v1.w - v2.w;

		return subtract;
	}

	// スカラー倍
	Vector4 operator*(const Vector4& vector, float scalar)
	{
		// スカラー倍
		Vector4 scalarMultiply;

		scalarMultiply.x = vector.x * scalar;
		scalarMultiply.y = vector.y * scalar;
		scalarMultiply.z = vector.z * scalar;
		scalarMultiply.w = vector.w * scalar;

		return scalarMultiply;
	}

	// スカラー倍
	Vector4 operator*(float scalar, const Vector4& vector)
	{
		// スカラー倍
		Vector4 scalarMultiply;

		scalarMultiply.x = scalar * vector.x;
		scalarMultiply.y = scalar * vector.y;
		scalarMultiply.z = scalar * vector.z;
		scalarMultiply.w = scalar * vector.w;

		return scalarMultiply;
	}

	// スカラー除算
	Vector4 operator/(const Vector4& vector, float scalar)
	{
		// スカラー除算
		Vector4 scalarDivision;

		scalarDivision.x = vector.x / scalar;
		scalarDivision.y = vector.y / scalar;
		scalarDivision.z = vector.z / scalar;
		scalarDivision.w = vector.w / scalar;

		return scalarDivision;
	}

	// スカラー除算
	Vector4 operator/(float scalar, const Vector4& vector)
	{
		// スカラー除算
		Vector4 scalarDivision;

		scalarDivision.x = scalar / vector.x;
		scalarDivision.y = scalar / vector.y;
		scalarDivision.z = scalar / vector.z;
		scalarDivision.w = scalar / vector.w;

		return scalarDivision;
	}

}

/// <summary>
/// ベクトルの長さを求める
/// </summary>
/// <param name="vector"></param>
/// <returns></returns>
float Length(const Vector4& vector);

/// <summary>
/// 正規化したベクトルを求める
/// </summary>
/// <param name="vector"></param>
/// <returns></returns>
Vector4 NormalizeVector4(const Vector4& vector);