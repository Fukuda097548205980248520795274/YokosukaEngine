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


	// プレイヤーのハンドル
	modelHandles_[kPlayer] = std::make_unique<PlayerModelHandles>();
	modelHandles_[kPlayer]->Initialize(engine_);

	// プレイヤーのハンドル
	modelHandles_[kPlayerBulletWeek] = std::make_unique<PlayerBulletWeekModelHandles>();
	modelHandles_[kPlayerBulletWeek]->Initialize(engine_);

	// プレイヤーのハンドル
	modelHandles_[kPlayerBulletStrong] = std::make_unique<PlayerBulletStrongModelHandles>();
	modelHandles_[kPlayerBulletStrong]->Initialize(engine_);

	// 敵 : 蝶
	modelHandles_[kEnemyButterfly] = std::make_unique<EnemyButterflyodelHandles>();
	modelHandles_[kEnemyButterfly]->Initialize(engine_);

	// 敵 : 鳥
	modelHandles_[kEnemyBird] = std::make_unique<EnemyBirdModelHandles>();
	modelHandles_[kEnemyBird]->Initialize(engine_);

	// プレイヤーのハンドル
	modelHandles_[kEnemyBulletWeek] = std::make_unique<EnemyBulletWeekModelHandles>();
	modelHandles_[kEnemyBulletWeek]->Initialize(engine_);

	// ステージオブジェクトの土星のモデルハンドル
	modelHandles_[kStageObjectSaturnPlanet] = std::make_unique<StageObjectSaturnPlanetModelHandles>();
	modelHandles_[kStageObjectSaturnPlanet]->Initialize(engine_);

	// ステージオブジェクトのビルディングA
	modelHandles_[kStageObjectBuildingA] = std::make_unique<StageObjectBuildingAModelHandles>();
	modelHandles_[kStageObjectBuildingA]->Initialize(engine_);

	// ステージオブジェクトのビルディングB
	modelHandles_[kStageObjectBuildingB] = std::make_unique<StageObjectBuildingBModelHandles>();
	modelHandles_[kStageObjectBuildingB]->Initialize(engine_);

	// ステージオブジェクトのビルディングC
	modelHandles_[kStageObjectBuildingC] = std::make_unique<StageObjectBuildingCModelHandles>();
	modelHandles_[kStageObjectBuildingC]->Initialize(engine_);

	// ステージオブジェクトのビルディングD
	modelHandles_[kStageObjectBuildingD] = std::make_unique<StageObjectBuildingDModelHandles>();
	modelHandles_[kStageObjectBuildingD]->Initialize(engine_);
}