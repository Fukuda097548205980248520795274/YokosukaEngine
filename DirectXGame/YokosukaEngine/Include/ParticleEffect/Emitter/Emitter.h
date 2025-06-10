#pragma once
#include "../../Math/Transform3D/Transform3D.h"
#include "../../Math/Vector3/Vector3.h"

// 発生源
struct Emitter
{
	// トランスフォーム
	Transform3D transform;

	// 発生数
	uint32_t count;

	// 発生頻度
	float frequency;

	// 発生頻度のための時間
	float frequencyTime;
};