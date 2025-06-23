#include "Matrix3x3.h"

/// <summary>
/// 座標変換を行う
/// </summary>
/// <param name="vector">ベクトル</param>
/// <param name="matrix">行列</param>
/// <returns></returns>
Vector2 Transform(const Vector2& vector, const Matrix3x3& matrix)
{
	// 座標変換した値
	Vector2 transform;

	transform.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + 1.0f * matrix.m[2][0];
	transform.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + 1.0f * matrix.m[2][1];
	float w = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + 1.0f * matrix.m[2][2];

	assert(w != 0.0f);

	transform /= w;
	return transform;
}

/// <summary>
/// 移動以外の座標変換を行う
/// </summary>
/// <param name="vector">ベクトル</param>
/// <param name="matrix">行列</param>
/// <returns></returns>
Vector2 TransformNormal(const Vector2& vector, const Matrix3x3& matrix)
{
	// 座標変換した値
	Vector2 transform;

	transform.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0];
	transform.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1];

	return transform;
}

/// <summary>
/// 拡大縮小行列を作る
/// </summary>
/// <param name="vector">ベクトル</param>
/// <returns></returns>
Matrix3x3 MakeScaleMatrix(const Vector2& scale)
{
	// 拡大縮小行列
	Matrix3x3 scaleMatrix;

	scaleMatrix.m[0][0] = scale.x;
	scaleMatrix.m[0][1] = 0.0f;
	scaleMatrix.m[0][2] = 0.0f;

	scaleMatrix.m[1][0] = 0.0f;
	scaleMatrix.m[1][1] = scale.y;
	scaleMatrix.m[1][2] = 0.0f;

	scaleMatrix.m[2][0] = 0.0f;
	scaleMatrix.m[2][1] = 0.0f;
	scaleMatrix.m[2][2] = 1.0f;

	return scaleMatrix;
}

/// <summary>
/// 回転行列を作る
/// </summary>
/// <param name="radian">ラジアン</param>
/// <returns></returns>
Matrix3x3 MakeRotateMatrix(float radian)
{
	// 回転行列
	Matrix3x3 rotateMatrix;

	rotateMatrix.m[0][0] = std::cos(radian);
	rotateMatrix.m[0][1] = std::sin(radian);
	rotateMatrix.m[0][2] = 0.0f;

	rotateMatrix.m[1][0] = -std::sin(radian);
	rotateMatrix.m[1][1] = std::cos(radian);
	rotateMatrix.m[1][2] = 0.0f;

	rotateMatrix.m[2][0] = 0.0f;
	rotateMatrix.m[2][1] = 0.0f;
	rotateMatrix.m[2][2] = 1.0f;

	return rotateMatrix;
}

/// <summary>
/// 平行移動行列を作る
/// </summary>
/// <param name="translation">移動</param>
/// <returns></returns>
Matrix3x3 MakeTranslateMatrix(const Vector2& translation)
{
	// 平行移動行列
	Matrix3x3 translateMatrix;

	translateMatrix.m[0][0] = 1.0f;
	translateMatrix.m[0][1] = 0.0f;
	translateMatrix.m[0][2] = 0.0f;

	translateMatrix.m[1][0] = 0.0f;
	translateMatrix.m[1][1] = 1.0f;
	translateMatrix.m[1][2] = 0.0f;

	translateMatrix.m[2][0] = translation.x;
	translateMatrix.m[2][1] = translation.y;
	translateMatrix.m[2][2] = 1.0f;

	return translateMatrix;
}

/// <summary>
/// アフィン変換行列を作る
/// </summary>
/// <param name="scale">拡縮</param>
/// <param name="radian">ラジアン</param>
/// <param name="translation">移動</param>
/// <returns></returns>
Matrix3x3 MakeAffineMatrix(const Vector2& scale, float radian, const Vector2& translation)
{
	// アフィン変換行列
	Matrix3x3 affineMatrix = MakeScaleMatrix(scale) * MakeRotateMatrix(radian) * MakeTranslateMatrix(translation);
	return affineMatrix;
}

