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

	// プレイヤーのHUD : ステートエリア
	modelHandles_[kPlayerHUDStateArea] = std::make_unique<PlayerHUDStateAreaModelHandle>();
	modelHandles_[kPlayerHUDStateArea]->Initialize(engine_);

	// プレイヤーのHUD : HP
	modelHandles_[kPlayerHUDHp] = std::make_unique<PlayerHUDHpModelHandle>();
	modelHandles_[kPlayerHUDHp]->Initialize(engine_);

	// プレイヤーのHUD : 弾エネルギー
	modelHandles_[kPlayerHUDBulletEnergy] = std::make_unique<PlayerHUDBulletEnergyModelHandle>();
	modelHandles_[kPlayerHUDBulletEnergy]->Initialize(engine_);

	// プレイヤーのHUD : HPのテキスト
	modelHandles_[kPlayerHUDTextHp] = std::make_unique<PlayerHUDTextHpModelHandle>();
	modelHandles_[kPlayerHUDTextHp]->Initialize(engine_);

	// 敵 : 蝶
	modelHandles_[kEnemyButterfly] = std::make_unique<EnemyButterflyodelHandles>();
	modelHandles_[kEnemyButterfly]->Initialize(engine_);

	// 敵 : 鳥
	modelHandles_[kEnemyBird] = std::make_unique<EnemyBirdModelHandles>();
	modelHandles_[kEnemyBird]->Initialize(engine_);

	// 敵 : エイ
	modelHandles_[kEnemyStingray] = std::make_unique<EnemyStingrayModelHandles>();
	modelHandles_[kEnemyStingray]->Initialize(engine_);

	// 敵 : 戦闘機
	modelHandles_[kEnemyJet] = std::make_unique<EnemyJetModelHandles>();
	modelHandles_[kEnemyJet]->Initialize(engine_);

	// 敵 : 妖精
	modelHandles_[kEnemyFairy] = std::make_unique<EnemyFairyModelHandles>();
	modelHandles_[kEnemyFairy]->Initialize(engine_);

	// 敵 : 悪魔
	modelHandles_[kEnemyDevil] = std::make_unique<EnemyDevilModelHandles>();
	modelHandles_[kEnemyDevil]->Initialize(engine_);

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