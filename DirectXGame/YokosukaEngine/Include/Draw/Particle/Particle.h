#pragma once
#include <random>
#include <list>
#include "../../Math/Transform3D/Transform3D.h"
#include "../../Math/Vector4/Vector4.h"
#include "../../Math/Matrix4x4/Matrix4x4.h"

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

struct ParticleForGPU
{
	Matrix4x4 worldViewProjection;
	Matrix4x4 world;
	Vector4 color;
};

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