/// <summary>
/// 逆行列を作る
/// </summary>
/// <param name="m"></param>
/// <returns></returns>
Matrix3x3 MakeInverseMatrix(const Matrix3x3& m)
{
	// 行列式
	float determinant = m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] + m.m[0][2] * m.m[1][0] * m.m[2][1] -
		m.m[0][2] * m.m[1][1] * m.m[2][0] - m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1];

	// 余因子行列
	Matrix3x3 adjugateMatrix;

	adjugateMatrix.m[0][0] = m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1];
	adjugateMatrix.m[0][1] = -(m.m[0][1] * m.m[2][2] - m.m[0][2] * m.m[2][1]);
	adjugateMatrix.m[0][2] = m.m[0][1] * m.m[1][2] - m.m[0][2] * m.m[1][1];

	adjugateMatrix.m[1][0] = -(m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0]);
	adjugateMatrix.m[1][1] = m.m[0][0] * m.m[2][2] - m.m[0][2] * m.m[2][0];
	adjugateMatrix.m[1][2] = -(m.m[0][0] * m.m[1][2] - m.m[0][2] * m.m[1][0]);

	adjugateMatrix.m[2][0] = m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0];
	adjugateMatrix.m[2][1] = -(m.m[0][0] * m.m[2][1] - m.m[0][1] * m.m[2][0]);
	adjugateMatrix.m[2][2] = m.m[0][0] * m.m[1][1] - m.m[0][1] * m.m[1][0];

	for (uint32_t i = 0; i < 3; i++)
	{
		for (uint32_t j = 0; j < 3; j++)
		{
			adjugateMatrix.m[i][j] *= 1.0f / determinant;
		}
	}

	return adjugateMatrix;
}

/// <summary>
/// 平行投影行列を作る
/// </summary>
/// <param name="left">左</param>
/// <param name="top">上</param>
/// <param name="right">右</param>
/// <param name="bottom">下</param>
/// <returns></returns>
Matrix3x3 MakeOrthographicMatrix(float left, float top, float right, float bottom)
{
	// 正射影行列
	Matrix3x3 projectionMatrix;

	projectionMatrix.m[0][0] = 2.0f / (right - left);
	projectionMatrix.m[0][1] = 0.0f;
	projectionMatrix.m[0][2] = 0.0f;

	projectionMatrix.m[1][0] = 0.0f;
	projectionMatrix.m[1][1] = 2.0f / (top - bottom);
	projectionMatrix.m[1][2] = 0.0f;

	projectionMatrix.m[2][0] = (left + right) / (left - right);
	projectionMatrix.m[2][1] = (top + bottom) / (bottom - top);
	projectionMatrix.m[2][2] = 1.0f;

	return projectionMatrix;
}

/// <summary>
/// ビューポート変換行列を作る
/// </summary>
/// <param name="left">左</param>
/// <param name="top">上</param>
/// <param name="width">幅</param>
/// <param name="height">高さ</param>
/// <returns></returns>
Matrix3x3 MakeViewportMatrix(float left, float top, float width, float height)
{
	// ビューポート変換行列
	Matrix3x3 viewportMatrix;

	viewportMatrix.m[0][0] = width / 2.0f;
	viewportMatrix.m[0][1] = 0.0f;
	viewportMatrix.m[0][2] = 0.0f;

	viewportMatrix.m[1][0] = 0.0f;
	viewportMatrix.m[1][1] = -(height / 2.0f);
	viewportMatrix.m[1][2] = 0.0f;

	viewportMatrix.m[2][0] = left + width / 2.0f;
	viewportMatrix.m[2][1] = top + height / 2.0f;
	viewportMatrix.m[2][2] = 1.0f;

	return viewportMatrix;
}

/// <summary>
/// 単位行列を作る
/// </summary>
/// <returns></returns>
Matrix3x3 MakeIdenityMatrix()
{
	// 単位行列
	Matrix3x3 idenityMatrix;

	for (uint32_t i = 0; i < 3; i++)
	{
		for (uint32_t j = 0; j < 3; j++)
		{
			if (i == j)
			{
				idenityMatrix.m[i][j] = 1.0f;
			}
			else
			{
				idenityMatrix.m[i][j] = 0.0f;
			}
		}
	}

	return idenityMatrix;
}

/// <summary>
/// 転置行列を作る
/// </summary>
/// <param name="m">行列</param>
/// <returns></returns>
Matrix3x3 MakeTransposeMatrix(const Matrix3x3& m)
{
	// 転置行列
	Matrix3x3 transposeMatrix;
	
	transposeMatrix.m[0][0] = m.m[0][0];
	transposeMatrix.m[0][1] = m.m[1][0];
	transposeMatrix.m[0][2] = m.m[2][0];

	transposeMatrix.m[1][0] = m.m[0][1];
	transposeMatrix.m[1][1] = m.m[1][1];
	transposeMatrix.m[1][2] = m.m[2][1];

	transposeMatrix.m[2][0] = m.m[0][2];
	transposeMatrix.m[2][1] = m.m[1][2];
	transposeMatrix.m[2][2] = m.m[2][2];

	return transposeMatrix;
}