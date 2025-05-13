#pragma once
#include <stdint.h>
#include "../../Math/Vector4/Vector4.h"

namespace Engine
{
	typedef struct Material
	{
		// 色
		Vector4 color;

		// ライティング有効化
		int32_t  enableLighting;

	}Material;
}