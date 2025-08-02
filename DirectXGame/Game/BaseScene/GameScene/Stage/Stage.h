#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"
#include "../../../ModelHandleStore/ModelHandleStore.h"
#include <sstream>

#include "BaseStageObject/StageObjectSaturnPlanet/StageObjectSaturnPlanet.h"

#include "BaseStageObject/StageObjectBuildingA/StageObjectBuildingA.h"
#include "BaseStageObject/StageObjectBuildingB/StageObjectBuildingB.h"
#include "BaseStageObject/StageObjectBuildingC/StageObjectBuildingC.h"
#include "BaseStageObject/StageObjectBuildingD/StageObjectBuildingD.h"

class Stage
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d ,const ModelHandleStore* modelHandleStore, const float* gameFrame);

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


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// カメラ
	const Camera3D* camera3d_ = nullptr;

	// モデルハンドル格納場所
	const ModelHandleStore* modelHandleStore_ = nullptr;

	// ゲームフレーム
	const float* gameFrame_ = nullptr;

	/// <summary>
	/// ステージスクリプトの更新処理
	/// </summary>
	void StageScriptUpdate();


	// 文字列ストリーム
	std::stringstream stageStream_;


	// ステージオブジェクトのリスト
	std::list<std::unique_ptr<BaseStageObject>> stageObjects_;
};

