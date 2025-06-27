#include "Matrix4x4.h"

/// <summary>
/// 座標変換を行う
/// </summary>
/// <param name="vector">ベクトル</param>
/// <param name="matrix">行列</param>
/// <returns>座標変換したベクトル</returns>
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix)
{
	// 座標変換したベクトル
	Vector3 transform;

	transform.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	transform.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	transform.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];

	assert(w != 0.0f);

	transform /= w;

	return transform;
}

/// <summary>
/// 座標変換を行う
/// </summary>
/// <param name="vector">ベクトル</param>
/// <param name="matrix">行列</param>
/// <returns>座標変換したベクトル</returns>
Vector4 Transform(const Vector4& vector, const Matrix4x4& matrix)
{
	// 座標変換したベクトル
	Vector4 transform;

	transform.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + vector.w * matrix.m[3][0];
	transform.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + vector.w * matrix.m[3][1];
	transform.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + vector.w * matrix.m[3][2];
	transform.w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + vector.w * matrix.m[3][3];

	assert(transform.w != 0.0f);

	transform /= transform.w;

	return transform;
}

/// <summary>
/// 法線の座標変換
/// </summary>
/// <param name="vector"></param>
/// <param name="matrix"></param>
/// <returns></returns>
Vector3 TransformNormal(const Vector3& vector, const Matrix4x4 matrix)
{
	// 座標変換したベクトル
	Vector3 transform;

	transform.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0];
	transform.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1];
	transform.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2];

	return transform;
}

/// <summary>
/// 拡大縮小行列を作る
/// </summary>
/// <param name="scale">拡縮</param>
/// <returns>拡大縮小行列</returns>
Matrix4x4 MakeScaleMatrix(const Vector3& scale)
{
	// 拡大縮小行列
	Matrix4x4 scaleMatrix;

	scaleMatrix.m[0][0] = scale.x;
	scaleMatrix.m[0][1] = 0.0f;
	scaleMatrix.m[0][2] = 0.0f;
	scaleMatrix.m[0][3] = 0.0f;

	scaleMatrix.m[1][0] = 0.0f;
	scaleMatrix.m[1][1] = scale.y;
	scaleMatrix.m[1][2] = 0.0f;
	scaleMatrix.m[1][3] = 0.0f;

	scaleMatrix.m[2][0] = 0.0f;
	scaleMatrix.m[2][1] = 0.0f;
	scaleMatrix.m[2][2] = scale.z;
	scaleMatrix.m[2][3] = 0.0f;
	
	scaleMatrix.m[3][0] = 0.0f;
	scaleMatrix.m[3][1] = 0.0f;
	scaleMatrix.m[3][2] = 0.0f;
	scaleMatrix.m[3][3] = 1.0f;

	return scaleMatrix;
}

/// <summary>
/// X軸の回転行列を作る
/// </summary>
/// <param name="radian">ラジアン</param>
/// <returns>X軸の回転行列</returns>
Matrix4x4 MakeRotateXMatrix(float radian)
{
	// X軸の回転行列
	Matrix4x4 rotateXMatrix;

	rotateXMatrix.m[0][0] = 1.0f;
	rotateXMatrix.m[0][1] = 0.0f;
	rotateXMatrix.m[0][2] = 0.0f;
	rotateXMatrix.m[0][3] = 0.0f;

	rotateXMatrix.m[1][0] = 0.0f;
	rotateXMatrix.m[1][1] = std::cos(radian);
	rotateXMatrix.m[1][2] = std::sin(radian);
	rotateXMatrix.m[1][3] = 0.0f;

	rotateXMatrix.m[2][0] = 0.0f;
	rotateXMatrix.m[2][1] = -std::sin(radian);
	rotateXMatrix.m[2][2] = std::cos(radian);
	rotateXMatrix.m[2][3] = 0.0f;

	rotateXMatrix.m[3][0] = 0.0f;
	rotateXMatrix.m[3][1] = 0.0f;
	rotateXMatrix.m[3][2] = 0.0f;
	rotateXMatrix.m[3][3] = 1.0f;

	return rotateXMatrix;
}

