#include "Particle.h"

/// <summary>
/// 新規パーティクルを生成する
/// </summary>
/// <param name="randomEngine"></param>
/// <returns></returns>
Particle MakeNewParticle(std::mt19937& randomEngine)
{
	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);

	Particle particle;
	particle.transform.scale = { 1.0f , 1.0f , 1.0f };
	particle.transform.rotation = { 0.0f , 0.0f , 0.0f };
	particle.transform.translation = { distribution(randomEngine),distribution(randomEngine), distribution(randomEngine) };
	particle.velocity = { distribution(randomEngine),distribution(randomEngine), distribution(randomEngine) };

	return particle;
}