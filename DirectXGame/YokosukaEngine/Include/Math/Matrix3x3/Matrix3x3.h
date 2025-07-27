#pragma once
#include <cstdint>
#include <cmath>
#include <cassert>
#include "../Vector2/Vector2.h"
#include "../Vector3/Vector3.h"

// 3x3行列
struct Matrix3x3
{
	float m[3][3];

	// 加算
	Matrix3x3 operator+=(const Matrix3x3& matrix)
	{
		for (uint32_t i = 0; i < 3; i++)
		{
			for (uint32_t j = 0; j < 3; j++)
			{
				this->m[i][j] += matrix.m[i][j];
			}
		}

		return *this;
	}

	// 減算
	Matrix3x3 operator-=(const Matrix3x3& matrix)
	{
		for (uint32_t i = 0; i < 3; i++)
		{
			for (uint32_t j = 0; j < 3; j++)
			{
				this->m[i][j] -= matrix.m[i][j];
			}
		}

		return *this;
	}

	// スカラー倍
	Matrix3x3 operator*=(float scalar)
	{
		for (uint32_t i = 0; i < 3; i++)
		{
			for (uint32_t j = 0; j < 3; j++)
			{
				this->m[i][j] *= scalar;
			}
		}

		return *this;
	}

	// 乗算
	Matrix3x3 operator*=(const Matrix3x3& matrix)
	{
		// 乗算
		Matrix3x3 multiply;

		multiply.m[0][0] = this->m[0][0] * matrix.m[0][0] + this->m[0][1] * matrix.m[1][0] + this->m[0][2] * matrix.m[2][0];
		multiply.m[0][1] = this->m[0][0] * matrix.m[0][1] + this->m[0][1] * matrix.m[1][1] + this->m[0][2] * matrix.m[2][1];
		multiply.m[0][2] = this->m[0][0] * matrix.m[0][2] + this->m[0][1] * matrix.m[1][2] + this->m[0][2] * matrix.m[2][2];

		multiply.m[1][0] = this->m[1][0] * matrix.m[0][0] + this->m[1][1] * matrix.m[1][0] + this->m[1][2] * matrix.m[2][0];
		multiply.m[1][1] = this->m[1][0] * matrix.m[0][1] + this->m[1][1] * matrix.m[1][1] + this->m[1][2] * matrix.m[2][1];
		multiply.m[1][2] = this->m[1][0] * matrix.m[0][2] + this->m[1][1] * matrix.m[1][2] + this->m[1][2] * matrix.m[2][2];

		multiply.m[2][0] = this->m[2][0] * matrix.m[0][0] + this->m[2][1] * matrix.m[1][0] + this->m[2][2] * matrix.m[2][0];
		multiply.m[2][1] = this->m[2][0] * matrix.m[0][1] + this->m[2][1] * matrix.m[1][1] + this->m[2][2] * matrix.m[2][1];
		multiply.m[2][2] = this->m[2][0] * matrix.m[0][2] + this->m[2][1] * matrix.m[1][2] + this->m[2][2] * matrix.m[2][2];

		*this = multiply;
		return *this;
	}

	// スカラー除算
	Matrix3x3 operator/=(float scalar)
	{
		for (uint32_t i = 0; i < 3; i++)
		{
			for (uint32_t j = 0; j < 3; j++)
			{
				this->m[i][j] /= scalar;
			}
		}

		return *this;
	}
};

namespace
{
	// 加算
	Matrix3x3 operator+(const Matrix3x3& m1, const Matrix3x3& m2)
	{
		// 加算
		Matrix3x3 add;

		for (uint32_t i = 0; i < 3; i++)
		{
			for (uint32_t j = 0; j < 3; j++)
			{
				add.m[i][j] = m1.m[i][j] + m2.m[i][j];
			}
		}

		return add;
	}

	// 減算
	Matrix3x3 operator-(const Matrix3x3& m1, const Matrix3x3& m2)
	{
		// 減算
		Matrix3x3 subtract;

		for (uint32_t i = 0; i < 3; i++)
		{
			for (uint32_t j = 0; j < 3; j++)
			{
				subtract.m[i][j] = m1.m[i][j] - m2.m[i][j];
			}
		}

		return subtract;
	}

