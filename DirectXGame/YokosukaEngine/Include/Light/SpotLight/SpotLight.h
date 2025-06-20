#pragma once
#include <numbers>
#include "../../Math/Vector3/Vector3.h"
#include "../../Math/Vector4/Vector4.h"

// スポットライト
class SpotLight
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

	// 方向
	Vector3 direction_;

	// ライトの届く最大距離
	float distance_;

	// 減衰率
	float decay_;

	// スポットライトの余弦
	float cosAngle_;

	// フォールオフ開始値
	float fallofStart_;
};

// GPUに送るスポットライト
struct SpotLightForGPU
{
	// 色
	Vector4 color;

	// 位置
	Vector3 position;

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
	
};

// 使用しているスポットライトの数
struct UseNumSpotLight
{
	uint32_t num;
};