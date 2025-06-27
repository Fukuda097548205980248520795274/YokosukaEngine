#pragma once
#include <cmath>
#include <cassert>
#include "../Vector3/Vector3.h"
#include "../Vector4/Vector4.h"

// 4x4行列
struct Matrix4x4
{
	float m[4][4];

	// 加算
	Matrix4x4 operator+=(const Matrix4x4& matrix)
	{
		for (uint32_t i = 0; i < 4; ++i)
		{
			for (uint32_t j = 0; j < 4; ++j)
			{
				this->m[i][j] += matrix.m[i][j];
			}
		}

		return *this;
	}

	// 減算
	Matrix4x4 operator-=(const Matrix4x4& matrix)
	{
		for (uint32_t i = 0; i < 4; ++i)
		{
			for (uint32_t j = 0; j < 4; ++j)
			{
				this->m[i][j] -= matrix.m[i][j];
			}
		}

		return *this;
	}

	// スカラー倍
	Matrix4x4 operator*=(float scalar)
	{
		for (uint32_t i = 0; i < 4; ++i)
		{
			for (uint32_t j = 0; j < 4; ++j)
			{
				this->m[i][j] *= scalar;
			}
		}

		return *this;
	}

	// 乗算
	Matrix4x4 operator*=(const Matrix4x4& matrix)
	{
		// 乗算
		Matrix4x4 multiply;

		multiply.m[0][0] = this->m[0][0] * matrix.m[0][0] + this->m[0][1] * matrix.m[1][0] + this->m[0][2] * matrix.m[2][0] + this->m[0][3] * matrix.m[3][0];
		multiply.m[0][1] = this->m[0][0] * matrix.m[0][1] + this->m[0][1] * matrix.m[1][1] + this->m[0][2] * matrix.m[2][1] + this->m[0][3] * matrix.m[3][1];
		multiply.m[0][2] = this->m[0][0] * matrix.m[0][2] + this->m[0][1] * matrix.m[1][2] + this->m[0][2] * matrix.m[2][2] + this->m[0][3] * matrix.m[3][2];
		multiply.m[0][3] = this->m[0][0] * matrix.m[0][3] + this->m[0][1] * matrix.m[1][3] + this->m[0][2] * matrix.m[2][3] + this->m[0][3] * matrix.m[3][3];

		multiply.m[1][0] = this->m[1][0] * matrix.m[0][0] + this->m[1][1] * matrix.m[1][0] + this->m[1][2] * matrix.m[2][0] + this->m[1][3] * matrix.m[3][0];
		multiply.m[1][1] = this->m[1][0] * matrix.m[0][1] + this->m[1][1] * matrix.m[1][1] + this->m[1][2] * matrix.m[2][1] + this->m[1][3] * matrix.m[3][1];
		multiply.m[1][2] = this->m[1][0] * matrix.m[0][2] + this->m[1][1] * matrix.m[1][2] + this->m[1][2] * matrix.m[2][2] + this->m[1][3] * matrix.m[3][2];
		multiply.m[1][3] = this->m[1][0] * matrix.m[0][3] + this->m[1][1] * matrix.m[1][3] + this->m[1][2] * matrix.m[2][3] + this->m[1][3] * matrix.m[3][3];

		multiply.m[2][0] = this->m[2][0] * matrix.m[0][0] + this->m[2][1] * matrix.m[1][0] + this->m[2][2] * matrix.m[2][0] + this->m[2][3] * matrix.m[3][0];
		multiply.m[2][1] = this->m[2][0] * matrix.m[0][1] + this->m[2][1] * matrix.m[1][1] + this->m[2][2] * matrix.m[2][1] + this->m[2][3] * matrix.m[3][1];
		multiply.m[2][2] = this->m[2][0] * matrix.m[0][2] + this->m[2][1] * matrix.m[1][2] + this->m[2][2] * matrix.m[2][2] + this->m[2][3] * matrix.m[3][2];
		multiply.m[2][3] = this->m[2][0] * matrix.m[0][3] + this->m[2][1] * matrix.m[1][3] + this->m[2][2] * matrix.m[2][3] + this->m[2][3] * matrix.m[3][3];

		multiply.m[3][0] = this->m[3][0] * matrix.m[0][0] + this->m[3][1] * matrix.m[1][0] + this->m[3][2] * matrix.m[2][0] + this->m[3][3] * matrix.m[3][0];
		multiply.m[3][1] = this->m[3][0] * matrix.m[0][1] + this->m[3][1] * matrix.m[1][1] + this->m[3][2] * matrix.m[2][1] + this->m[3][3] * matrix.m[3][1];
		multiply.m[3][2] = this->m[3][0] * matrix.m[0][2] + this->m[3][1] * matrix.m[1][2] + this->m[3][2] * matrix.m[2][2] + this->m[3][3] * matrix.m[3][2];
		multiply.m[3][3] = this->m[3][0] * matrix.m[0][3] + this->m[3][1] * matrix.m[1][3] + this->m[3][2] * matrix.m[2][3] + this->m[3][3] * matrix.m[3][3];

		*this = multiply;
		return *this;
	}

