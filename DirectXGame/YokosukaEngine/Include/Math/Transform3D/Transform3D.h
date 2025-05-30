#pragma once
#include "../Vector3/Vector3.h"

struct Transform3D
{
	// 拡縮
	Vector3 scale;

	// 回転
	Vector3 rotation;

	// 移動
	Vector3 translation;
};