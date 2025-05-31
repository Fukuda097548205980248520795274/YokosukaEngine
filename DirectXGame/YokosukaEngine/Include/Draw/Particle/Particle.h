#pragma once
#include <random>
#include "../../Math/Transform3D/Transform3D.h"
#include "../../Math/Vector4/Vector4.h"
#include "../../Math/Matrix4x4/Matrix4x4.h"

struct Particle
{
	Transform3D transform;
	Vector3 velocity;
	Vector4 color;
	float lifeTime;
	float currentTime;
};

struct ParticleForGPU
{
	Matrix4x4 worldViewProjection;
	Matrix4x4 world;
	Vector4 color;
};

/// <summary>
/// 新規パーティクルを生成する
/// </summary>
/// <param name="randomEngine"></param>
/// <returns></returns>
Particle MakeNewParticle(std::mt19937& randomEngine);