/// <summary>
/// Y軸の回転行列を作る
/// </summary>
/// <param name="radian">ラジアン</param>
/// <returns>Y軸の回転行列</returns>
Matrix4x4 MakeRotateYMatrix(float radian)
{
	// Y軸の回転行列
	Matrix4x4 rotateYMatrix;

	rotateYMatrix.m[0][0] = std::cos(radian);
	rotateYMatrix.m[0][1] = 0.0f;
	rotateYMatrix.m[0][2] = -std::sin(radian);
	rotateYMatrix.m[0][3] = 0.0f;

	rotateYMatrix.m[1][0] = 0.0f;
	rotateYMatrix.m[1][1] = 1.0f;
	rotateYMatrix.m[1][2] = 0.0f;
	rotateYMatrix.m[1][3] = 0.0f;

	rotateYMatrix.m[2][0] = std::sin(radian);
	rotateYMatrix.m[2][1] = 0.0f;
	rotateYMatrix.m[2][2] = std::cos(radian);
	rotateYMatrix.m[2][3] = 0.0f;

	rotateYMatrix.m[3][0] = 0.0f;
	rotateYMatrix.m[3][1] = 0.0f;
	rotateYMatrix.m[3][2] = 0.0f;
	rotateYMatrix.m[3][3] = 1.0f;

	return rotateYMatrix;
}

/// <summary>
/// Z軸の回転行列を作る
/// </summary>
/// <param name="radian">ラジアン</param>
/// <returns>Z軸の回転行列</returns>
Matrix4x4 MakeRotateZMatrix(float radian)
{
	// Z軸の回転行列
	Matrix4x4 rotateZMatrix;

	rotateZMatrix.m[0][0] = std::cos(radian);
	rotateZMatrix.m[0][1] = std::sin(radian);
	rotateZMatrix.m[0][2] = 0.0f;
	rotateZMatrix.m[0][3] = 0.0f;

	rotateZMatrix.m[1][0] = -std::sin(radian);
	rotateZMatrix.m[1][1] = std::cos(radian);
	rotateZMatrix.m[1][2] = 0.0f;
	rotateZMatrix.m[1][3] = 0.0f;

	rotateZMatrix.m[2][0] = 0.0f;
	rotateZMatrix.m[2][1] = 0.0f;
	rotateZMatrix.m[2][2] = 1.0f;
	rotateZMatrix.m[2][3] = 0.0f;

	rotateZMatrix.m[3][0] = 0.0f;
	rotateZMatrix.m[3][1] = 0.0f;
	rotateZMatrix.m[3][2] = 0.0f;
	rotateZMatrix.m[3][3] = 1.0f;

	return rotateZMatrix;
}

/// <summary>
/// 回転行列を作る
/// </summary>
/// <param name="rotation">回転</param>
/// <returns>回転行列</returns>
Matrix4x4 MakeRotateMatrix(const Vector3& rotation)
{
	// 回転行列
	Matrix4x4 rotateMatrix;
	rotateMatrix = MakeRotateXMatrix(rotation.x) * MakeRotateYMatrix(rotation.y) * MakeRotateZMatrix(rotation.z);
	return rotateMatrix;
}

/// <summary>
/// 平行移動行列を作る
/// </summary>
/// <param name="translation">移動</param>
/// <returns>平行移動行列</returns>
Matrix4x4 MakeTranslateMatrix(const Vector3& translation)
{
	// 平行移動行列
	Matrix4x4 translateMatrix;

	translateMatrix.m[0][0] = 1.0f;
	translateMatrix.m[0][1] = 0.0f;
	translateMatrix.m[0][2] = 0.0f;
	translateMatrix.m[0][3] = 0.0f;

	translateMatrix.m[1][0] = 0.0f;
	translateMatrix.m[1][1] = 1.0f;
	translateMatrix.m[1][2] = 0.0f;
	translateMatrix.m[1][3] = 0.0f;

	translateMatrix.m[2][0] = 0.0f;
	translateMatrix.m[2][1] = 0.0f;
	translateMatrix.m[2][2] = 1.0f;
	translateMatrix.m[2][3] = 0.0f;

	translateMatrix.m[3][0] = translation.x;
	translateMatrix.m[3][1] = translation.y;
	translateMatrix.m[3][2] = translation.z;
	translateMatrix.m[3][3] = 1.0f;

	return translateMatrix;
}

/// <summary>
/// アフィン変換行列を作る
/// </summary>
/// <param name="scale">拡縮</param>
/// <param name="rotation">回転</param>
/// <param name="translation">移動</param>
/// <returns>アフィン変換行列</returns>
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotation, const Vector3& translation)
{
	// アフィン変換行列
	Matrix4x4 affineMatrix;
	affineMatrix = MakeScaleMatrix(scale) * MakeRotateMatrix(rotation) * MakeTranslateMatrix(translation);
	return affineMatrix;
}

