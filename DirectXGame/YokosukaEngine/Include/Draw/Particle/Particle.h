#pragma once
#include <random>
#include "../../Math/Transform3D/Transform3D.h"

struct Particle
{
	Transform3D transform;
	Vector3 velocity;
};

/// <summary>
/// 新規パーティクルを生成する
/// </summary>
/// <param name="randomEngine"></param>
/// <returns></returns>
Particle MakeNewParticle(std::mt19937& randomEngine);