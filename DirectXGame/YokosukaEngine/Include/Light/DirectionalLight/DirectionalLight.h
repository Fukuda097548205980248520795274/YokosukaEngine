#pragma once
#include "../../Math/Vector4/Vector4.h"
#include "../../Math/Vector3/Vector3.h"

// 平行光源
class DirectionalLight
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	// 色
	Vector4 color_;

	// 向き
	Vector3 direction_;

	// 輝度
	float intensity_;
};

// GPUに送る平行光源
struct DirectionalLightForGPU
{
	// 色
	Vector4 color[1024];

	// 向き
	Vector4 direction[1024];

	// 配置したライト数
	uint32_t setNum = 0;
};