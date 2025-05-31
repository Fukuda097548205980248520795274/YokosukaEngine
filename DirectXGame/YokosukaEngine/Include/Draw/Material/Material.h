#pragma once
#include <stdint.h>
#include "../../Math/Vector4/Vector4.h"
#include "../../Math/Matrix4x4/Matrix4x4.h"


typedef struct Material
{
	// 色
	Vector4 color;

	// ライティング有効化
	int32_t  enableLighting;

	// パディング
	float padding[3];

	//  UVトランスフォーム
	Matrix4x4 uvTransform;

	// 光沢度
	float shininess;

}Material;