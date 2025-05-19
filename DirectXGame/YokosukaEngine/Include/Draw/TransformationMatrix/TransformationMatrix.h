#pragma once
#include "../../Math/Matrix4x4/Matrix4x4.h"


typedef struct TransformationMatrix
{
	Matrix4x4 worldViewProjection;
	Matrix4x4 world;
}TransformationMatrix;