	// スカラー除算
	Matrix4x4 operator/=(float scalar)
	{
		for (uint32_t i = 0; i < 4; ++i)
		{
			for (uint32_t j = 0; j < 4; ++j)
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
	Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2)
	{
		// 加算
		Matrix4x4 add;
		for (uint32_t i = 0; i < 4; ++i)
		{
			for (uint32_t j = 0; j < 4; ++j)
			{
				add.m[i][j] = m1.m[i][j] + m2.m[i][j];
			}
		}

		return add;
	}

	// 減算
	Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2)
	{
		// 加算
		Matrix4x4 subtract;
		for (uint32_t i = 0; i < 4; ++i)
		{
			for (uint32_t j = 0; j < 4; ++j)
			{
				subtract.m[i][j] = m1.m[i][j] - m2.m[i][j];
			}
		}

		return subtract;
	}

	// スカラー倍
	Matrix4x4 operator*(const Matrix4x4 matrix, float scalar)
	{
		// スカラー倍
		Matrix4x4 multiplyScalar;
		for (uint32_t i = 0; i < 4; ++i)
		{
			for (uint32_t j = 0; j < 4; ++j)
			{
				multiplyScalar.m[i][j] = matrix.m[i][j] * scalar;
			}
		}

		return multiplyScalar;
	}

	// スカラー倍
	Matrix4x4 operator*(float scalar, const Matrix4x4 matrix)
	{
		// スカラー倍
		Matrix4x4 multiplyScalar;
		for (uint32_t i = 0; i < 4; ++i)
		{
			for (uint32_t j = 0; j < 4; ++j)
			{
				multiplyScalar.m[i][j] = scalar * matrix.m[i][j];
			}
		}

		return multiplyScalar;
	}

	// 乗算
	Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2)
	{
		// 積
		Matrix4x4 multiply;

		multiply.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0];
		multiply.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1];
		multiply.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2];
		multiply.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3];

		multiply.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0];
		multiply.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1];
		multiply.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2];
		multiply.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3];

		multiply.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0];
		multiply.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1];
		multiply.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2];
		multiply.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3];

		multiply.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0];
		multiply.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1];
		multiply.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2];
		multiply.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3];

		return multiply;
	}

	// スカラー除算
	Matrix4x4 operator/(const Matrix4x4 matrix, float scalar)
	{
		// スカラー除算
		Matrix4x4 subdivisionScalar;
		for (uint32_t i = 0; i < 4; ++i)
		{
			for (uint32_t j = 0; j < 4; ++j)
			{
				subdivisionScalar.m[i][j] = matrix.m[i][j] / scalar;
			}
		}

		return subdivisionScalar;
	}
}

