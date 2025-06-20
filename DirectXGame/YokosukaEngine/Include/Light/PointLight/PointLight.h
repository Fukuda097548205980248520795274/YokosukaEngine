#pragma once
#include "../../Math/Vector3/Vector3.h"
#include "../../Math/Vector4/Vector4.h"

// ポイントライト
class PointLight
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	// 色
	Vector4 color_;

	// 位置
	Vector3 position_;

	// 輝度
	float intensity_;

	// ライトの届く最大距離
	float radius_;

	// 減衰率
	float decay_;
};

// GPUに送るポイントライト
struct PointLightForGPU
{
	// 色
	Vector4 color;

	// 位置
	Vector3 position;

	// ライトの届く最大距離
	float radius;

	// 減衰率
	float decay;

};

// 使用しているポイントライトの数
struct UseNumPointLight
{
	uint32_t num;
};