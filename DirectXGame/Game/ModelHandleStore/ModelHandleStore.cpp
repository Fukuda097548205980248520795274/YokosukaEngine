#include "ModelHandleStore.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void ModelHandleStore::Initialize(const YokosukaEngine* engine)
{
	// nullptrチェック
	assert(engine);

	// 引数を受け取る
	engine_ = engine;


	// プレイヤーのハンドルの生成と初期化
	modelHandles_[kPlayer] = std::make_unique<PlayerModelHandles>();
	modelHandles_[kPlayer]->Initialize(engine_);

	// プレイヤーのハンドルの生成と初期化
	modelHandles_[kPlayerBulletWeek] = std::make_unique<PlayerBulletWeekModelHandles>();
	modelHandles_[kPlayerBulletWeek]->Initialize(engine_);

	// プレイヤーのハンドルの生成と初期化
	modelHandles_[kPlayerBulletStrong] = std::make_unique<PlayerBulletStrongModelHandles>();
	modelHandles_[kPlayerBulletStrong]->Initialize(engine_);

	// プレイヤーのハンドルの生成と初期化
	modelHandles_[kEnemyButterfly] = std::make_unique<EnemyButterflyodelHandles>();
	modelHandles_[kEnemyButterfly]->Initialize(engine_);

	// プレイヤーのハンドルの生成と初期化
	modelHandles_[kEnemyBulletWeek] = std::make_unique<EnemyBulletWeekModelHandles>();
	modelHandles_[kEnemyBulletWeek]->Initialize(engine_);

	// ステージオブジェクトの土星のモデルハンドルの生成と初期化
	modelHandles_[kStageObjectSaturnPlanet] = std::make_unique<StageObjectSaturnPlanetModelHandles>();
	modelHandles_[kStageObjectSaturnPlanet]->Initialize(engine_);
}