/// <summary>
/// 座標変換を行う
/// </summary>
/// <param name="vector">ベクトル</param>
/// <param name="matrix">行列</param>
/// <returns>座標変換したベクトル</returns>
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

/// <summary>
/// 座標変換を行う
/// </summary>
/// <param name="vector">ベクトル</param>
/// <param name="matrix">行列</param>
/// <returns>座標変換したベクトル</returns>
Vector4 Transform(const Vector4& vector, const Matrix4x4& matrix);

/// <summary>
/// 法線の座標変換
/// </summary>
/// <param name="vector"></param>
/// <param name="matrix"></param>
/// <returns></returns>
Vector3 TransformNormal(const Vector3& vector, const Matrix4x4 matrix);

/// <summary>
/// 拡大縮小行列を作る
/// </summary>
/// <param name="scale">拡縮</param>
/// <returns>拡大縮小行列</returns>
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

/// <summary>
/// X軸の回転行列を作る
/// </summary>
/// <param name="radian">ラジアン</param>
/// <returns>X軸の回転行列</returns>
Matrix4x4 MakeRotateXMatrix(float radian);

/// <summary>
/// Y軸の回転行列を作る
/// </summary>
/// <param name="radian">ラジアン</param>
/// <returns>Y軸の回転行列</returns>
Matrix4x4 MakeRotateYMatrix(float radian);

/// <summary>
/// Z軸の回転行列を作る
/// </summary>
/// <param name="radian">ラジアン</param>
/// <returns>Z軸の回転行列</returns>
Matrix4x4 MakeRotateZMatrix(float radian);

/// <summary>
/// 回転行列を作る
/// </summary>
/// <param name="rotation">回転</param>
/// <returns>回転行列</returns>
Matrix4x4 MakeRotateMatrix(const Vector3& rotation);

/// <summary>
/// 平行移動行列を作る
/// </summary>
/// <param name="translation">移動</param>
/// <returns>平行移動行列</returns>
Matrix4x4 MakeTranslateMatrix(const Vector3& translation);

/// <summary>
/// アフィン変換行列を作る
/// </summary>
/// <param name="scale">拡縮</param>
/// <param name="rotation">回転</param>
/// <param name="translation">移動</param>
/// <returns>アフィン変換行列</returns>
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotation, const Vector3& translation);

/// <summary>
/// 逆行列を作る
/// </summary>
/// <param name="m">行列</param>
/// <returns>逆行列</returns>
Matrix4x4 MakeInverseMatrix(const Matrix4x4& m);

/// <summary>
/// 平行投影行列作る
/// </summary>
/// <param name="left">左</param>
/// <param name="top">上</param>
/// <param name="right">右</param>
/// <param name="bottom">下</param>
/// <param name="nearClip">近平面の距離</param>
/// <param name="farClip">遠平面の距離</param>
/// <returns>平行投影行列</returns>
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

/// <summary>
/// 透視投影行列を作る
/// </summary>
/// <param name="fovY">画角</param>
/// <param name="aspectRatio">アスペクト比</param>
/// <param name="nearClip">近平面の距離</param>
/// <param name="farClip">遠平面の距離</param>
/// <returns>透視投影行列</returns>
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

/// <summary>
/// ビューポート変換行列を作る
/// </summary>
/// <param name="left"></param>
/// <param name="top"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="minDepth"></param>
/// <param name="maxDepth"></param>
/// <returns></returns>
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

/// <summary>
/// 単位行列を作る
/// </summary>
/// <returns>単位行列</returns>
Matrix4x4 MakeIdenityMatirx();

/// <summary>
/// 転置行列を作る
/// </summary>
/// <param name="m">行列</param>
/// <returns></returns>
Matrix4x4 MakeTransposeMatrix(Matrix4x4 m);

Vector3 Project(const Vector3& worldPosition, float viewportX, float viewportY, float viewportWidth, float viewportHeight,
	const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix);