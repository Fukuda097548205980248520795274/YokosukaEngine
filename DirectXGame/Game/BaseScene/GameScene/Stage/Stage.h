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
	/// 敵のスクリプトを読み込む
	/// </summary>
	/// <param name="filePath"></param>
	void LoadEnemyScript(const char* filePath);

	/// <summary>
	/// ステージオブジェクトのスクリプトを読み込む
	/// </summary>
	/// <param name="filePath"></param>
	void LoadStageObjectScript(const char* filePath);





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

	/// <summary>
	/// 敵を生成する
	/// </summary>
	void SummonEnemy(std::string& enemyType, int32_t hp, const Vector3& position);

	/// <summary>
	/// ステージオブジェクトを生成する
	/// </summary>
	void SummonStageObject(std::string& enemyType, const Vector3& position, const Vector3& rotation);


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


	// クリアフラグ
	bool isClear_ = false;


	// 中心軸
	std::unique_ptr<CenterAxis> centerAxis_ = nullptr;

	// ステージオブジェクトのリスト
	std::list<std::unique_ptr<BaseStageObject>> stageObjects_;


	/*-----------------
	    敵スクリプト
	-----------------*/

	/// <summary>
	/// 敵スクリプトの更新処理
	/// </summary>
	void EnemyScriptUpdate();

	// 敵の文字列ストリーム
	std::stringstream enemyStream_;

	// 待機フラグ
	bool isWaitEnemyScript_ = false;

	// 待機タイマー
	float waitEnemyScriptTimer_ = 0.0f;


	/*--------------------------------
	    ステージオブジェクトスクリプト
	--------------------------------*/

	/// <summary>
	/// ステージオブジェクトスクリプトの更新処理
	/// </summary>
	void StageObjectScriptUpdate();

	// ステージオブジェクトの文字列ストリーム
	std::stringstream stageObjectStream_;

	// 待機フラグ
	bool isWaitStageObjectScript_ = false;

	// 待機タイマー
	float waitStageObjectScriptTimer_ = 0.0f;
};

