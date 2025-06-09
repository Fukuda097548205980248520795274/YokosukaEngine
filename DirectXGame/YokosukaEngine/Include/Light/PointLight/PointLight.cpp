#include "PointLight.h"

/// <summary>
/// 初期化
/// </summary>
void PointLight::Initialize()
{
	// 色
	color_ = { 1.0f , 1.0f , 1.0f , 1.0f };

	// 位置
	position_ = { 0.0f , 0.0f , 0.0f };

	// 輝度
	intensity_ = 1.0f;

	// ライトの届く距離
	radius_ = 3.0f;

	// 減衰率
	decay_ = 10.0f;
}