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

	// BGMを読み込む
	soundHandle_ = engine_->LoadSound("./Resources/Sounds/Bgm/Hard_Advent.mp3");


	// ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	
	// メインカメラを中心軸に設定する
	mainCamera_->SetPivotParent(worldTransform_.get());
	mainCamera_->SetCameraRotate(Vector3(0.1f , 0.0f , 0.0f));


	// プレイヤー戦闘機の生成と初期化
	playerJet_ = std::make_unique<PlayerJet>();
	playerJet_->Initialize(engine_, camera3d_, modelHandleStore_);
	playerJet_->SetParent(worldTransform_.get());

	// 宇宙の生成と初期化
	universeSkydome_ = std::make_unique<UniverseSkydome>();
	universeSkydome_->Initialize(engine_, camera3d_, modelHandleStore_);
}

/// <summary>
/// 更新処理
/// </summary>
void StageSelectScene::Update()
{
	// 次のビヘイビアのリクエストがあるとき
	if (behaviorRequest_)
	{
		// ビヘイビアを切り替える
		behavior_ = behaviorRequest_.value();

		// ビヘイビア初期化
		switch (behavior_)
		{
		case kFadeIn:
			// フェードイン
			BehaviorFadeInInitialize();

			break;

		case kOperation:
			// 操作
			BehaviorOperationInitialize();

			break;

		case kFadeOut:
			// フェードアウト
			BehaviorFadeOutInitialize();

			break;
		}

		// ビヘイビアリクエストを初期化
		behaviorRequest_ = std::nullopt;
	}

	// ビヘイビア更新処理
	switch (behavior_)
	{
	case kFadeIn:
		// フェードイン
		BehaviorFadeInUpdate();

		break;

	case kOperation:
		// 操作
		BehaviorOperationUpdate();

		break;

	case kFadeOut:
		// フェードアウト
		BehaviorFadeOutUpdate();

		break;
	}


	// 宇宙の天球がワールドトランスフォームの場所に追従するようにする
	universeSkydome_->SetPosition(worldTransform_->translation_);

	// 基底クラスの更新処理
	BaseScene::Update();

	// ワールドトランスフォームの更新
	worldTransform_->UpdateWorldMatrix();

	// プレイヤー戦闘機の更新
	playerJet_->Update();

	// 宇宙の更新
	universeSkydome_->Update();
}

/// <summary>
/// 描画処理
/// </summary>
void StageSelectScene::Draw()
{
	// 制御点の描画
	engine_->DrwaCatmullRomSpline(controlPoints_, Vector4(1.0f, 0.0f, 0.0f, 1.0f), camera3d_);

	// プレイヤー戦闘機の描画
	playerJet_->Draw();

	// 宇宙の描画
	universeSkydome_->Draw();

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
	float easing = 1.0f -  std::powf(1.0f - t, 3.0f);

	worldTransform_->translation_ = Lerp(prevPosition, nextPosition, easing);
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
	// スペースキーでフェードアウトする
	if (engine_->GetKeyTrigger(DIK_SPACE))
	{
		// フェードアウト
		behaviorRequest_ = kFadeOut;

		// BGMを止める
		engine_->StopSound(playHandle_);

		return;
	}

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
	// Aボタンでフェードアウトする
	if (engine_->GetGamepadButtonTrigger(0,XINPUT_GAMEPAD_A))
	{
		// フェードアウト
		behaviorRequest_ = kFadeOut;

		// BGMを止める
		engine_->StopSound(playHandle_);

		return;
	}

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







/*----------------------------
	ビヘイビア : フェードイン
----------------------------*/

/// <summary>
/// ビヘイビア : フェードイン : 初期化
/// </summary>
void StageSelectScene::BehaviorFadeInInitialize()
{
	// フェードインパラメータ
	fadeInParameter_ = 0.0f;
}

/// <summary>
/// ビヘイビア : フェードイン : 更新処理
/// </summary>
void StageSelectScene::BehaviorFadeInUpdate()
{
	// パラメータを進める
	fadeInParameter_ += 1.0f / 60.0f;
	fadeInParameter_ = std::min(fadeInParameter_, kFadeInPrameterMax);


	// 最大値になったら操作に遷移する
	if (fadeInParameter_ >= kFadeInPrameterMax)
	{
		behaviorRequest_ = kOperation;
		return;
	}
}

/// <summary>
/// ビヘイビア : フェードイン : 描画処理
/// </summary>
void StageSelectScene::BehaviorFadeInDraw()
{

}






/*---------------------
	ビヘイビア : 操作
---------------------*/

/// <summary>
/// ビヘイビア : 操作 : 初期化
/// </summary>
void StageSelectScene::BehaviorOperationInitialize()
{

}

/// <summary>
/// ビヘイビア : 操作 : 更新処理
/// </summary>
void StageSelectScene::BehaviorOperationUpdate()
{
	// BGMをループさせる
	if (!engine_->IsSoundPlay(playHandle_) || playHandle_ == 0)
	{
		playHandle_ = engine_->PlaySoundData(soundHandle_, 0.3f);
	}

	// 選択
	Select();

	// 移動
	Move();
}

/// <summary>
/// ビヘイビア : 操作 : 描画処理
/// </summary>
void StageSelectScene::BehaviorOperationDraw()
{

}






/*-----------------------------
	ビヘイビア : フェードアウト
-----------------------------*/

/// <summary>
/// ビヘイビア : フェードアウト : 初期化
/// </summary>
void StageSelectScene::BehaviorFadeOutInitialize()
{
	// フェードアウトパラメータ
	fadeOutParameter_ = 0.0f;
}

/// <summary>
/// ビヘイビア : フェードアウト : 更新処理
/// </summary>
void StageSelectScene::BehaviorFadeOutUpdate()
{
	// パラメータを進める
	fadeOutParameter_ += 1.0f / 60.0f;
	fadeOutParameter_ = std::min(fadeOutParameter_, kFadeOutPrameterMax);

	// 最大値になったら終了する
	if (fadeOutParameter_ >= kFadeOutPrameterMax)
	{
		isFinished_ = true;
		return;
	}
}

/// <summary>
/// ビヘイビア : フェードアウト : 描画処理
/// </summary>
void StageSelectScene::BehaviorFadeOutDraw()
{

}