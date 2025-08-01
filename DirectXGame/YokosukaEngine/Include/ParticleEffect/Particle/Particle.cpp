#include "Particle.h"

/// <summary>
/// 新規パーティクルを生成する
/// </summary>
/// <param name="randomEngine"></param>
/// <returns></returns>
Particle MakeNewParticle(std::mt19937& randomEngine , const Vector3& translation)
{
	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);

	Particle particle;
	particle.transform.scale = { 1.0f , 1.0f , 1.0f };
	particle.transform.rotation = { 0.0f , 0.0f , 0.0f };
	particle.transform.translation = { distribution(randomEngine),distribution(randomEngine), distribution(randomEngine) };
	particle.transform.translation += translation;
	particle.velocity = { distribution(randomEngine),distribution(randomEngine), distribution(randomEngine) };

	std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
	particle.color = { distColor(randomEngine),distColor(randomEngine), distColor(randomEngine), 1.0f };

	std::uniform_real_distribution<float> distTime(1.0f, 5.0f);
	particle.lifeTime = distTime(randomEngine);
	particle.currentTime = 0.0f;

	return particle;
}

/// <summary>
/// エミッターでパーティクルを発生させる
/// </summary>
/// <param name="emitter"></param>
/// <param name="randomEngine"></param>
/// <returns></returns>
std::list<Particle> Emit(const Emitter& emitter, std::mt19937& randomEngine)
{
	std::list<Particle> particles;
	for (uint32_t count = 0; count < emitter.count; count++)
	{
		particles.push_back(MakeNewParticle(randomEngine, emitter.transform.translation));
	}

	return particles;
}