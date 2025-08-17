#pragma once
#include "../YokosukaEngine/Include/YokosukaEngine.h"
#include "BaseScene/GameScene/GameScene.h"
#include "BaseScene/TitleScene/TitleScene.h"
#include "BaseScene/ClearScene/ClearScene.h"
#include "BaseScene/StageSelectScene/StageSelectScene.h"
#include "ModelHandleStore/ModelHandleStore.h"
#include "TextureHandleStore/TextureHandleStore.h"

class Game
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine">エンジン</param>
	void Initialize(const YokosukaEngine* engine);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// モデルハンドルの格納場所
	std::unique_ptr<ModelHandleStore> modelHandleStore_ = nullptr;

	// テクスチャハンドルの格納場所
	std::unique_ptr<TextureHandleStore> textureHandleStore_ = nullptr;

	// シーン
	std::unique_ptr<BaseScene> scene_ = nullptr;


	/*-----------------
	    シーンフェーズ
	-----------------*/

	enum ScenePhase
	{
		kTitle,
		kStageSelect,
		kGame,
		kClear
	};

	// 現在のシーンフェーズ
	ScenePhase scenePhase_ = kTitle;

	// 次のシーンフェーズのリクエスト
	std::optional<ScenePhase> scenePhaseRequest_ = std::nullopt;


	// 制御点スクリプト
	std::string controlPointScript_;

	// 敵スクリプト
	std::string enemyScript_;

	// ステージオブジェクトスクリプト
	std::string stageObjectScript_;
};