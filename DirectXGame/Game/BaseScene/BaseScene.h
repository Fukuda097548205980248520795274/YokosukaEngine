#pragma once
#include "../../YokosukaEngine/Include/YokosukaEngine.h"
#include "../ModelHandleStore/ModelHandleStore.h"
#include "../TextureHandleStore/TextureHandleStore.h"

class BaseScene
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	virtual void Initialize(const YokosukaEngine* engine , const ModelHandleStore* modelHandleStore , const TextureHandleStore* textureHandleStore);

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 終了フラグのGetter
	/// </summary>
	bool IsFinished() { return isFinished_; }

	/// <summary>
	/// 敵のスクリプトのGetter
	/// </summary>
	/// <returns></returns>
	virtual std::string GetEnemyScriptPass() { std::string str = ""; return str; }

	/// <summary>
	/// ステージオブジェクトのスクリプトのGetter
	/// </summary>
	/// <returns></returns>
	virtual std::string GetStageObjectScriptPass() { std::string str = ""; return str; }

	/// <summary>
	/// ステージを生成する
	/// </summary>
	/// <param name="enemyScriptPass"></param>
	/// <param name="stageObjectScriptPass"></param>
	virtual void CreateStage(const std::string& enemyScriptPass, const std::string& stageObjectScriptPass) {};


protected:

	// モデルハンドル格納場所
	const ModelHandleStore* modelHandleStore_ = nullptr;

	// テクスチャハンドル格納場所
	const TextureHandleStore* textureHandleStore_ = nullptr;


	// シーン
	std::unique_ptr<Scene> scene_ = nullptr;

	// メインカメラ
	MainCamera* mainCamera_ = nullptr;

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// 3Dカメラ
	const Camera3D* camera3d_ = nullptr;

	// 2Dカメラ
	std::unique_ptr<Camera2D> camera2d_ = nullptr;


	// 終了フラグ
	bool isFinished_ = false;
};

