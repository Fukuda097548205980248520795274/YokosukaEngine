#pragma once
#include <cmath>
#include <cassert>
#include "../Vector3/Vector3.h"
#include "../Vector4/Vector4.h"

// エンジン用の4x4行列
typedef struct Matrix4x4
{
	float m[4][4];
}Matrix4x4;

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
/// 行列の積を求める
/// </summary>
/// <param name="m1">行列1</param>
/// <param name="m2">行列2</param>
/// <returns>行列の積</returns>
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

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