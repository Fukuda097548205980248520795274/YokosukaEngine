#pragma once
#include <random>
#include <list>
#include "../Emitter/Emitter.h"
#include "../Feild/Feild.h"
#include "../../Math/Transform3D/Transform3D.h"
#include "../../Math/Vector4/Vector4.h"
#include "../../Math/Matrix4x4/Matrix4x4.h"

// パーティクル
struct Particle
{
	// トランスフォーム
	Transform3D transform;

	// 速度ベクトル
	Vector3 velocity;

	// 色
	Vector4 color;

	// 寿命
	float lifeTime;

	// 現在の寿命
	float currentTime;
};

// GPUに送るパーティクルのデータ
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
Particle MakeNewParticle(std::mt19937& randomEngine, const Vector3& translation);

/// <summary>
/// エミッターでパーティクルを発生させる
/// </summary>
/// <param name="emitter"></param>
/// <param name="randomEngine"></param>
/// <returns></returns>
std::list<Particle> Emit(const Emitter& emitter, std::mt19937& randomEngine);