/// <summary>
/// 逆行列を作る
/// </summary>
/// <param name="m">行列</param>
/// <returns>逆行列</returns>
Matrix4x4 MakeInverseMatrix(const Matrix4x4& m)
{
	// 行列式
	float determinant =
		m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2] -
		m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2] -

		m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2] +
		m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2] +

		m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2] -
		m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2] -

		m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0] +
		m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];

	// 余因子行列
	Matrix4x4 adjugateMatrix;

	adjugateMatrix.m[0][0] =
		m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[1][3] * m.m[2][1] * m.m[3][2] -
		m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2];

	adjugateMatrix.m[0][1] =
		-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[2][1] * m.m[3][2] +
		m.m[0][3] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2];

	adjugateMatrix.m[0][2] =
		m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[3][2] -
		m.m[0][3] * m.m[1][2] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2];

	adjugateMatrix.m[0][3] =
		-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] - m.m[0][3] * m.m[1][1] * m.m[2][2] +
		m.m[0][3] * m.m[1][2] * m.m[2][1] + m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2];


	adjugateMatrix.m[1][0] =
		-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[1][3] * m.m[2][0] * m.m[3][2] +
		m.m[1][3] * m.m[2][2] * m.m[3][0] + m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2];

	adjugateMatrix.m[1][1] =
		m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] + m.m[0][3] * m.m[2][0] * m.m[3][2] -
		m.m[0][3] * m.m[2][2] * m.m[3][0] - m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2];

	adjugateMatrix.m[1][2] =
		-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] - m.m[0][3] * m.m[1][0] * m.m[3][2] +
		m.m[0][3] * m.m[1][2] * m.m[3][0] + m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2];

	adjugateMatrix.m[1][3] =
		m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] + m.m[0][3] * m.m[1][0] * m.m[2][2] -
		m.m[0][3] * m.m[1][2] * m.m[2][0] - m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2];


	adjugateMatrix.m[2][0] =
		m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[1][3] * m.m[2][0] * m.m[3][1] -
		m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1];

	adjugateMatrix.m[2][1] =
		-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] - m.m[0][3] * m.m[2][0] * m.m[3][1] +
		m.m[0][3] * m.m[2][1] * m.m[3][0] + m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1];

	adjugateMatrix.m[2][2] =
		m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] + m.m[0][3] * m.m[1][0] * m.m[3][1] -
		m.m[0][3] * m.m[1][1] * m.m[3][0] - m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1];

	adjugateMatrix.m[2][3] =
		-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] - m.m[0][3] * m.m[1][0] * m.m[2][1] +
		m.m[0][3] * m.m[1][1] * m.m[2][0] + m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1];


	adjugateMatrix.m[3][0] =
		-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] - m.m[1][2] * m.m[2][0] * m.m[3][1] +
		m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1];

	adjugateMatrix.m[3][1] =
		m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] + m.m[0][2] * m.m[2][0] * m.m[3][1] -
		m.m[0][2] * m.m[2][1] * m.m[3][0] - m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1];

	adjugateMatrix.m[3][2] =
		-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] - m.m[0][2] * m.m[1][0] * m.m[3][1] +
		m.m[0][2] * m.m[1][1] * m.m[3][0] + m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1];

	adjugateMatrix.m[3][3] =
		m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] + m.m[0][2] * m.m[1][0] * m.m[2][1] -
		m.m[0][2] * m.m[1][1] * m.m[2][0] - m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1];


	for (uint32_t i = 0; i < 4; i++)
	{
		for (uint32_t j = 0; j < 4; j++)
		{
			adjugateMatrix.m[i][j] *= 1.0f / determinant;
		}
	}

	return adjugateMatrix;
}

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
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip)
{
	// 平行投影行列
	Matrix4x4 orthographicMatrix;

	orthographicMatrix.m[0][0] = 2.0f / (right - left);
	orthographicMatrix.m[0][1] = 0.0f;
	orthographicMatrix.m[0][2] = 0.0f;
	orthographicMatrix.m[0][3] = 0.0f;

	orthographicMatrix.m[1][0] = 0.0f;
	orthographicMatrix.m[1][1] = 2.0f / (top - bottom);
	orthographicMatrix.m[1][2] = 0.0f;
	orthographicMatrix.m[1][3] = 0.0f;

	orthographicMatrix.m[2][0] = 0.0f;
	orthographicMatrix.m[2][1] = 0.0f;
	orthographicMatrix.m[2][2] = 1.0f / (farClip - nearClip);
	orthographicMatrix.m[2][3] = 0.0f;

	orthographicMatrix.m[3][0] = (left + right) / (left - right);
	orthographicMatrix.m[3][1] = (top + bottom) / (bottom - top);
	orthographicMatrix.m[3][2] = nearClip / (nearClip - farClip);
	orthographicMatrix.m[3][3] = 1.0f;

	return orthographicMatrix;
}

