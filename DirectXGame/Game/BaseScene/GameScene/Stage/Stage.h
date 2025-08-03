#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"
#include "../../../ModelHandleStore/ModelHandleStore.h"
#include <sstream>

#include "../BaseCharacter/BaseCharacter.h"

#include "CenterAxis/CenterAxis.h"

#include "BaseStageObject/StageObjectSaturnPlanet/StageObjectSaturnPlanet.h"

#include "BaseStageObject/StageObjectBuildingA/StageObjectBuildingA.h"
#include "BaseStageObject/StageObjectBuildingB/StageObjectBuildingB.h"
#include "BaseStageObject/StageObjectBuildingC/StageObjectBuildingC.h"
#include "BaseStageObject/StageObjectBuildingD/StageObjectBuildingD.h"

// 前方宣言
class GameScene;

class Stage
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d ,
		const ModelHandleStore* modelHandleStore, const float* gameFrame , GameScene* gameScene);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// ステージのスクリプトを読み込む
	/// </summary>
	/// <param name="filePath"></param>
	void LoadStageScript(const char* filePath);

	/// <summary>
	/// 中心軸のワールドトランスフォームのGetter
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetCenterAxisWorldTransform() const { return centerAxis_->GetWorldTransform(); }

	/// <summary>
	/// 中心軸のワールド座標のGetter
	/// </summary>
	/// <returns></returns>
	Vector3 GetCenterAxisWorldPosition() const { return centerAxis_->GetWorldPosition(); }

	/// <summary>
	/// 対象者のインスタンスのSetter
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(BaseCharacter* target) { target_ = target; }

	/// <summary>
	/// クリアフラグのGetter
	/// </summary>
	/// <returns></returns>
	bool IsClear()const { return isClear_; }


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// カメラ
	const Camera3D* camera3d_ = nullptr;

	// モデルハンドル格納場所
	const ModelHandleStore* modelHandleStore_ = nullptr;

	// ゲームフレーム
	const float* gameFrame_ = nullptr;

	// ゲームシーン
	GameScene* gameScene_ = nullptr;

	// 対象キャラ
	BaseCharacter* target_ = nullptr;

	/// <summary>
	/// ステージスクリプトの更新処理
	/// </summary>
	void StageScriptUpdate();


	// タイマー
	float timer_ = 0.0f;

	// クリアフラグ
	bool isClear_ = false;


	// 中心軸
	std::unique_ptr<CenterAxis> centerAxis_ = nullptr;


	// 文字列ストリーム
	std::stringstream stageStream_;


	// ステージオブジェクトのリスト
	std::list<std::unique_ptr<BaseStageObject>> stageObjects_;
};

