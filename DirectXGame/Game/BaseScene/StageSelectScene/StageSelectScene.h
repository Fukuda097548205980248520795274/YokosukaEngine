#pragma once
#define NOMINMAX
#include "../BaseScene.h"

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
	const float kMoveTime = 0.5f;

	// 移動タイマー
	float moveTimer_ = 0.0f;
};