	// スカラー倍
	Matrix3x3 operator*(const Matrix3x3& matrix, float scalar)
	{
		// スカラー倍
		Matrix3x3 multiply;

		for (uint32_t i = 0; i < 3; i++)
		{
			for (uint32_t j = 0; j < 3; j++)
			{
				multiply.m[i][j] = matrix.m[i][j] * scalar;
			}
		}

		return multiply;
	}

	// 乗算
	Matrix3x3 operator*(const Matrix3x3& m1, const Matrix3x3& m2)
	{
		// 乗算
		Matrix3x3 multiply;

		multiply.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0];
		multiply.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1];
		multiply.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2];

		multiply.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0];
		multiply.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1];
		multiply.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2];

		multiply.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0];
		multiply.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1];
		multiply.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2];

		return multiply;
	}

	// スカラー倍
	Matrix3x3 operator*(float scalar , const Matrix3x3& matrix)
	{
		// スカラー倍
		Matrix3x3 multiply;

		for (uint32_t i = 0; i < 3; i++)
		{
			for (uint32_t j = 0; j < 3; j++)
			{
				multiply.m[i][j] = scalar * matrix.m[i][j];
			}
		}

		return multiply;
	}

	// スカラー除算
	Matrix3x3 operator/(const Matrix3x3& matrix, float scalar)
	{
		// スカラー除算
		Matrix3x3 division;

		for (uint32_t i = 0; i < 3; i++)
		{
			for (uint32_t j = 0; j < 3; j++)
			{
				division.m[i][j] = matrix.m[i][j] / scalar;
			}
		}

		return division;
	}
}


/// <summary>
/// 座標変換を行う
/// </summary>
/// <param name="vector">ベクトル</param>
/// <param name="matrix">行列</param>
/// <returns></returns>
Vector2 Transform(const Vector2& vector, const Matrix3x3& matrix);

/// <summary>
/// 移動以外の座標変換を行う
/// </summary>
/// <param name="vector">ベクトル</param>
/// <param name="matrix">行列</param>
/// <returns></returns>
Vector2 TransformNormal(const Vector2& vector, const Matrix3x3& matrix);

/// <summary>
/// 拡大縮小行列を作る
/// </summary>
/// <param name="vector">ベクトル</param>
/// <returns></returns>
Matrix3x3 MakeScaleMatrix(const Vector2& scale);

/// <summary>
/// 拡大縮小行列を作る
/// </summary>
/// <param name="scale"></param>
/// <returns></returns>
Matrix3x3 MakeScaleMatrix3x3(const Vector3& scale);

/// <summary>
/// 回転行列を作る
/// </summary>
/// <param name="radian">ラジアン</param>
/// <returns></returns>
Matrix3x3 MakeRotateMatrix(float radian);

/// <summary>
/// 平行移動行列を作る
/// </summary>
/// <param name="translation">移動</param>
/// <returns></returns>
Matrix3x3 MakeTranslateMatrix(const Vector2& translation);

/// <summary>
/// アフィン変換行列を作る
/// </summary>
/// <param name="scale">拡縮</param>
/// <param name="radian">ラジアン</param>
/// <param name="translation">移動</param>
/// <returns></returns>
Matrix3x3 MakeAffineMatrix(const Vector2& scale, float radian, const Vector2& translation);

/// <summary>
/// 逆行列を作る
/// </summary>
/// <param name="m"></param>
/// <returns></returns>
Matrix3x3 MakeInverseMatrix(const Matrix3x3& m);

/// <summary>
/// 平行投影行列を作る
/// </summary>
/// <param name="left">左</param>
/// <param name="top">上</param>
/// <param name="right">右</param>
/// <param name="bottom">下</param>
/// <returns></returns>
Matrix3x3 MakeOrthographicMatrix(float left, float top, float right, float bottom);

/// <summary>
/// ビューポート変換行列を作る
/// </summary>
/// <param name="left">左</param>
/// <param name="top">上</param>
/// <param name="width">幅</param>
/// <param name="height">高さ</param>
/// <returns></returns>
Matrix3x3 MakeViewportMatrix(float left, float top, float width, float height);

/// <summary>
/// 単位行列を作る
/// </summary>
/// <returns></returns>
Matrix3x3 MakeIdenityMatrix3x3();

/// <summary>
/// 転置行列を作る
/// </summary>
/// <param name="m">行列</param>
/// <returns></returns>
Matrix3x3 MakeTransposeMatrix(const Matrix3x3& m);