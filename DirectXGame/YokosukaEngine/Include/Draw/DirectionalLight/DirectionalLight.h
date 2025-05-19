#pragma once
#include "../../Math/Vector4/Vector4.h"
#include "../../Math/Vector3/Vector3.h"

// 平行光源
typedef struct DirectionalLight
{
	// 色
	Vector4 color;

	// 向き
	Vector3 direction;

	// 輝度
	float intensity;

}DirectionalLight;