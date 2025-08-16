#include "StageSelectScene.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void StageSelectScene::Initialize(const YokosukaEngine* engine, const ModelHandleStore* modelHandleStore, const TextureHandleStore* textureHandleStore)
{
	// 基底クラスの初期化
	BaseScene::Initialize(engine, modelHandleStore, textureHandleStore);

	// リストに追加する
	for (uint32_t i = 0; i < kNumStage; i++)
	{
		controlPoints_.push_back(controlPoint_[i]);
	}
}

/// <summary>
/// 更新処理
/// </summary>
void StageSelectScene::Update()
{
	// 選択
	Select();

	// 移動
	Move();

	// 基底クラスの更新処理
	BaseScene::Update();
}

/// <summary>
/// 描画処理
/// </summary>
void StageSelectScene::Draw()
{
	// 制御点の描画
	engine_->DrwaCatmullRomSpline(controlPoints_, Vector4(1.0f, 0.0f, 0.0f, 1.0f), camera3d_);

	// 基底クラス描画処理
	BaseScene::Draw();
}

/// <summary>
/// 移動
/// </summary>
void StageSelectScene::Move()
{

	// タイマーが終了していたら処理しない
	if (moveTimer_ >= kMoveTime)
	{
		isMove_ = false;
		return;
	}

	// 移動する
	isMove_ = true;

	// 位置を取得する
	Vector3 prevPosition = controlPoints_[prevStage_];
	Vector3 nextPosition = controlPoints_[nextStage_];

	// タイマーを進める
	moveTimer_ += 1.0f / 60.0f;
	moveTimer_ = std::min(moveTimer_, kMoveTime);

	// 補間を求める
	float t = moveTimer_ / kMoveTime;
	float easing = std::powf(t, 2.0f);

	position_ = Lerp(prevPosition, nextPosition, t);
}

/// <summary>
/// 選択
/// </summary>
void StageSelectScene::Select()
{
	// 移動中は操作できない
	if (isMove_)
		return;

	if (engine_->IsGamepadEnable(0))
	{
		SelectGamepad();
	}
	else
	{
		SelectKeyboard();
	}
}

/// <summary>
/// キーボードで選択
/// </summary>
void StageSelectScene::SelectKeyboard()
{
	// 上キーで奥のステージへ
	if (engine_->GetKeyTrigger(DIK_UP))
	{
		if (currentStage < kStage3)
		{
			currentStage++;
			nextStage_ = currentStage;
			prevStage_ = currentStage - 1;

			moveTimer_ = 0.0f;
		}
	}

	// 下キーで手前のステージへ
	if (engine_->GetKeyTrigger(DIK_DOWN))
	{
		if (currentStage > kTutorial)
		{
			currentStage--;
			nextStage_ = currentStage;
			prevStage_ = currentStage + 1;

			moveTimer_ = 0.0f;
		}
	}
}

/// <summary>
/// ゲームパッドで選択
/// </summary>
void StageSelectScene::SelectGamepad()
{
	// 左スティック上で奥のステージへ
	if (engine_->GetGamepadLeftStick(0).y >= 0.5f)
	{
		if (currentStage < kStage3)
		{
			currentStage++;
			nextStage_ = currentStage;
			prevStage_ = currentStage - 1;

			moveTimer_ = 0.0f;
		}
	}

	// 左スティック下で手前のステージへ
	if (engine_->GetGamepadLeftStick(0).y <= -0.5f)
	{
		if (currentStage > kTutorial)
		{
			currentStage--;
			nextStage_ = currentStage;
			prevStage_ = currentStage + 1;

			moveTimer_ = 0.0f;
		}
	}
}