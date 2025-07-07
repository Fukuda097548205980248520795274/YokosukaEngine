#pragma once
#include "../Vector2/Vector2.h"

struct Transform2D
{
	// 拡縮
	Vector2 scale;

	// 回転
	float rotation;

	// 移動
	Vector2 translation;
};