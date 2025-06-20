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
struct PointLightFoirGPU
{
	// 色
	Vector4 color[1024];

	// 位置
	Vector4 position[1024];

	// ライトの届く最大距離
	float radius[1024];

	// 減衰率
	float decay[1024];

	// 設置したライトの数
	uint32_t setNum;
};