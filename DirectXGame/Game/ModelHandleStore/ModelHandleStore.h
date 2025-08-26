#pragma once
#include "../../YokosukaEngine/Include/YokosukaEngine.h"

#include "BaseModelHandles/BaseModelHandles.h"

#include "BaseModelHandles/PlayerModelHandles/PlayerModelHandles.h"

#include "BaseModelHandles/PlayerBulletWeekModelHandles/PlayerBulletWeekModelHandles.h"
#include "BaseModelHandles/PlayerBulletStrongModelHandles/PlayerBulletStrongModelHandles.h"

#include "BaseModelHandles/PlayerHUDStateAreaModelHandle/PlayerHUDStateAreaModelHandle.h"
#include "BaseModelHandles/PlayerHUDHpModelHandle/PlayerHUDHpModelHandle.h"
#include "BaseModelHandles/PlayerHUDBulletEnergyModelHandle/PlayerHUDBulletEnergyModelHandle.h"
#include "BaseModelHandles/PlayerHUDTextHpModelHandle/PlayerHUDTextHpModelHandle.h"

#include "BaseModelHandles/EnemyButterflyModelHandles/EnemyButterflyodelHandles.h"
#include "BaseModelHandles/EnemyBirdModelHandles/EnemyBirdModelHandles.h"
#include "BaseModelHandles/EnemyStingrayModelHandles/EnemyStingrayModelHandles.h"
#include "BaseModelHandles/EnemyJetModelHandles/EnemyJetModelHandles.h"
#include "BaseModelHandles/EnemyFairyModelHandles/EnemyFairyModelHandles.h"
#include "BaseModelHandles/EnemyDevilModelHandles/EnemyDevilModelHandles.h"

#include "BaseModelHandles/EnemyBulletWeekModelHandles/EnemyBulletWeekModelHandles.h"
#include "BaseModelHandles/EnemyBulletStarModelHandle/EnemyBulletStarModelHandle.h"
#include "BaseModelHandles/EnemyBulletTridentModelHandle/EnemyBulletTridentModelHandle.h"

#include "BaseModelHandles/StageObjectSaturnPlanetModelHandles/StageObjectSaturnPlanetModelHandles.h"
#include "BaseModelHandles/StageObjectBuildingAModelHandles/StageObjectBuildingAModelHandles.h"
#include "BaseModelHandles/StageObjectBuildingBModelHandles/StageObjectBuildingBModelHandles.h"
#include "BaseModelHandles/StageObjectBuildingCModelHandles/StageObjectBuildingCModelHandles.h"
#include "BaseModelHandles/StageObjectBuildingDModelHandles/StageObjectBuildingDModelHandles.h"

#include "BaseModelHandles/StageObjectRainbowModelHandle/StageObjectRainbowModelHandle.h"
#include "BaseModelHandles/StageObjectHouseModelHandle/StageObjectHouseModelHandle.h"
#include "BaseModelHandles/StageObjectTreeModelHandle/StageObjectTreeModelHandle.h"

#include "BaseModelHandles/UniverseModelHandle/UniverseModelHandle.h"

class ModelHandleStore
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	void Initialize(const YokosukaEngine* engine);


	// ハンドルネーム
	enum HandleName
	{
		kPlayer,

		kPlayerBulletWeek,
		kPlayerBulletStrong,

		kPlayerHUDStateArea,
		kPlayerHUDHp,
		kPlayerHUDBulletEnergy,
		kPlayerHUDTextHp,

		kEnemyButterfly,
		kEnemyBird,
		kEnemyStingray,
		kEnemyJet,
		kEnemyFairy,
		kEnemyDevil,

		kEnemyBulletWeek,
		kEnemyBulletTrident,
		kEnemyBulletStar,

		kStageObjectSaturnPlanet,
		kStageObjectBuildingA,
		kStageObjectBuildingB,
		kStageObjectBuildingC,
		kStageObjectBuildingD,

		kStageObjectRainbow,
		kStageObjectHouse,
		kStageObjectTree,

		kUniverse,

		kNumHandleName
	};

	/// <summary>
	/// モデルハンドルのGetter
	/// </summary>
	/// <param name="handleName"></param>
	/// <returns></returns>
	std::vector<uint32_t> GetModelHandle(HandleName handleName) const { return modelHandles_[handleName]->GetModelHandles(); }


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// モデルハンドル
	std::unique_ptr<BaseModelHandles> modelHandles_[kNumHandleName];
};

