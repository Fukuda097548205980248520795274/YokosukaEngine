#pragma once
#define NOMINMAX
#include <optional>
#include "../BaseScene.h"

#include "../../FadeScreen/FadeScreen.h"

#include "PlayerJet/PlayerJet.h"
#include "UniverseSkydome/UniverseSkydome.h"
#include "StageBox/StageBox.h"

class StageSelectScene : public BaseScene
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	void Initialize(const YokosukaEngine* engine, const ModelHandleStore* modelHandleStore, const TextureHandleStore* textureHandleStore) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 敵のスクリプトのGetter
	/// </summary>
	/// <returns></returns>
	std::string GetEnemyScriptPass() override { return enemyScriptPass_[currentStage]; }

	/// <summary>
	/// ステージオブジェクトのスクリプトのGetter
	/// </summary>
	/// <returns></returns>
	std::string GetStageObjectScriptPass()override { return stageObjectScriptPass_[currentStage]; }

	/// <summary>
	/// 制御点のスクリプトのパスのGetter
	/// </summary>
	/// <returns></returns>
	std::string GetControlPointScriptPass()override { return controlPointScriptPass_[currentStage]; }

private:


	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 選択
	/// </summary>
	void Select();

	/// <summary>
	/// キーボードで選択
	/// </summary>
	void SelectKeyboard();

	/// <summary>
	/// ゲームパッドで選択
	/// </summary>
	void SelectGamepad();


	// ステージ
	enum Stage
	{
		kTutorial,
		kStage1,
		kStage2,
		kStage3,
		kNumStage
	};


	// 制御点
	Vector3 controlPoint_[kNumStage] =
	{
		{0.0f , 0.0f , 0.0f},
		{-10.0f , 10.0f , 500.0f},
		{5.0f , -5.0f , 1000.0f},
		{0.0f , 20.0f , 1500.0f},
	};

	// 制御点のスクリプトパス
	std::string controlPointScriptPass_[kNumStage] =
	{
		{"./Resources/Script/Tutorial/controlPoint.txt"},
		{"./Resources/Script/Stage1/controlPoint.txt"},
		{"./Resources/Script/Stage2/controlPoint.txt"},
		{"./Resources/Script/Stage3/controlPoint.txt"}
	};

	// 敵のスクリプトパス
	std::string enemyScriptPass_[kNumStage] =
	{
		{"./Resources/Script/Tutorial/enemy.txt"},
		{"./Resources/Script/Stage1/enemy.txt"},
		{"./Resources/Script/Stage2/enemy.txt"},
		{"./Resources/Script/Stage3/enemy.txt"}
	};

	// ステージオブジェクトののスクリプトパス
	std::string stageObjectScriptPass_[kNumStage] =
	{
		{"./Resources/Script/Tutorial/stageObject.txt"},
		{"./Resources/Script/Stage1/stageObject.txt"},
		{"./Resources/Script/Stage2/stageObject.txt"},
		{"./Resources/Script/Stage3/stageObject.txt"}
	};


	// 2Dカメラ
	std::unique_ptr<Camera2D> camera2d_ = nullptr;


	// bgm
	uint32_t soundHandle_ = 0;
	uint32_t playHandle_ = 0;

	// 進む音
	uint32_t shMove_ = 0;
	uint32_t phMove_ = 0;


	// 平行光源
	std::unique_ptr<DirectionalLight> directionalLight_ = nullptr;


	// 現在地
	Vector3 position_ = { 0.0f , 0.0f , 0.0f };

	// 制御点リスト
	std::vector<Vector3> controlPoints_;


	// 現在のステージ
	uint32_t currentStage = kTutorial;

	// 次のステージ
	uint32_t nextStage_ = kTutorial;

	// 直前のステージ
	uint32_t prevStage_ = kTutorial;


	// 移動フラグ
	bool isMove_ = false;

	// 移動時間
	const float kMoveTime = 1.0f;

	// 移動タイマー
	float moveTimer_ = 0.0f;


	// フェード
	std::unique_ptr<FadeScreen> fade_ = nullptr;

	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// プレイヤーの戦闘機
	std::unique_ptr<PlayerJet> playerJet_ = nullptr;

	// 宇宙
	std::unique_ptr<UniverseSkydome> universeSkydome_ = nullptr;

	// ステージボックス
	std::unique_ptr<StageBox> stageBox_ = nullptr;


	/*---------------
	    ビヘイビア
	---------------*/

	enum Behavior
	{
		kFadeIn,
		kOperation,
		kFadeOut
	};

	// 現在のビヘイビア
	Behavior behavior_ = kFadeIn;

	// 次のビヘイビアのリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;


	/*----------------------------
		ビヘイビア : フェードイン
	----------------------------*/

	/// <summary>
	/// ビヘイビア : フェードイン : 初期化
	/// </summary>
	void BehaviorFadeInInitialize();

	/// <summary>
	/// ビヘイビア : フェードイン : 更新処理
	/// </summary>
	void BehaviorFadeInUpdate();

	/// <summary>
	/// ビヘイビア : フェードイン : 描画処理
	/// </summary>
	void BehaviorFadeInDraw();

	// フェードインパラメータ
	float fadeInParameter_ = 0.0f;

	// フェードんインパラメータの最大値
	const float kFadeInPrameterMax = 1.0f;


	/*---------------------
		ビヘイビア : 操作
	---------------------*/

	/// <summary>
	/// ビヘイビア : 操作 : 初期化
	/// </summary>
	void BehaviorOperationInitialize();

	/// <summary>
	/// ビヘイビア : 操作 : 更新処理
	/// </summary>
	void BehaviorOperationUpdate();

	/// <summary>
	/// ビヘイビア : 操作 : 描画処理
	/// </summary>
	void BehaviorOperationDraw();


	/*-----------------------------
		ビヘイビア : フェードアウト
	-----------------------------*/

	/// <summary>
	/// ビヘイビア : フェードアウト : 初期化
	/// </summary>
	void BehaviorFadeOutInitialize();

	/// <summary>
	/// ビヘイビア : フェードアウト : 更新処理
	/// </summary>
	void BehaviorFadeOutUpdate();

	/// <summary>
	/// ビヘイビア : フェードアウト : 描画処理
	/// </summary>
	void BehaviorFadeOutDraw();

	// フェードアウトパラメータ
	float fadeOutParameter_ = 0.0f;

	// フェードアウトパラメータの最大値
	const float kFadeOutPrameterMax = 1.0f;
};

