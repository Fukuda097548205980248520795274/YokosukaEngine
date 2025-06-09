#include "DirectionalLight.h"

/// <summary>
/// 初期化
/// </summary>
void DirectionalLight::Initialize()
{
	// 色
	color_ = { 1.0f , 1.0f , 1.0f ,1.0f };

	// 向き
	direction_ = { 0.0f , -1.0f , 0.0f };

	// 輝度
	intensity_ = 1.0f;
}