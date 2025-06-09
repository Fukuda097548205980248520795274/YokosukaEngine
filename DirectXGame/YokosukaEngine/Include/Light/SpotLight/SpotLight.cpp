#include "SpotLight.h"

/// <summary>
/// 初期化
/// </summary>
void SpotLight::Initialize()
{
	// 色
	color_ = { 1.0f , 1.0f ,1.0f  ,1.0f };

	// 位置
	position_ = { 0.0f , 0.0f , 0.0f };

	// 向き
	direction_ = { 0.0f , -1.0f , 0.0f };

	// 輝度
	intensity_ = 4.0f;

	// ライトの届く最大距離
	distance_ = 7.0f;

	// 減衰率
	decay_ = 2.0f;

	// スポットライトの余弦
	cosAngle_ = 0.0f;

	// フォールアウト開始値
	fallofStart_ = std::cos(std::numbers::pi_v<float> / 3.0f);
}