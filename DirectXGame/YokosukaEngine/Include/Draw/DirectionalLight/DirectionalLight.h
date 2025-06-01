#pragma once
#include "../../Math/Vector4/Vector4.h"
#include "../../Math/Vector3/Vector3.h"

// 平行光源
struct DirectionalLight
{
	// 色
	Vector4 color;

	// 向き
	Vector3 direction;

	// 輝度
	float intensity;

};

// 点光源
struct PointLight
{
	// 色
	Vector4 color;

	// 位置
	Vector3 position;

	// 輝度
	float intensity;

	// ライトの届く最大距離
	float radius;

	// 減衰率
	float decay;

	float padding[1];

};

// スポットライト
struct SpotLight
{
	// 色
	Vector4 color;

	// 位置
	Vector3 position;

	// 輝度
	float intensity;

	// 方向
	Vector3 direction;

	// ライトの届く最大距離
	float distance;

	// 減衰率
	float decay;

	// スポットライトの余弦
	float cosAngle;

	// フォールオフ開始値
	float fallofStart;

	float padding[1];
};