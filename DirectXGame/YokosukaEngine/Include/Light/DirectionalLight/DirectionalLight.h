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
	Vector4 color;

	// 向き
	Vector3 direction;
};

// 使用している平行光源の数
struct UseNumDirectionalLight
{
	uint32_t num;
};