/// <summary>
/// 透視投影行列を作る
/// </summary>
/// <param name="fovY">画角</param>
/// <param name="aspectRatio">アスペクト比</param>
/// <param name="nearClip">近平面の距離</param>
/// <param name="farClip">遠平面の距離</param>
/// <returns>透視投影行列</returns>
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip)
{
	// 透視投影行列
	Matrix4x4 perspectiveFovMatrix;

	perspectiveFovMatrix.m[0][0] = (1.0f / aspectRatio) * (1.0f / tanf(fovY / 2.0f));
	perspectiveFovMatrix.m[0][1] = 0.0f;
	perspectiveFovMatrix.m[0][2] = 0.0f;
	perspectiveFovMatrix.m[0][3] = 0.0f;

	perspectiveFovMatrix.m[1][0] = 0.0f;
	perspectiveFovMatrix.m[1][1] = 1.0f / tanf(fovY / 2.0f);
	perspectiveFovMatrix.m[1][2] = 0.0f;
	perspectiveFovMatrix.m[1][3] = 0.0f;

	perspectiveFovMatrix.m[2][0] = 0.0f;
	perspectiveFovMatrix.m[2][1] = 0.0f;
	perspectiveFovMatrix.m[2][2] = farClip / (farClip - nearClip);
	perspectiveFovMatrix.m[2][3] = 1.0f;

	perspectiveFovMatrix.m[3][0] = 0.0f;
	perspectiveFovMatrix.m[3][1] = 0.0f;
	perspectiveFovMatrix.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);
	perspectiveFovMatrix.m[3][3] = 0.0f;

	return perspectiveFovMatrix;
	
}

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
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth)
{
	// ビューポート変換行列
	Matrix4x4 viewportMatrix;

	viewportMatrix.m[0][0] = width / 2.0f;
	viewportMatrix.m[0][1] = 0.0f;
	viewportMatrix.m[0][2] = 0.0f;
	viewportMatrix.m[0][3] = 0.0f;

	viewportMatrix.m[1][0] = 0.0f;
	viewportMatrix.m[1][1] = -(height / 2.0f);
	viewportMatrix.m[1][2] = 0.0f;
	viewportMatrix.m[1][3] = 0.0f;

	viewportMatrix.m[2][0] = 0.0f;
	viewportMatrix.m[2][1] = 0.0f;
	viewportMatrix.m[2][2] = maxDepth - minDepth;
	viewportMatrix.m[2][3] = 0.0f;

	viewportMatrix.m[3][0] = left + width / 2.0f;
	viewportMatrix.m[3][1] = top + height / 2.0f;
	viewportMatrix.m[3][2] = minDepth;
	viewportMatrix.m[3][3] = 1.0f;

	return viewportMatrix;
}

/// <summary>
/// 単位行列を作る
/// </summary>
/// <returns>単位行列</returns>
Matrix4x4 MakeIdenityMatirx()
{
	// 単位行列
	Matrix4x4 idenityMatrix;

	for (uint32_t i = 0; i < 4; i++)
	{
		for (uint32_t j = 0; j < 4; j++)
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
Matrix4x4 MakeTransposeMatrix(Matrix4x4 m)
{
	// 転置行列
	Matrix4x4 transposeMatrix;

	transposeMatrix.m[0][0] = m.m[0][0];
	transposeMatrix.m[0][1] = m.m[1][0];
	transposeMatrix.m[0][2] = m.m[2][0];
	transposeMatrix.m[0][3] = m.m[3][0];
	
	transposeMatrix.m[1][0] = m.m[0][1];
	transposeMatrix.m[1][1] = m.m[1][1];
	transposeMatrix.m[1][2] = m.m[2][1];
	transposeMatrix.m[1][3] = m.m[3][1];

	transposeMatrix.m[2][0] = m.m[0][2];
	transposeMatrix.m[2][1] = m.m[1][2];
	transposeMatrix.m[2][2] = m.m[2][2];
	transposeMatrix.m[2][3] = m.m[3][2];

	transposeMatrix.m[3][0] = m.m[0][3];
	transposeMatrix.m[3][1] = m.m[1][3];
	transposeMatrix.m[3][2] = m.m[2][3];
	transposeMatrix.m[3][3] = m.m[3][3];

	return transposeMatrix;
}


Vector3 Project(const Vector3& worldPosition, float viewportX, float viewportY, float viewportWidth, float viewportHeight,
	const Matrix4x4& viewMatrix , const Matrix4x4& projectionMatrix)
{
	Matrix4x4 viewportMatrix = MakeViewportMatrix(viewportX, viewportY, viewportWidth, viewportHeight, 0.0f, 100.0f);

	return Transform(worldPosition, viewMatrix * projectionMatrix * viewportMatrix);
}