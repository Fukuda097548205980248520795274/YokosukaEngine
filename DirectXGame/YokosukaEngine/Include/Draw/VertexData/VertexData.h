#pragma once
#include "../../Math/Vector4/Vector4.h"
#include "../../Math/Vector2/Vector2.h"
#include "../../Math/Vector3/Vector3.h"

// エンジン用の頂点情報
typedef struct VertexData
{
	// ローカル座標
	Vector4 position;

	// テクスチャ座標
	Vector2 texcoord;

	// 頂点法線
	Vector3 normal;
}VertexData;